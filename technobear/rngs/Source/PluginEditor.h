
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
// #include "Oscilloscope.h" 

class RngsEditor  : public AudioProcessorEditor, 
					public AudioProcessorListener,
					public Timer 
{
public:
	static const int keepout=100; 

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

private:
	Rngs& processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RngsEditor)
};

