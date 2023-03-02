
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"

#include <dlfcn.h>


#ifdef __APPLE__
const static int  dlopenmode = RTLD_LOCAL | RTLD_NOW;
#else
const static int  dlopenmode = RTLD_LOCAL | RTLD_NOW | RTLD_DEEPBIND;
#endif

void PluginProcessor::Module::alloc(
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


void PluginProcessor::Module::free() {
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


PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();


    bool scan = true;
    if (scan) {
        scanPlugins();
    } else {
        // Logger::writeToLog("plugin scan : SKIPPED");
#ifndef __APPLE__
        std::string path = "/Users/kodiak/Library/Audio/Plug-Ins/VST3/";
        supportedModules.push_back(path + "clds.vst3/Contents/MacOS/clds");
        supportedModules.push_back(path + "plts.vst3/Contents/MacOS/plts");
#else
        std::string path = "/media/BOOT/plugins/";
        supportedModules.push_back(path + "clds.so");
        supportedModules.push_back(path + "plts.so");
#endif

    }

    loadPlugin(0, supportedModules[0]);
    loadPlugin(1, supportedModules[1]);
}

PluginProcessor::~PluginProcessor() {
    for (auto &module: modules_) {
        module.free();
    }
}


void PluginProcessor::loadPlugin(unsigned m, const std::string &f) {
    modules_[m].free();
    loadModule(f, modules_[m]);
}

bool PluginProcessor::loadModule(const std::string &f, PluginProcessor::Module &m) {
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

                    return true;
                }
                delete desc;
            }
        }
        dlclose(fHandle);
    }
    return false;
}

bool PluginProcessor::checkPlugin(const std::string &f) {
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


void PluginProcessor::scanPlugins() {
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
            fileList.push_back(entry.getFile().getFileName().toStdString());
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
    for (const auto &module: supportedModules) {
        // Logger::writeToLog(module);
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);
    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    String bname =
        String("M")
        + String(channelIndex / MAX_IN)
        + String("_P")
        + String(channelIndex % MAX_IN);
    return bname;
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    String bname =
        String("M")
        + String(channelIndex / MAX_OUT)
        + String("_P")
        + String(channelIndex % MAX_OUT);
    return bname;
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);


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

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();
    int modIdx = 0;

    // prepare input & process audio
    for (auto &m: modules_) {
        auto &plugin = m.plugin_;
        if (!plugin) continue;

        int chOffset = modIdx * MAX_IN;
        int nCh = m.descriptor_->inputChannelNames.size();
        for (unsigned i = 0; i < nCh && i < MAX_IN; i++) {
            m.audioSampleBuffer_.copyFrom(i, 0, buffer.getReadPointer(i + chOffset), n);
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
                buffer.copyFrom(i + chOffset, 0, m.audioSampleBuffer_.getReadPointer(i), n);
            } else {
                // zero output where not enough channels
                buffer.applyGain(i + chOffset, 0, n, 0.0f);
            }
        }
        modIdx++;
    }
}

void PluginProcessor::onInputChanged(unsigned i, bool b) {
    if(i >= MAX_IN ) return;

    int midx = i / MAX_IN;
    int ch = i % MAX_IN;
    auto plugin = modules_[midx].plugin_;
    if (!plugin) return;

    if (ch < modules_[midx].descriptor_->inputChannelNames.size()) {
        plugin->inputEnabled(ch, b);
    }
}

void PluginProcessor::onOutputChanged(unsigned i, bool b) {
    if(i >= MAX_OUT ) return;

    int midx = i / MAX_OUT;
    int ch = i % MAX_OUT;
    auto plugin = modules_[midx].plugin_;
    if (!plugin) return;

    if (ch < modules_[midx].descriptor_->outputChannelNames.size()) {
        plugin->outputEnabled(ch, b);
    }
}

void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    // store plugin information
    return;

    // TODO - incomplete

    void *data[MAX_MODULES] = {nullptr, nullptr};
    size_t dataSz[MAX_MODULES] = {0, 0};
    int destSize = 0;

    // TODO MORE
    // 1. need to store plugin name
    // 2. need to store data size for each plugin, as we need it when loading

    int i = 0;
    for (auto &m: modules_) {
        auto &plugin = m.plugin_;
        if (!plugin) continue;

        plugin->getState(&data[i], &dataSz[i]);
        destSize += dataSz[i];
        i++;
    }

    int offset = 0;
    destData.setSize(destSize);
    for (int i = 0; i < MAX_MODULES; i++) {
        if (data[i] == nullptr) continue;
        destData.copyFrom(data[i], offset, dataSz[i]);
        offset += dataSz[i];
    }
}


void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // restore plugin state
    // TODO : Incomplete

    return;

    MemoryBlock srcData(data, sizeInBytes);

    int nModules = 0;
    std::string moduleName[MAX_MODULES];
    size_t dataSz[MAX_MODULES] = {0, 0};
    // TODO
    // deserialise name & data sz
    // loadModule using module name
    // restore data using size

    int offset = 0;
    for (int i = 0; i < MAX_MODULES; i++) {
        auto fname = moduleName[i];
        loadPlugin(i, fname);

        auto &plugin = modules_[i].plugin_;
        if (!plugin) continue;

        MemoryBlock moduleData;
        moduleData.setSize(dataSz[i]);
        moduleData.copyFrom(srcData.getData(), offset, dataSz[i]);
        offset += dataSz[i];
        plugin->setState(moduleData.getData(), moduleData.getSize());
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this, MAX_MODULES));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


