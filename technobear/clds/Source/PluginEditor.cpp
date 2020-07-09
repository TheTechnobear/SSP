#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


CldsEditor::CldsEditor (Clds& p)
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

	buttons_[B_FREEZE].init("Freeze");
	buttons_[B_FREEZE].active(processor_.data().f_freeze > 0.5);

	for(int i=0;i<B_MAX;i++) {
		addAndMakeVisible(buttons_[i]);
	}


	// parameters
	params_[P_PITCH].init("Pitch");

	params_[P_PITCH].value(processor_.data().f_pitch);

	altActive_ = 0;
	params_[P_PITCH].active(!altActive_);



	for(int i=0;i<P_MAX;i++) {
		addAndMakeVisible(params_[i]);
	}
	addAndMakeVisible(pitchParam_);
}

CldsEditor::~CldsEditor()
{
	processor_.removeListener(this);
	stopTimer();
}

void CldsEditor::timerCallback()
{
	if(activeParamCount_>0 ) {
		activeParamCount_--;
		if(activeParamCount_==0) {
			activeParam_=nullptr;
		}
	}
	repaint();
}


void CldsEditor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	auto fx =
	[ = ] {
		this->parameterChanged(parameterIndex, newValue);
	};

	MessageManager::callAsync (fx);
}

void 	CldsEditor::audioProcessorChanged (AudioProcessor *) {
	;
}

void CldsEditor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return;
	if (index >= Percussa::sspLast) return;

	//TODO 
	// add all params/buttons
	// reconsider active param handling
	// there are 3 (!) pages of params not two!

	// use this if you need to do something special
	// to process parameter
	switch (index) {
	case Percussa::sspEnc1:
		switch(paramActive_) {
			case 0 :  {
				float v = processor_.data().f_pitch + value / 4.0f;
				v = constrain(v, 0.0f, 60.0f);
				processor_.data().f_pitch = v;
				params_[P_PITCH].value(processor_.data().f_pitch);

				if(value) activeParam_=&pitchParam_;
				break;
			}
			case 1 :  {
				break;
			}
			case 2 : {			
				break;
			}
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 0 + paramActive_;
		}
		break;
	case Percussa::sspEnc2:
		switch(paramActive_) {
			case 0 :  {
				break;
			}
			case 1 :  {
				break;
			}
			case 2 : {			
				break;
			}
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 2 + paramActive_;
		}
		break;
	case Percussa::sspEnc3:
		switch(paramActive_) {
			case 0 :  {
				break;
			}
			case 1 :  {
				break;
			}
			case 2 : {			
				break;
			}
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 4 + paramActive_;
		}
		break;
	case Percussa::sspEnc4:
		switch(paramActive_) {
			case 0 :  {
				break;
			}
			case 1 :  {
				break;
			}
			case 2 : {			
				break;
			}
		}
		if(value) {
			activeParamCount_=PARAM_COUNTER;
			activeParamIdx_= 6 + paramActive_;
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
			processor_.data().f_freeze =
			    ! processor_.data().f_freeze;
			buttons[B_FREEZE].active(processor_.data().f_freeze > 0.5);
		}
		break;
	case Percussa::sspSw2:

		break;
	case Percussa::sspSw3:

		break;
	case Percussa::sspSw4:
		break;
	case Percussa::sspSw5:
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
		enPlus_.active(value > 0.5);
		if (paramState_[index] != value && !value) {
			paramActive_ = ((paramActive_ ++ ) % 2);
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
			if(paramActive_==0) paramActive_=2;
			else paramActive_--;
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

void CldsEditor::drawHelp(Graphics& g) {

	unsigned x = 900;
	unsigned y = 40;
	unsigned space = 30;
	unsigned yText = y + space * 2;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 40, Font::plain));
	g.drawSingleLineText("Input/Output Help", x, y);


        I_LEFT,
        I_RIGHT,
        I_TRIG,
        I_VOCT,
        I_POS,
        I_SIZE,
        I_DENSITY,
        I_TEXT,

	y = yText;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 18, Font::plain));
	g.drawSingleLineText("VST IN[1] : IN Left", x, y);	y += space;
	g.drawSingleLineText("VST IN[2] : IN Right", x, y);	y += space;
	g.drawSingleLineText("VST IN[3] : Trig", x, y);	y += space;
	g.drawSingleLineText("VST IN[4] : V/Oct", x, y);		y += space;
	g.drawSingleLineText("VST IN[5] : Position", x, y);	y += space;
	g.drawSingleLineText("VST IN[6] : Size", x, y);	y += space;
	g.drawSingleLineText("VST IN[7] : Density", x, y);	y += space;
	g.drawSingleLineText("VST IN[8] : Texture", x, y);	y += space;

	x = x + 300;
	y = yText;
	g.drawSingleLineText("VST OUT[1] : OUT Left", x, y);		y += space;
	g.drawSingleLineText("VST OUT[2] : Out Right", x, y);	y += space;
}

void CldsEditor::drawClds(Graphics& g) {
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

void CldsEditor::drawMenuBox(Graphics& g) {
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

void CldsEditor::drawParamBox(Graphics& g) {
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


void CldsEditor::drawEncoderValue(Graphics& g) {
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

void CldsEditor::paint(Graphics& g)
{
	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)

	g.fillAll (Colours::black);

	// title
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText("clds resonator", 20, 30 );
	g.setColour(Colours::grey);
    g.drawSingleLineText("preset : " + String(processor_.getCurrentProgram()),1400,30);

	drawMenuBox(g);
	drawParamBox(g);

	drawEncoderValue(g);	

	if (helpBtn_.active()) {
		drawHelp(g);
	} else {
		drawClds(g);
	}
}


void CldsEditor::setMenuBounds(SSPButton& btn, unsigned r) {
	const int w = 70;
	const int h = 45;
	unsigned x = 1530 + 1;
	unsigned y = menuTopY + (r * h);
	btn.setBounds(x, y, w, h);
}

void CldsEditor::setParamBounds(SSPParam& par, unsigned enc, unsigned var)
{
	unsigned h = 2 * paramSpaceY;
	unsigned w = unsigned(900.0f / 8.0f);
	unsigned x = ((enc * 2) + var) * w;
	unsigned y = paramTopY + 5;
	par.setBounds(x, y, w, h);
}


void CldsEditor::setButtonBounds(SSPButton& btn, unsigned r, unsigned c)
{
	const int w = 100;
	const int h = paramSpaceY;
	unsigned x = 900 + (c * w);
	unsigned y = paramTopY + (r * h);
	btn.setBounds(x, y, w, h);
}


void CldsEditor::resized()
{
	setMenuBounds(globalBtn_, 0);
	setMenuBounds(networkBtn_, 1);
	setMenuBounds(plugInBtn_, 2);
	setMenuBounds(recBtn_, 3);

	setButtonBounds(buttons_[B_FREEZE], 0, 0);
	// setButtonBounds(strumBtn_, 0, 1);
	// setButtonBounds(vOctBtn_, 0, 2);
	// setButtonBounds(enPlus_, 0, 5);
	setButtonBounds(buttons_[B_HELP], 1, 0);
	setButtonBounds(buttons_[B_EN_PLUS], 1, 2);
	setButtonBounds(buttons_[B_EN_PLUS], 1, 5);


	setParamBounds(params_[P_PITCH], 0, 0);
	// setParamBounds(structParam_, 1, 0);
	// setParamBounds(brightParam_, 2, 0);
	// setParamBounds(dampParam_, 3, 0);

	// setParamBounds(posParam_, 0, 1);
	// setParamBounds(polyParam_, 1, 1);
	// setParamBounds(modelParam_, 2, 1);
	// setParamBounds(nullParam_, 3, 1);
}
