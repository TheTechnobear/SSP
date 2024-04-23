
#include "PluginProcessor.h"

#include <dlfcn.h>

#include <fstream>

#include "PluginEditor.h"
#include "ssp/EditorHost.h"

#include "ssp/Log.h"

#ifdef __APPLE__
const static int dlopenmode = RTLD_LOCAL | RTLD_NOW;
const char *pluginSuffix = ".vst3/Contents/MacOS/";

#ifdef FORCE_COMPACT_UI
const char *pluginPath = "plugins/";
#else  // use full UI
const char *pluginPath = "~/Library/Audio/Plug-Ins/VST3/";
#endif // FORCE_COMPACT_UI

#else
// linux
const static int dlopenmode = RTLD_LOCAL | RTLD_NOW | RTLD_DEEPBIND;
const char *pluginPath = "plugins/";
#endif

void PluginProcessor::Module::alloc(const std::string &n, SSPExtendedApi::PluginInterface *p,
                                    SSPExtendedApi::PluginDescriptor *d, void *h) {
    pluginName_ = n;
    plugin_ = p;
    descriptor_ = d;
    dlHandle_ = h;
    requestedModule_ = n;
}


void PluginProcessor::Module::free() {
    if (plugin_) delete plugin_;
    if (descriptor_) delete descriptor_;
    if (dlHandle_) dlclose(dlHandle_);
    plugin_ = nullptr;
    editor_ = nullptr;
    dlHandle_ = nullptr;
    descriptor_ = nullptr;
    pluginName_ = "";
    requestedModule_ = "";
}


PluginProcessor::PluginProcessor() : PluginProcessor(getBusesProperties(), createParameterLayout()) {
}

PluginProcessor::PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                                 AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();
}

PluginProcessor::~PluginProcessor() {
    for (auto &module : modules_) { module.free(); }
}


bool PluginProcessor::requestModuleChange(unsigned midx, const std::string &mn) {
    auto &m = modules_[midx];

    if (!m.lockModule_.test_and_set()) {
        loadModule(mn, m);
        m.lockModule_.clear();
        return true;
    }
    return false;
}

std::string PluginProcessor::getPluginFile(const std::string &mname) {
    std::string file;
#ifdef __APPLE__
    File plugInDir(pluginPath);
    if (plugInDir.exists()) {
        std::string absPath = plugInDir.getFullPathName().toStdString();
        file = absPath + std::string("/") + mname + pluginSuffix + mname;
    }

#else
    File plugInDir(pluginPath);
    if (plugInDir.exists()) {
        std::string absPath = plugInDir.getFullPathName().toStdString();
        file = absPath + std::string("/") + mname + ".so";
    }
#endif
    return file;
}


bool PluginProcessor::loadModule(std::string mn, PluginProcessor::Module &m) {
    m.free();
    if (mn == "") {
        /// just cleared this module
        return true;
    }

    std::string f = getPluginFile(mn);
    auto fHandle = dlopen(f.c_str(), dlopenmode);
    //    auto fnVersion = (Percussa::SSP::VersionFun) dlsym(fHandle, Percussa::SSP::getApiVersionName);
    //    auto fnCreateDescriptor = (Percussa::SSP::DescriptorFun) dlsym(fHandle, Percussa::SSP::createDescriptorName);
    if (fHandle) {
        auto fnApiExtension = (SSPExtendedApi::apiExtensionFun)dlsym(fHandle, SSPExtendedApi::apiExtensionsName);
        if (fnApiExtension && fnApiExtension()) {
            auto fnCreateInterface = (Percussa::SSP::InstantiateFun)dlsym(fHandle, Percussa::SSP::createInstanceName);
            auto fnCreateExDescriptor =
                (SSPExtendedApi::descriptorExFun)dlsym(fHandle, SSPExtendedApi::createExDescriptorName);

            if (fnCreateInterface && fnCreateExDescriptor) {
                auto desc = fnCreateExDescriptor();
                bool supported = desc->supportCompactUI_;
                if (supported) {
                    ssp::log(std::string("Loaded modulule : " + mn));
                    auto pluginInterace = fnCreateInterface();
                    auto *plugin = (SSPExtendedApi::PluginInterface *)pluginInterace;
                    plugin->useCompactUI(true);
                    m.alloc(mn, plugin, desc, fHandle);

                    // prepare for play
                    int inSz = m.descriptor_->inputChannelNames.size();
                    int outSz = m.descriptor_->outputChannelNames.size();

                    int nCh = (inSz > outSz ? inSz : outSz);
                    int SR = getSampleRate();
                    int nSamples = getBlockSize();
                    if (nSamples == 0) nSamples = 128;
                    m.audioSampleBuffer_.setSize(nCh, nSamples);
                    if (SR != 0) { plugin->prepare(SR, m.audioSampleBuffer_.getNumSamples()); }

                    return true;
                }
                delete desc;
            }
        }
        dlclose(fHandle);
    }
    return false;
}

bool PluginProcessor::checkPlugin(const std::string &mn) {
    bool supported = false;
    std::string f = getPluginFile(mn);
    ssp::log(f);
    auto fHandle = dlopen(f.c_str(), dlopenmode);  // macOS does now have deepbind
    if (fHandle) {
        auto fnApiExtension = (SSPExtendedApi::apiExtensionFun)dlsym(fHandle, SSPExtendedApi::apiExtensionsName);
        if (fnApiExtension && fnApiExtension()) {
            auto fnCreateExDescriptor =
                (SSPExtendedApi::descriptorExFun)dlsym(fHandle, SSPExtendedApi::createExDescriptorName);
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
    ssp::log("plugin scan : STARTED");
    supportedModules_.clear();
    // Logger::writeToLog("plugin scan : STARTED");

    // build list of modules to consider
    std::vector<std::string> moduleList;
#ifdef __APPLE__
    for (const DirectoryEntry &entry :
         RangedDirectoryIterator(File(pluginPath), false, "*.vst3", File::findDirectories)) {
        if (!entry.isHidden()) {
            auto mname = entry.getFile().getFileNameWithoutExtension().toStdString();
            moduleList.push_back(mname);
        }
    }
#else
    for (DirectoryEntry entry : RangedDirectoryIterator(File(pluginPath), false, "*.so", File::findFiles)) {
        // if (!entry.isHidden()) { fileList.push_back(entry.getFile().getFullPathName().toStdString()); }
        if (!entry.isHidden()) { moduleList.push_back(entry.getFile().getFileNameWithoutExtension().toStdString()); }
    }
#endif
    // check for modules supporting compact ui
    for (const auto &mname : moduleList) {
        if (checkPlugin(mname)) { supportedModules_.push_back(mname); }
    }

    // Logger::writeToLog("plugin scan : COMPLETED");
    ssp::log("plugin scan : COMPLETED");
    for (const auto &module : supportedModules_) {
        // Logger::writeToLog(module);
        ssp::log(module);
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);
    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    String bname =
        String("M") + String((channelIndex / MAX_IN) + 1) + String(" In ") + String((channelIndex % MAX_IN) + 1);
    return bname;
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    String bname =
        String("M") + String((channelIndex / MAX_OUT) + 1) + String(" Out ") + String((channelIndex % MAX_OUT) + 1);
    return bname;
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);


    // buffers are allocated in loadModule, but sampleRate or blocksize could change
    // so these may need updating
    for (auto &m : modules_) {
        auto &plugin = m.plugin_;
        if (!plugin) continue;

        m.audioSampleBuffer_.setSize(m.audioSampleBuffer_.getNumChannels(), samplesPerBlock);
        plugin->prepare(sampleRate, samplesPerBlock);
    }
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();
    int modIdx = 0;
    bool processed[M_MAX] = { false, false };
    // prepare input & process audio
    for (auto &m : modules_) {
        processed[modIdx] = false;
        if (!m.lockModule_.test_and_set()) {
            processed[modIdx] = true;
            auto &plugin = m.plugin_;
            if (plugin) {
                int chOffset = modIdx * MAX_IN;
                int nCh = m.descriptor_->inputChannelNames.size();
                for (unsigned i = 0; i < nCh && i < MAX_IN; i++) {
                    m.audioSampleBuffer_.copyFrom(i, 0, buffer.getReadPointer(i + chOffset), n);
                }

                float *const *buffers = m.audioSampleBuffer_.getArrayOfWritePointers();
                // juce has changed, to using a const pointer to float*
                // now inconsistent with ssp sdk, but will work fine
                plugin->process((float **)buffers, nCh, n);
            }
        }
        modIdx++;
        // note: we cannot copy output yet, since this will overwrite input for next module!
    }

    // now write the audio back out
    modIdx = 0;
    for (auto &m : modules_) {
        if (processed[modIdx]) {
            auto &plugin = m.plugin_;
            if (plugin) {
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
            }
            m.lockModule_.clear();
        }
        modIdx++;
    }
}

void PluginProcessor::onInputChanged(unsigned i, bool b) {
    BaseProcessor::onInputChanged(i, b);
    if (i >= MAX_IN) return;

    int midx = i / MAX_IN;
    int ch = i % MAX_IN;
    auto plugin = modules_[midx].plugin_;
    if (!plugin) return;

    if (ch < modules_[midx].descriptor_->inputChannelNames.size()) { plugin->inputEnabled(ch, b); }
}

void PluginProcessor::onOutputChanged(unsigned i, bool b) {
    BaseProcessor::onOutputChanged(i, b);
    if (i >= MAX_OUT) return;

    int midx = i / MAX_OUT;
    int ch = i % MAX_OUT;
    auto plugin = modules_[midx].plugin_;
    if (!plugin) return;

    if (ch < modules_[midx].descriptor_->outputChannelNames.size()) { plugin->outputEnabled(ch, b); }
}

static constexpr int checkBytes = 0x1FF1;
static constexpr int protoVersion = 0x0001;

void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    // store plugin information
    MemoryOutputStream outStream(destData, false);

    outStream.writeInt(checkBytes);
    outStream.writeInt(protoVersion);

    outStream.writeInt(supportedModules_.size());
    for (auto mn : supportedModules_) { outStream.writeString(String(mn)); }

    int i = 0;
    for (auto &m : modules_) {
        outStream.writeInt(checkBytes);
        auto &plugin = m.plugin_;
        if (!plugin) {
            outStream.writeString("");
            outStream.writeInt(0);
            continue;
        }

        void *data;
        size_t dataSz;
        plugin->getState(&data, &dataSz);

        outStream.writeString(String(m.pluginName_.c_str()));
        outStream.writeInt(dataSz);
        outStream.write(data, dataSz);
        i++;
    }
}


void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    MemoryBlock srcData(data, sizeInBytes);
    MemoryInputStream inputStream(data, sizeInBytes, false);

    int check = inputStream.readInt();
    if (check != checkBytes) return;
    int proto = inputStream.readInt();
    if (proto != protoVersion) return;

    supportedModules_.clear();
    int nModules = inputStream.readInt();
    if (nModules == 0) {
        scanPlugins();
    } else {
        while (nModules--) {
            String m = inputStream.readString();
            supportedModules_.push_back(m.toStdString());
        }
    }

    for (int i = 0; i < M_MAX; i++) {
        int check = inputStream.readInt();
        if (check != checkBytes) { return; }

        String fname = inputStream.readString();
        int size = inputStream.readInt();

        if (!fname.isEmpty() && size > 0) {
            MemoryBlock moduleData;
            moduleData.setSize(size);
            inputStream.read(moduleData.getData(), size);

            while (!requestModuleChange(i, fname.toStdString())) {}
            auto &plugin = modules_[i].plugin_;
            if (!plugin) continue;

            plugin->setState(moduleData.getData(), moduleData.getSize());
        }
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    static constexpr bool useSysEditor = false, defaultDraw = false;
    return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI(), useSysEditor, defaultDraw);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
