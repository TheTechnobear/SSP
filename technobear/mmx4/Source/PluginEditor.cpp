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
	params_[P_VCA_1A].init("VCA 1A");
	params_[P_VCA_1B].init("VCA 1B");
	params_[P_VCA_1C].init("VCA 1C");
	params_[P_VCA_1D].init("VCA 1D");
	params_[P_VCA_2A].init("VCA 2A");
	params_[P_VCA_2B].init("VCA 2B");
	params_[P_VCA_2C].init("VCA 2C");
	params_[P_VCA_2D].init("VCA 2D");
	params_[P_VCA_3A].init("VCA 3A");
	params_[P_VCA_3B].init("VCA 3B");
	params_[P_VCA_3C].init("VCA 3C");
	params_[P_VCA_3D].init("VCA 3D");
	params_[P_VCA_4A].init("VCA 4A");
	params_[P_VCA_4B].init("VCA 4B");
	params_[P_VCA_4C].init("VCA 4C");
	params_[P_VCA_4D].init("VCA 4D");

	paramActive_ = 0;
	for (unsigned i = P_VCA_1A; i <= P_VCA_4D; i++) {
		unsigned vca = i - P_VCA_1A;
		bool active = (vca / PluginProcessor::MAX_SIG_OUT) == paramActive_;

		params_[i].value(
		    processor_.getVCA(
		        vca / PluginProcessor::MAX_SIG_OUT,
		        vca % PluginProcessor::MAX_SIG_OUT));
		params_[i].active(active);

		if (active) {
			addAndMakeVisible(params_[i]);
		} else {
			addChildComponent(params_[i]);
		}
	}
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
	case Percussa::sspEnc2:
	case Percussa::sspEnc3:
	case Percussa::sspEnc4:
		if (paramActive_ < PluginProcessor::MAX_SIG_IN)  {
			unsigned out = index - Percussa::sspEnc1;
			unsigned in = paramActive_;
			float v = processor_.getVCA(in, out) + value / 100.0f;
			v = constrain(v, -4.0f, 4.0f);
			processor_.setVCA(in, out, v);
			unsigned pidx = P_VCA_1A + (in * PluginProcessor::MAX_SIG_OUT) + out;
			params_[pidx].value(v);
			if (value) activeParam_ = &params_[pidx];
		}

		if (value) {
			activeParamCount_ = PARAM_COUNTER;
			activeEncIdx_ = index - Percussa::sspEnc1;
		}
		break;


	// encoder switches
	case Percussa::sspEncSw1:
	case Percussa::sspEncSw2:
	case Percussa::sspEncSw3:
	case Percussa::sspEncSw4:
		if (value < 0.5f) {
			if (paramActive_ < PluginProcessor::MAX_SIG_IN)  {
				unsigned out = index - Percussa::sspEncSw1;
				unsigned in = paramActive_;
				float v = processor_.getVCA(in, out);
				if (v == 0.0f) v = 1.0f;
				else v = 0.0f;
				processor_.setVCA(in, out, v);
				unsigned pidx = P_VCA_1A + (in * PluginProcessor::MAX_SIG_OUT) + out;
				params_[pidx].value(v);
			}
		}
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
		buttons_[B_UP].active(value > 0.5f);
		if (paramState_[index] > 0.5f && value < 0.5f) {
			if (paramActive_ > 0) {
				paramActive_--;
			} else {
				paramActive_ = PluginProcessor::MAX_SIG_IN - 1;
			}
		}
		break;
	case Percussa::sspSwDown:
		buttons_[B_DOWN].active(value > 0.5f);
		if (paramState_[index] > 0.5f && value < 0.5f) {
			paramActive_ = (paramActive_ + 1 ) % PluginProcessor::MAX_SIG_IN;
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

		for (unsigned i = P_VCA_1A; i <= P_VCA_4D; i++) {
			unsigned vca = i - P_VCA_1A;
			bool active = (vca / PluginProcessor::MAX_SIG_OUT) == paramActive_;

			params_[i].active(active);
			params_[i].setVisible(active);
		}
	}

	paramState_[index] = value;
}


void PluginEditor::drawGrid(Graphics& g) {
	int x  = 200;
	int y  = 30;
	int lx = 100;
	int fh = 24;
	int szx = fh * 4;
	int szy = fh * 2;
	int sp = fh;

	g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
	g.setColour(Colours::red);
	g.drawText("Outputs", x + lx, y,
	           (PluginProcessor::MAX_SIG_OUT * szx) + ((PluginProcessor::MAX_SIG_OUT - 1)  * sp),
	           fh, Justification::centred);
	y += sp;
	g.drawText("Inputs", x, y, lx, fh, Justification::left);
	for (unsigned xi = 0; xi < PluginProcessor::MAX_SIG_OUT; xi++) {
		if (processor_.isOutputEnabled(xi * 2) + processor_.isOutputEnabled(xi * 2 + 1) ) {
			g.setColour(Colours::red);
		} else {
			g.setColour(Colours::grey);
		}
		char chout[2] = "A";
		chout[0] = 'A' + xi;
		g.drawText(String(chout), x + lx + xi * (szx + sp), y, sp + szx, fh, Justification::left);
	}
	y += sp;

	for (unsigned yi = 0; yi < PluginProcessor::MAX_SIG_IN; yi++) {
		if (processor_.isInputEnabled(yi * 2) || processor_.isInputEnabled(yi * 2 + 1)) {
			g.setColour(Colours::red);
		} else {
			g.setColour(Colours::grey);
		}
		g.drawText(String(yi + 1), x, y, lx, fh, Justification::left);
		for (unsigned xi = 0; xi < PluginProcessor::MAX_SIG_OUT; xi++) {
			g.setColour(Colours::white);

			float vca = processor_.getVCA(yi, xi);
			String v = "-";
			if (vca <= -0.01f || vca >= 0.01f) v = String::formatted("%4.2f", vca);
			g.drawText(v, x + lx + xi * (szx + sp), y , sp + szx, fh, Justification::left);
		}
		y += sp + szy;
	}
}


void PluginEditor::drawIcon(Graphics & g) {
	// unsigned x = 1100;
	// unsigned y = 150;
	// unsigned d = 100;
	// unsigned sp = 75;
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
	drawGrid(g);
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
	setButtonBounds(buttons_[B_UP], 	0, 5);

	// setButtonBounds(buttons_[B_LEFT], 	1, 4);
	setButtonBounds(buttons_[B_DOWN], 	1, 5);
	// setButtonBounds(buttons_[B_RIGHT], 	1, 6);


	for (unsigned i = P_VCA_1A; i <= P_VCA_4D; i++) {
		unsigned vca = i - P_VCA_1A;
		unsigned out = vca % PluginProcessor::MAX_SIG_OUT;
		// unsigned in = vca / PluginProcessor::MAX_SIG_OUT;
		setParamBounds(params_[i], out, 0);

	}
}
