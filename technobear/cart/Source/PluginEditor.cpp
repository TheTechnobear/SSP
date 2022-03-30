
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_LAYERS = PluginProcessor::MAX_LAYERS;
static constexpr unsigned MAX_STEPS = PluginProcessor::MAX_STEPS;
static constexpr unsigned MAX_VIEW = MAX_LAYERS * 2;
static const unsigned MAX_X = 4, MAX_Y=4;

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW),
      processor_(p),
      clrs_{Colours::red, Colours::green, Colours::orange},
      currentLayer_(0),
      cvButtonMode_(B_GATEACCESS), encoderMode_(E_CV) {

    unsigned encView = 0;

    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.params_.layers_[layer % MAX_LAYERS];
        Colour clr = clrs_[layer % MAX_LAYERS];
        for (unsigned i = 0; i <  MAX_STEPS / MAX_Y; i++) {
            int pi = i * MAX_X;
            addParamPage(
                std::make_shared<pcontrol_type>(l->steps_[pi + 0]->cv, 0.25),
                std::make_shared<pcontrol_type>(l->steps_[pi + 1]->cv, 0.25),
                std::make_shared<pcontrol_type>(l->steps_[pi + 2]->cv, 0.25),
                std::make_shared<pcontrol_type>(l->steps_[pi + 3]->cv, 0.25),
                encView,
                clr
            );

            addButtonPage(
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->access, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->access, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->access, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->access, 24, clr),
                encView
            );
        }
        for (unsigned i = 0; i <  MAX_STEPS / MAX_Y; i++) {
            int pi = i * MAX_X;
            addButtonPage(
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->glide, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->glide, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->glide, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->glide, 24, clr),
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                encView
            );
        }
        encView++;
    }


    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.params_.layers_[layer % MAX_LAYERS];
        Colour clr = clrs_[layer % MAX_LAYERS];
        addParamPage(
            std::make_shared<pcontrol_type>(l->snake, 1),
            std::make_shared<pcontrol_type>(l->scale, 1),
            std::make_shared<pcontrol_type>(l->root, 1),
            nullptr,
            encView,
            clr
        );
        addParamPage(
            std::make_shared<pcontrol_type>(l->fun_op_trig, 1),
            std::make_shared<pcontrol_type>(l->fun_op_sleep, 1),
            std::make_shared<pcontrol_type>(l->fun_mod_mode, 1),
            std::make_shared<pcontrol_type>(l->fun_cv_mode, 1),
            encView,
            clr
        );
        encView++;
    }

    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    int startX = 1000;
    int y = 30;
    int fh = 18;
    int lsz = 75;;

    int szx = fh * 4;
    int szy = szx;
    int sp = fh;

    int x = startX;

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.setColour(Colours::red);

    x += sp + (szx * 2.5f);
    g.drawText("Outputs", x, y, lsz, fh, Justification::left);

    x = startX;
    y += sp;


    // draw header
    g.drawText("Inputs", x, y, lsz, fh, Justification::left);
    x += lsz + sp;
    for (unsigned xi = 0; xi < MAX_X; xi++) {
        if (processor_.isOutputEnabled(xi * 2))  {
            g.setColour(Colours::red);
        } else {
            g.setColour(Colours::grey);
        }
        char chout[2] = "A";
        chout[0] = 'A' + xi;
        g.drawText(String(chout), x, y, szx, fh, Justification::centred);
        x += szx;
    }



    static float values[MAX_X][MAX_Y];
    static bool init= false;
    if(!init) {
        init=true;
        Random random;
        for (unsigned yi = 0; yi < MAX_Y; yi++) {
            for (unsigned xi = 0; xi < MAX_X; xi++) {
                values[xi][yi] = (random.nextFloat() * 2.0f) - 1.0f;
            }
        }
    }

    y += szy;
    // draw each input line
    for (unsigned yi = 0; yi < MAX_Y; yi++) {
        x = startX;

        if (processor_.isInputEnabled(yi * 2) ) {
            g.setColour(Colours::red);
        } else {
            g.setColour(Colours::grey);
        }
        g.drawText(String(yi + 1), x, (y - (fh / 2)), lsz, fh, Justification::left);
        x += lsz + sp;


        for (unsigned xi = 0; xi < MAX_X; xi++) {
//            float vca = processor_.getVCA(yi, xi) + processor_.getVCACV(yi, xi);
            float vca = values[xi][yi];
            vca = constrain(vca / 1.2f, -1.0f, 1.0f);

            if (vca == 0.0f) {
                unsigned d = 5;
                unsigned o = ((szy - d) / 2);
                unsigned cy = y - (szy / 2) + o;
                unsigned cx = x + o;
                g.setColour(Colours::white);
                g.fillEllipse(cx, cy, d, d);
                // g.drawEllipse(cx, cy, d, d, 1);
            } else {
                unsigned d = szy;
                if (vca > 0.0f) {
                    d = vca * szy;
                    g.setColour(Colours::green);

                } else {
                    d = vca * -szy;
                    g.setColour(Colours::red);
                }
                unsigned o = ((szy - d) / 2);
                unsigned cy = y - (szy / 2) + o;
                unsigned cx = x + o;
                g.fillEllipse(cx, cy, d, d);
            }
            x += szx;

            // String v = "-";
            // if (vca <= -0.01f || vca >= 0.01f) v = String::formatted("%4.2f", vca);
            // g.drawText(v, x + lx + xi * (szx + sp), y , sp + szx, fh, Justification::left);
        }
        y += szy;
    }
}

void PluginEditor::resized() {
}

void PluginEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.value(v);
    if (!v) {
        switch (encoderMode_) {
            case E_CV : {
                switch (cvButtonMode_) {
                    case B_GATEACCESS :
                        cvButtonMode_ = B_GLIDE;
                        setButtonPage(buttonPage_ + MAX_Y);
                        break;
                    case B_GLIDE :
                        cvButtonMode_ = B_GATEACCESS;
                        setButtonPage(buttonPage_ - MAX_Y);
                        break;
                    default:
                        break;
                } // but mode
                break;
            }
            default:
                break;
        } // enc mode
    }
}

void PluginEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.value(v);
    if (!v) {
        encoderMode_ = (EncoderMode) ((encoderMode_ + 1) % MAX_ENCODER_MODE);
        setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
    }
}


void PluginEditor::onUpButton(bool v) {
    upBtn_.value(v);
    if (!v) {
        chgParamPage(-1);
        unsigned nextP = buttonPage_;
        unsigned minP = encoderMode_ == E_CV && cvButtonMode_ == B_GLIDE ? MAX_Y : 0;
        if (nextP > minP) nextP--;
        if (nextP != buttonPage_) setButtonPage(nextP);
    }
}

void PluginEditor::onDownButton(bool v) {
    downBtn_.value(v);
    if (!v) {
        chgParamPage(1);
        unsigned nextP = buttonPage_;
        unsigned maxP = encoderMode_ == E_CV && cvButtonMode_ == B_GATEACCESS ?
                        MAX_Y : (views_[view_].buttonPages_.size());
        if (nextP < (maxP - 1)) nextP++;
        if (nextP != buttonPage_) setButtonPage(nextP);
    }
}

void PluginEditor::onLeftButton(bool v) {
    leftBtn_.value(v);
    if (!v) {
        if (currentLayer_ > 0) {
            currentLayer_--;
            setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
        }
    }
}

void PluginEditor::onRightButton(bool v) {
    rightBtn_.value(v);
    if (!v) {
        if (currentLayer_ < (MAX_LAYERS - 1)) {
            currentLayer_++;
            setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
        }
    }
}

//
//static constexpr unsigned MAX_TONICS = 12;
//
//static const char tonics[MAX_TONICS][3] = {
//    "C ",
//    "C#",
//    "D ",
//    "D#",
//    "E ",
//    "F ",
//    "F#",
//    "G ",
//    "G#",
//    "A ",
//    "A#",
//    "B ",
//};

//String getNoteValue(float f) {
//    float voct = cv2Pitch(f) + 60.0f; // -5v = 0
//    voct += 0.005f; // so we round up fractions of cent
//    int oct = voct / 12;
//    unsigned note = unsigned(voct) % MAX_TONICS;
//    int cents = ((voct - floorf(voct)) * 100.0f);
//    if (cents > 50) {
//        cents = 50 - cents;
//        note = (note + 1) % MAX_TONICS;
//        oct += (note == 0);
//    }
//    String cts = String::formatted("%+02d", cents);
//    return String(tonics[note] + String(oct - (note < 3))) + " " + cts;
//}
