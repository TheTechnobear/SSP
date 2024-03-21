#include "MixerView.h"


MixerView::MixerView(PluginProcessor &p) : ssp::BaseView(&p, false), processor_(p) {
}

MixerView::~MixerView() {
}