#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


PltsEditor::PltsEditor (Plts& p)
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

	buttons_[B_UP].init("+EN", Colours::red);
	buttons_[B_DOWN].init("-EN", Colours::red);


	// TODO : should  not be needed, keep for inital test
	for (unsigned i = 0; i < B_MAX; i++) {
		addAndMakeVisible(buttons_[i]);
	}

	// parameters
	params_[P_PITCH].init("Pitch");
	params_[P_HARMONICS].init("Harm");
	params_[P_TIMBRE].init("Timbre");
	params_[P_MORPH].init("Morph");

	params_[P_MODEL].init("Model", "%1.0f");
	params_[P_LPG].init("Lpg");
	params_[P_VCA].init("Vca");

	params_[P_PITCH].value(processor_.data().pitch_);
	params_[P_HARMONICS].value(processor_.data().harmonics_);
	params_[P_TIMBRE].value(processor_.data().timbre_);
	params_[P_MORPH].value(processor_.data().morph_);

	params_[P_MODEL].value(processor_.data().model_);
	params_[P_LPG].value(processor_.data().lpg_colour_);
	params_[P_VCA].value(processor_.data().decay_);

	altActive_ = 0;
	params_[P_PITCH].active(!altActive_);
	params_[P_HARMONICS].active(!altActive_);
	params_[P_TIMBRE].active(!altActive_);
	params_[P_MORPH].active(!altActive_);

	params_[P_MODEL].active(altActive_);
	params_[P_LPG].active(altActive_);
	params_[P_VCA].active(altActive_);


	for (unsigned i = 0; i < P_MAX; i++) {
		addAndMakeVisible(params_[i]);
	}
}

PltsEditor::~PltsEditor()
{
	processor_.removeListener(this);
	stopTimer();
}

void PltsEditor::timerCallback()
{
	if (activeParamCount_ > 0 ) {
		activeParamCount_--;
		if (activeParamCount_ == 0) {
			activeParam_ = nullptr;
		}
	}
	repaint();
}


void PltsEditor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	auto fx =
	[ = ] {
		this->parameterChanged(parameterIndex, newValue);
	};

	MessageManager::callAsync (fx);
}

void 	PltsEditor::audioProcessorChanged (AudioProcessor *) {
	;
}

void PltsEditor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return;
	if (index >= Percussa::sspLast) return;


	switch (index) {

	// encoders
	case Percussa::sspEnc1:
		if (altActive_) {
			float v = processor_.data().model_ + value / 1.0f;
			v = constrain(v, 0.0f, 15.0f);
			processor_.data().model_ = v;
			params_[P_MODEL].value(processor_.data().model_);

			if (value) activeParam_ = &params_[P_MODEL];
		} else {
			float v = processor_.data().pitch_ + value / 20.0f;
			v = constrain(v, -30.0f, 30.0f); // todo check
			processor_.data().pitch_ = v;
			params_[P_PITCH].value(processor_.data().pitch_);

			if (value) activeParam_ = &params_[P_PITCH];
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeParamIdx_ = 0 + altActive_;
		}
		break;
	case Percussa::sspEnc2:
		if (altActive_) {
			float v = processor_.data().lpg_colour_ + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().lpg_colour_ = v;
			params_[P_LPG].value(processor_.data().lpg_colour_);

			if (value) activeParam_ = &params_[P_LPG];
		} else {
			float v = processor_.data().harmonics_ + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().harmonics_ = v;
			params_[P_HARMONICS].value(processor_.data().harmonics_);

			if (value) activeParam_ = &params_[P_HARMONICS];
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeParamIdx_ = 2 + altActive_;
		}
		break;
	case Percussa::sspEnc3:
		if (altActive_) {
			float v = processor_.data().decay_ + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().decay_ = v;
			params_[P_VCA].value(processor_.data().decay_);

			if (value) activeParam_ = &params_[P_VCA];
		} else {
			float v = processor_.data().timbre_ + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().timbre_ = v;
			params_[P_TIMBRE].value(processor_.data().timbre_);

			if (value) activeParam_ = &params_[P_TIMBRE];
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
			float v = processor_.data().morph_ + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().morph_ = v;
			params_[P_MORPH].value(processor_.data().morph_);

			if (value) activeParam_ = &params_[P_MORPH];
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeParamIdx_ = 6 + altActive_;
		}
		break;
	// encoder switches
	case Percussa::sspEncSw1:
		if (value > 0.5f) {
			if (altActive_) {
				processor_.data().model_ = 0.0f;
				params_[P_MODEL].value(processor_.data().model_);
			} else {
				processor_.data().pitch_ = 0.0f;
				params_[P_PITCH].value(processor_.data().pitch_);
			}
		}
		break;
	case Percussa::sspEncSw2:
		if (value > 0.5f) {
			if (altActive_) {
				processor_.data().lpg_colour_ = 0.50f;
				params_[P_LPG].value(processor_.data().lpg_colour_);
			} else {
				processor_.data().harmonics_ = 0.0f;
				params_[P_HARMONICS].value(processor_.data().harmonics_);
			}
		}
		break;
	case Percussa::sspEncSw3:
		if (value > 0.5f) {
			if (altActive_) {
				processor_.data().decay_ = 0.50f;
				params_[P_VCA].value(processor_.data().decay_);
			} else {
				processor_.data().timbre_ = 0.5f;
				params_[P_TIMBRE].value(processor_.data().timbre_);
			}
		}
		break;
	case Percussa::sspEncSw4:
		if (value > 0.5f) {
			if (altActive_) {
				;
			} else {
				processor_.data().morph_ = 0.5f;
				params_[P_MORPH].value(processor_.data().morph_);
			}
		}
		break;

	// buttons
	case Percussa::sspSw1:
		break;
	case Percussa::sspSw2:
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
		buttons_[B_UP].active(value > 0.5f);
		if (paramState_[index] != value && value < 0.5f) {
			altActive_ = ! altActive_;

			params_[P_PITCH].active(!altActive_);
			params_[P_HARMONICS].active(!altActive_);
			params_[P_TIMBRE].active(!altActive_);
			params_[P_MORPH].active(!altActive_);

			params_[P_MODEL].active(altActive_);
			params_[P_LPG].active(altActive_);
			params_[P_VCA].active(altActive_);
		}
		break;
	case Percussa::sspSwDown:
		buttons_[B_DOWN].active(value > 0.5f);
		if (paramState_[index] != value && value < 0.5f) {
			altActive_ = ! altActive_;

			params_[P_PITCH].active(!altActive_);
			params_[P_HARMONICS].active(!altActive_);
			params_[P_TIMBRE].active(!altActive_);
			params_[P_MORPH].active(!altActive_);

			params_[P_MODEL].active(altActive_);
			params_[P_LPG].active(altActive_);
			params_[P_VCA].active(altActive_);
		}
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


void PltsEditor::drawPlts(Graphics& g) {
	unsigned x = 1100;
	unsigned y = 200;
	unsigned d = 10;
	unsigned sp = 10;

	x=1100;
	g.setColour(Colours::green);
	for(unsigned i = 0;i < 8 ; i++ ) {
		g.fillEllipse(x, y, d, d);
		x+=sp+d;
	}
	x=1100;
	y+=sp+d;
	g.setColour(Colours::red);
	for(unsigned i = 0;i < 8 ; i++ ) {
		g.fillEllipse(x, y, d, d);
		x+=sp+d;
	}
}

void PltsEditor::drawMenuBox(Graphics & g) {
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

void PltsEditor::drawParamBox(Graphics & g) {
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


void PltsEditor::drawEncoderValue(Graphics & g) {
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

void PltsEditor::paint(Graphics & g)
{
	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)

	g.fillAll (Colours::black);

	// title
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText("plts macro oscillator", 20, 30 );
	g.setColour(Colours::grey);
	g.drawSingleLineText("version : " + String(JucePlugin_VersionString), 1400, 30);

	drawMenuBox(g);
	drawParamBox(g);

	drawEncoderValue(g);

	drawPlts(g);
}


void PltsEditor::setMenuBounds(SSPButton & btn, unsigned r) {
	const int w = 70;
	const int h = 45;
	unsigned x = 1530 + 1;
	unsigned y = menuTopY + (r * h);
	btn.setBounds(x, y, w, h);
}

void PltsEditor::setParamBounds(SSPParam & par, unsigned enc, unsigned var)
{
	unsigned h = 2 * paramSpaceY;
	unsigned w = unsigned(900.0f / 8.0f);
	unsigned x = ((enc * 2) + var) * w;
	unsigned y = paramTopY + 5;
	par.setBounds(x, y, w, h);
}


void PltsEditor::setButtonBounds(SSPButton & btn, unsigned r, unsigned c)
{
	const int w = 100;
	const int h = paramSpaceY;
	unsigned x = 900 + (c * w);
	unsigned y = paramTopY + (r * h);
	btn.setBounds(x, y, w, h);
}


void PltsEditor::resized()
{
	setMenuBounds(globalBtn_, 0);
	setMenuBounds(networkBtn_, 1);
	setMenuBounds(plugInBtn_, 2);
	setMenuBounds(recBtn_, 3);

	setButtonBounds(buttons_[B_UP], 0, 5);
	setButtonBounds(buttons_[B_DOWN], 1, 5);


	setParamBounds(params_[P_PITCH], 0, 0);
	setParamBounds(params_[P_HARMONICS], 1, 0);
	setParamBounds(params_[P_TIMBRE], 2, 0);
	setParamBounds(params_[P_MORPH], 3, 0);

	setParamBounds(params_[P_MODEL], 0, 1);
	setParamBounds(params_[P_LPG], 1, 1);
	setParamBounds(params_[P_VCA], 2, 1);
}
