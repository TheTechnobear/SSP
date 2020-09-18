#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


Msw8Editor::Msw8Editor (Msw8& p)
	: AudioProcessorEditor (&p), processor_ (p)
{
	// set this to true to see the parameter values update
	// in the vst plugin GUI (editor) when turning encoders/
	// pushing buttons

	processor_.addListener(this);

	setSize (1600, 480);

	startTimer(50);


	globalBtn_.init("G", Colours::red);
	networkBtn_.init("N", Colours::red);
	plugInBtn_.init("P", Colours::red);
	recBtn_.init("R", Colours::red);
	addAndMakeVisible(globalBtn_);
	addAndMakeVisible(networkBtn_);
	addAndMakeVisible(plugInBtn_);
	addAndMakeVisible(recBtn_);

	buttons_[B_USE_ACTIVE].init("Act");
	buttons_[B_USE_ACTIVE].active(processor_.data().useActive_);


	buttons_[B_SOFT].init("Soft");
	buttons_[B_SOFT].active(processor_.data().soft_);

	for (unsigned i = 0; i < B_MAX; i++) {
		addAndMakeVisible(buttons_[i]);
	}

	// parameters
	params_[P_IN_SEL].init("InS");
	params_[P_OUT_SEL].init("OutS");

	params_[P_IN_SEL].value(processor_.data().inSel_);
	params_[P_OUT_SEL].value(processor_.data().outSel_);

	altActive_ = 0;
	params_[P_IN_SEL].active(!altActive_);
	params_[P_OUT_SEL].active(!altActive_);


	for (unsigned i = 0; i < P_MAX; i++) {
		addAndMakeVisible(params_[i]);
	}
}

Msw8Editor::~Msw8Editor()
{
	processor_.removeListener(this);
	stopTimer();
}

void Msw8Editor::timerCallback()
{
	if (activeParamCount_ > 0 ) {
		activeParamCount_--;
		if (activeParamCount_ == 0) {
			activeParam_ = nullptr;
		}
	}
	repaint();
}


void Msw8Editor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	auto fx =
	[ = ] {
		this->parameterChanged(parameterIndex, newValue);
	};

	MessageManager::callAsync (fx);
}

void 	Msw8Editor::audioProcessorChanged (AudioProcessor *) {
	;
}

void Msw8Editor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return;
	if (index >= Percussa::sspLast) return;

	switch (index) {

	// encoders
	case Percussa::sspEnc1:
		if (altActive_) {
			;
		} else {
			float v = processor_.data().inSel_ + value / 20.0f;
			v = constrain(v, -1.0f, 1.0f);
			processor_.data().inSel_ = v;
			params_[P_IN_SEL].value(processor_.data().inSel_);

			if (value) activeParam_ = &params_[P_IN_SEL];
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeParamIdx_ = 0 + altActive_;
		}
		break;
	case Percussa::sspEnc2:
		if (altActive_) {
			;
		} else {
			float v = processor_.data().outSel_ + value / 20.0f;
			v = constrain(v, -1.0f, 1.0f);
			processor_.data().outSel_ = v;
			params_[P_OUT_SEL].value(processor_.data().outSel_);

			if (value) activeParam_ = &params_[P_OUT_SEL];
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeParamIdx_ = 2 + altActive_;
		}
		break;
	case Percussa::sspEnc3:
		if (altActive_) {
			;
		} else {
			;
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeParamIdx_ = 4 + altActive_;
		}
		break;
	case Percussa::sspEnc4:
		if (altActive_) {
			;
		} else {
			;
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeParamIdx_ = 6 + altActive_;
		}
		break;
	// encoder switches
	case Percussa::sspEncSw1:
		if (value < 0.5f) {
			if (altActive_) {
				;
			} else {
				processor_.data().inSel_ = 0.0f;
				params_[P_IN_SEL].value(processor_.data().inSel_);
			}
		}
		break;
	case Percussa::sspEncSw2:
		if (value < 0.5f) {
			if (altActive_) {
				;
			} else {
				processor_.data().outSel_ = 0.0f;
				params_[P_OUT_SEL].value(processor_.data().outSel_);
			}
		}
		break;
	case Percussa::sspEncSw3:
		if (value < 0.5f) {
			if (altActive_) {
				;
			} else {
				;
			}
		}
		break;
	case Percussa::sspEncSw4:
		if (value < 0.5f) {
			if (altActive_) {
				;
			} else {
				;
			}
		}
		break;

	// buttons
	case Percussa::sspSw1:
		if (paramState_[index] != value && value < 0.5f) {
			processor_.data().useActive_ = ! processor_.data().useActive_;
		}
		buttons_[B_USE_ACTIVE].active(processor_.data().useActive_);
		break;
	case Percussa::sspSw2:
		if (paramState_[index] != value && value < 0.5f) {
			processor_.data().soft_ = ! processor_.data().soft_;
		}
		buttons_[B_SOFT].active(processor_.data().soft_);
		break;
	case Percussa::sspSw3:
		break;
	case Percussa::sspSw4:
		break;
	case Percussa::sspSw5:
		break;
	case Percussa::sspSw6:
		break;
	case Percussa::sspSw7:
		break;
	case Percussa::sspSw8:
		break;
	case Percussa::sspSwLeft:
		break;
	case Percussa::sspSwRight:
		break;
	case Percussa::sspSwUp:
		break;
	case Percussa::sspSwDown:
		break;
	case Percussa::sspSwShiftL:
		break;
	case Percussa::sspSwShiftR:
		break;
	default:
		break;
	}
	paramState_[index] = value;
}


void Msw8Editor::drawMsw8(Graphics& g) {
	int x  = 1000;
	int y  = 30;
	int lx = 100;
	int fh = 18;
	int sz = fh;
	int sp = fh;

	unsigned insel, outsel;
	processor_.getLastSel(insel, outsel);

	g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
	g.setColour(Colours::red);
	g.drawText("Outputs", x + lx, y, (8 * sz) + (7 * sp), fh, Justification::centred);
	y += sp;
	g.drawText("Inputs", x, y, lx, fh, Justification::left);
	for (unsigned xi = 0; xi < 8; xi++) {
		if (processor_.isOutputEnabled(xi)) {
			g.setColour(Colours::red);
		} else {
			g.setColour(Colours::grey);
		}
		char chout[2]="A";
		chout[0]='A'+ xi;
		g.drawText(String(chout), x + lx + xi * (sz + sp), y, sp + sz, fh, Justification::left);
	}
	y += sp;

	for (unsigned yi = 0; yi < 8; yi++) {
		if (processor_.isInputEnabled(yi)) {
			g.setColour(Colours::red);
		} else {
			g.setColour(Colours::grey);
		}
		g.drawText(String(yi + 1), x, y, lx, fh, Justification::left);
		for (unsigned xi = 0; xi < 8; xi++) {
			g.setColour(Colours::white);

			if (xi == outsel && yi == insel) {
				g.fillRect(x + lx + xi * (sz + sp), y , sz, sz);
			} else {
				g.drawRect(x + lx + xi * (sz + sp), y , sz, sz, 1);
			}
		}
		y += sp + sz;
	}
}

void Msw8Editor::drawMenuBox(Graphics& g) {
	unsigned y = menuTopY - 1;
	unsigned x = 1530 - 1;
	unsigned butTopY = paramTopY;
	g.setColour(Colours::grey);
	g.drawVerticalLine(x, y, butTopY);
	g.drawVerticalLine(1600 - 1, y, butTopY);
	for (int i = 0; i < 5; i++) {
		g.drawHorizontalLine(y + i * 45, x, 1600 - 1);
	}

}

void Msw8Editor::drawParamBox(Graphics& g) {
	unsigned butTopY = paramTopY;
	unsigned butLeftX = 900 - 1;
	float x = butLeftX;
	float y = butTopY;
	g.setColour(Colours::grey);
	g.drawHorizontalLine(y, x, 1600 - 1);
	g.drawHorizontalLine(y + paramSpaceY, x, 1600 - 1);
	g.drawHorizontalLine(480 - 1, x, 1600 - 1);
	for (int i = 0; i < 8; i++ ) {
		g.drawVerticalLine(x + i * 100 , butTopY, 480 - 1);
	}
}


void Msw8Editor::drawEncoderValue(Graphics& g) {
	if (activeParam_ != nullptr) {
		unsigned butLeftX = 900 - 1;
		String val = String::formatted(activeParam_->fmt(), activeParam_->value());
		if (activeParam_->unit().length()) { val = val + " " + activeParam_->unit();}

		static constexpr unsigned pw = unsigned(900.0f / 8.0f);


		g.setColour(Colours::red);
		g.drawHorizontalLine(40, 0, butLeftX);
		g.drawVerticalLine(0, 40, paramTopY);
		g.drawVerticalLine(butLeftX, 40, paramTopY - 1);
		g.drawHorizontalLine(paramTopY - 1, 0,  activeParamIdx_ * pw);
		g.drawHorizontalLine(paramTopY - 1, (activeParamIdx_ + 1)* pw, butLeftX);

		g.drawVerticalLine(activeParamIdx_ * pw, paramTopY, 1600 - 1);
		g.drawVerticalLine((activeParamIdx_ + 1) * pw, paramTopY, 1600 - 1);
		// g.drawHorizontalLine(1600-1, activeParamIdx_ * pw, (activeParamIdx_+1)* pw);

		g.setColour(Colours::white);
		g.setFont(Font(Font::getDefaultMonospacedFontName(), 180, Font::plain));

		g.drawText(val, 0, 40, butLeftX, paramTopY - 40 - 40, Justification::centred);
	}
}

void Msw8Editor::paint(Graphics& g)
{
	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)

	g.fillAll (Colours::black);

	// title
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText("msw8 matrix switch 8", 20, 30 );
	g.setColour(Colours::grey);
	g.drawSingleLineText("version : " + String(JucePlugin_VersionString), 1400, 30);

	drawMenuBox(g);
	drawParamBox(g);

	drawEncoderValue(g);

	drawMsw8(g);
}


void Msw8Editor::setMenuBounds(SSPButton& btn, unsigned r) {
	const int w = 70;
	const int h = 45;
	unsigned x = 1530 + 1;
	unsigned y = menuTopY + (r * h);
	btn.setBounds(x, y, w, h);
}

void Msw8Editor::setParamBounds(SSPParam& par, unsigned enc, unsigned var)
{
	unsigned h = 2 * paramSpaceY;
	unsigned w = unsigned(900.0f / 8.0f);
	unsigned x = ((enc * 2) + var) * w;
	unsigned y = paramTopY + 5;
	par.setBounds(x, y, w, h);
}


void Msw8Editor::setButtonBounds(SSPButton& btn, unsigned r, unsigned c)
{
	const int w = 100;
	const int h = paramSpaceY;
	unsigned x = 900 + (c * w);
	unsigned y = paramTopY + (r * h);
	btn.setBounds(x, y, w, h);
}


void Msw8Editor::resized()
{
	setMenuBounds(globalBtn_, 0);
	setMenuBounds(networkBtn_, 1);
	setMenuBounds(plugInBtn_, 2);
	setMenuBounds(recBtn_, 3);

	setButtonBounds(buttons_[B_USE_ACTIVE], 0, 0);
	setButtonBounds(buttons_[B_SOFT], 0, 1);

	setParamBounds(params_[P_IN_SEL], 0, 0);
	setParamBounds(params_[P_OUT_SEL], 1, 0);
}
