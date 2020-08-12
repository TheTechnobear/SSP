#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


PmixEditor::PmixEditor (Pmix& p)
	: AudioProcessorEditor (&p), processor_ (p)
{
	processor_.addListener(this);

	setSize (1600, 480);

	for (unsigned i = 0; i < TS_MAX; i++) {
        buttonHeldCount_[i]=0;
    }
    encMode_=0;
    butMode_=0;
	activeTracks_ = OUT_14;
	curTracks_ = OUT_14;


	globalBtn_.init("G");
	networkBtn_.init("N");
	plugInBtn_.init("P");
	recBtn_.init("R");
	addAndMakeVisible(globalBtn_);
	addAndMakeVisible(networkBtn_);
	addAndMakeVisible(plugInBtn_);
	addAndMakeVisible(recBtn_);

	buttons_[B_A_1].init("Solo");
	buttons_[B_A_2].init("Solo");
	buttons_[B_A_3].init("Solo");
	buttons_[B_A_4].init("Solo");
	buttons_[B_SHIFTL].init("IN14");
	buttons_[B_UP].init("OUT");
	buttons_[B_SHIFTR].init("IN58");

	buttons_[B_B_1].init("Mute");
	buttons_[B_B_2].init("Mute");
	buttons_[B_B_3].init("Mute");
	buttons_[B_B_4].init("Mute");
	buttons_[B_LEFT].init("-EN");
	buttons_[B_DOWN].init("MODE");
	buttons_[B_RIGHT].init("+EN");

	for (unsigned i = 0; i < 4; i++) {
		params_[i].init("init");
		addAndMakeVisible(params_[i]);
	}

	inTracks_[0].init(&params_[0], "In 1", &processor_.inputTrack(0));
	inTracks_[1].init(&params_[1], "In 2", &processor_.inputTrack(1));
	inTracks_[2].init(&params_[2], "In 3", &processor_.inputTrack(2));
	inTracks_[3].init(&params_[3], "In 4", &processor_.inputTrack(3));
	inTracks_[4].init(&params_[0], "In 5", &processor_.inputTrack(4));
	inTracks_[5].init(&params_[1], "In 6", &processor_.inputTrack(5));
	inTracks_[6].init(&params_[2], "In 7", &processor_.inputTrack(6));
	inTracks_[7].init(&params_[3], "In 8", &processor_.inputTrack(7));
	for (unsigned i = 0; i < Pmix::I_MAX; i++) {
		inTracks_[i].active(false);
		addAndMakeVisible(inTracks_[i]);
	}

	outTracks_[0].init(&params_[0], "Main L", 	&processor_.outputTrack(0));
	outTracks_[1].init(nullptr, "Main R", 	&processor_.outputTrack(1));
	outTracks_[2].init(&params_[1], "Aux 1 L", 	&processor_.outputTrack(2));
	outTracks_[3].init(nullptr, "Aux 1 R", 	&processor_.outputTrack(3));
	outTracks_[4].init(&params_[2], "Aux 2 L", 	&processor_.outputTrack(4));
	outTracks_[5].init(nullptr, "Aux 2 R", 	&processor_.outputTrack(5));
	outTracks_[6].init(&params_[3], "Aux 3 L", 	&processor_.outputTrack(6));
	outTracks_[7].init(nullptr, "Aux 3 R", 	&processor_.outputTrack(7));

	for (unsigned i = 0; i < Pmix::O_MAX; i++) {
		outTracks_[i].active(false);
		addAndMakeVisible(outTracks_[i]);
	}



	butMode_ = SSPChannel::BM_SOLOMUTE;
	switch (butMode_) {
	case SSPChannel::BM_SOLOMUTE: {
		buttons_[B_A_1].label("Solo");
		buttons_[B_A_2].label("Solo");
		buttons_[B_A_3].label("Solo");
		buttons_[B_A_4].label("Solo");
		buttons_[B_B_1].label("Mute");
		buttons_[B_B_2].label("Mute");
		buttons_[B_B_3].label("Mute");
		buttons_[B_B_4].label("Mute");
	}
	break;
	case SSPChannel::BM_CUEAC: {
		buttons_[B_A_1].label("Cue");
		buttons_[B_A_2].label("Cue");
		buttons_[B_A_3].label("Cue");
		buttons_[B_A_4].label("Cue");
		buttons_[B_B_1].label("AC");
		buttons_[B_B_2].label("AC");
		buttons_[B_B_3].label("AC");
		buttons_[B_B_4].label("AC");
	}
	break;
	default: {
		buttons_[B_A_1].label("");
		buttons_[B_A_2].label("");
		buttons_[B_A_3].label("");
		buttons_[B_A_4].label("");
		buttons_[B_B_1].label("");
		buttons_[B_B_2].label("");
		buttons_[B_B_3].label("");
		buttons_[B_B_4].label("");
	}
	}

	for (unsigned i = 0; i < B_MAX; i++) {
		addAndMakeVisible(buttons_[i]);
	}


	trackSelect(activeTracks_, true);


	startTimer(POLL_TIME);
}

PmixEditor::~PmixEditor()
{
	processor_.removeListener(this);
	stopTimer();
}

void PmixEditor::timerCallback()
{
	for (unsigned i = 0; i < TS_MAX; i++) {
		if (buttonHeldCount_[i] > 0) buttonHeldCount_[i] -= POLL_TIME;
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

	switch (index) {
	case Percussa::sspEnc1:
		channelEncoder(0, value);
		break;
	case Percussa::sspEnc2:
		channelEncoder(1, value);
		break;
	case Percussa::sspEnc3:
		channelEncoder(2, value);
		break;
	case Percussa::sspEnc4:
		channelEncoder(3, value);
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
		buttons_[B_A_1].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(0, 0, value);
		}
		break;
	case Percussa::sspSw2:
		buttons_[B_A_2].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(1, 0, value);
		}
		break;
	case Percussa::sspSw3:
		buttons_[B_A_3].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(2, 0, value);
		}
		break;
	case Percussa::sspSw4:
		buttons_[B_A_4].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(3, 0, value);
		}
		break;
	case Percussa::sspSw5:
		buttons_[B_B_1].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(0, 1, value);
		}
		break;
	case Percussa::sspSw6:
		buttons_[B_B_2].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(1, 1, value);
		}
		break;
	case Percussa::sspSw7:
		buttons_[B_B_3].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(2, 1, value);
		}
		break;
	case Percussa::sspSw8:
		buttons_[B_B_4].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			channelButton(3, 1, value);
		}
		break;
	case Percussa::sspSwLeft:
		buttons_[B_LEFT].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			encMode_ = encMode_ > 0 ? encMode_ - 1 : (SSPChannel::EM_MAX - 1) ;
			auto m = static_cast<SSPChannel::EncMode>(encMode_);
			for (unsigned i = 0; i < Pmix::I_MAX ; i++) {
				inTracks_[i].encoderMode(m);
			}
			for (unsigned i = 0; i < Pmix::O_MAX ; i++) {
				outTracks_[i].encoderMode(m);
			}
		}
		break;
	case Percussa::sspSwRight:
		buttons_[B_RIGHT].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			encMode_ = encMode_ <= (SSPChannel::EM_MAX - 1) ?  encMode_ + 1 : 0 ;
			auto m = static_cast<SSPChannel::EncMode>(encMode_);
			for (unsigned i = 0; i < Pmix::I_MAX ; i++) {
				inTracks_[i].encoderMode(m);
			}
			for (unsigned i = 0; i < Pmix::O_MAX ; i++) {
				outTracks_[i].encoderMode(m);
			}
		}
		break;
	case Percussa::sspSwUp:
		if (paramState_[index] != value) {
			trackSelect(OUT_14, value > 0.5);
		}
		break;
	case Percussa::sspSwDown:
		buttons_[B_DOWN].active(value > 0.5);
		if (paramState_[index] != value && !value) {
			butMode_ = butMode_ <= (SSPChannel::BM_MAX - 1) ? butMode_+1 : 0 ;
			auto m = static_cast<SSPChannel::ButMode>(butMode_);
			for (unsigned i = 0; i < Pmix::I_MAX ; i++) {
				inTracks_[i].buttonMode(m);
			}
			for (unsigned i = 0; i < Pmix::O_MAX ; i++) {
				outTracks_[i].buttonMode(m);
			}
			switch (butMode_) {
			case SSPChannel::BM_SOLOMUTE: {
				buttons_[B_A_1].label("Solo");
				buttons_[B_A_2].label("Solo");
				buttons_[B_A_3].label("Solo");
				buttons_[B_A_4].label("Solo");
				buttons_[B_B_1].label("Mute");
				buttons_[B_B_2].label("Mute");
				buttons_[B_B_3].label("Mute");
				buttons_[B_B_4].label("Mute");
			}
			break;
			case SSPChannel::BM_CUEAC: {
				buttons_[B_A_1].label("Cue");
				buttons_[B_A_2].label("Cue");
				buttons_[B_A_3].label("Cue");
				buttons_[B_A_4].label("Cue");
				buttons_[B_B_1].label("AC");
				buttons_[B_B_2].label("AC");
				buttons_[B_B_3].label("AC");
				buttons_[B_B_4].label("AC");
			}
			break;
			default: {
				buttons_[B_A_1].label("");
				buttons_[B_A_2].label("");
				buttons_[B_A_3].label("");
				buttons_[B_A_4].label("");
				buttons_[B_B_1].label("");
				buttons_[B_B_2].label("");
				buttons_[B_B_3].label("");
				buttons_[B_B_4].label("");
			}
			}
		}
		break;
	case Percussa::sspSwShiftL:
		if (paramState_[index] != value) {
			trackSelect(IN_14, value > 0.5);
		}
		break;
	case Percussa::sspSwShiftR:
		if (paramState_[index] != value) {
			trackSelect(IN_58, value > 0.5);
		}
		break;
	default:
		break;
	}

	paramState_[index] = value;
}


void PmixEditor::channelEncoder(unsigned c, float v) {
	switch (curTracks_) {
	case IN_14: {
		inTracks_[c].encoder(v);
		break;
	}
	case IN_58: {
		inTracks_[c + 4].encoder(v);
		break;
	}
	case OUT_14: {
		outTracks_[c * 2].encoder(v);
		outTracks_[c * 2 + 1].encoder(v);
		break;
	}
	default: break;
	}
}


void PmixEditor::channelButton(unsigned c, unsigned i, bool v) {
	switch (curTracks_) {
	case IN_14: {
		inTracks_[c].button(i, v);
		break;
	}
	case IN_58: {
		inTracks_[c + 4].button(i, v);
		break;
	}
	case OUT_14: {
		outTracks_[c * 2].button(i, v);
		outTracks_[c * 2 + 1].button(i, v);
		break;
	}
	default: break;
	}
}

bool PmixEditor::buttonState(unsigned c, unsigned i) {
	switch (curTracks_) {
	case IN_14: {
		return inTracks_[c].button(i);
	}
	case IN_58: {
		return inTracks_[c + 4].button(i);
	}
	case OUT_14: {
		return outTracks_[c * 2].button(i) || outTracks_[c * 2  + 1].button(i);
	}
	default: break;
	}

	return false;
}


void PmixEditor::trackSelect(TrackSelect ts, bool active) {
	if (active) {
		buttonHeldCount_[ts] = BUT_COUNTER;
		curTracks_ = ts;
	}
	else {
		if (buttonHeldCount_[ts] > 0) {
			activeTracks_ = ts;
		}
		curTracks_ = activeTracks_;
		buttonHeldCount_[ts] = 0;
	}

	buttons_[B_SHIFTL].active(curTracks_ == IN_14);
	buttons_[B_UP].active(curTracks_ == OUT_14);
	buttons_[B_SHIFTR].active(curTracks_ == IN_58);

	for (unsigned i = 0; i < Pmix::I_MAX ; i++) {
		inTracks_[i].active( (curTracks_ == IN_14 && i < 5 ) || (curTracks_ == IN_58 && i > 4) ) ;
	}

	for (unsigned i = 0; i < Pmix::O_MAX ; i++) {
		outTracks_[i].active(curTracks_ == OUT_14);
	}
}

void PmixEditor::drawMenuBox(Graphics & g) {
	unsigned y = menuTopY - 1;
	unsigned x = 1530 - 1;
	unsigned butTopY = paramTopY;
	g.setColour(Colours::grey);
	g.drawVerticalLine(x, y, butTopY);
	g.drawVerticalLine(1600 - 1, y, butTopY);
	for (unsigned i = 0; i < 5; i++) {
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
	for (unsigned i = 0; i < 8; i++ ) {
		g.drawVerticalLine(x + i * 100 , butTopY, 480 - 1);
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
	// g.drawSingleLineText("preset : " + String(processor_.getCurrentProgram()), 1400, 30);

	drawMenuBox(g);
	drawParamBox(g);

	switch (butMode_) {
	case SSPChannel::BM_SOLOMUTE: {
		switch(curTracks_) {
			case IN_14: {
				for(unsigned i=0;i<4;i++) buttons_[B_A_1+i].active(processor_.inputTrack(i).solo_);
				for(unsigned i=0;i<4;i++) buttons_[B_B_1+i].active(processor_.inputTrack(i).mute_);
				break;
			}
			case IN_58: {
				for(unsigned i=0;i<4;i++) buttons_[B_A_1+i].active(processor_.inputTrack(4+i).solo_);
				for(unsigned i=0;i<4;i++) buttons_[B_B_1+i].active(processor_.inputTrack(2+i).mute_);
				break;
			}
			case OUT_14: {
				for(unsigned i =0;i<4;i++) buttons_[B_A_1+i].active(processor_.outputTrack(i).solo_);
				for(unsigned i=0;i<4;i++) buttons_[B_B_1+i].active(processor_.outputTrack(i).mute_);
				break;
			}
			default:
				;
		}
	}
	break;
	case SSPChannel::BM_CUEAC: {
		switch(curTracks_) {
			case IN_14: {
				for(unsigned i=0;i<4;i++) buttons_[B_A_1+i].active(processor_.inputTrack(i).cue_);
				for(unsigned i=0;i<4;i++) buttons_[B_B_1+i].active(processor_.inputTrack(i).ac_);
				break;
			}
			case IN_58: {
				for(unsigned i=0;i<4;i++) buttons_[B_A_1+i].active(processor_.inputTrack(4+i).cue_);
				for(unsigned i=0;i<4;i++) buttons_[B_B_1+i].active(processor_.inputTrack(2+i).ac_);
				break;
			}
			case OUT_14: {
				for(unsigned i =0;i<4;i++) buttons_[B_A_1+i].active(processor_.outputTrack(i).cue_);
				for(unsigned i=0;i<4;i++) buttons_[B_B_1+i].active(processor_.outputTrack(i).ac_);
				break;
			}
			default:
				;
		}
	}
	break;

	default:
	break;
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

	setButtonBounds(buttons_[B_A_1],	0, 0);
	setButtonBounds(buttons_[B_A_2],	0, 1);
	setButtonBounds(buttons_[B_A_3],	0, 2);
	setButtonBounds(buttons_[B_A_4],	0, 3);
	setButtonBounds(buttons_[B_SHIFTL],	0, 4);
	setButtonBounds(buttons_[B_UP], 	0, 5);
	setButtonBounds(buttons_[B_SHIFTR],	0, 6);

	setButtonBounds(buttons_[B_B_1],	1, 0);
	setButtonBounds(buttons_[B_B_2],	1, 1);
	setButtonBounds(buttons_[B_B_3],	1, 2);
	setButtonBounds(buttons_[B_B_4],	1, 3);
	setButtonBounds(buttons_[B_LEFT], 	1, 4);
	setButtonBounds(buttons_[B_DOWN], 	1, 5);
	setButtonBounds(buttons_[B_RIGHT], 	1, 6);
	// setButtonBounds(buttons_[B_HELP], 	1, 0);
	// setButtonBounds(buttons_[B_WRITE_PR], 1, 2);

	// params
	for (unsigned i = 0; i < 4; i++) {
		setParamBounds(params_[i], i, 0);
	}


	const unsigned space = 5;
	const unsigned inStart = space;
	const unsigned inw = 90;
	const unsigned h = 325;
	const unsigned y = 50;

	const unsigned outStart = 900;
	const unsigned outw = 70;

	for (unsigned i = 0; i < Pmix::I_MAX; i++) {
		unsigned x = inStart + (i * (space + inw));
		inTracks_[i].setBounds(x, y, inw, h);
	}

	for (unsigned i = 0; i < Pmix::O_MAX; i++) {
		unsigned x = outStart + (i * (space + outw));
		outTracks_[i].setBounds(x, y, outw, h);
	}


}
