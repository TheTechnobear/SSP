
#include "PluginProcessor.h"
#include "SSPApi.h"

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createDescriptor() {
    auto desc=SSP_createDescriptor();
    desc->colour = Colours::green.getARGB();
    return desc;
}

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginInterface *createInstance() {
#ifdef JUCE_DEBUG
    ScopedJuceInitialiser_GUI juceInitialiser_;
//     if (msgThreadManager == nullptr) msgThreadManager = new MsgThreadManager;
#endif
    return new SSP_PluginInterface(new PluginProcessor());
}
// Supports thetechnobear extensions
extern "C" __attribute__ ((visibility("default")))
bool apiExtensions() {
    return true;
}

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createExtendedDescriptor() {
    auto desc = new SSPExtendedApi::PluginDescriptor;
    SSP_defaultDescriptor(desc);
    desc->colour = Colours::green.getARGB();
    desc->supportCompactUI_ = true;
    return desc;
}
