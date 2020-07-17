#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


PmixEditor::PmixEditor (Pmix& p)
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

	buttons_[B_HELP].init("?");
	buttons_[B_UP].init("+EN");
	buttons_[B_DOWN].init("-EN");
	buttons_[B_LEFT].init("-PG");
	buttons_[B_RIGHT].init("+PG");
	buttons_[B_WRITE_PR].init("WriPr");


	inTracks_[0].init("In 1", &processor_.inputTrack(0));
	inTracks_[1].init("In 2", &processor_.inputTrack(1));
	inTracks_[2].init("In 3", &processor_.inputTrack(2));
	inTracks_[3].init("In 4", &processor_.inputTrack(3));
	inTracks_[4].init("In 5", &processor_.inputTrack(4));
	inTracks_[5].init("In 6", &processor_.inputTrack(5));
	inTracks_[6].init("In 7", &processor_.inputTrack(6));
	inTracks_[7].init("In 8", &processor_.inputTrack(7));
	for(unsigned i=0;i<PluginProcessor::I_MAX;i++) {
		inTracks_.active[i]=true;
	}

	outTracks_[0].init("Main L", 	&processor_.outputTrack(0));
	outTracks_[1].init("Main R", 	&processor_.outputTrack(1));
	outTracks_[2].init("Aux 1 L", 	&processor_.outputTrack(2));
	outTracks_[3].init("Aux 1 R", 	&processor_.outputTrack(3));
	outTracks_[4].init("Aux 2 L", 	&processor_.outputTrack(4));
	outTracks_[5].init("Aux 2 R", 	&processor_.outputTrack(5));
	outTracks_[6].init("Aux 3 L", 	&processor_.outputTrack(6));
	outTracks_[7].init("Aux 3 R", 	&processor_.outputTrack(7));

	for(unsigned i=0;i<PluginProcessor::O_MAX;i++) {
		outTracks_.active[i]=true;
	}


	// buttons_[B_FREEZE].active(processor_.data().f_freeze > 0.5);


	for (int i = 0; i < B_MAX; i++) {
		addAndMakeVisible(buttons_[i]);
	}

	// parameters
	// params_[P_POS].init("Pos");
	// params_[P_POS].value(processor_.data().f_position);

	// params_[P_POS].active(paramActive_ 		== 0);

	paramActive_ = 0;
	// addAndMakeVisible(params_[P_POS]);
	// addChildComponent(params_[P_PITCH]);
}

PmixEditor::~PmixEditor()
{
	processor_.removeListener(this);
	stopTimer();
}

void PmixEditor::timerCallback()
{
	if (activeParamCount_ > 0 ) {
		activeParamCount_--;
		if (activeParamCount_ == 0) {
			activeParam_ = nullptr;
		}
	}
	repaint();
}


void PmixEditor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	auto fx =
	[ = ] {
		this->parameterChanged(parameterIndex, newValue);
	};

	MessageManager::callAsync (fx);
}

void 	PmixEditor::audioProcessorChanged (AudioProcessor *) {
	;
}

void PmixEditor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return;
	if (index >= Percussa::sspLast) return;

	//TODO
	// reconsider active param handling
	// there are 3 (!) pages of params not two!

	// f_mono      = 0.0f;
	// f_lofi      = 0.0f;

	unsigned paramActive = paramActive_;

	switch (index) {
	case Percussa::sspEnc1:
		switch (paramActive_) {
		case 0 :  {
			channelEncoder(0,value);
			// float v = processor_.data().f_position + value / 100.0f;
			// v = constrain(v, 0.0f, 1.0f);
			// processor_.data().f_position = v;
			// params_[P_POS].value(processor_.data().f_position);

			// if (value) activeParam_ = &params_[P_POS];

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
			channelEncoder(1,value);
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
			activeEncIdx_ = 1;
		}
		break;
	case Percussa::sspEnc3:
		switch (paramActive_) {
		case 0 :  {
			channelEncoder(2,value);
			break;
		}
		case 1 :  {
			break;
		}
		case 2 : {
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
			channelEncoder(3,value);
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
			activeEncIdx_ = 3;
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
		channelButton(1,value);
		// if (paramState_[index] != value && !value) {
		// 	processor_.data().f_freeze =
		// 	    ! processor_.data().f_freeze;
		// 	buttons_[B_FREEZE].active(processor_.data().f_freeze > 0.5);
		// }
		break;
	case Percussa::sspSw2:
		channelButton(2,value);
		break;
	case Percussa::sspSw3:
		channelButton(3,value);
		break;
	case Percussa::sspSw4:
		channelButton(4,value);
		break;
	case Percussa::sspSw5:
		channelButton(5,value);
		// buttons_[B_HELP].active(value > 0.5);
		break;
	case Percussa::sspSw6:
		channelButton(6,value);
		break;
	case Percussa::sspSw7:
		channelButton(7,value);
		// buttons_[B_WRITE_PR].active(value > 0.5);
		// if (paramState_[index] != value && !value) {
		// 	processor_.write();
		// }
		break;
	case Percussa::sspSw8:
		channelButton(8,value);
		break;

	case Percussa::sspSwLeft:
		buttons_[B_LEFT].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			if (paramActive_ == 2) paramActive_ = 0;
			else paramActive_ = 2;
		}
		break;
	case Percussa::sspSwRight:
		buttons_[B_RIGHT].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			if (paramActive_ == 2) paramActive_ = 0;
			else paramActive_ = 2;
		}
		break;
	case Percussa::sspSwUp:
	  	//TODO : non-latching
		buttons_[B_UP].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			activeTracks_=OUT_14;
			// if (paramActive_ != 2) {
			// 	paramActive_ = (paramActive_ + 1 ) % 2;
			// }
		}
		break;
	case Percussa::sspSwDown:
		buttons_[B_DOWN].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			if (paramActive_ != 2) {
				paramActive_ = (paramActive_ + 1 ) % 2;
			}
		}
		break;
	case Percussa::sspSwShiftL:
	  	//TODO : non-latching
		buttons_[B_SHIFTL].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			activeTracks_=IN_14;
			// if (paramActive_ != 2) {
			// 	paramActive_ = (paramActive_ + 1 ) % 2;
			// }
		}
		break;
	case Percussa::sspSwShiftR:
	  	//TODO : non-latching
		buttons_[B_SHIFTR].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			activeTracks_=IN_58;
			// if (paramActive_ != 2) {
			// 	paramActive_ = (paramActive_ + 1 ) % 2;
			// }
		}
		break;
	default:
		break;
	}

	if (paramActive != paramActive_) {

		// params_[P_POS].setVisible(paramActive_ 		< 2);
	}

	paramState_[index] = value;
}


void PmixEditor::channelEncoder(unsigned c, float v) {
	TrackSelect ts=activeTracks_;
	if(paramState_[Percussa::sspSwShiftR:]) {
		ts= IN_14;
	} else if (paramState_[Percussa::sspSwUp:]) {
		ts= OUT_14;
	} else if (paramState_[Percussa::sspSwShiftR:]) {
		ts= IN_58;
	}

	switch(ts) {
		case IN_14: {
			inTracks_[c].encoder(v);
			break;
		}
		case IN_58: {
			inTracks_[c+4].encoder(v);
			break;
		}
		case OUT_14: {
			outTracks_[c].encoder(v);
			break;
		}
	}
}

	
void PmixEditor::channelButton(unsigned c, bool v) {
	TrackSelect ts=activeTracks_;
	if(paramState_[Percussa::sspSwShiftR:]) {
		ts= IN_14;
	} else if (paramState_[Percussa::sspSwUp:]) {
		ts= OUT_14;
	} else if (paramState_[Percussa::sspSwShiftR:]) {
		ts= IN_58;
	}

	switch(ts) {
		case IN_14: {
		case IN_58: {
			inTracks_[c].button(v);
			break;
		}
		case OUT_14: {
			outTracks_[c].button(v);
			break;
		}
	}
}


void PmixEditor::drawHelp(Graphics & g) {

	unsigned x = 900;
	unsigned y = 40;
	unsigned space = 30;
	unsigned yText = y + space * 2;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 40, Font::plain));
	g.drawSingleLineText("Input/Output Help", x, y);

	y = yText;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 18, Font::plain));
	for(unsigned i=0;i<PluginProcessor::I_MAX;i++) {
		String s=String("VST IN[")+String(i)+String("] : " + processor_.getInputChannelName(i));
		g.drawSingleLineText(s, x, y);	y += space;
	}

	x = x + 300;
	y = yText;

	for(unsigned i=0;i<PluginProcessor::O_MAX;i++) {
		String s=String("VST OUT[")+String(i)+String("] : " + processor_.getOutputChannelName(i));
		g.drawSingleLineText(s, x, y);	y += space;
	}
}

void PmixEditor::drawPmix(Graphics & g) {
	// unsigned x = 1100;
	// unsigned y = 150;
	// unsigned d = 100;
	// unsigned sp = 75;

	// g.setColour(Colours::white);
	// g.fillEllipse(x, y, d, d);
	// g.fillEllipse(x + sp, y, d, d);

	// x = x - sp / 2;
	// y = y + sp;
	// g.fillEllipse(x, y, d, d);
	// g.fillEllipse(x + sp, y, d, d);
	// g.fillEllipse(x + (2 * sp), y, d, d);
}

void PmixEditor::drawMenuBox(Graphics & g) {
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

void PmixEditor::drawParamBox(Graphics & g) {
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


void PmixEditor::drawEncoderValue(Graphics & g) {
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

void PmixEditor::paint(Graphics & g)
{
	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)

	g.fillAll (Colours::black);

	// title
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText("pmix performance mixer", 20, 30 );
	g.setColour(Colours::grey);
	g.drawSingleLineText("preset : " + String(processor_.getCurrentProgram()), 1400, 30);

	drawMenuBox(g);
	drawParamBox(g);

	drawEncoderValue(g);

	if (buttons_[B_HELP].active()) {
		drawHelp(g);
	} else {
		drawPmix(g);
	}
}


void PmixEditor::setMenuBounds(SSPButton & btn, unsigned r) {
	const int w = 70;
	const int h = 45;
	unsigned x = 1530 + 1;
	unsigned y = menuTopY + (r * h);
	btn.setBounds(x, y, w, h);
}

void PmixEditor::setParamBounds(SSPParam & par, unsigned enc, unsigned var)
{
	unsigned h = 2 * paramSpaceY;
	unsigned w = unsigned(900.0f / 8.0f);
	unsigned x = ((enc * 2) + (var % 2))  * w;
	unsigned y = paramTopY + 5;
	par.setBounds(x, y, w, h);
}


void PmixEditor::setButtonBounds(SSPButton & btn, unsigned r, unsigned c)
{
	const int w = 100;
	const int h = paramSpaceY;
	unsigned x = 900 + (c * w);
	unsigned y = paramTopY + (r * h);
	btn.setBounds(x, y, w, h);
}


void PmixEditor::resized()
{
	setMenuBounds(globalBtn_, 0);
	setMenuBounds(networkBtn_, 1);
	setMenuBounds(plugInBtn_, 2);
	setMenuBounds(recBtn_, 3);

	// setButtonBounds(buttons_[B_FREEZE], 0, 0);
	setButtonBounds(buttons_[B_UP], 	0, 5);

	setButtonBounds(buttons_[B_HELP], 	1, 0);
	setButtonBounds(buttons_[B_WRITE_PR], 1, 2);
	setButtonBounds(buttons_[B_LEFT], 	1, 4);
	setButtonBounds(buttons_[B_DOWN], 	1, 5);
	setButtonBounds(buttons_[B_RIGHT], 	1, 6);

	// params?


	const unsigned space = 20;
	const unsigned inStart = space;
	const unsigned w = 80;
	const unsigned h = 200; //?
	const unsigned y = 100;

	const unsigned outStart = 900;

	for(unsigned i=i<PluginProcessor::I_MAX;i++) {
		unsigned x = inStart + (i * (space+w));
		inTracks_.setBounds(x, y, w, h);
	}

	for(unsigned i=i<PluginProcessor::O_MAX;i++) {
		unsigned x = outStart + (i * (space+w));
		outTracks_.setBounds(x, y, w, h);
	}


}
