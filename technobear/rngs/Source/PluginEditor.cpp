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

	audioBtn_.init("Audio");
	strumBtn_.init("Strum");
	vOctBtn_.init("V/Oct");
	// blank.init("");
	// blank.init("");
	enPlus_.init("+EN");
	// blank.init("");

	helpBtn_.init("?");
	// blank.init("");
	writePrBtn_.init("WriPr");
	// blank.init("");
	// blank.init("");
	enMinus_.init("-EN");
	// blank.init("");


	audioBtn_.active(processor_.data().f_internal_exciter < 0.5);
	strumBtn_.active(processor_.data().f_internal_strum < 0.5);
	vOctBtn_.active(processor_.data().f_internal_note < 0.5);

	addAndMakeVisible(audioBtn_);
	addAndMakeVisible(strumBtn_);
	addAndMakeVisible(vOctBtn_);
	addAndMakeVisible(enPlus_);
	addAndMakeVisible(enMinus_);
	addAndMakeVisible(helpBtn_);
	addAndMakeVisible(writePrBtn_);

	// parameters
	pitchParam_.init("Pitch");
	structParam_.init("Struct");
	brightParam_.init("Bright");
	dampParam_.init("Damp");

	posParam_.init("Pos");
	polyParam_.init("Poly", "%1.0f");
	modelParam_.init("Model", "%1.0f");
	nullParam_.init("");

	pitchParam_.value(processor_.data().f_pitch);
	structParam_.value(processor_.data().f_structure);
	brightParam_.value(processor_.data().f_brightness);
	dampParam_.value(processor_.data().f_damping);

	posParam_.value(processor_.data().f_position);
	polyParam_.value(processor_.data().f_polyphony);
	modelParam_.value(processor_.data().f_model);
	//nullParam_

	altActive_ = 0;
	pitchParam_.active(!altActive_);
	structParam_.active(!altActive_);
	brightParam_.active(!altActive_);
	dampParam_.active(!altActive_);

	addAndMakeVisible(pitchParam_);
	addAndMakeVisible(structParam_);
	addAndMakeVisible(brightParam_);
	addAndMakeVisible(dampParam_);

	addAndMakeVisible(posParam_);
	addAndMakeVisible(polyParam_);
	addAndMakeVisible(modelParam_);
	addAndMakeVisible(nullParam_);
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
			posParam_.value(processor_.data().f_position);

			if(value) activeParam_=&posParam_;
		} else {
			float v = processor_.data().f_pitch + value / 4.0f;
			v = constrain(v, 0.0f, 60.0f);
			processor_.data().f_pitch = v;
			pitchParam_.value(processor_.data().f_pitch);

			if(value) activeParam_=&pitchParam_;
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
			polyParam_.value((int) processor_.data().f_polyphony);

			if(value) activeParam_=&polyParam_;

		} else {
			float v = processor_.data().f_structure + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_structure = v;
			structParam_.value(processor_.data().f_structure);

			if(value) activeParam_=&structParam_;
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
			modelParam_.value((int) processor_.data().f_model);

			if(value) activeParam_=&modelParam_;
		} else {
			float v = processor_.data().f_brightness + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_brightness = v;
			brightParam_.value(processor_.data().f_brightness);

			if(value) activeParam_=&brightParam_;
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 4 + altActive_;
		}
		break;
	case Percussa::sspEnc4:
		if (altActive_) {

		} else {
			float v = processor_.data().f_damping + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_damping = v;
			dampParam_.value(processor_.data().f_damping);

			if(value) activeParam_=&dampParam_;
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
		if (paramState_[index] != value && !value) {
			processor_.data().f_internal_exciter =
			    ! processor_.data().f_internal_exciter;
			audioBtn_.active(processor_.data().f_internal_exciter < 0.5);
		}
		break;
	case Percussa::sspSw2:
		if (paramState_[index] != value && !value) {
			processor_.data().f_internal_strum =
			    ! processor_.data().f_internal_strum ;
			strumBtn_.active(processor_.data().f_internal_strum < 0.5);
		}
		break;
	case Percussa::sspSw3:
		if (paramState_[index] != value && !value) {
			processor_.data().f_internal_note =
			    ! processor_.data().f_internal_note;
			vOctBtn_.active(processor_.data().f_internal_note < 0.5);
		}
		break;
	case Percussa::sspSw4:
		break;
	case Percussa::sspSw5:
		helpBtn_.active(value > 0.5);
		break;
	case Percussa::sspSw6:
		break;
	case Percussa::sspSw7:
		writePrBtn_.active(value > 0.5);
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
		enPlus_.active(value > 0.5);
		if (paramState_[index] != value && !value) {
			altActive_ = ! altActive_;

			pitchParam_.active(!altActive_);
			structParam_.active(!altActive_);
			brightParam_.active(!altActive_);
			dampParam_.active(!altActive_);

			posParam_.active(altActive_);
			polyParam_.active(altActive_);
			modelParam_.active(altActive_);
			nullParam_.active(altActive_);
		}
		break;
	case Percussa::sspSwDown:
		enMinus_.active(value > 0.5);
		if (paramState_[index] != value && !value) {
			altActive_ = ! altActive_;
			pitchParam_.active(!altActive_);
			structParam_.active(!altActive_);
			brightParam_.active(!altActive_);
			dampParam_.active(!altActive_);

			posParam_.active(altActive_);
			polyParam_.active(altActive_);
			modelParam_.active(altActive_);
			nullParam_.active(altActive_);
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

	if (helpBtn_.active()) {
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

	setButtonBounds(audioBtn_, 0, 0);
	setButtonBounds(strumBtn_, 0, 1);
	setButtonBounds(vOctBtn_, 0, 2);
	setButtonBounds(enPlus_, 0, 5);
	setButtonBounds(helpBtn_, 1, 0);
	setButtonBounds(writePrBtn_, 1, 2);
	setButtonBounds(enMinus_, 1, 5);


	setParamBounds(pitchParam_, 0, 0);
	setParamBounds(structParam_, 1, 0);
	setParamBounds(brightParam_, 2, 0);
	setParamBounds(dampParam_, 3, 0);

	setParamBounds(posParam_, 0, 1);
	setParamBounds(polyParam_, 1, 1);
	setParamBounds(modelParam_, 2, 1);
	setParamBounds(nullParam_, 3, 1);
}
