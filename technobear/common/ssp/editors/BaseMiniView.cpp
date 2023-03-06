#include "BaseMiniView.h"


namespace ssp {

BaseMiniView::BaseMiniView(BaseProcessor *p) : base_type(p,true) {

}


MiniParamView::MiniParamView(BaseProcessor *p) : base_type(p,true) {

}

void MiniParamView::drawView(Graphics &g) {

    g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + " : "
                         + String(JucePlugin_Desc)
                         + String(" @ thetechnobear"),
                         10, 30);

    g.setColour(Colours::grey);
    g.drawSingleLineText("v " + String(JucePlugin_VersionString), 1505, 30);
}


void MiniParamView::addParam(const std::shared_ptr<BaseParamControl> &p) {
    params_.push_back(p);
    addChildComponent(p.get());
    float pos = params_.size() - 1;

    p->setBounds(10, pos * 100 + 40, 600, 100);
    bool act = params_.size() <= 4;
    p->active(act);
    p->setVisible(act);
}

void MiniParamView::onEncoder(unsigned enc, float v) {
    base_type::onEncoder(enc, v);
    if (v > -0.01f && v < 0.01f) return;

    if (enc < params_.size()) {
        auto c = params_[enc];
        if (c != nullptr) {
            if (encoderState_[enc]) {
                encoderFine_[enc] = true;
            }
            if (v > 0.0f) {
                c->inc(encoderFine_[enc]);
            } else if (v < 0.0f) {
                c->dec(encoderFine_[enc]);
            }
        }
    }
}

void MiniParamView::onEncoderSwitch(unsigned enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
    encoderState_[enc] = v;
    if (v) return; // change on button up

    if (!encoderFine_[enc]) {
        if (enc < params_.size()) {
            auto c = params_[enc];
            if (c != nullptr) {
                if (v == 0) {
                    c->reset();
                }
            }
        }
    }
    encoderFine_[enc] = false;
}


} // namespace