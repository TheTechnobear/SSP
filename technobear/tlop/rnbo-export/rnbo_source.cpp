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

class RNBOSubpatcher_09 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_09()
    {
    }
    
    ~RNBOSubpatcher_09()
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
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number minimum(number x, number y) {
        return (y < x ? y : x);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    Index vectorsize() {
        return this->vs;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "rate")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "mode")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "loop")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "begin")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "crossfade")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "end")) {
            return 5;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 6;
        }
    
        if (!stringCompare(paramid, "button_obj-43/bangval")) {
            return 7;
        }
    
        if (!stringCompare(paramid, "button_obj-46/bangval")) {
            return 8;
        }
    
        if (!stringCompare(paramid, "button_obj-22/bangval")) {
            return 9;
        }
    
        if (!stringCompare(paramid, "number_obj-20/value")) {
            return 10;
        }
    
        if (!stringCompare(paramid, "number_obj-5/value")) {
            return 11;
        }
    
        return INVALID_INDEX;
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
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    
        this->groove_01_perform(
            this->groove_01_rate_auto,
            this->groove_01_begin,
            this->groove_01_end,
            this->signals[0],
            out2,
            n
        );
    
        this->dspexpr_01_perform(this->signals[0], this->dspexpr_01_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 1; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->groove_01_dspsetup(forceDSPSetup);
        this->data_01_dspsetup(forceDSPSetup);
    
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
        this->param_01_getPresetValue(getSubState(preset, "rate"));
        this->param_02_getPresetValue(getSubState(preset, "mode"));
        this->param_03_getPresetValue(getSubState(preset, "loop"));
        this->param_04_getPresetValue(getSubState(preset, "begin"));
        this->param_05_getPresetValue(getSubState(preset, "crossfade"));
        this->param_06_getPresetValue(getSubState(preset, "end"));
        this->param_07_getPresetValue(getSubState(preset, "gain"));
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
        case 2:
            this->param_03_value_set(v);
            break;
        case 3:
            this->param_04_value_set(v);
            break;
        case 4:
            this->param_05_value_set(v);
            break;
        case 5:
            this->param_06_value_set(v);
            break;
        case 6:
            this->param_07_value_set(v);
            break;
        case 7:
            this->button_01_bangval_bang();
            break;
        case 8:
            this->button_02_bangval_bang();
            break;
        case 9:
            this->button_03_bangval_bang();
            break;
        case 10:
            this->numberobj_01_value_set(v);
            break;
        case 11:
            this->numberobj_02_value_set(v);
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
        case 2:
            return this->param_03_value;
        case 3:
            return this->param_04_value;
        case 4:
            return this->param_05_value;
        case 5:
            return this->param_06_value;
        case 6:
            return this->param_07_value;
        case 10:
            return this->numberobj_01_value;
        case 11:
            return this->numberobj_02_value;
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
        return 12;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "rate";
        case 1:
            return "mode";
        case 2:
            return "loop";
        case 3:
            return "begin";
        case 4:
            return "crossfade";
        case 5:
            return "end";
        case 6:
            return "gain";
        case 7:
            return "button_01_bangval";
        case 8:
            return "button_02_bangval";
        case 9:
            return "button_03_bangval";
        case 10:
            return "numberobj_01_value";
        case 11:
            return "numberobj_02_value";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer1/rate";
        case 1:
            return "layer1/mode";
        case 2:
            return "layer1/loop";
        case 3:
            return "layer1/begin";
        case 4:
            return "layer1/crossfade";
        case 5:
            return "layer1/end";
        case 6:
            return "layer1/gain";
        case 7:
            return "layer1/button_obj-43/bangval";
        case 8:
            return "layer1/button_obj-46/bangval";
        case 9:
            return "layer1/button_obj-22/bangval";
        case 10:
            return "layer1/number_obj-20/value";
        case 11:
            return "layer1/number_obj-5/value";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = -4;
                info->max = 4;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal1[] = {"Play", "Stop"};
                info->enumValues = eVal1;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal2[] = {"false", "true"};
                info->enumValues = eVal2;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 100;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 6:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 10;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 7:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 8:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 9:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 10:
                info->type = ParameterTypeNumber;
                info->initialValue = 10000;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 11:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
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
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
                return normalizedValue;
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 2);
                }
    
                return normalizedValue;
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 100 ? 100 : value));
                ParameterValue normalizedValue = (value - 0) / (100 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 0:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 2);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (100 - 0);
                }
            }
        case 0:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
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
        case 2:
            return this->param_03_value_constrain(value);
        case 3:
            return this->param_04_value_constrain(value);
        case 4:
            return this->param_05_value_constrain(value);
        case 5:
            return this->param_06_value_constrain(value);
        case 6:
            return this->param_07_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
        this->updateTime(time);
    
        switch (tag) {
        case TAG("format"):
            if (TAG("layer1/number_obj-20") == objectId)
                this->numberobj_01_format_set(payload);
    
            if (TAG("layer1/number_obj-5") == objectId)
                this->numberobj_02_format_set(payload);
    
            break;
        }
    }
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("setup"):
            return "setup";
        case TAG("layer1/number_obj-20"):
            return "layer1/number_obj-20";
        case TAG("layer1/number_obj-5"):
            return "layer1/number_obj-5";
        case TAG("format"):
            return "format";
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
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->groove_01_buffer = new Float32Buffer(this->getPatcher()->layer1);
            this->data_01_buffer = new Float32Buffer(this->getPatcher()->layer1);
            this->data_01_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->groove_01_buffer = new Float32Buffer(this->getPatcher()->layer1);
        this->data_01_buffer = new Float32Buffer(this->getPatcher()->layer1);
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
    
        this->groove_01_rate_auto_set(v);
    }
    
    void param_02_value_set(number v) {
        v = this->param_02_value_constrain(v);
        this->param_02_value = v;
        this->sendParameter(1, false);
    
        if (this->param_02_value != this->param_02_lastValue) {
            this->getEngine()->presetTouched();
            this->param_02_lastValue = this->param_02_value;
        }
    
        this->select_01_input_number_set(v);
    }
    
    void param_03_value_set(number v) {
        v = this->param_03_value_constrain(v);
        this->param_03_value = v;
        this->sendParameter(2, false);
    
        if (this->param_03_value != this->param_03_lastValue) {
            this->getEngine()->presetTouched();
            this->param_03_lastValue = this->param_03_value;
        }
    
        this->groove_01_loop_set(v);
    }
    
    void param_04_value_set(number v) {
        v = this->param_04_value_constrain(v);
        this->param_04_value = v;
        this->sendParameter(3, false);
    
        if (this->param_04_value != this->param_04_lastValue) {
            this->getEngine()->presetTouched();
            this->param_04_lastValue = this->param_04_value;
        }
    
        this->expr_03_in1_set(v);
    }
    
    void param_05_value_set(number v) {
        v = this->param_05_value_constrain(v);
        this->param_05_value = v;
        this->sendParameter(4, false);
    
        if (this->param_05_value != this->param_05_lastValue) {
            this->getEngine()->presetTouched();
            this->param_05_lastValue = this->param_05_value;
        }
    
        this->groove_01_crossfade_set(v);
    }
    
    void param_06_value_set(number v) {
        v = this->param_06_value_constrain(v);
        this->param_06_value = v;
        this->sendParameter(5, false);
    
        if (this->param_06_value != this->param_06_lastValue) {
            this->getEngine()->presetTouched();
            this->param_06_lastValue = this->param_06_value;
        }
    
        this->expr_04_in1_set(v);
    }
    
    void param_07_value_set(number v) {
        v = this->param_07_value_constrain(v);
        this->param_07_value = v;
        this->sendParameter(6, false);
    
        if (this->param_07_value != this->param_07_lastValue) {
            this->getEngine()->presetTouched();
            this->param_07_lastValue = this->param_07_value;
        }
    
        this->dspexpr_01_in2_set(v);
    }
    
    void button_03_bangval_bang() {
        this->sendParameter(9, true);
        this->data_01_info_bang();
    }
    
    void numberobj_01_format_set(number v) {
        if (v == 0) {
            this->numberobj_01_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_01_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_01_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_01_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_01_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_01_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_01_currentFormat = 6;
        }
    }
    
    void numberobj_02_format_set(number v) {
        if (v == 0) {
            this->numberobj_02_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_02_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_02_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_02_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_02_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_02_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_02_currentFormat = 6;
        }
    }
    
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
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->data_01_init();
        this->numberobj_01_init();
        this->numberobj_02_init();
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
    
        {
            this->scheduleParamInit(2, 0);
        }
    
        {
            this->scheduleParamInit(3, 0);
        }
    
        {
            this->scheduleParamInit(4, 0);
        }
    
        {
            this->scheduleParamInit(5, 0);
        }
    
        {
            this->scheduleParamInit(6, 0);
        }
    }
    
    void allocateDataRefs() {
        this->groove_01_buffer = this->groove_01_buffer->allocateIfNeeded();
        this->data_01_buffer = this->data_01_buffer->allocateIfNeeded();
    }
    
    static number param_01_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void groove_01_rate_auto_set(number v) {
        this->groove_01_rate_auto = v;
    }
    
    static number param_02_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void trigger_01_out2_set(number ) {}
    
    void groove_01_rate_bang_bang() {
        this->groove_01_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_01_incomingChange = 1;
    }
    
    void button_01_bangval_bang() {
        this->sendParameter(7, true);
        this->groove_01_rate_bang_bang();
    }
    
    void receive_01_output_bang_bang() {
        this->button_01_bangval_bang();
    }
    
    void send_01_input_bang_bang() {
        this->receive_01_output_bang_bang();
        this->getPatcher()->p_01_source_Playu45layer1_bang_bang();
    }
    
    void trigger_01_out1_bang() {
        this->send_01_input_bang_bang();
    }
    
    void trigger_01_input_bang_bang() {
        this->trigger_01_out2_set(0);
        this->trigger_01_out1_bang();
    }
    
    void select_01_match1_bang() {
        this->trigger_01_input_bang_bang();
    }
    
    void groove_01_stop_bang() {
        this->groove_01_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_01_incomingChange = 0;
    }
    
    void button_02_bangval_bang() {
        this->sendParameter(8, true);
        this->groove_01_stop_bang();
    }
    
    void receive_02_output_bang_bang() {
        this->button_02_bangval_bang();
    }
    
    void send_02_input_bang_bang() {
        this->receive_02_output_bang_bang();
        this->getPatcher()->p_01_source_Stopu45layer1_bang_bang();
    }
    
    void trigger_02_out1_bang() {
        this->send_02_input_bang_bang();
    }
    
    void trigger_02_input_bang_bang() {
        this->trigger_02_out1_bang();
    }
    
    void select_01_match2_bang() {
        this->trigger_02_input_bang_bang();
    }
    
    void select_01_nomatch_number_set(number ) {}
    
    void select_01_input_number_set(number v) {
        if (v == this->select_01_test1)
            this->select_01_match1_bang();
        else if (v == this->select_01_test2)
            this->select_01_match2_bang();
        else
            this->select_01_nomatch_number_set(v);
    }
    
    static number param_03_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_01_loop_set(number v) {
        this->groove_01_loop = v;
    }
    
    static number param_04_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void numberobj_02_output_set(number ) {}
    
    void numberobj_02_value_set(number v) {
        this->numberobj_02_value_setter(v);
        v = this->numberobj_02_value;
        this->sendParameter(11, false);
        this->numberobj_02_output_set(v);
    }
    
    void groove_01_begin_set(number v) {
        this->groove_01_begin = v;
    }
    
    void expr_03_out1_set(number v) {
        this->expr_03_out1 = v;
        this->numberobj_02_value_set(this->expr_03_out1);
        this->groove_01_begin_set(this->expr_03_out1);
    }
    
    void expr_03_in1_set(number in1) {
        this->expr_03_in1 = in1;
        this->expr_03_out1_set(this->expr_03_in1 * this->expr_03_in2);//#map:layer1/*_obj-23:1
    }
    
    static number param_05_value_constrain(number v) {
        v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)100 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_01_crossfade_set(number v) {
        this->groove_01_crossfade = v;
        this->groove_01_crossfadeInSamples = this->mstosamps(v);
    }
    
    static number param_06_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_01_end_set(number v) {
        this->groove_01_end = v;
    }
    
    void expr_04_out1_set(number v) {
        this->expr_04_out1 = v;
        this->groove_01_end_set(this->expr_04_out1);
    }
    
    void expr_04_in1_set(number in1) {
        this->expr_04_in1 = in1;
        this->expr_04_out1_set(this->expr_04_in1 * this->expr_04_in2);//#map:layer1/*_obj-28:1
    }
    
    static number param_07_value_constrain(number v) {
        v = (v > 10 ? 10 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)10 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_01_in2_set(number v) {
        this->dspexpr_01_in2 = v;
    }
    
    void expr_04_in2_set(number v) {
        this->expr_04_in2 = v;
        this->expr_04_in1_set(this->expr_04_in1);
    }
    
    void expr_03_in2_set(number v) {
        this->expr_03_in2 = v;
        this->expr_03_in1_set(this->expr_03_in1);
    }
    
    void numberobj_01_output_set(number ) {}
    
    void numberobj_01_value_set(number v) {
        this->numberobj_01_value_setter(v);
        v = this->numberobj_01_value;
        this->sendParameter(10, false);
        this->numberobj_01_output_set(v);
    }
    
    void eventoutlet_01_in1_number_set(number v) {
        this->getPatcher()->p_01_out3_number_set(v);
    }
    
    void expr_01_out1_set(number v) {
        this->expr_01_out1 = v;
        this->expr_04_in2_set(this->expr_01_out1);
        this->expr_03_in2_set(this->expr_01_out1);
        this->numberobj_01_value_set(this->expr_01_out1);
        this->eventoutlet_01_in1_number_set(this->expr_01_out1);
    }
    
    void expr_01_in1_set(number in1) {
        this->expr_01_in1 = in1;
    
        this->expr_01_out1_set(
            (this->expr_01_in2 == 0 ? 0 : (this->expr_01_in2 == 0. ? 0. : this->expr_01_in1 / this->expr_01_in2))
        );//#map:layer1//_obj-15:1
    }
    
    void data_01_sizeout_set(number v) {
        this->data_01_sizeout = v;
        this->expr_01_in1_set(v);
    }
    
    void data_01_chanout_set(number ) {}
    
    void expr_01_in2_set(number v) {
        this->expr_01_in2 = v;
        this->expr_01_in1_set(this->expr_01_in1);
    }
    
    void expr_02_out1_set(number v) {
        this->expr_02_out1 = v;
        this->expr_01_in2_set(this->expr_02_out1);
    }
    
    void expr_02_in1_set(number in1) {
        this->expr_02_in1 = in1;
    
        this->expr_02_out1_set(
            (this->expr_02_in2 == 0 ? 0 : (this->expr_02_in2 == 0. ? 0. : this->expr_02_in1 / this->expr_02_in2))
        );//#map:layer1//_obj-16:1
    }
    
    void data_01_srout_set(number v) {
        this->expr_02_in1_set(v);
    }
    
    void data_01_info_bang() {
        this->data_01_report();
    }
    
    void groove_01_perform(
        number rate_auto,
        number begin,
        number end,
        Sample * out1,
        Sample * sync,
        Index n
    ) {
        RNBO_UNUSED(out1);
        auto __groove_01_crossfade = this->groove_01_crossfade;
        auto __groove_01_loop = this->groove_01_loop;
        auto __groove_01_playStatus = this->groove_01_playStatus;
        auto __groove_01_readIndex = this->groove_01_readIndex;
        auto __groove_01_incomingChange = this->groove_01_incomingChange;
        auto __groove_01_changeIncomingInSamples = this->groove_01_changeIncomingInSamples;
        SampleArray<1> out = {out1};
        SampleIndex bufferLength = (SampleIndex)(this->groove_01_buffer->getSize());
        Index i = 0;
    
        if (bufferLength > 1) {
            number effectiveChannels = this->minimum(this->groove_01_buffer->getChannels(), 1);
            number srMult = 0.001 * this->groove_01_buffer->getSampleRate();
            number srInv = (number)1 / this->samplerate();
            number rateMult = this->groove_01_buffer->getSampleRate() * srInv;
    
            for (; i < n; i++) {
                Index channel = 0;
                number offset = 0;
                number loopMin = begin * srMult;
                loopMin = (loopMin > bufferLength - 1 ? bufferLength - 1 : (loopMin < 0 ? 0 : loopMin));
                number loopMax = (end < 0 ? bufferLength : end * srMult);
                loopMax = (loopMax > bufferLength ? bufferLength : (loopMax < 0 ? 0 : loopMax));
    
                if (loopMin >= loopMax) {
                    offset = loopMax;
                    loopMax = bufferLength;
                    loopMin -= offset;
                }
    
                number loopLength = loopMax - loopMin;
                number currentRate = rate_auto * rateMult;
                number currentSync = 0;
    
                if (__groove_01_changeIncomingInSamples > 0) {
                    __groove_01_changeIncomingInSamples--;
    
                    if (__groove_01_changeIncomingInSamples <= 0) {
                        if (__groove_01_incomingChange == 1) {
                            if (currentRate < 0) {
                                __groove_01_readIndex = loopMax - 1;
                            } else {
                                __groove_01_readIndex = loopMin;
                            }
    
                            __groove_01_playStatus = 1;
                        } else if (__groove_01_incomingChange == 0) {
                            __groove_01_playStatus = 0;
                        }
    
                        __groove_01_incomingChange = 2;
                    }
                }
    
                if (loopLength > 0) {
                    if (currentRate != 0) {
                        if (__groove_01_playStatus == 1) {
                            if ((bool)(__groove_01_loop)) {
                                while (__groove_01_readIndex < loopMin) {
                                    __groove_01_readIndex += loopLength;
                                }
    
                                while (__groove_01_readIndex >= loopMax) {
                                    __groove_01_readIndex -= loopLength;
                                }
                            } else if (__groove_01_readIndex >= loopMax || __groove_01_readIndex < loopMin) {
                                __groove_01_playStatus = 0;
                                break;
                            }
    
                            for (; channel < effectiveChannels; channel++) {
                                number outSample = (currentRate == 1 ? this->groove_01_getSample((Index)(channel), rnbo_trunc(__groove_01_readIndex), offset, bufferLength) : this->groove_01_interpolatedSample(
                                    (Index)(channel),
                                    __groove_01_readIndex,
                                    loopMax,
                                    loopLength,
                                    offset,
                                    bufferLength
                                ));
    
                                if (__groove_01_crossfade > 0) {
                                    out[(Index)channel][(Index)i] = this->groove_01_crossfadedSample(
                                        outSample,
                                        __groove_01_readIndex,
                                        (Index)(channel),
                                        currentRate,
                                        loopMin,
                                        loopMax,
                                        loopLength,
                                        offset,
                                        bufferLength
                                    );
                                } else {
                                    out[(Index)channel][(Index)i] = outSample;
                                }
                            }
    
                            {
                                currentSync = this->groove_01_calcSync(__groove_01_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                            }
    
                            __groove_01_readIndex += currentRate;
                        }
                    } else {
                        if (__groove_01_playStatus == 1) {
                            currentSync = this->groove_01_calcSync(__groove_01_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                        }
                    }
                }
    
                {
                    sync[(Index)i] = currentSync;
                }
    
                for (; channel < 1; channel++) {
                    if (__groove_01_playStatus <= 0)
                        sync[(Index)i] = 0;
    
                    out[(Index)channel][(Index)i] = 0;
                }
            }
        }
    
        for (; i < n; i++) {
            if (__groove_01_playStatus <= 0)
                sync[(Index)i] = 0;
    
            for (number channel = 0; channel < 1; channel++) {
                out[(Index)channel][(Index)i] = 0;
            }
        }
    
        this->groove_01_changeIncomingInSamples = __groove_01_changeIncomingInSamples;
        this->groove_01_incomingChange = __groove_01_incomingChange;
        this->groove_01_readIndex = __groove_01_readIndex;
        this->groove_01_playStatus = __groove_01_playStatus;
    }
    
    void dspexpr_01_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_01_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_01_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_01_value = localvalue;
    }
    
    void numberobj_02_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_02_value = localvalue;
    }
    
    number groove_01_getSample(
        Index channel,
        SampleIndex index,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        if (offset > 0) {
            index += offset;
    
            if (index >= bufferLength)
                index -= bufferLength;
        }
    
        return this->groove_01_buffer->getSample(channel, index);
    }
    
    number groove_01_interpolatedSample(
        Index channel,
        number index,
        SampleIndex end,
        SampleIndex length,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(index));
        number i_x = index - index1;
        number i_1px = 1. + i_x;
        number i_1mx = 1. - i_x;
        number i_2mx = 2. - i_x;
        number i_a = i_1mx * i_2mx;
        number i_b = i_1px * i_x;
        number i_p1 = -.1666667 * i_a * i_x;
        number i_p2 = .5 * i_1px * i_a;
        number i_p3 = .5 * i_b * i_2mx;
        number i_p4 = -.1666667 * i_b * i_1mx;
        SampleIndex index2 = (SampleIndex)(index1 + 1);
    
        if (index2 >= end)
            index2 -= length;
    
        SampleIndex index3 = (SampleIndex)(index1 + 2);
    
        if (index3 >= end)
            index3 -= length;
    
        SampleIndex index4 = (SampleIndex)(index1 + 3);
    
        if (index4 >= end)
            index4 -= length;
    
        return this->groove_01_getSample(channel, index1, offset, bufferLength) * i_p1 + this->groove_01_getSample(channel, index2, offset, bufferLength) * i_p2 + this->groove_01_getSample(channel, index3, offset, bufferLength) * i_p3 + this->groove_01_getSample(channel, index4, offset, bufferLength) * i_p4;
    }
    
    number groove_01_crossfadedSample(
        SampleValue out,
        number readIndex,
        Index channel,
        number rate,
        number loopMin,
        number loopMax,
        number loopLength,
        number offset,
        number bufferLength
    ) {
        number crossFadeStart1 = this->maximum(loopMin - this->groove_01_crossfadeInSamples, 0);
        number crossFadeEnd1 = this->minimum(crossFadeStart1 + this->groove_01_crossfadeInSamples, bufferLength);
        number crossFadeStart2 = crossFadeStart1 + loopLength;
        number crossFadeEnd2 = this->minimum(crossFadeEnd1 + loopLength, bufferLength);
        number crossFadeLength = crossFadeEnd2 - crossFadeStart2;
    
        if (crossFadeLength > 0) {
            crossFadeEnd1 = crossFadeStart1 + crossFadeLength;
            number diff = -1;
            number addFactor = 0;
    
            if (readIndex >= crossFadeStart2) {
                diff = readIndex - crossFadeStart2;
                addFactor = -1;
            } else if (readIndex < crossFadeEnd1) {
                diff = crossFadeEnd1 - readIndex + loopMax - crossFadeStart2;
                addFactor = 1;
            }
    
            if (diff >= 0) {
                number out2ReadIndex = readIndex + loopLength * addFactor;
                number out2 = (rate == 1 ? this->groove_01_getSample(channel, rnbo_trunc(out2ReadIndex), offset, bufferLength) : this->groove_01_interpolatedSample(channel, out2ReadIndex, loopMax, loopLength, offset, bufferLength));
                number out2Factor = diff / crossFadeLength;
                number out1Factor = 1 - out2Factor;
                return out * out1Factor + out2 * out2Factor;
            }
        }
    
        return out;
    }
    
    number groove_01_calcSync(
        number readIndex,
        number offset,
        number loopMin,
        number loopLength,
        SampleIndex bufferLength,
        number srInv
    ) {
        RNBO_UNUSED(srInv);
        RNBO_UNUSED(loopLength);
        RNBO_UNUSED(loopMin);
    
        {
            if (offset > 0) {
                readIndex += offset;
    
                if (readIndex >= bufferLength)
                    readIndex -= bufferLength;
            }
    
            {
                return readIndex / bufferLength;
            }
        }
    }
    
    void groove_01_dspsetup(bool force) {
        if ((bool)(this->groove_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->groove_01_crossfadeInSamples = this->mstosamps(this->groove_01_crossfade);
        this->groove_01_setupDone = true;
    }
    
    void param_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_01_value;
    }
    
    void param_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_01_value_set(preset["value"]);
    }
    
    void param_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_02_value;
    }
    
    void param_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_02_value_set(preset["value"]);
    }
    
    void param_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_03_value;
    }
    
    void param_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_03_value_set(preset["value"]);
    }
    
    void data_01_init() {
        this->data_01_buffer->setWantsFill(true);
    }
    
    void data_01_report() {
        this->data_01_sizeout_set(this->data_01_buffer->getSize());
        this->data_01_chanout_set(this->data_01_buffer->getChannels());
        this->data_01_srout_set(this->data_01_buffer->getSampleRate());
    }
    
    Index data_01_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        number size = 0;
        return (Index)(size);
    }
    
    void data_01_dspsetup(bool force) {
        if ((bool)(this->data_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        if (this->data_01_sizemode == 2) {
            this->data_01_buffer = this->data_01_buffer->setSize((Index)(this->mstosamps(this->data_01_sizems)));
            updateDataRef(this, this->data_01_buffer);
        } else if (this->data_01_sizemode == 3) {
            this->data_01_buffer = this->data_01_buffer->setSize(this->data_01_evaluateSizeExpr(this->samplerate(), this->vectorsize()));
            updateDataRef(this, this->data_01_buffer);
        }
    
        this->data_01_setupDone = true;
    }
    
    void data_01_bufferUpdated() {
        this->data_01_report();
    }
    
    void numberobj_01_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer1/number_obj-20"), 1, this->_currentTime);
    }
    
    void numberobj_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_01_value;
    }
    
    void numberobj_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_01_value_set(preset["value"]);
    }
    
    void param_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_04_value;
    }
    
    void param_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_04_value_set(preset["value"]);
    }
    
    void numberobj_02_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer1/number_obj-5"), 1, this->_currentTime);
    }
    
    void numberobj_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_02_value;
    }
    
    void numberobj_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_02_value_set(preset["value"]);
    }
    
    void param_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_05_value;
    }
    
    void param_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_05_value_set(preset["value"]);
    }
    
    void param_06_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_06_value;
    }
    
    void param_06_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_06_value_set(preset["value"]);
    }
    
    void param_07_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_07_value;
    }
    
    void param_07_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_07_value_set(preset["value"]);
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
        receive_01_output_number = 0;
        groove_01_rate_auto = 1;
        groove_01_begin = 0;
        groove_01_end = -1;
        groove_01_loop = 1;
        groove_01_crossfade = 0;
        param_01_value = 1;
        receive_02_output_number = 0;
        param_02_value = 1;
        send_01_input_number = 0;
        select_01_test1 = 0;
        select_01_test2 = 1;
        send_02_input_number = 0;
        param_03_value = 1;
        expr_01_in1 = 0;
        expr_01_in2 = 0;
        expr_01_out1 = 0;
        data_01_sizeout = 0;
        data_01_size = 0;
        data_01_sizems = 0;
        data_01_channels = 1;
        numberobj_01_value = 10000;
        numberobj_01_value_setter(numberobj_01_value);
        expr_02_in1 = 0;
        expr_02_in2 = 1000;
        expr_02_out1 = 0;
        expr_03_in1 = 0;
        expr_03_in2 = 0;
        expr_03_out1 = 0;
        param_04_value = 0;
        numberobj_02_value = 0;
        numberobj_02_value_setter(numberobj_02_value);
        param_05_value = 1;
        expr_04_in1 = 0;
        expr_04_in2 = 0;
        expr_04_out1 = 0;
        param_06_value = 1;
        param_07_value = 1;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        groove_01_readIndex = 0;
        groove_01_playStatus = 0;
        groove_01_changeIncomingInSamples = 0;
        groove_01_incomingChange = 2;
        groove_01_crossfadeInSamples = 0;
        groove_01_setupDone = false;
        param_01_lastValue = 0;
        param_02_lastValue = 0;
        param_03_lastValue = 0;
        data_01_sizemode = 0;
        data_01_setupDone = false;
        numberobj_01_currentFormat = 6;
        numberobj_01_lastValue = 0;
        param_04_lastValue = 0;
        numberobj_02_currentFormat = 6;
        numberobj_02_lastValue = 0;
        param_05_lastValue = 0;
        param_06_lastValue = 0;
        param_07_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number receive_01_output_number;
        list receive_01_output_list;
        number groove_01_rate_auto;
        number groove_01_begin;
        number groove_01_end;
        number groove_01_loop;
        number groove_01_crossfade;
        number param_01_value;
        number receive_02_output_number;
        list receive_02_output_list;
        number param_02_value;
        number send_01_input_number;
        list send_01_input_list;
        number select_01_test1;
        number select_01_test2;
        number send_02_input_number;
        list send_02_input_list;
        number param_03_value;
        number expr_01_in1;
        number expr_01_in2;
        number expr_01_out1;
        number data_01_sizeout;
        number data_01_size;
        number data_01_sizems;
        number data_01_channels;
        number numberobj_01_value;
        number expr_02_in1;
        number expr_02_in2;
        number expr_02_out1;
        number expr_03_in1;
        number expr_03_in2;
        number expr_03_out1;
        number param_04_value;
        number numberobj_02_value;
        number param_05_value;
        number expr_04_in1;
        number expr_04_in2;
        number expr_04_out1;
        number param_06_value;
        number param_07_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[1];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef groove_01_buffer;
        number groove_01_readIndex;
        Index groove_01_playStatus;
        SampleIndex groove_01_changeIncomingInSamples;
        Int groove_01_incomingChange;
        SampleIndex groove_01_crossfadeInSamples;
        bool groove_01_setupDone;
        number param_01_lastValue;
        number param_02_lastValue;
        number param_03_lastValue;
        Float32BufferRef data_01_buffer;
        Int data_01_sizemode;
        bool data_01_setupDone;
        Int numberobj_01_currentFormat;
        number numberobj_01_lastValue;
        number param_04_lastValue;
        Int numberobj_02_currentFormat;
        number numberobj_02_lastValue;
        number param_05_lastValue;
        number param_06_lastValue;
        number param_07_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_10 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_10()
    {
    }
    
    ~RNBOSubpatcher_10()
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
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number minimum(number x, number y) {
        return (y < x ? y : x);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    Index vectorsize() {
        return this->vs;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "rate")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "mode")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "loop")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "begin")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "crossfade")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "end")) {
            return 5;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 6;
        }
    
        if (!stringCompare(paramid, "button_obj-43/bangval")) {
            return 7;
        }
    
        if (!stringCompare(paramid, "button_obj-46/bangval")) {
            return 8;
        }
    
        if (!stringCompare(paramid, "button_obj-22/bangval")) {
            return 9;
        }
    
        if (!stringCompare(paramid, "number_obj-20/value")) {
            return 10;
        }
    
        if (!stringCompare(paramid, "number_obj-5/value")) {
            return 11;
        }
    
        return INVALID_INDEX;
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
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    
        this->groove_02_perform(
            this->groove_02_rate_auto,
            this->groove_02_begin,
            this->groove_02_end,
            this->signals[0],
            out2,
            n
        );
    
        this->dspexpr_02_perform(this->signals[0], this->dspexpr_02_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 1; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->groove_02_dspsetup(forceDSPSetup);
        this->data_02_dspsetup(forceDSPSetup);
    
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
        this->param_08_getPresetValue(getSubState(preset, "rate"));
        this->param_09_getPresetValue(getSubState(preset, "mode"));
        this->param_10_getPresetValue(getSubState(preset, "loop"));
        this->param_11_getPresetValue(getSubState(preset, "begin"));
        this->param_12_getPresetValue(getSubState(preset, "crossfade"));
        this->param_13_getPresetValue(getSubState(preset, "end"));
        this->param_14_getPresetValue(getSubState(preset, "gain"));
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
            this->param_08_value_set(v);
            break;
        case 1:
            this->param_09_value_set(v);
            break;
        case 2:
            this->param_10_value_set(v);
            break;
        case 3:
            this->param_11_value_set(v);
            break;
        case 4:
            this->param_12_value_set(v);
            break;
        case 5:
            this->param_13_value_set(v);
            break;
        case 6:
            this->param_14_value_set(v);
            break;
        case 7:
            this->button_04_bangval_bang();
            break;
        case 8:
            this->button_05_bangval_bang();
            break;
        case 9:
            this->button_06_bangval_bang();
            break;
        case 10:
            this->numberobj_03_value_set(v);
            break;
        case 11:
            this->numberobj_04_value_set(v);
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
            return this->param_08_value;
        case 1:
            return this->param_09_value;
        case 2:
            return this->param_10_value;
        case 3:
            return this->param_11_value;
        case 4:
            return this->param_12_value;
        case 5:
            return this->param_13_value;
        case 6:
            return this->param_14_value;
        case 10:
            return this->numberobj_03_value;
        case 11:
            return this->numberobj_04_value;
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
        return 12;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "rate";
        case 1:
            return "mode";
        case 2:
            return "loop";
        case 3:
            return "begin";
        case 4:
            return "crossfade";
        case 5:
            return "end";
        case 6:
            return "gain";
        case 7:
            return "button_04_bangval";
        case 8:
            return "button_05_bangval";
        case 9:
            return "button_06_bangval";
        case 10:
            return "numberobj_03_value";
        case 11:
            return "numberobj_04_value";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer2/rate";
        case 1:
            return "layer2/mode";
        case 2:
            return "layer2/loop";
        case 3:
            return "layer2/begin";
        case 4:
            return "layer2/crossfade";
        case 5:
            return "layer2/end";
        case 6:
            return "layer2/gain";
        case 7:
            return "layer2/button_obj-43/bangval";
        case 8:
            return "layer2/button_obj-46/bangval";
        case 9:
            return "layer2/button_obj-22/bangval";
        case 10:
            return "layer2/number_obj-20/value";
        case 11:
            return "layer2/number_obj-5/value";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = -4;
                info->max = 4;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal1[] = {"Play", "Stop"};
                info->enumValues = eVal1;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal2[] = {"false", "true"};
                info->enumValues = eVal2;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 100;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 6:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 10;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 7:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 8:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 9:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 10:
                info->type = ParameterTypeNumber;
                info->initialValue = 10000;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 11:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
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
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
                return normalizedValue;
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 2);
                }
    
                return normalizedValue;
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 100 ? 100 : value));
                ParameterValue normalizedValue = (value - 0) / (100 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 0:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 2);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (100 - 0);
                }
            }
        case 0:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_08_value_constrain(value);
        case 1:
            return this->param_09_value_constrain(value);
        case 2:
            return this->param_10_value_constrain(value);
        case 3:
            return this->param_11_value_constrain(value);
        case 4:
            return this->param_12_value_constrain(value);
        case 5:
            return this->param_13_value_constrain(value);
        case 6:
            return this->param_14_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
        this->updateTime(time);
    
        switch (tag) {
        case TAG("format"):
            if (TAG("layer2/number_obj-20") == objectId)
                this->numberobj_03_format_set(payload);
    
            if (TAG("layer2/number_obj-5") == objectId)
                this->numberobj_04_format_set(payload);
    
            break;
        }
    }
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("setup"):
            return "setup";
        case TAG("layer2/number_obj-20"):
            return "layer2/number_obj-20";
        case TAG("layer2/number_obj-5"):
            return "layer2/number_obj-5";
        case TAG("format"):
            return "format";
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
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 1) {
            this->groove_02_buffer = new Float32Buffer(this->getPatcher()->layer2);
            this->data_02_buffer = new Float32Buffer(this->getPatcher()->layer2);
            this->data_02_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->groove_02_buffer = new Float32Buffer(this->getPatcher()->layer2);
        this->data_02_buffer = new Float32Buffer(this->getPatcher()->layer2);
    }
    
    protected:
    
    void param_08_value_set(number v) {
        v = this->param_08_value_constrain(v);
        this->param_08_value = v;
        this->sendParameter(0, false);
    
        if (this->param_08_value != this->param_08_lastValue) {
            this->getEngine()->presetTouched();
            this->param_08_lastValue = this->param_08_value;
        }
    
        this->groove_02_rate_auto_set(v);
    }
    
    void param_09_value_set(number v) {
        v = this->param_09_value_constrain(v);
        this->param_09_value = v;
        this->sendParameter(1, false);
    
        if (this->param_09_value != this->param_09_lastValue) {
            this->getEngine()->presetTouched();
            this->param_09_lastValue = this->param_09_value;
        }
    
        this->select_02_input_number_set(v);
    }
    
    void param_10_value_set(number v) {
        v = this->param_10_value_constrain(v);
        this->param_10_value = v;
        this->sendParameter(2, false);
    
        if (this->param_10_value != this->param_10_lastValue) {
            this->getEngine()->presetTouched();
            this->param_10_lastValue = this->param_10_value;
        }
    
        this->groove_02_loop_set(v);
    }
    
    void param_11_value_set(number v) {
        v = this->param_11_value_constrain(v);
        this->param_11_value = v;
        this->sendParameter(3, false);
    
        if (this->param_11_value != this->param_11_lastValue) {
            this->getEngine()->presetTouched();
            this->param_11_lastValue = this->param_11_value;
        }
    
        this->expr_07_in1_set(v);
    }
    
    void param_12_value_set(number v) {
        v = this->param_12_value_constrain(v);
        this->param_12_value = v;
        this->sendParameter(4, false);
    
        if (this->param_12_value != this->param_12_lastValue) {
            this->getEngine()->presetTouched();
            this->param_12_lastValue = this->param_12_value;
        }
    
        this->groove_02_crossfade_set(v);
    }
    
    void param_13_value_set(number v) {
        v = this->param_13_value_constrain(v);
        this->param_13_value = v;
        this->sendParameter(5, false);
    
        if (this->param_13_value != this->param_13_lastValue) {
            this->getEngine()->presetTouched();
            this->param_13_lastValue = this->param_13_value;
        }
    
        this->expr_08_in1_set(v);
    }
    
    void param_14_value_set(number v) {
        v = this->param_14_value_constrain(v);
        this->param_14_value = v;
        this->sendParameter(6, false);
    
        if (this->param_14_value != this->param_14_lastValue) {
            this->getEngine()->presetTouched();
            this->param_14_lastValue = this->param_14_value;
        }
    
        this->dspexpr_02_in2_set(v);
    }
    
    void button_06_bangval_bang() {
        this->sendParameter(9, true);
        this->data_02_info_bang();
    }
    
    void numberobj_03_format_set(number v) {
        if (v == 0) {
            this->numberobj_03_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_03_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_03_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_03_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_03_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_03_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_03_currentFormat = 6;
        }
    }
    
    void numberobj_04_format_set(number v) {
        if (v == 0) {
            this->numberobj_04_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_04_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_04_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_04_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_04_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_04_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_04_currentFormat = 6;
        }
    }
    
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
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->data_02_init();
        this->numberobj_03_init();
        this->numberobj_04_init();
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
    
        {
            this->scheduleParamInit(2, 0);
        }
    
        {
            this->scheduleParamInit(3, 0);
        }
    
        {
            this->scheduleParamInit(4, 0);
        }
    
        {
            this->scheduleParamInit(5, 0);
        }
    
        {
            this->scheduleParamInit(6, 0);
        }
    }
    
    void allocateDataRefs() {
        this->groove_02_buffer = this->groove_02_buffer->allocateIfNeeded();
        this->data_02_buffer = this->data_02_buffer->allocateIfNeeded();
    }
    
    static number param_08_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void groove_02_rate_auto_set(number v) {
        this->groove_02_rate_auto = v;
    }
    
    static number param_09_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void trigger_03_out2_set(number ) {}
    
    void groove_02_rate_bang_bang() {
        this->groove_02_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_02_incomingChange = 1;
    }
    
    void button_04_bangval_bang() {
        this->sendParameter(7, true);
        this->groove_02_rate_bang_bang();
    }
    
    void receive_03_output_bang_bang() {
        this->button_04_bangval_bang();
    }
    
    void send_03_input_bang_bang() {
        this->receive_03_output_bang_bang();
        this->getPatcher()->p_02_source_Playu45layer2_bang_bang();
    }
    
    void trigger_03_out1_bang() {
        this->send_03_input_bang_bang();
    }
    
    void trigger_03_input_bang_bang() {
        this->trigger_03_out2_set(0);
        this->trigger_03_out1_bang();
    }
    
    void select_02_match1_bang() {
        this->trigger_03_input_bang_bang();
    }
    
    void groove_02_stop_bang() {
        this->groove_02_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_02_incomingChange = 0;
    }
    
    void button_05_bangval_bang() {
        this->sendParameter(8, true);
        this->groove_02_stop_bang();
    }
    
    void receive_04_output_bang_bang() {
        this->button_05_bangval_bang();
    }
    
    void send_04_input_bang_bang() {
        this->receive_04_output_bang_bang();
        this->getPatcher()->p_02_source_Stopu45layer2_bang_bang();
    }
    
    void trigger_04_out1_bang() {
        this->send_04_input_bang_bang();
    }
    
    void trigger_04_input_bang_bang() {
        this->trigger_04_out1_bang();
    }
    
    void select_02_match2_bang() {
        this->trigger_04_input_bang_bang();
    }
    
    void select_02_nomatch_number_set(number ) {}
    
    void select_02_input_number_set(number v) {
        if (v == this->select_02_test1)
            this->select_02_match1_bang();
        else if (v == this->select_02_test2)
            this->select_02_match2_bang();
        else
            this->select_02_nomatch_number_set(v);
    }
    
    static number param_10_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_02_loop_set(number v) {
        this->groove_02_loop = v;
    }
    
    static number param_11_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void numberobj_04_output_set(number ) {}
    
    void numberobj_04_value_set(number v) {
        this->numberobj_04_value_setter(v);
        v = this->numberobj_04_value;
        this->sendParameter(11, false);
        this->numberobj_04_output_set(v);
    }
    
    void groove_02_begin_set(number v) {
        this->groove_02_begin = v;
    }
    
    void expr_07_out1_set(number v) {
        this->expr_07_out1 = v;
        this->numberobj_04_value_set(this->expr_07_out1);
        this->groove_02_begin_set(this->expr_07_out1);
    }
    
    void expr_07_in1_set(number in1) {
        this->expr_07_in1 = in1;
        this->expr_07_out1_set(this->expr_07_in1 * this->expr_07_in2);//#map:layer2/*_obj-23:1
    }
    
    static number param_12_value_constrain(number v) {
        v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)100 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_02_crossfade_set(number v) {
        this->groove_02_crossfade = v;
        this->groove_02_crossfadeInSamples = this->mstosamps(v);
    }
    
    static number param_13_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_02_end_set(number v) {
        this->groove_02_end = v;
    }
    
    void expr_08_out1_set(number v) {
        this->expr_08_out1 = v;
        this->groove_02_end_set(this->expr_08_out1);
    }
    
    void expr_08_in1_set(number in1) {
        this->expr_08_in1 = in1;
        this->expr_08_out1_set(this->expr_08_in1 * this->expr_08_in2);//#map:layer2/*_obj-28:1
    }
    
    static number param_14_value_constrain(number v) {
        v = (v > 10 ? 10 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)10 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_02_in2_set(number v) {
        this->dspexpr_02_in2 = v;
    }
    
    void expr_08_in2_set(number v) {
        this->expr_08_in2 = v;
        this->expr_08_in1_set(this->expr_08_in1);
    }
    
    void expr_07_in2_set(number v) {
        this->expr_07_in2 = v;
        this->expr_07_in1_set(this->expr_07_in1);
    }
    
    void numberobj_03_output_set(number ) {}
    
    void numberobj_03_value_set(number v) {
        this->numberobj_03_value_setter(v);
        v = this->numberobj_03_value;
        this->sendParameter(10, false);
        this->numberobj_03_output_set(v);
    }
    
    void eventoutlet_02_in1_number_set(number v) {
        this->getPatcher()->p_02_out3_number_set(v);
    }
    
    void expr_05_out1_set(number v) {
        this->expr_05_out1 = v;
        this->expr_08_in2_set(this->expr_05_out1);
        this->expr_07_in2_set(this->expr_05_out1);
        this->numberobj_03_value_set(this->expr_05_out1);
        this->eventoutlet_02_in1_number_set(this->expr_05_out1);
    }
    
    void expr_05_in1_set(number in1) {
        this->expr_05_in1 = in1;
    
        this->expr_05_out1_set(
            (this->expr_05_in2 == 0 ? 0 : (this->expr_05_in2 == 0. ? 0. : this->expr_05_in1 / this->expr_05_in2))
        );//#map:layer2//_obj-15:1
    }
    
    void data_02_sizeout_set(number v) {
        this->data_02_sizeout = v;
        this->expr_05_in1_set(v);
    }
    
    void data_02_chanout_set(number ) {}
    
    void expr_05_in2_set(number v) {
        this->expr_05_in2 = v;
        this->expr_05_in1_set(this->expr_05_in1);
    }
    
    void expr_06_out1_set(number v) {
        this->expr_06_out1 = v;
        this->expr_05_in2_set(this->expr_06_out1);
    }
    
    void expr_06_in1_set(number in1) {
        this->expr_06_in1 = in1;
    
        this->expr_06_out1_set(
            (this->expr_06_in2 == 0 ? 0 : (this->expr_06_in2 == 0. ? 0. : this->expr_06_in1 / this->expr_06_in2))
        );//#map:layer2//_obj-16:1
    }
    
    void data_02_srout_set(number v) {
        this->expr_06_in1_set(v);
    }
    
    void data_02_info_bang() {
        this->data_02_report();
    }
    
    void groove_02_perform(
        number rate_auto,
        number begin,
        number end,
        Sample * out1,
        Sample * sync,
        Index n
    ) {
        RNBO_UNUSED(out1);
        auto __groove_02_crossfade = this->groove_02_crossfade;
        auto __groove_02_loop = this->groove_02_loop;
        auto __groove_02_playStatus = this->groove_02_playStatus;
        auto __groove_02_readIndex = this->groove_02_readIndex;
        auto __groove_02_incomingChange = this->groove_02_incomingChange;
        auto __groove_02_changeIncomingInSamples = this->groove_02_changeIncomingInSamples;
        SampleArray<1> out = {out1};
        SampleIndex bufferLength = (SampleIndex)(this->groove_02_buffer->getSize());
        Index i = 0;
    
        if (bufferLength > 1) {
            number effectiveChannels = this->minimum(this->groove_02_buffer->getChannels(), 1);
            number srMult = 0.001 * this->groove_02_buffer->getSampleRate();
            number srInv = (number)1 / this->samplerate();
            number rateMult = this->groove_02_buffer->getSampleRate() * srInv;
    
            for (; i < n; i++) {
                Index channel = 0;
                number offset = 0;
                number loopMin = begin * srMult;
                loopMin = (loopMin > bufferLength - 1 ? bufferLength - 1 : (loopMin < 0 ? 0 : loopMin));
                number loopMax = (end < 0 ? bufferLength : end * srMult);
                loopMax = (loopMax > bufferLength ? bufferLength : (loopMax < 0 ? 0 : loopMax));
    
                if (loopMin >= loopMax) {
                    offset = loopMax;
                    loopMax = bufferLength;
                    loopMin -= offset;
                }
    
                number loopLength = loopMax - loopMin;
                number currentRate = rate_auto * rateMult;
                number currentSync = 0;
    
                if (__groove_02_changeIncomingInSamples > 0) {
                    __groove_02_changeIncomingInSamples--;
    
                    if (__groove_02_changeIncomingInSamples <= 0) {
                        if (__groove_02_incomingChange == 1) {
                            if (currentRate < 0) {
                                __groove_02_readIndex = loopMax - 1;
                            } else {
                                __groove_02_readIndex = loopMin;
                            }
    
                            __groove_02_playStatus = 1;
                        } else if (__groove_02_incomingChange == 0) {
                            __groove_02_playStatus = 0;
                        }
    
                        __groove_02_incomingChange = 2;
                    }
                }
    
                if (loopLength > 0) {
                    if (currentRate != 0) {
                        if (__groove_02_playStatus == 1) {
                            if ((bool)(__groove_02_loop)) {
                                while (__groove_02_readIndex < loopMin) {
                                    __groove_02_readIndex += loopLength;
                                }
    
                                while (__groove_02_readIndex >= loopMax) {
                                    __groove_02_readIndex -= loopLength;
                                }
                            } else if (__groove_02_readIndex >= loopMax || __groove_02_readIndex < loopMin) {
                                __groove_02_playStatus = 0;
                                break;
                            }
    
                            for (; channel < effectiveChannels; channel++) {
                                number outSample = (currentRate == 1 ? this->groove_02_getSample((Index)(channel), rnbo_trunc(__groove_02_readIndex), offset, bufferLength) : this->groove_02_interpolatedSample(
                                    (Index)(channel),
                                    __groove_02_readIndex,
                                    loopMax,
                                    loopLength,
                                    offset,
                                    bufferLength
                                ));
    
                                if (__groove_02_crossfade > 0) {
                                    out[(Index)channel][(Index)i] = this->groove_02_crossfadedSample(
                                        outSample,
                                        __groove_02_readIndex,
                                        (Index)(channel),
                                        currentRate,
                                        loopMin,
                                        loopMax,
                                        loopLength,
                                        offset,
                                        bufferLength
                                    );
                                } else {
                                    out[(Index)channel][(Index)i] = outSample;
                                }
                            }
    
                            {
                                currentSync = this->groove_02_calcSync(__groove_02_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                            }
    
                            __groove_02_readIndex += currentRate;
                        }
                    } else {
                        if (__groove_02_playStatus == 1) {
                            currentSync = this->groove_02_calcSync(__groove_02_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                        }
                    }
                }
    
                {
                    sync[(Index)i] = currentSync;
                }
    
                for (; channel < 1; channel++) {
                    if (__groove_02_playStatus <= 0)
                        sync[(Index)i] = 0;
    
                    out[(Index)channel][(Index)i] = 0;
                }
            }
        }
    
        for (; i < n; i++) {
            if (__groove_02_playStatus <= 0)
                sync[(Index)i] = 0;
    
            for (number channel = 0; channel < 1; channel++) {
                out[(Index)channel][(Index)i] = 0;
            }
        }
    
        this->groove_02_changeIncomingInSamples = __groove_02_changeIncomingInSamples;
        this->groove_02_incomingChange = __groove_02_incomingChange;
        this->groove_02_readIndex = __groove_02_readIndex;
        this->groove_02_playStatus = __groove_02_playStatus;
    }
    
    void dspexpr_02_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_03_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_03_value = localvalue;
    }
    
    void numberobj_04_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_04_value = localvalue;
    }
    
    number groove_02_getSample(
        Index channel,
        SampleIndex index,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        if (offset > 0) {
            index += offset;
    
            if (index >= bufferLength)
                index -= bufferLength;
        }
    
        return this->groove_02_buffer->getSample(channel, index);
    }
    
    number groove_02_interpolatedSample(
        Index channel,
        number index,
        SampleIndex end,
        SampleIndex length,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(index));
        number i_x = index - index1;
        number i_1px = 1. + i_x;
        number i_1mx = 1. - i_x;
        number i_2mx = 2. - i_x;
        number i_a = i_1mx * i_2mx;
        number i_b = i_1px * i_x;
        number i_p1 = -.1666667 * i_a * i_x;
        number i_p2 = .5 * i_1px * i_a;
        number i_p3 = .5 * i_b * i_2mx;
        number i_p4 = -.1666667 * i_b * i_1mx;
        SampleIndex index2 = (SampleIndex)(index1 + 1);
    
        if (index2 >= end)
            index2 -= length;
    
        SampleIndex index3 = (SampleIndex)(index1 + 2);
    
        if (index3 >= end)
            index3 -= length;
    
        SampleIndex index4 = (SampleIndex)(index1 + 3);
    
        if (index4 >= end)
            index4 -= length;
    
        return this->groove_02_getSample(channel, index1, offset, bufferLength) * i_p1 + this->groove_02_getSample(channel, index2, offset, bufferLength) * i_p2 + this->groove_02_getSample(channel, index3, offset, bufferLength) * i_p3 + this->groove_02_getSample(channel, index4, offset, bufferLength) * i_p4;
    }
    
    number groove_02_crossfadedSample(
        SampleValue out,
        number readIndex,
        Index channel,
        number rate,
        number loopMin,
        number loopMax,
        number loopLength,
        number offset,
        number bufferLength
    ) {
        number crossFadeStart1 = this->maximum(loopMin - this->groove_02_crossfadeInSamples, 0);
        number crossFadeEnd1 = this->minimum(crossFadeStart1 + this->groove_02_crossfadeInSamples, bufferLength);
        number crossFadeStart2 = crossFadeStart1 + loopLength;
        number crossFadeEnd2 = this->minimum(crossFadeEnd1 + loopLength, bufferLength);
        number crossFadeLength = crossFadeEnd2 - crossFadeStart2;
    
        if (crossFadeLength > 0) {
            crossFadeEnd1 = crossFadeStart1 + crossFadeLength;
            number diff = -1;
            number addFactor = 0;
    
            if (readIndex >= crossFadeStart2) {
                diff = readIndex - crossFadeStart2;
                addFactor = -1;
            } else if (readIndex < crossFadeEnd1) {
                diff = crossFadeEnd1 - readIndex + loopMax - crossFadeStart2;
                addFactor = 1;
            }
    
            if (diff >= 0) {
                number out2ReadIndex = readIndex + loopLength * addFactor;
                number out2 = (rate == 1 ? this->groove_02_getSample(channel, rnbo_trunc(out2ReadIndex), offset, bufferLength) : this->groove_02_interpolatedSample(channel, out2ReadIndex, loopMax, loopLength, offset, bufferLength));
                number out2Factor = diff / crossFadeLength;
                number out1Factor = 1 - out2Factor;
                return out * out1Factor + out2 * out2Factor;
            }
        }
    
        return out;
    }
    
    number groove_02_calcSync(
        number readIndex,
        number offset,
        number loopMin,
        number loopLength,
        SampleIndex bufferLength,
        number srInv
    ) {
        RNBO_UNUSED(srInv);
        RNBO_UNUSED(loopLength);
        RNBO_UNUSED(loopMin);
    
        {
            if (offset > 0) {
                readIndex += offset;
    
                if (readIndex >= bufferLength)
                    readIndex -= bufferLength;
            }
    
            {
                return readIndex / bufferLength;
            }
        }
    }
    
    void groove_02_dspsetup(bool force) {
        if ((bool)(this->groove_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->groove_02_crossfadeInSamples = this->mstosamps(this->groove_02_crossfade);
        this->groove_02_setupDone = true;
    }
    
    void param_08_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_08_value;
    }
    
    void param_08_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_08_value_set(preset["value"]);
    }
    
    void param_09_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_09_value;
    }
    
    void param_09_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_09_value_set(preset["value"]);
    }
    
    void param_10_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_10_value;
    }
    
    void param_10_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_10_value_set(preset["value"]);
    }
    
    void data_02_init() {
        this->data_02_buffer->setWantsFill(true);
    }
    
    void data_02_report() {
        this->data_02_sizeout_set(this->data_02_buffer->getSize());
        this->data_02_chanout_set(this->data_02_buffer->getChannels());
        this->data_02_srout_set(this->data_02_buffer->getSampleRate());
    }
    
    Index data_02_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        number size = 0;
        return (Index)(size);
    }
    
    void data_02_dspsetup(bool force) {
        if ((bool)(this->data_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        if (this->data_02_sizemode == 2) {
            this->data_02_buffer = this->data_02_buffer->setSize((Index)(this->mstosamps(this->data_02_sizems)));
            updateDataRef(this, this->data_02_buffer);
        } else if (this->data_02_sizemode == 3) {
            this->data_02_buffer = this->data_02_buffer->setSize(this->data_02_evaluateSizeExpr(this->samplerate(), this->vectorsize()));
            updateDataRef(this, this->data_02_buffer);
        }
    
        this->data_02_setupDone = true;
    }
    
    void data_02_bufferUpdated() {
        this->data_02_report();
    }
    
    void numberobj_03_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer2/number_obj-20"), 1, this->_currentTime);
    }
    
    void numberobj_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_03_value;
    }
    
    void numberobj_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_03_value_set(preset["value"]);
    }
    
    void param_11_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_11_value;
    }
    
    void param_11_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_11_value_set(preset["value"]);
    }
    
    void numberobj_04_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer2/number_obj-5"), 1, this->_currentTime);
    }
    
    void numberobj_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_04_value;
    }
    
    void numberobj_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_04_value_set(preset["value"]);
    }
    
    void param_12_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_12_value;
    }
    
    void param_12_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_12_value_set(preset["value"]);
    }
    
    void param_13_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_13_value;
    }
    
    void param_13_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_13_value_set(preset["value"]);
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
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 0;
        receive_03_output_number = 0;
        groove_02_rate_auto = 1;
        groove_02_begin = 0;
        groove_02_end = -1;
        groove_02_loop = 1;
        groove_02_crossfade = 0;
        param_08_value = 1;
        receive_04_output_number = 0;
        param_09_value = 1;
        send_03_input_number = 0;
        select_02_test1 = 0;
        select_02_test2 = 1;
        send_04_input_number = 0;
        param_10_value = 1;
        expr_05_in1 = 0;
        expr_05_in2 = 0;
        expr_05_out1 = 0;
        data_02_sizeout = 0;
        data_02_size = 0;
        data_02_sizems = 0;
        data_02_channels = 1;
        numberobj_03_value = 10000;
        numberobj_03_value_setter(numberobj_03_value);
        expr_06_in1 = 0;
        expr_06_in2 = 1000;
        expr_06_out1 = 0;
        expr_07_in1 = 0;
        expr_07_in2 = 0;
        expr_07_out1 = 0;
        param_11_value = 0;
        numberobj_04_value = 0;
        numberobj_04_value_setter(numberobj_04_value);
        param_12_value = 1;
        expr_08_in1 = 0;
        expr_08_in2 = 0;
        expr_08_out1 = 0;
        param_13_value = 1;
        param_14_value = 1;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        groove_02_readIndex = 0;
        groove_02_playStatus = 0;
        groove_02_changeIncomingInSamples = 0;
        groove_02_incomingChange = 2;
        groove_02_crossfadeInSamples = 0;
        groove_02_setupDone = false;
        param_08_lastValue = 0;
        param_09_lastValue = 0;
        param_10_lastValue = 0;
        data_02_sizemode = 0;
        data_02_setupDone = false;
        numberobj_03_currentFormat = 6;
        numberobj_03_lastValue = 0;
        param_11_lastValue = 0;
        numberobj_04_currentFormat = 6;
        numberobj_04_lastValue = 0;
        param_12_lastValue = 0;
        param_13_lastValue = 0;
        param_14_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number receive_03_output_number;
        list receive_03_output_list;
        number groove_02_rate_auto;
        number groove_02_begin;
        number groove_02_end;
        number groove_02_loop;
        number groove_02_crossfade;
        number param_08_value;
        number receive_04_output_number;
        list receive_04_output_list;
        number param_09_value;
        number send_03_input_number;
        list send_03_input_list;
        number select_02_test1;
        number select_02_test2;
        number send_04_input_number;
        list send_04_input_list;
        number param_10_value;
        number expr_05_in1;
        number expr_05_in2;
        number expr_05_out1;
        number data_02_sizeout;
        number data_02_size;
        number data_02_sizems;
        number data_02_channels;
        number numberobj_03_value;
        number expr_06_in1;
        number expr_06_in2;
        number expr_06_out1;
        number expr_07_in1;
        number expr_07_in2;
        number expr_07_out1;
        number param_11_value;
        number numberobj_04_value;
        number param_12_value;
        number expr_08_in1;
        number expr_08_in2;
        number expr_08_out1;
        number param_13_value;
        number param_14_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[1];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef groove_02_buffer;
        number groove_02_readIndex;
        Index groove_02_playStatus;
        SampleIndex groove_02_changeIncomingInSamples;
        Int groove_02_incomingChange;
        SampleIndex groove_02_crossfadeInSamples;
        bool groove_02_setupDone;
        number param_08_lastValue;
        number param_09_lastValue;
        number param_10_lastValue;
        Float32BufferRef data_02_buffer;
        Int data_02_sizemode;
        bool data_02_setupDone;
        Int numberobj_03_currentFormat;
        number numberobj_03_lastValue;
        number param_11_lastValue;
        Int numberobj_04_currentFormat;
        number numberobj_04_lastValue;
        number param_12_lastValue;
        number param_13_lastValue;
        number param_14_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_11 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_11()
    {
    }
    
    ~RNBOSubpatcher_11()
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
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number minimum(number x, number y) {
        return (y < x ? y : x);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    Index vectorsize() {
        return this->vs;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "rate")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "mode")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "loop")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "begin")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "crossfade")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "end")) {
            return 5;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 6;
        }
    
        if (!stringCompare(paramid, "button_obj-43/bangval")) {
            return 7;
        }
    
        if (!stringCompare(paramid, "button_obj-46/bangval")) {
            return 8;
        }
    
        if (!stringCompare(paramid, "button_obj-22/bangval")) {
            return 9;
        }
    
        if (!stringCompare(paramid, "number_obj-20/value")) {
            return 10;
        }
    
        if (!stringCompare(paramid, "number_obj-5/value")) {
            return 11;
        }
    
        return INVALID_INDEX;
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
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    
        this->groove_03_perform(
            this->groove_03_rate_auto,
            this->groove_03_begin,
            this->groove_03_end,
            this->signals[0],
            out2,
            n
        );
    
        this->dspexpr_03_perform(this->signals[0], this->dspexpr_03_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 1; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->groove_03_dspsetup(forceDSPSetup);
        this->data_03_dspsetup(forceDSPSetup);
    
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
        this->param_15_getPresetValue(getSubState(preset, "rate"));
        this->param_16_getPresetValue(getSubState(preset, "mode"));
        this->param_17_getPresetValue(getSubState(preset, "loop"));
        this->param_18_getPresetValue(getSubState(preset, "begin"));
        this->param_19_getPresetValue(getSubState(preset, "crossfade"));
        this->param_20_getPresetValue(getSubState(preset, "end"));
        this->param_21_getPresetValue(getSubState(preset, "gain"));
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
        case 2:
            this->param_17_value_set(v);
            break;
        case 3:
            this->param_18_value_set(v);
            break;
        case 4:
            this->param_19_value_set(v);
            break;
        case 5:
            this->param_20_value_set(v);
            break;
        case 6:
            this->param_21_value_set(v);
            break;
        case 7:
            this->button_07_bangval_bang();
            break;
        case 8:
            this->button_08_bangval_bang();
            break;
        case 9:
            this->button_09_bangval_bang();
            break;
        case 10:
            this->numberobj_05_value_set(v);
            break;
        case 11:
            this->numberobj_06_value_set(v);
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
        case 2:
            return this->param_17_value;
        case 3:
            return this->param_18_value;
        case 4:
            return this->param_19_value;
        case 5:
            return this->param_20_value;
        case 6:
            return this->param_21_value;
        case 10:
            return this->numberobj_05_value;
        case 11:
            return this->numberobj_06_value;
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
        return 12;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "rate";
        case 1:
            return "mode";
        case 2:
            return "loop";
        case 3:
            return "begin";
        case 4:
            return "crossfade";
        case 5:
            return "end";
        case 6:
            return "gain";
        case 7:
            return "button_07_bangval";
        case 8:
            return "button_08_bangval";
        case 9:
            return "button_09_bangval";
        case 10:
            return "numberobj_05_value";
        case 11:
            return "numberobj_06_value";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer3/rate";
        case 1:
            return "layer3/mode";
        case 2:
            return "layer3/loop";
        case 3:
            return "layer3/begin";
        case 4:
            return "layer3/crossfade";
        case 5:
            return "layer3/end";
        case 6:
            return "layer3/gain";
        case 7:
            return "layer3/button_obj-43/bangval";
        case 8:
            return "layer3/button_obj-46/bangval";
        case 9:
            return "layer3/button_obj-22/bangval";
        case 10:
            return "layer3/number_obj-20/value";
        case 11:
            return "layer3/number_obj-5/value";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = -4;
                info->max = 4;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal1[] = {"Play", "Stop"};
                info->enumValues = eVal1;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal2[] = {"false", "true"};
                info->enumValues = eVal2;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 100;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 6:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 10;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 7:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 8:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 9:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 10:
                info->type = ParameterTypeNumber;
                info->initialValue = 10000;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 11:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
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
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
                return normalizedValue;
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 2);
                }
    
                return normalizedValue;
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 100 ? 100 : value));
                ParameterValue normalizedValue = (value - 0) / (100 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 0:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 2);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (100 - 0);
                }
            }
        case 0:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
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
        case 2:
            return this->param_17_value_constrain(value);
        case 3:
            return this->param_18_value_constrain(value);
        case 4:
            return this->param_19_value_constrain(value);
        case 5:
            return this->param_20_value_constrain(value);
        case 6:
            return this->param_21_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
        this->updateTime(time);
    
        switch (tag) {
        case TAG("format"):
            if (TAG("layer3/number_obj-20") == objectId)
                this->numberobj_05_format_set(payload);
    
            if (TAG("layer3/number_obj-5") == objectId)
                this->numberobj_06_format_set(payload);
    
            break;
        }
    }
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("setup"):
            return "setup";
        case TAG("layer3/number_obj-20"):
            return "layer3/number_obj-20";
        case TAG("layer3/number_obj-5"):
            return "layer3/number_obj-5";
        case TAG("format"):
            return "format";
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
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 2) {
            this->groove_03_buffer = new Float32Buffer(this->getPatcher()->layer3);
            this->data_03_buffer = new Float32Buffer(this->getPatcher()->layer3);
            this->data_03_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->groove_03_buffer = new Float32Buffer(this->getPatcher()->layer3);
        this->data_03_buffer = new Float32Buffer(this->getPatcher()->layer3);
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
    
        this->groove_03_rate_auto_set(v);
    }
    
    void param_16_value_set(number v) {
        v = this->param_16_value_constrain(v);
        this->param_16_value = v;
        this->sendParameter(1, false);
    
        if (this->param_16_value != this->param_16_lastValue) {
            this->getEngine()->presetTouched();
            this->param_16_lastValue = this->param_16_value;
        }
    
        this->select_03_input_number_set(v);
    }
    
    void param_17_value_set(number v) {
        v = this->param_17_value_constrain(v);
        this->param_17_value = v;
        this->sendParameter(2, false);
    
        if (this->param_17_value != this->param_17_lastValue) {
            this->getEngine()->presetTouched();
            this->param_17_lastValue = this->param_17_value;
        }
    
        this->groove_03_loop_set(v);
    }
    
    void param_18_value_set(number v) {
        v = this->param_18_value_constrain(v);
        this->param_18_value = v;
        this->sendParameter(3, false);
    
        if (this->param_18_value != this->param_18_lastValue) {
            this->getEngine()->presetTouched();
            this->param_18_lastValue = this->param_18_value;
        }
    
        this->expr_11_in1_set(v);
    }
    
    void param_19_value_set(number v) {
        v = this->param_19_value_constrain(v);
        this->param_19_value = v;
        this->sendParameter(4, false);
    
        if (this->param_19_value != this->param_19_lastValue) {
            this->getEngine()->presetTouched();
            this->param_19_lastValue = this->param_19_value;
        }
    
        this->groove_03_crossfade_set(v);
    }
    
    void param_20_value_set(number v) {
        v = this->param_20_value_constrain(v);
        this->param_20_value = v;
        this->sendParameter(5, false);
    
        if (this->param_20_value != this->param_20_lastValue) {
            this->getEngine()->presetTouched();
            this->param_20_lastValue = this->param_20_value;
        }
    
        this->expr_12_in1_set(v);
    }
    
    void param_21_value_set(number v) {
        v = this->param_21_value_constrain(v);
        this->param_21_value = v;
        this->sendParameter(6, false);
    
        if (this->param_21_value != this->param_21_lastValue) {
            this->getEngine()->presetTouched();
            this->param_21_lastValue = this->param_21_value;
        }
    
        this->dspexpr_03_in2_set(v);
    }
    
    void button_09_bangval_bang() {
        this->sendParameter(9, true);
        this->data_03_info_bang();
    }
    
    void numberobj_05_format_set(number v) {
        if (v == 0) {
            this->numberobj_05_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_05_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_05_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_05_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_05_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_05_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_05_currentFormat = 6;
        }
    }
    
    void numberobj_06_format_set(number v) {
        if (v == 0) {
            this->numberobj_06_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_06_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_06_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_06_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_06_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_06_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_06_currentFormat = 6;
        }
    }
    
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
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->data_03_init();
        this->numberobj_05_init();
        this->numberobj_06_init();
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
    
        {
            this->scheduleParamInit(2, 0);
        }
    
        {
            this->scheduleParamInit(3, 0);
        }
    
        {
            this->scheduleParamInit(4, 0);
        }
    
        {
            this->scheduleParamInit(5, 0);
        }
    
        {
            this->scheduleParamInit(6, 0);
        }
    }
    
    void allocateDataRefs() {
        this->groove_03_buffer = this->groove_03_buffer->allocateIfNeeded();
        this->data_03_buffer = this->data_03_buffer->allocateIfNeeded();
    }
    
    static number param_15_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void groove_03_rate_auto_set(number v) {
        this->groove_03_rate_auto = v;
    }
    
    static number param_16_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void trigger_05_out2_set(number ) {}
    
    void groove_03_rate_bang_bang() {
        this->groove_03_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_03_incomingChange = 1;
    }
    
    void button_07_bangval_bang() {
        this->sendParameter(7, true);
        this->groove_03_rate_bang_bang();
    }
    
    void receive_05_output_bang_bang() {
        this->button_07_bangval_bang();
    }
    
    void send_05_input_bang_bang() {
        this->receive_05_output_bang_bang();
        this->getPatcher()->p_03_source_Playu45layer3_bang_bang();
    }
    
    void trigger_05_out1_bang() {
        this->send_05_input_bang_bang();
    }
    
    void trigger_05_input_bang_bang() {
        this->trigger_05_out2_set(0);
        this->trigger_05_out1_bang();
    }
    
    void select_03_match1_bang() {
        this->trigger_05_input_bang_bang();
    }
    
    void groove_03_stop_bang() {
        this->groove_03_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_03_incomingChange = 0;
    }
    
    void button_08_bangval_bang() {
        this->sendParameter(8, true);
        this->groove_03_stop_bang();
    }
    
    void receive_06_output_bang_bang() {
        this->button_08_bangval_bang();
    }
    
    void send_06_input_bang_bang() {
        this->receive_06_output_bang_bang();
        this->getPatcher()->p_03_source_Stopu45layer3_bang_bang();
    }
    
    void trigger_06_out1_bang() {
        this->send_06_input_bang_bang();
    }
    
    void trigger_06_input_bang_bang() {
        this->trigger_06_out1_bang();
    }
    
    void select_03_match2_bang() {
        this->trigger_06_input_bang_bang();
    }
    
    void select_03_nomatch_number_set(number ) {}
    
    void select_03_input_number_set(number v) {
        if (v == this->select_03_test1)
            this->select_03_match1_bang();
        else if (v == this->select_03_test2)
            this->select_03_match2_bang();
        else
            this->select_03_nomatch_number_set(v);
    }
    
    static number param_17_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_03_loop_set(number v) {
        this->groove_03_loop = v;
    }
    
    static number param_18_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void numberobj_06_output_set(number ) {}
    
    void numberobj_06_value_set(number v) {
        this->numberobj_06_value_setter(v);
        v = this->numberobj_06_value;
        this->sendParameter(11, false);
        this->numberobj_06_output_set(v);
    }
    
    void groove_03_begin_set(number v) {
        this->groove_03_begin = v;
    }
    
    void expr_11_out1_set(number v) {
        this->expr_11_out1 = v;
        this->numberobj_06_value_set(this->expr_11_out1);
        this->groove_03_begin_set(this->expr_11_out1);
    }
    
    void expr_11_in1_set(number in1) {
        this->expr_11_in1 = in1;
        this->expr_11_out1_set(this->expr_11_in1 * this->expr_11_in2);//#map:layer3/*_obj-23:1
    }
    
    static number param_19_value_constrain(number v) {
        v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)100 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_03_crossfade_set(number v) {
        this->groove_03_crossfade = v;
        this->groove_03_crossfadeInSamples = this->mstosamps(v);
    }
    
    static number param_20_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_03_end_set(number v) {
        this->groove_03_end = v;
    }
    
    void expr_12_out1_set(number v) {
        this->expr_12_out1 = v;
        this->groove_03_end_set(this->expr_12_out1);
    }
    
    void expr_12_in1_set(number in1) {
        this->expr_12_in1 = in1;
        this->expr_12_out1_set(this->expr_12_in1 * this->expr_12_in2);//#map:layer3/*_obj-28:1
    }
    
    static number param_21_value_constrain(number v) {
        v = (v > 10 ? 10 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)10 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_03_in2_set(number v) {
        this->dspexpr_03_in2 = v;
    }
    
    void expr_12_in2_set(number v) {
        this->expr_12_in2 = v;
        this->expr_12_in1_set(this->expr_12_in1);
    }
    
    void expr_11_in2_set(number v) {
        this->expr_11_in2 = v;
        this->expr_11_in1_set(this->expr_11_in1);
    }
    
    void numberobj_05_output_set(number ) {}
    
    void numberobj_05_value_set(number v) {
        this->numberobj_05_value_setter(v);
        v = this->numberobj_05_value;
        this->sendParameter(10, false);
        this->numberobj_05_output_set(v);
    }
    
    void eventoutlet_03_in1_number_set(number v) {
        this->getPatcher()->p_03_out3_number_set(v);
    }
    
    void expr_09_out1_set(number v) {
        this->expr_09_out1 = v;
        this->expr_12_in2_set(this->expr_09_out1);
        this->expr_11_in2_set(this->expr_09_out1);
        this->numberobj_05_value_set(this->expr_09_out1);
        this->eventoutlet_03_in1_number_set(this->expr_09_out1);
    }
    
    void expr_09_in1_set(number in1) {
        this->expr_09_in1 = in1;
    
        this->expr_09_out1_set(
            (this->expr_09_in2 == 0 ? 0 : (this->expr_09_in2 == 0. ? 0. : this->expr_09_in1 / this->expr_09_in2))
        );//#map:layer3//_obj-15:1
    }
    
    void data_03_sizeout_set(number v) {
        this->data_03_sizeout = v;
        this->expr_09_in1_set(v);
    }
    
    void data_03_chanout_set(number ) {}
    
    void expr_09_in2_set(number v) {
        this->expr_09_in2 = v;
        this->expr_09_in1_set(this->expr_09_in1);
    }
    
    void expr_10_out1_set(number v) {
        this->expr_10_out1 = v;
        this->expr_09_in2_set(this->expr_10_out1);
    }
    
    void expr_10_in1_set(number in1) {
        this->expr_10_in1 = in1;
    
        this->expr_10_out1_set(
            (this->expr_10_in2 == 0 ? 0 : (this->expr_10_in2 == 0. ? 0. : this->expr_10_in1 / this->expr_10_in2))
        );//#map:layer3//_obj-16:1
    }
    
    void data_03_srout_set(number v) {
        this->expr_10_in1_set(v);
    }
    
    void data_03_info_bang() {
        this->data_03_report();
    }
    
    void groove_03_perform(
        number rate_auto,
        number begin,
        number end,
        Sample * out1,
        Sample * sync,
        Index n
    ) {
        RNBO_UNUSED(out1);
        auto __groove_03_crossfade = this->groove_03_crossfade;
        auto __groove_03_loop = this->groove_03_loop;
        auto __groove_03_playStatus = this->groove_03_playStatus;
        auto __groove_03_readIndex = this->groove_03_readIndex;
        auto __groove_03_incomingChange = this->groove_03_incomingChange;
        auto __groove_03_changeIncomingInSamples = this->groove_03_changeIncomingInSamples;
        SampleArray<1> out = {out1};
        SampleIndex bufferLength = (SampleIndex)(this->groove_03_buffer->getSize());
        Index i = 0;
    
        if (bufferLength > 1) {
            number effectiveChannels = this->minimum(this->groove_03_buffer->getChannels(), 1);
            number srMult = 0.001 * this->groove_03_buffer->getSampleRate();
            number srInv = (number)1 / this->samplerate();
            number rateMult = this->groove_03_buffer->getSampleRate() * srInv;
    
            for (; i < n; i++) {
                Index channel = 0;
                number offset = 0;
                number loopMin = begin * srMult;
                loopMin = (loopMin > bufferLength - 1 ? bufferLength - 1 : (loopMin < 0 ? 0 : loopMin));
                number loopMax = (end < 0 ? bufferLength : end * srMult);
                loopMax = (loopMax > bufferLength ? bufferLength : (loopMax < 0 ? 0 : loopMax));
    
                if (loopMin >= loopMax) {
                    offset = loopMax;
                    loopMax = bufferLength;
                    loopMin -= offset;
                }
    
                number loopLength = loopMax - loopMin;
                number currentRate = rate_auto * rateMult;
                number currentSync = 0;
    
                if (__groove_03_changeIncomingInSamples > 0) {
                    __groove_03_changeIncomingInSamples--;
    
                    if (__groove_03_changeIncomingInSamples <= 0) {
                        if (__groove_03_incomingChange == 1) {
                            if (currentRate < 0) {
                                __groove_03_readIndex = loopMax - 1;
                            } else {
                                __groove_03_readIndex = loopMin;
                            }
    
                            __groove_03_playStatus = 1;
                        } else if (__groove_03_incomingChange == 0) {
                            __groove_03_playStatus = 0;
                        }
    
                        __groove_03_incomingChange = 2;
                    }
                }
    
                if (loopLength > 0) {
                    if (currentRate != 0) {
                        if (__groove_03_playStatus == 1) {
                            if ((bool)(__groove_03_loop)) {
                                while (__groove_03_readIndex < loopMin) {
                                    __groove_03_readIndex += loopLength;
                                }
    
                                while (__groove_03_readIndex >= loopMax) {
                                    __groove_03_readIndex -= loopLength;
                                }
                            } else if (__groove_03_readIndex >= loopMax || __groove_03_readIndex < loopMin) {
                                __groove_03_playStatus = 0;
                                break;
                            }
    
                            for (; channel < effectiveChannels; channel++) {
                                number outSample = (currentRate == 1 ? this->groove_03_getSample((Index)(channel), rnbo_trunc(__groove_03_readIndex), offset, bufferLength) : this->groove_03_interpolatedSample(
                                    (Index)(channel),
                                    __groove_03_readIndex,
                                    loopMax,
                                    loopLength,
                                    offset,
                                    bufferLength
                                ));
    
                                if (__groove_03_crossfade > 0) {
                                    out[(Index)channel][(Index)i] = this->groove_03_crossfadedSample(
                                        outSample,
                                        __groove_03_readIndex,
                                        (Index)(channel),
                                        currentRate,
                                        loopMin,
                                        loopMax,
                                        loopLength,
                                        offset,
                                        bufferLength
                                    );
                                } else {
                                    out[(Index)channel][(Index)i] = outSample;
                                }
                            }
    
                            {
                                currentSync = this->groove_03_calcSync(__groove_03_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                            }
    
                            __groove_03_readIndex += currentRate;
                        }
                    } else {
                        if (__groove_03_playStatus == 1) {
                            currentSync = this->groove_03_calcSync(__groove_03_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                        }
                    }
                }
    
                {
                    sync[(Index)i] = currentSync;
                }
    
                for (; channel < 1; channel++) {
                    if (__groove_03_playStatus <= 0)
                        sync[(Index)i] = 0;
    
                    out[(Index)channel][(Index)i] = 0;
                }
            }
        }
    
        for (; i < n; i++) {
            if (__groove_03_playStatus <= 0)
                sync[(Index)i] = 0;
    
            for (number channel = 0; channel < 1; channel++) {
                out[(Index)channel][(Index)i] = 0;
            }
        }
    
        this->groove_03_changeIncomingInSamples = __groove_03_changeIncomingInSamples;
        this->groove_03_incomingChange = __groove_03_incomingChange;
        this->groove_03_readIndex = __groove_03_readIndex;
        this->groove_03_playStatus = __groove_03_playStatus;
    }
    
    void dspexpr_03_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_05_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_05_value = localvalue;
    }
    
    void numberobj_06_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_06_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_06_value = localvalue;
    }
    
    number groove_03_getSample(
        Index channel,
        SampleIndex index,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        if (offset > 0) {
            index += offset;
    
            if (index >= bufferLength)
                index -= bufferLength;
        }
    
        return this->groove_03_buffer->getSample(channel, index);
    }
    
    number groove_03_interpolatedSample(
        Index channel,
        number index,
        SampleIndex end,
        SampleIndex length,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(index));
        number i_x = index - index1;
        number i_1px = 1. + i_x;
        number i_1mx = 1. - i_x;
        number i_2mx = 2. - i_x;
        number i_a = i_1mx * i_2mx;
        number i_b = i_1px * i_x;
        number i_p1 = -.1666667 * i_a * i_x;
        number i_p2 = .5 * i_1px * i_a;
        number i_p3 = .5 * i_b * i_2mx;
        number i_p4 = -.1666667 * i_b * i_1mx;
        SampleIndex index2 = (SampleIndex)(index1 + 1);
    
        if (index2 >= end)
            index2 -= length;
    
        SampleIndex index3 = (SampleIndex)(index1 + 2);
    
        if (index3 >= end)
            index3 -= length;
    
        SampleIndex index4 = (SampleIndex)(index1 + 3);
    
        if (index4 >= end)
            index4 -= length;
    
        return this->groove_03_getSample(channel, index1, offset, bufferLength) * i_p1 + this->groove_03_getSample(channel, index2, offset, bufferLength) * i_p2 + this->groove_03_getSample(channel, index3, offset, bufferLength) * i_p3 + this->groove_03_getSample(channel, index4, offset, bufferLength) * i_p4;
    }
    
    number groove_03_crossfadedSample(
        SampleValue out,
        number readIndex,
        Index channel,
        number rate,
        number loopMin,
        number loopMax,
        number loopLength,
        number offset,
        number bufferLength
    ) {
        number crossFadeStart1 = this->maximum(loopMin - this->groove_03_crossfadeInSamples, 0);
        number crossFadeEnd1 = this->minimum(crossFadeStart1 + this->groove_03_crossfadeInSamples, bufferLength);
        number crossFadeStart2 = crossFadeStart1 + loopLength;
        number crossFadeEnd2 = this->minimum(crossFadeEnd1 + loopLength, bufferLength);
        number crossFadeLength = crossFadeEnd2 - crossFadeStart2;
    
        if (crossFadeLength > 0) {
            crossFadeEnd1 = crossFadeStart1 + crossFadeLength;
            number diff = -1;
            number addFactor = 0;
    
            if (readIndex >= crossFadeStart2) {
                diff = readIndex - crossFadeStart2;
                addFactor = -1;
            } else if (readIndex < crossFadeEnd1) {
                diff = crossFadeEnd1 - readIndex + loopMax - crossFadeStart2;
                addFactor = 1;
            }
    
            if (diff >= 0) {
                number out2ReadIndex = readIndex + loopLength * addFactor;
                number out2 = (rate == 1 ? this->groove_03_getSample(channel, rnbo_trunc(out2ReadIndex), offset, bufferLength) : this->groove_03_interpolatedSample(channel, out2ReadIndex, loopMax, loopLength, offset, bufferLength));
                number out2Factor = diff / crossFadeLength;
                number out1Factor = 1 - out2Factor;
                return out * out1Factor + out2 * out2Factor;
            }
        }
    
        return out;
    }
    
    number groove_03_calcSync(
        number readIndex,
        number offset,
        number loopMin,
        number loopLength,
        SampleIndex bufferLength,
        number srInv
    ) {
        RNBO_UNUSED(srInv);
        RNBO_UNUSED(loopLength);
        RNBO_UNUSED(loopMin);
    
        {
            if (offset > 0) {
                readIndex += offset;
    
                if (readIndex >= bufferLength)
                    readIndex -= bufferLength;
            }
    
            {
                return readIndex / bufferLength;
            }
        }
    }
    
    void groove_03_dspsetup(bool force) {
        if ((bool)(this->groove_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->groove_03_crossfadeInSamples = this->mstosamps(this->groove_03_crossfade);
        this->groove_03_setupDone = true;
    }
    
    void param_15_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_15_value;
    }
    
    void param_15_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_15_value_set(preset["value"]);
    }
    
    void param_16_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_16_value;
    }
    
    void param_16_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_16_value_set(preset["value"]);
    }
    
    void param_17_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_17_value;
    }
    
    void param_17_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_17_value_set(preset["value"]);
    }
    
    void data_03_init() {
        this->data_03_buffer->setWantsFill(true);
    }
    
    void data_03_report() {
        this->data_03_sizeout_set(this->data_03_buffer->getSize());
        this->data_03_chanout_set(this->data_03_buffer->getChannels());
        this->data_03_srout_set(this->data_03_buffer->getSampleRate());
    }
    
    Index data_03_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        number size = 0;
        return (Index)(size);
    }
    
    void data_03_dspsetup(bool force) {
        if ((bool)(this->data_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        if (this->data_03_sizemode == 2) {
            this->data_03_buffer = this->data_03_buffer->setSize((Index)(this->mstosamps(this->data_03_sizems)));
            updateDataRef(this, this->data_03_buffer);
        } else if (this->data_03_sizemode == 3) {
            this->data_03_buffer = this->data_03_buffer->setSize(this->data_03_evaluateSizeExpr(this->samplerate(), this->vectorsize()));
            updateDataRef(this, this->data_03_buffer);
        }
    
        this->data_03_setupDone = true;
    }
    
    void data_03_bufferUpdated() {
        this->data_03_report();
    }
    
    void numberobj_05_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer3/number_obj-20"), 1, this->_currentTime);
    }
    
    void numberobj_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_05_value;
    }
    
    void numberobj_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_05_value_set(preset["value"]);
    }
    
    void param_18_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_18_value;
    }
    
    void param_18_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_18_value_set(preset["value"]);
    }
    
    void numberobj_06_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer3/number_obj-5"), 1, this->_currentTime);
    }
    
    void numberobj_06_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_06_value;
    }
    
    void numberobj_06_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_06_value_set(preset["value"]);
    }
    
    void param_19_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_19_value;
    }
    
    void param_19_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_19_value_set(preset["value"]);
    }
    
    void param_20_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_20_value;
    }
    
    void param_20_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_20_value_set(preset["value"]);
    }
    
    void param_21_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_21_value;
    }
    
    void param_21_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_21_value_set(preset["value"]);
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
        dspexpr_03_in1 = 0;
        dspexpr_03_in2 = 0;
        receive_05_output_number = 0;
        groove_03_rate_auto = 1;
        groove_03_begin = 0;
        groove_03_end = -1;
        groove_03_loop = 1;
        groove_03_crossfade = 0;
        param_15_value = 1;
        receive_06_output_number = 0;
        param_16_value = 1;
        send_05_input_number = 0;
        select_03_test1 = 0;
        select_03_test2 = 1;
        send_06_input_number = 0;
        param_17_value = 1;
        expr_09_in1 = 0;
        expr_09_in2 = 0;
        expr_09_out1 = 0;
        data_03_sizeout = 0;
        data_03_size = 0;
        data_03_sizems = 0;
        data_03_channels = 1;
        numberobj_05_value = 10000;
        numberobj_05_value_setter(numberobj_05_value);
        expr_10_in1 = 0;
        expr_10_in2 = 1000;
        expr_10_out1 = 0;
        expr_11_in1 = 0;
        expr_11_in2 = 0;
        expr_11_out1 = 0;
        param_18_value = 0;
        numberobj_06_value = 0;
        numberobj_06_value_setter(numberobj_06_value);
        param_19_value = 1;
        expr_12_in1 = 0;
        expr_12_in2 = 0;
        expr_12_out1 = 0;
        param_20_value = 1;
        param_21_value = 1;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        groove_03_readIndex = 0;
        groove_03_playStatus = 0;
        groove_03_changeIncomingInSamples = 0;
        groove_03_incomingChange = 2;
        groove_03_crossfadeInSamples = 0;
        groove_03_setupDone = false;
        param_15_lastValue = 0;
        param_16_lastValue = 0;
        param_17_lastValue = 0;
        data_03_sizemode = 0;
        data_03_setupDone = false;
        numberobj_05_currentFormat = 6;
        numberobj_05_lastValue = 0;
        param_18_lastValue = 0;
        numberobj_06_currentFormat = 6;
        numberobj_06_lastValue = 0;
        param_19_lastValue = 0;
        param_20_lastValue = 0;
        param_21_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_03_in1;
        number dspexpr_03_in2;
        number receive_05_output_number;
        list receive_05_output_list;
        number groove_03_rate_auto;
        number groove_03_begin;
        number groove_03_end;
        number groove_03_loop;
        number groove_03_crossfade;
        number param_15_value;
        number receive_06_output_number;
        list receive_06_output_list;
        number param_16_value;
        number send_05_input_number;
        list send_05_input_list;
        number select_03_test1;
        number select_03_test2;
        number send_06_input_number;
        list send_06_input_list;
        number param_17_value;
        number expr_09_in1;
        number expr_09_in2;
        number expr_09_out1;
        number data_03_sizeout;
        number data_03_size;
        number data_03_sizems;
        number data_03_channels;
        number numberobj_05_value;
        number expr_10_in1;
        number expr_10_in2;
        number expr_10_out1;
        number expr_11_in1;
        number expr_11_in2;
        number expr_11_out1;
        number param_18_value;
        number numberobj_06_value;
        number param_19_value;
        number expr_12_in1;
        number expr_12_in2;
        number expr_12_out1;
        number param_20_value;
        number param_21_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[1];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef groove_03_buffer;
        number groove_03_readIndex;
        Index groove_03_playStatus;
        SampleIndex groove_03_changeIncomingInSamples;
        Int groove_03_incomingChange;
        SampleIndex groove_03_crossfadeInSamples;
        bool groove_03_setupDone;
        number param_15_lastValue;
        number param_16_lastValue;
        number param_17_lastValue;
        Float32BufferRef data_03_buffer;
        Int data_03_sizemode;
        bool data_03_setupDone;
        Int numberobj_05_currentFormat;
        number numberobj_05_lastValue;
        number param_18_lastValue;
        Int numberobj_06_currentFormat;
        number numberobj_06_lastValue;
        number param_19_lastValue;
        number param_20_lastValue;
        number param_21_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_12 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_12()
    {
    }
    
    ~RNBOSubpatcher_12()
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
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number minimum(number x, number y) {
        return (y < x ? y : x);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    Index vectorsize() {
        return this->vs;
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "rate")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "mode")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "loop")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "begin")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "crossfade")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "end")) {
            return 5;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 6;
        }
    
        if (!stringCompare(paramid, "button_obj-43/bangval")) {
            return 7;
        }
    
        if (!stringCompare(paramid, "button_obj-46/bangval")) {
            return 8;
        }
    
        if (!stringCompare(paramid, "button_obj-22/bangval")) {
            return 9;
        }
    
        if (!stringCompare(paramid, "number_obj-20/value")) {
            return 10;
        }
    
        if (!stringCompare(paramid, "number_obj-5/value")) {
            return 11;
        }
    
        return INVALID_INDEX;
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
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    
        this->groove_04_perform(
            this->groove_04_rate_auto,
            this->groove_04_begin,
            this->groove_04_end,
            this->signals[0],
            out2,
            n
        );
    
        this->dspexpr_04_perform(this->signals[0], this->dspexpr_04_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 1; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->groove_04_dspsetup(forceDSPSetup);
        this->data_04_dspsetup(forceDSPSetup);
    
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
        this->param_22_getPresetValue(getSubState(preset, "rate"));
        this->param_23_getPresetValue(getSubState(preset, "mode"));
        this->param_24_getPresetValue(getSubState(preset, "loop"));
        this->param_25_getPresetValue(getSubState(preset, "begin"));
        this->param_26_getPresetValue(getSubState(preset, "crossfade"));
        this->param_27_getPresetValue(getSubState(preset, "end"));
        this->param_28_getPresetValue(getSubState(preset, "gain"));
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
            this->param_22_value_set(v);
            break;
        case 1:
            this->param_23_value_set(v);
            break;
        case 2:
            this->param_24_value_set(v);
            break;
        case 3:
            this->param_25_value_set(v);
            break;
        case 4:
            this->param_26_value_set(v);
            break;
        case 5:
            this->param_27_value_set(v);
            break;
        case 6:
            this->param_28_value_set(v);
            break;
        case 7:
            this->button_10_bangval_bang();
            break;
        case 8:
            this->button_11_bangval_bang();
            break;
        case 9:
            this->button_12_bangval_bang();
            break;
        case 10:
            this->numberobj_07_value_set(v);
            break;
        case 11:
            this->numberobj_08_value_set(v);
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
            return this->param_22_value;
        case 1:
            return this->param_23_value;
        case 2:
            return this->param_24_value;
        case 3:
            return this->param_25_value;
        case 4:
            return this->param_26_value;
        case 5:
            return this->param_27_value;
        case 6:
            return this->param_28_value;
        case 10:
            return this->numberobj_07_value;
        case 11:
            return this->numberobj_08_value;
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
        return 12;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "rate";
        case 1:
            return "mode";
        case 2:
            return "loop";
        case 3:
            return "begin";
        case 4:
            return "crossfade";
        case 5:
            return "end";
        case 6:
            return "gain";
        case 7:
            return "button_10_bangval";
        case 8:
            return "button_11_bangval";
        case 9:
            return "button_12_bangval";
        case 10:
            return "numberobj_07_value";
        case 11:
            return "numberobj_08_value";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer4/rate";
        case 1:
            return "layer4/mode";
        case 2:
            return "layer4/loop";
        case 3:
            return "layer4/begin";
        case 4:
            return "layer4/crossfade";
        case 5:
            return "layer4/end";
        case 6:
            return "layer4/gain";
        case 7:
            return "layer4/button_obj-43/bangval";
        case 8:
            return "layer4/button_obj-46/bangval";
        case 9:
            return "layer4/button_obj-22/bangval";
        case 10:
            return "layer4/number_obj-20/value";
        case 11:
            return "layer4/number_obj-5/value";
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = -4;
                info->max = 4;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 1:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal1[] = {"Play", "Stop"};
                info->enumValues = eVal1;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 2;
                static const char * eVal2[] = {"false", "true"};
                info->enumValues = eVal2;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 100;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 6:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 10;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 7:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 8:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 9:
                info->type = ParameterTypeBang;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = false;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 10:
                info->type = ParameterTypeNumber;
                info->initialValue = 10000;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 11:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
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
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
                return normalizedValue;
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 2);
                }
    
                return normalizedValue;
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 100 ? 100 : value));
                ParameterValue normalizedValue = (value - 0) / (100 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 0:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 10:
        case 11:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 1:
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 2);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 3:
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 6:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (100 - 0);
                }
            }
        case 0:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_22_value_constrain(value);
        case 1:
            return this->param_23_value_constrain(value);
        case 2:
            return this->param_24_value_constrain(value);
        case 3:
            return this->param_25_value_constrain(value);
        case 4:
            return this->param_26_value_constrain(value);
        case 5:
            return this->param_27_value_constrain(value);
        case 6:
            return this->param_28_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
        this->updateTime(time);
    
        switch (tag) {
        case TAG("format"):
            if (TAG("layer4/number_obj-20") == objectId)
                this->numberobj_07_format_set(payload);
    
            if (TAG("layer4/number_obj-5") == objectId)
                this->numberobj_08_format_set(payload);
    
            break;
        }
    }
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("setup"):
            return "setup";
        case TAG("layer4/number_obj-20"):
            return "layer4/number_obj-20";
        case TAG("layer4/number_obj-5"):
            return "layer4/number_obj-5";
        case TAG("format"):
            return "format";
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
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 3) {
            this->groove_04_buffer = new Float32Buffer(this->getPatcher()->layer4);
            this->data_04_buffer = new Float32Buffer(this->getPatcher()->layer4);
            this->data_04_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->groove_04_buffer = new Float32Buffer(this->getPatcher()->layer4);
        this->data_04_buffer = new Float32Buffer(this->getPatcher()->layer4);
    }
    
    protected:
    
    void param_22_value_set(number v) {
        v = this->param_22_value_constrain(v);
        this->param_22_value = v;
        this->sendParameter(0, false);
    
        if (this->param_22_value != this->param_22_lastValue) {
            this->getEngine()->presetTouched();
            this->param_22_lastValue = this->param_22_value;
        }
    
        this->groove_04_rate_auto_set(v);
    }
    
    void param_23_value_set(number v) {
        v = this->param_23_value_constrain(v);
        this->param_23_value = v;
        this->sendParameter(1, false);
    
        if (this->param_23_value != this->param_23_lastValue) {
            this->getEngine()->presetTouched();
            this->param_23_lastValue = this->param_23_value;
        }
    
        this->select_04_input_number_set(v);
    }
    
    void param_24_value_set(number v) {
        v = this->param_24_value_constrain(v);
        this->param_24_value = v;
        this->sendParameter(2, false);
    
        if (this->param_24_value != this->param_24_lastValue) {
            this->getEngine()->presetTouched();
            this->param_24_lastValue = this->param_24_value;
        }
    
        this->groove_04_loop_set(v);
    }
    
    void param_25_value_set(number v) {
        v = this->param_25_value_constrain(v);
        this->param_25_value = v;
        this->sendParameter(3, false);
    
        if (this->param_25_value != this->param_25_lastValue) {
            this->getEngine()->presetTouched();
            this->param_25_lastValue = this->param_25_value;
        }
    
        this->expr_15_in1_set(v);
    }
    
    void param_26_value_set(number v) {
        v = this->param_26_value_constrain(v);
        this->param_26_value = v;
        this->sendParameter(4, false);
    
        if (this->param_26_value != this->param_26_lastValue) {
            this->getEngine()->presetTouched();
            this->param_26_lastValue = this->param_26_value;
        }
    
        this->groove_04_crossfade_set(v);
    }
    
    void param_27_value_set(number v) {
        v = this->param_27_value_constrain(v);
        this->param_27_value = v;
        this->sendParameter(5, false);
    
        if (this->param_27_value != this->param_27_lastValue) {
            this->getEngine()->presetTouched();
            this->param_27_lastValue = this->param_27_value;
        }
    
        this->expr_16_in1_set(v);
    }
    
    void param_28_value_set(number v) {
        v = this->param_28_value_constrain(v);
        this->param_28_value = v;
        this->sendParameter(6, false);
    
        if (this->param_28_value != this->param_28_lastValue) {
            this->getEngine()->presetTouched();
            this->param_28_lastValue = this->param_28_value;
        }
    
        this->dspexpr_04_in2_set(v);
    }
    
    void button_12_bangval_bang() {
        this->sendParameter(9, true);
        this->data_04_info_bang();
    }
    
    void numberobj_07_format_set(number v) {
        if (v == 0) {
            this->numberobj_07_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_07_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_07_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_07_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_07_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_07_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_07_currentFormat = 6;
        }
    }
    
    void numberobj_08_format_set(number v) {
        if (v == 0) {
            this->numberobj_08_currentFormat = 0;
        } else if (v == 1) {
            this->numberobj_08_currentFormat = 1;
        } else if (v == 2) {
            this->numberobj_08_currentFormat = 2;
        } else if (v == 3) {
            this->numberobj_08_currentFormat = 3;
        } else if (v == 4) {
            this->numberobj_08_currentFormat = 4;
        } else if (v == 5) {
            this->numberobj_08_currentFormat = 5;
        } else if (v == 6) {
            this->numberobj_08_currentFormat = 6;
        }
    }
    
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
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->data_04_init();
        this->numberobj_07_init();
        this->numberobj_08_init();
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
    
        {
            this->scheduleParamInit(2, 0);
        }
    
        {
            this->scheduleParamInit(3, 0);
        }
    
        {
            this->scheduleParamInit(4, 0);
        }
    
        {
            this->scheduleParamInit(5, 0);
        }
    
        {
            this->scheduleParamInit(6, 0);
        }
    }
    
    void allocateDataRefs() {
        this->groove_04_buffer = this->groove_04_buffer->allocateIfNeeded();
        this->data_04_buffer = this->data_04_buffer->allocateIfNeeded();
    }
    
    static number param_22_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void groove_04_rate_auto_set(number v) {
        this->groove_04_rate_auto = v;
    }
    
    static number param_23_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void trigger_07_out2_set(number ) {}
    
    void groove_04_rate_bang_bang() {
        this->groove_04_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_04_incomingChange = 1;
    }
    
    void button_10_bangval_bang() {
        this->sendParameter(7, true);
        this->groove_04_rate_bang_bang();
    }
    
    void receive_07_output_bang_bang() {
        this->button_10_bangval_bang();
    }
    
    void send_07_input_bang_bang() {
        this->receive_07_output_bang_bang();
        this->getPatcher()->p_04_source_Playu45layer4_bang_bang();
    }
    
    void trigger_07_out1_bang() {
        this->send_07_input_bang_bang();
    }
    
    void trigger_07_input_bang_bang() {
        this->trigger_07_out2_set(0);
        this->trigger_07_out1_bang();
    }
    
    void select_04_match1_bang() {
        this->trigger_07_input_bang_bang();
    }
    
    void groove_04_stop_bang() {
        this->groove_04_changeIncomingInSamples = this->sampleOffsetIntoNextAudioBuffer + 1;
        this->groove_04_incomingChange = 0;
    }
    
    void button_11_bangval_bang() {
        this->sendParameter(8, true);
        this->groove_04_stop_bang();
    }
    
    void receive_08_output_bang_bang() {
        this->button_11_bangval_bang();
    }
    
    void send_08_input_bang_bang() {
        this->receive_08_output_bang_bang();
        this->getPatcher()->p_04_source_Stopu45layer4_bang_bang();
    }
    
    void trigger_08_out1_bang() {
        this->send_08_input_bang_bang();
    }
    
    void trigger_08_input_bang_bang() {
        this->trigger_08_out1_bang();
    }
    
    void select_04_match2_bang() {
        this->trigger_08_input_bang_bang();
    }
    
    void select_04_nomatch_number_set(number ) {}
    
    void select_04_input_number_set(number v) {
        if (v == this->select_04_test1)
            this->select_04_match1_bang();
        else if (v == this->select_04_test2)
            this->select_04_match2_bang();
        else
            this->select_04_nomatch_number_set(v);
    }
    
    static number param_24_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)1;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_04_loop_set(number v) {
        this->groove_04_loop = v;
    }
    
    static number param_25_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void numberobj_08_output_set(number ) {}
    
    void numberobj_08_value_set(number v) {
        this->numberobj_08_value_setter(v);
        v = this->numberobj_08_value;
        this->sendParameter(11, false);
        this->numberobj_08_output_set(v);
    }
    
    void groove_04_begin_set(number v) {
        this->groove_04_begin = v;
    }
    
    void expr_15_out1_set(number v) {
        this->expr_15_out1 = v;
        this->numberobj_08_value_set(this->expr_15_out1);
        this->groove_04_begin_set(this->expr_15_out1);
    }
    
    void expr_15_in1_set(number in1) {
        this->expr_15_in1 = in1;
        this->expr_15_out1_set(this->expr_15_in1 * this->expr_15_in2);//#map:layer4/*_obj-23:1
    }
    
    static number param_26_value_constrain(number v) {
        v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)100 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_04_crossfade_set(number v) {
        this->groove_04_crossfade = v;
        this->groove_04_crossfadeInSamples = this->mstosamps(v);
    }
    
    static number param_27_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void groove_04_end_set(number v) {
        this->groove_04_end = v;
    }
    
    void expr_16_out1_set(number v) {
        this->expr_16_out1 = v;
        this->groove_04_end_set(this->expr_16_out1);
    }
    
    void expr_16_in1_set(number in1) {
        this->expr_16_in1 = in1;
        this->expr_16_out1_set(this->expr_16_in1 * this->expr_16_in2);//#map:layer4/*_obj-28:1
    }
    
    static number param_28_value_constrain(number v) {
        v = (v > 10 ? 10 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)10 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_04_in2_set(number v) {
        this->dspexpr_04_in2 = v;
    }
    
    void expr_16_in2_set(number v) {
        this->expr_16_in2 = v;
        this->expr_16_in1_set(this->expr_16_in1);
    }
    
    void expr_15_in2_set(number v) {
        this->expr_15_in2 = v;
        this->expr_15_in1_set(this->expr_15_in1);
    }
    
    void numberobj_07_output_set(number ) {}
    
    void numberobj_07_value_set(number v) {
        this->numberobj_07_value_setter(v);
        v = this->numberobj_07_value;
        this->sendParameter(10, false);
        this->numberobj_07_output_set(v);
    }
    
    void eventoutlet_04_in1_number_set(number v) {
        this->getPatcher()->p_04_out3_number_set(v);
    }
    
    void expr_13_out1_set(number v) {
        this->expr_13_out1 = v;
        this->expr_16_in2_set(this->expr_13_out1);
        this->expr_15_in2_set(this->expr_13_out1);
        this->numberobj_07_value_set(this->expr_13_out1);
        this->eventoutlet_04_in1_number_set(this->expr_13_out1);
    }
    
    void expr_13_in1_set(number in1) {
        this->expr_13_in1 = in1;
    
        this->expr_13_out1_set(
            (this->expr_13_in2 == 0 ? 0 : (this->expr_13_in2 == 0. ? 0. : this->expr_13_in1 / this->expr_13_in2))
        );//#map:layer4//_obj-15:1
    }
    
    void data_04_sizeout_set(number v) {
        this->data_04_sizeout = v;
        this->expr_13_in1_set(v);
    }
    
    void data_04_chanout_set(number ) {}
    
    void expr_13_in2_set(number v) {
        this->expr_13_in2 = v;
        this->expr_13_in1_set(this->expr_13_in1);
    }
    
    void expr_14_out1_set(number v) {
        this->expr_14_out1 = v;
        this->expr_13_in2_set(this->expr_14_out1);
    }
    
    void expr_14_in1_set(number in1) {
        this->expr_14_in1 = in1;
    
        this->expr_14_out1_set(
            (this->expr_14_in2 == 0 ? 0 : (this->expr_14_in2 == 0. ? 0. : this->expr_14_in1 / this->expr_14_in2))
        );//#map:layer4//_obj-16:1
    }
    
    void data_04_srout_set(number v) {
        this->expr_14_in1_set(v);
    }
    
    void data_04_info_bang() {
        this->data_04_report();
    }
    
    void groove_04_perform(
        number rate_auto,
        number begin,
        number end,
        Sample * out1,
        Sample * sync,
        Index n
    ) {
        RNBO_UNUSED(out1);
        auto __groove_04_crossfade = this->groove_04_crossfade;
        auto __groove_04_loop = this->groove_04_loop;
        auto __groove_04_playStatus = this->groove_04_playStatus;
        auto __groove_04_readIndex = this->groove_04_readIndex;
        auto __groove_04_incomingChange = this->groove_04_incomingChange;
        auto __groove_04_changeIncomingInSamples = this->groove_04_changeIncomingInSamples;
        SampleArray<1> out = {out1};
        SampleIndex bufferLength = (SampleIndex)(this->groove_04_buffer->getSize());
        Index i = 0;
    
        if (bufferLength > 1) {
            number effectiveChannels = this->minimum(this->groove_04_buffer->getChannels(), 1);
            number srMult = 0.001 * this->groove_04_buffer->getSampleRate();
            number srInv = (number)1 / this->samplerate();
            number rateMult = this->groove_04_buffer->getSampleRate() * srInv;
    
            for (; i < n; i++) {
                Index channel = 0;
                number offset = 0;
                number loopMin = begin * srMult;
                loopMin = (loopMin > bufferLength - 1 ? bufferLength - 1 : (loopMin < 0 ? 0 : loopMin));
                number loopMax = (end < 0 ? bufferLength : end * srMult);
                loopMax = (loopMax > bufferLength ? bufferLength : (loopMax < 0 ? 0 : loopMax));
    
                if (loopMin >= loopMax) {
                    offset = loopMax;
                    loopMax = bufferLength;
                    loopMin -= offset;
                }
    
                number loopLength = loopMax - loopMin;
                number currentRate = rate_auto * rateMult;
                number currentSync = 0;
    
                if (__groove_04_changeIncomingInSamples > 0) {
                    __groove_04_changeIncomingInSamples--;
    
                    if (__groove_04_changeIncomingInSamples <= 0) {
                        if (__groove_04_incomingChange == 1) {
                            if (currentRate < 0) {
                                __groove_04_readIndex = loopMax - 1;
                            } else {
                                __groove_04_readIndex = loopMin;
                            }
    
                            __groove_04_playStatus = 1;
                        } else if (__groove_04_incomingChange == 0) {
                            __groove_04_playStatus = 0;
                        }
    
                        __groove_04_incomingChange = 2;
                    }
                }
    
                if (loopLength > 0) {
                    if (currentRate != 0) {
                        if (__groove_04_playStatus == 1) {
                            if ((bool)(__groove_04_loop)) {
                                while (__groove_04_readIndex < loopMin) {
                                    __groove_04_readIndex += loopLength;
                                }
    
                                while (__groove_04_readIndex >= loopMax) {
                                    __groove_04_readIndex -= loopLength;
                                }
                            } else if (__groove_04_readIndex >= loopMax || __groove_04_readIndex < loopMin) {
                                __groove_04_playStatus = 0;
                                break;
                            }
    
                            for (; channel < effectiveChannels; channel++) {
                                number outSample = (currentRate == 1 ? this->groove_04_getSample((Index)(channel), rnbo_trunc(__groove_04_readIndex), offset, bufferLength) : this->groove_04_interpolatedSample(
                                    (Index)(channel),
                                    __groove_04_readIndex,
                                    loopMax,
                                    loopLength,
                                    offset,
                                    bufferLength
                                ));
    
                                if (__groove_04_crossfade > 0) {
                                    out[(Index)channel][(Index)i] = this->groove_04_crossfadedSample(
                                        outSample,
                                        __groove_04_readIndex,
                                        (Index)(channel),
                                        currentRate,
                                        loopMin,
                                        loopMax,
                                        loopLength,
                                        offset,
                                        bufferLength
                                    );
                                } else {
                                    out[(Index)channel][(Index)i] = outSample;
                                }
                            }
    
                            {
                                currentSync = this->groove_04_calcSync(__groove_04_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                            }
    
                            __groove_04_readIndex += currentRate;
                        }
                    } else {
                        if (__groove_04_playStatus == 1) {
                            currentSync = this->groove_04_calcSync(__groove_04_readIndex, offset, loopMin, loopLength, bufferLength, srInv);
                        }
                    }
                }
    
                {
                    sync[(Index)i] = currentSync;
                }
    
                for (; channel < 1; channel++) {
                    if (__groove_04_playStatus <= 0)
                        sync[(Index)i] = 0;
    
                    out[(Index)channel][(Index)i] = 0;
                }
            }
        }
    
        for (; i < n; i++) {
            if (__groove_04_playStatus <= 0)
                sync[(Index)i] = 0;
    
            for (number channel = 0; channel < 1; channel++) {
                out[(Index)channel][(Index)i] = 0;
            }
        }
    
        this->groove_04_changeIncomingInSamples = __groove_04_changeIncomingInSamples;
        this->groove_04_incomingChange = __groove_04_incomingChange;
        this->groove_04_readIndex = __groove_04_readIndex;
        this->groove_04_playStatus = __groove_04_playStatus;
    }
    
    void dspexpr_04_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_07_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_07_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_07_value = localvalue;
    }
    
    void numberobj_08_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_08_currentFormat != 6) {
            localvalue = rnbo_trunc(localvalue);
        }
    
        this->numberobj_08_value = localvalue;
    }
    
    number groove_04_getSample(
        Index channel,
        SampleIndex index,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        if (offset > 0) {
            index += offset;
    
            if (index >= bufferLength)
                index -= bufferLength;
        }
    
        return this->groove_04_buffer->getSample(channel, index);
    }
    
    number groove_04_interpolatedSample(
        Index channel,
        number index,
        SampleIndex end,
        SampleIndex length,
        SampleIndex offset,
        SampleIndex bufferLength
    ) {
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(index));
        number i_x = index - index1;
        number i_1px = 1. + i_x;
        number i_1mx = 1. - i_x;
        number i_2mx = 2. - i_x;
        number i_a = i_1mx * i_2mx;
        number i_b = i_1px * i_x;
        number i_p1 = -.1666667 * i_a * i_x;
        number i_p2 = .5 * i_1px * i_a;
        number i_p3 = .5 * i_b * i_2mx;
        number i_p4 = -.1666667 * i_b * i_1mx;
        SampleIndex index2 = (SampleIndex)(index1 + 1);
    
        if (index2 >= end)
            index2 -= length;
    
        SampleIndex index3 = (SampleIndex)(index1 + 2);
    
        if (index3 >= end)
            index3 -= length;
    
        SampleIndex index4 = (SampleIndex)(index1 + 3);
    
        if (index4 >= end)
            index4 -= length;
    
        return this->groove_04_getSample(channel, index1, offset, bufferLength) * i_p1 + this->groove_04_getSample(channel, index2, offset, bufferLength) * i_p2 + this->groove_04_getSample(channel, index3, offset, bufferLength) * i_p3 + this->groove_04_getSample(channel, index4, offset, bufferLength) * i_p4;
    }
    
    number groove_04_crossfadedSample(
        SampleValue out,
        number readIndex,
        Index channel,
        number rate,
        number loopMin,
        number loopMax,
        number loopLength,
        number offset,
        number bufferLength
    ) {
        number crossFadeStart1 = this->maximum(loopMin - this->groove_04_crossfadeInSamples, 0);
        number crossFadeEnd1 = this->minimum(crossFadeStart1 + this->groove_04_crossfadeInSamples, bufferLength);
        number crossFadeStart2 = crossFadeStart1 + loopLength;
        number crossFadeEnd2 = this->minimum(crossFadeEnd1 + loopLength, bufferLength);
        number crossFadeLength = crossFadeEnd2 - crossFadeStart2;
    
        if (crossFadeLength > 0) {
            crossFadeEnd1 = crossFadeStart1 + crossFadeLength;
            number diff = -1;
            number addFactor = 0;
    
            if (readIndex >= crossFadeStart2) {
                diff = readIndex - crossFadeStart2;
                addFactor = -1;
            } else if (readIndex < crossFadeEnd1) {
                diff = crossFadeEnd1 - readIndex + loopMax - crossFadeStart2;
                addFactor = 1;
            }
    
            if (diff >= 0) {
                number out2ReadIndex = readIndex + loopLength * addFactor;
                number out2 = (rate == 1 ? this->groove_04_getSample(channel, rnbo_trunc(out2ReadIndex), offset, bufferLength) : this->groove_04_interpolatedSample(channel, out2ReadIndex, loopMax, loopLength, offset, bufferLength));
                number out2Factor = diff / crossFadeLength;
                number out1Factor = 1 - out2Factor;
                return out * out1Factor + out2 * out2Factor;
            }
        }
    
        return out;
    }
    
    number groove_04_calcSync(
        number readIndex,
        number offset,
        number loopMin,
        number loopLength,
        SampleIndex bufferLength,
        number srInv
    ) {
        RNBO_UNUSED(srInv);
        RNBO_UNUSED(loopLength);
        RNBO_UNUSED(loopMin);
    
        {
            if (offset > 0) {
                readIndex += offset;
    
                if (readIndex >= bufferLength)
                    readIndex -= bufferLength;
            }
    
            {
                return readIndex / bufferLength;
            }
        }
    }
    
    void groove_04_dspsetup(bool force) {
        if ((bool)(this->groove_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->groove_04_crossfadeInSamples = this->mstosamps(this->groove_04_crossfade);
        this->groove_04_setupDone = true;
    }
    
    void param_22_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_22_value;
    }
    
    void param_22_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_22_value_set(preset["value"]);
    }
    
    void param_23_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_23_value;
    }
    
    void param_23_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_23_value_set(preset["value"]);
    }
    
    void param_24_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_24_value;
    }
    
    void param_24_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_24_value_set(preset["value"]);
    }
    
    void data_04_init() {
        this->data_04_buffer->setWantsFill(true);
    }
    
    void data_04_report() {
        this->data_04_sizeout_set(this->data_04_buffer->getSize());
        this->data_04_chanout_set(this->data_04_buffer->getChannels());
        this->data_04_srout_set(this->data_04_buffer->getSampleRate());
    }
    
    Index data_04_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        number size = 0;
        return (Index)(size);
    }
    
    void data_04_dspsetup(bool force) {
        if ((bool)(this->data_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        if (this->data_04_sizemode == 2) {
            this->data_04_buffer = this->data_04_buffer->setSize((Index)(this->mstosamps(this->data_04_sizems)));
            updateDataRef(this, this->data_04_buffer);
        } else if (this->data_04_sizemode == 3) {
            this->data_04_buffer = this->data_04_buffer->setSize(this->data_04_evaluateSizeExpr(this->samplerate(), this->vectorsize()));
            updateDataRef(this, this->data_04_buffer);
        }
    
        this->data_04_setupDone = true;
    }
    
    void data_04_bufferUpdated() {
        this->data_04_report();
    }
    
    void numberobj_07_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer4/number_obj-20"), 1, this->_currentTime);
    }
    
    void numberobj_07_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_07_value;
    }
    
    void numberobj_07_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_07_value_set(preset["value"]);
    }
    
    void param_25_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_25_value;
    }
    
    void param_25_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_25_value_set(preset["value"]);
    }
    
    void numberobj_08_init() {
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("layer4/number_obj-5"), 1, this->_currentTime);
    }
    
    void numberobj_08_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_08_value;
    }
    
    void numberobj_08_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_08_value_set(preset["value"]);
    }
    
    void param_26_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_26_value;
    }
    
    void param_26_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_26_value_set(preset["value"]);
    }
    
    void param_27_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_27_value;
    }
    
    void param_27_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_27_value_set(preset["value"]);
    }
    
    void param_28_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_28_value;
    }
    
    void param_28_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_28_value_set(preset["value"]);
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
        dspexpr_04_in1 = 0;
        dspexpr_04_in2 = 0;
        receive_07_output_number = 0;
        groove_04_rate_auto = 1;
        groove_04_begin = 0;
        groove_04_end = -1;
        groove_04_loop = 1;
        groove_04_crossfade = 0;
        param_22_value = 1;
        receive_08_output_number = 0;
        param_23_value = 1;
        send_07_input_number = 0;
        select_04_test1 = 0;
        select_04_test2 = 1;
        send_08_input_number = 0;
        param_24_value = 1;
        expr_13_in1 = 0;
        expr_13_in2 = 0;
        expr_13_out1 = 0;
        data_04_sizeout = 0;
        data_04_size = 0;
        data_04_sizems = 0;
        data_04_channels = 1;
        numberobj_07_value = 10000;
        numberobj_07_value_setter(numberobj_07_value);
        expr_14_in1 = 0;
        expr_14_in2 = 1000;
        expr_14_out1 = 0;
        expr_15_in1 = 0;
        expr_15_in2 = 0;
        expr_15_out1 = 0;
        param_25_value = 0;
        numberobj_08_value = 0;
        numberobj_08_value_setter(numberobj_08_value);
        param_26_value = 1;
        expr_16_in1 = 0;
        expr_16_in2 = 0;
        expr_16_out1 = 0;
        param_27_value = 1;
        param_28_value = 1;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        groove_04_readIndex = 0;
        groove_04_playStatus = 0;
        groove_04_changeIncomingInSamples = 0;
        groove_04_incomingChange = 2;
        groove_04_crossfadeInSamples = 0;
        groove_04_setupDone = false;
        param_22_lastValue = 0;
        param_23_lastValue = 0;
        param_24_lastValue = 0;
        data_04_sizemode = 0;
        data_04_setupDone = false;
        numberobj_07_currentFormat = 6;
        numberobj_07_lastValue = 0;
        param_25_lastValue = 0;
        numberobj_08_currentFormat = 6;
        numberobj_08_lastValue = 0;
        param_26_lastValue = 0;
        param_27_lastValue = 0;
        param_28_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_04_in1;
        number dspexpr_04_in2;
        number receive_07_output_number;
        list receive_07_output_list;
        number groove_04_rate_auto;
        number groove_04_begin;
        number groove_04_end;
        number groove_04_loop;
        number groove_04_crossfade;
        number param_22_value;
        number receive_08_output_number;
        list receive_08_output_list;
        number param_23_value;
        number send_07_input_number;
        list send_07_input_list;
        number select_04_test1;
        number select_04_test2;
        number send_08_input_number;
        list send_08_input_list;
        number param_24_value;
        number expr_13_in1;
        number expr_13_in2;
        number expr_13_out1;
        number data_04_sizeout;
        number data_04_size;
        number data_04_sizems;
        number data_04_channels;
        number numberobj_07_value;
        number expr_14_in1;
        number expr_14_in2;
        number expr_14_out1;
        number expr_15_in1;
        number expr_15_in2;
        number expr_15_out1;
        number param_25_value;
        number numberobj_08_value;
        number param_26_value;
        number expr_16_in1;
        number expr_16_in2;
        number expr_16_out1;
        number param_27_value;
        number param_28_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[1];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef groove_04_buffer;
        number groove_04_readIndex;
        Index groove_04_playStatus;
        SampleIndex groove_04_changeIncomingInSamples;
        Int groove_04_incomingChange;
        SampleIndex groove_04_crossfadeInSamples;
        bool groove_04_setupDone;
        number param_22_lastValue;
        number param_23_lastValue;
        number param_24_lastValue;
        Float32BufferRef data_04_buffer;
        Int data_04_sizemode;
        bool data_04_setupDone;
        Int numberobj_07_currentFormat;
        number numberobj_07_lastValue;
        number param_25_lastValue;
        Int numberobj_08_currentFormat;
        number numberobj_08_lastValue;
        number param_26_lastValue;
        number param_27_lastValue;
        number param_28_lastValue;
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

number samplerate() {
    return this->sr;
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

ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
    if (!stringCompare(paramid, "rec_mon")) {
        return 0;
    }

    if (!stringCompare(paramid, "layer")) {
        return 1;
    }

    if (!stringCompare(paramid, "record-mode")) {
        return 2;
    }

    if (!stringCompare(paramid, "rec-gain")) {
        return 3;
    }

    if (!stringCompare(paramid, "toggle_obj-40/value")) {
        return 4;
    }

    return INVALID_INDEX;
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
    SampleValue * out100 = (numOutputs >= 10 && outputs[9] ? outputs[9] : this->dummyBuffer);
    SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    this->dspexpr_10_perform(in1, this->dspexpr_10_in2, this->signals[0], n);

    this->recordtilde_01_perform(
        this->recordtilde_01_record,
        this->recordtilde_01_begin,
        this->recordtilde_01_end,
        this->signals[0],
        out6,
        n
    );

    this->dspexpr_05_perform(in1, this->dspexpr_05_in2, this->signals[0], n);
    this->p_01_perform(out2, out7, n);
    this->dspexpr_09_perform(this->signals[0], out2, this->signals[1], n);
    this->p_02_perform(out3, out8, n);
    this->dspexpr_07_perform(this->signals[1], out3, this->signals[0], n);
    this->p_03_perform(out4, out9, n);
    this->dspexpr_08_perform(this->signals[0], out4, this->signals[1], n);
    this->p_04_perform(out5, out100, n);
    this->dspexpr_06_perform(this->signals[1], out5, out1, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 2; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

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

    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_02->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_03->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_04->prepareToProcess(sampleRate, maxBlockSize, force);

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
        return addressOf(this->layer1);
        break;
    case 1:
        return addressOf(this->layer2);
        break;
    case 2:
        return addressOf(this->layer3);
        break;
    case 3:
        return addressOf(this->layer4);
        break;
    default:
        return nullptr;
    }
}

DataRefIndex getNumDataRefs() const {
    return 4;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void zeroDataRef(DataRef& ref) {
    ref->setZero();
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->updateTime(time);

    if (index == -1 || index == 0 || index == 1 || index == 2 || index == 3) {
        this->recordtilde_01_buffer = new Float32MultiBuffer(this->recordtilde_01_bufferobj);
    }

    this->p_01->processDataViewUpdate(index, time);
    this->p_02->processDataViewUpdate(index, time);
    this->p_03->processDataViewUpdate(index, time);
    this->p_04->processDataViewUpdate(index, time);
}

void initialize() {
    this->recordtilde_01_bufferobj = initMultiRef(this->layer1, this->layer2, this->layer3, this->layer4);
    this->layer1 = initDataRef("layer1", false, nullptr);
    this->layer2 = initDataRef("layer2", false, nullptr);
    this->layer3 = initDataRef("layer3", false, nullptr);
    this->layer4 = initDataRef("layer4", false, nullptr);
    this->assign_defaults();
    this->setState();
    this->recordtilde_01_bufferobj->setIndex(-1);
    this->recordtilde_01_buffer = new Float32MultiBuffer(this->recordtilde_01_bufferobj);
    this->layer1->setIndex(0);
    this->layer2->setIndex(1);
    this->layer3->setIndex(2);
    this->layer4->setIndex(3);
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
    this->p_01 = new RNBOSubpatcher_09();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_10();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));
    this->p_03 = new RNBOSubpatcher_11();
    this->p_03->setEngineAndPatcher(this->getEngine(), this);
    this->p_03->initialize();
    this->p_03->setParameterOffset(this->getParameterOffset(this->p_03));
    this->p_04 = new RNBOSubpatcher_12();
    this->p_04->setEngineAndPatcher(this->getEngine(), this);
    this->p_04->initialize();
    this->p_04->setParameterOffset(this->getParameterOffset(this->p_04));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_29_getPresetValue(getSubState(preset, "rec_mon"));
    this->param_30_getPresetValue(getSubState(preset, "layer"));
    this->param_31_getPresetValue(getSubState(preset, "record-mode"));
    this->param_32_getPresetValue(getSubState(preset, "rec-gain"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "layer1"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "layer2"));
    this->p_03->getPreset(getSubState(getSubState(preset, "__sps"), "layer3"));
    this->p_04->getPreset(getSubState(getSubState(preset, "__sps"), "layer4"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_29_setPresetValue(getSubState(preset, "rec_mon"));
    this->param_30_setPresetValue(getSubState(preset, "layer"));
    this->param_31_setPresetValue(getSubState(preset, "record-mode"));
    this->param_32_setPresetValue(getSubState(preset, "rec-gain"));
    this->p_01->param_01_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "rate"));
    this->p_01->param_02_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "mode"));
    this->p_01->param_03_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "loop"));
    this->p_01->param_04_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "begin"));

    this->p_01->param_05_setPresetValue(
        getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "crossfade")
    );

    this->p_01->param_06_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "end"));
    this->p_01->param_07_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "gain"));
    this->p_02->param_08_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "rate"));
    this->p_02->param_09_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "mode"));
    this->p_02->param_10_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "loop"));
    this->p_02->param_11_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "begin"));

    this->p_02->param_12_setPresetValue(
        getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "crossfade")
    );

    this->p_02->param_13_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "end"));
    this->p_02->param_14_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "gain"));
    this->p_03->param_15_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "rate"));
    this->p_03->param_16_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "mode"));
    this->p_03->param_17_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "loop"));
    this->p_03->param_18_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "begin"));

    this->p_03->param_19_setPresetValue(
        getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "crossfade")
    );

    this->p_03->param_20_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "end"));
    this->p_03->param_21_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "gain"));
    this->p_04->param_22_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "rate"));
    this->p_04->param_23_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "mode"));
    this->p_04->param_24_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "loop"));
    this->p_04->param_25_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "begin"));

    this->p_04->param_26_setPresetValue(
        getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "crossfade")
    );

    this->p_04->param_27_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "end"));
    this->p_04->param_28_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "gain"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(tempo, false)) {
        this->p_01->processTempoEvent(time, tempo);
        this->p_02->processTempoEvent(time, tempo);
        this->p_03->processTempoEvent(time, tempo);
        this->p_04->processTempoEvent(time, tempo);
    }
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(state, false)) {
        this->p_01->processTransportEvent(time, state);
        this->p_02->processTransportEvent(time, state);
        this->p_03->processTransportEvent(time, state);
        this->p_04->processTransportEvent(time, state);
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(beattime, false)) {
        this->p_01->processBeatTimeEvent(time, beattime);
        this->p_02->processBeatTimeEvent(time, beattime);
        this->p_03->processBeatTimeEvent(time, beattime);
        this->p_04->processBeatTimeEvent(time, beattime);
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
    }
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        this->param_29_value_set(v);
        break;
    case 1:
        this->param_30_value_set(v);
        break;
    case 2:
        this->param_31_value_set(v);
        break;
    case 3:
        this->param_32_value_set(v);
        break;
    case 4:
        this->toggle_01_value_set(v);
        break;
    default:
        index -= 5;

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
        return this->param_29_value;
    case 1:
        return this->param_30_value;
    case 2:
        return this->param_31_value;
    case 3:
        return this->param_32_value;
    case 4:
        return this->toggle_01_value;
    default:
        index -= 5;

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
    return 5 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "rec_mon";
    case 1:
        return "layer";
    case 2:
        return "record-mode";
    case 3:
        return "rec-gain";
    case 4:
        return "toggle_01_value";
    default:
        index -= 5;

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

        return "bogus";
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "rec_mon";
    case 1:
        return "layer";
    case 2:
        return "record-mode";
    case 3:
        return "rec-gain";
    case 4:
        return "toggle_obj-40/value";
    default:
        index -= 5;

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

        return "bogus";
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            info->type = ParameterTypeNumber;
            info->initialValue = 100;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 101;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 1:
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 3;
            info->exponent = 1;
            info->steps = 4;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 2:
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 2;
            static const char * eVal2[] = {"Stop", "Record"};
            info->enumValues = eVal2;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 3:
            info->type = ParameterTypeNumber;
            info->initialValue = 1;
            info->min = 0;
            info->max = 10;
            info->exponent = 1;
            info->steps = 101;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 4:
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = false;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        default:
            index -= 5;

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

            break;
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->p_01)
        return 5;

    if (subpatcher == this->p_02)
        return 5 + this->p_01->getNumParameters();

    if (subpatcher == this->p_03)
        return 5 + this->p_01->getNumParameters() + this->p_02->getNumParameters();

    if (subpatcher == this->p_04)
        return 5 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters();

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
    case 4:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
    case 2:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 2);
            }

            return normalizedValue;
        }
    case 1:
        {
            value = (value < 0 ? 0 : (value > 3 ? 3 : value));
            ParameterValue normalizedValue = (value - 0) / (3 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 4);
            }

            return normalizedValue;
        }
    case 3:
        {
            value = (value < 0 ? 0 : (value > 10 ? 10 : value));
            ParameterValue normalizedValue = (value - 0) / (10 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
            }

            return normalizedValue;
        }
    case 0:
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
            }

            return normalizedValue;
        }
    default:
        index -= 5;

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

        return value;
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 4:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (1 - 0);
            }
        }
    case 2:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                value = this->applyStepsToNormalizedParameterValue(value, 2);
            }

            {
                return 0 + value * (1 - 0);
            }
        }
    case 1:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                value = this->applyStepsToNormalizedParameterValue(value, 4);
            }

            {
                return 0 + value * (3 - 0);
            }
        }
    case 3:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                value = this->applyStepsToNormalizedParameterValue(value, 101);
            }

            {
                return 0 + value * (10 - 0);
            }
        }
    case 0:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                value = this->applyStepsToNormalizedParameterValue(value, 101);
            }

            {
                return 0 + value * (100 - 0);
            }
        }
    default:
        index -= 5;

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

        return value;
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        return this->param_29_value_constrain(value);
    case 1:
        return this->param_30_value_constrain(value);
    case 2:
        return this->param_31_value_constrain(value);
    case 3:
        return this->param_32_value_constrain(value);
    default:
        index -= 5;

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
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processBangMessage(tag, objectId, time);
    this->p_02->processBangMessage(tag, objectId, time);
    this->p_03->processBangMessage(tag, objectId, time);
    this->p_04->processBangMessage(tag, objectId, time);
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

void param_29_value_set(number v) {
    v = this->param_29_value_constrain(v);
    this->param_29_value = v;
    this->sendParameter(0, false);

    if (this->param_29_value != this->param_29_lastValue) {
        this->getEngine()->presetTouched();
        this->param_29_lastValue = this->param_29_value;
    }

    this->expr_18_in1_set(v);
}

void param_30_value_set(number v) {
    v = this->param_30_value_constrain(v);
    this->param_30_value = v;
    this->sendParameter(1, false);

    if (this->param_30_value != this->param_30_lastValue) {
        this->getEngine()->presetTouched();
        this->param_30_lastValue = this->param_30_value;
    }

    this->recordtilde_01_buffer_set(v);
}

void param_31_value_set(number v) {
    v = this->param_31_value_constrain(v);
    this->param_31_value = v;
    this->sendParameter(2, false);

    if (this->param_31_value != this->param_31_lastValue) {
        this->getEngine()->presetTouched();
        this->param_31_lastValue = this->param_31_value;
    }

    this->send_09_input_number_set(v);
}

void param_32_value_set(number v) {
    v = this->param_32_value_constrain(v);
    this->param_32_value = v;
    this->sendParameter(3, false);

    if (this->param_32_value != this->param_32_lastValue) {
        this->getEngine()->presetTouched();
        this->param_32_lastValue = this->param_32_value;
    }

    this->dspexpr_10_in2_set(v);
}

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
    return 1;
}

Index getNumOutputChannels() const {
    return 10;
}

void allocateDataRefs() {
    this->p_01->allocateDataRefs();
    this->p_02->allocateDataRefs();
    this->p_03->allocateDataRefs();
    this->p_04->allocateDataRefs();

    if (this->layer1->hasRequestedSize()) {
        if (this->layer1->wantsFill())
            this->zeroDataRef(this->layer1);

        this->getEngine()->sendDataRefUpdated(0);
    }

    if (this->layer2->hasRequestedSize()) {
        if (this->layer2->wantsFill())
            this->zeroDataRef(this->layer2);

        this->getEngine()->sendDataRefUpdated(1);
    }

    if (this->layer3->hasRequestedSize()) {
        if (this->layer3->wantsFill())
            this->zeroDataRef(this->layer3);

        this->getEngine()->sendDataRefUpdated(2);
    }

    if (this->layer4->hasRequestedSize()) {
        if (this->layer4->wantsFill())
            this->zeroDataRef(this->layer4);

        this->getEngine()->sendDataRefUpdated(3);
    }
}

void initializeObjects() {
    this->p_01->initializeObjects();
    this->p_02->initializeObjects();
    this->p_03->initializeObjects();
    this->p_04->initializeObjects();
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

    {
        this->scheduleParamInit(0, 0);
    }

    {
        this->scheduleParamInit(1, 0);
    }

    {
        this->scheduleParamInit(2, 0);
    }

    {
        this->scheduleParamInit(3, 0);
    }

    this->processParamInitEvents();
}

static number param_29_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)100 / (number)100;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void dspexpr_05_in2_set(number v) {
    this->dspexpr_05_in2 = v;
}

void expr_17_out1_set(number v) {
    this->expr_17_out1 = v;
    this->dspexpr_05_in2_set(this->expr_17_out1);
}

void expr_17_in1_set(number in1) {
    this->expr_17_in1 = in1;
    this->expr_17_out1_set(this->expr_17_in1 * this->expr_17_in2);//#map:*_obj-51:1
}

void expr_18_out1_set(number v) {
    this->expr_18_out1 = v;
    this->expr_17_in1_set(this->expr_18_out1);
}

void expr_18_in1_set(number in1) {
    this->expr_18_in1 = in1;

    this->expr_18_out1_set(
        (this->expr_18_in2 == 0 ? 0 : (this->expr_18_in2 == 0. ? 0. : this->expr_18_in1 / this->expr_18_in2))
    );//#map:/_obj-49:1
}

static number param_30_value_constrain(number v) {
    v = (v > 3 ? 3 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)3 / (number)3;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void recordtilde_01_buffer_set(number v) {
    updateMultiRef(this, this->recordtilde_01_buffer, v);
}

static number param_31_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)1 / (number)1;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void recordtilde_01_record_set(number v) {
    this->recordtilde_01_record = v;
}

void expr_17_in2_set(number v) {
    this->expr_17_in2 = v;
    this->expr_17_in1_set(this->expr_17_in1);
}

void toggle_01_value_set(number v) {
    this->toggle_01_value = v;
    this->sendParameter(4, false);
    this->recordtilde_01_record_set(v);
    this->expr_17_in2_set(v);
}

void receive_09_output_number_set(number v) {
    this->receive_09_output_number = v;
    this->toggle_01_value_set(v);
}

void send_09_input_number_set(number v) {
    this->send_09_input_number = v;
    this->receive_09_output_number_set(v);
}

static number param_32_value_constrain(number v) {
    v = (v > 10 ? 10 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)10 / (number)100;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void dspexpr_10_in2_set(number v) {
    this->dspexpr_10_in2 = v;
}

void p_01_target_Playu45layer1_bang_bang() {
    this->p_01->receive_01_output_bang_bang();
}

void p_01_source_Playu45layer1_bang_bang() {
    this->p_01_target_Playu45layer1_bang_bang();
}

void p_01_target_Stopu45layer1_bang_bang() {
    this->p_01->receive_02_output_bang_bang();
}

void p_01_source_Stopu45layer1_bang_bang() {
    this->p_01_target_Stopu45layer1_bang_bang();
}

void p_01_out3_number_set(number ) {}

void p_02_target_Playu45layer2_bang_bang() {
    this->p_02->receive_03_output_bang_bang();
}

void p_02_source_Playu45layer2_bang_bang() {
    this->p_02_target_Playu45layer2_bang_bang();
}

void p_02_target_Stopu45layer2_bang_bang() {
    this->p_02->receive_04_output_bang_bang();
}

void p_02_source_Stopu45layer2_bang_bang() {
    this->p_02_target_Stopu45layer2_bang_bang();
}

void p_02_out3_number_set(number ) {}

void p_03_target_Playu45layer3_bang_bang() {
    this->p_03->receive_05_output_bang_bang();
}

void p_03_source_Playu45layer3_bang_bang() {
    this->p_03_target_Playu45layer3_bang_bang();
}

void p_03_target_Stopu45layer3_bang_bang() {
    this->p_03->receive_06_output_bang_bang();
}

void p_03_source_Stopu45layer3_bang_bang() {
    this->p_03_target_Stopu45layer3_bang_bang();
}

void p_03_out3_number_set(number ) {}

void p_04_target_Playu45layer4_bang_bang() {
    this->p_04->receive_07_output_bang_bang();
}

void p_04_source_Playu45layer4_bang_bang() {
    this->p_04_target_Playu45layer4_bang_bang();
}

void p_04_target_Stopu45layer4_bang_bang() {
    this->p_04->receive_08_output_bang_bang();
}

void p_04_source_Stopu45layer4_bang_bang() {
    this->p_04_target_Stopu45layer4_bang_bang();
}

void p_04_out3_number_set(number ) {}

void dspexpr_10_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
    }
}

void recordtilde_01_perform(
    number record,
    number begin,
    number end,
    const Sample * input1,
    Sample * sync,
    Index n
) {
    RNBO_UNUSED(input1);
    RNBO_UNUSED(end);
    RNBO_UNUSED(begin);
    auto __recordtilde_01_loop = this->recordtilde_01_loop;
    auto __recordtilde_01_wIndex = this->recordtilde_01_wIndex;
    auto __recordtilde_01_lastRecord = this->recordtilde_01_lastRecord;
    ConstSampleArray<1> input = {input1};
    number bufferSize = this->recordtilde_01_buffer->getSize();
    number srInv = (number)1 / this->samplerate();

    if (bufferSize > 0) {
        number maxChannel = this->recordtilde_01_buffer->getChannels();
        number touched = false;

        for (Index i = 0; i < n; i++) {
            number loopBegin = 0;
            number loopEnd = bufferSize;

            if (loopEnd > loopBegin) {
                {
                    if ((bool)(record) && __recordtilde_01_lastRecord != record) {
                        __recordtilde_01_wIndex = loopBegin;
                    }
                }

                if (record != 0 && __recordtilde_01_wIndex < loopEnd) {
                    for (number channel = 0; channel < 1; channel++) {
                        number effectiveChannel = channel + 0;

                        if (effectiveChannel < maxChannel) {
                            this->recordtilde_01_buffer->setSample(channel, __recordtilde_01_wIndex, input[(Index)channel][(Index)i]);
                            touched = true;
                        } else
                            break;
                    }

                    __recordtilde_01_wIndex++;

                    if ((bool)(__recordtilde_01_loop) && __recordtilde_01_wIndex >= loopEnd) {
                        __recordtilde_01_wIndex = loopBegin;
                    }

                    {
                        sync[(Index)i] = this->recordtilde_01_calcSync(__recordtilde_01_wIndex, loopBegin, loopEnd - loopBegin, bufferSize, srInv);
                    }
                } else {
                    sync[(Index)i] = 0;
                }

                {
                    __recordtilde_01_lastRecord = record;
                }
            }
        }

        if ((bool)(touched)) {
            this->recordtilde_01_buffer->setTouched(true);
            this->recordtilde_01_buffer->setSampleRate(this->samplerate());
        }
    }

    this->recordtilde_01_lastRecord = __recordtilde_01_lastRecord;
    this->recordtilde_01_wIndex = __recordtilde_01_wIndex;
}

void dspexpr_05_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
    }
}

void p_01_perform(Sample * out1, Sample * out2, Index n) {
    // subpatcher: layer1
    SampleArray<2> outs = {out1, out2};

    this->p_01->process(nullptr, 0, outs, 2, n);
}

void dspexpr_09_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_02_perform(Sample * out1, Sample * out2, Index n) {
    // subpatcher: layer2
    SampleArray<2> outs = {out1, out2};

    this->p_02->process(nullptr, 0, outs, 2, n);
}

void dspexpr_07_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_03_perform(Sample * out1, Sample * out2, Index n) {
    // subpatcher: layer3
    SampleArray<2> outs = {out1, out2};

    this->p_03->process(nullptr, 0, outs, 2, n);
}

void dspexpr_08_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_04_perform(Sample * out1, Sample * out2, Index n) {
    // subpatcher: layer4
    SampleArray<2> outs = {out1, out2};

    this->p_04->process(nullptr, 0, outs, 2, n);
}

void dspexpr_06_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void toggle_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->toggle_01_value;
}

void toggle_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->toggle_01_value_set(preset["value"]);
}

void param_29_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_29_value;
}

void param_29_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_29_value_set(preset["value"]);
}

number recordtilde_01_calcSync(
    number writeIndex,
    number loopMin,
    number loopLength,
    SampleIndex bufferLength,
    number srInv
) {
    RNBO_UNUSED(srInv);
    RNBO_UNUSED(loopLength);
    RNBO_UNUSED(loopMin);

    {
        {
            return writeIndex / bufferLength;
        }
    }
}

void param_30_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_30_value;
}

void param_30_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_30_value_set(preset["value"]);
}

void param_31_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_31_value;
}

void param_31_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_31_value_set(preset["value"]);
}

void param_32_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_32_value;
}

void param_32_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_32_value_set(preset["value"]);
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
    dspexpr_05_in1 = 0;
    dspexpr_05_in2 = 0;
    dspexpr_06_in1 = 0;
    dspexpr_06_in2 = 0;
    dspexpr_07_in1 = 0;
    dspexpr_07_in2 = 0;
    dspexpr_08_in1 = 0;
    dspexpr_08_in2 = 0;
    dspexpr_09_in1 = 0;
    dspexpr_09_in2 = 0;
    expr_17_in1 = 0;
    expr_17_in2 = 0;
    expr_17_out1 = 0;
    expr_18_in1 = 0;
    expr_18_in2 = 100;
    expr_18_out1 = 0;
    toggle_01_value = 0;
    receive_09_output_number = 0;
    param_29_value = 100;
    recordtilde_01_record = 0;
    recordtilde_01_begin = 0;
    recordtilde_01_end = -1;
    recordtilde_01_loop = 0;
    p_01_target = 0;
    dspexpr_10_in1 = 0;
    dspexpr_10_in2 = 0;
    p_02_target = 0;
    param_30_value = 0;
    p_03_target = 0;
    send_09_input_number = 0;
    param_31_value = 0;
    p_04_target = 0;
    param_32_value = 1;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    param_29_lastValue = 0;
    recordtilde_01_wIndex = 0;
    recordtilde_01_lastRecord = 0;
    param_30_lastValue = 0;
    param_31_lastValue = 0;
    param_32_lastValue = 0;
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

    number dspexpr_05_in1;
    number dspexpr_05_in2;
    number dspexpr_06_in1;
    number dspexpr_06_in2;
    number dspexpr_07_in1;
    number dspexpr_07_in2;
    number dspexpr_08_in1;
    number dspexpr_08_in2;
    number dspexpr_09_in1;
    number dspexpr_09_in2;
    number expr_17_in1;
    number expr_17_in2;
    number expr_17_out1;
    number expr_18_in1;
    number expr_18_in2;
    number expr_18_out1;
    number toggle_01_value;
    number receive_09_output_number;
    list receive_09_output_list;
    number param_29_value;
    number recordtilde_01_record;
    number recordtilde_01_begin;
    number recordtilde_01_end;
    number recordtilde_01_loop;
    number p_01_target;
    number dspexpr_10_in1;
    number dspexpr_10_in2;
    number p_02_target;
    number param_30_value;
    number p_03_target;
    number send_09_input_number;
    list send_09_input_list;
    number param_31_value;
    number p_04_target;
    number param_32_value;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[2];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number param_29_lastValue;
    Float32MultiBufferRef recordtilde_01_buffer;
    SampleIndex recordtilde_01_wIndex;
    number recordtilde_01_lastRecord;
    number param_30_lastValue;
    number param_31_lastValue;
    number param_32_lastValue;
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
    MultiDataRef recordtilde_01_bufferobj;
    DataRef layer1;
    DataRef layer2;
    DataRef layer3;
    DataRef layer4;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_09* p_01;
    RNBOSubpatcher_10* p_02;
    RNBOSubpatcher_11* p_03;
    RNBOSubpatcher_12* p_04;

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

