#include "Module.h"

#include <dlfcn.h>
#include <juce_core/juce_core.h>

#include <algorithm>

#include "ssp/Log.h"


#ifdef __APPLE__
const static int dlopenmode = RTLD_LOCAL | RTLD_NOW;
const char *pluginSuffix = ".vst3/Contents/MacOS/";

#ifdef FORCE_COMPACT_UI
const char *pluginPath = "plugins/";
#else   // use full UI
const char *pluginPath = "~/Library/Audio/Plug-Ins/VST3/";
#endif  // FORCE_COMPACT_UI

#else
// linux
const static int dlopenmode = RTLD_LOCAL | RTLD_NOW | RTLD_DEEPBIND;
const char *pluginPath = "plugins/";
#endif

Module::Module() {
    // default to 16 channel, with 128 samples
    // descriptor and host (via prepare) will change this
    audioBuffer_.setSize(16, 128);
}

void Module::alloc(const std::string &pname, SSPExtendedApi::PluginInterface *p, SSPExtendedApi::PluginDescriptor *d,
                   void *h) {
    pluginName_ = pname;
    plugin_ = p;
    descriptor_ = d;
    dlHandle_ = h;
    requestedModule_ = pluginName_;

    editor_ = nullptr; // load when needed!

    if (descriptor_) {
        int inSz = descriptor_->inputChannelNames.size();
        int outSz = descriptor_->outputChannelNames.size();
        int nCh = std::max(inSz, outSz);
        if (nCh != audioBuffer_.getNumChannels()) { audioBuffer_.setSize(nCh, audioBuffer_.getNumSamples()); }
    }
}


void Module::free() {
    if (dlHandle_) {
        // do not delete for internal plugins
        if (plugin_) delete plugin_;
        dlclose(dlHandle_);
    }
    if (descriptor_) { delete descriptor_; }

    plugin_ = nullptr;
    editor_ = nullptr;
    dlHandle_ = nullptr;
    descriptor_ = nullptr;
    pluginName_ = "";
    requestedModule_ = "";
}


void Module::prepare(int sampleRate, int blockSize) {
    if (!plugin_) return;

    if (descriptor_) {
        int inSz = descriptor_->inputChannelNames.size();
        int outSz = descriptor_->outputChannelNames.size();
        int nCh = std::max(inSz, outSz);

        if (nCh != audioBuffer_.getNumChannels() || blockSize != audioBuffer_.getNumSamples()) {
            audioBuffer_.setSize(nCh, blockSize);
        }

    } else {
        return;
    }

    if (sampleRate != 0) { plugin_->prepare(sampleRate, blockSize); }
}


void Module::process(juce::AudioSampleBuffer &buffer) {
    if (!plugin_) return;

    float *const *buffers = buffer.getArrayOfWritePointers();
    // juce has changed, to using a const pointer to float*
    // now inconsistent with ssp sdk, but will work fine
    plugin_->process((float **)buffers, buffer.getNumChannels(), buffer.getNumSamples());
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
    juce::File plugInDir(pluginPath);
    if (!plugInDir.exists()) { plugInDir = juce::File("/media/BOOT/plugins/"); }

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

bool Module::checkPlugin(const std::string &mn, ModuleDesc &md) {
    bool supported = false;
    std::string f = getPluginFile(mn);
    // ssp::log(f);
    auto fHandle = dlopen(f.c_str(), dlopenmode);  // macOS does now have deepbind
    if (fHandle) {
        auto fnApiExtension = (SSPExtendedApi::apiExtensionFun)dlsym(fHandle, SSPExtendedApi::apiExtensionsName);
        if (fnApiExtension && fnApiExtension()) {
            auto fnCreateExDescriptor =
                (SSPExtendedApi::descriptorExFun)dlsym(fHandle, SSPExtendedApi::createExDescriptorName);
            if (fnCreateExDescriptor) {
                auto desc = fnCreateExDescriptor();
                supported = desc->supportCompactUI_;
                md.name = desc->name;
                md.description = desc->descriptiveName;
                md.categories = desc->categories_;
                delete desc;
            }
        }
        dlclose(fHandle);
    }

    return supported;
}


void Module::scanPlugins(std::vector<ModuleDesc> &supportedModules) {
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
    // log(std::string("Checking plugin dir : ") + pluginPath);
    juce::File plugInDir(pluginPath);
    if (!plugInDir.exists()) { plugInDir = juce::File("/media/BOOT/plugins/"); }

    for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator(plugInDir, false, "*.so", juce::File::findFiles)) {
        if (!entry.isHidden()) { moduleList.push_back(entry.getFile().getFileNameWithoutExtension().toStdString()); }
    }
#endif
    // check for modules supporting compact ui
    for (const auto &mname : moduleList) {
        // log(std::string("Checking plugin : " + mname));
        ModuleDesc md;
        if (mname == JucePlugin_Name) continue;
        if (checkPlugin(mname, md)) { supportedModules.push_back(md); }
    }

    // log("plugin scan : COMPLETED");
    for (const auto &module : supportedModules) {
        // log(std::string("supported plugin : " + module));
    }
    saveSupportedModules(supportedModules);
}

static const char *MODLIST_XML_TAG = "ModuleList";


bool Module::loadSupportedModules(std::vector<ModuleDesc> &supportedModules) {
    auto xmlModList = juce::XmlDocument::parse(juce::File(juce::String("./")+JucePlugin_Name+"_modules.xml"));
    if (xmlModList != nullptr && xmlModList->hasTagName(MODLIST_XML_TAG)) {
        supportedModules.clear();
        for (auto xmlMod : xmlModList->getChildIterator()) {
            ModuleDesc md;
            md.name = xmlMod->getStringAttribute("name").toStdString();
            md.description = xmlMod->getStringAttribute("desc").toStdString();
            auto catstr = xmlMod->getStringAttribute("cat").toStdString();
            std::istringstream iss(catstr);
            std::string cat;
            while (std::getline(iss, cat, ',')) { md.categories.push_back(cat); }
            supportedModules.push_back(md);
            // ssp::log("setStateInformation : supported module : " + mn);
        }
        return true;
    } else {
        ssp::log("loadSupportedModules : no MODLIST_XML_TAG tag");
    }
    return false;
}

bool Module::saveSupportedModules(std::vector<ModuleDesc> &supportedModules) {
    std::unique_ptr<juce::XmlElement> xmlModList = std::make_unique<juce::XmlElement>(MODLIST_XML_TAG);
    for (auto &md : supportedModules) {
        std::unique_ptr<juce::XmlElement> xmlMod = std::make_unique<juce::XmlElement>("Module");
        xmlMod->setAttribute("name", md.name);
        xmlMod->setAttribute("desc", md.description);
        std::string catstr;
        for (auto &cat : md.categories) { catstr += cat + ","; }
        xmlMod->setAttribute("cat", catstr);
        xmlModList->addChildElement(xmlMod.release());
    }
    juce::File(juce::String("./")+JucePlugin_Name+"_modules.xml").replaceWithText(xmlModList->toString());
    return true;
}