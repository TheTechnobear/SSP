
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h" 


Rngs::Rngs()
{
    memset(params_,sizeof(params_),0);
}

Rngs::~Rngs()
{
}

const String Rngs::getName() const
{
    return JucePlugin_Name;
}

int Rngs::getNumParameters()
{
    return Percussa::sspLast;
}

float Rngs::getParameter (int index)
{
    if(index<Percussa::sspLast) return params_[index];
    return 0.0f;
}

void Rngs::setParameter (int index, float newValue)
{
	//Logger::writeToLog(String::formatted("Rngs::setParameter: index=%d newValue=%4.2f", index, newValue)); 

	// see Percussa.h for more info about the parameters below 

	// The SSP's software does not provide access to your 
	// parameters from outside your plugin code, 
	// so your plugin's editor (GUI) is what is used to 
	// see and change parameters using the SSP's hardware 
	// controls (provided directly to you via the VST parameter
	// interface here). 

	// Calls to the setParameter function of your VST plugin 
	// might be done from the ssp's software audio callback 
	// which means you cannot do any gui, file or other time 
	// consuming operations in your setParameter function. 

	// See the JUCE documentation and examples for 
	// AsyncUpdater, ChangeBroadcaster ... for suggestions 
	// on triggering updates on a diferent thread from within 
	// setParameter(). 

    if(index<Percussa::sspLast) params_[index]=newValue;
    AudioProcessor::sendParamChangeMessageToListeners(index,newValue);
}

const String Rngs::getParameterName (int index)
{
    return String();
}

const String Rngs::getParameterText (int index)
{
    return String();
}

const String Rngs::getInputChannelName (int channelIndex) const
{
    switch(channelIndex) {
        case I_IN:          { return String("In");} 
        case I_STRUM:       { return String("Strum");}
        case I_VOCT:        { return String("Pitch");}
        case I_FM:          { return String("FM");}
        case I_STUCTURE:    { return String("Structure");}
        case I_BRIGHTNESS:  { return String("Brighness");}
        case I_DAMPING:      { return String("Damping");}
        case I_POSITION:      { return String("Position");}
    }
    return String("Uknown:")+String (channelIndex + 1);
}

const String Rngs::getOutputChannelName (int channelIndex) const
{

    switch(channelIndex) {
        case O_ODD : {
            return String("Odd");
        }
        case O_EVEN : {
            return String("Even");
        }
    }
    return String("Uknown:")+String (channelIndex + 1);
}

bool Rngs::isInputChannelStereoPair (int index) const
{
    return false;
}

bool Rngs::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool Rngs::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Rngs::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Rngs::silenceInProducesSilenceOut() const
{
    return false;
}

double Rngs::getTailLengthSeconds() const
{
    return 0.0;
}

int Rngs::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Rngs::getCurrentProgram()
{
    return currentProgram_;
}

void Rngs::setCurrentProgram (int index)
{
	// see below (functions for storing/recalling state info) 
	// for more info about what the SSP's software stores and recalls for your plugin 
    currentProgram_=index;
}

const String Rngs::getProgramName (int index)
{
    String pn= String("program")+String(index);
    return pn;
}

void Rngs::changeProgramName (int index, const String& newName)
{
	// The SSP's software does not provide for changing program 
	// names so this is something you need to do (if you need it) 
	// in your own plugin GUI. 
}

void Rngs::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	// this is called by the SSP's software right after loading 
	// the plugin and before it starts calling processBlock below  


    data_.strummer.Init(0.01f, sampleRate / samplesPerBlock);
    data_.string_synth.Init(data_.buffer);
    data_.part.Init(data_.buffer);

    if (samplesPerBlock > data_.iobufsz) {
        // note: in practice this will never happen !
        delete [] data_.in;
        delete [] data_.out;
        delete [] data_.aux;
        data_.iobufsz = samplesPerBlock;
        data_.in = new float[data_.iobufsz];
        data_.out = new float[data_.iobufsz];
        data_.aux = new float[data_.iobufsz];
        memset(data_.in,0,samplesPerBlock);
        memset(data_.out,0,samplesPerBlock);
        memset(data_.aux,0,samplesPerBlock);
    }

}

void Rngs::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void Rngs::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// plugin is 8ch in, 8ch out 
	// the ssp's software treats modulation and audio 
	// signals equally in its patcher matrix 
	// so what you would typically do below is use one or more 
	// incoming signals to change internal parameters 
	// (e.g. an incoming signal can change the frequency of an oscillator) 
	// if you don't want to do audio rate modulation you'd process the 
	// changes at a lower control rate. 

//    return;	


    auto& in=data_.in;
    auto& out=data_.out;
    auto& aux=data_.aux;
    auto n=buffer.getNumSamples();
    size_t size = n;

    bool strum=false;

    for (int i = 0; i < n; i++) {
        in[i] = out[i] = buffer.getSample(I_IN, i);
        float trig = buffer.getSample(I_STRUM,i);
        if(trig!=data_.f_trig && trig) {
            strum=true;
        }
        data_.f_trig=trig;
    }

    if (n > data_.iobufsz) {
        // note: in practice this will never happen !
        delete [] in;
        delete [] out;
        delete [] aux;
        data_.iobufsz = n;
        in = new float[data_.iobufsz];
        out = new float[data_.iobufsz];
        aux = new float[data_.iobufsz];
        memset(in,0,n);
        memset(out,0,n);
        memset(aux,0,n);
        data_.iobufsz = n;
    }

    // control rate 
    float pitch = data_.f_pitch + buffer.getSample(I_VOCT,0);
    float fm = data_.f_fm + buffer.getSample(I_FM,0) * 48.0f;
    float damping = data_.f_damping + buffer.getSample(I_DAMPING,0);
    float structure = data_.f_structure + buffer.getSample(I_STUCTURE,0);
    float brightness = data_.f_brightness + buffer.getSample(I_BRIGHTNESS,0);
    float position = data_.f_position + buffer.getSample(I_POSITION,0);


    auto& patch=data_.patch;
    auto& part=data_.part;
    auto& performance_state=data_.performance_state;

    patch.brightness = constrain(brightness, 0.0f, 1.0f);
    patch.damping = constrain(damping, 0.0f, 1.0f);
    patch.position = constrain(position, 0.0f, 1.0f);
    patch.structure = constrain(structure, 0.0f, 0.9995f);
    
    performance_state.fm = constrain(fm, -48.0f, 48.0f);
    performance_state.note = pitch;



    performance_state.tonic = 12.0f + data_.f_transpose; //todo ??

    //todo
    performance_state.internal_exciter = data_.f_internal_exciter > 0.5;
    performance_state.internal_strum =  data_.f_internal_strum > 0.5;
    performance_state.internal_note =  data_.f_internal_note > 0.5;
    performance_state.chord = data_.patch.structure * constrain(data_.f_chord, 0, rings::kNumChords - 1);



    performance_state.strum = !strum;


    int f_polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);

    if(f_polyphony != part.polyphony()) {
        part.set_polyphony(f_polyphony);
        data_.string_synth.set_polyphony(f_polyphony);
    }

    data_.f_model=constrain(data_.f_model,0,rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
    rings::ResonatorModel model = static_cast<rings::ResonatorModel>((int) data_.f_model);
    if(model != part.model()) {
        part.set_model(model);
        data_.string_synth.set_fx(static_cast<rings::FxType>(model));
    }


    part.set_bypass(data_.f_bypass > 0.5);

    if (data_.f_easter_egg > 0.5) {
        // todo?
        // for (size_t i = 0; i < size; ++i) {
        //     in[i] = in[i];
        // }
        data_.strummer.Process(NULL, size, &(data_.performance_state));
        data_.string_synth.Process(data_.performance_state, data_.patch, in, out, aux, size);
    } else {
        // Apply noise gate.
        for (size_t i = 0; i < size; ++i) {
            float in_sample = in[i];
            float error, gain;
            error = in_sample * in_sample - data_.in_level;
            data_.in_level += error * (error > 0.0f ? 0.1f : 0.0001f);
            gain = data_.in_level <= data_.kNoiseGateThreshold
                   ? (1.0f / data_.kNoiseGateThreshold) * data_.in_level : 1.0f;
            in[i] = gain * in_sample;
        }
        data_.strummer.Process(in, size, &(data_.performance_state));
        part.Process(performance_state, patch, in, out, aux, size);
    }

    for (int i=0; i<buffer.getNumSamples(); i++) {
        buffer.setSample(O_ODD,i,out[i]); 
        buffer.setSample(O_EVEN,i,aux[i]); 
    }
}

bool Rngs::hasEditor() const
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

AudioProcessorEditor* Rngs::createEditor()
{
    return new RngsEditor (*this);
}

void Rngs::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	// the ssp's software currently does not store any info in its presets 
	// about plugins, only the plugin file and program number 
	// this means that this function won't be called. 
}

void Rngs::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	// the ssp's software currently does not store any info in its presets 
	// about plugins, only the plugin file and program number 
	// this means that this function won't be called. 

	// this means you have to provide your own way of storing your VST plugin 
	// presets as files and recalling them. The SSP's software will call get/set 
	// program and related above functions to let the user choose programs in the 
	// VST module UI. 
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Rngs();
}
