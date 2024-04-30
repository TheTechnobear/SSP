/*******************************************************************************************************************
Cycling '74 License for Max-Generated Code for Export
Copyright (c) 2022 Cycling '74
The code that Max generates automatically and that end users are capable of exporting and using, and any
  associated documentation files (the “Software”) is a work of authorship for which Cycling '74 is the author
  and owner for copyright purposes.  A license is hereby granted, free of charge, to any person obtaining a
  copy of the Software (“Licensee”) to use, copy, modify, merge, publish, and distribute copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The Software is licensed to Licensee only for non-commercial use. Users who wish to make commercial use of the
  Software must contact the copyright owner to determine if a license for commercial use is available, and the
  terms and conditions for same, which may include fees or royalties. For commercial use, please send inquiries
  to licensing@cycling74.com.  The determination of whether a use is commercial use or non-commercial use is based
  upon the use, not the user. The Software may be used by individuals, institutions, governments, corporations, or
  other business whether for-profit or non-profit so long as the use itself is not a commercialization of the
  materials or a use that generates or is intended to generate income, revenue, sales or profit.
The above copyright notice and this license shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
  THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

Please see https://support.cycling74.com/hc/en-us/articles/10730637742483-RNBO-Export-Licensing-FAQ for additional information
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define floor(x) ((long)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class rnbomatic : public PatcherInterfaceImpl {
public:

class RNBOSubpatcher_493 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_493()
    {
    }
    
    ~RNBOSubpatcher_493()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_01_perform(this->signals[0], n);
        this->dspexpr_05_perform(in2, this->dspexpr_05_in2, this->signals[1], n);
        this->dspexpr_07_perform(this->signals[1], this->dspexpr_07_in2, this->signals[2], n);
    
        this->dspexpr_06_perform(
            this->signals[2],
            this->dspexpr_06_in2,
            this->dspexpr_06_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_01_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_03_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_02_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_10_perform(in3, this->dspexpr_10_in2, this->signals[3], n);
        this->dspexpr_11_perform(this->signals[3], this->dspexpr_11_in2, this->signals[0], n);
        this->dspexpr_09_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_08_perform(
            this->signals[3],
            this->dspexpr_08_in2,
            this->dspexpr_08_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_02_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_04_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_01_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_01_sigbuf = resizeSignal(this->ip_01_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_01_dspsetup(forceDSPSetup);
        this->numbertilde_01_dspsetup(forceDSPSetup);
        this->numbertilde_02_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_01_getPresetValue(getSubState(preset, "bin"));
        this->param_02_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_01_value_set(v);
            break;
        case 1:
            this->param_02_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_01_value;
        case 1:
            return this->param_02_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f1/bin";
        case 1:
            return "f1/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-1";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-1";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_01_value_constrain(value);
        case 1:
            return this->param_02_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_01_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_02_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f1/number~_obj-83") == objectId)
                this->numbertilde_01_sig_number_set(payload);
    
            if (TAG("f1/number~_obj-84") == objectId)
                this->numbertilde_02_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f1/number~_obj-83") == objectId)
                this->numbertilde_01_mode_set(payload);
    
            if (TAG("f1/number~_obj-84") == objectId)
                this->numbertilde_02_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f1/number~_obj-83") == objectId)
                this->numbertilde_01_sig_list_set(payload);
    
            if (TAG("f1/number~_obj-84") == objectId)
                this->numbertilde_02_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f1/number~_obj-83"):
            return "f1/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f1/number~_obj-84"):
            return "f1/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_01_value_set(number v) {
        v = this->param_01_value_constrain(v);
        this->param_01_value = v;
        this->sendParameter(0, false);
    
        if (this->param_01_value != this->param_01_lastValue) {
            this->getEngine()->presetTouched();
            this->param_01_lastValue = this->param_01_value;
        }
    
        this->dspexpr_07_in2_set(v);
    }
    
    void param_02_value_set(number v) {
        v = this->param_02_value_constrain(v);
        this->param_02_value = v;
        this->sendParameter(1, false);
    
        if (this->param_02_value != this->param_02_lastValue) {
            this->getEngine()->presetTouched();
            this->param_02_lastValue = this->param_02_value;
        }
    
        this->dspexpr_11_in2_set(v);
    }
    
    void numbertilde_01_sig_number_set(number v) {
        this->numbertilde_01_outValue = v;
    }
    
    void numbertilde_01_sig_list_set(const list& v) {
        this->numbertilde_01_outValue = v[0];
    }
    
    void numbertilde_01_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_01_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_01_currentMode = 1;
        }
    }
    
    void numbertilde_02_sig_number_set(number v) {
        this->numbertilde_02_outValue = v;
    }
    
    void numbertilde_02_sig_list_set(const list& v) {
        this->numbertilde_02_outValue = v[0];
    }
    
    void numbertilde_02_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_02_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_02_currentMode = 1;
        }
    }
    
    void numbertilde_01_value_set(number ) {}
    
    void numbertilde_02_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_01_init();
        this->numbertilde_01_init();
        this->numbertilde_02_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_01_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_07_in2_set(number v) {
        this->dspexpr_07_in2 = v;
    }
    
    static number param_02_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_11_in2_set(number v) {
        this->dspexpr_11_in2 = v;
    }
    
    void ip_01_perform(Sample * out, Index n) {
        auto __ip_01_sigbuf = this->ip_01_sigbuf;
        auto __ip_01_lastValue = this->ip_01_lastValue;
        auto __ip_01_lastIndex = this->ip_01_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_01_lastIndex ? __ip_01_lastValue : __ip_01_sigbuf[(Index)i]);
        }
    
        __ip_01_lastIndex = 0;
        this->ip_01_lastIndex = __ip_01_lastIndex;
    }
    
    void dspexpr_05_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_07_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_06_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_01_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_01_currentIntervalInSamples = this->numbertilde_01_currentIntervalInSamples;
        auto __numbertilde_01_lastValue = this->numbertilde_01_lastValue;
        auto __numbertilde_01_currentInterval = this->numbertilde_01_currentInterval;
        auto __numbertilde_01_rampInSamples = this->numbertilde_01_rampInSamples;
        auto __numbertilde_01_outValue = this->numbertilde_01_outValue;
        auto __numbertilde_01_currentMode = this->numbertilde_01_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_01_currentMode == 0) {
                output[(Index)i] = this->numbertilde_01_smooth_next(
                    __numbertilde_01_outValue,
                    __numbertilde_01_rampInSamples,
                    __numbertilde_01_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_01_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_01_lastValue && __numbertilde_01_currentInterval <= 0) {
            __numbertilde_01_currentInterval = __numbertilde_01_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_01_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f1/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_01_currentInterval = __numbertilde_01_currentInterval;
        this->numbertilde_01_lastValue = __numbertilde_01_lastValue;
    }
    
    void dspexpr_03_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_02_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_10_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_11_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_09_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_08_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_02_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_02_currentIntervalInSamples = this->numbertilde_02_currentIntervalInSamples;
        auto __numbertilde_02_lastValue = this->numbertilde_02_lastValue;
        auto __numbertilde_02_currentInterval = this->numbertilde_02_currentInterval;
        auto __numbertilde_02_rampInSamples = this->numbertilde_02_rampInSamples;
        auto __numbertilde_02_outValue = this->numbertilde_02_outValue;
        auto __numbertilde_02_currentMode = this->numbertilde_02_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_02_currentMode == 0) {
                output[(Index)i] = this->numbertilde_02_smooth_next(
                    __numbertilde_02_outValue,
                    __numbertilde_02_rampInSamples,
                    __numbertilde_02_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_02_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_02_lastValue && __numbertilde_02_currentInterval <= 0) {
            __numbertilde_02_currentInterval = __numbertilde_02_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_02_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f1/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_02_currentInterval = __numbertilde_02_currentInterval;
        this->numbertilde_02_lastValue = __numbertilde_02_lastValue;
    }
    
    void dspexpr_04_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_01_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_01_init() {
        this->ip_01_lastValue = this->ip_01_value;
    }
    
    void ip_01_dspsetup(bool force) {
        if ((bool)(this->ip_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_01_lastIndex = 0;
        this->ip_01_setupDone = true;
    }
    
    number numbertilde_01_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_01_smooth_d_prev);
        this->numbertilde_01_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_01_smooth_d_dspsetup() {
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_smooth_d_reset() {
        this->numbertilde_01_smooth_d_prev = 0;
    }
    
    number numbertilde_01_smooth_next(number x, number up, number down) {
        if (this->numbertilde_01_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_01_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_01_smooth_index = _up;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _up;
            } else if (x < this->numbertilde_01_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_01_smooth_index = _down;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_01_smooth_index > 0) {
            this->numbertilde_01_smooth_prev += this->numbertilde_01_smooth_increment;
            this->numbertilde_01_smooth_index -= 1;
        } else {
            this->numbertilde_01_smooth_prev = x;
        }
    
        return this->numbertilde_01_smooth_prev;
    }
    
    void numbertilde_01_smooth_reset() {
        this->numbertilde_01_smooth_prev = 0;
        this->numbertilde_01_smooth_index = 0;
        this->numbertilde_01_smooth_increment = 0;
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f1/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_01_dspsetup(bool force) {
        if ((bool)(this->numbertilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_01_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_01_currentInterval = this->numbertilde_01_currentIntervalInSamples;
        this->numbertilde_01_rampInSamples = this->mstosamps(this->numbertilde_01_ramp);
        this->numbertilde_01_setupDone = true;
        this->numbertilde_01_smooth_d_dspsetup();
    }
    
    void param_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_01_value;
    }
    
    void param_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_01_value_set(preset["value"]);
    }
    
    number numbertilde_02_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_02_smooth_d_prev);
        this->numbertilde_02_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_02_smooth_d_dspsetup() {
        this->numbertilde_02_smooth_d_reset();
    }
    
    void numbertilde_02_smooth_d_reset() {
        this->numbertilde_02_smooth_d_prev = 0;
    }
    
    number numbertilde_02_smooth_next(number x, number up, number down) {
        if (this->numbertilde_02_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_02_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_02_smooth_index = _up;
                this->numbertilde_02_smooth_increment = (x - this->numbertilde_02_smooth_prev) / _up;
            } else if (x < this->numbertilde_02_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_02_smooth_index = _down;
                this->numbertilde_02_smooth_increment = (x - this->numbertilde_02_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_02_smooth_index > 0) {
            this->numbertilde_02_smooth_prev += this->numbertilde_02_smooth_increment;
            this->numbertilde_02_smooth_index -= 1;
        } else {
            this->numbertilde_02_smooth_prev = x;
        }
    
        return this->numbertilde_02_smooth_prev;
    }
    
    void numbertilde_02_smooth_reset() {
        this->numbertilde_02_smooth_prev = 0;
        this->numbertilde_02_smooth_index = 0;
        this->numbertilde_02_smooth_increment = 0;
        this->numbertilde_02_smooth_d_reset();
    }
    
    void numbertilde_02_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f1/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_02_dspsetup(bool force) {
        if ((bool)(this->numbertilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_02_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_02_currentInterval = this->numbertilde_02_currentIntervalInSamples;
        this->numbertilde_02_rampInSamples = this->mstosamps(this->numbertilde_02_ramp);
        this->numbertilde_02_setupDone = true;
        this->numbertilde_02_smooth_d_dspsetup();
    }
    
    void param_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_02_value;
    }
    
    void param_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_02_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_01_in1 = 0;
        dspexpr_01_in2 = 0;
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 0;
        ip_01_value = 1;
        ip_01_impulse = 0;
        dspexpr_03_in1 = 0;
        dspexpr_03_in2 = 0;
        dspexpr_04_in1 = 0;
        dspexpr_04_in2 = 0;
        dspexpr_05_in1 = 0;
        dspexpr_05_in2 = 511;
        dspexpr_06_in1 = 0;
        dspexpr_06_in2 = 0;
        dspexpr_06_in3 = 511;
        dspexpr_07_in1 = 0;
        dspexpr_07_in2 = 0;
        numbertilde_01_input_number = 0;
        numbertilde_01_ramp = 0;
        param_01_value = 0;
        numbertilde_02_input_number = 0;
        numbertilde_02_ramp = 0;
        dspexpr_08_in1 = 0;
        dspexpr_08_in2 = 0;
        dspexpr_08_in3 = 511;
        dspexpr_09_in1 = 0;
        dspexpr_09_in2 = 0;
        dspexpr_10_in1 = 0;
        dspexpr_10_in2 = 511;
        dspexpr_11_in1 = 0;
        dspexpr_11_in2 = 0;
        param_02_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_01_lastIndex = 0;
        ip_01_lastValue = 0;
        ip_01_resetCount = 0;
        ip_01_sigbuf = nullptr;
        ip_01_setupDone = false;
        numbertilde_01_currentInterval = 0;
        numbertilde_01_currentIntervalInSamples = 0;
        numbertilde_01_lastValue = 0;
        numbertilde_01_outValue = 0;
        numbertilde_01_rampInSamples = 0;
        numbertilde_01_currentMode = 1;
        numbertilde_01_smooth_d_prev = 0;
        numbertilde_01_smooth_prev = 0;
        numbertilde_01_smooth_index = 0;
        numbertilde_01_smooth_increment = 0;
        numbertilde_01_setupDone = false;
        param_01_lastValue = 0;
        numbertilde_02_currentInterval = 0;
        numbertilde_02_currentIntervalInSamples = 0;
        numbertilde_02_lastValue = 0;
        numbertilde_02_outValue = 0;
        numbertilde_02_rampInSamples = 0;
        numbertilde_02_currentMode = 1;
        numbertilde_02_smooth_d_prev = 0;
        numbertilde_02_smooth_prev = 0;
        numbertilde_02_smooth_index = 0;
        numbertilde_02_smooth_increment = 0;
        numbertilde_02_setupDone = false;
        param_02_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number ip_01_value;
        number ip_01_impulse;
        number dspexpr_03_in1;
        number dspexpr_03_in2;
        number dspexpr_04_in1;
        number dspexpr_04_in2;
        number dspexpr_05_in1;
        number dspexpr_05_in2;
        number dspexpr_06_in1;
        number dspexpr_06_in2;
        number dspexpr_06_in3;
        number dspexpr_07_in1;
        number dspexpr_07_in2;
        number numbertilde_01_input_number;
        number numbertilde_01_ramp;
        number param_01_value;
        number numbertilde_02_input_number;
        number numbertilde_02_ramp;
        number dspexpr_08_in1;
        number dspexpr_08_in2;
        number dspexpr_08_in3;
        number dspexpr_09_in1;
        number dspexpr_09_in2;
        number dspexpr_10_in1;
        number dspexpr_10_in2;
        number dspexpr_11_in1;
        number dspexpr_11_in2;
        number param_02_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_01_lastIndex;
        number ip_01_lastValue;
        SampleIndex ip_01_resetCount;
        signal ip_01_sigbuf;
        bool ip_01_setupDone;
        SampleIndex numbertilde_01_currentInterval;
        SampleIndex numbertilde_01_currentIntervalInSamples;
        number numbertilde_01_lastValue;
        number numbertilde_01_outValue;
        number numbertilde_01_rampInSamples;
        Int numbertilde_01_currentMode;
        number numbertilde_01_smooth_d_prev;
        number numbertilde_01_smooth_prev;
        number numbertilde_01_smooth_index;
        number numbertilde_01_smooth_increment;
        bool numbertilde_01_setupDone;
        number param_01_lastValue;
        SampleIndex numbertilde_02_currentInterval;
        SampleIndex numbertilde_02_currentIntervalInSamples;
        number numbertilde_02_lastValue;
        number numbertilde_02_outValue;
        number numbertilde_02_rampInSamples;
        Int numbertilde_02_currentMode;
        number numbertilde_02_smooth_d_prev;
        number numbertilde_02_smooth_prev;
        number numbertilde_02_smooth_index;
        number numbertilde_02_smooth_increment;
        bool numbertilde_02_setupDone;
        number param_02_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_494 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_494()
    {
    }
    
    ~RNBOSubpatcher_494()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_02_perform(this->signals[0], n);
        this->dspexpr_16_perform(in2, this->dspexpr_16_in2, this->signals[1], n);
        this->dspexpr_18_perform(this->signals[1], this->dspexpr_18_in2, this->signals[2], n);
    
        this->dspexpr_17_perform(
            this->signals[2],
            this->dspexpr_17_in2,
            this->dspexpr_17_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_03_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_14_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_13_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_21_perform(in3, this->dspexpr_21_in2, this->signals[3], n);
        this->dspexpr_22_perform(this->signals[3], this->dspexpr_22_in2, this->signals[0], n);
        this->dspexpr_20_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_19_perform(
            this->signals[3],
            this->dspexpr_19_in2,
            this->dspexpr_19_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_04_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_15_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_12_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_02_sigbuf = resizeSignal(this->ip_02_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_02_dspsetup(forceDSPSetup);
        this->numbertilde_03_dspsetup(forceDSPSetup);
        this->numbertilde_04_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_03_getPresetValue(getSubState(preset, "bin"));
        this->param_04_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_03_value_set(v);
            break;
        case 1:
            this->param_04_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_03_value;
        case 1:
            return this->param_04_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f2/bin";
        case 1:
            return "f2/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-2";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-2";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_03_value_constrain(value);
        case 1:
            return this->param_04_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_03_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_04_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f2/number~_obj-83") == objectId)
                this->numbertilde_03_sig_number_set(payload);
    
            if (TAG("f2/number~_obj-84") == objectId)
                this->numbertilde_04_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f2/number~_obj-83") == objectId)
                this->numbertilde_03_mode_set(payload);
    
            if (TAG("f2/number~_obj-84") == objectId)
                this->numbertilde_04_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f2/number~_obj-83") == objectId)
                this->numbertilde_03_sig_list_set(payload);
    
            if (TAG("f2/number~_obj-84") == objectId)
                this->numbertilde_04_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f2/number~_obj-83"):
            return "f2/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f2/number~_obj-84"):
            return "f2/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_03_value_set(number v) {
        v = this->param_03_value_constrain(v);
        this->param_03_value = v;
        this->sendParameter(0, false);
    
        if (this->param_03_value != this->param_03_lastValue) {
            this->getEngine()->presetTouched();
            this->param_03_lastValue = this->param_03_value;
        }
    
        this->dspexpr_18_in2_set(v);
    }
    
    void param_04_value_set(number v) {
        v = this->param_04_value_constrain(v);
        this->param_04_value = v;
        this->sendParameter(1, false);
    
        if (this->param_04_value != this->param_04_lastValue) {
            this->getEngine()->presetTouched();
            this->param_04_lastValue = this->param_04_value;
        }
    
        this->dspexpr_22_in2_set(v);
    }
    
    void numbertilde_03_sig_number_set(number v) {
        this->numbertilde_03_outValue = v;
    }
    
    void numbertilde_03_sig_list_set(const list& v) {
        this->numbertilde_03_outValue = v[0];
    }
    
    void numbertilde_03_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_03_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_03_currentMode = 1;
        }
    }
    
    void numbertilde_04_sig_number_set(number v) {
        this->numbertilde_04_outValue = v;
    }
    
    void numbertilde_04_sig_list_set(const list& v) {
        this->numbertilde_04_outValue = v[0];
    }
    
    void numbertilde_04_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_04_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_04_currentMode = 1;
        }
    }
    
    void numbertilde_03_value_set(number ) {}
    
    void numbertilde_04_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_02_init();
        this->numbertilde_03_init();
        this->numbertilde_04_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_03_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_18_in2_set(number v) {
        this->dspexpr_18_in2 = v;
    }
    
    static number param_04_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_22_in2_set(number v) {
        this->dspexpr_22_in2 = v;
    }
    
    void ip_02_perform(Sample * out, Index n) {
        auto __ip_02_sigbuf = this->ip_02_sigbuf;
        auto __ip_02_lastValue = this->ip_02_lastValue;
        auto __ip_02_lastIndex = this->ip_02_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_02_lastIndex ? __ip_02_lastValue : __ip_02_sigbuf[(Index)i]);
        }
    
        __ip_02_lastIndex = 0;
        this->ip_02_lastIndex = __ip_02_lastIndex;
    }
    
    void dspexpr_16_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_18_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_17_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_03_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_03_currentIntervalInSamples = this->numbertilde_03_currentIntervalInSamples;
        auto __numbertilde_03_lastValue = this->numbertilde_03_lastValue;
        auto __numbertilde_03_currentInterval = this->numbertilde_03_currentInterval;
        auto __numbertilde_03_rampInSamples = this->numbertilde_03_rampInSamples;
        auto __numbertilde_03_outValue = this->numbertilde_03_outValue;
        auto __numbertilde_03_currentMode = this->numbertilde_03_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_03_currentMode == 0) {
                output[(Index)i] = this->numbertilde_03_smooth_next(
                    __numbertilde_03_outValue,
                    __numbertilde_03_rampInSamples,
                    __numbertilde_03_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_03_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_03_lastValue && __numbertilde_03_currentInterval <= 0) {
            __numbertilde_03_currentInterval = __numbertilde_03_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_03_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f2/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_03_currentInterval = __numbertilde_03_currentInterval;
        this->numbertilde_03_lastValue = __numbertilde_03_lastValue;
    }
    
    void dspexpr_14_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_13_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_21_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_22_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_20_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_19_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_04_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_04_currentIntervalInSamples = this->numbertilde_04_currentIntervalInSamples;
        auto __numbertilde_04_lastValue = this->numbertilde_04_lastValue;
        auto __numbertilde_04_currentInterval = this->numbertilde_04_currentInterval;
        auto __numbertilde_04_rampInSamples = this->numbertilde_04_rampInSamples;
        auto __numbertilde_04_outValue = this->numbertilde_04_outValue;
        auto __numbertilde_04_currentMode = this->numbertilde_04_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_04_currentMode == 0) {
                output[(Index)i] = this->numbertilde_04_smooth_next(
                    __numbertilde_04_outValue,
                    __numbertilde_04_rampInSamples,
                    __numbertilde_04_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_04_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_04_lastValue && __numbertilde_04_currentInterval <= 0) {
            __numbertilde_04_currentInterval = __numbertilde_04_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_04_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f2/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_04_currentInterval = __numbertilde_04_currentInterval;
        this->numbertilde_04_lastValue = __numbertilde_04_lastValue;
    }
    
    void dspexpr_15_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_12_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_02_init() {
        this->ip_02_lastValue = this->ip_02_value;
    }
    
    void ip_02_dspsetup(bool force) {
        if ((bool)(this->ip_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_02_lastIndex = 0;
        this->ip_02_setupDone = true;
    }
    
    number numbertilde_03_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_03_smooth_d_prev);
        this->numbertilde_03_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_03_smooth_d_dspsetup() {
        this->numbertilde_03_smooth_d_reset();
    }
    
    void numbertilde_03_smooth_d_reset() {
        this->numbertilde_03_smooth_d_prev = 0;
    }
    
    number numbertilde_03_smooth_next(number x, number up, number down) {
        if (this->numbertilde_03_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_03_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_03_smooth_index = _up;
                this->numbertilde_03_smooth_increment = (x - this->numbertilde_03_smooth_prev) / _up;
            } else if (x < this->numbertilde_03_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_03_smooth_index = _down;
                this->numbertilde_03_smooth_increment = (x - this->numbertilde_03_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_03_smooth_index > 0) {
            this->numbertilde_03_smooth_prev += this->numbertilde_03_smooth_increment;
            this->numbertilde_03_smooth_index -= 1;
        } else {
            this->numbertilde_03_smooth_prev = x;
        }
    
        return this->numbertilde_03_smooth_prev;
    }
    
    void numbertilde_03_smooth_reset() {
        this->numbertilde_03_smooth_prev = 0;
        this->numbertilde_03_smooth_index = 0;
        this->numbertilde_03_smooth_increment = 0;
        this->numbertilde_03_smooth_d_reset();
    }
    
    void numbertilde_03_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f2/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_03_dspsetup(bool force) {
        if ((bool)(this->numbertilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_03_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_03_currentInterval = this->numbertilde_03_currentIntervalInSamples;
        this->numbertilde_03_rampInSamples = this->mstosamps(this->numbertilde_03_ramp);
        this->numbertilde_03_setupDone = true;
        this->numbertilde_03_smooth_d_dspsetup();
    }
    
    void param_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_03_value;
    }
    
    void param_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_03_value_set(preset["value"]);
    }
    
    number numbertilde_04_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_04_smooth_d_prev);
        this->numbertilde_04_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_04_smooth_d_dspsetup() {
        this->numbertilde_04_smooth_d_reset();
    }
    
    void numbertilde_04_smooth_d_reset() {
        this->numbertilde_04_smooth_d_prev = 0;
    }
    
    number numbertilde_04_smooth_next(number x, number up, number down) {
        if (this->numbertilde_04_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_04_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_04_smooth_index = _up;
                this->numbertilde_04_smooth_increment = (x - this->numbertilde_04_smooth_prev) / _up;
            } else if (x < this->numbertilde_04_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_04_smooth_index = _down;
                this->numbertilde_04_smooth_increment = (x - this->numbertilde_04_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_04_smooth_index > 0) {
            this->numbertilde_04_smooth_prev += this->numbertilde_04_smooth_increment;
            this->numbertilde_04_smooth_index -= 1;
        } else {
            this->numbertilde_04_smooth_prev = x;
        }
    
        return this->numbertilde_04_smooth_prev;
    }
    
    void numbertilde_04_smooth_reset() {
        this->numbertilde_04_smooth_prev = 0;
        this->numbertilde_04_smooth_index = 0;
        this->numbertilde_04_smooth_increment = 0;
        this->numbertilde_04_smooth_d_reset();
    }
    
    void numbertilde_04_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f2/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_04_dspsetup(bool force) {
        if ((bool)(this->numbertilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_04_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_04_currentInterval = this->numbertilde_04_currentIntervalInSamples;
        this->numbertilde_04_rampInSamples = this->mstosamps(this->numbertilde_04_ramp);
        this->numbertilde_04_setupDone = true;
        this->numbertilde_04_smooth_d_dspsetup();
    }
    
    void param_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_04_value;
    }
    
    void param_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_04_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_12_in1 = 0;
        dspexpr_12_in2 = 0;
        dspexpr_13_in1 = 0;
        dspexpr_13_in2 = 0;
        ip_02_value = 1;
        ip_02_impulse = 0;
        dspexpr_14_in1 = 0;
        dspexpr_14_in2 = 0;
        dspexpr_15_in1 = 0;
        dspexpr_15_in2 = 0;
        dspexpr_16_in1 = 0;
        dspexpr_16_in2 = 511;
        dspexpr_17_in1 = 0;
        dspexpr_17_in2 = 0;
        dspexpr_17_in3 = 511;
        dspexpr_18_in1 = 0;
        dspexpr_18_in2 = 0;
        numbertilde_03_input_number = 0;
        numbertilde_03_ramp = 0;
        param_03_value = 0;
        numbertilde_04_input_number = 0;
        numbertilde_04_ramp = 0;
        dspexpr_19_in1 = 0;
        dspexpr_19_in2 = 0;
        dspexpr_19_in3 = 511;
        dspexpr_20_in1 = 0;
        dspexpr_20_in2 = 0;
        dspexpr_21_in1 = 0;
        dspexpr_21_in2 = 511;
        dspexpr_22_in1 = 0;
        dspexpr_22_in2 = 0;
        param_04_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_02_lastIndex = 0;
        ip_02_lastValue = 0;
        ip_02_resetCount = 0;
        ip_02_sigbuf = nullptr;
        ip_02_setupDone = false;
        numbertilde_03_currentInterval = 0;
        numbertilde_03_currentIntervalInSamples = 0;
        numbertilde_03_lastValue = 0;
        numbertilde_03_outValue = 0;
        numbertilde_03_rampInSamples = 0;
        numbertilde_03_currentMode = 1;
        numbertilde_03_smooth_d_prev = 0;
        numbertilde_03_smooth_prev = 0;
        numbertilde_03_smooth_index = 0;
        numbertilde_03_smooth_increment = 0;
        numbertilde_03_setupDone = false;
        param_03_lastValue = 0;
        numbertilde_04_currentInterval = 0;
        numbertilde_04_currentIntervalInSamples = 0;
        numbertilde_04_lastValue = 0;
        numbertilde_04_outValue = 0;
        numbertilde_04_rampInSamples = 0;
        numbertilde_04_currentMode = 1;
        numbertilde_04_smooth_d_prev = 0;
        numbertilde_04_smooth_prev = 0;
        numbertilde_04_smooth_index = 0;
        numbertilde_04_smooth_increment = 0;
        numbertilde_04_setupDone = false;
        param_04_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_12_in1;
        number dspexpr_12_in2;
        number dspexpr_13_in1;
        number dspexpr_13_in2;
        number ip_02_value;
        number ip_02_impulse;
        number dspexpr_14_in1;
        number dspexpr_14_in2;
        number dspexpr_15_in1;
        number dspexpr_15_in2;
        number dspexpr_16_in1;
        number dspexpr_16_in2;
        number dspexpr_17_in1;
        number dspexpr_17_in2;
        number dspexpr_17_in3;
        number dspexpr_18_in1;
        number dspexpr_18_in2;
        number numbertilde_03_input_number;
        number numbertilde_03_ramp;
        number param_03_value;
        number numbertilde_04_input_number;
        number numbertilde_04_ramp;
        number dspexpr_19_in1;
        number dspexpr_19_in2;
        number dspexpr_19_in3;
        number dspexpr_20_in1;
        number dspexpr_20_in2;
        number dspexpr_21_in1;
        number dspexpr_21_in2;
        number dspexpr_22_in1;
        number dspexpr_22_in2;
        number param_04_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_02_lastIndex;
        number ip_02_lastValue;
        SampleIndex ip_02_resetCount;
        signal ip_02_sigbuf;
        bool ip_02_setupDone;
        SampleIndex numbertilde_03_currentInterval;
        SampleIndex numbertilde_03_currentIntervalInSamples;
        number numbertilde_03_lastValue;
        number numbertilde_03_outValue;
        number numbertilde_03_rampInSamples;
        Int numbertilde_03_currentMode;
        number numbertilde_03_smooth_d_prev;
        number numbertilde_03_smooth_prev;
        number numbertilde_03_smooth_index;
        number numbertilde_03_smooth_increment;
        bool numbertilde_03_setupDone;
        number param_03_lastValue;
        SampleIndex numbertilde_04_currentInterval;
        SampleIndex numbertilde_04_currentIntervalInSamples;
        number numbertilde_04_lastValue;
        number numbertilde_04_outValue;
        number numbertilde_04_rampInSamples;
        Int numbertilde_04_currentMode;
        number numbertilde_04_smooth_d_prev;
        number numbertilde_04_smooth_prev;
        number numbertilde_04_smooth_index;
        number numbertilde_04_smooth_increment;
        bool numbertilde_04_setupDone;
        number param_04_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_495 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_495()
    {
    }
    
    ~RNBOSubpatcher_495()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_03_perform(this->signals[0], n);
        this->dspexpr_27_perform(in2, this->dspexpr_27_in2, this->signals[1], n);
        this->dspexpr_29_perform(this->signals[1], this->dspexpr_29_in2, this->signals[2], n);
    
        this->dspexpr_28_perform(
            this->signals[2],
            this->dspexpr_28_in2,
            this->dspexpr_28_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_05_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_25_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_24_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_32_perform(in3, this->dspexpr_32_in2, this->signals[3], n);
        this->dspexpr_33_perform(this->signals[3], this->dspexpr_33_in2, this->signals[0], n);
        this->dspexpr_31_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_30_perform(
            this->signals[3],
            this->dspexpr_30_in2,
            this->dspexpr_30_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_06_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_26_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_23_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_03_sigbuf = resizeSignal(this->ip_03_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_03_dspsetup(forceDSPSetup);
        this->numbertilde_05_dspsetup(forceDSPSetup);
        this->numbertilde_06_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_05_getPresetValue(getSubState(preset, "bin"));
        this->param_06_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_05_value_set(v);
            break;
        case 1:
            this->param_06_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_05_value;
        case 1:
            return this->param_06_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f3/bin";
        case 1:
            return "f3/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-3";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-3";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_05_value_constrain(value);
        case 1:
            return this->param_06_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_05_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_06_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f3/number~_obj-83") == objectId)
                this->numbertilde_05_sig_number_set(payload);
    
            if (TAG("f3/number~_obj-84") == objectId)
                this->numbertilde_06_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f3/number~_obj-83") == objectId)
                this->numbertilde_05_mode_set(payload);
    
            if (TAG("f3/number~_obj-84") == objectId)
                this->numbertilde_06_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f3/number~_obj-83") == objectId)
                this->numbertilde_05_sig_list_set(payload);
    
            if (TAG("f3/number~_obj-84") == objectId)
                this->numbertilde_06_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f3/number~_obj-83"):
            return "f3/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f3/number~_obj-84"):
            return "f3/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_05_value_set(number v) {
        v = this->param_05_value_constrain(v);
        this->param_05_value = v;
        this->sendParameter(0, false);
    
        if (this->param_05_value != this->param_05_lastValue) {
            this->getEngine()->presetTouched();
            this->param_05_lastValue = this->param_05_value;
        }
    
        this->dspexpr_29_in2_set(v);
    }
    
    void param_06_value_set(number v) {
        v = this->param_06_value_constrain(v);
        this->param_06_value = v;
        this->sendParameter(1, false);
    
        if (this->param_06_value != this->param_06_lastValue) {
            this->getEngine()->presetTouched();
            this->param_06_lastValue = this->param_06_value;
        }
    
        this->dspexpr_33_in2_set(v);
    }
    
    void numbertilde_05_sig_number_set(number v) {
        this->numbertilde_05_outValue = v;
    }
    
    void numbertilde_05_sig_list_set(const list& v) {
        this->numbertilde_05_outValue = v[0];
    }
    
    void numbertilde_05_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_05_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_05_currentMode = 1;
        }
    }
    
    void numbertilde_06_sig_number_set(number v) {
        this->numbertilde_06_outValue = v;
    }
    
    void numbertilde_06_sig_list_set(const list& v) {
        this->numbertilde_06_outValue = v[0];
    }
    
    void numbertilde_06_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_06_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_06_currentMode = 1;
        }
    }
    
    void numbertilde_05_value_set(number ) {}
    
    void numbertilde_06_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_03_init();
        this->numbertilde_05_init();
        this->numbertilde_06_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_05_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_29_in2_set(number v) {
        this->dspexpr_29_in2 = v;
    }
    
    static number param_06_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_33_in2_set(number v) {
        this->dspexpr_33_in2 = v;
    }
    
    void ip_03_perform(Sample * out, Index n) {
        auto __ip_03_sigbuf = this->ip_03_sigbuf;
        auto __ip_03_lastValue = this->ip_03_lastValue;
        auto __ip_03_lastIndex = this->ip_03_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_03_lastIndex ? __ip_03_lastValue : __ip_03_sigbuf[(Index)i]);
        }
    
        __ip_03_lastIndex = 0;
        this->ip_03_lastIndex = __ip_03_lastIndex;
    }
    
    void dspexpr_27_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_29_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_28_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_05_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_05_currentIntervalInSamples = this->numbertilde_05_currentIntervalInSamples;
        auto __numbertilde_05_lastValue = this->numbertilde_05_lastValue;
        auto __numbertilde_05_currentInterval = this->numbertilde_05_currentInterval;
        auto __numbertilde_05_rampInSamples = this->numbertilde_05_rampInSamples;
        auto __numbertilde_05_outValue = this->numbertilde_05_outValue;
        auto __numbertilde_05_currentMode = this->numbertilde_05_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_05_currentMode == 0) {
                output[(Index)i] = this->numbertilde_05_smooth_next(
                    __numbertilde_05_outValue,
                    __numbertilde_05_rampInSamples,
                    __numbertilde_05_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_05_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_05_lastValue && __numbertilde_05_currentInterval <= 0) {
            __numbertilde_05_currentInterval = __numbertilde_05_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_05_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f3/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_05_currentInterval = __numbertilde_05_currentInterval;
        this->numbertilde_05_lastValue = __numbertilde_05_lastValue;
    }
    
    void dspexpr_25_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_24_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_32_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_33_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_31_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_30_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_06_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_06_currentIntervalInSamples = this->numbertilde_06_currentIntervalInSamples;
        auto __numbertilde_06_lastValue = this->numbertilde_06_lastValue;
        auto __numbertilde_06_currentInterval = this->numbertilde_06_currentInterval;
        auto __numbertilde_06_rampInSamples = this->numbertilde_06_rampInSamples;
        auto __numbertilde_06_outValue = this->numbertilde_06_outValue;
        auto __numbertilde_06_currentMode = this->numbertilde_06_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_06_currentMode == 0) {
                output[(Index)i] = this->numbertilde_06_smooth_next(
                    __numbertilde_06_outValue,
                    __numbertilde_06_rampInSamples,
                    __numbertilde_06_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_06_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_06_lastValue && __numbertilde_06_currentInterval <= 0) {
            __numbertilde_06_currentInterval = __numbertilde_06_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_06_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f3/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_06_currentInterval = __numbertilde_06_currentInterval;
        this->numbertilde_06_lastValue = __numbertilde_06_lastValue;
    }
    
    void dspexpr_26_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_23_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_03_init() {
        this->ip_03_lastValue = this->ip_03_value;
    }
    
    void ip_03_dspsetup(bool force) {
        if ((bool)(this->ip_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_03_lastIndex = 0;
        this->ip_03_setupDone = true;
    }
    
    number numbertilde_05_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_05_smooth_d_prev);
        this->numbertilde_05_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_05_smooth_d_dspsetup() {
        this->numbertilde_05_smooth_d_reset();
    }
    
    void numbertilde_05_smooth_d_reset() {
        this->numbertilde_05_smooth_d_prev = 0;
    }
    
    number numbertilde_05_smooth_next(number x, number up, number down) {
        if (this->numbertilde_05_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_05_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_05_smooth_index = _up;
                this->numbertilde_05_smooth_increment = (x - this->numbertilde_05_smooth_prev) / _up;
            } else if (x < this->numbertilde_05_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_05_smooth_index = _down;
                this->numbertilde_05_smooth_increment = (x - this->numbertilde_05_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_05_smooth_index > 0) {
            this->numbertilde_05_smooth_prev += this->numbertilde_05_smooth_increment;
            this->numbertilde_05_smooth_index -= 1;
        } else {
            this->numbertilde_05_smooth_prev = x;
        }
    
        return this->numbertilde_05_smooth_prev;
    }
    
    void numbertilde_05_smooth_reset() {
        this->numbertilde_05_smooth_prev = 0;
        this->numbertilde_05_smooth_index = 0;
        this->numbertilde_05_smooth_increment = 0;
        this->numbertilde_05_smooth_d_reset();
    }
    
    void numbertilde_05_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f3/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_05_dspsetup(bool force) {
        if ((bool)(this->numbertilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_05_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_05_currentInterval = this->numbertilde_05_currentIntervalInSamples;
        this->numbertilde_05_rampInSamples = this->mstosamps(this->numbertilde_05_ramp);
        this->numbertilde_05_setupDone = true;
        this->numbertilde_05_smooth_d_dspsetup();
    }
    
    void param_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_05_value;
    }
    
    void param_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_05_value_set(preset["value"]);
    }
    
    number numbertilde_06_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_06_smooth_d_prev);
        this->numbertilde_06_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_06_smooth_d_dspsetup() {
        this->numbertilde_06_smooth_d_reset();
    }
    
    void numbertilde_06_smooth_d_reset() {
        this->numbertilde_06_smooth_d_prev = 0;
    }
    
    number numbertilde_06_smooth_next(number x, number up, number down) {
        if (this->numbertilde_06_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_06_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_06_smooth_index = _up;
                this->numbertilde_06_smooth_increment = (x - this->numbertilde_06_smooth_prev) / _up;
            } else if (x < this->numbertilde_06_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_06_smooth_index = _down;
                this->numbertilde_06_smooth_increment = (x - this->numbertilde_06_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_06_smooth_index > 0) {
            this->numbertilde_06_smooth_prev += this->numbertilde_06_smooth_increment;
            this->numbertilde_06_smooth_index -= 1;
        } else {
            this->numbertilde_06_smooth_prev = x;
        }
    
        return this->numbertilde_06_smooth_prev;
    }
    
    void numbertilde_06_smooth_reset() {
        this->numbertilde_06_smooth_prev = 0;
        this->numbertilde_06_smooth_index = 0;
        this->numbertilde_06_smooth_increment = 0;
        this->numbertilde_06_smooth_d_reset();
    }
    
    void numbertilde_06_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f3/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_06_dspsetup(bool force) {
        if ((bool)(this->numbertilde_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_06_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_06_currentInterval = this->numbertilde_06_currentIntervalInSamples;
        this->numbertilde_06_rampInSamples = this->mstosamps(this->numbertilde_06_ramp);
        this->numbertilde_06_setupDone = true;
        this->numbertilde_06_smooth_d_dspsetup();
    }
    
    void param_06_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_06_value;
    }
    
    void param_06_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_06_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_23_in1 = 0;
        dspexpr_23_in2 = 0;
        dspexpr_24_in1 = 0;
        dspexpr_24_in2 = 0;
        ip_03_value = 1;
        ip_03_impulse = 0;
        dspexpr_25_in1 = 0;
        dspexpr_25_in2 = 0;
        dspexpr_26_in1 = 0;
        dspexpr_26_in2 = 0;
        dspexpr_27_in1 = 0;
        dspexpr_27_in2 = 511;
        dspexpr_28_in1 = 0;
        dspexpr_28_in2 = 0;
        dspexpr_28_in3 = 511;
        dspexpr_29_in1 = 0;
        dspexpr_29_in2 = 0;
        numbertilde_05_input_number = 0;
        numbertilde_05_ramp = 0;
        param_05_value = 0;
        numbertilde_06_input_number = 0;
        numbertilde_06_ramp = 0;
        dspexpr_30_in1 = 0;
        dspexpr_30_in2 = 0;
        dspexpr_30_in3 = 511;
        dspexpr_31_in1 = 0;
        dspexpr_31_in2 = 0;
        dspexpr_32_in1 = 0;
        dspexpr_32_in2 = 511;
        dspexpr_33_in1 = 0;
        dspexpr_33_in2 = 0;
        param_06_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_03_lastIndex = 0;
        ip_03_lastValue = 0;
        ip_03_resetCount = 0;
        ip_03_sigbuf = nullptr;
        ip_03_setupDone = false;
        numbertilde_05_currentInterval = 0;
        numbertilde_05_currentIntervalInSamples = 0;
        numbertilde_05_lastValue = 0;
        numbertilde_05_outValue = 0;
        numbertilde_05_rampInSamples = 0;
        numbertilde_05_currentMode = 1;
        numbertilde_05_smooth_d_prev = 0;
        numbertilde_05_smooth_prev = 0;
        numbertilde_05_smooth_index = 0;
        numbertilde_05_smooth_increment = 0;
        numbertilde_05_setupDone = false;
        param_05_lastValue = 0;
        numbertilde_06_currentInterval = 0;
        numbertilde_06_currentIntervalInSamples = 0;
        numbertilde_06_lastValue = 0;
        numbertilde_06_outValue = 0;
        numbertilde_06_rampInSamples = 0;
        numbertilde_06_currentMode = 1;
        numbertilde_06_smooth_d_prev = 0;
        numbertilde_06_smooth_prev = 0;
        numbertilde_06_smooth_index = 0;
        numbertilde_06_smooth_increment = 0;
        numbertilde_06_setupDone = false;
        param_06_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_23_in1;
        number dspexpr_23_in2;
        number dspexpr_24_in1;
        number dspexpr_24_in2;
        number ip_03_value;
        number ip_03_impulse;
        number dspexpr_25_in1;
        number dspexpr_25_in2;
        number dspexpr_26_in1;
        number dspexpr_26_in2;
        number dspexpr_27_in1;
        number dspexpr_27_in2;
        number dspexpr_28_in1;
        number dspexpr_28_in2;
        number dspexpr_28_in3;
        number dspexpr_29_in1;
        number dspexpr_29_in2;
        number numbertilde_05_input_number;
        number numbertilde_05_ramp;
        number param_05_value;
        number numbertilde_06_input_number;
        number numbertilde_06_ramp;
        number dspexpr_30_in1;
        number dspexpr_30_in2;
        number dspexpr_30_in3;
        number dspexpr_31_in1;
        number dspexpr_31_in2;
        number dspexpr_32_in1;
        number dspexpr_32_in2;
        number dspexpr_33_in1;
        number dspexpr_33_in2;
        number param_06_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_03_lastIndex;
        number ip_03_lastValue;
        SampleIndex ip_03_resetCount;
        signal ip_03_sigbuf;
        bool ip_03_setupDone;
        SampleIndex numbertilde_05_currentInterval;
        SampleIndex numbertilde_05_currentIntervalInSamples;
        number numbertilde_05_lastValue;
        number numbertilde_05_outValue;
        number numbertilde_05_rampInSamples;
        Int numbertilde_05_currentMode;
        number numbertilde_05_smooth_d_prev;
        number numbertilde_05_smooth_prev;
        number numbertilde_05_smooth_index;
        number numbertilde_05_smooth_increment;
        bool numbertilde_05_setupDone;
        number param_05_lastValue;
        SampleIndex numbertilde_06_currentInterval;
        SampleIndex numbertilde_06_currentIntervalInSamples;
        number numbertilde_06_lastValue;
        number numbertilde_06_outValue;
        number numbertilde_06_rampInSamples;
        Int numbertilde_06_currentMode;
        number numbertilde_06_smooth_d_prev;
        number numbertilde_06_smooth_prev;
        number numbertilde_06_smooth_index;
        number numbertilde_06_smooth_increment;
        bool numbertilde_06_setupDone;
        number param_06_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_496 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_496()
    {
    }
    
    ~RNBOSubpatcher_496()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_04_perform(this->signals[0], n);
        this->dspexpr_38_perform(in2, this->dspexpr_38_in2, this->signals[1], n);
        this->dspexpr_40_perform(this->signals[1], this->dspexpr_40_in2, this->signals[2], n);
    
        this->dspexpr_39_perform(
            this->signals[2],
            this->dspexpr_39_in2,
            this->dspexpr_39_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_07_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_36_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_35_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_43_perform(in3, this->dspexpr_43_in2, this->signals[3], n);
        this->dspexpr_44_perform(this->signals[3], this->dspexpr_44_in2, this->signals[0], n);
        this->dspexpr_42_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_41_perform(
            this->signals[3],
            this->dspexpr_41_in2,
            this->dspexpr_41_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_08_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_37_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_34_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_04_sigbuf = resizeSignal(this->ip_04_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_04_dspsetup(forceDSPSetup);
        this->numbertilde_07_dspsetup(forceDSPSetup);
        this->numbertilde_08_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_07_getPresetValue(getSubState(preset, "bin"));
        this->param_08_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_07_value_set(v);
            break;
        case 1:
            this->param_08_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_07_value;
        case 1:
            return this->param_08_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f4/bin";
        case 1:
            return "f4/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-4";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-4";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_07_value_constrain(value);
        case 1:
            return this->param_08_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_07_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_08_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f4/number~_obj-83") == objectId)
                this->numbertilde_07_sig_number_set(payload);
    
            if (TAG("f4/number~_obj-84") == objectId)
                this->numbertilde_08_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f4/number~_obj-83") == objectId)
                this->numbertilde_07_mode_set(payload);
    
            if (TAG("f4/number~_obj-84") == objectId)
                this->numbertilde_08_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f4/number~_obj-83") == objectId)
                this->numbertilde_07_sig_list_set(payload);
    
            if (TAG("f4/number~_obj-84") == objectId)
                this->numbertilde_08_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f4/number~_obj-83"):
            return "f4/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f4/number~_obj-84"):
            return "f4/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_07_value_set(number v) {
        v = this->param_07_value_constrain(v);
        this->param_07_value = v;
        this->sendParameter(0, false);
    
        if (this->param_07_value != this->param_07_lastValue) {
            this->getEngine()->presetTouched();
            this->param_07_lastValue = this->param_07_value;
        }
    
        this->dspexpr_40_in2_set(v);
    }
    
    void param_08_value_set(number v) {
        v = this->param_08_value_constrain(v);
        this->param_08_value = v;
        this->sendParameter(1, false);
    
        if (this->param_08_value != this->param_08_lastValue) {
            this->getEngine()->presetTouched();
            this->param_08_lastValue = this->param_08_value;
        }
    
        this->dspexpr_44_in2_set(v);
    }
    
    void numbertilde_07_sig_number_set(number v) {
        this->numbertilde_07_outValue = v;
    }
    
    void numbertilde_07_sig_list_set(const list& v) {
        this->numbertilde_07_outValue = v[0];
    }
    
    void numbertilde_07_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_07_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_07_currentMode = 1;
        }
    }
    
    void numbertilde_08_sig_number_set(number v) {
        this->numbertilde_08_outValue = v;
    }
    
    void numbertilde_08_sig_list_set(const list& v) {
        this->numbertilde_08_outValue = v[0];
    }
    
    void numbertilde_08_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_08_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_08_currentMode = 1;
        }
    }
    
    void numbertilde_07_value_set(number ) {}
    
    void numbertilde_08_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_04_init();
        this->numbertilde_07_init();
        this->numbertilde_08_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_07_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_40_in2_set(number v) {
        this->dspexpr_40_in2 = v;
    }
    
    static number param_08_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_44_in2_set(number v) {
        this->dspexpr_44_in2 = v;
    }
    
    void ip_04_perform(Sample * out, Index n) {
        auto __ip_04_sigbuf = this->ip_04_sigbuf;
        auto __ip_04_lastValue = this->ip_04_lastValue;
        auto __ip_04_lastIndex = this->ip_04_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_04_lastIndex ? __ip_04_lastValue : __ip_04_sigbuf[(Index)i]);
        }
    
        __ip_04_lastIndex = 0;
        this->ip_04_lastIndex = __ip_04_lastIndex;
    }
    
    void dspexpr_38_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_40_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_39_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_07_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_07_currentIntervalInSamples = this->numbertilde_07_currentIntervalInSamples;
        auto __numbertilde_07_lastValue = this->numbertilde_07_lastValue;
        auto __numbertilde_07_currentInterval = this->numbertilde_07_currentInterval;
        auto __numbertilde_07_rampInSamples = this->numbertilde_07_rampInSamples;
        auto __numbertilde_07_outValue = this->numbertilde_07_outValue;
        auto __numbertilde_07_currentMode = this->numbertilde_07_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_07_currentMode == 0) {
                output[(Index)i] = this->numbertilde_07_smooth_next(
                    __numbertilde_07_outValue,
                    __numbertilde_07_rampInSamples,
                    __numbertilde_07_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_07_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_07_lastValue && __numbertilde_07_currentInterval <= 0) {
            __numbertilde_07_currentInterval = __numbertilde_07_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_07_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f4/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_07_currentInterval = __numbertilde_07_currentInterval;
        this->numbertilde_07_lastValue = __numbertilde_07_lastValue;
    }
    
    void dspexpr_36_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_35_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_43_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_44_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_42_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_41_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_08_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_08_currentIntervalInSamples = this->numbertilde_08_currentIntervalInSamples;
        auto __numbertilde_08_lastValue = this->numbertilde_08_lastValue;
        auto __numbertilde_08_currentInterval = this->numbertilde_08_currentInterval;
        auto __numbertilde_08_rampInSamples = this->numbertilde_08_rampInSamples;
        auto __numbertilde_08_outValue = this->numbertilde_08_outValue;
        auto __numbertilde_08_currentMode = this->numbertilde_08_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_08_currentMode == 0) {
                output[(Index)i] = this->numbertilde_08_smooth_next(
                    __numbertilde_08_outValue,
                    __numbertilde_08_rampInSamples,
                    __numbertilde_08_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_08_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_08_lastValue && __numbertilde_08_currentInterval <= 0) {
            __numbertilde_08_currentInterval = __numbertilde_08_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_08_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f4/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_08_currentInterval = __numbertilde_08_currentInterval;
        this->numbertilde_08_lastValue = __numbertilde_08_lastValue;
    }
    
    void dspexpr_37_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_34_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_04_init() {
        this->ip_04_lastValue = this->ip_04_value;
    }
    
    void ip_04_dspsetup(bool force) {
        if ((bool)(this->ip_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_04_lastIndex = 0;
        this->ip_04_setupDone = true;
    }
    
    number numbertilde_07_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_07_smooth_d_prev);
        this->numbertilde_07_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_07_smooth_d_dspsetup() {
        this->numbertilde_07_smooth_d_reset();
    }
    
    void numbertilde_07_smooth_d_reset() {
        this->numbertilde_07_smooth_d_prev = 0;
    }
    
    number numbertilde_07_smooth_next(number x, number up, number down) {
        if (this->numbertilde_07_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_07_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_07_smooth_index = _up;
                this->numbertilde_07_smooth_increment = (x - this->numbertilde_07_smooth_prev) / _up;
            } else if (x < this->numbertilde_07_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_07_smooth_index = _down;
                this->numbertilde_07_smooth_increment = (x - this->numbertilde_07_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_07_smooth_index > 0) {
            this->numbertilde_07_smooth_prev += this->numbertilde_07_smooth_increment;
            this->numbertilde_07_smooth_index -= 1;
        } else {
            this->numbertilde_07_smooth_prev = x;
        }
    
        return this->numbertilde_07_smooth_prev;
    }
    
    void numbertilde_07_smooth_reset() {
        this->numbertilde_07_smooth_prev = 0;
        this->numbertilde_07_smooth_index = 0;
        this->numbertilde_07_smooth_increment = 0;
        this->numbertilde_07_smooth_d_reset();
    }
    
    void numbertilde_07_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f4/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_07_dspsetup(bool force) {
        if ((bool)(this->numbertilde_07_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_07_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_07_currentInterval = this->numbertilde_07_currentIntervalInSamples;
        this->numbertilde_07_rampInSamples = this->mstosamps(this->numbertilde_07_ramp);
        this->numbertilde_07_setupDone = true;
        this->numbertilde_07_smooth_d_dspsetup();
    }
    
    void param_07_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_07_value;
    }
    
    void param_07_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_07_value_set(preset["value"]);
    }
    
    number numbertilde_08_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_08_smooth_d_prev);
        this->numbertilde_08_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_08_smooth_d_dspsetup() {
        this->numbertilde_08_smooth_d_reset();
    }
    
    void numbertilde_08_smooth_d_reset() {
        this->numbertilde_08_smooth_d_prev = 0;
    }
    
    number numbertilde_08_smooth_next(number x, number up, number down) {
        if (this->numbertilde_08_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_08_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_08_smooth_index = _up;
                this->numbertilde_08_smooth_increment = (x - this->numbertilde_08_smooth_prev) / _up;
            } else if (x < this->numbertilde_08_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_08_smooth_index = _down;
                this->numbertilde_08_smooth_increment = (x - this->numbertilde_08_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_08_smooth_index > 0) {
            this->numbertilde_08_smooth_prev += this->numbertilde_08_smooth_increment;
            this->numbertilde_08_smooth_index -= 1;
        } else {
            this->numbertilde_08_smooth_prev = x;
        }
    
        return this->numbertilde_08_smooth_prev;
    }
    
    void numbertilde_08_smooth_reset() {
        this->numbertilde_08_smooth_prev = 0;
        this->numbertilde_08_smooth_index = 0;
        this->numbertilde_08_smooth_increment = 0;
        this->numbertilde_08_smooth_d_reset();
    }
    
    void numbertilde_08_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f4/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_08_dspsetup(bool force) {
        if ((bool)(this->numbertilde_08_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_08_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_08_currentInterval = this->numbertilde_08_currentIntervalInSamples;
        this->numbertilde_08_rampInSamples = this->mstosamps(this->numbertilde_08_ramp);
        this->numbertilde_08_setupDone = true;
        this->numbertilde_08_smooth_d_dspsetup();
    }
    
    void param_08_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_08_value;
    }
    
    void param_08_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_08_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_34_in1 = 0;
        dspexpr_34_in2 = 0;
        dspexpr_35_in1 = 0;
        dspexpr_35_in2 = 0;
        ip_04_value = 1;
        ip_04_impulse = 0;
        dspexpr_36_in1 = 0;
        dspexpr_36_in2 = 0;
        dspexpr_37_in1 = 0;
        dspexpr_37_in2 = 0;
        dspexpr_38_in1 = 0;
        dspexpr_38_in2 = 511;
        dspexpr_39_in1 = 0;
        dspexpr_39_in2 = 0;
        dspexpr_39_in3 = 511;
        dspexpr_40_in1 = 0;
        dspexpr_40_in2 = 0;
        numbertilde_07_input_number = 0;
        numbertilde_07_ramp = 0;
        param_07_value = 0;
        numbertilde_08_input_number = 0;
        numbertilde_08_ramp = 0;
        dspexpr_41_in1 = 0;
        dspexpr_41_in2 = 0;
        dspexpr_41_in3 = 511;
        dspexpr_42_in1 = 0;
        dspexpr_42_in2 = 0;
        dspexpr_43_in1 = 0;
        dspexpr_43_in2 = 511;
        dspexpr_44_in1 = 0;
        dspexpr_44_in2 = 0;
        param_08_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_04_lastIndex = 0;
        ip_04_lastValue = 0;
        ip_04_resetCount = 0;
        ip_04_sigbuf = nullptr;
        ip_04_setupDone = false;
        numbertilde_07_currentInterval = 0;
        numbertilde_07_currentIntervalInSamples = 0;
        numbertilde_07_lastValue = 0;
        numbertilde_07_outValue = 0;
        numbertilde_07_rampInSamples = 0;
        numbertilde_07_currentMode = 1;
        numbertilde_07_smooth_d_prev = 0;
        numbertilde_07_smooth_prev = 0;
        numbertilde_07_smooth_index = 0;
        numbertilde_07_smooth_increment = 0;
        numbertilde_07_setupDone = false;
        param_07_lastValue = 0;
        numbertilde_08_currentInterval = 0;
        numbertilde_08_currentIntervalInSamples = 0;
        numbertilde_08_lastValue = 0;
        numbertilde_08_outValue = 0;
        numbertilde_08_rampInSamples = 0;
        numbertilde_08_currentMode = 1;
        numbertilde_08_smooth_d_prev = 0;
        numbertilde_08_smooth_prev = 0;
        numbertilde_08_smooth_index = 0;
        numbertilde_08_smooth_increment = 0;
        numbertilde_08_setupDone = false;
        param_08_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_34_in1;
        number dspexpr_34_in2;
        number dspexpr_35_in1;
        number dspexpr_35_in2;
        number ip_04_value;
        number ip_04_impulse;
        number dspexpr_36_in1;
        number dspexpr_36_in2;
        number dspexpr_37_in1;
        number dspexpr_37_in2;
        number dspexpr_38_in1;
        number dspexpr_38_in2;
        number dspexpr_39_in1;
        number dspexpr_39_in2;
        number dspexpr_39_in3;
        number dspexpr_40_in1;
        number dspexpr_40_in2;
        number numbertilde_07_input_number;
        number numbertilde_07_ramp;
        number param_07_value;
        number numbertilde_08_input_number;
        number numbertilde_08_ramp;
        number dspexpr_41_in1;
        number dspexpr_41_in2;
        number dspexpr_41_in3;
        number dspexpr_42_in1;
        number dspexpr_42_in2;
        number dspexpr_43_in1;
        number dspexpr_43_in2;
        number dspexpr_44_in1;
        number dspexpr_44_in2;
        number param_08_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_04_lastIndex;
        number ip_04_lastValue;
        SampleIndex ip_04_resetCount;
        signal ip_04_sigbuf;
        bool ip_04_setupDone;
        SampleIndex numbertilde_07_currentInterval;
        SampleIndex numbertilde_07_currentIntervalInSamples;
        number numbertilde_07_lastValue;
        number numbertilde_07_outValue;
        number numbertilde_07_rampInSamples;
        Int numbertilde_07_currentMode;
        number numbertilde_07_smooth_d_prev;
        number numbertilde_07_smooth_prev;
        number numbertilde_07_smooth_index;
        number numbertilde_07_smooth_increment;
        bool numbertilde_07_setupDone;
        number param_07_lastValue;
        SampleIndex numbertilde_08_currentInterval;
        SampleIndex numbertilde_08_currentIntervalInSamples;
        number numbertilde_08_lastValue;
        number numbertilde_08_outValue;
        number numbertilde_08_rampInSamples;
        Int numbertilde_08_currentMode;
        number numbertilde_08_smooth_d_prev;
        number numbertilde_08_smooth_prev;
        number numbertilde_08_smooth_index;
        number numbertilde_08_smooth_increment;
        bool numbertilde_08_setupDone;
        number param_08_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_497 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_497()
    {
    }
    
    ~RNBOSubpatcher_497()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_05_perform(this->signals[0], n);
        this->dspexpr_49_perform(in2, this->dspexpr_49_in2, this->signals[1], n);
        this->dspexpr_51_perform(this->signals[1], this->dspexpr_51_in2, this->signals[2], n);
    
        this->dspexpr_50_perform(
            this->signals[2],
            this->dspexpr_50_in2,
            this->dspexpr_50_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_09_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_47_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_46_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_54_perform(in3, this->dspexpr_54_in2, this->signals[3], n);
        this->dspexpr_55_perform(this->signals[3], this->dspexpr_55_in2, this->signals[0], n);
        this->dspexpr_53_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_52_perform(
            this->signals[3],
            this->dspexpr_52_in2,
            this->dspexpr_52_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_10_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_48_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_45_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_05_sigbuf = resizeSignal(this->ip_05_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_05_dspsetup(forceDSPSetup);
        this->numbertilde_09_dspsetup(forceDSPSetup);
        this->numbertilde_10_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_09_getPresetValue(getSubState(preset, "bin"));
        this->param_10_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_09_value_set(v);
            break;
        case 1:
            this->param_10_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_09_value;
        case 1:
            return this->param_10_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f5/bin";
        case 1:
            return "f5/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-5";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-5";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_09_value_constrain(value);
        case 1:
            return this->param_10_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_09_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_10_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f5/number~_obj-83") == objectId)
                this->numbertilde_09_sig_number_set(payload);
    
            if (TAG("f5/number~_obj-84") == objectId)
                this->numbertilde_10_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f5/number~_obj-83") == objectId)
                this->numbertilde_09_mode_set(payload);
    
            if (TAG("f5/number~_obj-84") == objectId)
                this->numbertilde_10_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f5/number~_obj-83") == objectId)
                this->numbertilde_09_sig_list_set(payload);
    
            if (TAG("f5/number~_obj-84") == objectId)
                this->numbertilde_10_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f5/number~_obj-83"):
            return "f5/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f5/number~_obj-84"):
            return "f5/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_09_value_set(number v) {
        v = this->param_09_value_constrain(v);
        this->param_09_value = v;
        this->sendParameter(0, false);
    
        if (this->param_09_value != this->param_09_lastValue) {
            this->getEngine()->presetTouched();
            this->param_09_lastValue = this->param_09_value;
        }
    
        this->dspexpr_51_in2_set(v);
    }
    
    void param_10_value_set(number v) {
        v = this->param_10_value_constrain(v);
        this->param_10_value = v;
        this->sendParameter(1, false);
    
        if (this->param_10_value != this->param_10_lastValue) {
            this->getEngine()->presetTouched();
            this->param_10_lastValue = this->param_10_value;
        }
    
        this->dspexpr_55_in2_set(v);
    }
    
    void numbertilde_09_sig_number_set(number v) {
        this->numbertilde_09_outValue = v;
    }
    
    void numbertilde_09_sig_list_set(const list& v) {
        this->numbertilde_09_outValue = v[0];
    }
    
    void numbertilde_09_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_09_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_09_currentMode = 1;
        }
    }
    
    void numbertilde_10_sig_number_set(number v) {
        this->numbertilde_10_outValue = v;
    }
    
    void numbertilde_10_sig_list_set(const list& v) {
        this->numbertilde_10_outValue = v[0];
    }
    
    void numbertilde_10_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_10_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_10_currentMode = 1;
        }
    }
    
    void numbertilde_09_value_set(number ) {}
    
    void numbertilde_10_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_05_init();
        this->numbertilde_09_init();
        this->numbertilde_10_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_09_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_51_in2_set(number v) {
        this->dspexpr_51_in2 = v;
    }
    
    static number param_10_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_55_in2_set(number v) {
        this->dspexpr_55_in2 = v;
    }
    
    void ip_05_perform(Sample * out, Index n) {
        auto __ip_05_sigbuf = this->ip_05_sigbuf;
        auto __ip_05_lastValue = this->ip_05_lastValue;
        auto __ip_05_lastIndex = this->ip_05_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_05_lastIndex ? __ip_05_lastValue : __ip_05_sigbuf[(Index)i]);
        }
    
        __ip_05_lastIndex = 0;
        this->ip_05_lastIndex = __ip_05_lastIndex;
    }
    
    void dspexpr_49_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_51_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_50_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_09_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_09_currentIntervalInSamples = this->numbertilde_09_currentIntervalInSamples;
        auto __numbertilde_09_lastValue = this->numbertilde_09_lastValue;
        auto __numbertilde_09_currentInterval = this->numbertilde_09_currentInterval;
        auto __numbertilde_09_rampInSamples = this->numbertilde_09_rampInSamples;
        auto __numbertilde_09_outValue = this->numbertilde_09_outValue;
        auto __numbertilde_09_currentMode = this->numbertilde_09_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_09_currentMode == 0) {
                output[(Index)i] = this->numbertilde_09_smooth_next(
                    __numbertilde_09_outValue,
                    __numbertilde_09_rampInSamples,
                    __numbertilde_09_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_09_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_09_lastValue && __numbertilde_09_currentInterval <= 0) {
            __numbertilde_09_currentInterval = __numbertilde_09_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_09_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f5/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_09_currentInterval = __numbertilde_09_currentInterval;
        this->numbertilde_09_lastValue = __numbertilde_09_lastValue;
    }
    
    void dspexpr_47_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_46_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_54_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_55_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_53_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_52_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_10_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_10_currentIntervalInSamples = this->numbertilde_10_currentIntervalInSamples;
        auto __numbertilde_10_lastValue = this->numbertilde_10_lastValue;
        auto __numbertilde_10_currentInterval = this->numbertilde_10_currentInterval;
        auto __numbertilde_10_rampInSamples = this->numbertilde_10_rampInSamples;
        auto __numbertilde_10_outValue = this->numbertilde_10_outValue;
        auto __numbertilde_10_currentMode = this->numbertilde_10_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_10_currentMode == 0) {
                output[(Index)i] = this->numbertilde_10_smooth_next(
                    __numbertilde_10_outValue,
                    __numbertilde_10_rampInSamples,
                    __numbertilde_10_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_10_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_10_lastValue && __numbertilde_10_currentInterval <= 0) {
            __numbertilde_10_currentInterval = __numbertilde_10_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_10_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f5/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_10_currentInterval = __numbertilde_10_currentInterval;
        this->numbertilde_10_lastValue = __numbertilde_10_lastValue;
    }
    
    void dspexpr_48_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_45_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_05_init() {
        this->ip_05_lastValue = this->ip_05_value;
    }
    
    void ip_05_dspsetup(bool force) {
        if ((bool)(this->ip_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_05_lastIndex = 0;
        this->ip_05_setupDone = true;
    }
    
    number numbertilde_09_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_09_smooth_d_prev);
        this->numbertilde_09_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_09_smooth_d_dspsetup() {
        this->numbertilde_09_smooth_d_reset();
    }
    
    void numbertilde_09_smooth_d_reset() {
        this->numbertilde_09_smooth_d_prev = 0;
    }
    
    number numbertilde_09_smooth_next(number x, number up, number down) {
        if (this->numbertilde_09_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_09_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_09_smooth_index = _up;
                this->numbertilde_09_smooth_increment = (x - this->numbertilde_09_smooth_prev) / _up;
            } else if (x < this->numbertilde_09_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_09_smooth_index = _down;
                this->numbertilde_09_smooth_increment = (x - this->numbertilde_09_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_09_smooth_index > 0) {
            this->numbertilde_09_smooth_prev += this->numbertilde_09_smooth_increment;
            this->numbertilde_09_smooth_index -= 1;
        } else {
            this->numbertilde_09_smooth_prev = x;
        }
    
        return this->numbertilde_09_smooth_prev;
    }
    
    void numbertilde_09_smooth_reset() {
        this->numbertilde_09_smooth_prev = 0;
        this->numbertilde_09_smooth_index = 0;
        this->numbertilde_09_smooth_increment = 0;
        this->numbertilde_09_smooth_d_reset();
    }
    
    void numbertilde_09_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f5/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_09_dspsetup(bool force) {
        if ((bool)(this->numbertilde_09_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_09_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_09_currentInterval = this->numbertilde_09_currentIntervalInSamples;
        this->numbertilde_09_rampInSamples = this->mstosamps(this->numbertilde_09_ramp);
        this->numbertilde_09_setupDone = true;
        this->numbertilde_09_smooth_d_dspsetup();
    }
    
    void param_09_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_09_value;
    }
    
    void param_09_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_09_value_set(preset["value"]);
    }
    
    number numbertilde_10_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_10_smooth_d_prev);
        this->numbertilde_10_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_10_smooth_d_dspsetup() {
        this->numbertilde_10_smooth_d_reset();
    }
    
    void numbertilde_10_smooth_d_reset() {
        this->numbertilde_10_smooth_d_prev = 0;
    }
    
    number numbertilde_10_smooth_next(number x, number up, number down) {
        if (this->numbertilde_10_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_10_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_10_smooth_index = _up;
                this->numbertilde_10_smooth_increment = (x - this->numbertilde_10_smooth_prev) / _up;
            } else if (x < this->numbertilde_10_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_10_smooth_index = _down;
                this->numbertilde_10_smooth_increment = (x - this->numbertilde_10_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_10_smooth_index > 0) {
            this->numbertilde_10_smooth_prev += this->numbertilde_10_smooth_increment;
            this->numbertilde_10_smooth_index -= 1;
        } else {
            this->numbertilde_10_smooth_prev = x;
        }
    
        return this->numbertilde_10_smooth_prev;
    }
    
    void numbertilde_10_smooth_reset() {
        this->numbertilde_10_smooth_prev = 0;
        this->numbertilde_10_smooth_index = 0;
        this->numbertilde_10_smooth_increment = 0;
        this->numbertilde_10_smooth_d_reset();
    }
    
    void numbertilde_10_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f5/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_10_dspsetup(bool force) {
        if ((bool)(this->numbertilde_10_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_10_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_10_currentInterval = this->numbertilde_10_currentIntervalInSamples;
        this->numbertilde_10_rampInSamples = this->mstosamps(this->numbertilde_10_ramp);
        this->numbertilde_10_setupDone = true;
        this->numbertilde_10_smooth_d_dspsetup();
    }
    
    void param_10_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_10_value;
    }
    
    void param_10_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_10_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_45_in1 = 0;
        dspexpr_45_in2 = 0;
        dspexpr_46_in1 = 0;
        dspexpr_46_in2 = 0;
        ip_05_value = 1;
        ip_05_impulse = 0;
        dspexpr_47_in1 = 0;
        dspexpr_47_in2 = 0;
        dspexpr_48_in1 = 0;
        dspexpr_48_in2 = 0;
        dspexpr_49_in1 = 0;
        dspexpr_49_in2 = 511;
        dspexpr_50_in1 = 0;
        dspexpr_50_in2 = 0;
        dspexpr_50_in3 = 511;
        dspexpr_51_in1 = 0;
        dspexpr_51_in2 = 0;
        numbertilde_09_input_number = 0;
        numbertilde_09_ramp = 0;
        param_09_value = 0;
        numbertilde_10_input_number = 0;
        numbertilde_10_ramp = 0;
        dspexpr_52_in1 = 0;
        dspexpr_52_in2 = 0;
        dspexpr_52_in3 = 511;
        dspexpr_53_in1 = 0;
        dspexpr_53_in2 = 0;
        dspexpr_54_in1 = 0;
        dspexpr_54_in2 = 511;
        dspexpr_55_in1 = 0;
        dspexpr_55_in2 = 0;
        param_10_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_05_lastIndex = 0;
        ip_05_lastValue = 0;
        ip_05_resetCount = 0;
        ip_05_sigbuf = nullptr;
        ip_05_setupDone = false;
        numbertilde_09_currentInterval = 0;
        numbertilde_09_currentIntervalInSamples = 0;
        numbertilde_09_lastValue = 0;
        numbertilde_09_outValue = 0;
        numbertilde_09_rampInSamples = 0;
        numbertilde_09_currentMode = 1;
        numbertilde_09_smooth_d_prev = 0;
        numbertilde_09_smooth_prev = 0;
        numbertilde_09_smooth_index = 0;
        numbertilde_09_smooth_increment = 0;
        numbertilde_09_setupDone = false;
        param_09_lastValue = 0;
        numbertilde_10_currentInterval = 0;
        numbertilde_10_currentIntervalInSamples = 0;
        numbertilde_10_lastValue = 0;
        numbertilde_10_outValue = 0;
        numbertilde_10_rampInSamples = 0;
        numbertilde_10_currentMode = 1;
        numbertilde_10_smooth_d_prev = 0;
        numbertilde_10_smooth_prev = 0;
        numbertilde_10_smooth_index = 0;
        numbertilde_10_smooth_increment = 0;
        numbertilde_10_setupDone = false;
        param_10_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_45_in1;
        number dspexpr_45_in2;
        number dspexpr_46_in1;
        number dspexpr_46_in2;
        number ip_05_value;
        number ip_05_impulse;
        number dspexpr_47_in1;
        number dspexpr_47_in2;
        number dspexpr_48_in1;
        number dspexpr_48_in2;
        number dspexpr_49_in1;
        number dspexpr_49_in2;
        number dspexpr_50_in1;
        number dspexpr_50_in2;
        number dspexpr_50_in3;
        number dspexpr_51_in1;
        number dspexpr_51_in2;
        number numbertilde_09_input_number;
        number numbertilde_09_ramp;
        number param_09_value;
        number numbertilde_10_input_number;
        number numbertilde_10_ramp;
        number dspexpr_52_in1;
        number dspexpr_52_in2;
        number dspexpr_52_in3;
        number dspexpr_53_in1;
        number dspexpr_53_in2;
        number dspexpr_54_in1;
        number dspexpr_54_in2;
        number dspexpr_55_in1;
        number dspexpr_55_in2;
        number param_10_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_05_lastIndex;
        number ip_05_lastValue;
        SampleIndex ip_05_resetCount;
        signal ip_05_sigbuf;
        bool ip_05_setupDone;
        SampleIndex numbertilde_09_currentInterval;
        SampleIndex numbertilde_09_currentIntervalInSamples;
        number numbertilde_09_lastValue;
        number numbertilde_09_outValue;
        number numbertilde_09_rampInSamples;
        Int numbertilde_09_currentMode;
        number numbertilde_09_smooth_d_prev;
        number numbertilde_09_smooth_prev;
        number numbertilde_09_smooth_index;
        number numbertilde_09_smooth_increment;
        bool numbertilde_09_setupDone;
        number param_09_lastValue;
        SampleIndex numbertilde_10_currentInterval;
        SampleIndex numbertilde_10_currentIntervalInSamples;
        number numbertilde_10_lastValue;
        number numbertilde_10_outValue;
        number numbertilde_10_rampInSamples;
        Int numbertilde_10_currentMode;
        number numbertilde_10_smooth_d_prev;
        number numbertilde_10_smooth_prev;
        number numbertilde_10_smooth_index;
        number numbertilde_10_smooth_increment;
        bool numbertilde_10_setupDone;
        number param_10_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_498 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_498()
    {
    }
    
    ~RNBOSubpatcher_498()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_06_perform(this->signals[0], n);
        this->dspexpr_60_perform(in2, this->dspexpr_60_in2, this->signals[1], n);
        this->dspexpr_62_perform(this->signals[1], this->dspexpr_62_in2, this->signals[2], n);
    
        this->dspexpr_61_perform(
            this->signals[2],
            this->dspexpr_61_in2,
            this->dspexpr_61_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_11_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_58_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_57_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_65_perform(in3, this->dspexpr_65_in2, this->signals[3], n);
        this->dspexpr_66_perform(this->signals[3], this->dspexpr_66_in2, this->signals[0], n);
        this->dspexpr_64_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_63_perform(
            this->signals[3],
            this->dspexpr_63_in2,
            this->dspexpr_63_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_12_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_59_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_56_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_06_sigbuf = resizeSignal(this->ip_06_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_06_dspsetup(forceDSPSetup);
        this->numbertilde_11_dspsetup(forceDSPSetup);
        this->numbertilde_12_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_11_getPresetValue(getSubState(preset, "bin"));
        this->param_12_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_11_value_set(v);
            break;
        case 1:
            this->param_12_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_11_value;
        case 1:
            return this->param_12_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f6/bin";
        case 1:
            return "f6/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-6";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-6";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_11_value_constrain(value);
        case 1:
            return this->param_12_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_11_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_12_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f6/number~_obj-83") == objectId)
                this->numbertilde_11_sig_number_set(payload);
    
            if (TAG("f6/number~_obj-84") == objectId)
                this->numbertilde_12_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f6/number~_obj-83") == objectId)
                this->numbertilde_11_mode_set(payload);
    
            if (TAG("f6/number~_obj-84") == objectId)
                this->numbertilde_12_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f6/number~_obj-83") == objectId)
                this->numbertilde_11_sig_list_set(payload);
    
            if (TAG("f6/number~_obj-84") == objectId)
                this->numbertilde_12_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f6/number~_obj-83"):
            return "f6/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f6/number~_obj-84"):
            return "f6/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_11_value_set(number v) {
        v = this->param_11_value_constrain(v);
        this->param_11_value = v;
        this->sendParameter(0, false);
    
        if (this->param_11_value != this->param_11_lastValue) {
            this->getEngine()->presetTouched();
            this->param_11_lastValue = this->param_11_value;
        }
    
        this->dspexpr_62_in2_set(v);
    }
    
    void param_12_value_set(number v) {
        v = this->param_12_value_constrain(v);
        this->param_12_value = v;
        this->sendParameter(1, false);
    
        if (this->param_12_value != this->param_12_lastValue) {
            this->getEngine()->presetTouched();
            this->param_12_lastValue = this->param_12_value;
        }
    
        this->dspexpr_66_in2_set(v);
    }
    
    void numbertilde_11_sig_number_set(number v) {
        this->numbertilde_11_outValue = v;
    }
    
    void numbertilde_11_sig_list_set(const list& v) {
        this->numbertilde_11_outValue = v[0];
    }
    
    void numbertilde_11_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_11_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_11_currentMode = 1;
        }
    }
    
    void numbertilde_12_sig_number_set(number v) {
        this->numbertilde_12_outValue = v;
    }
    
    void numbertilde_12_sig_list_set(const list& v) {
        this->numbertilde_12_outValue = v[0];
    }
    
    void numbertilde_12_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_12_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_12_currentMode = 1;
        }
    }
    
    void numbertilde_11_value_set(number ) {}
    
    void numbertilde_12_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_06_init();
        this->numbertilde_11_init();
        this->numbertilde_12_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_11_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_62_in2_set(number v) {
        this->dspexpr_62_in2 = v;
    }
    
    static number param_12_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_66_in2_set(number v) {
        this->dspexpr_66_in2 = v;
    }
    
    void ip_06_perform(Sample * out, Index n) {
        auto __ip_06_sigbuf = this->ip_06_sigbuf;
        auto __ip_06_lastValue = this->ip_06_lastValue;
        auto __ip_06_lastIndex = this->ip_06_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_06_lastIndex ? __ip_06_lastValue : __ip_06_sigbuf[(Index)i]);
        }
    
        __ip_06_lastIndex = 0;
        this->ip_06_lastIndex = __ip_06_lastIndex;
    }
    
    void dspexpr_60_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_62_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_61_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_11_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_11_currentIntervalInSamples = this->numbertilde_11_currentIntervalInSamples;
        auto __numbertilde_11_lastValue = this->numbertilde_11_lastValue;
        auto __numbertilde_11_currentInterval = this->numbertilde_11_currentInterval;
        auto __numbertilde_11_rampInSamples = this->numbertilde_11_rampInSamples;
        auto __numbertilde_11_outValue = this->numbertilde_11_outValue;
        auto __numbertilde_11_currentMode = this->numbertilde_11_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_11_currentMode == 0) {
                output[(Index)i] = this->numbertilde_11_smooth_next(
                    __numbertilde_11_outValue,
                    __numbertilde_11_rampInSamples,
                    __numbertilde_11_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_11_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_11_lastValue && __numbertilde_11_currentInterval <= 0) {
            __numbertilde_11_currentInterval = __numbertilde_11_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_11_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f6/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_11_currentInterval = __numbertilde_11_currentInterval;
        this->numbertilde_11_lastValue = __numbertilde_11_lastValue;
    }
    
    void dspexpr_58_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_57_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_65_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_66_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_64_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_63_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_12_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_12_currentIntervalInSamples = this->numbertilde_12_currentIntervalInSamples;
        auto __numbertilde_12_lastValue = this->numbertilde_12_lastValue;
        auto __numbertilde_12_currentInterval = this->numbertilde_12_currentInterval;
        auto __numbertilde_12_rampInSamples = this->numbertilde_12_rampInSamples;
        auto __numbertilde_12_outValue = this->numbertilde_12_outValue;
        auto __numbertilde_12_currentMode = this->numbertilde_12_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_12_currentMode == 0) {
                output[(Index)i] = this->numbertilde_12_smooth_next(
                    __numbertilde_12_outValue,
                    __numbertilde_12_rampInSamples,
                    __numbertilde_12_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_12_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_12_lastValue && __numbertilde_12_currentInterval <= 0) {
            __numbertilde_12_currentInterval = __numbertilde_12_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_12_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f6/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_12_currentInterval = __numbertilde_12_currentInterval;
        this->numbertilde_12_lastValue = __numbertilde_12_lastValue;
    }
    
    void dspexpr_59_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_56_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_06_init() {
        this->ip_06_lastValue = this->ip_06_value;
    }
    
    void ip_06_dspsetup(bool force) {
        if ((bool)(this->ip_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_06_lastIndex = 0;
        this->ip_06_setupDone = true;
    }
    
    number numbertilde_11_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_11_smooth_d_prev);
        this->numbertilde_11_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_11_smooth_d_dspsetup() {
        this->numbertilde_11_smooth_d_reset();
    }
    
    void numbertilde_11_smooth_d_reset() {
        this->numbertilde_11_smooth_d_prev = 0;
    }
    
    number numbertilde_11_smooth_next(number x, number up, number down) {
        if (this->numbertilde_11_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_11_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_11_smooth_index = _up;
                this->numbertilde_11_smooth_increment = (x - this->numbertilde_11_smooth_prev) / _up;
            } else if (x < this->numbertilde_11_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_11_smooth_index = _down;
                this->numbertilde_11_smooth_increment = (x - this->numbertilde_11_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_11_smooth_index > 0) {
            this->numbertilde_11_smooth_prev += this->numbertilde_11_smooth_increment;
            this->numbertilde_11_smooth_index -= 1;
        } else {
            this->numbertilde_11_smooth_prev = x;
        }
    
        return this->numbertilde_11_smooth_prev;
    }
    
    void numbertilde_11_smooth_reset() {
        this->numbertilde_11_smooth_prev = 0;
        this->numbertilde_11_smooth_index = 0;
        this->numbertilde_11_smooth_increment = 0;
        this->numbertilde_11_smooth_d_reset();
    }
    
    void numbertilde_11_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f6/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_11_dspsetup(bool force) {
        if ((bool)(this->numbertilde_11_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_11_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_11_currentInterval = this->numbertilde_11_currentIntervalInSamples;
        this->numbertilde_11_rampInSamples = this->mstosamps(this->numbertilde_11_ramp);
        this->numbertilde_11_setupDone = true;
        this->numbertilde_11_smooth_d_dspsetup();
    }
    
    void param_11_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_11_value;
    }
    
    void param_11_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_11_value_set(preset["value"]);
    }
    
    number numbertilde_12_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_12_smooth_d_prev);
        this->numbertilde_12_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_12_smooth_d_dspsetup() {
        this->numbertilde_12_smooth_d_reset();
    }
    
    void numbertilde_12_smooth_d_reset() {
        this->numbertilde_12_smooth_d_prev = 0;
    }
    
    number numbertilde_12_smooth_next(number x, number up, number down) {
        if (this->numbertilde_12_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_12_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_12_smooth_index = _up;
                this->numbertilde_12_smooth_increment = (x - this->numbertilde_12_smooth_prev) / _up;
            } else if (x < this->numbertilde_12_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_12_smooth_index = _down;
                this->numbertilde_12_smooth_increment = (x - this->numbertilde_12_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_12_smooth_index > 0) {
            this->numbertilde_12_smooth_prev += this->numbertilde_12_smooth_increment;
            this->numbertilde_12_smooth_index -= 1;
        } else {
            this->numbertilde_12_smooth_prev = x;
        }
    
        return this->numbertilde_12_smooth_prev;
    }
    
    void numbertilde_12_smooth_reset() {
        this->numbertilde_12_smooth_prev = 0;
        this->numbertilde_12_smooth_index = 0;
        this->numbertilde_12_smooth_increment = 0;
        this->numbertilde_12_smooth_d_reset();
    }
    
    void numbertilde_12_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f6/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_12_dspsetup(bool force) {
        if ((bool)(this->numbertilde_12_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_12_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_12_currentInterval = this->numbertilde_12_currentIntervalInSamples;
        this->numbertilde_12_rampInSamples = this->mstosamps(this->numbertilde_12_ramp);
        this->numbertilde_12_setupDone = true;
        this->numbertilde_12_smooth_d_dspsetup();
    }
    
    void param_12_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_12_value;
    }
    
    void param_12_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_12_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_56_in1 = 0;
        dspexpr_56_in2 = 0;
        dspexpr_57_in1 = 0;
        dspexpr_57_in2 = 0;
        ip_06_value = 1;
        ip_06_impulse = 0;
        dspexpr_58_in1 = 0;
        dspexpr_58_in2 = 0;
        dspexpr_59_in1 = 0;
        dspexpr_59_in2 = 0;
        dspexpr_60_in1 = 0;
        dspexpr_60_in2 = 511;
        dspexpr_61_in1 = 0;
        dspexpr_61_in2 = 0;
        dspexpr_61_in3 = 511;
        dspexpr_62_in1 = 0;
        dspexpr_62_in2 = 0;
        numbertilde_11_input_number = 0;
        numbertilde_11_ramp = 0;
        param_11_value = 0;
        numbertilde_12_input_number = 0;
        numbertilde_12_ramp = 0;
        dspexpr_63_in1 = 0;
        dspexpr_63_in2 = 0;
        dspexpr_63_in3 = 511;
        dspexpr_64_in1 = 0;
        dspexpr_64_in2 = 0;
        dspexpr_65_in1 = 0;
        dspexpr_65_in2 = 511;
        dspexpr_66_in1 = 0;
        dspexpr_66_in2 = 0;
        param_12_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_06_lastIndex = 0;
        ip_06_lastValue = 0;
        ip_06_resetCount = 0;
        ip_06_sigbuf = nullptr;
        ip_06_setupDone = false;
        numbertilde_11_currentInterval = 0;
        numbertilde_11_currentIntervalInSamples = 0;
        numbertilde_11_lastValue = 0;
        numbertilde_11_outValue = 0;
        numbertilde_11_rampInSamples = 0;
        numbertilde_11_currentMode = 1;
        numbertilde_11_smooth_d_prev = 0;
        numbertilde_11_smooth_prev = 0;
        numbertilde_11_smooth_index = 0;
        numbertilde_11_smooth_increment = 0;
        numbertilde_11_setupDone = false;
        param_11_lastValue = 0;
        numbertilde_12_currentInterval = 0;
        numbertilde_12_currentIntervalInSamples = 0;
        numbertilde_12_lastValue = 0;
        numbertilde_12_outValue = 0;
        numbertilde_12_rampInSamples = 0;
        numbertilde_12_currentMode = 1;
        numbertilde_12_smooth_d_prev = 0;
        numbertilde_12_smooth_prev = 0;
        numbertilde_12_smooth_index = 0;
        numbertilde_12_smooth_increment = 0;
        numbertilde_12_setupDone = false;
        param_12_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_56_in1;
        number dspexpr_56_in2;
        number dspexpr_57_in1;
        number dspexpr_57_in2;
        number ip_06_value;
        number ip_06_impulse;
        number dspexpr_58_in1;
        number dspexpr_58_in2;
        number dspexpr_59_in1;
        number dspexpr_59_in2;
        number dspexpr_60_in1;
        number dspexpr_60_in2;
        number dspexpr_61_in1;
        number dspexpr_61_in2;
        number dspexpr_61_in3;
        number dspexpr_62_in1;
        number dspexpr_62_in2;
        number numbertilde_11_input_number;
        number numbertilde_11_ramp;
        number param_11_value;
        number numbertilde_12_input_number;
        number numbertilde_12_ramp;
        number dspexpr_63_in1;
        number dspexpr_63_in2;
        number dspexpr_63_in3;
        number dspexpr_64_in1;
        number dspexpr_64_in2;
        number dspexpr_65_in1;
        number dspexpr_65_in2;
        number dspexpr_66_in1;
        number dspexpr_66_in2;
        number param_12_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_06_lastIndex;
        number ip_06_lastValue;
        SampleIndex ip_06_resetCount;
        signal ip_06_sigbuf;
        bool ip_06_setupDone;
        SampleIndex numbertilde_11_currentInterval;
        SampleIndex numbertilde_11_currentIntervalInSamples;
        number numbertilde_11_lastValue;
        number numbertilde_11_outValue;
        number numbertilde_11_rampInSamples;
        Int numbertilde_11_currentMode;
        number numbertilde_11_smooth_d_prev;
        number numbertilde_11_smooth_prev;
        number numbertilde_11_smooth_index;
        number numbertilde_11_smooth_increment;
        bool numbertilde_11_setupDone;
        number param_11_lastValue;
        SampleIndex numbertilde_12_currentInterval;
        SampleIndex numbertilde_12_currentIntervalInSamples;
        number numbertilde_12_lastValue;
        number numbertilde_12_outValue;
        number numbertilde_12_rampInSamples;
        Int numbertilde_12_currentMode;
        number numbertilde_12_smooth_d_prev;
        number numbertilde_12_smooth_prev;
        number numbertilde_12_smooth_index;
        number numbertilde_12_smooth_increment;
        bool numbertilde_12_setupDone;
        number param_12_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_499 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_499()
    {
    }
    
    ~RNBOSubpatcher_499()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_07_perform(this->signals[0], n);
        this->dspexpr_71_perform(in2, this->dspexpr_71_in2, this->signals[1], n);
        this->dspexpr_73_perform(this->signals[1], this->dspexpr_73_in2, this->signals[2], n);
    
        this->dspexpr_72_perform(
            this->signals[2],
            this->dspexpr_72_in2,
            this->dspexpr_72_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_13_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_69_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_68_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_76_perform(in3, this->dspexpr_76_in2, this->signals[3], n);
        this->dspexpr_77_perform(this->signals[3], this->dspexpr_77_in2, this->signals[0], n);
        this->dspexpr_75_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_74_perform(
            this->signals[3],
            this->dspexpr_74_in2,
            this->dspexpr_74_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_14_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_70_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_67_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_07_sigbuf = resizeSignal(this->ip_07_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_07_dspsetup(forceDSPSetup);
        this->numbertilde_13_dspsetup(forceDSPSetup);
        this->numbertilde_14_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_13_getPresetValue(getSubState(preset, "bin"));
        this->param_14_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_13_value_set(v);
            break;
        case 1:
            this->param_14_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_13_value;
        case 1:
            return this->param_14_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f7/bin";
        case 1:
            return "f7/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-7";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-7";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_13_value_constrain(value);
        case 1:
            return this->param_14_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_13_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_14_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f7/number~_obj-83") == objectId)
                this->numbertilde_13_sig_number_set(payload);
    
            if (TAG("f7/number~_obj-84") == objectId)
                this->numbertilde_14_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f7/number~_obj-83") == objectId)
                this->numbertilde_13_mode_set(payload);
    
            if (TAG("f7/number~_obj-84") == objectId)
                this->numbertilde_14_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f7/number~_obj-83") == objectId)
                this->numbertilde_13_sig_list_set(payload);
    
            if (TAG("f7/number~_obj-84") == objectId)
                this->numbertilde_14_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f7/number~_obj-83"):
            return "f7/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f7/number~_obj-84"):
            return "f7/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_13_value_set(number v) {
        v = this->param_13_value_constrain(v);
        this->param_13_value = v;
        this->sendParameter(0, false);
    
        if (this->param_13_value != this->param_13_lastValue) {
            this->getEngine()->presetTouched();
            this->param_13_lastValue = this->param_13_value;
        }
    
        this->dspexpr_73_in2_set(v);
    }
    
    void param_14_value_set(number v) {
        v = this->param_14_value_constrain(v);
        this->param_14_value = v;
        this->sendParameter(1, false);
    
        if (this->param_14_value != this->param_14_lastValue) {
            this->getEngine()->presetTouched();
            this->param_14_lastValue = this->param_14_value;
        }
    
        this->dspexpr_77_in2_set(v);
    }
    
    void numbertilde_13_sig_number_set(number v) {
        this->numbertilde_13_outValue = v;
    }
    
    void numbertilde_13_sig_list_set(const list& v) {
        this->numbertilde_13_outValue = v[0];
    }
    
    void numbertilde_13_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_13_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_13_currentMode = 1;
        }
    }
    
    void numbertilde_14_sig_number_set(number v) {
        this->numbertilde_14_outValue = v;
    }
    
    void numbertilde_14_sig_list_set(const list& v) {
        this->numbertilde_14_outValue = v[0];
    }
    
    void numbertilde_14_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_14_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_14_currentMode = 1;
        }
    }
    
    void numbertilde_13_value_set(number ) {}
    
    void numbertilde_14_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_07_init();
        this->numbertilde_13_init();
        this->numbertilde_14_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_13_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_73_in2_set(number v) {
        this->dspexpr_73_in2 = v;
    }
    
    static number param_14_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_77_in2_set(number v) {
        this->dspexpr_77_in2 = v;
    }
    
    void ip_07_perform(Sample * out, Index n) {
        auto __ip_07_sigbuf = this->ip_07_sigbuf;
        auto __ip_07_lastValue = this->ip_07_lastValue;
        auto __ip_07_lastIndex = this->ip_07_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_07_lastIndex ? __ip_07_lastValue : __ip_07_sigbuf[(Index)i]);
        }
    
        __ip_07_lastIndex = 0;
        this->ip_07_lastIndex = __ip_07_lastIndex;
    }
    
    void dspexpr_71_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_73_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_72_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_13_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_13_currentIntervalInSamples = this->numbertilde_13_currentIntervalInSamples;
        auto __numbertilde_13_lastValue = this->numbertilde_13_lastValue;
        auto __numbertilde_13_currentInterval = this->numbertilde_13_currentInterval;
        auto __numbertilde_13_rampInSamples = this->numbertilde_13_rampInSamples;
        auto __numbertilde_13_outValue = this->numbertilde_13_outValue;
        auto __numbertilde_13_currentMode = this->numbertilde_13_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_13_currentMode == 0) {
                output[(Index)i] = this->numbertilde_13_smooth_next(
                    __numbertilde_13_outValue,
                    __numbertilde_13_rampInSamples,
                    __numbertilde_13_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_13_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_13_lastValue && __numbertilde_13_currentInterval <= 0) {
            __numbertilde_13_currentInterval = __numbertilde_13_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_13_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f7/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_13_currentInterval = __numbertilde_13_currentInterval;
        this->numbertilde_13_lastValue = __numbertilde_13_lastValue;
    }
    
    void dspexpr_69_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_68_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_76_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_77_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_75_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_74_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_14_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_14_currentIntervalInSamples = this->numbertilde_14_currentIntervalInSamples;
        auto __numbertilde_14_lastValue = this->numbertilde_14_lastValue;
        auto __numbertilde_14_currentInterval = this->numbertilde_14_currentInterval;
        auto __numbertilde_14_rampInSamples = this->numbertilde_14_rampInSamples;
        auto __numbertilde_14_outValue = this->numbertilde_14_outValue;
        auto __numbertilde_14_currentMode = this->numbertilde_14_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_14_currentMode == 0) {
                output[(Index)i] = this->numbertilde_14_smooth_next(
                    __numbertilde_14_outValue,
                    __numbertilde_14_rampInSamples,
                    __numbertilde_14_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_14_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_14_lastValue && __numbertilde_14_currentInterval <= 0) {
            __numbertilde_14_currentInterval = __numbertilde_14_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_14_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f7/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_14_currentInterval = __numbertilde_14_currentInterval;
        this->numbertilde_14_lastValue = __numbertilde_14_lastValue;
    }
    
    void dspexpr_70_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_67_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_07_init() {
        this->ip_07_lastValue = this->ip_07_value;
    }
    
    void ip_07_dspsetup(bool force) {
        if ((bool)(this->ip_07_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_07_lastIndex = 0;
        this->ip_07_setupDone = true;
    }
    
    number numbertilde_13_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_13_smooth_d_prev);
        this->numbertilde_13_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_13_smooth_d_dspsetup() {
        this->numbertilde_13_smooth_d_reset();
    }
    
    void numbertilde_13_smooth_d_reset() {
        this->numbertilde_13_smooth_d_prev = 0;
    }
    
    number numbertilde_13_smooth_next(number x, number up, number down) {
        if (this->numbertilde_13_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_13_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_13_smooth_index = _up;
                this->numbertilde_13_smooth_increment = (x - this->numbertilde_13_smooth_prev) / _up;
            } else if (x < this->numbertilde_13_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_13_smooth_index = _down;
                this->numbertilde_13_smooth_increment = (x - this->numbertilde_13_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_13_smooth_index > 0) {
            this->numbertilde_13_smooth_prev += this->numbertilde_13_smooth_increment;
            this->numbertilde_13_smooth_index -= 1;
        } else {
            this->numbertilde_13_smooth_prev = x;
        }
    
        return this->numbertilde_13_smooth_prev;
    }
    
    void numbertilde_13_smooth_reset() {
        this->numbertilde_13_smooth_prev = 0;
        this->numbertilde_13_smooth_index = 0;
        this->numbertilde_13_smooth_increment = 0;
        this->numbertilde_13_smooth_d_reset();
    }
    
    void numbertilde_13_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f7/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_13_dspsetup(bool force) {
        if ((bool)(this->numbertilde_13_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_13_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_13_currentInterval = this->numbertilde_13_currentIntervalInSamples;
        this->numbertilde_13_rampInSamples = this->mstosamps(this->numbertilde_13_ramp);
        this->numbertilde_13_setupDone = true;
        this->numbertilde_13_smooth_d_dspsetup();
    }
    
    void param_13_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_13_value;
    }
    
    void param_13_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_13_value_set(preset["value"]);
    }
    
    number numbertilde_14_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_14_smooth_d_prev);
        this->numbertilde_14_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_14_smooth_d_dspsetup() {
        this->numbertilde_14_smooth_d_reset();
    }
    
    void numbertilde_14_smooth_d_reset() {
        this->numbertilde_14_smooth_d_prev = 0;
    }
    
    number numbertilde_14_smooth_next(number x, number up, number down) {
        if (this->numbertilde_14_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_14_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_14_smooth_index = _up;
                this->numbertilde_14_smooth_increment = (x - this->numbertilde_14_smooth_prev) / _up;
            } else if (x < this->numbertilde_14_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_14_smooth_index = _down;
                this->numbertilde_14_smooth_increment = (x - this->numbertilde_14_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_14_smooth_index > 0) {
            this->numbertilde_14_smooth_prev += this->numbertilde_14_smooth_increment;
            this->numbertilde_14_smooth_index -= 1;
        } else {
            this->numbertilde_14_smooth_prev = x;
        }
    
        return this->numbertilde_14_smooth_prev;
    }
    
    void numbertilde_14_smooth_reset() {
        this->numbertilde_14_smooth_prev = 0;
        this->numbertilde_14_smooth_index = 0;
        this->numbertilde_14_smooth_increment = 0;
        this->numbertilde_14_smooth_d_reset();
    }
    
    void numbertilde_14_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f7/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_14_dspsetup(bool force) {
        if ((bool)(this->numbertilde_14_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_14_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_14_currentInterval = this->numbertilde_14_currentIntervalInSamples;
        this->numbertilde_14_rampInSamples = this->mstosamps(this->numbertilde_14_ramp);
        this->numbertilde_14_setupDone = true;
        this->numbertilde_14_smooth_d_dspsetup();
    }
    
    void param_14_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_14_value;
    }
    
    void param_14_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_14_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_67_in1 = 0;
        dspexpr_67_in2 = 0;
        dspexpr_68_in1 = 0;
        dspexpr_68_in2 = 0;
        ip_07_value = 1;
        ip_07_impulse = 0;
        dspexpr_69_in1 = 0;
        dspexpr_69_in2 = 0;
        dspexpr_70_in1 = 0;
        dspexpr_70_in2 = 0;
        dspexpr_71_in1 = 0;
        dspexpr_71_in2 = 511;
        dspexpr_72_in1 = 0;
        dspexpr_72_in2 = 0;
        dspexpr_72_in3 = 511;
        dspexpr_73_in1 = 0;
        dspexpr_73_in2 = 0;
        numbertilde_13_input_number = 0;
        numbertilde_13_ramp = 0;
        param_13_value = 0;
        numbertilde_14_input_number = 0;
        numbertilde_14_ramp = 0;
        dspexpr_74_in1 = 0;
        dspexpr_74_in2 = 0;
        dspexpr_74_in3 = 511;
        dspexpr_75_in1 = 0;
        dspexpr_75_in2 = 0;
        dspexpr_76_in1 = 0;
        dspexpr_76_in2 = 511;
        dspexpr_77_in1 = 0;
        dspexpr_77_in2 = 0;
        param_14_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_07_lastIndex = 0;
        ip_07_lastValue = 0;
        ip_07_resetCount = 0;
        ip_07_sigbuf = nullptr;
        ip_07_setupDone = false;
        numbertilde_13_currentInterval = 0;
        numbertilde_13_currentIntervalInSamples = 0;
        numbertilde_13_lastValue = 0;
        numbertilde_13_outValue = 0;
        numbertilde_13_rampInSamples = 0;
        numbertilde_13_currentMode = 1;
        numbertilde_13_smooth_d_prev = 0;
        numbertilde_13_smooth_prev = 0;
        numbertilde_13_smooth_index = 0;
        numbertilde_13_smooth_increment = 0;
        numbertilde_13_setupDone = false;
        param_13_lastValue = 0;
        numbertilde_14_currentInterval = 0;
        numbertilde_14_currentIntervalInSamples = 0;
        numbertilde_14_lastValue = 0;
        numbertilde_14_outValue = 0;
        numbertilde_14_rampInSamples = 0;
        numbertilde_14_currentMode = 1;
        numbertilde_14_smooth_d_prev = 0;
        numbertilde_14_smooth_prev = 0;
        numbertilde_14_smooth_index = 0;
        numbertilde_14_smooth_increment = 0;
        numbertilde_14_setupDone = false;
        param_14_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_67_in1;
        number dspexpr_67_in2;
        number dspexpr_68_in1;
        number dspexpr_68_in2;
        number ip_07_value;
        number ip_07_impulse;
        number dspexpr_69_in1;
        number dspexpr_69_in2;
        number dspexpr_70_in1;
        number dspexpr_70_in2;
        number dspexpr_71_in1;
        number dspexpr_71_in2;
        number dspexpr_72_in1;
        number dspexpr_72_in2;
        number dspexpr_72_in3;
        number dspexpr_73_in1;
        number dspexpr_73_in2;
        number numbertilde_13_input_number;
        number numbertilde_13_ramp;
        number param_13_value;
        number numbertilde_14_input_number;
        number numbertilde_14_ramp;
        number dspexpr_74_in1;
        number dspexpr_74_in2;
        number dspexpr_74_in3;
        number dspexpr_75_in1;
        number dspexpr_75_in2;
        number dspexpr_76_in1;
        number dspexpr_76_in2;
        number dspexpr_77_in1;
        number dspexpr_77_in2;
        number param_14_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_07_lastIndex;
        number ip_07_lastValue;
        SampleIndex ip_07_resetCount;
        signal ip_07_sigbuf;
        bool ip_07_setupDone;
        SampleIndex numbertilde_13_currentInterval;
        SampleIndex numbertilde_13_currentIntervalInSamples;
        number numbertilde_13_lastValue;
        number numbertilde_13_outValue;
        number numbertilde_13_rampInSamples;
        Int numbertilde_13_currentMode;
        number numbertilde_13_smooth_d_prev;
        number numbertilde_13_smooth_prev;
        number numbertilde_13_smooth_index;
        number numbertilde_13_smooth_increment;
        bool numbertilde_13_setupDone;
        number param_13_lastValue;
        SampleIndex numbertilde_14_currentInterval;
        SampleIndex numbertilde_14_currentIntervalInSamples;
        number numbertilde_14_lastValue;
        number numbertilde_14_outValue;
        number numbertilde_14_rampInSamples;
        Int numbertilde_14_currentMode;
        number numbertilde_14_smooth_d_prev;
        number numbertilde_14_smooth_prev;
        number numbertilde_14_smooth_index;
        number numbertilde_14_smooth_increment;
        bool numbertilde_14_setupDone;
        number param_14_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_500 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_500()
    {
    }
    
    ~RNBOSubpatcher_500()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 2098551528, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        SampleValue ** inputs,
        Index numInputs,
        SampleValue ** outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        this->ip_08_perform(this->signals[0], n);
        this->dspexpr_82_perform(in2, this->dspexpr_82_in2, this->signals[1], n);
        this->dspexpr_84_perform(this->signals[1], this->dspexpr_84_in2, this->signals[2], n);
    
        this->dspexpr_83_perform(
            this->signals[2],
            this->dspexpr_83_in2,
            this->dspexpr_83_in3,
            this->signals[1],
            n
        );
    
        this->numbertilde_15_perform(this->signals[1], this->dummyBuffer, n);
        this->dspexpr_80_perform(in1, this->signals[1], this->signals[3], n);
        this->dspexpr_79_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->dspexpr_87_perform(in3, this->dspexpr_87_in2, this->signals[3], n);
        this->dspexpr_88_perform(this->signals[3], this->dspexpr_88_in2, this->signals[0], n);
        this->dspexpr_86_perform(this->signals[2], this->signals[0], this->signals[3], n);
    
        this->dspexpr_85_perform(
            this->signals[3],
            this->dspexpr_85_in2,
            this->dspexpr_85_in3,
            this->signals[0],
            n
        );
    
        this->numbertilde_16_perform(this->signals[0], this->dummyBuffer, n);
        this->dspexpr_81_perform(in1, this->signals[0], this->signals[3], n);
        this->dspexpr_78_perform(this->signals[1], this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->ip_08_sigbuf = resizeSignal(this->ip_08_sigbuf, this->maxvs, maxBlockSize);
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->ip_08_dspsetup(forceDSPSetup);
        this->numbertilde_15_dspsetup(forceDSPSetup);
        this->numbertilde_16_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            this->setProbingIndex(-1);
            break;
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_15_getPresetValue(getSubState(preset, "bin"));
        this->param_16_getPresetValue(getSubState(preset, "width"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_15_value_set(v);
            break;
        case 1:
            this->param_16_value_set(v);
            break;
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            return this->param_15_value;
        case 1:
            return this->param_16_value;
        default:
            return 0;
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 2;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "bin";
        case 1:
            return "width";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "f8/bin";
        case 1:
            return "f8/width";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Bin-8";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 511;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Width-8";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 511 ? 511 : value));
                ParameterValue normalizedValue = (value - 0) / (511 - 0);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 0:
        case 1:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (511 - 0);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_15_value_constrain(value);
        case 1:
            return this->param_16_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 2098551528:
            this->numbertilde_15_value_set(value);
            break;
        case -1494586265:
            this->numbertilde_16_value_set(value);
            break;
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f8/number~_obj-83") == objectId)
                this->numbertilde_15_sig_number_set(payload);
    
            if (TAG("f8/number~_obj-84") == objectId)
                this->numbertilde_16_sig_number_set(payload);
    
            break;
        case TAG("mode"):
            if (TAG("f8/number~_obj-83") == objectId)
                this->numbertilde_15_mode_set(payload);
    
            if (TAG("f8/number~_obj-84") == objectId)
                this->numbertilde_16_mode_set(payload);
    
            break;
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            if (TAG("f8/number~_obj-83") == objectId)
                this->numbertilde_15_sig_list_set(payload);
    
            if (TAG("f8/number~_obj-84") == objectId)
                this->numbertilde_16_sig_list_set(payload);
    
            break;
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            return "monitor";
        case TAG("f8/number~_obj-83"):
            return "f8/number~_obj-83";
        case TAG("assign"):
            return "assign";
        case TAG("setup"):
            return "setup";
        case TAG("f8/number~_obj-84"):
            return "f8/number~_obj-84";
        case TAG("sig"):
            return "sig";
        case TAG("mode"):
            return "mode";
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            return nullptr;
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
    void param_15_value_set(number v) {
        v = this->param_15_value_constrain(v);
        this->param_15_value = v;
        this->sendParameter(0, false);
    
        if (this->param_15_value != this->param_15_lastValue) {
            this->getEngine()->presetTouched();
            this->param_15_lastValue = this->param_15_value;
        }
    
        this->dspexpr_84_in2_set(v);
    }
    
    void param_16_value_set(number v) {
        v = this->param_16_value_constrain(v);
        this->param_16_value = v;
        this->sendParameter(1, false);
    
        if (this->param_16_value != this->param_16_lastValue) {
            this->getEngine()->presetTouched();
            this->param_16_lastValue = this->param_16_value;
        }
    
        this->dspexpr_88_in2_set(v);
    }
    
    void numbertilde_15_sig_number_set(number v) {
        this->numbertilde_15_outValue = v;
    }
    
    void numbertilde_15_sig_list_set(const list& v) {
        this->numbertilde_15_outValue = v[0];
    }
    
    void numbertilde_15_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_15_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_15_currentMode = 1;
        }
    }
    
    void numbertilde_16_sig_number_set(number v) {
        this->numbertilde_16_outValue = v;
    }
    
    void numbertilde_16_sig_list_set(const list& v) {
        this->numbertilde_16_outValue = v[0];
    }
    
    void numbertilde_16_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_16_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_16_currentMode = 1;
        }
    }
    
    void numbertilde_15_value_set(number ) {}
    
    void numbertilde_16_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 3;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->ip_08_init();
        this->numbertilde_15_init();
        this->numbertilde_16_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
    
        {
            this->scheduleParamInit(0, 0);
        }
    
        {
            this->scheduleParamInit(1, 0);
        }
    }
    
    void allocateDataRefs() {}
    
    static number param_15_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_84_in2_set(number v) {
        this->dspexpr_84_in2 = v;
    }
    
    static number param_16_value_constrain(number v) {
        v = (v > 511 ? 511 : (v < 0 ? 0 : v));
        return v;
    }
    
    void dspexpr_88_in2_set(number v) {
        this->dspexpr_88_in2 = v;
    }
    
    void ip_08_perform(Sample * out, Index n) {
        auto __ip_08_sigbuf = this->ip_08_sigbuf;
        auto __ip_08_lastValue = this->ip_08_lastValue;
        auto __ip_08_lastIndex = this->ip_08_lastIndex;
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = ((SampleIndex)(i) >= __ip_08_lastIndex ? __ip_08_lastValue : __ip_08_sigbuf[(Index)i]);
        }
    
        __ip_08_lastIndex = 0;
        this->ip_08_lastIndex = __ip_08_lastIndex;
    }
    
    void dspexpr_82_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_84_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_83_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_15_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_15_currentIntervalInSamples = this->numbertilde_15_currentIntervalInSamples;
        auto __numbertilde_15_lastValue = this->numbertilde_15_lastValue;
        auto __numbertilde_15_currentInterval = this->numbertilde_15_currentInterval;
        auto __numbertilde_15_rampInSamples = this->numbertilde_15_rampInSamples;
        auto __numbertilde_15_outValue = this->numbertilde_15_outValue;
        auto __numbertilde_15_currentMode = this->numbertilde_15_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_15_currentMode == 0) {
                output[(Index)i] = this->numbertilde_15_smooth_next(
                    __numbertilde_15_outValue,
                    __numbertilde_15_rampInSamples,
                    __numbertilde_15_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_15_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_15_lastValue && __numbertilde_15_currentInterval <= 0) {
            __numbertilde_15_currentInterval = __numbertilde_15_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                2098551528,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_15_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f8/number~_obj-83"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_15_currentInterval = __numbertilde_15_currentInterval;
        this->numbertilde_15_lastValue = __numbertilde_15_lastValue;
    }
    
    void dspexpr_80_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] >= in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_79_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_87_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 511;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_88_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_86_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_85_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 511 ? 511 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void numbertilde_16_perform(const Sample * input_signal, Sample * output, Index n) {
        auto __numbertilde_16_currentIntervalInSamples = this->numbertilde_16_currentIntervalInSamples;
        auto __numbertilde_16_lastValue = this->numbertilde_16_lastValue;
        auto __numbertilde_16_currentInterval = this->numbertilde_16_currentInterval;
        auto __numbertilde_16_rampInSamples = this->numbertilde_16_rampInSamples;
        auto __numbertilde_16_outValue = this->numbertilde_16_outValue;
        auto __numbertilde_16_currentMode = this->numbertilde_16_currentMode;
        number monitorvalue = 0;
    
        for (Index i = 0; i < n; i++) {
            monitorvalue += input_signal[(Index)i];
    
            if (__numbertilde_16_currentMode == 0) {
                output[(Index)i] = this->numbertilde_16_smooth_next(
                    __numbertilde_16_outValue,
                    __numbertilde_16_rampInSamples,
                    __numbertilde_16_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_16_currentInterval -= n;
        monitorvalue /= n;
    
        if (monitorvalue != __numbertilde_16_lastValue && __numbertilde_16_currentInterval <= 0) {
            __numbertilde_16_currentInterval = __numbertilde_16_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_16_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("f8/number~_obj-84"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_16_currentInterval = __numbertilde_16_currentInterval;
        this->numbertilde_16_lastValue = __numbertilde_16_lastValue;
    }
    
    void dspexpr_81_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] < in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_78_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void ip_08_init() {
        this->ip_08_lastValue = this->ip_08_value;
    }
    
    void ip_08_dspsetup(bool force) {
        if ((bool)(this->ip_08_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->ip_08_lastIndex = 0;
        this->ip_08_setupDone = true;
    }
    
    number numbertilde_15_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_15_smooth_d_prev);
        this->numbertilde_15_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_15_smooth_d_dspsetup() {
        this->numbertilde_15_smooth_d_reset();
    }
    
    void numbertilde_15_smooth_d_reset() {
        this->numbertilde_15_smooth_d_prev = 0;
    }
    
    number numbertilde_15_smooth_next(number x, number up, number down) {
        if (this->numbertilde_15_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_15_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_15_smooth_index = _up;
                this->numbertilde_15_smooth_increment = (x - this->numbertilde_15_smooth_prev) / _up;
            } else if (x < this->numbertilde_15_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_15_smooth_index = _down;
                this->numbertilde_15_smooth_increment = (x - this->numbertilde_15_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_15_smooth_index > 0) {
            this->numbertilde_15_smooth_prev += this->numbertilde_15_smooth_increment;
            this->numbertilde_15_smooth_index -= 1;
        } else {
            this->numbertilde_15_smooth_prev = x;
        }
    
        return this->numbertilde_15_smooth_prev;
    }
    
    void numbertilde_15_smooth_reset() {
        this->numbertilde_15_smooth_prev = 0;
        this->numbertilde_15_smooth_index = 0;
        this->numbertilde_15_smooth_increment = 0;
        this->numbertilde_15_smooth_d_reset();
    }
    
    void numbertilde_15_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f8/number~_obj-83"), 1, this->_currentTime);
    }
    
    void numbertilde_15_dspsetup(bool force) {
        if ((bool)(this->numbertilde_15_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_15_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_15_currentInterval = this->numbertilde_15_currentIntervalInSamples;
        this->numbertilde_15_rampInSamples = this->mstosamps(this->numbertilde_15_ramp);
        this->numbertilde_15_setupDone = true;
        this->numbertilde_15_smooth_d_dspsetup();
    }
    
    void param_15_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_15_value;
    }
    
    void param_15_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_15_value_set(preset["value"]);
    }
    
    number numbertilde_16_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_16_smooth_d_prev);
        this->numbertilde_16_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_16_smooth_d_dspsetup() {
        this->numbertilde_16_smooth_d_reset();
    }
    
    void numbertilde_16_smooth_d_reset() {
        this->numbertilde_16_smooth_d_prev = 0;
    }
    
    number numbertilde_16_smooth_next(number x, number up, number down) {
        if (this->numbertilde_16_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_16_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_16_smooth_index = _up;
                this->numbertilde_16_smooth_increment = (x - this->numbertilde_16_smooth_prev) / _up;
            } else if (x < this->numbertilde_16_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_16_smooth_index = _down;
                this->numbertilde_16_smooth_increment = (x - this->numbertilde_16_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_16_smooth_index > 0) {
            this->numbertilde_16_smooth_prev += this->numbertilde_16_smooth_increment;
            this->numbertilde_16_smooth_index -= 1;
        } else {
            this->numbertilde_16_smooth_prev = x;
        }
    
        return this->numbertilde_16_smooth_prev;
    }
    
    void numbertilde_16_smooth_reset() {
        this->numbertilde_16_smooth_prev = 0;
        this->numbertilde_16_smooth_index = 0;
        this->numbertilde_16_smooth_increment = 0;
        this->numbertilde_16_smooth_d_reset();
    }
    
    void numbertilde_16_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("f8/number~_obj-84"), 1, this->_currentTime);
    }
    
    void numbertilde_16_dspsetup(bool force) {
        if ((bool)(this->numbertilde_16_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_16_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_16_currentInterval = this->numbertilde_16_currentIntervalInSamples;
        this->numbertilde_16_rampInSamples = this->mstosamps(this->numbertilde_16_ramp);
        this->numbertilde_16_setupDone = true;
        this->numbertilde_16_smooth_d_dspsetup();
    }
    
    void param_16_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_16_value;
    }
    
    void param_16_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_16_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        dspexpr_78_in1 = 0;
        dspexpr_78_in2 = 0;
        dspexpr_79_in1 = 0;
        dspexpr_79_in2 = 0;
        ip_08_value = 1;
        ip_08_impulse = 0;
        dspexpr_80_in1 = 0;
        dspexpr_80_in2 = 0;
        dspexpr_81_in1 = 0;
        dspexpr_81_in2 = 0;
        dspexpr_82_in1 = 0;
        dspexpr_82_in2 = 511;
        dspexpr_83_in1 = 0;
        dspexpr_83_in2 = 0;
        dspexpr_83_in3 = 511;
        dspexpr_84_in1 = 0;
        dspexpr_84_in2 = 0;
        numbertilde_15_input_number = 0;
        numbertilde_15_ramp = 0;
        param_15_value = 0;
        numbertilde_16_input_number = 0;
        numbertilde_16_ramp = 0;
        dspexpr_85_in1 = 0;
        dspexpr_85_in2 = 0;
        dspexpr_85_in3 = 511;
        dspexpr_86_in1 = 0;
        dspexpr_86_in2 = 0;
        dspexpr_87_in1 = 0;
        dspexpr_87_in2 = 511;
        dspexpr_88_in1 = 0;
        dspexpr_88_in2 = 0;
        param_16_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        ip_08_lastIndex = 0;
        ip_08_lastValue = 0;
        ip_08_resetCount = 0;
        ip_08_sigbuf = nullptr;
        ip_08_setupDone = false;
        numbertilde_15_currentInterval = 0;
        numbertilde_15_currentIntervalInSamples = 0;
        numbertilde_15_lastValue = 0;
        numbertilde_15_outValue = 0;
        numbertilde_15_rampInSamples = 0;
        numbertilde_15_currentMode = 1;
        numbertilde_15_smooth_d_prev = 0;
        numbertilde_15_smooth_prev = 0;
        numbertilde_15_smooth_index = 0;
        numbertilde_15_smooth_increment = 0;
        numbertilde_15_setupDone = false;
        param_15_lastValue = 0;
        numbertilde_16_currentInterval = 0;
        numbertilde_16_currentIntervalInSamples = 0;
        numbertilde_16_lastValue = 0;
        numbertilde_16_outValue = 0;
        numbertilde_16_rampInSamples = 0;
        numbertilde_16_currentMode = 1;
        numbertilde_16_smooth_d_prev = 0;
        numbertilde_16_smooth_prev = 0;
        numbertilde_16_smooth_index = 0;
        numbertilde_16_smooth_increment = 0;
        numbertilde_16_setupDone = false;
        param_16_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_78_in1;
        number dspexpr_78_in2;
        number dspexpr_79_in1;
        number dspexpr_79_in2;
        number ip_08_value;
        number ip_08_impulse;
        number dspexpr_80_in1;
        number dspexpr_80_in2;
        number dspexpr_81_in1;
        number dspexpr_81_in2;
        number dspexpr_82_in1;
        number dspexpr_82_in2;
        number dspexpr_83_in1;
        number dspexpr_83_in2;
        number dspexpr_83_in3;
        number dspexpr_84_in1;
        number dspexpr_84_in2;
        number numbertilde_15_input_number;
        number numbertilde_15_ramp;
        number param_15_value;
        number numbertilde_16_input_number;
        number numbertilde_16_ramp;
        number dspexpr_85_in1;
        number dspexpr_85_in2;
        number dspexpr_85_in3;
        number dspexpr_86_in1;
        number dspexpr_86_in2;
        number dspexpr_87_in1;
        number dspexpr_87_in2;
        number dspexpr_88_in1;
        number dspexpr_88_in2;
        number param_16_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        SampleIndex ip_08_lastIndex;
        number ip_08_lastValue;
        SampleIndex ip_08_resetCount;
        signal ip_08_sigbuf;
        bool ip_08_setupDone;
        SampleIndex numbertilde_15_currentInterval;
        SampleIndex numbertilde_15_currentIntervalInSamples;
        number numbertilde_15_lastValue;
        number numbertilde_15_outValue;
        number numbertilde_15_rampInSamples;
        Int numbertilde_15_currentMode;
        number numbertilde_15_smooth_d_prev;
        number numbertilde_15_smooth_prev;
        number numbertilde_15_smooth_index;
        number numbertilde_15_smooth_increment;
        bool numbertilde_15_setupDone;
        number param_15_lastValue;
        SampleIndex numbertilde_16_currentInterval;
        SampleIndex numbertilde_16_currentIntervalInSamples;
        number numbertilde_16_lastValue;
        number numbertilde_16_outValue;
        number numbertilde_16_rampInSamples;
        Int numbertilde_16_currentMode;
        number numbertilde_16_smooth_d_prev;
        number numbertilde_16_smooth_prev;
        number numbertilde_16_smooth_index;
        number numbertilde_16_smooth_increment;
        bool numbertilde_16_setupDone;
        number param_16_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

rnbomatic()
{
}

~rnbomatic()
{
    delete this->p_01;
    delete this->p_02;
    delete this->p_03;
    delete this->p_04;
    delete this->p_05;
    delete this->p_06;
    delete this->p_07;
    delete this->p_08;
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

inline number safesqrt(number num) {
    return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
}

number hann(number x) {
    x = (x > 1 ? 1 : (x < 0 ? 0 : x));
    return 0.5 * (1 - rnbo_cos(6.28318530717958647692 * x));
}

number hamming(number x) {
    x = (x > 1 ? 1 : (x < 0 ? 0 : x));
    return 0.54 - 0.46 * rnbo_cos(6.28318530717958647692 * x);
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo();
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

Index getNumMidiInputPorts() const {
    return 0;
}

void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    SampleValue ** inputs,
    Index numInputs,
    SampleValue ** outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    SampleValue * out3 = (numOutputs >= 3 && outputs[2] ? outputs[2] : this->dummyBuffer);
    SampleValue * out4 = (numOutputs >= 4 && outputs[3] ? outputs[3] : this->dummyBuffer);
    SampleValue * out5 = (numOutputs >= 5 && outputs[4] ? outputs[4] : this->dummyBuffer);
    SampleValue * out6 = (numOutputs >= 6 && outputs[5] ? outputs[5] : this->dummyBuffer);
    SampleValue * out7 = (numOutputs >= 7 && outputs[6] ? outputs[6] : this->dummyBuffer);
    SampleValue * out8 = (numOutputs >= 8 && outputs[7] ? outputs[7] : this->dummyBuffer);
    SampleValue * out9 = (numOutputs >= 9 && outputs[8] ? outputs[8] : this->dummyBuffer);
    SampleValue * out10 = (numOutputs >= 10 && outputs[9] ? outputs[9] : this->dummyBuffer);
    SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
    SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
    SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
    SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
    SampleValue * in6 = (numInputs >= 6 && inputs[5] ? inputs[5] : this->zeroBuffer);
    SampleValue * in7 = (numInputs >= 7 && inputs[6] ? inputs[6] : this->zeroBuffer);
    SampleValue * in8 = (numInputs >= 8 && inputs[7] ? inputs[7] : this->zeroBuffer);
    SampleValue * in9 = (numInputs >= 9 && inputs[8] ? inputs[8] : this->zeroBuffer);
    SampleValue * in10 = (numInputs >= 10 && inputs[9] ? inputs[9] : this->zeroBuffer);
    SampleValue * in11 = (numInputs >= 11 && inputs[10] ? inputs[10] : this->zeroBuffer);
    SampleValue * in12 = (numInputs >= 12 && inputs[11] ? inputs[11] : this->zeroBuffer);
    SampleValue * in13 = (numInputs >= 13 && inputs[12] ? inputs[12] : this->zeroBuffer);
    SampleValue * in14 = (numInputs >= 14 && inputs[13] ? inputs[13] : this->zeroBuffer);
    SampleValue * in15 = (numInputs >= 15 && inputs[14] ? inputs[14] : this->zeroBuffer);
    SampleValue * in16 = (numInputs >= 16 && inputs[15] ? inputs[15] : this->zeroBuffer);
    SampleValue * in17 = (numInputs >= 17 && inputs[16] ? inputs[16] : this->zeroBuffer);

    this->fftstream_tilde_01_perform(
        in1,
        this->fftstream_tilde_01_imagIn,
        this->signals[0],
        this->signals[1],
        this->signals[2],
        n
    );

    this->cartopol_tilde_01_perform(this->signals[0], this->signals[1], this->signals[3], this->signals[4], n);
    this->dspexpr_90_perform(this->signals[2], this->dspexpr_90_in2, this->signals[1], n);
    this->dspexpr_89_perform(this->signals[3], this->signals[1], this->signals[0], n);
    this->ip_09_perform(this->signals[1], n);
    this->p_01_perform(this->signals[2], in2, in3, this->signals[3], n);
    this->dspexpr_91_perform(this->signals[3], this->signals[0], this->signals[5], n);
    this->poltocar_tilde_01_perform(this->signals[5], this->signals[4], this->signals[6], this->signals[7], n);

    this->ifftstream_tilde_01_perform(
        this->signals[6],
        this->signals[7],
        this->signals[5],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signalforwarder_01_perform(this->signals[5], out1, n);
    this->dspexpr_92_perform(this->signals[1], this->signals[3], this->signals[7], n);
    this->p_02_perform(this->signals[2], in4, in5, this->signals[3], n);
    this->dspexpr_94_perform(this->signals[7], this->signals[3], this->signals[1], n);
    this->dspexpr_93_perform(this->signals[3], this->signals[0], this->signals[7], n);
    this->poltocar_tilde_02_perform(this->signals[7], this->signals[4], this->signals[3], this->signals[6], n);

    this->ifftstream_tilde_02_perform(
        this->signals[3],
        this->signals[6],
        this->signals[7],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signalforwarder_02_perform(this->signals[7], out2, n);
    this->p_03_perform(this->signals[2], in6, in7, this->signals[6], n);
    this->dspexpr_96_perform(this->signals[1], this->signals[6], this->signals[3], n);
    this->dspexpr_95_perform(this->signals[6], this->signals[0], this->signals[1], n);
    this->poltocar_tilde_03_perform(this->signals[1], this->signals[4], this->signals[6], this->signals[8], n);

    this->ifftstream_tilde_03_perform(
        this->signals[6],
        this->signals[8],
        this->signals[1],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signalforwarder_03_perform(this->signals[1], out3, n);
    this->p_04_perform(this->signals[2], in8, in9, this->signals[8], n);
    this->dspexpr_98_perform(this->signals[3], this->signals[8], this->signals[6], n);
    this->dspexpr_97_perform(this->signals[8], this->signals[0], this->signals[3], n);
    this->poltocar_tilde_04_perform(this->signals[3], this->signals[4], this->signals[8], this->signals[9], n);

    this->ifftstream_tilde_04_perform(
        this->signals[8],
        this->signals[9],
        this->signals[3],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signalforwarder_04_perform(this->signals[3], out4, n);
    this->p_05_perform(this->signals[2], in10, in11, this->signals[9], n);
    this->dspexpr_99_perform(this->signals[9], this->signals[0], this->signals[8], n);

    this->poltocar_tilde_05_perform(
        this->signals[8],
        this->signals[4],
        this->signals[10],
        this->signals[11],
        n
    );

    this->ifftstream_tilde_05_perform(
        this->signals[10],
        this->signals[11],
        this->signals[8],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signalforwarder_05_perform(this->signals[8], out5, n);
    this->dspexpr_100_perform(this->signals[6], this->signals[9], this->signals[11], n);
    this->p_06_perform(this->signals[2], in12, in13, this->signals[9], n);
    this->dspexpr_101_perform(this->signals[9], this->signals[0], this->signals[6], n);

    this->poltocar_tilde_06_perform(
        this->signals[6],
        this->signals[4],
        this->signals[10],
        this->signals[12],
        n
    );

    this->ifftstream_tilde_06_perform(
        this->signals[10],
        this->signals[12],
        this->signals[6],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signalforwarder_06_perform(this->signals[6], out6, n);
    this->dspexpr_102_perform(this->signals[11], this->signals[9], this->signals[12], n);
    this->p_07_perform(this->signals[2], in14, in15, this->signals[9], n);
    this->dspexpr_103_perform(this->signals[9], this->signals[0], this->signals[11], n);

    this->poltocar_tilde_07_perform(
        this->signals[11],
        this->signals[4],
        this->signals[10],
        this->signals[13],
        n
    );

    this->ifftstream_tilde_07_perform(
        this->signals[10],
        this->signals[13],
        this->signals[11],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signalforwarder_07_perform(this->signals[11], out7, n);
    this->dspexpr_104_perform(this->signals[12], this->signals[9], this->signals[13], n);
    this->p_08_perform(this->signals[2], in16, in17, this->signals[9], n);
    this->dspexpr_105_perform(this->signals[9], this->signals[0], this->signals[2], n);

    this->poltocar_tilde_08_perform(
        this->signals[2],
        this->signals[4],
        this->signals[12],
        this->signals[10],
        n
    );

    this->ifftstream_tilde_08_perform(
        this->signals[12],
        this->signals[10],
        this->signals[2],
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->signaladder_01_perform(
        this->signals[8],
        this->signals[6],
        this->signals[11],
        this->signals[2],
        this->signals[1],
        this->signals[3],
        this->signals[7],
        this->signals[5],
        out9,
        n
    );

    this->signalforwarder_08_perform(this->signals[2], out8, n);
    this->dspexpr_106_perform(this->signals[13], this->signals[9], this->signals[2], n);

    this->dspexpr_107_perform(
        this->signals[2],
        this->dspexpr_107_in2,
        this->dspexpr_107_in3,
        this->signals[9],
        n
    );

    this->dspexpr_108_perform(this->signals[9], this->signals[0], this->signals[2], n);
    this->poltocar_tilde_09_perform(this->signals[2], this->signals[4], this->signals[0], this->signals[9], n);

    this->ifftstream_tilde_09_perform(
        this->signals[0],
        this->signals[9],
        out10,
        this->dummyBuffer,
        this->dummyBuffer,
        n
    );

    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 14; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

        this->ip_09_sigbuf = resizeSignal(this->ip_09_sigbuf, this->maxvs, maxBlockSize);
        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->fftstream_tilde_01_dspsetup(forceDSPSetup);
    this->ip_09_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_01_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_02_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_03_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_04_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_05_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_06_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_07_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_08_dspsetup(forceDSPSetup);
    this->ifftstream_tilde_09_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_02->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_03->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_04->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_05->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_06->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_07->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_08->prepareToProcess(sampleRate, maxBlockSize, force);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        this->setProbingIndex(-1);
        break;
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    case 0:
        return addressOf(this->RNBODefaultFftWindow);
        break;
    default:
        return nullptr;
    }
}

DataRefIndex getNumDataRefs() const {
    return 1;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void zeroDataRef(DataRef& ref) {
    ref->setZero();
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->updateTime(time);

    if (index == 0) {
        this->fftstream_tilde_01_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_01_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_02_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_03_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_04_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_05_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_06_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_07_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_08_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
        this->ifftstream_tilde_09_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    }

    this->p_01->processDataViewUpdate(index, time);
    this->p_02->processDataViewUpdate(index, time);
    this->p_03->processDataViewUpdate(index, time);
    this->p_04->processDataViewUpdate(index, time);
    this->p_05->processDataViewUpdate(index, time);
    this->p_06->processDataViewUpdate(index, time);
    this->p_07->processDataViewUpdate(index, time);
    this->p_08->processDataViewUpdate(index, time);
}

void initialize() {
    this->RNBODefaultFftWindow = initDataRef("RNBODefaultFftWindow", false, nullptr);
    this->assign_defaults();
    this->setState();
    this->RNBODefaultFftWindow->setIndex(0);
    this->fftstream_tilde_01_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_01_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_02_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_03_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_04_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_05_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_06_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_07_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_08_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->ifftstream_tilde_09_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {
    this->p_01 = new RNBOSubpatcher_493();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_494();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));
    this->p_03 = new RNBOSubpatcher_495();
    this->p_03->setEngineAndPatcher(this->getEngine(), this);
    this->p_03->initialize();
    this->p_03->setParameterOffset(this->getParameterOffset(this->p_03));
    this->p_04 = new RNBOSubpatcher_496();
    this->p_04->setEngineAndPatcher(this->getEngine(), this);
    this->p_04->initialize();
    this->p_04->setParameterOffset(this->getParameterOffset(this->p_04));
    this->p_05 = new RNBOSubpatcher_497();
    this->p_05->setEngineAndPatcher(this->getEngine(), this);
    this->p_05->initialize();
    this->p_05->setParameterOffset(this->getParameterOffset(this->p_05));
    this->p_06 = new RNBOSubpatcher_498();
    this->p_06->setEngineAndPatcher(this->getEngine(), this);
    this->p_06->initialize();
    this->p_06->setParameterOffset(this->getParameterOffset(this->p_06));
    this->p_07 = new RNBOSubpatcher_499();
    this->p_07->setEngineAndPatcher(this->getEngine(), this);
    this->p_07->initialize();
    this->p_07->setParameterOffset(this->getParameterOffset(this->p_07));
    this->p_08 = new RNBOSubpatcher_500();
    this->p_08->setEngineAndPatcher(this->getEngine(), this);
    this->p_08->initialize();
    this->p_08->setParameterOffset(this->getParameterOffset(this->p_08));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "f1"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "f2"));
    this->p_03->getPreset(getSubState(getSubState(preset, "__sps"), "f3"));
    this->p_04->getPreset(getSubState(getSubState(preset, "__sps"), "f4"));
    this->p_05->getPreset(getSubState(getSubState(preset, "__sps"), "f5"));
    this->p_06->getPreset(getSubState(getSubState(preset, "__sps"), "f6"));
    this->p_07->getPreset(getSubState(getSubState(preset, "__sps"), "f7"));
    this->p_08->getPreset(getSubState(getSubState(preset, "__sps"), "f8"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->p_01->param_01_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f1"), "bin"));
    this->p_01->param_02_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f1"), "width"));
    this->p_02->param_03_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f2"), "bin"));
    this->p_02->param_04_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f2"), "width"));
    this->p_03->param_05_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f3"), "bin"));
    this->p_03->param_06_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f3"), "width"));
    this->p_04->param_07_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f4"), "bin"));
    this->p_04->param_08_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f4"), "width"));
    this->p_05->param_09_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f5"), "bin"));
    this->p_05->param_10_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f5"), "width"));
    this->p_06->param_11_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f6"), "bin"));
    this->p_06->param_12_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f6"), "width"));
    this->p_07->param_13_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f7"), "bin"));
    this->p_07->param_14_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f7"), "width"));
    this->p_08->param_15_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f8"), "bin"));
    this->p_08->param_16_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "f8"), "width"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(tempo, false)) {
        this->p_01->processTempoEvent(time, tempo);
        this->p_02->processTempoEvent(time, tempo);
        this->p_03->processTempoEvent(time, tempo);
        this->p_04->processTempoEvent(time, tempo);
        this->p_05->processTempoEvent(time, tempo);
        this->p_06->processTempoEvent(time, tempo);
        this->p_07->processTempoEvent(time, tempo);
        this->p_08->processTempoEvent(time, tempo);
    }
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(state, false)) {
        this->p_01->processTransportEvent(time, state);
        this->p_02->processTransportEvent(time, state);
        this->p_03->processTransportEvent(time, state);
        this->p_04->processTransportEvent(time, state);
        this->p_05->processTransportEvent(time, state);
        this->p_06->processTransportEvent(time, state);
        this->p_07->processTransportEvent(time, state);
        this->p_08->processTransportEvent(time, state);
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(beattime, false)) {
        this->p_01->processBeatTimeEvent(time, beattime);
        this->p_02->processBeatTimeEvent(time, beattime);
        this->p_03->processBeatTimeEvent(time, beattime);
        this->p_04->processBeatTimeEvent(time, beattime);
        this->p_05->processBeatTimeEvent(time, beattime);
        this->p_06->processBeatTimeEvent(time, beattime);
        this->p_07->processBeatTimeEvent(time, beattime);
        this->p_08->processBeatTimeEvent(time, beattime);
    }
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(numerator, denominator, false)) {
        this->p_01->processTimeSignatureEvent(time, numerator, denominator);
        this->p_02->processTimeSignatureEvent(time, numerator, denominator);
        this->p_03->processTimeSignatureEvent(time, numerator, denominator);
        this->p_04->processTimeSignatureEvent(time, numerator, denominator);
        this->p_05->processTimeSignatureEvent(time, numerator, denominator);
        this->p_06->processTimeSignatureEvent(time, numerator, denominator);
        this->p_07->processTimeSignatureEvent(time, numerator, denominator);
        this->p_08->processTimeSignatureEvent(time, numerator, denominator);
    }
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    RNBO_UNUSED(v);
    this->updateTime(time);

    switch (index) {
    default:
        index -= 0;

        if (index < this->p_01->getNumParameters())
            this->p_01->setParameterValue(index, v, time);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            this->p_02->setParameterValue(index, v, time);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            this->p_03->setParameterValue(index, v, time);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            this->p_04->setParameterValue(index, v, time);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            this->p_05->setParameterValue(index, v, time);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            this->p_06->setParameterValue(index, v, time);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            this->p_07->setParameterValue(index, v, time);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            this->p_08->setParameterValue(index, v, time);

        break;
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    default:
        index -= 0;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterValue(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterValue(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterValue(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterValue(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterValue(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->getParameterValue(index);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterValue(index);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->getParameterValue(index);

        return 0;
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 0 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    default:
        index -= 0;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterName(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterName(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterName(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterName(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterName(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->getParameterName(index);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterName(index);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->getParameterName(index);

        return "bogus";
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    default:
        index -= 0;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterId(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterId(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterId(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterId(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterId(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->getParameterId(index);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterId(index);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->getParameterId(index);

        return "bogus";
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        default:
            index -= 0;

            if (index < this->p_01->getNumParameters())
                this->p_01->getParameterInfo(index, info);

            index -= this->p_01->getNumParameters();

            if (index < this->p_02->getNumParameters())
                this->p_02->getParameterInfo(index, info);

            index -= this->p_02->getNumParameters();

            if (index < this->p_03->getNumParameters())
                this->p_03->getParameterInfo(index, info);

            index -= this->p_03->getNumParameters();

            if (index < this->p_04->getNumParameters())
                this->p_04->getParameterInfo(index, info);

            index -= this->p_04->getNumParameters();

            if (index < this->p_05->getNumParameters())
                this->p_05->getParameterInfo(index, info);

            index -= this->p_05->getNumParameters();

            if (index < this->p_06->getNumParameters())
                this->p_06->getParameterInfo(index, info);

            index -= this->p_06->getNumParameters();

            if (index < this->p_07->getNumParameters())
                this->p_07->getParameterInfo(index, info);

            index -= this->p_07->getNumParameters();

            if (index < this->p_08->getNumParameters())
                this->p_08->getParameterInfo(index, info);

            break;
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->p_01)
        return 0;

    if (subpatcher == this->p_02)
        return 0 + this->p_01->getNumParameters();

    if (subpatcher == this->p_03)
        return 0 + this->p_01->getNumParameters() + this->p_02->getNumParameters();

    if (subpatcher == this->p_04)
        return 0 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters();

    if (subpatcher == this->p_05)
        return 0 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters();

    if (subpatcher == this->p_06)
        return 0 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters();

    if (subpatcher == this->p_07)
        return 0 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters();

    if (subpatcher == this->p_08)
        return 0 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters();

    return 0;
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    default:
        index -= 0;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertToNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertToNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->convertToNormalizedParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertToNormalizedParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->convertToNormalizedParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->convertToNormalizedParameterValue(index, value);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->convertToNormalizedParameterValue(index, value);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->convertToNormalizedParameterValue(index, value);

        return value;
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    default:
        index -= 0;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertFromNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertFromNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->convertFromNormalizedParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertFromNormalizedParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->convertFromNormalizedParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->convertFromNormalizedParameterValue(index, value);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->convertFromNormalizedParameterValue(index, value);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->convertFromNormalizedParameterValue(index, value);

        return value;
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    default:
        index -= 0;

        if (index < this->p_01->getNumParameters())
            return this->p_01->constrainParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->constrainParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->constrainParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->constrainParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->constrainParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->constrainParameterValue(index, value);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->constrainParameterValue(index, value);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->constrainParameterValue(index, value);

        return value;
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterChange(
            this->paramInitIndices[i],
            this->getParameterValue(this->paramInitIndices[i]),
            0
        );
    }
}

void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processNumMessage(tag, objectId, time, payload);
    this->p_02->processNumMessage(tag, objectId, time, payload);
    this->p_03->processNumMessage(tag, objectId, time, payload);
    this->p_04->processNumMessage(tag, objectId, time, payload);
    this->p_05->processNumMessage(tag, objectId, time, payload);
    this->p_06->processNumMessage(tag, objectId, time, payload);
    this->p_07->processNumMessage(tag, objectId, time, payload);
    this->p_08->processNumMessage(tag, objectId, time, payload);
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processListMessage(tag, objectId, time, payload);
    this->p_02->processListMessage(tag, objectId, time, payload);
    this->p_03->processListMessage(tag, objectId, time, payload);
    this->p_04->processListMessage(tag, objectId, time, payload);
    this->p_05->processListMessage(tag, objectId, time, payload);
    this->p_06->processListMessage(tag, objectId, time, payload);
    this->p_07->processListMessage(tag, objectId, time, payload);
    this->p_08->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processBangMessage(tag, objectId, time);
    this->p_02->processBangMessage(tag, objectId, time);
    this->p_03->processBangMessage(tag, objectId, time);
    this->p_04->processBangMessage(tag, objectId, time);
    this->p_05->processBangMessage(tag, objectId, time);
    this->p_06->processBangMessage(tag, objectId, time);
    this->p_07->processBangMessage(tag, objectId, time);
    this->p_08->processBangMessage(tag, objectId, time);
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {

    }

    auto subpatchResult_0 = this->p_01->resolveTag(tag);

    if (subpatchResult_0)
        return subpatchResult_0;

    auto subpatchResult_1 = this->p_02->resolveTag(tag);

    if (subpatchResult_1)
        return subpatchResult_1;

    auto subpatchResult_2 = this->p_03->resolveTag(tag);

    if (subpatchResult_2)
        return subpatchResult_2;

    auto subpatchResult_3 = this->p_04->resolveTag(tag);

    if (subpatchResult_3)
        return subpatchResult_3;

    auto subpatchResult_4 = this->p_05->resolveTag(tag);

    if (subpatchResult_4)
        return subpatchResult_4;

    auto subpatchResult_5 = this->p_06->resolveTag(tag);

    if (subpatchResult_5)
        return subpatchResult_5;

    auto subpatchResult_6 = this->p_07->resolveTag(tag);

    if (subpatchResult_6)
        return subpatchResult_6;

    auto subpatchResult_7 = this->p_08->resolveTag(tag);

    if (subpatchResult_7)
        return subpatchResult_7;

    return "";
}

MessageIndex getNumMessages() const {
    return 0;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {

    }

    return NullMessageInfo;
}

protected:

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 17;
}

Index getNumOutputChannels() const {
    return 10;
}

void allocateDataRefs() {
    this->p_01->allocateDataRefs();
    this->p_02->allocateDataRefs();
    this->p_03->allocateDataRefs();
    this->p_04->allocateDataRefs();
    this->p_05->allocateDataRefs();
    this->p_06->allocateDataRefs();
    this->p_07->allocateDataRefs();
    this->p_08->allocateDataRefs();
    this->fftstream_tilde_01_win_buf = this->fftstream_tilde_01_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_01_win_buf = this->ifftstream_tilde_01_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_02_win_buf = this->ifftstream_tilde_02_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_03_win_buf = this->ifftstream_tilde_03_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_04_win_buf = this->ifftstream_tilde_04_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_05_win_buf = this->ifftstream_tilde_05_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_06_win_buf = this->ifftstream_tilde_06_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_07_win_buf = this->ifftstream_tilde_07_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_08_win_buf = this->ifftstream_tilde_08_win_buf->allocateIfNeeded();
    this->ifftstream_tilde_09_win_buf = this->ifftstream_tilde_09_win_buf->allocateIfNeeded();

    if (this->RNBODefaultFftWindow->hasRequestedSize()) {
        if (this->RNBODefaultFftWindow->wantsFill())
            this->zeroDataRef(this->RNBODefaultFftWindow);

        this->getEngine()->sendDataRefUpdated(0);
    }
}

void initializeObjects() {
    this->ip_09_init();
    this->p_01->initializeObjects();
    this->p_02->initializeObjects();
    this->p_03->initializeObjects();
    this->p_04->initializeObjects();
    this->p_05->initializeObjects();
    this->p_06->initializeObjects();
    this->p_07->initializeObjects();
    this->p_08->initializeObjects();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());
    this->p_01->startup();
    this->p_02->startup();
    this->p_03->startup();
    this->p_04->startup();
    this->p_05->startup();
    this->p_06->startup();
    this->p_07->startup();
    this->p_08->startup();
    this->processParamInitEvents();
}

void fftstream_tilde_01_perform(
    const Sample * realIn,
    number imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    RNBO_UNUSED(imagIn);
    auto __fftstream_tilde_01_outWorkspace = this->fftstream_tilde_01_outWorkspace;
    auto __fftstream_tilde_01_inWorkspace = this->fftstream_tilde_01_inWorkspace;
    auto __fftstream_tilde_01_datapos = this->fftstream_tilde_01_datapos;
    number invFftSize = (number)1 / (number)1024;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        SampleIndex i = (SampleIndex)(__fftstream_tilde_01_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hann(i * invFftSize);

        if (__fftstream_tilde_01_datapos < 1024) {
            __fftstream_tilde_01_inWorkspace[(Index)(2 * i)] = windowFactor * realIn[(Index)i0];
            out[0] = __fftstream_tilde_01_outWorkspace[(Index)(2 * i)];
            __fftstream_tilde_01_inWorkspace[(Index)(2 * i + 1)] = windowFactor * 0;
            out[1] = __fftstream_tilde_01_outWorkspace[(Index)(2 * i + 1)];

            if (i == 1024 - 1) {
                this->fftstream_tilde_01_fft_next(__fftstream_tilde_01_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __fftstream_tilde_01_outWorkspace[(Index)j] = __fftstream_tilde_01_inWorkspace[(Index)j];
                }
            }

            __fftstream_tilde_01_datapos = (__fftstream_tilde_01_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->fftstream_tilde_01_datapos = __fftstream_tilde_01_datapos;
}

void cartopol_tilde_01_perform(const Sample * x, const Sample * y, Sample * out1, Sample * out2, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = this->safesqrt(x[(Index)i] * x[(Index)i] + y[(Index)i] * y[(Index)i]);
        out2[(Index)i] = rnbo_atan2(y[(Index)i], x[(Index)i]);
    }
}

void dspexpr_90_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    RNBO_UNUSED(in2);
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] < 512;//#map:_###_obj_###_:1
    }
}

void dspexpr_89_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void ip_09_perform(Sample * out, Index n) {
    auto __ip_09_sigbuf = this->ip_09_sigbuf;
    auto __ip_09_lastValue = this->ip_09_lastValue;
    auto __ip_09_lastIndex = this->ip_09_lastIndex;

    for (Index i = 0; i < n; i++) {
        out[(Index)i] = ((SampleIndex)(i) >= __ip_09_lastIndex ? __ip_09_lastValue : __ip_09_sigbuf[(Index)i]);
    }

    __ip_09_lastIndex = 0;
    this->ip_09_lastIndex = __ip_09_lastIndex;
}

void p_01_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f1
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_01->process(ins, 3, outs, 1, n);
}

void dspexpr_91_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_01_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_01_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_01_outWorkspace = this->ifftstream_tilde_01_outWorkspace;
    auto __ifftstream_tilde_01_inWorkspace = this->ifftstream_tilde_01_inWorkspace;
    auto __ifftstream_tilde_01_datapos = this->ifftstream_tilde_01_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_01_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_01_datapos < 1024) {
            __ifftstream_tilde_01_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_01_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_01_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_01_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_01_fft_next(__ifftstream_tilde_01_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_01_outWorkspace[(Index)j] = __ifftstream_tilde_01_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_01_datapos = (__ifftstream_tilde_01_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_01_datapos = __ifftstream_tilde_01_datapos;
}

void signalforwarder_01_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void dspexpr_92_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_02_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f2
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_02->process(ins, 3, outs, 1, n);
}

void dspexpr_94_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_93_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_02_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_02_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_02_outWorkspace = this->ifftstream_tilde_02_outWorkspace;
    auto __ifftstream_tilde_02_inWorkspace = this->ifftstream_tilde_02_inWorkspace;
    auto __ifftstream_tilde_02_datapos = this->ifftstream_tilde_02_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_02_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_02_datapos < 1024) {
            __ifftstream_tilde_02_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_02_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_02_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_02_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_02_fft_next(__ifftstream_tilde_02_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_02_outWorkspace[(Index)j] = __ifftstream_tilde_02_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_02_datapos = (__ifftstream_tilde_02_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_02_datapos = __ifftstream_tilde_02_datapos;
}

void signalforwarder_02_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void p_03_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f3
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_03->process(ins, 3, outs, 1, n);
}

void dspexpr_96_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_95_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_03_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_03_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_03_outWorkspace = this->ifftstream_tilde_03_outWorkspace;
    auto __ifftstream_tilde_03_inWorkspace = this->ifftstream_tilde_03_inWorkspace;
    auto __ifftstream_tilde_03_datapos = this->ifftstream_tilde_03_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_03_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_03_datapos < 1024) {
            __ifftstream_tilde_03_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_03_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_03_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_03_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_03_fft_next(__ifftstream_tilde_03_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_03_outWorkspace[(Index)j] = __ifftstream_tilde_03_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_03_datapos = (__ifftstream_tilde_03_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_03_datapos = __ifftstream_tilde_03_datapos;
}

void signalforwarder_03_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void p_04_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f4
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_04->process(ins, 3, outs, 1, n);
}

void dspexpr_98_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_97_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_04_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_04_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_04_outWorkspace = this->ifftstream_tilde_04_outWorkspace;
    auto __ifftstream_tilde_04_inWorkspace = this->ifftstream_tilde_04_inWorkspace;
    auto __ifftstream_tilde_04_datapos = this->ifftstream_tilde_04_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_04_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_04_datapos < 1024) {
            __ifftstream_tilde_04_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_04_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_04_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_04_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_04_fft_next(__ifftstream_tilde_04_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_04_outWorkspace[(Index)j] = __ifftstream_tilde_04_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_04_datapos = (__ifftstream_tilde_04_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_04_datapos = __ifftstream_tilde_04_datapos;
}

void signalforwarder_04_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void p_05_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f5
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_05->process(ins, 3, outs, 1, n);
}

void dspexpr_99_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_05_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_05_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_05_outWorkspace = this->ifftstream_tilde_05_outWorkspace;
    auto __ifftstream_tilde_05_inWorkspace = this->ifftstream_tilde_05_inWorkspace;
    auto __ifftstream_tilde_05_datapos = this->ifftstream_tilde_05_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_05_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_05_datapos < 1024) {
            __ifftstream_tilde_05_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_05_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_05_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_05_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_05_fft_next(__ifftstream_tilde_05_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_05_outWorkspace[(Index)j] = __ifftstream_tilde_05_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_05_datapos = (__ifftstream_tilde_05_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_05_datapos = __ifftstream_tilde_05_datapos;
}

void signalforwarder_05_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void dspexpr_100_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_06_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f6
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_06->process(ins, 3, outs, 1, n);
}

void dspexpr_101_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_06_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_06_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_06_outWorkspace = this->ifftstream_tilde_06_outWorkspace;
    auto __ifftstream_tilde_06_inWorkspace = this->ifftstream_tilde_06_inWorkspace;
    auto __ifftstream_tilde_06_datapos = this->ifftstream_tilde_06_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_06_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_06_datapos < 1024) {
            __ifftstream_tilde_06_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_06_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_06_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_06_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_06_fft_next(__ifftstream_tilde_06_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_06_outWorkspace[(Index)j] = __ifftstream_tilde_06_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_06_datapos = (__ifftstream_tilde_06_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_06_datapos = __ifftstream_tilde_06_datapos;
}

void signalforwarder_06_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void dspexpr_102_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_07_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f7
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_07->process(ins, 3, outs, 1, n);
}

void dspexpr_103_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_07_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_07_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_07_outWorkspace = this->ifftstream_tilde_07_outWorkspace;
    auto __ifftstream_tilde_07_inWorkspace = this->ifftstream_tilde_07_inWorkspace;
    auto __ifftstream_tilde_07_datapos = this->ifftstream_tilde_07_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_07_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_07_datapos < 1024) {
            __ifftstream_tilde_07_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_07_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_07_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_07_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_07_fft_next(__ifftstream_tilde_07_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_07_outWorkspace[(Index)j] = __ifftstream_tilde_07_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_07_datapos = (__ifftstream_tilde_07_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_07_datapos = __ifftstream_tilde_07_datapos;
}

void signalforwarder_07_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void dspexpr_104_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_08_perform(Sample * in1, Sample * in2, Sample * in3, Sample * out1, Index n) {
    // subpatcher: f8
    SampleArray<3> ins = {in1, in2, in3};

    SampleArray<1> outs = {out1};
    this->p_08->process(ins, 3, outs, 1, n);
}

void dspexpr_105_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_08_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_08_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_08_outWorkspace = this->ifftstream_tilde_08_outWorkspace;
    auto __ifftstream_tilde_08_inWorkspace = this->ifftstream_tilde_08_inWorkspace;
    auto __ifftstream_tilde_08_datapos = this->ifftstream_tilde_08_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_08_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_08_datapos < 1024) {
            __ifftstream_tilde_08_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_08_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_08_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_08_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_08_fft_next(__ifftstream_tilde_08_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_08_outWorkspace[(Index)j] = __ifftstream_tilde_08_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_08_datapos = (__ifftstream_tilde_08_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_08_datapos = __ifftstream_tilde_08_datapos;
}

void signaladder_01_perform(
    const Sample * in1,
    const Sample * in2,
    const Sample * in3,
    const Sample * in4,
    const Sample * in5,
    const Sample * in6,
    const Sample * in7,
    const Sample * in8,
    Sample * out,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out[(Index)i] = in1[(Index)i] + in2[(Index)i] + in3[(Index)i] + in4[(Index)i] + in5[(Index)i] + in6[(Index)i] + in7[(Index)i] + in8[(Index)i];
    }
}

void signalforwarder_08_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void dspexpr_106_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_107_perform(const Sample * in1, number in2, number in3, Sample * out1, Index n) {
    RNBO_UNUSED(in3);
    RNBO_UNUSED(in2);
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = (in1[(Index)i] > 1 ? 1 : (in1[(Index)i] < 0 ? 0 : in1[(Index)i]));//#map:_###_obj_###_:1
    }
}

void dspexpr_108_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void poltocar_tilde_09_perform(
    const Sample * radius,
    const Sample * angle,
    Sample * out1,
    Sample * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = radius[(Index)i] * rnbo_cos(angle[(Index)i]);
        out2[(Index)i] = radius[(Index)i] * rnbo_sin(angle[(Index)i]);
    }
}

void ifftstream_tilde_09_perform(
    const Sample * realIn,
    const Sample * imagIn,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Index n
) {
    auto __ifftstream_tilde_09_outWorkspace = this->ifftstream_tilde_09_outWorkspace;
    auto __ifftstream_tilde_09_inWorkspace = this->ifftstream_tilde_09_inWorkspace;
    auto __ifftstream_tilde_09_datapos = this->ifftstream_tilde_09_datapos;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        number invFftSize = (number)1 / (number)1024;
        SampleIndex i = (SampleIndex)(__ifftstream_tilde_09_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hamming(i * invFftSize);

        if (__ifftstream_tilde_09_datapos < 1024) {
            __ifftstream_tilde_09_inWorkspace[(Index)(2 * i + 1)] = realIn[(Index)i0];
            out[0] = windowFactor * __ifftstream_tilde_09_outWorkspace[(Index)(2 * i + 1)];
            __ifftstream_tilde_09_inWorkspace[(Index)(2 * i)] = imagIn[(Index)i0];
            out[1] = windowFactor * __ifftstream_tilde_09_outWorkspace[(Index)(2 * i)];

            if (i == 1024 - 1) {
                this->ifftstream_tilde_09_fft_next(__ifftstream_tilde_09_inWorkspace, 1024);

                for (SampleIndex j = 0; j < 1024 * 2; j++) {
                    __ifftstream_tilde_09_outWorkspace[(Index)j] = __ifftstream_tilde_09_inWorkspace[(Index)j] * invFftSize;
                }
            }

            __ifftstream_tilde_09_datapos = (__ifftstream_tilde_09_datapos + 1) % 1024;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->ifftstream_tilde_09_datapos = __ifftstream_tilde_09_datapos;
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

template <typename T> void fftstream_tilde_01_fft_next(T& buffer, int fftsize) {
    if (this->fftstream_tilde_01_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->fftstream_tilde_01_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->fftstream_tilde_01_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->fftstream_tilde_01_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->fftstream_tilde_01_fft_lastsize = fftsize;
    }

    if (this->fftstream_tilde_01_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->fftstream_tilde_01_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->fftstream_tilde_01_fft_costab[(Index)k] + imag_l * this->fftstream_tilde_01_fft_sintab[(Index)k];
                    number tpim = -real_l * this->fftstream_tilde_01_fft_sintab[(Index)k] + imag_l * this->fftstream_tilde_01_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void fftstream_tilde_01_fft_reset() {
    this->fftstream_tilde_01_fft_lastsize = 0;
    this->fftstream_tilde_01_fft_costab = {};
    this->fftstream_tilde_01_fft_sintab = {};
    this->fftstream_tilde_01_fft_levels = -1;
}

void fftstream_tilde_01_dspsetup(bool force) {
    if ((bool)(this->fftstream_tilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->fftstream_tilde_01_inWorkspace[(Index)i] = 0;
        this->fftstream_tilde_01_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->fftstream_tilde_01_datapos = framepos;
    } else {
        this->fftstream_tilde_01_datapos = 0;
    }

    this->fftstream_tilde_01_setupDone = true;
}

void ip_09_init() {
    this->ip_09_lastValue = this->ip_09_value;
}

void ip_09_dspsetup(bool force) {
    if ((bool)(this->ip_09_setupDone) && (bool)(!(bool)(force)))
        return;

    this->ip_09_lastIndex = 0;
    this->ip_09_setupDone = true;
}

template <typename T> void ifftstream_tilde_01_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_01_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_01_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_01_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_01_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_01_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_01_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_01_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_01_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_01_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_01_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_01_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_01_fft_reset() {
    this->ifftstream_tilde_01_fft_lastsize = 0;
    this->ifftstream_tilde_01_fft_costab = {};
    this->ifftstream_tilde_01_fft_sintab = {};
    this->ifftstream_tilde_01_fft_levels = -1;
}

void ifftstream_tilde_01_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_01_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_01_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_01_datapos = framepos;
    } else {
        this->ifftstream_tilde_01_datapos = 0;
    }

    this->ifftstream_tilde_01_setupDone = true;
}

template <typename T> void ifftstream_tilde_02_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_02_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_02_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_02_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_02_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_02_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_02_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_02_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_02_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_02_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_02_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_02_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_02_fft_reset() {
    this->ifftstream_tilde_02_fft_lastsize = 0;
    this->ifftstream_tilde_02_fft_costab = {};
    this->ifftstream_tilde_02_fft_sintab = {};
    this->ifftstream_tilde_02_fft_levels = -1;
}

void ifftstream_tilde_02_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_02_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_02_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_02_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_02_datapos = framepos;
    } else {
        this->ifftstream_tilde_02_datapos = 0;
    }

    this->ifftstream_tilde_02_setupDone = true;
}

template <typename T> void ifftstream_tilde_03_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_03_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_03_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_03_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_03_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_03_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_03_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_03_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_03_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_03_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_03_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_03_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_03_fft_reset() {
    this->ifftstream_tilde_03_fft_lastsize = 0;
    this->ifftstream_tilde_03_fft_costab = {};
    this->ifftstream_tilde_03_fft_sintab = {};
    this->ifftstream_tilde_03_fft_levels = -1;
}

void ifftstream_tilde_03_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_03_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_03_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_03_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_03_datapos = framepos;
    } else {
        this->ifftstream_tilde_03_datapos = 0;
    }

    this->ifftstream_tilde_03_setupDone = true;
}

template <typename T> void ifftstream_tilde_04_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_04_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_04_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_04_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_04_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_04_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_04_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_04_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_04_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_04_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_04_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_04_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_04_fft_reset() {
    this->ifftstream_tilde_04_fft_lastsize = 0;
    this->ifftstream_tilde_04_fft_costab = {};
    this->ifftstream_tilde_04_fft_sintab = {};
    this->ifftstream_tilde_04_fft_levels = -1;
}

void ifftstream_tilde_04_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_04_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_04_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_04_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_04_datapos = framepos;
    } else {
        this->ifftstream_tilde_04_datapos = 0;
    }

    this->ifftstream_tilde_04_setupDone = true;
}

template <typename T> void ifftstream_tilde_05_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_05_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_05_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_05_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_05_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_05_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_05_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_05_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_05_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_05_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_05_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_05_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_05_fft_reset() {
    this->ifftstream_tilde_05_fft_lastsize = 0;
    this->ifftstream_tilde_05_fft_costab = {};
    this->ifftstream_tilde_05_fft_sintab = {};
    this->ifftstream_tilde_05_fft_levels = -1;
}

void ifftstream_tilde_05_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_05_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_05_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_05_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_05_datapos = framepos;
    } else {
        this->ifftstream_tilde_05_datapos = 0;
    }

    this->ifftstream_tilde_05_setupDone = true;
}

template <typename T> void ifftstream_tilde_06_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_06_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_06_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_06_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_06_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_06_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_06_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_06_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_06_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_06_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_06_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_06_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_06_fft_reset() {
    this->ifftstream_tilde_06_fft_lastsize = 0;
    this->ifftstream_tilde_06_fft_costab = {};
    this->ifftstream_tilde_06_fft_sintab = {};
    this->ifftstream_tilde_06_fft_levels = -1;
}

void ifftstream_tilde_06_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_06_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_06_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_06_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_06_datapos = framepos;
    } else {
        this->ifftstream_tilde_06_datapos = 0;
    }

    this->ifftstream_tilde_06_setupDone = true;
}

template <typename T> void ifftstream_tilde_07_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_07_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_07_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_07_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_07_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_07_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_07_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_07_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_07_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_07_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_07_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_07_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_07_fft_reset() {
    this->ifftstream_tilde_07_fft_lastsize = 0;
    this->ifftstream_tilde_07_fft_costab = {};
    this->ifftstream_tilde_07_fft_sintab = {};
    this->ifftstream_tilde_07_fft_levels = -1;
}

void ifftstream_tilde_07_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_07_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_07_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_07_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_07_datapos = framepos;
    } else {
        this->ifftstream_tilde_07_datapos = 0;
    }

    this->ifftstream_tilde_07_setupDone = true;
}

template <typename T> void ifftstream_tilde_08_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_08_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_08_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_08_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_08_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_08_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_08_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_08_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_08_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_08_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_08_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_08_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_08_fft_reset() {
    this->ifftstream_tilde_08_fft_lastsize = 0;
    this->ifftstream_tilde_08_fft_costab = {};
    this->ifftstream_tilde_08_fft_sintab = {};
    this->ifftstream_tilde_08_fft_levels = -1;
}

void ifftstream_tilde_08_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_08_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_08_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_08_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_08_datapos = framepos;
    } else {
        this->ifftstream_tilde_08_datapos = 0;
    }

    this->ifftstream_tilde_08_setupDone = true;
}

template <typename T> void ifftstream_tilde_09_fft_next(T& buffer, int fftsize) {
    if (this->ifftstream_tilde_09_fft_lastsize != fftsize) {
        for (Index i = 0; i < 32; i++) {
            if ((BinOpInt)((UBinOpInt)1 << (UBinOpInt)i) == fftsize) {
                this->ifftstream_tilde_09_fft_levels = i;
            }
        }

        for (Index i = 0; i < fftsize; i++) {
            this->ifftstream_tilde_09_fft_costab->push(rnbo_cos(6.28318530717958647692 * i / fftsize));
            this->ifftstream_tilde_09_fft_sintab->push(rnbo_sin(6.28318530717958647692 * i / fftsize));
        }

        this->ifftstream_tilde_09_fft_lastsize = fftsize;
    }

    if (this->ifftstream_tilde_09_fft_levels != -1) {
        for (Index i = 0; i < fftsize; i++) {
            Index x = (Index)(i);
            Index j = 0;

            for (Index k = 0; k < this->ifftstream_tilde_09_fft_levels; k++) {
                j = (BinOpInt)((UBinOpInt)((UBinOpInt)j << (UBinOpInt)1) | (UBinOpInt)((UBinOpInt)x & (UBinOpInt)1));
                x = (BinOpInt)((UBinOpInt)x >> (UBinOpInt)1);
            }

            if (j > i) {
                number temp = buffer[(Index)(2 * i)];
                buffer[(Index)(2 * i)] = buffer[(Index)(2 * j)];
                buffer[(Index)(2 * j)] = temp;
                temp = buffer[(Index)(2 * i + 1)];
                buffer[(Index)(2 * i + 1)] = buffer[(Index)(2 * j + 1)];
                buffer[(Index)(2 * j + 1)] = temp;
            }
        }

        for (Index size = 2; size <= fftsize; size *= 2) {
            Index halfsize = (Index)(size / (number)2);
            Index tablestep = (Index)(fftsize / size);

            for (Index i = 0; i < fftsize; i += size) {
                Index k = 0;

                for (Index j = (Index)(i); j < i + halfsize; j++) {
                    Index l = (Index)(j + halfsize);
                    number real_l = buffer[(Index)(2 * l)];
                    number imag_l = buffer[(Index)(2 * l + 1)];
                    number tpre = real_l * this->ifftstream_tilde_09_fft_costab[(Index)k] + imag_l * this->ifftstream_tilde_09_fft_sintab[(Index)k];
                    number tpim = -real_l * this->ifftstream_tilde_09_fft_sintab[(Index)k] + imag_l * this->ifftstream_tilde_09_fft_costab[(Index)k];
                    number real_j = buffer[(Index)(2 * j)];
                    number imag_j = buffer[(Index)(2 * j + 1)];
                    buffer[(Index)(2 * l)] = real_j - tpre;
                    buffer[(Index)(2 * l + 1)] = imag_j - tpim;
                    buffer[(Index)(2 * j)] = real_j + tpre;
                    buffer[(Index)(2 * j + 1)] = imag_j + tpim;
                    k += tablestep;
                }
            }
        }
    }
}

void ifftstream_tilde_09_fft_reset() {
    this->ifftstream_tilde_09_fft_lastsize = 0;
    this->ifftstream_tilde_09_fft_costab = {};
    this->ifftstream_tilde_09_fft_sintab = {};
    this->ifftstream_tilde_09_fft_levels = -1;
}

void ifftstream_tilde_09_dspsetup(bool force) {
    if ((bool)(this->ifftstream_tilde_09_setupDone) && (bool)(!(bool)(force)))
        return;

    for (int i = 0; i < 1024 * 2; i++) {
        this->ifftstream_tilde_09_inWorkspace[(Index)i] = 0;
        this->ifftstream_tilde_09_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(1024);

    {
        safeframesize = nextpoweroftwo(1024);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 1024) {
        this->ifftstream_tilde_09_datapos = framepos;
    } else {
        this->ifftstream_tilde_09_datapos = 0;
    }

    this->ifftstream_tilde_09_setupDone = true;
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getTempo() {
    return this->globaltransport_getTempoAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    RNBO_UNUSED(sampleOffset);
    return (this->vs > 0 ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState() {
    return this->globaltransport_getStateAtSample(this->sampleOffsetIntoNextAudioBuffer);
}

number globaltransport_getBeatTimeAtMsTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState() == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(this->currenttime(), tempo);
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getTempo() != tempo) {
        MillisecondTime ct = this->currenttime();
        this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTimeAtMsTime(ct));
        this->globaltransport_beatTimeChanges->push(ct);

        fillSignal(
            this->globaltransport_tempo,
            this->vs,
            tempo,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastTempo = tempo;
        this->globaltransport_tempoNeedsReset = true;
        return true;
    }

    return false;
}

number globaltransport_getBeatTime() {
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime());
}

bool globaltransport_setState(number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(this->currenttime(), TransportState(state));
        this->globaltransport_notify = true;
    } else if (this->globaltransport_getState() != state) {
        fillSignal(
            this->globaltransport_state,
            this->vs,
            state,
            (Index)(this->sampleOffsetIntoNextAudioBuffer)
        );

        this->globaltransport_lastState = TransportState(state);
        this->globaltransport_stateNeedsReset = true;

        if (state == 0) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime());
            this->globaltransport_beatTimeChanges->push(this->currenttime());
        }

        return true;
    }

    return false;
}

bool globaltransport_setBeatTime(number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(this->currenttime(), beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime());
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(this->currenttime());
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTimeAtMsTime(this->currenttime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignatureAtMsTime(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignature() {
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime());
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    MillisecondTime msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignatureAtMsTime(this->currenttime() + msOffset);
}

bool globaltransport_setTimeSignature(number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(this->currenttime(), (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature();

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(this->currenttime());
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    cartopol_tilde_01_x = 0;
    cartopol_tilde_01_y = 0;
    fftstream_tilde_01_realIn = 0;
    fftstream_tilde_01_imagIn = 0;
    dspexpr_89_in1 = 0;
    dspexpr_89_in2 = 1;
    dspexpr_90_in1 = 0;
    dspexpr_90_in2 = 512;
    ip_09_value = 1;
    ip_09_impulse = 0;
    p_01_target = 0;
    dspexpr_91_in1 = 0;
    dspexpr_91_in2 = 1;
    poltocar_tilde_01_radius = 0;
    poltocar_tilde_01_angle = 0;
    ifftstream_tilde_01_realIn = 0;
    ifftstream_tilde_01_imagIn = 0;
    dspexpr_92_in1 = 0;
    dspexpr_92_in2 = 0;
    dspexpr_93_in1 = 0;
    dspexpr_93_in2 = 1;
    poltocar_tilde_02_radius = 0;
    poltocar_tilde_02_angle = 0;
    ifftstream_tilde_02_realIn = 0;
    ifftstream_tilde_02_imagIn = 0;
    p_02_target = 0;
    dspexpr_94_in1 = 0;
    dspexpr_94_in2 = 0;
    p_03_target = 0;
    dspexpr_95_in1 = 0;
    dspexpr_95_in2 = 1;
    poltocar_tilde_03_radius = 0;
    poltocar_tilde_03_angle = 0;
    ifftstream_tilde_03_realIn = 0;
    ifftstream_tilde_03_imagIn = 0;
    dspexpr_96_in1 = 0;
    dspexpr_96_in2 = 0;
    p_04_target = 0;
    dspexpr_97_in1 = 0;
    dspexpr_97_in2 = 1;
    poltocar_tilde_04_radius = 0;
    poltocar_tilde_04_angle = 0;
    ifftstream_tilde_04_realIn = 0;
    ifftstream_tilde_04_imagIn = 0;
    dspexpr_98_in1 = 0;
    dspexpr_98_in2 = 0;
    p_05_target = 0;
    dspexpr_99_in1 = 0;
    dspexpr_99_in2 = 1;
    poltocar_tilde_05_radius = 0;
    poltocar_tilde_05_angle = 0;
    ifftstream_tilde_05_realIn = 0;
    ifftstream_tilde_05_imagIn = 0;
    dspexpr_100_in1 = 0;
    dspexpr_100_in2 = 0;
    dspexpr_101_in1 = 0;
    dspexpr_101_in2 = 1;
    poltocar_tilde_06_radius = 0;
    poltocar_tilde_06_angle = 0;
    ifftstream_tilde_06_realIn = 0;
    ifftstream_tilde_06_imagIn = 0;
    p_06_target = 0;
    dspexpr_102_in1 = 0;
    dspexpr_102_in2 = 0;
    p_07_target = 0;
    dspexpr_103_in1 = 0;
    dspexpr_103_in2 = 1;
    poltocar_tilde_07_radius = 0;
    poltocar_tilde_07_angle = 0;
    ifftstream_tilde_07_realIn = 0;
    ifftstream_tilde_07_imagIn = 0;
    dspexpr_104_in1 = 0;
    dspexpr_104_in2 = 0;
    p_08_target = 0;
    dspexpr_105_in1 = 0;
    dspexpr_105_in2 = 1;
    poltocar_tilde_08_radius = 0;
    poltocar_tilde_08_angle = 0;
    ifftstream_tilde_08_realIn = 0;
    ifftstream_tilde_08_imagIn = 0;
    dspexpr_106_in1 = 0;
    dspexpr_106_in2 = 0;
    dspexpr_107_in1 = 0;
    dspexpr_107_in2 = 0;
    dspexpr_107_in3 = 1;
    dspexpr_108_in1 = 0;
    dspexpr_108_in2 = 1;
    poltocar_tilde_09_radius = 0;
    poltocar_tilde_09_angle = 0;
    ifftstream_tilde_09_realIn = 0;
    ifftstream_tilde_09_imagIn = 0;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    signals[2] = nullptr;
    signals[3] = nullptr;
    signals[4] = nullptr;
    signals[5] = nullptr;
    signals[6] = nullptr;
    signals[7] = nullptr;
    signals[8] = nullptr;
    signals[9] = nullptr;
    signals[10] = nullptr;
    signals[11] = nullptr;
    signals[12] = nullptr;
    signals[13] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    fftstream_tilde_01_datapos = 0;
    fftstream_tilde_01_fft_lastsize = 0;
    fftstream_tilde_01_fft_levels = -1;
    fftstream_tilde_01_setupDone = false;
    ip_09_lastIndex = 0;
    ip_09_lastValue = 0;
    ip_09_resetCount = 0;
    ip_09_sigbuf = nullptr;
    ip_09_setupDone = false;
    ifftstream_tilde_01_datapos = 0;
    ifftstream_tilde_01_fft_lastsize = 0;
    ifftstream_tilde_01_fft_levels = -1;
    ifftstream_tilde_01_setupDone = false;
    ifftstream_tilde_02_datapos = 0;
    ifftstream_tilde_02_fft_lastsize = 0;
    ifftstream_tilde_02_fft_levels = -1;
    ifftstream_tilde_02_setupDone = false;
    ifftstream_tilde_03_datapos = 0;
    ifftstream_tilde_03_fft_lastsize = 0;
    ifftstream_tilde_03_fft_levels = -1;
    ifftstream_tilde_03_setupDone = false;
    ifftstream_tilde_04_datapos = 0;
    ifftstream_tilde_04_fft_lastsize = 0;
    ifftstream_tilde_04_fft_levels = -1;
    ifftstream_tilde_04_setupDone = false;
    ifftstream_tilde_05_datapos = 0;
    ifftstream_tilde_05_fft_lastsize = 0;
    ifftstream_tilde_05_fft_levels = -1;
    ifftstream_tilde_05_setupDone = false;
    ifftstream_tilde_06_datapos = 0;
    ifftstream_tilde_06_fft_lastsize = 0;
    ifftstream_tilde_06_fft_levels = -1;
    ifftstream_tilde_06_setupDone = false;
    ifftstream_tilde_07_datapos = 0;
    ifftstream_tilde_07_fft_lastsize = 0;
    ifftstream_tilde_07_fft_levels = -1;
    ifftstream_tilde_07_setupDone = false;
    ifftstream_tilde_08_datapos = 0;
    ifftstream_tilde_08_fft_lastsize = 0;
    ifftstream_tilde_08_fft_levels = -1;
    ifftstream_tilde_08_setupDone = false;
    ifftstream_tilde_09_datapos = 0;
    ifftstream_tilde_09_fft_lastsize = 0;
    ifftstream_tilde_09_fft_levels = -1;
    ifftstream_tilde_09_setupDone = false;
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number cartopol_tilde_01_x;
    number cartopol_tilde_01_y;
    number fftstream_tilde_01_realIn;
    number fftstream_tilde_01_imagIn;
    number dspexpr_89_in1;
    number dspexpr_89_in2;
    number dspexpr_90_in1;
    number dspexpr_90_in2;
    number ip_09_value;
    number ip_09_impulse;
    number p_01_target;
    number dspexpr_91_in1;
    number dspexpr_91_in2;
    number poltocar_tilde_01_radius;
    number poltocar_tilde_01_angle;
    number ifftstream_tilde_01_realIn;
    number ifftstream_tilde_01_imagIn;
    number dspexpr_92_in1;
    number dspexpr_92_in2;
    number dspexpr_93_in1;
    number dspexpr_93_in2;
    number poltocar_tilde_02_radius;
    number poltocar_tilde_02_angle;
    number ifftstream_tilde_02_realIn;
    number ifftstream_tilde_02_imagIn;
    number p_02_target;
    number dspexpr_94_in1;
    number dspexpr_94_in2;
    number p_03_target;
    number dspexpr_95_in1;
    number dspexpr_95_in2;
    number poltocar_tilde_03_radius;
    number poltocar_tilde_03_angle;
    number ifftstream_tilde_03_realIn;
    number ifftstream_tilde_03_imagIn;
    number dspexpr_96_in1;
    number dspexpr_96_in2;
    number p_04_target;
    number dspexpr_97_in1;
    number dspexpr_97_in2;
    number poltocar_tilde_04_radius;
    number poltocar_tilde_04_angle;
    number ifftstream_tilde_04_realIn;
    number ifftstream_tilde_04_imagIn;
    number dspexpr_98_in1;
    number dspexpr_98_in2;
    number p_05_target;
    number dspexpr_99_in1;
    number dspexpr_99_in2;
    number poltocar_tilde_05_radius;
    number poltocar_tilde_05_angle;
    number ifftstream_tilde_05_realIn;
    number ifftstream_tilde_05_imagIn;
    number dspexpr_100_in1;
    number dspexpr_100_in2;
    number dspexpr_101_in1;
    number dspexpr_101_in2;
    number poltocar_tilde_06_radius;
    number poltocar_tilde_06_angle;
    number ifftstream_tilde_06_realIn;
    number ifftstream_tilde_06_imagIn;
    number p_06_target;
    number dspexpr_102_in1;
    number dspexpr_102_in2;
    number p_07_target;
    number dspexpr_103_in1;
    number dspexpr_103_in2;
    number poltocar_tilde_07_radius;
    number poltocar_tilde_07_angle;
    number ifftstream_tilde_07_realIn;
    number ifftstream_tilde_07_imagIn;
    number dspexpr_104_in1;
    number dspexpr_104_in2;
    number p_08_target;
    number dspexpr_105_in1;
    number dspexpr_105_in2;
    number poltocar_tilde_08_radius;
    number poltocar_tilde_08_angle;
    number ifftstream_tilde_08_realIn;
    number ifftstream_tilde_08_imagIn;
    number dspexpr_106_in1;
    number dspexpr_106_in2;
    number dspexpr_107_in1;
    number dspexpr_107_in2;
    number dspexpr_107_in3;
    number dspexpr_108_in1;
    number dspexpr_108_in2;
    number poltocar_tilde_09_radius;
    number poltocar_tilde_09_angle;
    number ifftstream_tilde_09_realIn;
    number ifftstream_tilde_09_imagIn;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[14];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    SampleValue fftstream_tilde_01_inWorkspace[2048] = { };
    SampleValue fftstream_tilde_01_outWorkspace[2048] = { };
    Float32BufferRef fftstream_tilde_01_win_buf;
    SampleIndex fftstream_tilde_01_datapos;
    int fftstream_tilde_01_fft_lastsize;
    list fftstream_tilde_01_fft_costab;
    list fftstream_tilde_01_fft_sintab;
    int fftstream_tilde_01_fft_levels;
    bool fftstream_tilde_01_setupDone;
    SampleIndex ip_09_lastIndex;
    number ip_09_lastValue;
    SampleIndex ip_09_resetCount;
    signal ip_09_sigbuf;
    bool ip_09_setupDone;
    SampleValue ifftstream_tilde_01_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_01_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_01_win_buf;
    SampleIndex ifftstream_tilde_01_datapos;
    int ifftstream_tilde_01_fft_lastsize;
    list ifftstream_tilde_01_fft_costab;
    list ifftstream_tilde_01_fft_sintab;
    int ifftstream_tilde_01_fft_levels;
    bool ifftstream_tilde_01_setupDone;
    SampleValue ifftstream_tilde_02_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_02_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_02_win_buf;
    SampleIndex ifftstream_tilde_02_datapos;
    int ifftstream_tilde_02_fft_lastsize;
    list ifftstream_tilde_02_fft_costab;
    list ifftstream_tilde_02_fft_sintab;
    int ifftstream_tilde_02_fft_levels;
    bool ifftstream_tilde_02_setupDone;
    SampleValue ifftstream_tilde_03_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_03_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_03_win_buf;
    SampleIndex ifftstream_tilde_03_datapos;
    int ifftstream_tilde_03_fft_lastsize;
    list ifftstream_tilde_03_fft_costab;
    list ifftstream_tilde_03_fft_sintab;
    int ifftstream_tilde_03_fft_levels;
    bool ifftstream_tilde_03_setupDone;
    SampleValue ifftstream_tilde_04_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_04_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_04_win_buf;
    SampleIndex ifftstream_tilde_04_datapos;
    int ifftstream_tilde_04_fft_lastsize;
    list ifftstream_tilde_04_fft_costab;
    list ifftstream_tilde_04_fft_sintab;
    int ifftstream_tilde_04_fft_levels;
    bool ifftstream_tilde_04_setupDone;
    SampleValue ifftstream_tilde_05_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_05_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_05_win_buf;
    SampleIndex ifftstream_tilde_05_datapos;
    int ifftstream_tilde_05_fft_lastsize;
    list ifftstream_tilde_05_fft_costab;
    list ifftstream_tilde_05_fft_sintab;
    int ifftstream_tilde_05_fft_levels;
    bool ifftstream_tilde_05_setupDone;
    SampleValue ifftstream_tilde_06_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_06_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_06_win_buf;
    SampleIndex ifftstream_tilde_06_datapos;
    int ifftstream_tilde_06_fft_lastsize;
    list ifftstream_tilde_06_fft_costab;
    list ifftstream_tilde_06_fft_sintab;
    int ifftstream_tilde_06_fft_levels;
    bool ifftstream_tilde_06_setupDone;
    SampleValue ifftstream_tilde_07_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_07_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_07_win_buf;
    SampleIndex ifftstream_tilde_07_datapos;
    int ifftstream_tilde_07_fft_lastsize;
    list ifftstream_tilde_07_fft_costab;
    list ifftstream_tilde_07_fft_sintab;
    int ifftstream_tilde_07_fft_levels;
    bool ifftstream_tilde_07_setupDone;
    SampleValue ifftstream_tilde_08_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_08_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_08_win_buf;
    SampleIndex ifftstream_tilde_08_datapos;
    int ifftstream_tilde_08_fft_lastsize;
    list ifftstream_tilde_08_fft_costab;
    list ifftstream_tilde_08_fft_sintab;
    int ifftstream_tilde_08_fft_levels;
    bool ifftstream_tilde_08_setupDone;
    SampleValue ifftstream_tilde_09_inWorkspace[2048] = { };
    SampleValue ifftstream_tilde_09_outWorkspace[2048] = { };
    Float32BufferRef ifftstream_tilde_09_win_buf;
    SampleIndex ifftstream_tilde_09_datapos;
    int ifftstream_tilde_09_fft_lastsize;
    list ifftstream_tilde_09_fft_costab;
    list ifftstream_tilde_09_fft_sintab;
    int ifftstream_tilde_09_fft_levels;
    bool ifftstream_tilde_09_setupDone;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    DataRef RNBODefaultFftWindow;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_493* p_01;
    RNBOSubpatcher_494* p_02;
    RNBOSubpatcher_495* p_03;
    RNBOSubpatcher_496* p_04;
    RNBOSubpatcher_497* p_05;
    RNBOSubpatcher_498* p_06;
    RNBOSubpatcher_499* p_07;
    RNBOSubpatcher_500* p_08;

};

PatcherInterface* creaternbomatic()
{
    return new rnbomatic();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creaternbomatic;
}

} // end RNBO namespace

