#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SSPButton.h"
#include "SSPParam.h"
#include "SSPChannel.h"

class PmixEditor  : public AudioProcessorEditor,
	public AudioProcessorListener,
	public Timer
{
public:
	PmixEditor (Pmix&);
	~PmixEditor();

	void paint (Graphics&) override;
	void resized() override;

	void parameterChanged(int parameterIndex, float newValue);

	void timerCallback();

	// audio thread!!
	void 	audioProcessorParameterChanged (AudioProcessor *processor_, int parameterIndex, float newValue) override;
	void 	audioProcessorChanged (AudioProcessor *processor_) override;

protected:

	void drawMenuBox(Graphics& g);
	void drawParamBox(Graphics& g);
	void drawPmix(Graphics& g);
	void drawHelp(Graphics& g);
	void drawEncoderValue(Graphics& g);

	void setMenuBounds(SSPButton& btn, unsigned r);
	void setParamBounds(SSPParam& par, unsigned enc, unsigned var);
	void setButtonBounds(SSPButton& btn, unsigned r, unsigned c);

private:
	Pmix& processor_;

	enum Params {
		// P_POS,
		P_MAX
	};

	enum Buttons {
		B_A_1,  	// mute
		B_A_2,
		B_A_3,
		B_A_4,

		B_SHIFTR, 	// in14
		B_UP, 		// out 
		B_SHIFTL, 	// in58

		B_B_1, 		// solo
		B_B_2,
		B_B_3,
		B_B_4,

		B_LEFT,
		B_DOWN,
		B_RIGHT,

		B_HELP,
		B_WRITE_PR,

		B_MAX
	};

	void channelEncoder(unsigned c, float v);
	void channelButtonA(unsigned c, bool v);
	void channelButtonB(unsigned c, bool v);
	bool buttonAState(unsigned c);
	bool buttonBState(unsigned c);

	SSPButton buttons_[B_MAX];
	SSPParam  params_[P_MAX];

	// display only
	SSPButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;

	SSPParam* 	activeParam_ = nullptr;
	unsigned 	activeEncIdx_ = 0;
	unsigned  	paramActive_ = 0;
	bool 		paramState_[Percussa::sspLast];

	enum TrackSelect {
		IN_14,
		IN_58,
		OUT_14
	} activeTracks_ ;


	// this includes non-channel modes!
    enum ButMode {
        BM_MUTESOLO,
        // BM_HPF,
        BM_MISC,
        BM_MAX
    } butMode_;

	SSPChannel inTracks_[Pmix::I_MAX];
	SSPChannel outTracks_[Pmix::O_MAX];

	static constexpr unsigned PARAM_COUNTER = 20;
	unsigned activeParamCount_ = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PmixEditor)
};

