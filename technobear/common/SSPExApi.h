#pragma once

#include "../../ssp-sdk/Percussa.h"


namespace SSPExtendedApi {

class PluginInterface : public Percussa::SSP::PluginInterface {
public:
    ~PluginInterface() override = default;

    virtual void useCompactUI(bool b) = 0;
};

class PluginEditorInterface : public Percussa::SSP::PluginEditorInterface {
public:
    ~PluginEditorInterface() override = default;
};


struct PluginDescriptor : public Percussa::SSP::PluginDescriptor {
    bool supportCompactUI_ = false;
};

static const char *apiExtensionsName = "apiExtensions";
static const char *createExDescriptorName = "createExtendedDescriptor";

typedef bool (*apiExtensionFun)();
typedef PluginDescriptor *(*descriptorExFun)();

}