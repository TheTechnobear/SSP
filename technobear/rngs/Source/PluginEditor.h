#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "SSPButton.h"
#include "SSPParam.h"

class RngsEditor  : public AudioProcessorEditor,
	public AudioProcessorListener,
	public Timer
{
public:
	RngsEditor (Rngs&);
	~RngsEditor();

	void paint (Graphics&) override;
	void resized() override;

	void parameterChanged(int parameterIndex, float newValue);

	void timerCallback();

	// audio thread!!
	void 	audioProcessorParameterChanged (AudioProcessor *processor_, int parameterIndex, float newValue) override;
	void 	audioProcessorChanged (AudioProcessor *processor_) override;

protected:
	void displayMainMenu(Graphics&g, const String& label, unsigned idx,  bool active );
	void drawRngs(Graphics& g);
	void drawHelp(Graphics& g);

	void setMenuBounds(SSPButton& btn, unsigned r);
	void setParamBounds(SSPParam& par, unsigned enc, unsigned var);
	void setButtonBounds(SSPButton& btn, unsigned r, unsigned c);

private:
	Rngs& processor_;
	bool altActive_ = false;
	bool paramState_[Percussa::sspLast];

	SSPButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;

	SSPButton audioBtn_, strumBtn_, vOctBtn_;
	SSPButton helpBtn_;
	SSPButton enPlus_, enMinus_;

	SSPParam pitchParam_, structParam_, brightParam_, dampParam_;
	SSPParam posParam_, polyParam_, modelParam_, nullParam_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RngsEditor)
};

