#pragma once

#include "../../ssp-sdk/Percussa.h"

#include <juce_gui_basics/juce_gui_basics.h>


namespace SSPExtendedApi {

class PluginInterface : public Percussa::SSP::PluginInterface {
public:
    ~PluginInterface() override = default;

    virtual void useCompactUI(bool b) = 0;
    virtual unsigned numberOfParameters() = 0;
    virtual std::string parameterName(unsigned idx) = 0;
    virtual float parameterValue(unsigned idx) = 0; 
    virtual void parameterValue(unsigned idx, float v) = 0; 
};

class PluginEditorInterface : public Percussa::SSP::PluginEditorInterface {
public:
    ~PluginEditorInterface() override = default;
    virtual juce::Component* editorComponent() { return nullptr;}
};


struct PluginDescriptor : public Percussa::SSP::PluginDescriptor {
    bool supportCompactUI_ = false;
    std::vector<std::string> categories_;
};

static const char *apiExtensionsName = "apiExtensions";
static const char *createExDescriptorName = "createExtendedDescriptor";

typedef bool (*apiExtensionFun)();
typedef PluginDescriptor *(*descriptorExFun)();

}