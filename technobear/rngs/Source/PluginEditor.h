#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

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
	void 	audioProcessorParameterChanged (AudioProcessor *processor, int parameterIndex, float newValue) override;
	void 	audioProcessorChanged (AudioProcessor *processor) override;

protected:
	void paintBack (Graphics&);
	void paintParams (Graphics&);
	void displayParameter(Graphics& g, unsigned idx, bool alt, const String& label, float value, const String& unit);
	void displayMainMenu(Graphics&g, const String& label, unsigned idx,  bool active );
	void displayButton(Graphics&g, const String& label, unsigned r, unsigned c, bool active );
	void drawRngs(Graphics& g);

private:
	Rngs& processor;
	bool altActive_ = false;
	bool paramState_[Percussa::sspLast];
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RngsEditor)
};

