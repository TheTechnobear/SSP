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
	buttons_[B_LEFT].init("-PG", Colours::red);
	buttons_[B_RIGHT].init("+PG", Colours::red);


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

	params_[P_FREQ_MOD].init("Frq M");
	params_[P_TIMBRE_MOD].init("Tmb M");
	params_[P_MORPH_MOD].init("Mrph M");

	params_[P_LPG].init("Lpg");
	params_[P_VCA].init("Vca");

	params_[P_PITCH].value(processor_.data().pitch_);
	params_[P_HARMONICS].value(processor_.data().harmonics_);
	params_[P_TIMBRE].value(processor_.data().timbre_);
	params_[P_MORPH].value(processor_.data().morph_);

	params_[P_FREQ_MOD].value(processor_.data().freq_mod_);
	params_[P_TIMBRE_MOD].value(processor_.data().timbre_mod_);
	params_[P_MORPH_MOD].value(processor_.data().morph_mod_);



	params_[P_MODEL].value(processor_.data().model_);
	params_[P_LPG].value(processor_.data().lpg_colour_);
	params_[P_VCA].value(processor_.data().decay_);

	for (unsigned i = 0; i < P_MAX; i++) {
		addChildComponent(params_[i]);
	}

	paramActive_ = 0;

	params_[P_PITCH].setVisible(paramActive_<2);
	params_[P_HARMONICS].setVisible(paramActive_<2);
	params_[P_TIMBRE].setVisible(paramActive_<2);
	params_[P_MORPH].setVisible(paramActive_<2);
	params_[P_MODEL].setVisible(paramActive_<2);
	params_[P_FREQ_MOD].setVisible(paramActive_<2);
	params_[P_TIMBRE_MOD].setVisible(paramActive_<2);
	params_[P_MORPH_MOD].setVisible(paramActive_<2);
	params_[P_LPG].setVisible(paramActive_==2);
	params_[P_VCA].setVisible(paramActive_==2);

	params_[P_PITCH].active(paramActive_==0);
	params_[P_HARMONICS].active(paramActive_==0);
	params_[P_TIMBRE].active(paramActive_==0);
	params_[P_MORPH].active(paramActive_==0);
	params_[P_MODEL].active(paramActive_==1);
	params_[P_FREQ_MOD].active(paramActive_==1);
	params_[P_TIMBRE_MOD].active(paramActive_==1);
	params_[P_MORPH_MOD].active(paramActive_==1);
	params_[P_LPG].active(paramActive_==2);
	params_[P_VCA].active(paramActive_==2);

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


	unsigned paramActive = paramActive_;
	float v=0.0f;

	switch (index) {

	// encoders
	case Percussa::sspEnc1:
		switch(paramActive_) {
			case 0: 
				v = processor_.data().pitch_ + value / 20.0f;
				v = constrain(v, -30.0f, 30.0f); // todo check
				processor_.data().pitch_ = v;
				params_[P_PITCH].value(processor_.data().pitch_);

				if (value) activeParam_ = &params_[P_PITCH];
				break;
			case 1: 
				v = processor_.data().model_ + value / 1.0f;
				v = constrain(v, 0.0f, 15.0f);
				processor_.data().model_ = v;
				params_[P_MODEL].value(processor_.data().model_);

				if (value) activeParam_ = &params_[P_MODEL];
				break;
			case 2: 
				v = processor_.data().lpg_colour_ + value / 100.0f;
				v = constrain(v, 0.0f, 1.0f);
				processor_.data().lpg_colour_ = v;
				params_[P_LPG].value(processor_.data().lpg_colour_);

				if (value) activeParam_ = &params_[P_LPG];
				break;
			default:
				break;
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 0;
		}
		break;
	case Percussa::sspEnc2:
		switch(paramActive_) {
			case 0: 
				v = processor_.data().harmonics_ + value / 100.0f;
				v = constrain(v, 0.0f, 1.0f);
				processor_.data().harmonics_ = v;
				params_[P_HARMONICS].value(processor_.data().harmonics_);

				if (value) activeParam_ = &params_[P_HARMONICS];
				break;
			case 1: 
				v = processor_.data().freq_mod_ + value / 100.0f;
				v = constrain(v, -1.0f, 1.0f);
				processor_.data().freq_mod_ = v;
				params_[P_FREQ_MOD].value(processor_.data().freq_mod_);

				if (value) activeParam_ = &params_[P_FREQ_MOD];
				break;
			case 2: 
				v = processor_.data().decay_ + value / 100.0f;
				v = constrain(v, 0.0f, 1.0f);
				processor_.data().decay_ = v;
				params_[P_VCA].value(processor_.data().decay_);

				if (value) activeParam_ = &params_[P_VCA];
				break;
			default:
				break;
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 1;
		}
		break;
	case Percussa::sspEnc3:
		switch(paramActive_) {
			case 0: 
				v = processor_.data().timbre_ + value / 100.0f;
				v = constrain(v, 0.0f, 1.0f);
				processor_.data().timbre_ = v;
				params_[P_TIMBRE].value(processor_.data().timbre_);

				if (value) activeParam_ = &params_[P_TIMBRE];
				break;
			case 1: 
				v = processor_.data().timbre_mod_ + value / 100.0f;
				v = constrain(v, -1.0f, 1.0f);
				processor_.data().timbre_mod_ = v;
				params_[P_TIMBRE_MOD].value(processor_.data().timbre_mod_);

				if (value) activeParam_ = &params_[P_TIMBRE_MOD];
				break;
			case 2: 
				break;
			default:
				break;
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 2;
		}
		break;
	case Percussa::sspEnc4:
		switch(paramActive_) {
			case 0: 
				v = processor_.data().morph_ + value / 100.0f;
				v = constrain(v, 0.0f, 1.0f);
				processor_.data().morph_ = v;
				params_[P_MORPH].value(processor_.data().morph_);

				if (value) activeParam_ = &params_[P_MORPH];
				break;
			case 1: 
				v = processor_.data().morph_mod_ + value / 100.0f;
				v = constrain(v, -1.0f, 1.0f);
				processor_.data().morph_mod_ = v;
				params_[P_MORPH_MOD].value(processor_.data().morph_mod_);

				if (value) activeParam_ = &params_[P_MORPH_MOD];
				break;
			case 2: 
				break;
			default:
				break;
		}
		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = 3;
		}
		break;
	// encoder switches
	case Percussa::sspEncSw1:
		if (value < 0.5f) {
			switch(paramActive_) {
				case 0: 
					processor_.data().pitch_ = 0.0f;
					params_[P_PITCH].value(processor_.data().pitch_);
					break;
				case 1: 
					processor_.data().model_ = 0.0f;
					params_[P_MODEL].value(processor_.data().model_);
					break;
				case 2: 
					processor_.data().lpg_colour_ = 0.50f;
					params_[P_LPG].value(processor_.data().lpg_colour_);
					break;
				default:
					break;
			}
		}
		break;
	case Percussa::sspEncSw2:
		if (value < 0.5f) {
			switch(paramActive_) {
				case 0: 
					processor_.data().harmonics_ = 0.0f;
					params_[P_HARMONICS].value(processor_.data().harmonics_);
					break;
				case 1: 
					processor_.data().freq_mod_ = 0.0f;
					params_[P_FREQ_MOD].value(processor_.data().freq_mod_);
					break;
				case 2: 
					processor_.data().decay_ = 0.50f;
					params_[P_VCA].value(processor_.data().decay_);
					break;
				default:
					break;
			}
		}
		break;
	case Percussa::sspEncSw3:
		if (value < 0.5f) {
			switch(paramActive_) {
				case 0: 
					processor_.data().timbre_ = 0.5f;
					params_[P_TIMBRE].value(processor_.data().timbre_);
					break;
				case 1: 
					processor_.data().timbre_mod_ = 0.0f;
					params_[P_TIMBRE_MOD].value(processor_.data().timbre_mod_);
					break;
				case 2: 
					break;
				default:
					break;
			}
		}
		break;
	case Percussa::sspEncSw4:
		if (value < 0.5f) {
			switch(paramActive_) {
				case 0: 
					processor_.data().morph_ = 0.5f;
					params_[P_MORPH].value(processor_.data().morph_);
					break;
				case 1: 
					processor_.data().morph_mod_ = 0.0f;
					params_[P_MORPH_MOD].value(processor_.data().morph_mod_);
					break;
				case 2: 
					break;
				default:
					break;
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

		params_[P_PITCH].setVisible(paramActive_<2);
		params_[P_HARMONICS].setVisible(paramActive_<2);
		params_[P_TIMBRE].setVisible(paramActive_<2);
		params_[P_MORPH].setVisible(paramActive_<2);
		params_[P_MODEL].setVisible(paramActive_<2);
		params_[P_FREQ_MOD].setVisible(paramActive_<2);
		params_[P_TIMBRE_MOD].setVisible(paramActive_<2);
		params_[P_MORPH_MOD].setVisible(paramActive_<2);
		params_[P_LPG].setVisible(paramActive_==2);
		params_[P_VCA].setVisible(paramActive_==2);

		params_[P_PITCH].active(paramActive_==0);
		params_[P_HARMONICS].active(paramActive_==0);
		params_[P_TIMBRE].active(paramActive_==0);
		params_[P_MORPH].active(paramActive_==0);
		params_[P_MODEL].active(paramActive_==1);
		params_[P_FREQ_MOD].active(paramActive_==1);
		params_[P_TIMBRE_MOD].active(paramActive_==1);
		params_[P_MORPH_MOD].active(paramActive_==1);
		params_[P_LPG].active(paramActive_==2);
		params_[P_VCA].active(paramActive_==2);
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
	unsigned x = ((enc * 2) + (var % 2))  * w;
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

	setButtonBounds(buttons_[B_UP], 	0, 5);

	setButtonBounds(buttons_[B_LEFT], 	1, 4);
	setButtonBounds(buttons_[B_DOWN], 	1, 5);
	setButtonBounds(buttons_[B_RIGHT], 	1, 6);

	setParamBounds(params_[P_PITCH], 		0, 0);
	setParamBounds(params_[P_HARMONICS], 	1, 0);
	setParamBounds(params_[P_TIMBRE], 		2, 0);
	setParamBounds(params_[P_MORPH], 		3, 0);

	setParamBounds(params_[P_MODEL], 		0, 1);
	setParamBounds(params_[P_FREQ_MOD], 	1, 1);
	setParamBounds(params_[P_TIMBRE_MOD], 	2, 1);
	setParamBounds(params_[P_MORPH_MOD], 	3, 1);

	setParamBounds(params_[P_LPG], 			0, 2);
	setParamBounds(params_[P_VCA], 			1, 2);
}
