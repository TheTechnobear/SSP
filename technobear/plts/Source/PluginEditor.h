#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SSPButton.h"
#include "SSPParam.h"

class PltsEditor  : public AudioProcessorEditor,
	public AudioProcessorListener,
	public Timer
{
public:
	PltsEditor (Plts&);
	~PltsEditor();

	void paint (Graphics&) override;
	void resized() override;

	void parameterChanged(int parameterIndex, float newValue);

	void timerCallback() override;

	// audio thread!!
	void 	audioProcessorParameterChanged (AudioProcessor *processor_, int parameterIndex, float newValue) override;
	void 	audioProcessorChanged (AudioProcessor *processor_) override;

protected:

	void drawMenuBox(Graphics& g);
	void drawParamBox(Graphics& g);
	void drawPlts(Graphics& g);
	void drawEncoderValue(Graphics& g);

	void setMenuBounds(SSPButton& btn, unsigned r);
	void setParamBounds(SSPParam& par, unsigned enc, unsigned var);
	void setButtonBounds(SSPButton& btn, unsigned r, unsigned c);

private:
	Plts& processor_;
	bool paramState_[Percussa::sspLast];

	SSPButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;

	enum {
		B_UP,
		B_DOWN,
		B_LEFT,
		B_RIGHT,
		B_MAX
	};

	SSPButton buttons_[B_MAX];

	enum {
		P_PITCH,
		P_HARMONICS,
		P_TIMBRE,
		P_MORPH,

		P_MODEL,
		P_FREQ_MOD,
		P_TIMBRE_MOD,
		P_MORPH_MOD,

		P_LPG,
		P_VCA,

		P_MAX
	};

	SSPParam params_[P_MAX];


	SSPParam* 	activeParam_ = nullptr;
	unsigned 	activeEncIdx_ = 0;
	unsigned  	paramActive_ = 0;
	static constexpr unsigned PARAM_COUNTER = 20;
	unsigned activeParamCount_ = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PltsEditor)
};

