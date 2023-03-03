#include "PluginMiniEditor.h"

#include "PluginProcessor.h"

namespace ssp {

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) :
    BaseMiniView(&p),
    processor_(p) {

}

void PluginMiniEditor::drawView(Graphics &) {
}

void PluginMiniEditor::resized() {
}


} //namespace