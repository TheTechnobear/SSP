#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


RngsEditor::RngsEditor (Rngs& p)
	: AudioProcessorEditor (&p), processor_ (p)
{
	// set this to true to see the parameter values update
	// in the vst plugin GUI (editor) when turning encoders/
	// pushing buttons

	processor_.addListener(this);

	setSize (1600, 480);

	startTimer(50);


	globalBtn_.init("G");
	networkBtn_.init("N");
	plugInBtn_.init("P");
	recBtn_.init("R");
	addAndMakeVisible(globalBtn_);
	addAndMakeVisible(networkBtn_);
	addAndMakeVisible(plugInBtn_);
	addAndMakeVisible(recBtn_);

	buttons_[B_AUDIO].init("Audio");
	buttons_[B_STRUM].init("Strum");
	buttons_[B_VOCT].init("V/Oct");
	buttons_[B_UP].init("+EN");

	buttons_[B_HELP].init("?");
	buttons_[B_WRITE_PR].init("WriPr");
	buttons_[B_DOWN].init("-EN");


	// TODO : should  not be needed, keep for inital test
	buttons_[B_AUDIO].active(processor_.data().f_internal_exciter < 0.5);
	buttons_[B_STRUM].active(processor_.data().f_internal_strum < 0.5);
	buttons_[B_VOCT].active(processor_.data().f_internal_note < 0.5);
	for(unsigned i=0;i<B_MAX;i++) {
		addAndMakeVisible(buttons_[i]);
	}

	// parameters
	params_[P_PITCH].init("Pitch");
	params_[P_STRUCT].init("Struct");
	params_[P_BRIGHT].init("Bright");
	params_[P_DAMP].init("Damp");

	params_[P_POS].init("Pos");
	params_[P_POLY].init("Poly", "%1.0f");
	params_[P_MODEL].init("Model", "%1.0f");
	params_[P_IN_GAIN].init("InGain");

	params_[P_PITCH].value(processor_.data().f_pitch);
	params_[P_STRUCT].value(processor_.data().f_structure);
	params_[P_BRIGHT].value(processor_.data().f_brightness);
	params_[P_DAMP].value(processor_.data().f_damping);

	params_[P_POS].value(processor_.data().f_position);
	params_[P_POLY].value(processor_.data().f_polyphony);
	params_[P_MODEL].value(processor_.data().f_model);
	params_[P_IN_GAIN].value(processor_.data().f_in_gain);

	altActive_ = 0;
	params_[P_PITCH].active(!altActive_);
	params_[P_STRUCT].active(!altActive_);
	params_[P_BRIGHT].active(!altActive_);
	params_[P_DAMP].active(!altActive_);


	for(unsigned i=0;i<P_MAX;i++) {
		addAndMakeVisible(params_[i]);
	}
}

RngsEditor::~RngsEditor()
{
	processor_.removeListener(this);
	stopTimer();
}

void RngsEditor::timerCallback()
{
	if(activeParamCount_>0 ) {
		activeParamCount_--;
		if(activeParamCount_==0) {
			activeParam_=nullptr;
		}
	}
	repaint();
}


void RngsEditor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	auto fx =
	[ = ] {
		this->parameterChanged(parameterIndex, newValue);
	};

	MessageManager::callAsync (fx);
}

void 	RngsEditor::audioProcessorChanged (AudioProcessor *) {
	;
}

void RngsEditor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return;
	if (index >= Percussa::sspLast) return;


	// unused so far
	// std::atomic<float>  f_bypass;
	// std::atomic<float>  f_easter_egg;
	// std::atomic<float>  f_chord;
	// std::atomic<float>  f_fm;

	// use this if you need to do something special
	// to process parameter
	switch (index) {
	case Percussa::sspEnc1:
		if (altActive_) {
			float v = processor_.data().f_position + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_position = v;
			params_[P_POS].value(processor_.data().f_position);

			if(value) activeParam_=&params_[P_POS];
		} else {
			float v = processor_.data().f_pitch + value / 4.0f;
			v = constrain(v, 0.0f, 60.0f);
			processor_.data().f_pitch = v;
			params_[P_PITCH].value(processor_.data().f_pitch);

			if(value) activeParam_=&params_[P_PITCH];
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 0 + altActive_;
		}
		break;
	case Percussa::sspEnc2:
		if (altActive_) {
			float v = processor_.data().f_polyphony + value / 20.0f;
			v = constrain(v, 0.0f, 2.0f);
			processor_.data().f_polyphony = v;
			params_[P_POLY].value((int) processor_.data().f_polyphony);

			if(value) activeParam_=&params_[P_POLY];

		} else {
			float v = processor_.data().f_structure + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_structure = v;
			params_[P_STRUCT].value(processor_.data().f_structure);

			if(value) activeParam_=&params_[P_STRUCT];
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 2 + altActive_;
		}
		break;
	case Percussa::sspEnc3:
		if (altActive_) {
			float v = processor_.data().f_model + value / 10.0f;
			v = constrain(v, 0.0f, 5.0f);
			processor_.data().f_model = v;
			params_[P_MODEL].value((int) processor_.data().f_model);

			if(value) activeParam_=&params_[P_MODEL];
		} else {
			float v = processor_.data().f_brightness + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_brightness = v;
			params_[P_BRIGHT].value(processor_.data().f_brightness);

			if(value) activeParam_=&params_[P_BRIGHT];
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 4 + altActive_;
		}
		break;
	case Percussa::sspEnc4:
		if (altActive_) {
			float v = processor_.data().f_in_gain + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_in_gain = v;
			params_[P_IN_GAIN].value(processor_.data().f_in_gain);

			if(value) activeParam_=&params_[P_IN_GAIN];
		} else {
			float v = processor_.data().f_damping + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_damping = v;
			params_[P_DAMP].value(processor_.data().f_damping);

			if(value) activeParam_=&params_[P_DAMP];
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 6 + altActive_;
		}
		break;
	case Percussa::sspEncSw1:
		break;
	case Percussa::sspEncSw2:
		break;
	case Percussa::sspEncSw3:
		break;
	case Percussa::sspEncSw4:
		break;
	case Percussa::sspSw1:
		//TODO - should not need using connection state instead

		// if (paramState_[index] != value && !value) {
		// 	processor_.data().f_internal_exciter =
		// 	    ! processor_.data().f_internal_exciter;
		// 	buttons_[B_AUDIO].active(processor_.data().f_internal_exciter < 0.5);
		// }
		break;
	case Percussa::sspSw2:
		// if (paramState_[index] != value && !value) {
		// 	processor_.data().f_internal_strum =
		// 	    ! processor_.data().f_internal_strum ;
		// 	buttons_[B_STRUM].active(processor_.data().f_internal_strum < 0.5);
		// }
		break;
	case Percussa::sspSw3:
		// if (paramState_[index] != value && !value) {
		// 	processor_.data().f_internal_note =
		// 	    ! processor_.data().f_internal_note;
		// 	buttons_[B_VOCT].active(processor_.data().f_internal_note < 0.5);
		// }
		break;
	case Percussa::sspSw4:
		break;
	case Percussa::sspSw5:
		//TODO - should not need if params are named
		buttons_[B_HELP].active(value > 0.5);
		break;
	case Percussa::sspSw6:
		break;
	case Percussa::sspSw7:
		buttons_[B_WRITE_PR].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			processor_.write();
		}
		break;
	case Percussa::sspSw8:
		break;
	case Percussa::sspSwLeft:
		break;
	case Percussa::sspSwRight:
		break;
	case Percussa::sspSwUp:
		buttons_[B_UP].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			altActive_ = ! altActive_;

			params_[P_PITCH].active(!altActive_);
			params_[P_STRUCT].active(!altActive_);
			params_[P_BRIGHT].active(!altActive_);
			params_[P_DAMP].active(!altActive_);

			params_[P_POS].active(altActive_);
			params_[P_POLY].active(altActive_);
			params_[P_MODEL].active(altActive_);
			params_[P_IN_GAIN].active(altActive_);
		}
		break;
	case Percussa::sspSwDown:
		buttons_[B_DOWN].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			altActive_ = ! altActive_;
			params_[P_PITCH].active(!altActive_);
			params_[P_STRUCT].active(!altActive_);
			params_[P_BRIGHT].active(!altActive_);
			params_[P_DAMP].active(!altActive_);

			params_[P_POS].active(altActive_);
			params_[P_POLY].active(altActive_);
			params_[P_MODEL].active(altActive_);
			params_[P_IN_GAIN].active(altActive_);
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

void RngsEditor::drawHelp(Graphics& g) {

	//TODO - should not need if params are named
	unsigned x = 900;
	unsigned y = 40;
	unsigned space = 30;
	unsigned yText = y + space * 2;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 40, Font::plain));
	g.drawSingleLineText("Input/Output Help", x, y);

	y = yText;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 18, Font::plain));
	g.drawSingleLineText("VST IN[1] : Audio", x, y);	y += space;
	g.drawSingleLineText("VST IN[2] : Strum", x, y);	y += space;
	g.drawSingleLineText("VST IN[3] : V/Oct", x, y);	y += space;
	g.drawSingleLineText("VST IN[4] : FM", x, y);		y += space;
	g.drawSingleLineText("VST IN[5] : Structure", x, y);	y += space;
	g.drawSingleLineText("VST IN[6] : Brightness", x, y);	y += space;
	g.drawSingleLineText("VST IN[7] : Damping", x, y);	y += space;
	g.drawSingleLineText("VST IN[8] : Position", x, y);	y += space;

	x = x + 300;
	y = yText;
	g.drawSingleLineText("VST OUT[1] : Odd", x, y);		y += space;
	g.drawSingleLineText("VST OUT[2] : Even", x, y);	y += space;
}

void RngsEditor::drawRngs(Graphics& g) {
	unsigned x = 1100;
	unsigned y = 150;
	unsigned d = 100;
	unsigned sp = 75;

	g.setColour(Colours::white);
	g.drawEllipse(x, y, d, d, 1);
	g.setColour(Colours::green);
	g.drawEllipse(x + sp, y, d, d, 1);

	x = x - sp / 2;
	y = y + sp;
	g.setColour(Colours::yellow);
	g.drawEllipse(x, y, d, d, 1);
	g.setColour(Colours::blue);
	g.drawEllipse(x + sp, y, d, d, 1);
	g.setColour(Colours::red);
	g.drawEllipse(x + (2 * sp), y, d, d, 1);
}

void RngsEditor::drawMenuBox(Graphics& g) {
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

void RngsEditor::drawParamBox(Graphics& g) {
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


void RngsEditor::drawEncoderValue(Graphics& g) {
	if(activeParam_!=nullptr) {
		unsigned butLeftX = 900 - 1;
	    String val = String::formatted(activeParam_->fmt(), activeParam_->value());
        if(activeParam_->unit().length()) { val = val + " " +activeParam_->unit();}

		static constexpr unsigned pw = unsigned(900.0f / 8.0f);


		g.setColour(Colours::red);
		g.drawHorizontalLine(40, 0, butLeftX);
		g.drawVerticalLine(0, 40, paramTopY);
		g.drawVerticalLine(butLeftX, 40, paramTopY - 1);
		g.drawHorizontalLine(paramTopY - 1, 0,  activeParamIdx_ * pw);
		g.drawHorizontalLine(paramTopY - 1, (activeParamIdx_+1)* pw, butLeftX);

		g.drawVerticalLine(activeParamIdx_ * pw, paramTopY, 1600-1);
		g.drawVerticalLine((activeParamIdx_+1) * pw, paramTopY, 1600-1);
		// g.drawHorizontalLine(1600-1, activeParamIdx_ * pw, (activeParamIdx_+1)* pw);

		g.setColour(Colours::white);
		g.setFont(Font(Font::getDefaultMonospacedFontName(), 180, Font::plain));

		g.drawText(val, 0, 40, butLeftX, paramTopY - 40 - 40, Justification::centred);
	}
}

void RngsEditor::paint(Graphics& g)
{
	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)

	//TODO test only
	buttons_[B_AUDIO].active(processor_.data().f_internal_exciter < 0.5);
	buttons_[B_STRUM].active(processor_.data().f_internal_strum < 0.5);
	buttons_[B_VOCT].active(processor_.data().f_internal_note < 0.5);


	g.fillAll (Colours::black);

	// title
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText("rngs resonator", 20, 30 );
	g.setColour(Colours::grey);
    g.drawSingleLineText("preset : " + String(processor_.getCurrentProgram()),1400,30);

	drawMenuBox(g);
	drawParamBox(g);

	drawEncoderValue(g);	

	if (buttons_[B_HELP].active()) {
		drawHelp(g);
	} else {
		drawRngs(g);
	}
}


void RngsEditor::setMenuBounds(SSPButton& btn, unsigned r) {
	const int w = 70;
	const int h = 45;
	unsigned x = 1530 + 1;
	unsigned y = menuTopY + (r * h);
	btn.setBounds(x, y, w, h);
}

void RngsEditor::setParamBounds(SSPParam& par, unsigned enc, unsigned var)
{
	unsigned h = 2 * paramSpaceY;
	unsigned w = unsigned(900.0f / 8.0f);
	unsigned x = ((enc * 2) + var) * w;
	unsigned y = paramTopY + 5;
	par.setBounds(x, y, w, h);
}


void RngsEditor::setButtonBounds(SSPButton& btn, unsigned r, unsigned c)
{
	const int w = 100;
	const int h = paramSpaceY;
	unsigned x = 900 + (c * w);
	unsigned y = paramTopY + (r * h);
	btn.setBounds(x, y, w, h);
}


void RngsEditor::resized()
{
	setMenuBounds(globalBtn_, 0);
	setMenuBounds(networkBtn_, 1);
	setMenuBounds(plugInBtn_, 2);
	setMenuBounds(recBtn_, 3);

	setButtonBounds(buttons_[B_AUDIO], 0, 0);
	setButtonBounds(buttons_[B_STRUM], 0, 1);
	setButtonBounds(buttons_[B_VOCT], 0, 2);
	setButtonBounds(buttons_[B_UP], 0, 5);
	setButtonBounds(buttons_[B_HELP], 1, 0);
	setButtonBounds(buttons_[B_WRITE_PR], 1, 2);
	setButtonBounds(buttons_[B_DOWN], 1, 5);


	setParamBounds(params_[P_PITCH], 0, 0);
	setParamBounds(params_[P_STRUCT], 1, 0);
	setParamBounds(params_[P_BRIGHT], 2, 0);
	setParamBounds(params_[P_DAMP], 3, 0);

	setParamBounds(params_[P_POS], 0, 1);
	setParamBounds(params_[P_POLY], 1, 1);
	setParamBounds(params_[P_MODEL], 2, 1);
	setParamBounds(params_[P_IN_GAIN], 3, 1);
}
