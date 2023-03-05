
#include "PluginHost.h"

#include <dlfcn.h>

#include <fstream>
#include <iostream>


void log(const std::string &m) {
#ifdef __APPLE__
    std::cerr << m << std::endl;
#else
    std::cerr << m << std::endl;
//    std::ofstream s( "/dev/kmsg" );
//    s << m << std::endl;
#endif
}

void logMessage(const String &m) {
    log(m.toStdString());
}

#ifdef __APPLE__
const static int dlopenmode = RTLD_LOCAL | RTLD_NOW;
#else
const static int  dlopenmode = RTLD_LOCAL | RTLD_NOW | RTLD_DEEPBIND;
#endif


static bool callbackDone = false;
void PluginHost::audioDeviceIOCallback(const float **inputChannelData,
                                       int numInputChannels,
                                       float **outputChannelData,
                                       int numOutputChannels,
                                       int numSamples) {
    if(!callbackDone) {
        logMessage("audio received first callback");
        callbackDone = true;
    }
    MAX_IN = numInputChannels;
    MAX_OUT = numOutputChannels;

    AudioBuffer<const float> input(inputChannelData,numInputChannels,numSamples);
    AudioBuffer< float> output(outputChannelData,numOutputChannels,numSamples);
//    AudioBuffer<float> output(numOutputChannels,numSamples);
    processBlock(input,output);
}

void PluginHost::audioDeviceAboutToStart(AudioIODevice *device) {
    logMessage("audio starting : " + device->getName());
    prepareToPlay(getSampleRate(),getBlockSize());
}

void PluginHost::audioDeviceStopped() {
    logMessage("audio stopped");
}

void PluginHost::audioDeviceError(const String &errorMessage) {
    logMessage("audio error : " + errorMessage);
}


void PluginHost::Module::alloc(
    const std::string &f,
    SSPExtendedApi::PluginInterface *p,
    SSPExtendedApi::PluginEditorInterface *e,
    SSPExtendedApi::PluginDescriptor *d,
    void *h) {
    pluginFile_ = f;
    plugin_ = p;
    editor_ = e;
    descriptor_ = d;
    dlHandle_ = h;
}


void PluginHost::Module::free() {
    if (plugin_) delete plugin_;
//    if(editor_) delete editor_; // plugin owns and deletes
    if (descriptor_) delete descriptor_;
    if (dlHandle_) dlclose(dlHandle_);
    plugin_ = nullptr;
    editor_ = nullptr;
    dlHandle_ = nullptr;
    descriptor_ = nullptr;
    pluginFile_ = "";
}


PluginHost::PluginHost(int argc, char *argv[]) {

    bool scan = false;
    if (argc > 1) {
        scan = !strcmp(argv[1], "scan");
    }

    deviceManager_.initialiseWithDefaultDevices(MAX_IN, MAX_OUT);
    dumpDeviceInfo();

    AudioDeviceManager::AudioDeviceSetup audioSetup;
    deviceManager_.getAudioDeviceSetup(audioSetup);
    audioSetup.sampleRate = sampleRate_;
    audioSetup.bufferSize = blockSize_;
    deviceManager_.setAudioDeviceSetup(audioSetup,true);

    dumpDeviceInfo();
    auto *device = deviceManager_.getCurrentAudioDevice();
    sampleRate_ = device->getCurrentSampleRate();
    blockSize_ = device->getCurrentBufferSizeSamples();
    inputBuffer_.setSize(MAX_IN, getBlockSize());
    outputBuffer_.setSize(MAX_OUT, getBlockSize());



    if (scan) {
        scanPlugins();
    } else {
        // Logger::writeToLog("plugin scan : SKIPPED");
#ifdef __APPLE__
        std::string path = "/Users/kodiak/Library/Audio/Plug-Ins/VST3/";
//        supportedModules.push_back(path + "clds.vst3/Contents/MacOS/clds");
        supportedModules.push_back(path + "plts.vst3/Contents/MacOS/plts");
#else
        std::string path = "/media/BOOT/plugins/";
//        supportedModules.push_back(path + "clds.so");
        supportedModules.push_back(path + "plts.so");
#endif

    }

    loadPlugin(0, supportedModules[0]);
//    loadPlugin(1, supportedModules[1]);

    deviceManager_.addAudioCallback(this);

}

PluginHost::~PluginHost() {
    for (auto &module: modules_) {
        module.free();
    }
    deviceManager_.removeAudioCallback(this);
}


void PluginHost::loadPlugin(unsigned m, const std::string &f) {
    modules_[m].free();
    loadModule(f, modules_[m]);
}

bool PluginHost::loadModule(const std::string &f, PluginHost::Module &m) {
    auto fHandle = dlopen(f.c_str(), dlopenmode);
//    auto fnVersion = (Percussa::SSP::VersionFun) dlsym(fHandle, Percussa::SSP::getApiVersionName);
//    auto fnCreateDescriptor = (Percussa::SSP::DescriptorFun) dlsym(fHandle, Percussa::SSP::createDescriptorName);
    if (fHandle) {
        auto fnApiExtension = (SSPExtendedApi::apiExtensionFun) dlsym(fHandle, SSPExtendedApi::apiExtensionsName);
        if (fnApiExtension && fnApiExtension()) {
            auto fnCreateInterface = (Percussa::SSP::InstantiateFun) dlsym(fHandle, Percussa::SSP::createInstanceName);
            auto fnCreateExDescriptor = (SSPExtendedApi::descriptorExFun) dlsym(fHandle, SSPExtendedApi::createExDescriptorName);

            if (fnCreateInterface && fnCreateExDescriptor) {
                auto desc = fnCreateExDescriptor();
                bool supported = desc->supportCompactUI_;
                if (supported) {
                    auto pluginInterace = fnCreateInterface();
                    auto *plugin = (SSPExtendedApi::PluginInterface *) pluginInterace;
                    plugin->useCompactUI(true);
                    auto *editor = (SSPExtendedApi::PluginEditorInterface *) plugin->getEditor();
                    m.alloc(f, plugin, editor, desc, fHandle);


                    // prepare for play
                    int inSz = m.descriptor_->inputChannelNames.size();
                    int outSz = m.descriptor_->inputChannelNames.size();

                    int nCh = (inSz > outSz ? inSz : outSz);
                    int SR = getSampleRate();
                    int nSamples = getBlockSize();
                    if (nSamples == 0) nSamples = 128;
                    m.audioSampleBuffer_.setSize(nCh, nSamples);
                    if (SR != 0) {
                        plugin->prepare(SR, m.audioSampleBuffer_.getNumSamples());
                    }
                    log("plugin loaded : " + f);
                    return true;
                }
                delete desc;
            }
        }
        dlclose(fHandle);
    }
    log("unable to load : " + f);

    return false;
}

bool PluginHost::checkPlugin(const std::string &f) {
    bool supported = false;
    auto fHandle = dlopen(f.c_str(), dlopenmode); // macOS does now have deepbind
    if (fHandle) {
        auto fnApiExtension = (SSPExtendedApi::apiExtensionFun) dlsym(fHandle, SSPExtendedApi::apiExtensionsName);
        if (fnApiExtension && fnApiExtension()) {
            auto fnCreateExDescriptor = (SSPExtendedApi::descriptorExFun) dlsym(fHandle, SSPExtendedApi::createExDescriptorName);
            if (fnCreateExDescriptor) {
                auto desc = fnCreateExDescriptor();
                supported = desc->supportCompactUI_;
                delete desc;
            }
        }
        dlclose(fHandle);
    }

    return supported;
}


void PluginHost::scanPlugins() {
    log("plugin scan : STARTED");
    // Logger::writeToLog("plugin scan : STARTED");

    // build list of files to consider
    std::vector<std::string> fileList;
#ifdef __APPLE__
    const char *pluginPath = "~/Library/Audio/Plug-Ins/VST3";
    for (const DirectoryEntry &entry: RangedDirectoryIterator(File(pluginPath),
                                                              false, "*.vst3",
                                                              File::findDirectories)) {
        if (!entry.isHidden()) {
            auto fname = entry.getFile().getFileNameWithoutExtension().toStdString();
            auto path = entry.getFile().getFullPathName().toStdString();
            auto binname = path + "/Contents/MacOS/" + fname;
            fileList.push_back(binname);
        }
    }
#else
    const char* pluginPath = "/media/BOOT/plugins";
    for (DirectoryEntry entry: RangedDirectoryIterator(File(pluginPath),
                                                       false, "*.so",
                                                       File::findFiles)) {
        if (!entry.isHidden()) {
            fileList.push_back(entry.getFile().getFullPathName().toStdString());
        }
    }
#endif
    // check for modules supporting compact ui
    for (const auto &fname: fileList) {
        if (checkPlugin(fname)) {
            supportedModules.push_back(fname);
        }
    }

    // Logger::writeToLog("plugin scan : COMPLETED");
    log("plugin scan : COMPLETED");
    for (const auto &module: supportedModules) {
        // Logger::writeToLog(module);
        log(module);
    }
}


void PluginHost::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // buffers are allocated in loadModule, but sampleRate or blocksize could change
    // so these may need updating
    for (auto &m: modules_) {
        auto &plugin = m.plugin_;
        if (!plugin) continue;

        m.audioSampleBuffer_.setSize(m.audioSampleBuffer_.getNumChannels(),
                                     samplesPerBlock);
        plugin->prepare(sampleRate, samplesPerBlock);
    }
}

void PluginHost::processBlock(AudioBuffer<const float> &in, AudioBuffer<float>& out) {
    size_t n = in.getNumSamples();
    int modIdx = 0;

    // prepare input & process audio
    for (auto &m: modules_) {
        auto &plugin = m.plugin_;
        if (!plugin) continue;

        int chOffset = modIdx * MAX_IN;
        int nCh = m.descriptor_->inputChannelNames.size();
        for (unsigned i = 0; i < nCh && i < MAX_IN; i++) {
            m.audioSampleBuffer_.copyFrom(i, 0, in.getReadPointer(i + chOffset), n);
        }
        float **buffers = m.audioSampleBuffer_.getArrayOfWritePointers();
        plugin->process(buffers, nCh, n);
        modIdx++;
        //note: we cannot copy output yet, since this will overwrite input for next module!
    }

    // now write the audio back out
    modIdx = 0;
    for (auto &m: modules_) {
        auto &plugin = m.plugin_;
        if (!plugin) continue;

        int chOffset = modIdx * MAX_OUT;
        int nCh = m.descriptor_->outputChannelNames.size();
        for (unsigned i = 0; i < MAX_OUT; i++) {
            if (i < nCh) {
                out.copyFrom(i + chOffset, 0, m.audioSampleBuffer_.getReadPointer(i), n);
            } else {
                // zero output where not enough channels
                out.applyGain(i + chOffset, 0, n, 0.0f);
            }
        }
        modIdx++;
    }
}



void PluginHost::dumpDeviceInfo() {
    logMessage("--------------------------------------");
    logMessage("Current audio device type: " + (deviceManager_.getCurrentDeviceTypeObject() != nullptr
                                                ? deviceManager_.getCurrentDeviceTypeObject()->getTypeName()
                                                : "<none>"));

    if (auto *device = deviceManager_.getCurrentAudioDevice()) {
        logMessage("Current audio device: " + device->getName().quoted());
        logMessage("Sample rate: " + juce::String(device->getCurrentSampleRate()) + " Hz");
        logMessage("Block size: " + juce::String(device->getCurrentBufferSizeSamples()) + " samples");
        logMessage("Bit depth: " + juce::String(device->getCurrentBitDepth()));
        logMessage("Input channel names: " + device->getInputChannelNames().joinIntoString(", "));
//        logMessage ("Active input channels: "  + getListOfActiveBits (device->getActiveInputChannels()));
        logMessage("Output channel names: " + device->getOutputChannelNames().joinIntoString(", "));
//        logMessage ("Active output channels: " + getListOfActiveBits (device->getActiveOutputChannels()));
    } else {
        logMessage("No audio device open");
    }
}

