#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SSPButton.h"
#include "SSPParam.h"

class Msw8Editor  : public AudioProcessorEditor,
	public AudioProcessorListener,
	public Timer
{
public:
	Msw8Editor (Msw8&);
	~Msw8Editor();

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
	void drawMsw8(Graphics& g);
	void drawEncoderValue(Graphics& g);

	void setMenuBounds(SSPButton& btn, unsigned r);
	void setParamBounds(SSPParam& par, unsigned enc, unsigned var);
	void setButtonBounds(SSPButton& btn, unsigned r, unsigned c);

private:
	Msw8& processor_;
	bool altActive_ = false;
	bool paramState_[Percussa::sspLast];

	SSPButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;

	enum {
		B_USE_ACTIVE,
		B_SOFT,
		B_MAX
	};

	SSPButton buttons_[B_MAX];

	enum {
		P_IN_SEL,
		P_OUT_SEL,
		P_MAX
	};

	SSPParam params_[P_MAX];


	SSPParam* 	activeParam_ = nullptr;
	unsigned 	activeParamIdx_ = 0;
	static constexpr unsigned PARAM_COUNTER = 20;
	unsigned activeParamCount_ = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Msw8Editor)
};

