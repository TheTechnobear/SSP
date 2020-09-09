#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SSPButton.h"
#include "SSPParam.h"

class PluginEditor  : public AudioProcessorEditor,
	public AudioProcessorListener,
	public Timer
{
public:
	PluginEditor (PluginProcessor&);
	~PluginEditor();

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
	void drawIcon(Graphics& g);
	void drawEncoderValue(Graphics& g);

	void setMenuBounds(SSPButton& btn, unsigned r);
	void setParamBounds(SSPParam& par, unsigned enc, unsigned var);
	void setButtonBounds(SSPButton& btn, unsigned r, unsigned c);

private:
	PluginProcessor& processor_;

	enum Params {
		P_SCALE,
		P_ROOT,
		P_MAX
	};

	enum Buttons {
		// B_FREEZE,

		B_UP,
		B_DOWN,
		B_LEFT,
		B_RIGHT,

		B_MAX
	};

	SSPButton buttons_[B_MAX];
	SSPParam  params_[P_MAX];

	// display only
	SSPButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;

	SSPParam* 	activeParam_ = nullptr;
	unsigned 	activeEncIdx_ = 0;
	unsigned  	paramActive_ = 0;
	bool paramState_[Percussa::sspLast];

	static constexpr unsigned PARAM_COUNTER = 20;
	unsigned activeParamCount_ = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

