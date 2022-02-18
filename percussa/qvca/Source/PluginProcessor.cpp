// see header file for license 

#include <iostream> 
#include <cstring> 
#include <stdexcept> 

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"

const std::vector<std::string> QVCA::inputNames = {
    "In1",  "In2",  "In3",  "In4",  "In5",  "In6",  "In7",  "In8" };
const std::vector<std::string> QVCA::outputNames = {
    "Out1", "Out2", "Out3", "Out4", "Out5", "Out6", "Out7", "Out8" };

QVCA::QVCA() : AudioProcessor(getBusesProperties()) {
}

QVCA::~QVCA() {
}

const String QVCA::getInputBusName(int channelIndex) {
    if (channelIndex < I_MAX) { return inputNames[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String QVCA::getOutputBusName(int channelIndex) {
    if (channelIndex < O_MAX) { return outputNames[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}



const String QVCA::getName() const
{
    return JucePlugin_Name;
}

int QVCA::getNumParameters()
{
    return 0;
}

float QVCA::getParameter (int index)
{
    return 0.00f; 
}

void QVCA::setParameter (int index, float newValue)
{
}

const String QVCA::getParameterName (int index)
{
    return String();
}

const String QVCA::getParameterText (int index)
{
    return String();
}

const String QVCA::getInputChannelName (int channelIndex) const
{
    return getInputBusName(channelIndex);
}

const String QVCA::getOutputChannelName (int channelIndex) const
{
    return getOutputBusName(channelIndex);
}

bool QVCA::isInputChannelStereoPair (int index) const
{
    return false;
}

bool QVCA::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool QVCA::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool QVCA::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool QVCA::silenceInProducesSilenceOut() const
{
    return false;
}

double QVCA::getTailLengthSeconds() const
{
    return 0.0;
}

int QVCA::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int QVCA::getCurrentProgram()
{
    return 0;
}

void QVCA::setCurrentProgram (int index)
{
	// Program numbers are not stored or recalled from SSP preset files and there is no way 
	// to change a program from within the SSP software. The SSP software which hosts your 
	// plugin will ONLY store and recall state using the functions below. 
}

const String QVCA::getProgramName (int index)
{
    return String("default");
}

void QVCA::changeProgramName (int index, const String& newName)
{
	// The SSP's software does not provide for changing program names so this is something 
	// you need to do (if you need it) in your own plugin GUI. Keep in mind the SSP software
	// does not set, store or recall program numbers and there is no way for you to change
	// a program number from within the SSP software.  
}

void QVCA::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback initialisation that you need..
	const ScopedLock sl(lock); 

	// allocate space in the input/output buffers for visualisation here, to make sure 
	// processBlock() does not do any allocations. make sure buffers are cleared at 
	// the same time (clearExtraSpace) 
	inBuffer.setSize(I_MAX, samplesPerBlock, false, true, false);
	outBuffer.setSize(O_MAX, samplesPerBlock, false, true, false);

	prepared = true; 
}

void QVCA::releaseResources()
{
	// when playback stops, you can use this as an opportunity to free up any memory. 
}

void QVCA::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// the QVCA plugin has 8 input and 8 output channels. 
	// the ssp's software treats modulation and audio signals equally in its patcher matrix 
	// so what you would typically do below is use one or more incoming signals to change 
	// internal parameters (e.g. an incoming signal can change the frequency of an oscillator) 
	// if you don't want to do audio rate modulation you'd process the changes at a lower 
	// control rate. 	

	assert(prepared);
//	assert(inBuffer.getNumChannels() == inputNames.size());
//	assert(outBuffer.getNumChannels() == outputNames.size());

	// try to get lock and copy input buffer  
	if (lock.tryEnter()) {  
		for (int ch=0; ch<I_MAX; ch++)
			inBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples()); 
		lock.exit(); 
	}

	// process signals 
	for (int i=0; i<buffer.getNumSamples(); i++) { 

		// multiply side by side channels 	
		float out1=buffer.getSample(0, i)*buffer.getSample(1, i); 
		float out2=buffer.getSample(2, i)*buffer.getSample(3, i); 
		float out3=buffer.getSample(4, i)*buffer.getSample(5, i); 
		float out4=buffer.getSample(6, i)*buffer.getSample(7, i); 

		// output multiplied signals and also output their inverted version 
		buffer.setSample(0, i,  out1); 
		buffer.setSample(1, i, -out1); 
		buffer.setSample(2, i,  out2); 
		buffer.setSample(3, i, -out2); 
		buffer.setSample(4, i,  out3); 
		buffer.setSample(5, i, -out3); 
		buffer.setSample(6, i,  out4); 
		buffer.setSample(7, i, -out4); 
	}

	// try to get lock and copy output buffer  
	if (lock.tryEnter()) { 
		for (int ch=0; ch<O_MAX; ch++)
			outBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples()); 
		lock.exit();
	}

}

bool QVCA::hasEditor() const
{
	return true; 
}

AudioProcessorEditor* QVCA::createEditor()
{
	return new QVCAEditor (*this); 
}

void QVCA::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	// IMPORTANT: as a plugin developer you are responsible for resizing the 
	// memory block passed by reference to this function, befor writing into it.
	// the memory block passed has zero size initially when entering this function.   
	// calling append() on the memory block will automatically grow the block. 
}

void QVCA::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from the data buffer 
	// passed to this function, whose contents will have been created originally by the 
	// getStateInformation() call.
}

// called by the juce VST framework to instantiate the plugin. 
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new QVCA(); 
}

// called by the SSP plugin hosting code to create/get an editor for the plugin. 
Percussa::SSP::PluginEditorInterface* QVCA::getEditor() { 

	// editor is deallocated by caller. repeated calls
	// to this function, should return the previously 
	// allocated editor instance. 
	if (!editorInterface) { 
		editorInterface = new QVCAEditor(*this); 
	}

	return editorInterface; 
}

// called by the SSP plugin hosting code to create/get a descriptor for the plugin. 
extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor* createDescriptor() {

	// descriptor is deallocated by caller. 
	Percussa::SSP::PluginDescriptor* desc = 
		new Percussa::SSP::PluginDescriptor; 

	desc->name = JucePlugin_Name;
	desc->descriptiveName = JucePlugin_Desc;
	desc->manufacturerName = JucePlugin_Manufacturer;
    desc->version = JucePlugin_VersionString;
	desc->uid = (int)JucePlugin_VSTUniqueID;
	desc->inputChannelNames = QVCA::inputNames;
	desc->outputChannelNames = QVCA::outputNames;  

	return desc; 
}

// called by the SSP plugin hosting code to create/get an instance of the plugin. 
extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginInterface* createInstance() {

	// instance is deallocated by caller. 
	return new QVCA(); 
}

