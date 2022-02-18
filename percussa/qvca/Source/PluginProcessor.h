/*
	Copyright (c) 2022 - Bert Schiettecatte, Noisetron LLC. 

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

#include <array>
#include <string> 

class QVCA: public AudioProcessor, public Percussa::SSP::PluginInterface  
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

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    //////////////////////////////////////////////////////////////////////
	// SSP specific plugin interface overrides (see Percussa.h) 

	Percussa::SSP::PluginEditorInterface* editorInterface = nullptr;
	Percussa::SSP::PluginEditorInterface* getEditor() override ; 

	void getState(void** buffer, size_t* size) override {

		// before returning, write a pointer to state data, 
		// and state data size into *buffer and *size respectively. 
		// the host will copy the data using this pointer and size.

		// allocate new memory block. the previous one will be 
		// deallocated (scoped ptr). 
		stateInfo = new MemoryBlock();  
		getStateInformation(*stateInfo); 

		// pointer written into *buffer needs to remain valid after 
		// returning from this function. this is the reason that 
		// stateInfo is of type ScopedPointer<MemoryBlock>. 
		*buffer = stateInfo->getData(); 
		*size = stateInfo->getSize(); 
	} 

	void setState(void* buffer, size_t size) override {
		setStateInformation(buffer, size); 	
	} 

	void setParam(int paramNr, float val) override {
		setParameter(paramNr, val); 
	} 

	void prepare(double sampleRate, int estimatedSamplesPerBlock) override {
		// necessary for AudioProcessor internals, this function is
		// called before prepareToPlay is called, by the juce 
		// VST wrapper code.  
		setPlayConfigDetails(
            I_MAX,
			O_MAX,
			sampleRate, 
			estimatedSamplesPerBlock); 

		prepareToPlay (sampleRate, estimatedSamplesPerBlock);
	}

	void process(float** channelData, int numChannels, int numSamples) override {
		MidiBuffer midiBuffer; 		
		AudioSampleBuffer buffer(channelData, numChannels, numSamples); 
		processBlock(buffer, midiBuffer); 
	}

    static BusesProperties getBusesProperties() {
        BusesProperties props;
        for (auto i = 0; i < I_MAX; i++) {
            props.addBus(true, getInputBusName(i), AudioChannelSet::mono());
        }
        for (auto i = 0; i < O_MAX; i++) {
            props.addBus(false, getOutputBusName(i), AudioChannelSet::mono());
        }
        return props;
    }

public: 
    static constexpr unsigned I_MAX=8;
    static constexpr unsigned O_MAX=8;

	CriticalSection lock;
	bool prepared = false; 
	AudioSampleBuffer inBuffer; 
	AudioSampleBuffer outBuffer; 
	static const std::vector<std::string> inputNames;
	static const std::vector<std::string> outputNames;
	ScopedPointer<MemoryBlock> stateInfo; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QVCA)
};

extern "C" {
	Percussa::SSP::PluginDescriptor* createDescriptor();
	Percussa::SSP::PluginInterface* createInstance();
}

#endif 
 
