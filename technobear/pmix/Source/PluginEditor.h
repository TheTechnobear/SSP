#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SSPButton.h"
#include "SSPParam.h"

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
		// B_MUTE_1,
		// B_MUTE_2,
		// B_MUTE_3,
		// B_MUTE_4,
		// B_IN_14
		B_UP, // B_OUT_14
		// B_IN_58

		// B_SOLO_1,
		// B_SOLO_2,
		// B_SOLO_3,
		// B_SOLO_4,
		B_DOWN,
		B_LEFT,
		B_RIGHT,

		B_HELP,
		B_WRITE_PR,

		B_MAX
	};

	void channelEncoder(unsigned c, float v);
	void channelButton(unsigned c, bool v);

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


	SSPChannel inTracks_[PluginProcessor::I_MAX];
	SSPChannel outTracks_[PluginProcessor::O_MAX];

	static constexpr unsigned PARAM_COUNTER = 20;
	unsigned activeParamCount_ = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PmixEditor)
};

