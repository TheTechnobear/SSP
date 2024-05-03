
#include "PluginProcessor.h"
#include "SSPApi.h"

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createDescriptor() {
    auto desc = new Percussa::SSP::PluginDescriptor;
    SSP_defaultDescriptor(desc);
    desc->colour = Colours::aqua.getARGB();
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
    desc->colour = Colours::aqua.getARGB();
    desc->supportCompactUI_ = true;
    desc->categories_.push_back(CAT_OSCILLATOR);
    return desc;
}
