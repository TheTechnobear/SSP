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

#pragma once 

#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h" 

#include <atomic>


// currently this is based off my PD implmentation

#include "rings/dsp/part.h"
#include "rings/dsp/patch.h"
#include "rings/dsp/strummer.h"
#include "rings/dsp/string_synth_part.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

struct RngsData {
	RngsData() {
		f_polyphony = 1.0f;
		f_model = 0.0f;
		f_pitch = 0.0f;
		f_structure = 0.0f;
		f_brightness = 0.0f;
		f_damping = 0.0f;
		f_position = 0.0f;
		f_bypass = 0.0f;
		f_easter_egg = 0.0f;
		f_internal_exciter = 1;
		f_internal_strum = 1;
		f_internal_note= 0;
		f_chord = 0;
		f_transpose = 0;
		f_fm = 0;
		f_trig = 0;

		in_level = 0.0f;
		kNoiseGateThreshold = 0.00003f;

		iobufsz = 128;
	    in = new float[iobufsz];
        out = new float[iobufsz];
        aux = new float[iobufsz];
	}
	~RngsData() {
		delete [] in;
        delete [] out;
        delete [] aux;
		in=nullptr;
        out=nullptr;
        aux=nullptr;
	}

	// poly - 1-4 button
	// mode - 1-4 button

	// frequency = para+cvin
	// structure = para+cvin
	// brightness = para+cvin
	// damping = para+cvin
	// position = para+cvin

	// strum = cvin
	// v/oct = cvin  (note: v/oct diff from frequency)
	// in = cvin

	// odd = audio out
	// even = audio out



    std::atomic<float>  f_pitch;
    std::atomic<float>  f_transpose;
    std::atomic<float>  f_structure;
    std::atomic<float>  f_brightness;
    std::atomic<float>  f_damping;
    std::atomic<float>  f_position;
    std::atomic<float>  f_bypass;
    std::atomic<float>  f_easter_egg;
    std::atomic<float>  f_polyphony;
    std::atomic<float>  f_model;
    std::atomic<float>  f_chord;
    std::atomic<float>  f_trig;
    std::atomic<float>  f_fm;
    std::atomic<float>  f_internal_strum;
    std::atomic<float>  f_internal_exciter;
    std::atomic<float>  f_internal_note;

    rings::Part part;
    rings::PerformanceState performance_state;
    rings::StringSynthPart string_synth;
    rings::Strummer strummer;
    rings::Patch patch;


    float kNoiseGateThreshold;
    float in_level;

    float *in,*out,*aux;
    int iobufsz;

    static const int REVERB_SZ = 32768;
    uint16_t buffer[REVERB_SZ];
};


class Rngs  : public AudioProcessor
{
public:
	Rngs();
	~Rngs();

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
    RngsData& data() { return data_;}

private:
    enum {
        I_IN,
        I_STRUM,
        I_VOCT,
        I_FM,
        I_STUCTURE,
        I_BRIGHTNESS,
        I_DAMPING,
        I_POSITION,
        I_MAX
    };
    enum {
        O_ODD,
        O_EVEN,
        O_MAX
    };

	// see Percussa.h 
	float paramValues[Percussa::sspLast-Percussa::sspFirst]; 
	RngsData data_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Rngs)
};

 
