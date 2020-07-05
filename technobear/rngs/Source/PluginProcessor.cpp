
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"


Rngs::Rngs()
{
    memset(params_, 0, sizeof(params_));
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
    // SSP appears to only send parameters if they appear to have changed
    // so we need to track the parameters values(!)
    if (index < Percussa::sspLast) return params_[index];
    return 0.0f;
}

void Rngs::setParameter (int index, float newValue)
{
    // SSP currently sends control information as parameters
    // see Percussa.h for more info about the parameters below

    if (index < Percussa::sspLast) params_[index] = newValue;
    AudioProcessor::sendParamChangeMessageToListeners(index, newValue);
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
    switch (channelIndex) {
    case I_IN:          { return String("In");}
    case I_STRUM:       { return String("Strum");}
    case I_VOCT:        { return String("Pitch");}
    case I_FM:          { return String("FM");}
    case I_STUCTURE:    { return String("Structure");}
    case I_BRIGHTNESS:  { return String("Brighness");}
    case I_DAMPING:      { return String("Damping");}
    case I_POSITION:      { return String("Position");}
    }
    return String("Uknown:") + String (channelIndex + 1);
}

const String Rngs::getOutputChannelName (int channelIndex) const
{

    switch (channelIndex) {
    case O_ODD : {
        return String("Odd");
    }
    case O_EVEN : {
        return String("Even");
    }
    }
    return String("Uknown:") + String (channelIndex + 1);
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
    // NB: some hosts don't cope very well if you tell them there are 0 programs
    return 1;
}

int Rngs::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return currentProgram_;
}

void Rngs::setCurrentProgram (int index)
{
    // SSP calls when program being loaded
    currentProgram_ = index;
}

const String Rngs::getProgramName (int index)
{
    String pn = String("program") + String(index);
    return pn;
}

void Rngs::changeProgramName (int index, const String& newName)
{
    // SSP - not using
}

void Rngs::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // this is called by the SSP's software right after loading
    // the plugin and before it starts calling processBlock below


    data_.strummer.Init(0.01f, sampleRate / RingsBlock);
    data_.string_synth.Init(data_.buffer);
    data_.part.Init(data_.buffer);

    if (RingsBlock > data_.iobufsz) {
        delete [] data_.in;
        delete [] data_.out;
        delete [] data_.aux;
        data_.iobufsz = RingsBlock;
        data_.in = new float[data_.iobufsz];
        data_.out = new float[data_.iobufsz];
        data_.aux = new float[data_.iobufsz];
        memset(data_.in, 0, data_.iobufsz);
        memset(data_.out, 0, data_.iobufsz);
        memset(data_.aux, 0, data_.iobufsz);
    }
}

void Rngs::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Rngs::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    auto& in = data_.in;
    auto& out = data_.out;
    auto& aux = data_.aux;
    auto n = RingsBlock;
    size_t size = n;


    // Rings usually has a blocks size of 16,
    // SSP = 128 (@48k), so split up, so we read the control rate date every 16
    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {

        bool strum = false;
        for (int i = 0; i < n; i++) {
            in[i] = out[i] = buffer.getSample(I_IN, bidx + i);
            bool trig = buffer.getSample(I_STRUM, bidx + i) > 0.5;
            if (trig != data_.f_trig && trig) {
                strum = true;
            }
            data_.f_trig = trig;
        }

        // control rate
        float transpose = data_.f_frequency;
        float note = buffer.getSample(I_VOCT, bidx) * 60.0f;
        float fm = buffer.getSample(I_FM, bidx) * 48.0f;
        float damping = data_.f_damping + buffer.getSample(I_DAMPING, bidx);
        float structure = data_.f_structure + buffer.getSample(I_STUCTURE, bidx);
        float brightness = data_.f_brightness + buffer.getSample(I_BRIGHTNESS, bidx);
        float position = data_.f_position + buffer.getSample(I_POSITION, bidx);
        float chord = data_.patch.structure * rings::kNumChords - 1;

        auto& patch = data_.patch;
        auto& part = data_.part;
        auto& performance_state = data_.performance_state;

        patch.brightness = constrain(brightness, 0.0f, 1.0f);
        patch.damping = constrain(damping, 0.0f, 1.0f);
        patch.position = constrain(position, 0.0f, 1.0f);
        patch.structure = constrain(structure, 0.0f, 0.9995f);

        performance_state.fm = constrain(fm, -48.0f, 48.0f);
        performance_state.note = note;
        performance_state.tonic = 12.0f + transpose;

        performance_state.internal_exciter = data_.f_internal_exciter > 0.5;
        performance_state.internal_strum =  data_.f_internal_strum > 0.5;
        performance_state.internal_note =  data_.f_internal_note > 0.5;
        performance_state.chord = constrain(chord , 0, rings::kNumChords - 1);;

        if (performance_state.internal_note) {

            performance_state.note = 0.0;
            performance_state.tonic = 12.0f + transpose;
        }

        performance_state.strum = strum;

        int polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);

        if (polyphony != part.polyphony()) {
            part.set_polyphony(polyphony);
            data_.string_synth.set_polyphony(polyphony);
        }

        int imodel = constrain(data_.f_model, 0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
        rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
        if (model != part.model()) {
            part.set_model(model);
            data_.string_synth.set_fx(static_cast<rings::FxType>(model));
        }


        part.set_bypass(data_.f_bypass > 0.5);

        if (data_.f_easter_egg > 0.5) {
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

        for (int i = 0; i < RingsBlock; i++) {
            buffer.setSample(O_ODD, bidx + i, out[i]);
            buffer.setSample(O_EVEN, bidx + i, aux[i]);
        }

    }
}

bool Rngs::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Rngs::createEditor()
{
    return new RngsEditor (*this);
}

void Rngs::getStateInformation (MemoryBlock& destData)
{
    // SSP not curently using
    // store state information
}

void Rngs::setStateInformation (const void* data, int sizeInBytes)
{
    // SSP not curently using
    // recall state information - created by getStateInformation
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Rngs();
}
