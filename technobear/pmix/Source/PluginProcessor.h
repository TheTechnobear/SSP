#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>
#include <algorithm>


inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

struct TrackData {
    TrackData() {
        init();
    }

    void makeFollow(unsigned f) {
        dummy_=true;
        follows_=f;
    }

    void init() {
        level_[0] = 1.0f;
        for(unsigned i=1;i<OUT_TRACKS;i++) level_[i] = 0.0f;
        pan_=0.0f;
        gain_=1.0f;
        mute_=false;
        solo_=false;
        dummy_=false;
        follows_=0;
    }

    static constexpr unsigned OUT_TRACKS = 3;
    std::atomic<float>  level_[OUT_TRACKS];  // 0==master, >1 = sends
    std::atomic<float>  pan_;    // -1 to 1
    std::atomic<float>  gain_;   
    std::atomic<bool>   mute_; 
    std::atomic<bool>   solo_; 
    std::atomic<float>  rms_;

    // only used by processor
    bool                dummy_;
    unsigned            follows_; // dummy
    static constexpr    unsigned MAX_RMS = 100 ; // 1000 / (48000/129)  = 1 block = 2.8 mS, we want ~ 300mS 
    unsigned            rmsHead_=0;
    float               rmsSum_=0.0f;
    float               rmsHistory_[MAX_RMS];
};


class Pmix  : public AudioProcessor
{
public:
    Pmix();
    ~Pmix();

    void write();

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

    unsigned numInTracks() { return  IN_T_MAX;}
    unsigned numOutTracks() { return  OUT_T_MAX;}

    TrackData& inputTrack(unsigned t)   {  return t < IN_T_MAX ? inTracks_[t] : inTracks_[IN_T_MAX-1];}
    TrackData& outputTrack(unsigned t)  {  return t < OUT_T_MAX ? outTracks_[t] : outTracks_[OUT_T_MAX-1];}



    enum {
        I_IN_1,
        I_IN_2,
        I_IN_3,
        I_IN_4,
        I_IN_5,
        I_IN_6,
        I_IN_7,
        I_IN_8,
        I_MAX
    };

    enum {
        O_MAIN_L,
        O_MAIN_R,
        O_AUX_1_L,
        O_AUX_1_R,
        O_AUX_2_L,
        O_AUX_2_R,
        O_AUX_3_L,
        O_AUX_3_R,
        O_MAX
    };
    
protected:
    float cv2Pitch(float r) {
        // SSP SDK
        static constexpr float p1 = 0.02325f; // first C note
        static constexpr float p2 = 0.21187f; // second C note
        static constexpr float scale = 12.0f / (p2 - p1);
        float arg = r;
        arg = arg - p1;
        arg = arg * scale;
        return arg;
    }

    void writeToJson();
    void readFromJson();
    String fileFromIdx(int idx, bool&);

private:

    static constexpr unsigned IN_T_MAX = 8;
    static constexpr unsigned OUT_T_MAX = 8;

    TrackData inTracks_[IN_T_MAX];
    TrackData outTracks_[OUT_T_MAX];
    void initTracks();


    int currentProgram_ = -1;


    float params_[Percussa::sspLast];

    AudioSampleBuffer inputBuffers_;
    AudioSampleBuffer outputBuffers_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pmix)
};


