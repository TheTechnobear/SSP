#include "Module.h"

#include <dlfcn.h>
#include <juce_core/juce_core.h>


#ifdef __APPLE__
const static int dlopenmode = RTLD_LOCAL | RTLD_NOW;
const char *pluginPath = "~/Library/Audio/Plug-Ins/VST3/";
const char *pluginSuffix = ".vst3/Contents/MacOS/";
#else
const static int dlopenmode = RTLD_LOCAL | RTLD_NOW | RTLD_DEEPBIND;
const char *pluginPath = "/media/BOOT/plugins/";

#endif

void Module::alloc(const std::string &pname, SSPExtendedApi::PluginInterface *p, SSPExtendedApi::PluginDescriptor *d,
                   void *h) {
    pluginName_ = pname;
    plugin_ = p;
    descriptor_ = d;
    dlHandle_ = h;
    requestedModule_ = pluginName_;
    if (plugin_ != nullptr) editor_ = (SSPExtendedApi::PluginEditorInterface *)plugin_->getEditor();
}


void Module::free() {
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


void Module::prepare(int sampleRate, int blockSize) {
    if (!plugin_) return;

    // prepare for play
    int inSz = descriptor_->inputChannelNames.size();
    int outSz = descriptor_->outputChannelNames.size();

    int nCh = (inSz > outSz ? inSz : outSz);
    if (blockSize == 0) blockSize = 128;

    if (sampleRate != 0) { plugin_->prepare(sampleRate, blockSize); }
}


void Module::process(juce::AudioSampleBuffer &buffer) {
    if (!plugin_) return;

    float *const *buffers = buffer.getArrayOfWritePointers();
    // juce has changed, to using a const pointer to float*
    // now inconsistent with ssp sdk, but will work fine
    plugin_->process((float **)buffers,buffer.getNumChannels(), buffer.getNumSamples());
}

std::string Module::getPluginFile(const std::string &mname) {
    std::string file;
#ifdef __APPLE__
    juce::File plugInDir(pluginPath);
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


bool Module::loadModule(std::string mn) {
    free();
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
                    // log(std::string("Loaded modulule : "+ mn));
                    auto pluginInterace = fnCreateInterface();
                    auto *plugin = (SSPExtendedApi::PluginInterface *)pluginInterace;
                    plugin->useCompactUI(true);
                    alloc(mn, plugin, desc, fHandle);
                    return true;
                }
                delete desc;
            }
        }
        dlclose(fHandle);
    }
    return false;
}

bool Module::checkPlugin(const std::string &mn) {
    bool supported = false;
    std::string f = getPluginFile(mn);
    // log(f);
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


void Module::scanPlugins(std::vector<std::string> &supportedModules) {
    supportedModules.clear();
    // build list of modules to consider
    std::vector<std::string> moduleList;
#ifdef __APPLE__
    for (const juce::DirectoryEntry &entry :
         juce::RangedDirectoryIterator(juce::File(pluginPath), false, "*.vst3", juce::File::findDirectories)) {
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
        if (checkPlugin(mname)) { supportedModules.push_back(mname); }
    }

    // log("plugin scan : COMPLETED");
    for (const auto &module : supportedModules) {
        // log(module);
    }
}