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

	globalBtn_.init("G", Colours::red);
	networkBtn_.init("N", Colours::red);
	plugInBtn_.init("P", Colours::red);
	recBtn_.init("R", Colours::red);
	addAndMakeVisible(globalBtn_);
	addAndMakeVisible(networkBtn_);
	addAndMakeVisible(plugInBtn_);
	addAndMakeVisible(recBtn_);

	buttons_[B_FREEZE].init("Frze");
	buttons_[B_UP].init("+EN", Colours::red);
	buttons_[B_DOWN].init("-EN", Colours::red);
	buttons_[B_LEFT].init("-PG", Colours::red);
	buttons_[B_RIGHT].init("+PG", Colours::red);


	buttons_[B_FREEZE].active(processor_.data().f_freeze > 0.5);


	for (int i = 0; i < B_MAX; i++) {
		addAndMakeVisible(buttons_[i]);
	}

	// parameters
	params_[P_POS].init("Pos");
	params_[P_SIZE].init("Size");
	params_[P_DENSITY].init("Dens");
	params_[P_TEXTURE].init("Text");
	params_[P_MIX].init("Mix");
	params_[P_SPREAD].init("Spread");
	params_[P_FEEDBACK].init("Fdbk");
	params_[P_REVERB].init("Reverb");
	params_[P_PITCH].init("Pitch");
	params_[P_MODE].init("Mode");
	params_[P_IN_GAIN].init("Gain");

	params_[P_POS].value(processor_.data().f_position);
	params_[P_SIZE].value(processor_.data().f_size);
	params_[P_DENSITY].value(processor_.data().f_density);
	params_[P_TEXTURE].value(processor_.data().f_texture);
	params_[P_MIX].value(processor_.data().f_mix);
	params_[P_SPREAD].value(processor_.data().f_spread);
	params_[P_FEEDBACK].value(processor_.data().f_feedback);
	params_[P_REVERB].value(processor_.data().f_reverb);

	params_[P_PITCH].value(processor_.data().f_pitch);
	params_[P_MODE].value((int) processor_.data().f_mode);
	params_[P_IN_GAIN].value(processor_.data().f_in_gain);

	params_[P_POS].active(paramActive_ 		== 0);
	params_[P_SIZE].active(paramActive_ 	== 0);
	params_[P_DENSITY].active(paramActive_ 	== 0);
	params_[P_TEXTURE].active(paramActive_ 	== 0);

	params_[P_MIX].active(paramActive_ 		== 1);
	params_[P_SPREAD].active(paramActive_ 	== 1);
	params_[P_FEEDBACK].active(paramActive_ == 1);
	params_[P_REVERB].active(paramActive_ 	== 1);

	params_[P_PITCH].active(paramActive_ 	== 2);
	params_[P_MODE].active(paramActive_ 	== 2);
	params_[P_IN_GAIN].active(paramActive_ 	== 2);

	paramActive_ = 0;
	addAndMakeVisible(params_[P_POS]);
	addAndMakeVisible(params_[P_SIZE]);
	addAndMakeVisible(params_[P_DENSITY]);
	addAndMakeVisible(params_[P_TEXTURE]);
	addAndMakeVisible(params_[P_MIX]);
	addAndMakeVisible(params_[P_SPREAD]);
	addAndMakeVisible(params_[P_FEEDBACK]);
	addAndMakeVisible(params_[P_REVERB]);

	addChildComponent(params_[P_PITCH]);
	addChildComponent(params_[P_MODE]);
	addChildComponent(params_[P_IN_GAIN]);
}

CldsEditor::~CldsEditor()
{
	processor_.removeListener(this);
	stopTimer();
}

void CldsEditor::timerCallback()
{
	if (activeParamCount_ > 0 ) {
		activeParamCount_--;
		if (activeParamCount_ == 0) {
			activeParam_ = nullptr;
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
	// reconsider active param handling
	// there are 3 (!) pages of params not two!

	// f_mono      = 0.0f;
	// f_lofi      = 0.0f;

	unsigned paramActive = paramActive_;

	switch (index) {
	// encoders
	case Percussa::sspEnc1:
		switch (paramActive_) {
		case 0 :  {
			float v = processor_.data().f_position + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_position = v;
			params_[P_POS].value(processor_.data().f_position);

			if (value) activeParam_ = &params_[P_POS];
			break;
		}
		case 1 :  {
			float v = processor_.data().f_mix + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_mix = v;
			params_[P_MIX].value(processor_.data().f_mix);

			if (value) activeParam_ = &params_[P_MIX];

			break;
		}
		case 2 : {
			float v = processor_.data().f_pitch + value / 20.0f;
			v = constrain(v, -48.0f, 48.0f);
			processor_.data().f_pitch = v;
			params_[P_PITCH].value(processor_.data().f_pitch);

			if (value) activeParam_ = &params_[P_PITCH];
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
			float v = processor_.data().f_size + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_size = v;
			params_[P_SIZE].value(processor_.data().f_size);

			if (value) activeParam_ = &params_[P_SIZE];
			break;
		}
		case 1 :  {
			float v = processor_.data().f_spread + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_spread = v;
			params_[P_SPREAD].value(processor_.data().f_spread);

			if (value) activeParam_ = &params_[P_SPREAD];
			break;
		}
		case 2 : {
			float v = processor_.data().f_mode + value / 20.0f;
			v = constrain(v, 0.0f, 3.0f);
			processor_.data().f_mode = v;
			params_[P_MODE].value((int) processor_.data().f_mode);

			if (value) activeParam_ = &params_[P_MODE];
			break;
		}
		} //switch paramActive_
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 1;
		}
		break;
	case Percussa::sspEnc3:
		switch (paramActive_) {
		case 0 :  {
			float v = processor_.data().f_density + value / 100.0f;
			v = constrain(v, -1.0f, 1.0f);
			processor_.data().f_density = v;
			params_[P_DENSITY].value(processor_.data().f_density);

			if (value) activeParam_ = &params_[P_DENSITY];
			break;
		}
		case 1 :  {
			float v = processor_.data().f_feedback + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_feedback = v;
			params_[P_FEEDBACK].value(processor_.data().f_feedback);

			if (value) activeParam_ = &params_[P_FEEDBACK];
			break;
		}
		case 2 : {
			float v = processor_.data().f_in_gain + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_in_gain = v;
			params_[P_IN_GAIN].value(processor_.data().f_in_gain);

			if (value) activeParam_ = &params_[P_IN_GAIN];
		}
		} //switch paramActive_
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 2;
		}
		break;
	case Percussa::sspEnc4:
		switch (paramActive_) {
		case 0 :  {
			float v = processor_.data().f_texture + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_texture = v;
			params_[P_TEXTURE].value(processor_.data().f_texture);

			if (value) activeParam_ = &params_[P_TEXTURE];
			break;
		}
		case 1 :  {
			float v = processor_.data().f_reverb + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor_.data().f_reverb = v;
			params_[P_REVERB].value(processor_.data().f_reverb);

			if (value) activeParam_ = &params_[P_REVERB];
			break;
		}
		case 2 : {
			break;
		}
		} //switch paramActive_
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 3;
		}
		break;


	// encoder switches
	case Percussa::sspEncSw1:
		if (value > 0.5f) {
			switch (paramActive_) {
			case 0 :  {
				processor_.data().f_position = 0.0f;
				params_[P_POS].value(processor_.data().f_position);
				break;
			}
			case 1 :  {
				processor_.data().f_mix = 0.0f;
				params_[P_MIX].value(processor_.data().f_mix);
				break;
			}
			case 2 : {
				processor_.data().f_pitch = 0.0f;
				params_[P_PITCH].value(processor_.data().f_pitch);
				break;
			}
			} //switch paramActive_
		}
		break;
	case Percussa::sspEncSw2:
		if (value > 0.5f) {
			switch (paramActive_) {
			case 0 :  {
				processor_.data().f_size = 0.0f;
				params_[P_SIZE].value(processor_.data().f_size);
				break;
			}
			case 1 :  {
				processor_.data().f_spread = 0.0f;
				params_[P_SPREAD].value(processor_.data().f_spread);
				break;
			}
			case 2 : {
				processor_.data().f_mode = 0.0f;
				params_[P_MODE].value((int) processor_.data().f_mode);
				break;
			}
			} //switch paramActive_
		}
		break;
	case Percussa::sspEncSw3:
		if (value > 0.5f) {
			switch (paramActive_) {
			case 0 :  {
				processor_.data().f_density = 0.0f;
				params_[P_DENSITY].value(processor_.data().f_density);
				break;
			}
			case 1 :  {
				processor_.data().f_feedback = 0.0f;
				params_[P_FEEDBACK].value(processor_.data().f_feedback);
				break;
			}
			case 2 : {
				processor_.data().f_in_gain = 0.0f;
				params_[P_IN_GAIN].value(processor_.data().f_in_gain);
			}
			} //switch paramActive_
		}
		break;
	case Percussa::sspEncSw4:
		if (value > 0.5f) {
			switch (paramActive_) {
			case 0 :  {
				processor_.data().f_texture = 0.0f;
				params_[P_TEXTURE].value(processor_.data().f_texture);
				break;
			}
			case 1 :  {
				processor_.data().f_reverb = 0.0f;
				params_[P_REVERB].value(processor_.data().f_reverb);
				break;
			}
			case 2 : {
				break;
			}
			} //switch paramActive_
		}
		break;
	// buttons
	case Percussa::sspSw1:
		if (paramState_[index] != value && value < 0.5f) {
			processor_.data().f_freeze =
			    ! processor_.data().f_freeze;
			buttons_[B_FREEZE].active(processor_.data().f_freeze > 0.5f);
		}
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
		buttons_[B_LEFT].active(value > 0.5f);
		if (paramState_[index] != value && value < 0.5f) {
			if (paramActive_ == 2) paramActive_ = 0;
			else paramActive_ = 2;
		}
		break;
	case Percussa::sspSwRight:
		buttons_[B_RIGHT].active(value > 0.5f);
		if (paramState_[index] != value && value < 0.5f) {
			if (paramActive_ == 2) paramActive_ = 0;
			else paramActive_ = 2;
		}
		break;
	case Percussa::sspSwUp:
		buttons_[B_UP].active(value > 0.5f);
		if (paramState_[index] != value && value < 0.5f) {
			if (paramActive_ != 2) {
				paramActive_ = (paramActive_ + 1 ) % 2;
			}
		}
		break;
	case Percussa::sspSwDown:
		buttons_[B_DOWN].active(value > 0.5f);
		if (paramState_[index] != value && value < 0.5f) {
			if (paramActive_ != 2) {
				paramActive_ = (paramActive_ + 1 ) % 2;
			}
		}
		break;
	case Percussa::sspSwShiftL:
		break;
	case Percussa::sspSwShiftR:
		break;
	default:
		break;
	}

	if (paramActive != paramActive_) {

		params_[P_POS].setVisible(paramActive_ 		< 2);
		params_[P_SIZE].setVisible(paramActive_ 	< 2);
		params_[P_DENSITY].setVisible(paramActive_	< 2);
		params_[P_TEXTURE].setVisible(paramActive_ 	< 2);
		params_[P_MIX].setVisible(paramActive_ 		< 2);
		params_[P_SPREAD].setVisible(paramActive_ 	< 2);
		params_[P_FEEDBACK].setVisible(paramActive_ < 2);
		params_[P_REVERB].setVisible(paramActive_ 	< 2);

		params_[P_PITCH].setVisible(paramActive_ 	== 2);
		params_[P_MODE].setVisible(paramActive_ 	== 2);
		params_[P_IN_GAIN].setVisible(paramActive_ 	== 2);


		params_[P_POS].active(paramActive_ 		== 0);
		params_[P_SIZE].active(paramActive_ 	== 0);
		params_[P_DENSITY].active(paramActive_ 	== 0);
		params_[P_TEXTURE].active(paramActive_ 	== 0);

		params_[P_MIX].active(paramActive_ 		== 1);
		params_[P_SPREAD].active(paramActive_ 	== 1);
		params_[P_FEEDBACK].active(paramActive_ == 1);
		params_[P_REVERB].active(paramActive_ 	== 1);

		params_[P_PITCH].active(paramActive_ 	== 2);
		params_[P_MODE].active(paramActive_ 	== 2);
		params_[P_IN_GAIN].active(paramActive_ 	== 2);
	}

	paramState_[index] = value;
}


void CldsEditor::drawClds(Graphics & g) {
	unsigned x = 1100;
	unsigned y = 150;
	unsigned d = 100;
	unsigned sp = 75;

	g.setColour(Colours::white);
	g.fillEllipse(x, y, d, d);
	g.fillEllipse(x + sp, y, d, d);

	x = x - sp / 2;
	y = y + sp;
	g.fillEllipse(x, y, d, d);
	g.fillEllipse(x + sp, y, d, d);
	g.fillEllipse(x + (2 * sp), y, d, d);
}

void CldsEditor::drawMenuBox(Graphics & g) {
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

void CldsEditor::drawParamBox(Graphics & g) {
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


void CldsEditor::drawEncoderValue(Graphics & g) {
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

void CldsEditor::paint(Graphics & g)
{
	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)

	g.fillAll (Colours::black);

	// title
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText("clds texture synthesizer", 20, 30 );
	g.setColour(Colours::grey);
	g.drawSingleLineText("version : " + String(JucePlugin_VersionString), 1400, 30);

	drawMenuBox(g);
	drawParamBox(g);

	drawEncoderValue(g);

	drawClds(g);
}


void CldsEditor::setMenuBounds(SSPButton & btn, unsigned r) {
	const int w = 70;
	const int h = 45;
	unsigned x = 1530 + 1;
	unsigned y = menuTopY + (r * h);
	btn.setBounds(x, y, w, h);
}

void CldsEditor::setParamBounds(SSPParam & par, unsigned enc, unsigned var)
{
	unsigned h = 2 * paramSpaceY;
	unsigned w = unsigned(900.0f / 8.0f);
	unsigned x = ((enc * 2) + (var % 2))  * w;
	unsigned y = paramTopY + 5;
	par.setBounds(x, y, w, h);
}


void CldsEditor::setButtonBounds(SSPButton & btn, unsigned r, unsigned c)
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
	setButtonBounds(buttons_[B_UP], 	0, 5);

	setButtonBounds(buttons_[B_LEFT], 	1, 4);
	setButtonBounds(buttons_[B_DOWN], 	1, 5);
	setButtonBounds(buttons_[B_RIGHT], 	1, 6);

	setParamBounds(params_[P_POS], 		0, 0);
	setParamBounds(params_[P_SIZE], 	1, 0);
	setParamBounds(params_[P_DENSITY], 	2, 0);
	setParamBounds(params_[P_TEXTURE], 	3, 0);

	setParamBounds(params_[P_MIX], 		0, 1);
	setParamBounds(params_[P_SPREAD], 	1, 1);
	setParamBounds(params_[P_FEEDBACK], 2, 1);
	setParamBounds(params_[P_REVERB], 	3, 1);

	setParamBounds(params_[P_PITCH], 	0, 2);
	setParamBounds(params_[P_MODE], 	1, 2);
	setParamBounds(params_[P_IN_GAIN], 	2, 2);
}
