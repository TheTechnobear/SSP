// see header file for license 

#include <cstring> 
#include <stdexcept> 

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h" 

QVCA::QVCA(): chnames({
	"a", "b", "c", "d", "e", "f", "g", "h", 
	"i", "j", "k", "l", "m", "n", "o", "p", 
	"q", "r", "s", "t", "u", "v", "w", "x", 
}) 
{
	std::memset(paramValues, 0, sizeof(paramValues)); 
}

QVCA::~QVCA()
{
}

const String QVCA::getName() const
{
    return JucePlugin_Name;
}

int QVCA::getNumParameters()
{
    return numParams; 
}

float QVCA::getParameter (int index)
{
	if (index < Percussa::sspFirst) return 0.00f; 
	if (index >= Percussa::sspLast) return 0.00f; 

	return paramValues[index-Percussa::sspFirst];
}

void QVCA::setParameter (int index, float newValue)
{
	// The SSP's software uses VST parameters to pass the state 
	// of its hardware buttons and encoders to your plugin. 
	// See Percussa.h, these parameters begin at sspFirst and 
	// end at sspLast. There are also special parameters passed
	// to indicate which of the inputs and outputs of the 
	// VST module (which hosts the plugin) are enabled. 
	// You can add your own VST parameters starting at sspLast, 
	// if you wish, so your plugin can also be used with desktop
	// DAWs. 

	// Calls to the setParameter function of your VST plugin 
	// might be done from the ssp's software audio callback 
	// which means you cannot do any gui, file or other time 
	// consuming operations in your setParameter function. 

	// See the JUCE documentation and examples for 
	// AsyncUpdater, ChangeBroadcaster ... for suggestions 
	// on triggering updates on a diferent thread from within 
	// setParameter(). 

	//Logger::writeToLog(String::formatted(
	//	"QVCA::setParameter: index=%d newValue=%4.2f", 
	//		index, newValue)); 

	if (index < Percussa::sspFirst) return; 
	if (index >= Percussa::sspLast) return; 

	switch(index) { 
		case Percussa::sspEnc1:
		case Percussa::sspEnc2: 
		case Percussa::sspEnc3:
		case Percussa::sspEnc4: 
		{
			if (newValue > 0.5) {  
				paramValues[index-Percussa::sspFirst]++; 
			} else if (newValue < 0.5) { 
				paramValues[index-Percussa::sspFirst]--; 
			}
			break; 
		}

		case Percussa::sspEncSw1: 
		case Percussa::sspEncSw2: 
		case Percussa::sspEncSw3: 
		case Percussa::sspEncSw4: 
		case Percussa::sspSw1: 
		case Percussa::sspSw2: 
		case Percussa::sspSw3: 
		case Percussa::sspSw4: 
		case Percussa::sspSw5: 
		case Percussa::sspSw6: 
		case Percussa::sspSw7: 
		case Percussa::sspSw8: 
		case Percussa::sspSwLeft: 
		case Percussa::sspSwRight: 
		case Percussa::sspSwUp: 
		case Percussa::sspSwDown: 
		case Percussa::sspSwShiftL:
		case Percussa::sspSwShiftR: 

		case Percussa::sspOutEn1: 
		case Percussa::sspOutEn2: 
		case Percussa::sspOutEn3: 
		case Percussa::sspOutEn4: 
		case Percussa::sspOutEn5: 
		case Percussa::sspOutEn6: 
		case Percussa::sspOutEn7: 
		case Percussa::sspOutEn8: 
		case Percussa::sspOutEn9: 
		case Percussa::sspOutEn10: 
		case Percussa::sspOutEn11: 
		case Percussa::sspOutEn12: 
		case Percussa::sspOutEn13: 
		case Percussa::sspOutEn14: 
		case Percussa::sspOutEn15: 
		case Percussa::sspOutEn16: 
		case Percussa::sspOutEn17: 
		case Percussa::sspOutEn18: 
		case Percussa::sspOutEn19: 
		case Percussa::sspOutEn20: 
		case Percussa::sspOutEn21: 
		case Percussa::sspOutEn22: 
		case Percussa::sspOutEn23: 
		case Percussa::sspOutEn24: 

		case Percussa::sspInEn1: 
		case Percussa::sspInEn2: 
		case Percussa::sspInEn3: 
		case Percussa::sspInEn4: 
		case Percussa::sspInEn5: 
		case Percussa::sspInEn6: 
		case Percussa::sspInEn7: 
		case Percussa::sspInEn8: 
		case Percussa::sspInEn9: 
		case Percussa::sspInEn10: 
		case Percussa::sspInEn11: 
		case Percussa::sspInEn12: 
		case Percussa::sspInEn13: 
		case Percussa::sspInEn14: 
		case Percussa::sspInEn15: 
		case Percussa::sspInEn16: 
		case Percussa::sspInEn17: 
		case Percussa::sspInEn18: 
		case Percussa::sspInEn19: 
		case Percussa::sspInEn20: 
		case Percussa::sspInEn21: 
		case Percussa::sspInEn22: 
		case Percussa::sspInEn23: 
		case Percussa::sspInEn24: 

		default:
		{
			paramValues[index-Percussa::sspFirst] = newValue;  
			break; 
		}
	}
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
    return chnames[channelIndex]; 
}

const String QVCA::getOutputChannelName (int channelIndex) const
{
    return chnames[channelIndex]; 
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
	// The SSP software allows the user to switch programs on the P-page of the VST module. 
	// Program numbers are not stored or recalled from SSP preset files. It's up to you 
	// as a plugin developer how you store your presets. The SSP software will list all 
	// available programs on the P-page of the VST module. It will use the state get/set 
	// functions below to store and recall state of your plugin, when it writes/reads a 
	// SSP preset files.  
}

const String QVCA::getProgramName (int index)
{
    return String("default");
}

void QVCA::changeProgramName (int index, const String& newName)
{
	// The SSP's software does not provide for changing program 
	// names so this is something you need to do (if you need it) 
	// in your own plugin GUI. 
}

void QVCA::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	// this is called by the SSP's software right after loading 
	// the plugin and before it starts calling processBlock below  

	const ScopedLock sl(lock); 

	// allocate space in the input/output buffers for visualisation
	// here, to make sure processBlock() does not do any allocations. 
	// make sure buffers are cleared at the same time (clearExtraSpace) 
	inBuffer.setSize(getNumInputChannels(), samplesPerBlock, false, true, false); 
	outBuffer.setSize(getNumOutputChannels(), samplesPerBlock, false, true, false); 
}

void QVCA::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void QVCA::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// plugin is 8ch in, 8ch out 
	// the ssp's software treats modulation and audio 
	// signals equally in its patcher matrix 
	// so what you would typically do below is use one or more 
	// incoming signals to change internal parameters 
	// (e.g. an incoming signal can change the frequency of an oscillator) 
	// if you don't want to do audio rate modulation you'd process the 
	// changes at a lower control rate. 	

	// try to get lock and copy input buffer  
	if (lock.tryEnter()) {  
		for (int ch=0; ch<getNumInputChannels(); ch++) 
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
		for (int ch=0; ch<getNumOutputChannels(); ch++) 
			outBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples()); 
		lock.exit();
	} 
}

bool QVCA::hasEditor() const
{
	// (change this to false if you choose to not supply an editor)
	// please note that you pretty much need an editor at the moment 
	// because the ssp's software does not provide a macro user interface 
	// to expose VST parameters for editing, outside the VST's GUI.
	// the moment the VST GUI is brought to front all hardware controls 
	// are sent to the plugin as parameter changes (i.e. setParameter() 
	// calls). The SSP software will not get or set any parameters you 
	// might choose to implement starting from 0.  
 
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

	// The SSP software will call this function to capture the state of your plugin
	// and will store the state inside its preset files when saved. 

	// IMPORTANT: as a plugin developer you are responsible for resizing the 
	// memory block passed by reference to this function, befor writing into it.
	// the memory block passed has zero size initially when entering this function.   
	// calling append() on the memory block will automatically grow the block. 

	// write header consisting of plugin name and version string 
	destData.append(JucePlugin_Name, sizeof(JucePlugin_Name)); 
	destData.append(JucePlugin_VersionString, sizeof(JucePlugin_VersionString)); 

	// write parameter value array 
	destData.append(paramValues, sizeof(paramValues)); 

	// write footer
	destData.append(JucePlugin_Name, sizeof(JucePlugin_Name)); 
}

void QVCA::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	// The SSP software will call this function after reading the state from a preset
	// file, when the user loads a new preset. 

	// code below can probably use additional error handling and bounds checking, 
	// it's just here as a general example and not guaranteed to be reliable. 

	// read and compare header consisting of plugin name and version 
	const char* cdata = static_cast<const char*>(data);
	if (std::memcmp(cdata, JucePlugin_Name, sizeof(JucePlugin_Name)) != 0) 
		throw std::runtime_error("error reading plugin name"); 
	cdata += sizeof(JucePlugin_Name); 

	if (std::memcmp(cdata, JucePlugin_VersionString, sizeof(JucePlugin_VersionString)) != 0) 
		throw std::runtime_error("error reading plugin version string"); 
	cdata += sizeof(JucePlugin_VersionString); 

	// restore parameter values 
	std::memcpy(paramValues, cdata, sizeof(paramValues)); 	
	cdata += sizeof(paramValues); 

	// read and compare footer 
	if (std::memcmp(cdata, JucePlugin_Name, sizeof(JucePlugin_Name)) != 0) 
		throw std::runtime_error("error reading plugin name"); 
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new QVCA();
}
