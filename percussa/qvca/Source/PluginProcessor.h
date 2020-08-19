/*
	Copyright (c) 2018 - Bert Schiettecatte, Noisetron LLC. 

	This software is part of the Percussa SSP's software development kit (SDK). 
	For more info about Percussa or the SSP visit http://www.percussa.com/ 
	and our forum at http://forum.percussa.com/ 

	Permission is granted to use this software under the terms of either:
	a) the GPL v2 (or any later version)
	b) the Affero GPL v3

	Details of these licenses can be found at: www.gnu.org/licenses

	This software is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h" 

class QVCA: public AudioProcessor
{
public:
	QVCA();
	~QVCA();

	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	const String getName() const override;

	int getNumParameters() override;
	float getParameter (int index) override;
	void setParameter (int index, float newValue) override;

	const String getParameterName (int index) override;
	const String getParameterText (int index) override;

	const String getInputChannelName (int channelIndex) const override;
	const String getOutputChannelName (int channelIndex) const override;
	bool isInputChannelStereoPair (int index) const override;
	bool isOutputChannelStereoPair (int index) const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool silenceInProducesSilenceOut() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram (int index) override;
	const String getProgramName (int index) override;
	void changeProgramName (int index, const String& newName) override;

	void getStateInformation (MemoryBlock& destData) override;
	void setStateInformation (const void* data, int sizeInBytes) override;

private:
	static const int numParams = Percussa::sspLast-Percussa::sspFirst; 
	float paramValues[numParams]; 

public: 
	CriticalSection lock;
	AudioSampleBuffer inBuffer; 
	AudioSampleBuffer outBuffer; 
	std::vector<String> chnames; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QVCA)
};

#endif 
 
