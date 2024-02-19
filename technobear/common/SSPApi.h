
#include "../../ssp-sdk/Percussa.h"

// you will need to create a SSPApi.cpp in your project file,
// this will allows specifc to be overriden, though usually most of the default as ok

/* 
// example: SSPApi.cpp
// you'll want to override colour at least, you might want to overide name
// usually createInstance is fine (and uses implementation in this file), 
// but specialised cases can override 

------

#include "PluginProcessor.h"
#include "SSPApi.h"

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createDescriptor() {
    auto desc=SSP_createDescriptor();
    desc->colour = Colours::red.getARGB();
    return desc;
}

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginInterface *createInstance() {
    return new SSP_PluginInterface(new PluginProcessor());

}
*/


#include "SSPApiProcessor.h"

void SSP_defaultDescriptor(Percussa::SSP::PluginDescriptor *desc) {
    std::vector<std::string> inNames;
    std::vector<std::string> outNames;
    auto busProps = PluginProcessor::getBusesProperties();

    for (auto layout: busProps.inputLayouts) {
        inNames.push_back(layout.busName.toStdString());
    }
    for (auto layout: busProps.outputLayouts) {
        outNames.push_back(layout.busName.toStdString());
    }

    desc->name = JucePlugin_Name;
    desc->descriptiveName = JucePlugin_Desc;
    desc->manufacturerName = JucePlugin_Manufacturer;
    desc->version = JucePlugin_VersionString;
    desc->uid = (int) JucePlugin_VSTUniqueID;
    desc->inputChannelNames = inNames;
    desc->outputChannelNames = outNames;
    desc->colour = Colours::green.getARGB();
}


Percussa::SSP::PluginDescriptor *SSP_createDescriptor() {
    auto desc = new Percussa::SSP::PluginDescriptor;
    SSP_defaultDescriptor(desc);
    return desc;
}


extern "C" __attribute__ ((visibility("default")))
void getApiVersion(unsigned &major, unsigned &minor) {
    major = Percussa::SSP::API_MAJOR_VERSION;
    minor = Percussa::SSP::API_MINOR_VERSION;
}




# if 0
// now using ScopedJuceInitialiser_GUI juceInitialiser_;


// MessageManager for JUCE
// JUCE requires a message manager if we load via this api, otherwise you get assertions in debug
// in release you do not get this !
// this is a utility class for use by plugins that need it, so crated in their SSPApi.cpp
class MsgThead : public juce::Thread {
public:
    MsgThead();
    void run() override;
};
struct MsgThreadManager {
    MsgThreadManager();
    MsgThead msgThread_;
};
MsgThead::MsgThead() : Thread(String(JucePlugin_Name) + String(": MsgThread")) {}
void MsgThead::run() {
    if (MessageManager::getInstance()->isThisTheMessageThread()) {
        MessageManager::getInstance()->runDispatchLoop();
    }
}
MsgThreadManager::MsgThreadManager() {
    if (MessageManager::getInstanceWithoutCreating() == nullptr) {
//            std::cerr << "create Message manager in " << JucePlugin_Name << std::endl;
        MessageManager::getInstance();
        msgThread_.startThread();
    }
}
static MsgThreadManager* msgThreadManager = nullptr;

#endif