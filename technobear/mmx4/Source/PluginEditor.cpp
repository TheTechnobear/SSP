#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


PluginEditor::PluginEditor (PluginProcessor& p)
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

	// buttons_[B_FREEZE].init("Frze");
	buttons_[B_UP].init("+EN", Colours::red);
	buttons_[B_DOWN].init("-EN", Colours::red);
	buttons_[B_LEFT].init("-PG", Colours::red);
	buttons_[B_RIGHT].init("+PG", Colours::red);


	// buttons_[B_FREEZE].active(processor_.data().f_freeze > 0.5);


	for (int i = 0; i < B_MAX; i++) {
		addAndMakeVisible(buttons_[i]);
	}

	// parameters
	// params_[P_SCALE].init("Scale");
	// params_[P_ROOT].init("Root");

	// params_[P_POS].value(processor_.data().f_position);
	// params_[P_SIZE].value(processor_.data().f_size);
	// params_[P_SCALE].active(paramActive_ 		== 0);
	// params_[P_ROOT].active(paramActive_ 	== 0);

	paramActive_ = 0;
	addAndMakeVisible(params_[P_SCALE]);
	addAndMakeVisible(params_[P_ROOT]);
}

PluginEditor::~PluginEditor()
{
	processor_.removeListener(this);
	stopTimer();
}

void PluginEditor::timerCallback()
{
	if (activeParamCount_ > 0 ) {
		activeParamCount_--;
		if (activeParamCount_ == 0) {
			activeParam_ = nullptr;
		}
	}
	repaint();
}


void PluginEditor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	auto fx =
	[ = ] {
		this->parameterChanged(parameterIndex, newValue);
	};

	MessageManager::callAsync (fx);
}

void 	PluginEditor::audioProcessorChanged (AudioProcessor *) {
	;
}

void PluginEditor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return;
	if (index >= Percussa::sspLast) return;

	unsigned paramActive = paramActive_;

	switch (index) {
	// encoders
	case Percussa::sspEnc1:
		switch (paramActive_) {
		case 0 :  {
			float v = processor_.data().scale_ + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.scale_ = v;
			params_[P_POS].value(processor_.data().scale_);

			if (value) activeParam_ = &params_[P_SCALE];
			break;
		}
		case 1 :  {
			break;
		}
		case 2 : {
			break;
		}
		} //switch paramActive_
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 0;
		}
		break;
	case Percussa::sspEnc2:
		switch (paramActive_) {
		case 0 :  {
			float v = processor_.root_ + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.root_ = v;
			params_[P_ROOT].value(processor_.root_);

			if (value) activeParam_ = &params_[P_ROOT];
			break;
		case 1 :  {
			break;
		}
		case 2 : {
			break;
		}
		}
		} //switch paramActive_
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 1;
		}
		break;
	case Percussa::sspEnc3:
		// switch (paramActive_) {
		// case 0 :  {
		// 	break;
		// }
		// case 1 :  {
		// 	break;
		// }
		// case 2 : {
		// 	break;
		// }
		// } //switch paramActive_
		// if (value) {
		// 	activeParamCount_ = PARAM_COUNTER;
		// 	activeEncIdx_ = 2;
		// }
		break;
	case Percussa::sspEnc4:
		// switch (paramActive_) {
		// case 0 :  {
		// 	break;
		// }
		// case 1 :  {
		// 	break;
		// }
		// case 2 : {
		// 	break;
		// }
		// } //switch paramActive_
		// if (value) {
		// 	activeParamCount_ = PARAM_COUNTER;
		// 	activeEncIdx_ = 3;
		// }
		break;


	// encoder switches
	case Percussa::sspEncSw1:
		if (value < 0.5f) {
			switch (paramActive_) {
			case 0 :  {
				processor_.scale_ = 0.0f;
				params_[P_SCALE].value(processor_.scale_);
				break;
			}
			case 1 :  {
				break;
			}
			case 2 : {
				break;
			}
			} //switch paramActive_
		// }
		break;
	case Percussa::sspEncSw2:
		if (value < 0.5f) {
			switch (paramActive_) {
			case 0 :  {
				processor_.root_ = 0.0f;
				params_[P_ROOT].value(processor_.root_);
				break;
			}
			case 1 :  {
				break;
			}
			case 2 : {
				break;
			}
			} //switch paramActive_
		break;
	case Percussa::sspEncSw3:
		break;
	case Percussa::sspEncSw4:
		break;
	// buttons
	case Percussa::sspSw1:
		// if (paramState_[index] != value && value < 0.5f) {
		// 	processor_.data().f_freeze =
		// 	    ! processor_.data().f_freeze;
		// 	buttons_[B_FREEZE].active(processor_.data().f_freeze > 0.5f);
		// }
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
		// buttons_[B_LEFT].active(value > 0.5f);
		// if (paramState_[index] != value && value < 0.5f) {
		// 	if (paramActive_ == 2) paramActive_ = 0;
		// 	else paramActive_ = 2;
		// }
		break;
	case Percussa::sspSwRight:
		// buttons_[B_RIGHT].active(value > 0.5f);
		// if (paramState_[index] != value && value < 0.5f) {
		// 	if (paramActive_ == 2) paramActive_ = 0;
		// 	else paramActive_ = 2;
		// }
		break;
	case Percussa::sspSwUp:
		// buttons_[B_UP].active(value > 0.5f);
		// if (paramState_[index] != value && value < 0.5f) {
		// 	if (paramActive_ != 2) {
		// 		paramActive_ = (paramActive_ + 1 ) % 2;
		// 	}
		// }
		break;
	case Percussa::sspSwDown:
		// buttons_[B_DOWN].active(value > 0.5f);
		// if (paramState_[index] != value && value < 0.5f) {
		// 	if (paramActive_ != 2) {
		// 		paramActive_ = (paramActive_ + 1 ) % 2;
		// 	}
		// }
		break;
	case Percussa::sspSwShiftL:
		break;
	case Percussa::sspSwShiftR:
		break;
	default:
		break;
	}

	if (paramActive != paramActive_) {

		// params_[P_SCALE].setVisible(paramActive_ 	< 2);
		// params_[P_ROOT].setVisible(paramActive_ 	< 2);
		// params_[P_PITCH].setVisible(paramActive_ 	== 2);
		// params_[P_MODE].setVisible(paramActive_ 	== 2);
		// params_[P_IN_GAIN].setVisible(paramActive_ 	== 2);


		// params_[P_SCALE].active(paramActive_ 		== 0);
		// params_[P_ROOT].active(paramActive_ 		== 1);
		// params_[P_PITCH].active(paramActive_ 	== 2);
	}

	paramState_[index] = value;
}


void PluginEditor::drawIcon(Graphics & g) {
	unsigned x = 1100;
	unsigned y = 150;
	unsigned d = 100;
	unsigned sp = 75;

	g.setColour(Colours::white);
	g.fillEllipse(x, y, d, d);
	g.fillEllipse(x + sp, y, d, d);

	// x = x - sp / 2;
	// y = y + sp;
	// g.fillEllipse(x, y, d, d);
	// g.fillEllipse(x + sp, y, d, d);
	// g.fillEllipse(x + (2 * sp), y, d, d);
}

void PluginEditor::drawMenuBox(Graphics & g) {
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

void PluginEditor::drawParamBox(Graphics & g) {
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


void PluginEditor::drawEncoderValue(Graphics & g) {
	if (activeParam_ != nullptr) {
		unsigned butLeftX = 900 - 1;
		String val = String::formatted(activeParam_->fmt(), activeParam_->value());
		if (activeParam_->unit().length()) { val = val + " " + activeParam_->unit();}

		static constexpr unsigned pw = unsigned(900.0f / 8.0f);

		unsigned activeParamIdx = (activeEncIdx_ * 2) + paramActive_ % 2;

		g.setColour(Colours::red);
		g.drawHorizontalLine(40, 0, butLeftX);
		g.drawVerticalLine(0, 40, paramTopY);
		g.drawVerticalLine(butLeftX, 40, paramTopY - 1);
		g.drawHorizontalLine(paramTopY - 1, 0,  activeParamIdx * pw);
		g.drawHorizontalLine(paramTopY - 1, (activeParamIdx + 1)* pw, butLeftX);

		g.drawVerticalLine(activeParamIdx * pw, paramTopY, 1600 - 1);
		g.drawVerticalLine((activeParamIdx + 1) * pw, paramTopY, 1600 - 1);
		// g.drawHorizontalLine(1600-1, activeParamIdx * pw, (activeParamIdx+1)* pw);

		g.setColour(Colours::white);
		g.setFont(Font(Font::getDefaultMonospacedFontName(), 180, Font::plain));

		g.drawText(val, 0, 40, butLeftX, paramTopY - 40 - 40, Justification::centred);
	}
}

void PluginEditor::paint(Graphics & g)
{
	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)

	g.fillAll (Colours::black);

	// title
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText("mmx4 matrix mixer", 20, 30 );
	g.setColour(Colours::grey);
	g.drawSingleLineText("version : " + String(JucePlugin_VersionString), 1400, 30);

	drawMenuBox(g);
	drawParamBox(g);

	drawEncoderValue(g);

	drawIcon(g);
}


void PluginEditor::setMenuBounds(SSPButton & btn, unsigned r) {
	const int w = 70;
	const int h = 45;
	unsigned x = 1530 + 1;
	unsigned y = menuTopY + (r * h);
	btn.setBounds(x, y, w, h);
}

void PluginEditor::setParamBounds(SSPParam & par, unsigned enc, unsigned var)
{
	unsigned h = 2 * paramSpaceY;
	unsigned w = unsigned(900.0f / 8.0f);
	unsigned x = ((enc * 2) + (var % 2))  * w;
	unsigned y = paramTopY + 5;
	par.setBounds(x, y, w, h);
}


void PluginEditor::setButtonBounds(SSPButton & btn, unsigned r, unsigned c)
{
	const int w = 100;
	const int h = paramSpaceY;
	unsigned x = 900 + (c * w);
	unsigned y = paramTopY + (r * h);
	btn.setBounds(x, y, w, h);
}


void PluginEditor::resized()
{
	setMenuBounds(globalBtn_, 0);
	setMenuBounds(networkBtn_, 1);
	setMenuBounds(plugInBtn_, 2);
	setMenuBounds(recBtn_, 3);

	// setButtonBounds(buttons_[B_FREEZE], 0, 0);
	// setButtonBounds(buttons_[B_UP], 	0, 5);

	// setButtonBounds(buttons_[B_LEFT], 	1, 4);
	// setButtonBounds(buttons_[B_DOWN], 	1, 5);
	// setButtonBounds(buttons_[B_RIGHT], 	1, 6);

	// setParamBounds(params_[P_SCALE], 		0, 0);
	// setParamBounds(params_[P_ROOT], 	1, 0);

	// setParamBounds(params_[P_MIX], 		0, 1);

	// setParamBounds(params_[P_PITCH], 	0, 2);
}
