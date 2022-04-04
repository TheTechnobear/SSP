
#include "PluginProcessor.h"
#include "SSPApi.h"


extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createDescriptor() {
    auto desc=SSP_createDescriptor();
    desc->colour = Colours::aqua.getARGB();
    return desc;
}

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginInterface *createInstance() {
    return new SSP_PluginInterface(new PluginProcessor());

}

