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

class RNBOSubpatcher_1199 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1199()
    {
    }
    
    ~RNBOSubpatcher_1199()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_08_perform(this->signals[0], n);
        this->dspexpr_01_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_01_perform(in1, this->signals[1], this->sah_tilde_01_thresh, out1, n);
        this->sah_tilde_02_perform(in2, this->signals[1], this->sah_tilde_02_thresh, this->signals[2], n);
        this->delaytilde_01_perform(this->delaytilde_01_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_06_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_07_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_05_perform(this->signals[1], this->dspexpr_05_in2, this->signals[3], n);
        this->dspexpr_04_perform(this->signals[3], this->dspexpr_04_in2, this->signals[1], n);
        this->dspexpr_03_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_09_perform(in4, this->dspexpr_09_in2, this->signals[0], n);
        this->dspexpr_02_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_01_perform(
            this->signals[1],
            this->slide_tilde_01_up,
            this->slide_tilde_01_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_01_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_01_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_01_del_buffer = new Float64Buffer(this->delaytilde_01_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_01_del_bufferobj = initDataRef("delaytilde_01_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_01_del_bufferobj->setIndex(1);
        this->delaytilde_01_del_buffer = new Float64Buffer(this->delaytilde_01_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_01_out1_bang_bang() {
        this->expr_01_in1_bang();
    }
    
    void eventinlet_01_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_01_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_01_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_05_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_01_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_01_del_buffer = this->delaytilde_01_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_01_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_01_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_01_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_09_in2_set(number v) {
        this->dspexpr_09_in2 = v;
    }
    
    void expr_01_out1_set(number v) {
        this->expr_01_out1 = v;
        this->dspexpr_09_in2_set(this->expr_01_out1);
    }
    
    void expr_01_in1_set(number in1) {
        this->expr_01_in1 = in1;
    
        this->expr_01_out1_set(
            (this->expr_01_in2 == 0 ? 0 : (this->expr_01_in2 == 0. ? 0. : this->expr_01_in1 / this->expr_01_in2))
        );//#map:calcgainpitch//_obj-64:1
    }
    
    void dspexpr_05_in2_set(number v) {
        this->dspexpr_05_in2 = v;
    }
    
    void eventinlet_01_out1_number_set(number v) {
        this->expr_01_in1_set(v);
        this->dspexpr_05_in2_set(v);
    }
    
    void expr_01_in1_bang() {
        this->expr_01_out1_set(
            (this->expr_01_in2 == 0 ? 0 : (this->expr_01_in2 == 0. ? 0. : this->expr_01_in1 / this->expr_01_in2))
        );//#map:calcgainpitch//_obj-64:1
    }
    
    void expr_01_in2_set(number v) {
        this->expr_01_in2 = v;
    }
    
    void dspexpr_08_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_01_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_01_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_01_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_02_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_02_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_01_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_01_crossfadeDelay = this->delaytilde_01_crossfadeDelay;
        auto __delaytilde_01_rampInSamples = this->delaytilde_01_rampInSamples;
        auto __delaytilde_01_ramp = this->delaytilde_01_ramp;
        auto __delaytilde_01_lastDelay = this->delaytilde_01_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_01_lastDelay == -1) {
                __delaytilde_01_lastDelay = 512;
            }
    
            if (__delaytilde_01_ramp > 0) {
                number factor = __delaytilde_01_ramp / __delaytilde_01_rampInSamples;
                output[(Index)i] = this->delaytilde_01_del_read(__delaytilde_01_crossfadeDelay, 0) * factor + this->delaytilde_01_del_read(__delaytilde_01_lastDelay, 0) * (1. - factor);
                __delaytilde_01_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_01_lastDelay) {
                    __delaytilde_01_ramp = __delaytilde_01_rampInSamples;
                    __delaytilde_01_crossfadeDelay = __delaytilde_01_lastDelay;
                    __delaytilde_01_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_01_del_read(__delaytilde_01_crossfadeDelay, 0);
                    __delaytilde_01_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_01_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_01_del_write(input[(Index)i]);
            this->delaytilde_01_del_step();
        }
    
        this->delaytilde_01_lastDelay = __delaytilde_01_lastDelay;
        this->delaytilde_01_ramp = __delaytilde_01_ramp;
        this->delaytilde_01_crossfadeDelay = __delaytilde_01_crossfadeDelay;
    }
    
    void dspexpr_06_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_07_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_05_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_04_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_03_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_09_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_02_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_01_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_01_prev = this->slide_tilde_01_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_01_prev;
            __slide_tilde_01_prev = __slide_tilde_01_prev + ((x[(Index)i] > __slide_tilde_01_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_01_prev;
        }
    
        this->slide_tilde_01_prev = __slide_tilde_01_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_01_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_01_s_prev <= thresh && trig > thresh)
            this->sah_tilde_01_s_value = x;
    
        this->sah_tilde_01_s_prev = trig;
        return this->sah_tilde_01_s_value;
    }
    
    void sah_tilde_01_s_reset() {
        this->sah_tilde_01_s_prev = 0.;
        this->sah_tilde_01_s_value = 0.;
    }
    
    number sah_tilde_02_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_02_s_prev <= thresh && trig > thresh)
            this->sah_tilde_02_s_value = x;
    
        this->sah_tilde_02_s_prev = trig;
        return this->sah_tilde_02_s_value;
    }
    
    void sah_tilde_02_s_reset() {
        this->sah_tilde_02_s_prev = 0.;
        this->sah_tilde_02_s_value = 0.;
    }
    
    void delaytilde_01_del_step() {
        this->delaytilde_01_del_reader++;
    
        if (this->delaytilde_01_del_reader >= (int)(this->delaytilde_01_del_buffer->getSize()))
            this->delaytilde_01_del_reader = 0;
    }
    
    number delaytilde_01_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? 1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? 1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_01_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_01_del_wrap))
        );
    }
    
    void delaytilde_01_del_write(number v) {
        this->delaytilde_01_del_writer = this->delaytilde_01_del_reader;
        this->delaytilde_01_del_buffer[(Index)this->delaytilde_01_del_writer] = v;
    }
    
    number delaytilde_01_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_01_del__maxdelay : size);
        number val = this->delaytilde_01_del_read(effectiveSize, 0);
        this->delaytilde_01_del_write(v);
        this->delaytilde_01_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_01_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_01_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_01_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_01_del_init() {
        auto result = this->delaytilde_01_del_calcSizeInSamples();
        this->delaytilde_01_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_01_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_01_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_01_del_clear() {
        this->delaytilde_01_del_buffer->setZero();
    }
    
    void delaytilde_01_del_reset() {
        auto result = this->delaytilde_01_del_calcSizeInSamples();
        this->delaytilde_01_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_01_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_01_del_buffer);
        this->delaytilde_01_del_wrap = this->delaytilde_01_del_buffer->getSize() - 1;
        this->delaytilde_01_del_clear();
    
        if (this->delaytilde_01_del_reader >= this->delaytilde_01_del__maxdelay || this->delaytilde_01_del_writer >= this->delaytilde_01_del__maxdelay) {
            this->delaytilde_01_del_reader = 0;
            this->delaytilde_01_del_writer = 0;
        }
    }
    
    void delaytilde_01_del_dspsetup() {
        this->delaytilde_01_del_reset();
    }
    
    number delaytilde_01_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_01_del_size() {
        return this->delaytilde_01_del__maxdelay;
    }
    
    void delaytilde_01_dspsetup(bool force) {
        if ((bool)(this->delaytilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_01_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_01_lastDelay = -1;
        this->delaytilde_01_setupDone = true;
        this->delaytilde_01_del_dspsetup();
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
        sah_tilde_01_input = 0;
        sah_tilde_01_trig = -1;
        sah_tilde_01_thresh = 0;
        dspexpr_01_in1 = 0;
        dspexpr_01_in2 = 0;
        slide_tilde_01_x = 0;
        slide_tilde_01_up = 1000;
        slide_tilde_01_down = 1000;
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 0;
        dspexpr_03_in1 = 0;
        dspexpr_03_in2 = 0;
        dspexpr_04_in1 = 0;
        dspexpr_04_in2 = 512;
        dspexpr_05_in1 = 0;
        dspexpr_05_in2 = 48000;
        dspexpr_06_in1 = 0;
        dspexpr_06_in2 = 0;
        dspexpr_07_in1 = 0;
        sah_tilde_02_input = 0;
        sah_tilde_02_trig = -1;
        sah_tilde_02_thresh = 0;
        delaytilde_01_delay = 512;
        dspexpr_09_in1 = 0;
        dspexpr_09_in2 = 0;
        expr_01_in1 = 0;
        expr_01_in2 = 512;
        expr_01_out1 = 0;
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
        sah_tilde_01_s_prev = 0;
        sah_tilde_01_s_value = 0;
        slide_tilde_01_prev = 0;
        sah_tilde_02_s_prev = 0;
        sah_tilde_02_s_value = 0;
        delaytilde_01_lastDelay = -1;
        delaytilde_01_crossfadeDelay = 0;
        delaytilde_01_ramp = 0;
        delaytilde_01_rampInSamples = 0;
        delaytilde_01_del__maxdelay = 0;
        delaytilde_01_del_sizemode = 0;
        delaytilde_01_del_wrap = 0;
        delaytilde_01_del_reader = 0;
        delaytilde_01_del_writer = 0;
        delaytilde_01_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_01_input;
        number sah_tilde_01_trig;
        number sah_tilde_01_thresh;
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number slide_tilde_01_x;
        number slide_tilde_01_up;
        number slide_tilde_01_down;
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number dspexpr_03_in1;
        number dspexpr_03_in2;
        number dspexpr_04_in1;
        number dspexpr_04_in2;
        number dspexpr_05_in1;
        number dspexpr_05_in2;
        number dspexpr_06_in1;
        number dspexpr_06_in2;
        number dspexpr_07_in1;
        number sah_tilde_02_input;
        number sah_tilde_02_trig;
        number sah_tilde_02_thresh;
        number delaytilde_01_delay;
        number dspexpr_09_in1;
        number dspexpr_09_in2;
        number expr_01_in1;
        number expr_01_in2;
        number expr_01_out1;
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
        number sah_tilde_01_s_prev;
        number sah_tilde_01_s_value;
        number slide_tilde_01_prev;
        number sah_tilde_02_s_prev;
        number sah_tilde_02_s_value;
        number delaytilde_01_lastDelay;
        number delaytilde_01_crossfadeDelay;
        number delaytilde_01_ramp;
        long delaytilde_01_rampInSamples;
        Float64BufferRef delaytilde_01_del_buffer;
        Index delaytilde_01_del__maxdelay;
        Int delaytilde_01_del_sizemode;
        Index delaytilde_01_del_wrap;
        Int delaytilde_01_del_reader;
        Int delaytilde_01_del_writer;
        bool delaytilde_01_setupDone;
        number stackprotect_count;
        DataRef delaytilde_01_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1200 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1200()
    {
    }
    
    ~RNBOSubpatcher_1200()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_01_perform(in1, this->signals[0], n);
        this->dspexpr_11_perform(this->signals[0], this->dspexpr_11_in2, this->signals[1], n);
        this->dspexpr_10_perform(this->signals[1], this->dspexpr_10_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_01_innerScala_init();
        this->ftom_tilde_01_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_01_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_01_base = this->ftom_tilde_01_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_01_innerFtoM_next(frequency[(Index)i], __ftom_tilde_01_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_11_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_10_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_01_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_01_innerFtoM_lastInValue && tuning == this->ftom_tilde_01_innerFtoM_lastTuning) {
            return this->ftom_tilde_01_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_01_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_01_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_01_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_01_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_01_innerFtoM_reset() {
        this->ftom_tilde_01_innerFtoM_lastInValue = 0;
        this->ftom_tilde_01_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_01_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_01_innerScala_mid(int v) {
        this->ftom_tilde_01_innerScala_kbmMid = v;
        this->ftom_tilde_01_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_01_innerScala_ref(int v) {
        this->ftom_tilde_01_innerScala_kbmRefNum = v;
        this->ftom_tilde_01_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_01_innerScala_base(number v) {
        this->ftom_tilde_01_innerScala_kbmRefFreq = v;
        this->ftom_tilde_01_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_01_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_01_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_01_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_01_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_01_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_01_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_01_innerScala_lastValid) && this->ftom_tilde_01_innerScala_lastNote == note) {
            return this->ftom_tilde_01_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_01_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_01_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_01_innerScala_refFreq);
        }
    
        this->ftom_tilde_01_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_01_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_01_innerScala_lastValid) && this->ftom_tilde_01_innerScala_lastFreq == hz) {
            return this->ftom_tilde_01_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_01_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_01_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_01_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_01_innerScala_octdegree(degree, this->ftom_tilde_01_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_01_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_01_innerScala_kbmValid[(Index)(i + this->ftom_tilde_01_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_01_innerScala_kbmSize + entry + this->ftom_tilde_01_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_01_innerScala_updateLast(out, hz);
        return this->ftom_tilde_01_innerScala_lastNote;
    }
    
    int ftom_tilde_01_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_01_innerScala_lastValid = false;
            this->ftom_tilde_01_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_01_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_01_innerScala_sclOctaveMul = last;
            this->ftom_tilde_01_innerScala_sclEntryCount = (int)(this->ftom_tilde_01_innerScala_sclExpMul->length);
            this->ftom_tilde_01_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_01_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_01_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_01_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_01_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_01_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_01_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_01_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_01_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_01_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_01_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_01_innerScala_kbmValid = kbm;
            this->ftom_tilde_01_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_01_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_01_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_01_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_01_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_01_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_01_innerScala_lastValid = true;
        this->ftom_tilde_01_innerScala_lastNote = note;
        this->ftom_tilde_01_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_01_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_01_innerScala_refFreq) / this->ftom_tilde_01_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_01_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_01_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_01_innerScala_refFreq);
            n = this->ftom_tilde_01_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_01_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_01_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_01_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_01_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_01_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_01_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_01_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_01_innerScala_kbmMin == this->ftom_tilde_01_innerScala_kbmMax && this->ftom_tilde_01_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_01_innerScala_kbmMin && note <= this->ftom_tilde_01_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_01_innerScala_kbmMid));
    
            if (this->ftom_tilde_01_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_01_innerScala_octdegree(degree, this->ftom_tilde_01_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_01_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_01_innerScala_kbmValid[(Index)(this->ftom_tilde_01_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_01_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_01_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_01_innerScala_octdegree(degree, this->ftom_tilde_01_innerScala_sclEntryCount);
        return this->ftom_tilde_01_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_01_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_01_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_01_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_01_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_01_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_01_innerScala_updateRefFreq() {
        this->ftom_tilde_01_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_01_innerScala_kbmRefNum - this->ftom_tilde_01_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_01_innerScala_refFreq = this->ftom_tilde_01_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_01_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_01_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_01_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_01_innerScala_kbmSize) {
                if (index < this->ftom_tilde_01_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_01_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_01_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_01_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_01_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_01_innerScala_refFreq = this->ftom_tilde_01_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_01_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_01_innerScala_reset() {
        this->ftom_tilde_01_innerScala_internal = true;
        this->ftom_tilde_01_innerScala_lastValid = false;
        this->ftom_tilde_01_innerScala_lastNote = 0;
        this->ftom_tilde_01_innerScala_lastFreq = 0;
        this->ftom_tilde_01_innerScala_sclEntryCount = 0;
        this->ftom_tilde_01_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_01_innerScala_sclExpMul = {};
        this->ftom_tilde_01_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_01_innerScala_kbmMid = 60;
        this->ftom_tilde_01_innerScala_kbmRefNum = 69;
        this->ftom_tilde_01_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_01_innerScala_kbmSize = 0;
        this->ftom_tilde_01_innerScala_kbmMin = 0;
        this->ftom_tilde_01_innerScala_kbmMax = 0;
        this->ftom_tilde_01_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_01_innerScala_kbmMapSize = 0;
        this->ftom_tilde_01_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_01_init() {
        this->ftom_tilde_01_innerScala_update(this->ftom_tilde_01_scale, this->ftom_tilde_01_map);
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
        dspexpr_10_in1 = 0;
        dspexpr_10_in2 = 60;
        dspexpr_11_in1 = 0;
        dspexpr_11_in2 = 60;
        ftom_tilde_01_frequency = 0;
        ftom_tilde_01_base = 440;
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
        ftom_tilde_01_innerFtoM_lastInValue = 0;
        ftom_tilde_01_innerFtoM_lastOutValue = 0;
        ftom_tilde_01_innerFtoM_lastTuning = 0;
        ftom_tilde_01_innerScala_internal = true;
        ftom_tilde_01_innerScala_lastValid = false;
        ftom_tilde_01_innerScala_lastNote = 0;
        ftom_tilde_01_innerScala_lastFreq = 0;
        ftom_tilde_01_innerScala_sclEntryCount = 0;
        ftom_tilde_01_innerScala_sclOctaveMul = 1;
        ftom_tilde_01_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_01_innerScala_kbmMid = 60;
        ftom_tilde_01_innerScala_kbmRefNum = 69;
        ftom_tilde_01_innerScala_kbmRefFreq = 440;
        ftom_tilde_01_innerScala_kbmSize = 0;
        ftom_tilde_01_innerScala_kbmMin = 0;
        ftom_tilde_01_innerScala_kbmMax = 0;
        ftom_tilde_01_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_01_innerScala_kbmMapSize = 0;
        ftom_tilde_01_innerScala_refFreq = 261.63;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_10_in1;
        number dspexpr_10_in2;
        number dspexpr_11_in1;
        number dspexpr_11_in2;
        number ftom_tilde_01_frequency;
        list ftom_tilde_01_scale;
        list ftom_tilde_01_map;
        number ftom_tilde_01_base;
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
        number ftom_tilde_01_innerFtoM_lastInValue;
        number ftom_tilde_01_innerFtoM_lastOutValue;
        number ftom_tilde_01_innerFtoM_lastTuning;
        bool ftom_tilde_01_innerScala_internal;
        const Index ftom_tilde_01_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_01_innerScala_lastValid;
        number ftom_tilde_01_innerScala_lastNote;
        number ftom_tilde_01_innerScala_lastFreq;
        int ftom_tilde_01_innerScala_sclEntryCount;
        number ftom_tilde_01_innerScala_sclOctaveMul;
        list ftom_tilde_01_innerScala_sclExpMul;
        list ftom_tilde_01_innerScala_kbmValid;
        int ftom_tilde_01_innerScala_kbmMid;
        int ftom_tilde_01_innerScala_kbmRefNum;
        number ftom_tilde_01_innerScala_kbmRefFreq;
        int ftom_tilde_01_innerScala_kbmSize;
        int ftom_tilde_01_innerScala_kbmMin;
        int ftom_tilde_01_innerScala_kbmMax;
        int ftom_tilde_01_innerScala_kbmOctaveDegree;
        Index ftom_tilde_01_innerScala_kbmMapSize;
        number ftom_tilde_01_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1201 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1201()
    {
    }
    
    ~RNBOSubpatcher_1201()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_02_perform(in1, this->signals[0], n);
        this->dspexpr_13_perform(this->signals[0], this->dspexpr_13_in2, this->signals[1], n);
        this->dspexpr_12_perform(this->signals[1], this->dspexpr_12_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_02_innerScala_init();
        this->ftom_tilde_02_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_02_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_02_base = this->ftom_tilde_02_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_02_innerFtoM_next(frequency[(Index)i], __ftom_tilde_02_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_13_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_12_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_02_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_02_innerFtoM_lastInValue && tuning == this->ftom_tilde_02_innerFtoM_lastTuning) {
            return this->ftom_tilde_02_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_02_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_02_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_02_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_02_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_02_innerFtoM_reset() {
        this->ftom_tilde_02_innerFtoM_lastInValue = 0;
        this->ftom_tilde_02_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_02_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_02_innerScala_mid(int v) {
        this->ftom_tilde_02_innerScala_kbmMid = v;
        this->ftom_tilde_02_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_02_innerScala_ref(int v) {
        this->ftom_tilde_02_innerScala_kbmRefNum = v;
        this->ftom_tilde_02_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_02_innerScala_base(number v) {
        this->ftom_tilde_02_innerScala_kbmRefFreq = v;
        this->ftom_tilde_02_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_02_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_02_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_02_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_02_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_02_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_02_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_02_innerScala_lastValid) && this->ftom_tilde_02_innerScala_lastNote == note) {
            return this->ftom_tilde_02_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_02_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_02_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_02_innerScala_refFreq);
        }
    
        this->ftom_tilde_02_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_02_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_02_innerScala_lastValid) && this->ftom_tilde_02_innerScala_lastFreq == hz) {
            return this->ftom_tilde_02_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_02_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_02_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_02_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_02_innerScala_octdegree(degree, this->ftom_tilde_02_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_02_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_02_innerScala_kbmValid[(Index)(i + this->ftom_tilde_02_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_02_innerScala_kbmSize + entry + this->ftom_tilde_02_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_02_innerScala_updateLast(out, hz);
        return this->ftom_tilde_02_innerScala_lastNote;
    }
    
    int ftom_tilde_02_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_02_innerScala_lastValid = false;
            this->ftom_tilde_02_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_02_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_02_innerScala_sclOctaveMul = last;
            this->ftom_tilde_02_innerScala_sclEntryCount = (int)(this->ftom_tilde_02_innerScala_sclExpMul->length);
            this->ftom_tilde_02_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_02_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_02_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_02_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_02_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_02_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_02_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_02_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_02_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_02_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_02_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_02_innerScala_kbmValid = kbm;
            this->ftom_tilde_02_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_02_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_02_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_02_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_02_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_02_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_02_innerScala_lastValid = true;
        this->ftom_tilde_02_innerScala_lastNote = note;
        this->ftom_tilde_02_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_02_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_02_innerScala_refFreq) / this->ftom_tilde_02_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_02_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_02_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_02_innerScala_refFreq);
            n = this->ftom_tilde_02_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_02_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_02_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_02_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_02_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_02_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_02_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_02_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_02_innerScala_kbmMin == this->ftom_tilde_02_innerScala_kbmMax && this->ftom_tilde_02_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_02_innerScala_kbmMin && note <= this->ftom_tilde_02_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_02_innerScala_kbmMid));
    
            if (this->ftom_tilde_02_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_02_innerScala_octdegree(degree, this->ftom_tilde_02_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_02_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_02_innerScala_kbmValid[(Index)(this->ftom_tilde_02_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_02_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_02_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_02_innerScala_octdegree(degree, this->ftom_tilde_02_innerScala_sclEntryCount);
        return this->ftom_tilde_02_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_02_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_02_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_02_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_02_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_02_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_02_innerScala_updateRefFreq() {
        this->ftom_tilde_02_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_02_innerScala_kbmRefNum - this->ftom_tilde_02_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_02_innerScala_refFreq = this->ftom_tilde_02_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_02_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_02_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_02_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_02_innerScala_kbmSize) {
                if (index < this->ftom_tilde_02_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_02_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_02_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_02_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_02_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_02_innerScala_refFreq = this->ftom_tilde_02_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_02_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_02_innerScala_reset() {
        this->ftom_tilde_02_innerScala_internal = true;
        this->ftom_tilde_02_innerScala_lastValid = false;
        this->ftom_tilde_02_innerScala_lastNote = 0;
        this->ftom_tilde_02_innerScala_lastFreq = 0;
        this->ftom_tilde_02_innerScala_sclEntryCount = 0;
        this->ftom_tilde_02_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_02_innerScala_sclExpMul = {};
        this->ftom_tilde_02_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_02_innerScala_kbmMid = 60;
        this->ftom_tilde_02_innerScala_kbmRefNum = 69;
        this->ftom_tilde_02_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_02_innerScala_kbmSize = 0;
        this->ftom_tilde_02_innerScala_kbmMin = 0;
        this->ftom_tilde_02_innerScala_kbmMax = 0;
        this->ftom_tilde_02_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_02_innerScala_kbmMapSize = 0;
        this->ftom_tilde_02_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_02_init() {
        this->ftom_tilde_02_innerScala_update(this->ftom_tilde_02_scale, this->ftom_tilde_02_map);
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
        dspexpr_12_in2 = 60;
        dspexpr_13_in1 = 0;
        dspexpr_13_in2 = 60;
        ftom_tilde_02_frequency = 0;
        ftom_tilde_02_base = 440;
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
        ftom_tilde_02_innerFtoM_lastInValue = 0;
        ftom_tilde_02_innerFtoM_lastOutValue = 0;
        ftom_tilde_02_innerFtoM_lastTuning = 0;
        ftom_tilde_02_innerScala_internal = true;
        ftom_tilde_02_innerScala_lastValid = false;
        ftom_tilde_02_innerScala_lastNote = 0;
        ftom_tilde_02_innerScala_lastFreq = 0;
        ftom_tilde_02_innerScala_sclEntryCount = 0;
        ftom_tilde_02_innerScala_sclOctaveMul = 1;
        ftom_tilde_02_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_02_innerScala_kbmMid = 60;
        ftom_tilde_02_innerScala_kbmRefNum = 69;
        ftom_tilde_02_innerScala_kbmRefFreq = 440;
        ftom_tilde_02_innerScala_kbmSize = 0;
        ftom_tilde_02_innerScala_kbmMin = 0;
        ftom_tilde_02_innerScala_kbmMax = 0;
        ftom_tilde_02_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_02_innerScala_kbmMapSize = 0;
        ftom_tilde_02_innerScala_refFreq = 261.63;
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
        number ftom_tilde_02_frequency;
        list ftom_tilde_02_scale;
        list ftom_tilde_02_map;
        number ftom_tilde_02_base;
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
        number ftom_tilde_02_innerFtoM_lastInValue;
        number ftom_tilde_02_innerFtoM_lastOutValue;
        number ftom_tilde_02_innerFtoM_lastTuning;
        bool ftom_tilde_02_innerScala_internal;
        const Index ftom_tilde_02_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_02_innerScala_lastValid;
        number ftom_tilde_02_innerScala_lastNote;
        number ftom_tilde_02_innerScala_lastFreq;
        int ftom_tilde_02_innerScala_sclEntryCount;
        number ftom_tilde_02_innerScala_sclOctaveMul;
        list ftom_tilde_02_innerScala_sclExpMul;
        list ftom_tilde_02_innerScala_kbmValid;
        int ftom_tilde_02_innerScala_kbmMid;
        int ftom_tilde_02_innerScala_kbmRefNum;
        number ftom_tilde_02_innerScala_kbmRefFreq;
        int ftom_tilde_02_innerScala_kbmSize;
        int ftom_tilde_02_innerScala_kbmMin;
        int ftom_tilde_02_innerScala_kbmMax;
        int ftom_tilde_02_innerScala_kbmOctaveDegree;
        Index ftom_tilde_02_innerScala_kbmMapSize;
        number ftom_tilde_02_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1202 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1202()
    {
    }
    
    ~RNBOSubpatcher_1202()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_21_perform(this->signals[0], n);
        this->dspexpr_14_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_03_perform(in1, this->signals[1], this->sah_tilde_03_thresh, out1, n);
        this->sah_tilde_04_perform(in2, this->signals[1], this->sah_tilde_04_thresh, this->signals[2], n);
        this->delaytilde_02_perform(this->delaytilde_02_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_19_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_20_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_18_perform(this->signals[1], this->dspexpr_18_in2, this->signals[3], n);
        this->dspexpr_17_perform(this->signals[3], this->dspexpr_17_in2, this->signals[1], n);
        this->dspexpr_16_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_22_perform(in4, this->dspexpr_22_in2, this->signals[0], n);
        this->dspexpr_15_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_02_perform(
            this->signals[1],
            this->slide_tilde_02_up,
            this->slide_tilde_02_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_02_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_02_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_02_del_buffer = new Float64Buffer(this->delaytilde_02_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_02_del_bufferobj = initDataRef("delaytilde_02_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_02_del_bufferobj->setIndex(1);
        this->delaytilde_02_del_buffer = new Float64Buffer(this->delaytilde_02_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_02_out1_bang_bang() {
        this->expr_02_in1_bang();
    }
    
    void eventinlet_02_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_02_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_02_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_18_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_02_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_02_del_buffer = this->delaytilde_02_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_02_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_02_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_02_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_22_in2_set(number v) {
        this->dspexpr_22_in2 = v;
    }
    
    void expr_02_out1_set(number v) {
        this->expr_02_out1 = v;
        this->dspexpr_22_in2_set(this->expr_02_out1);
    }
    
    void expr_02_in1_set(number in1) {
        this->expr_02_in1 = in1;
    
        this->expr_02_out1_set(
            (this->expr_02_in2 == 0 ? 0 : (this->expr_02_in2 == 0. ? 0. : this->expr_02_in1 / this->expr_02_in2))
        );//#map:calcgainpitch[1]//_obj-64:1
    }
    
    void dspexpr_18_in2_set(number v) {
        this->dspexpr_18_in2 = v;
    }
    
    void eventinlet_02_out1_number_set(number v) {
        this->expr_02_in1_set(v);
        this->dspexpr_18_in2_set(v);
    }
    
    void expr_02_in1_bang() {
        this->expr_02_out1_set(
            (this->expr_02_in2 == 0 ? 0 : (this->expr_02_in2 == 0. ? 0. : this->expr_02_in1 / this->expr_02_in2))
        );//#map:calcgainpitch[1]//_obj-64:1
    }
    
    void expr_02_in2_set(number v) {
        this->expr_02_in2 = v;
    }
    
    void dspexpr_21_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_14_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_03_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_03_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_04_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_04_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_02_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_02_crossfadeDelay = this->delaytilde_02_crossfadeDelay;
        auto __delaytilde_02_rampInSamples = this->delaytilde_02_rampInSamples;
        auto __delaytilde_02_ramp = this->delaytilde_02_ramp;
        auto __delaytilde_02_lastDelay = this->delaytilde_02_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_02_lastDelay == -1) {
                __delaytilde_02_lastDelay = 512;
            }
    
            if (__delaytilde_02_ramp > 0) {
                number factor = __delaytilde_02_ramp / __delaytilde_02_rampInSamples;
                output[(Index)i] = this->delaytilde_02_del_read(__delaytilde_02_crossfadeDelay, 0) * factor + this->delaytilde_02_del_read(__delaytilde_02_lastDelay, 0) * (1. - factor);
                __delaytilde_02_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_02_lastDelay) {
                    __delaytilde_02_ramp = __delaytilde_02_rampInSamples;
                    __delaytilde_02_crossfadeDelay = __delaytilde_02_lastDelay;
                    __delaytilde_02_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_02_del_read(__delaytilde_02_crossfadeDelay, 0);
                    __delaytilde_02_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_02_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_02_del_write(input[(Index)i]);
            this->delaytilde_02_del_step();
        }
    
        this->delaytilde_02_lastDelay = __delaytilde_02_lastDelay;
        this->delaytilde_02_ramp = __delaytilde_02_ramp;
        this->delaytilde_02_crossfadeDelay = __delaytilde_02_crossfadeDelay;
    }
    
    void dspexpr_19_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_20_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_18_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_17_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_16_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_22_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_15_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_02_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_02_prev = this->slide_tilde_02_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_02_prev;
            __slide_tilde_02_prev = __slide_tilde_02_prev + ((x[(Index)i] > __slide_tilde_02_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_02_prev;
        }
    
        this->slide_tilde_02_prev = __slide_tilde_02_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_03_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_03_s_prev <= thresh && trig > thresh)
            this->sah_tilde_03_s_value = x;
    
        this->sah_tilde_03_s_prev = trig;
        return this->sah_tilde_03_s_value;
    }
    
    void sah_tilde_03_s_reset() {
        this->sah_tilde_03_s_prev = 0.;
        this->sah_tilde_03_s_value = 0.;
    }
    
    number sah_tilde_04_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_04_s_prev <= thresh && trig > thresh)
            this->sah_tilde_04_s_value = x;
    
        this->sah_tilde_04_s_prev = trig;
        return this->sah_tilde_04_s_value;
    }
    
    void sah_tilde_04_s_reset() {
        this->sah_tilde_04_s_prev = 0.;
        this->sah_tilde_04_s_value = 0.;
    }
    
    void delaytilde_02_del_step() {
        this->delaytilde_02_del_reader++;
    
        if (this->delaytilde_02_del_reader >= (int)(this->delaytilde_02_del_buffer->getSize()))
            this->delaytilde_02_del_reader = 0;
    }
    
    number delaytilde_02_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? 1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? 1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_02_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_02_del_wrap))
        );
    }
    
    void delaytilde_02_del_write(number v) {
        this->delaytilde_02_del_writer = this->delaytilde_02_del_reader;
        this->delaytilde_02_del_buffer[(Index)this->delaytilde_02_del_writer] = v;
    }
    
    number delaytilde_02_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_02_del__maxdelay : size);
        number val = this->delaytilde_02_del_read(effectiveSize, 0);
        this->delaytilde_02_del_write(v);
        this->delaytilde_02_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_02_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_02_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_02_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_02_del_init() {
        auto result = this->delaytilde_02_del_calcSizeInSamples();
        this->delaytilde_02_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_02_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_02_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_02_del_clear() {
        this->delaytilde_02_del_buffer->setZero();
    }
    
    void delaytilde_02_del_reset() {
        auto result = this->delaytilde_02_del_calcSizeInSamples();
        this->delaytilde_02_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_02_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_02_del_buffer);
        this->delaytilde_02_del_wrap = this->delaytilde_02_del_buffer->getSize() - 1;
        this->delaytilde_02_del_clear();
    
        if (this->delaytilde_02_del_reader >= this->delaytilde_02_del__maxdelay || this->delaytilde_02_del_writer >= this->delaytilde_02_del__maxdelay) {
            this->delaytilde_02_del_reader = 0;
            this->delaytilde_02_del_writer = 0;
        }
    }
    
    void delaytilde_02_del_dspsetup() {
        this->delaytilde_02_del_reset();
    }
    
    number delaytilde_02_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_02_del_size() {
        return this->delaytilde_02_del__maxdelay;
    }
    
    void delaytilde_02_dspsetup(bool force) {
        if ((bool)(this->delaytilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_02_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_02_lastDelay = -1;
        this->delaytilde_02_setupDone = true;
        this->delaytilde_02_del_dspsetup();
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
        sah_tilde_03_input = 0;
        sah_tilde_03_trig = -1;
        sah_tilde_03_thresh = 0;
        dspexpr_14_in1 = 0;
        dspexpr_14_in2 = 0;
        slide_tilde_02_x = 0;
        slide_tilde_02_up = 1000;
        slide_tilde_02_down = 1000;
        dspexpr_15_in1 = 0;
        dspexpr_15_in2 = 0;
        dspexpr_16_in1 = 0;
        dspexpr_16_in2 = 0;
        dspexpr_17_in1 = 0;
        dspexpr_17_in2 = 512;
        dspexpr_18_in1 = 0;
        dspexpr_18_in2 = 48000;
        dspexpr_19_in1 = 0;
        dspexpr_19_in2 = 0;
        dspexpr_20_in1 = 0;
        sah_tilde_04_input = 0;
        sah_tilde_04_trig = -1;
        sah_tilde_04_thresh = 0;
        delaytilde_02_delay = 512;
        dspexpr_22_in1 = 0;
        dspexpr_22_in2 = 0;
        expr_02_in1 = 0;
        expr_02_in2 = 512;
        expr_02_out1 = 0;
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
        sah_tilde_03_s_prev = 0;
        sah_tilde_03_s_value = 0;
        slide_tilde_02_prev = 0;
        sah_tilde_04_s_prev = 0;
        sah_tilde_04_s_value = 0;
        delaytilde_02_lastDelay = -1;
        delaytilde_02_crossfadeDelay = 0;
        delaytilde_02_ramp = 0;
        delaytilde_02_rampInSamples = 0;
        delaytilde_02_del__maxdelay = 0;
        delaytilde_02_del_sizemode = 0;
        delaytilde_02_del_wrap = 0;
        delaytilde_02_del_reader = 0;
        delaytilde_02_del_writer = 0;
        delaytilde_02_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_03_input;
        number sah_tilde_03_trig;
        number sah_tilde_03_thresh;
        number dspexpr_14_in1;
        number dspexpr_14_in2;
        number slide_tilde_02_x;
        number slide_tilde_02_up;
        number slide_tilde_02_down;
        number dspexpr_15_in1;
        number dspexpr_15_in2;
        number dspexpr_16_in1;
        number dspexpr_16_in2;
        number dspexpr_17_in1;
        number dspexpr_17_in2;
        number dspexpr_18_in1;
        number dspexpr_18_in2;
        number dspexpr_19_in1;
        number dspexpr_19_in2;
        number dspexpr_20_in1;
        number sah_tilde_04_input;
        number sah_tilde_04_trig;
        number sah_tilde_04_thresh;
        number delaytilde_02_delay;
        number dspexpr_22_in1;
        number dspexpr_22_in2;
        number expr_02_in1;
        number expr_02_in2;
        number expr_02_out1;
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
        number sah_tilde_03_s_prev;
        number sah_tilde_03_s_value;
        number slide_tilde_02_prev;
        number sah_tilde_04_s_prev;
        number sah_tilde_04_s_value;
        number delaytilde_02_lastDelay;
        number delaytilde_02_crossfadeDelay;
        number delaytilde_02_ramp;
        long delaytilde_02_rampInSamples;
        Float64BufferRef delaytilde_02_del_buffer;
        Index delaytilde_02_del__maxdelay;
        Int delaytilde_02_del_sizemode;
        Index delaytilde_02_del_wrap;
        Int delaytilde_02_del_reader;
        Int delaytilde_02_del_writer;
        bool delaytilde_02_setupDone;
        number stackprotect_count;
        DataRef delaytilde_02_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1203 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1203()
    {
    }
    
    ~RNBOSubpatcher_1203()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_03_perform(in1, this->signals[0], n);
        this->dspexpr_24_perform(this->signals[0], this->dspexpr_24_in2, this->signals[1], n);
        this->dspexpr_23_perform(this->signals[1], this->dspexpr_23_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_03_innerScala_init();
        this->ftom_tilde_03_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_03_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_03_base = this->ftom_tilde_03_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_03_innerFtoM_next(frequency[(Index)i], __ftom_tilde_03_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_24_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_23_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_03_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_03_innerFtoM_lastInValue && tuning == this->ftom_tilde_03_innerFtoM_lastTuning) {
            return this->ftom_tilde_03_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_03_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_03_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_03_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_03_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_03_innerFtoM_reset() {
        this->ftom_tilde_03_innerFtoM_lastInValue = 0;
        this->ftom_tilde_03_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_03_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_03_innerScala_mid(int v) {
        this->ftom_tilde_03_innerScala_kbmMid = v;
        this->ftom_tilde_03_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_03_innerScala_ref(int v) {
        this->ftom_tilde_03_innerScala_kbmRefNum = v;
        this->ftom_tilde_03_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_03_innerScala_base(number v) {
        this->ftom_tilde_03_innerScala_kbmRefFreq = v;
        this->ftom_tilde_03_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_03_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_03_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_03_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_03_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_03_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_03_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_03_innerScala_lastValid) && this->ftom_tilde_03_innerScala_lastNote == note) {
            return this->ftom_tilde_03_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_03_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_03_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_03_innerScala_refFreq);
        }
    
        this->ftom_tilde_03_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_03_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_03_innerScala_lastValid) && this->ftom_tilde_03_innerScala_lastFreq == hz) {
            return this->ftom_tilde_03_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_03_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_03_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_03_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_03_innerScala_octdegree(degree, this->ftom_tilde_03_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_03_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_03_innerScala_kbmValid[(Index)(i + this->ftom_tilde_03_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_03_innerScala_kbmSize + entry + this->ftom_tilde_03_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_03_innerScala_updateLast(out, hz);
        return this->ftom_tilde_03_innerScala_lastNote;
    }
    
    int ftom_tilde_03_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_03_innerScala_lastValid = false;
            this->ftom_tilde_03_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_03_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_03_innerScala_sclOctaveMul = last;
            this->ftom_tilde_03_innerScala_sclEntryCount = (int)(this->ftom_tilde_03_innerScala_sclExpMul->length);
            this->ftom_tilde_03_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_03_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_03_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_03_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_03_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_03_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_03_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_03_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_03_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_03_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_03_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_03_innerScala_kbmValid = kbm;
            this->ftom_tilde_03_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_03_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_03_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_03_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_03_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_03_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_03_innerScala_lastValid = true;
        this->ftom_tilde_03_innerScala_lastNote = note;
        this->ftom_tilde_03_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_03_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_03_innerScala_refFreq) / this->ftom_tilde_03_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_03_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_03_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_03_innerScala_refFreq);
            n = this->ftom_tilde_03_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_03_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_03_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_03_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_03_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_03_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_03_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_03_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_03_innerScala_kbmMin == this->ftom_tilde_03_innerScala_kbmMax && this->ftom_tilde_03_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_03_innerScala_kbmMin && note <= this->ftom_tilde_03_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_03_innerScala_kbmMid));
    
            if (this->ftom_tilde_03_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_03_innerScala_octdegree(degree, this->ftom_tilde_03_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_03_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_03_innerScala_kbmValid[(Index)(this->ftom_tilde_03_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_03_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_03_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_03_innerScala_octdegree(degree, this->ftom_tilde_03_innerScala_sclEntryCount);
        return this->ftom_tilde_03_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_03_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_03_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_03_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_03_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_03_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_03_innerScala_updateRefFreq() {
        this->ftom_tilde_03_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_03_innerScala_kbmRefNum - this->ftom_tilde_03_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_03_innerScala_refFreq = this->ftom_tilde_03_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_03_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_03_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_03_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_03_innerScala_kbmSize) {
                if (index < this->ftom_tilde_03_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_03_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_03_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_03_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_03_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_03_innerScala_refFreq = this->ftom_tilde_03_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_03_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_03_innerScala_reset() {
        this->ftom_tilde_03_innerScala_internal = true;
        this->ftom_tilde_03_innerScala_lastValid = false;
        this->ftom_tilde_03_innerScala_lastNote = 0;
        this->ftom_tilde_03_innerScala_lastFreq = 0;
        this->ftom_tilde_03_innerScala_sclEntryCount = 0;
        this->ftom_tilde_03_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_03_innerScala_sclExpMul = {};
        this->ftom_tilde_03_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_03_innerScala_kbmMid = 60;
        this->ftom_tilde_03_innerScala_kbmRefNum = 69;
        this->ftom_tilde_03_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_03_innerScala_kbmSize = 0;
        this->ftom_tilde_03_innerScala_kbmMin = 0;
        this->ftom_tilde_03_innerScala_kbmMax = 0;
        this->ftom_tilde_03_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_03_innerScala_kbmMapSize = 0;
        this->ftom_tilde_03_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_03_init() {
        this->ftom_tilde_03_innerScala_update(this->ftom_tilde_03_scale, this->ftom_tilde_03_map);
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
        dspexpr_23_in2 = 60;
        dspexpr_24_in1 = 0;
        dspexpr_24_in2 = 60;
        ftom_tilde_03_frequency = 0;
        ftom_tilde_03_base = 440;
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
        ftom_tilde_03_innerFtoM_lastInValue = 0;
        ftom_tilde_03_innerFtoM_lastOutValue = 0;
        ftom_tilde_03_innerFtoM_lastTuning = 0;
        ftom_tilde_03_innerScala_internal = true;
        ftom_tilde_03_innerScala_lastValid = false;
        ftom_tilde_03_innerScala_lastNote = 0;
        ftom_tilde_03_innerScala_lastFreq = 0;
        ftom_tilde_03_innerScala_sclEntryCount = 0;
        ftom_tilde_03_innerScala_sclOctaveMul = 1;
        ftom_tilde_03_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_03_innerScala_kbmMid = 60;
        ftom_tilde_03_innerScala_kbmRefNum = 69;
        ftom_tilde_03_innerScala_kbmRefFreq = 440;
        ftom_tilde_03_innerScala_kbmSize = 0;
        ftom_tilde_03_innerScala_kbmMin = 0;
        ftom_tilde_03_innerScala_kbmMax = 0;
        ftom_tilde_03_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_03_innerScala_kbmMapSize = 0;
        ftom_tilde_03_innerScala_refFreq = 261.63;
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
        number ftom_tilde_03_frequency;
        list ftom_tilde_03_scale;
        list ftom_tilde_03_map;
        number ftom_tilde_03_base;
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
        number ftom_tilde_03_innerFtoM_lastInValue;
        number ftom_tilde_03_innerFtoM_lastOutValue;
        number ftom_tilde_03_innerFtoM_lastTuning;
        bool ftom_tilde_03_innerScala_internal;
        const Index ftom_tilde_03_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_03_innerScala_lastValid;
        number ftom_tilde_03_innerScala_lastNote;
        number ftom_tilde_03_innerScala_lastFreq;
        int ftom_tilde_03_innerScala_sclEntryCount;
        number ftom_tilde_03_innerScala_sclOctaveMul;
        list ftom_tilde_03_innerScala_sclExpMul;
        list ftom_tilde_03_innerScala_kbmValid;
        int ftom_tilde_03_innerScala_kbmMid;
        int ftom_tilde_03_innerScala_kbmRefNum;
        number ftom_tilde_03_innerScala_kbmRefFreq;
        int ftom_tilde_03_innerScala_kbmSize;
        int ftom_tilde_03_innerScala_kbmMin;
        int ftom_tilde_03_innerScala_kbmMax;
        int ftom_tilde_03_innerScala_kbmOctaveDegree;
        Index ftom_tilde_03_innerScala_kbmMapSize;
        number ftom_tilde_03_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1204 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1204()
    {
    }
    
    ~RNBOSubpatcher_1204()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_32_perform(this->signals[0], n);
        this->dspexpr_25_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_05_perform(in1, this->signals[1], this->sah_tilde_05_thresh, out1, n);
        this->sah_tilde_06_perform(in2, this->signals[1], this->sah_tilde_06_thresh, this->signals[2], n);
        this->delaytilde_03_perform(this->delaytilde_03_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_30_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_31_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_29_perform(this->signals[1], this->dspexpr_29_in2, this->signals[3], n);
        this->dspexpr_28_perform(this->signals[3], this->dspexpr_28_in2, this->signals[1], n);
        this->dspexpr_27_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_33_perform(in4, this->dspexpr_33_in2, this->signals[0], n);
        this->dspexpr_26_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_03_perform(
            this->signals[1],
            this->slide_tilde_03_up,
            this->slide_tilde_03_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_03_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_03_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_03_del_buffer = new Float64Buffer(this->delaytilde_03_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_03_del_bufferobj = initDataRef("delaytilde_03_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_03_del_bufferobj->setIndex(1);
        this->delaytilde_03_del_buffer = new Float64Buffer(this->delaytilde_03_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_03_out1_bang_bang() {
        this->expr_03_in1_bang();
    }
    
    void eventinlet_03_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_03_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_03_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_29_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_03_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_03_del_buffer = this->delaytilde_03_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_03_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_03_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_03_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_33_in2_set(number v) {
        this->dspexpr_33_in2 = v;
    }
    
    void expr_03_out1_set(number v) {
        this->expr_03_out1 = v;
        this->dspexpr_33_in2_set(this->expr_03_out1);
    }
    
    void expr_03_in1_set(number in1) {
        this->expr_03_in1 = in1;
    
        this->expr_03_out1_set(
            (this->expr_03_in2 == 0 ? 0 : (this->expr_03_in2 == 0. ? 0. : this->expr_03_in1 / this->expr_03_in2))
        );//#map:calcgainpitch[2]//_obj-64:1
    }
    
    void dspexpr_29_in2_set(number v) {
        this->dspexpr_29_in2 = v;
    }
    
    void eventinlet_03_out1_number_set(number v) {
        this->expr_03_in1_set(v);
        this->dspexpr_29_in2_set(v);
    }
    
    void expr_03_in1_bang() {
        this->expr_03_out1_set(
            (this->expr_03_in2 == 0 ? 0 : (this->expr_03_in2 == 0. ? 0. : this->expr_03_in1 / this->expr_03_in2))
        );//#map:calcgainpitch[2]//_obj-64:1
    }
    
    void expr_03_in2_set(number v) {
        this->expr_03_in2 = v;
    }
    
    void dspexpr_32_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_25_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_05_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_05_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_06_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_06_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_03_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_03_crossfadeDelay = this->delaytilde_03_crossfadeDelay;
        auto __delaytilde_03_rampInSamples = this->delaytilde_03_rampInSamples;
        auto __delaytilde_03_ramp = this->delaytilde_03_ramp;
        auto __delaytilde_03_lastDelay = this->delaytilde_03_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_03_lastDelay == -1) {
                __delaytilde_03_lastDelay = 512;
            }
    
            if (__delaytilde_03_ramp > 0) {
                number factor = __delaytilde_03_ramp / __delaytilde_03_rampInSamples;
                output[(Index)i] = this->delaytilde_03_del_read(__delaytilde_03_crossfadeDelay, 0) * factor + this->delaytilde_03_del_read(__delaytilde_03_lastDelay, 0) * (1. - factor);
                __delaytilde_03_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_03_lastDelay) {
                    __delaytilde_03_ramp = __delaytilde_03_rampInSamples;
                    __delaytilde_03_crossfadeDelay = __delaytilde_03_lastDelay;
                    __delaytilde_03_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_03_del_read(__delaytilde_03_crossfadeDelay, 0);
                    __delaytilde_03_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_03_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_03_del_write(input[(Index)i]);
            this->delaytilde_03_del_step();
        }
    
        this->delaytilde_03_lastDelay = __delaytilde_03_lastDelay;
        this->delaytilde_03_ramp = __delaytilde_03_ramp;
        this->delaytilde_03_crossfadeDelay = __delaytilde_03_crossfadeDelay;
    }
    
    void dspexpr_30_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_31_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_29_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_28_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_27_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_33_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_26_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_03_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_03_prev = this->slide_tilde_03_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_03_prev;
            __slide_tilde_03_prev = __slide_tilde_03_prev + ((x[(Index)i] > __slide_tilde_03_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_03_prev;
        }
    
        this->slide_tilde_03_prev = __slide_tilde_03_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_05_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_05_s_prev <= thresh && trig > thresh)
            this->sah_tilde_05_s_value = x;
    
        this->sah_tilde_05_s_prev = trig;
        return this->sah_tilde_05_s_value;
    }
    
    void sah_tilde_05_s_reset() {
        this->sah_tilde_05_s_prev = 0.;
        this->sah_tilde_05_s_value = 0.;
    }
    
    number sah_tilde_06_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_06_s_prev <= thresh && trig > thresh)
            this->sah_tilde_06_s_value = x;
    
        this->sah_tilde_06_s_prev = trig;
        return this->sah_tilde_06_s_value;
    }
    
    void sah_tilde_06_s_reset() {
        this->sah_tilde_06_s_prev = 0.;
        this->sah_tilde_06_s_value = 0.;
    }
    
    void delaytilde_03_del_step() {
        this->delaytilde_03_del_reader++;
    
        if (this->delaytilde_03_del_reader >= (int)(this->delaytilde_03_del_buffer->getSize()))
            this->delaytilde_03_del_reader = 0;
    }
    
    number delaytilde_03_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_03_del_buffer->getSize()) + this->delaytilde_03_del_reader - ((size > this->delaytilde_03_del__maxdelay ? this->delaytilde_03_del__maxdelay : (size < (this->delaytilde_03_del_reader != this->delaytilde_03_del_writer) ? this->delaytilde_03_del_reader != this->delaytilde_03_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_03_del_buffer->getSize()) + this->delaytilde_03_del_reader - ((size > this->delaytilde_03_del__maxdelay ? this->delaytilde_03_del__maxdelay : (size < (1 + this->delaytilde_03_del_reader != this->delaytilde_03_del_writer) ? 1 + this->delaytilde_03_del_reader != this->delaytilde_03_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_03_del_buffer->getSize()) + this->delaytilde_03_del_reader - ((size > this->delaytilde_03_del__maxdelay ? this->delaytilde_03_del__maxdelay : (size < (1 + this->delaytilde_03_del_reader != this->delaytilde_03_del_writer) ? 1 + this->delaytilde_03_del_reader != this->delaytilde_03_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_03_del_buffer->getSize()) + this->delaytilde_03_del_reader - ((size > this->delaytilde_03_del__maxdelay ? this->delaytilde_03_del__maxdelay : (size < (this->delaytilde_03_del_reader != this->delaytilde_03_del_writer) ? this->delaytilde_03_del_reader != this->delaytilde_03_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ), this->delaytilde_03_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_03_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_03_del_buffer->getSize()) + this->delaytilde_03_del_reader - ((size > this->delaytilde_03_del__maxdelay ? this->delaytilde_03_del__maxdelay : (size < (this->delaytilde_03_del_reader != this->delaytilde_03_del_writer) ? this->delaytilde_03_del_reader != this->delaytilde_03_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_03_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_03_del_wrap))
        );
    }
    
    void delaytilde_03_del_write(number v) {
        this->delaytilde_03_del_writer = this->delaytilde_03_del_reader;
        this->delaytilde_03_del_buffer[(Index)this->delaytilde_03_del_writer] = v;
    }
    
    number delaytilde_03_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_03_del__maxdelay : size);
        number val = this->delaytilde_03_del_read(effectiveSize, 0);
        this->delaytilde_03_del_write(v);
        this->delaytilde_03_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_03_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_03_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_03_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_03_del_init() {
        auto result = this->delaytilde_03_del_calcSizeInSamples();
        this->delaytilde_03_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_03_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_03_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_03_del_clear() {
        this->delaytilde_03_del_buffer->setZero();
    }
    
    void delaytilde_03_del_reset() {
        auto result = this->delaytilde_03_del_calcSizeInSamples();
        this->delaytilde_03_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_03_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_03_del_buffer);
        this->delaytilde_03_del_wrap = this->delaytilde_03_del_buffer->getSize() - 1;
        this->delaytilde_03_del_clear();
    
        if (this->delaytilde_03_del_reader >= this->delaytilde_03_del__maxdelay || this->delaytilde_03_del_writer >= this->delaytilde_03_del__maxdelay) {
            this->delaytilde_03_del_reader = 0;
            this->delaytilde_03_del_writer = 0;
        }
    }
    
    void delaytilde_03_del_dspsetup() {
        this->delaytilde_03_del_reset();
    }
    
    number delaytilde_03_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_03_del_size() {
        return this->delaytilde_03_del__maxdelay;
    }
    
    void delaytilde_03_dspsetup(bool force) {
        if ((bool)(this->delaytilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_03_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_03_lastDelay = -1;
        this->delaytilde_03_setupDone = true;
        this->delaytilde_03_del_dspsetup();
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
        sah_tilde_05_input = 0;
        sah_tilde_05_trig = -1;
        sah_tilde_05_thresh = 0;
        dspexpr_25_in1 = 0;
        dspexpr_25_in2 = 0;
        slide_tilde_03_x = 0;
        slide_tilde_03_up = 1000;
        slide_tilde_03_down = 1000;
        dspexpr_26_in1 = 0;
        dspexpr_26_in2 = 0;
        dspexpr_27_in1 = 0;
        dspexpr_27_in2 = 0;
        dspexpr_28_in1 = 0;
        dspexpr_28_in2 = 512;
        dspexpr_29_in1 = 0;
        dspexpr_29_in2 = 48000;
        dspexpr_30_in1 = 0;
        dspexpr_30_in2 = 0;
        dspexpr_31_in1 = 0;
        sah_tilde_06_input = 0;
        sah_tilde_06_trig = -1;
        sah_tilde_06_thresh = 0;
        delaytilde_03_delay = 512;
        dspexpr_33_in1 = 0;
        dspexpr_33_in2 = 0;
        expr_03_in1 = 0;
        expr_03_in2 = 512;
        expr_03_out1 = 0;
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
        sah_tilde_05_s_prev = 0;
        sah_tilde_05_s_value = 0;
        slide_tilde_03_prev = 0;
        sah_tilde_06_s_prev = 0;
        sah_tilde_06_s_value = 0;
        delaytilde_03_lastDelay = -1;
        delaytilde_03_crossfadeDelay = 0;
        delaytilde_03_ramp = 0;
        delaytilde_03_rampInSamples = 0;
        delaytilde_03_del__maxdelay = 0;
        delaytilde_03_del_sizemode = 0;
        delaytilde_03_del_wrap = 0;
        delaytilde_03_del_reader = 0;
        delaytilde_03_del_writer = 0;
        delaytilde_03_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_05_input;
        number sah_tilde_05_trig;
        number sah_tilde_05_thresh;
        number dspexpr_25_in1;
        number dspexpr_25_in2;
        number slide_tilde_03_x;
        number slide_tilde_03_up;
        number slide_tilde_03_down;
        number dspexpr_26_in1;
        number dspexpr_26_in2;
        number dspexpr_27_in1;
        number dspexpr_27_in2;
        number dspexpr_28_in1;
        number dspexpr_28_in2;
        number dspexpr_29_in1;
        number dspexpr_29_in2;
        number dspexpr_30_in1;
        number dspexpr_30_in2;
        number dspexpr_31_in1;
        number sah_tilde_06_input;
        number sah_tilde_06_trig;
        number sah_tilde_06_thresh;
        number delaytilde_03_delay;
        number dspexpr_33_in1;
        number dspexpr_33_in2;
        number expr_03_in1;
        number expr_03_in2;
        number expr_03_out1;
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
        number sah_tilde_05_s_prev;
        number sah_tilde_05_s_value;
        number slide_tilde_03_prev;
        number sah_tilde_06_s_prev;
        number sah_tilde_06_s_value;
        number delaytilde_03_lastDelay;
        number delaytilde_03_crossfadeDelay;
        number delaytilde_03_ramp;
        long delaytilde_03_rampInSamples;
        Float64BufferRef delaytilde_03_del_buffer;
        Index delaytilde_03_del__maxdelay;
        Int delaytilde_03_del_sizemode;
        Index delaytilde_03_del_wrap;
        Int delaytilde_03_del_reader;
        Int delaytilde_03_del_writer;
        bool delaytilde_03_setupDone;
        number stackprotect_count;
        DataRef delaytilde_03_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1205 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1205()
    {
    }
    
    ~RNBOSubpatcher_1205()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_04_perform(in1, this->signals[0], n);
        this->dspexpr_35_perform(this->signals[0], this->dspexpr_35_in2, this->signals[1], n);
        this->dspexpr_34_perform(this->signals[1], this->dspexpr_34_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_04_innerScala_init();
        this->ftom_tilde_04_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_04_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_04_base = this->ftom_tilde_04_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_04_innerFtoM_next(frequency[(Index)i], __ftom_tilde_04_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_35_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_34_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_04_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_04_innerFtoM_lastInValue && tuning == this->ftom_tilde_04_innerFtoM_lastTuning) {
            return this->ftom_tilde_04_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_04_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_04_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_04_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_04_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_04_innerFtoM_reset() {
        this->ftom_tilde_04_innerFtoM_lastInValue = 0;
        this->ftom_tilde_04_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_04_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_04_innerScala_mid(int v) {
        this->ftom_tilde_04_innerScala_kbmMid = v;
        this->ftom_tilde_04_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_04_innerScala_ref(int v) {
        this->ftom_tilde_04_innerScala_kbmRefNum = v;
        this->ftom_tilde_04_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_04_innerScala_base(number v) {
        this->ftom_tilde_04_innerScala_kbmRefFreq = v;
        this->ftom_tilde_04_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_04_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_04_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_04_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_04_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_04_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_04_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_04_innerScala_lastValid) && this->ftom_tilde_04_innerScala_lastNote == note) {
            return this->ftom_tilde_04_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_04_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_04_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_04_innerScala_refFreq);
        }
    
        this->ftom_tilde_04_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_04_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_04_innerScala_lastValid) && this->ftom_tilde_04_innerScala_lastFreq == hz) {
            return this->ftom_tilde_04_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_04_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_04_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_04_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_04_innerScala_octdegree(degree, this->ftom_tilde_04_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_04_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_04_innerScala_kbmValid[(Index)(i + this->ftom_tilde_04_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_04_innerScala_kbmSize + entry + this->ftom_tilde_04_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_04_innerScala_updateLast(out, hz);
        return this->ftom_tilde_04_innerScala_lastNote;
    }
    
    int ftom_tilde_04_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_04_innerScala_lastValid = false;
            this->ftom_tilde_04_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_04_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_04_innerScala_sclOctaveMul = last;
            this->ftom_tilde_04_innerScala_sclEntryCount = (int)(this->ftom_tilde_04_innerScala_sclExpMul->length);
            this->ftom_tilde_04_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_04_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_04_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_04_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_04_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_04_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_04_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_04_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_04_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_04_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_04_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_04_innerScala_kbmValid = kbm;
            this->ftom_tilde_04_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_04_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_04_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_04_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_04_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_04_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_04_innerScala_lastValid = true;
        this->ftom_tilde_04_innerScala_lastNote = note;
        this->ftom_tilde_04_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_04_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_04_innerScala_refFreq) / this->ftom_tilde_04_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_04_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_04_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_04_innerScala_refFreq);
            n = this->ftom_tilde_04_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_04_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_04_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_04_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_04_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_04_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_04_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_04_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_04_innerScala_kbmMin == this->ftom_tilde_04_innerScala_kbmMax && this->ftom_tilde_04_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_04_innerScala_kbmMin && note <= this->ftom_tilde_04_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_04_innerScala_kbmMid));
    
            if (this->ftom_tilde_04_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_04_innerScala_octdegree(degree, this->ftom_tilde_04_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_04_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_04_innerScala_kbmValid[(Index)(this->ftom_tilde_04_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_04_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_04_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_04_innerScala_octdegree(degree, this->ftom_tilde_04_innerScala_sclEntryCount);
        return this->ftom_tilde_04_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_04_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_04_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_04_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_04_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_04_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_04_innerScala_updateRefFreq() {
        this->ftom_tilde_04_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_04_innerScala_kbmRefNum - this->ftom_tilde_04_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_04_innerScala_refFreq = this->ftom_tilde_04_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_04_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_04_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_04_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_04_innerScala_kbmSize) {
                if (index < this->ftom_tilde_04_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_04_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_04_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_04_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_04_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_04_innerScala_refFreq = this->ftom_tilde_04_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_04_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_04_innerScala_reset() {
        this->ftom_tilde_04_innerScala_internal = true;
        this->ftom_tilde_04_innerScala_lastValid = false;
        this->ftom_tilde_04_innerScala_lastNote = 0;
        this->ftom_tilde_04_innerScala_lastFreq = 0;
        this->ftom_tilde_04_innerScala_sclEntryCount = 0;
        this->ftom_tilde_04_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_04_innerScala_sclExpMul = {};
        this->ftom_tilde_04_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_04_innerScala_kbmMid = 60;
        this->ftom_tilde_04_innerScala_kbmRefNum = 69;
        this->ftom_tilde_04_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_04_innerScala_kbmSize = 0;
        this->ftom_tilde_04_innerScala_kbmMin = 0;
        this->ftom_tilde_04_innerScala_kbmMax = 0;
        this->ftom_tilde_04_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_04_innerScala_kbmMapSize = 0;
        this->ftom_tilde_04_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_04_init() {
        this->ftom_tilde_04_innerScala_update(this->ftom_tilde_04_scale, this->ftom_tilde_04_map);
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
        dspexpr_34_in2 = 60;
        dspexpr_35_in1 = 0;
        dspexpr_35_in2 = 60;
        ftom_tilde_04_frequency = 0;
        ftom_tilde_04_base = 440;
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
        ftom_tilde_04_innerFtoM_lastInValue = 0;
        ftom_tilde_04_innerFtoM_lastOutValue = 0;
        ftom_tilde_04_innerFtoM_lastTuning = 0;
        ftom_tilde_04_innerScala_internal = true;
        ftom_tilde_04_innerScala_lastValid = false;
        ftom_tilde_04_innerScala_lastNote = 0;
        ftom_tilde_04_innerScala_lastFreq = 0;
        ftom_tilde_04_innerScala_sclEntryCount = 0;
        ftom_tilde_04_innerScala_sclOctaveMul = 1;
        ftom_tilde_04_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_04_innerScala_kbmMid = 60;
        ftom_tilde_04_innerScala_kbmRefNum = 69;
        ftom_tilde_04_innerScala_kbmRefFreq = 440;
        ftom_tilde_04_innerScala_kbmSize = 0;
        ftom_tilde_04_innerScala_kbmMin = 0;
        ftom_tilde_04_innerScala_kbmMax = 0;
        ftom_tilde_04_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_04_innerScala_kbmMapSize = 0;
        ftom_tilde_04_innerScala_refFreq = 261.63;
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
        number ftom_tilde_04_frequency;
        list ftom_tilde_04_scale;
        list ftom_tilde_04_map;
        number ftom_tilde_04_base;
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
        number ftom_tilde_04_innerFtoM_lastInValue;
        number ftom_tilde_04_innerFtoM_lastOutValue;
        number ftom_tilde_04_innerFtoM_lastTuning;
        bool ftom_tilde_04_innerScala_internal;
        const Index ftom_tilde_04_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_04_innerScala_lastValid;
        number ftom_tilde_04_innerScala_lastNote;
        number ftom_tilde_04_innerScala_lastFreq;
        int ftom_tilde_04_innerScala_sclEntryCount;
        number ftom_tilde_04_innerScala_sclOctaveMul;
        list ftom_tilde_04_innerScala_sclExpMul;
        list ftom_tilde_04_innerScala_kbmValid;
        int ftom_tilde_04_innerScala_kbmMid;
        int ftom_tilde_04_innerScala_kbmRefNum;
        number ftom_tilde_04_innerScala_kbmRefFreq;
        int ftom_tilde_04_innerScala_kbmSize;
        int ftom_tilde_04_innerScala_kbmMin;
        int ftom_tilde_04_innerScala_kbmMax;
        int ftom_tilde_04_innerScala_kbmOctaveDegree;
        Index ftom_tilde_04_innerScala_kbmMapSize;
        number ftom_tilde_04_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1206 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1206()
    {
    }
    
    ~RNBOSubpatcher_1206()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_43_perform(this->signals[0], n);
        this->dspexpr_36_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_07_perform(in1, this->signals[1], this->sah_tilde_07_thresh, out1, n);
        this->sah_tilde_08_perform(in2, this->signals[1], this->sah_tilde_08_thresh, this->signals[2], n);
        this->delaytilde_04_perform(this->delaytilde_04_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_41_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_42_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_40_perform(this->signals[1], this->dspexpr_40_in2, this->signals[3], n);
        this->dspexpr_39_perform(this->signals[3], this->dspexpr_39_in2, this->signals[1], n);
        this->dspexpr_38_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_44_perform(in4, this->dspexpr_44_in2, this->signals[0], n);
        this->dspexpr_37_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_04_perform(
            this->signals[1],
            this->slide_tilde_04_up,
            this->slide_tilde_04_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_04_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_04_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_04_del_buffer = new Float64Buffer(this->delaytilde_04_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_04_del_bufferobj = initDataRef("delaytilde_04_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_04_del_bufferobj->setIndex(1);
        this->delaytilde_04_del_buffer = new Float64Buffer(this->delaytilde_04_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_04_out1_bang_bang() {
        this->expr_04_in1_bang();
    }
    
    void eventinlet_04_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_04_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_04_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_40_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_04_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_04_del_buffer = this->delaytilde_04_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_04_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_04_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_04_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_44_in2_set(number v) {
        this->dspexpr_44_in2 = v;
    }
    
    void expr_04_out1_set(number v) {
        this->expr_04_out1 = v;
        this->dspexpr_44_in2_set(this->expr_04_out1);
    }
    
    void expr_04_in1_set(number in1) {
        this->expr_04_in1 = in1;
    
        this->expr_04_out1_set(
            (this->expr_04_in2 == 0 ? 0 : (this->expr_04_in2 == 0. ? 0. : this->expr_04_in1 / this->expr_04_in2))
        );//#map:calcgainpitch[3]//_obj-64:1
    }
    
    void dspexpr_40_in2_set(number v) {
        this->dspexpr_40_in2 = v;
    }
    
    void eventinlet_04_out1_number_set(number v) {
        this->expr_04_in1_set(v);
        this->dspexpr_40_in2_set(v);
    }
    
    void expr_04_in1_bang() {
        this->expr_04_out1_set(
            (this->expr_04_in2 == 0 ? 0 : (this->expr_04_in2 == 0. ? 0. : this->expr_04_in1 / this->expr_04_in2))
        );//#map:calcgainpitch[3]//_obj-64:1
    }
    
    void expr_04_in2_set(number v) {
        this->expr_04_in2 = v;
    }
    
    void dspexpr_43_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_36_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_07_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_07_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_08_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_08_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_04_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_04_crossfadeDelay = this->delaytilde_04_crossfadeDelay;
        auto __delaytilde_04_rampInSamples = this->delaytilde_04_rampInSamples;
        auto __delaytilde_04_ramp = this->delaytilde_04_ramp;
        auto __delaytilde_04_lastDelay = this->delaytilde_04_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_04_lastDelay == -1) {
                __delaytilde_04_lastDelay = 512;
            }
    
            if (__delaytilde_04_ramp > 0) {
                number factor = __delaytilde_04_ramp / __delaytilde_04_rampInSamples;
                output[(Index)i] = this->delaytilde_04_del_read(__delaytilde_04_crossfadeDelay, 0) * factor + this->delaytilde_04_del_read(__delaytilde_04_lastDelay, 0) * (1. - factor);
                __delaytilde_04_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_04_lastDelay) {
                    __delaytilde_04_ramp = __delaytilde_04_rampInSamples;
                    __delaytilde_04_crossfadeDelay = __delaytilde_04_lastDelay;
                    __delaytilde_04_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_04_del_read(__delaytilde_04_crossfadeDelay, 0);
                    __delaytilde_04_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_04_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_04_del_write(input[(Index)i]);
            this->delaytilde_04_del_step();
        }
    
        this->delaytilde_04_lastDelay = __delaytilde_04_lastDelay;
        this->delaytilde_04_ramp = __delaytilde_04_ramp;
        this->delaytilde_04_crossfadeDelay = __delaytilde_04_crossfadeDelay;
    }
    
    void dspexpr_41_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_42_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_40_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_39_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_38_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_44_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_37_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_04_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_04_prev = this->slide_tilde_04_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_04_prev;
            __slide_tilde_04_prev = __slide_tilde_04_prev + ((x[(Index)i] > __slide_tilde_04_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_04_prev;
        }
    
        this->slide_tilde_04_prev = __slide_tilde_04_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_07_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_07_s_prev <= thresh && trig > thresh)
            this->sah_tilde_07_s_value = x;
    
        this->sah_tilde_07_s_prev = trig;
        return this->sah_tilde_07_s_value;
    }
    
    void sah_tilde_07_s_reset() {
        this->sah_tilde_07_s_prev = 0.;
        this->sah_tilde_07_s_value = 0.;
    }
    
    number sah_tilde_08_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_08_s_prev <= thresh && trig > thresh)
            this->sah_tilde_08_s_value = x;
    
        this->sah_tilde_08_s_prev = trig;
        return this->sah_tilde_08_s_value;
    }
    
    void sah_tilde_08_s_reset() {
        this->sah_tilde_08_s_prev = 0.;
        this->sah_tilde_08_s_value = 0.;
    }
    
    void delaytilde_04_del_step() {
        this->delaytilde_04_del_reader++;
    
        if (this->delaytilde_04_del_reader >= (int)(this->delaytilde_04_del_buffer->getSize()))
            this->delaytilde_04_del_reader = 0;
    }
    
    number delaytilde_04_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_04_del_buffer->getSize()) + this->delaytilde_04_del_reader - ((size > this->delaytilde_04_del__maxdelay ? this->delaytilde_04_del__maxdelay : (size < (this->delaytilde_04_del_reader != this->delaytilde_04_del_writer) ? this->delaytilde_04_del_reader != this->delaytilde_04_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_04_del_buffer->getSize()) + this->delaytilde_04_del_reader - ((size > this->delaytilde_04_del__maxdelay ? this->delaytilde_04_del__maxdelay : (size < (1 + this->delaytilde_04_del_reader != this->delaytilde_04_del_writer) ? 1 + this->delaytilde_04_del_reader != this->delaytilde_04_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_04_del_buffer->getSize()) + this->delaytilde_04_del_reader - ((size > this->delaytilde_04_del__maxdelay ? this->delaytilde_04_del__maxdelay : (size < (1 + this->delaytilde_04_del_reader != this->delaytilde_04_del_writer) ? 1 + this->delaytilde_04_del_reader != this->delaytilde_04_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_04_del_buffer->getSize()) + this->delaytilde_04_del_reader - ((size > this->delaytilde_04_del__maxdelay ? this->delaytilde_04_del__maxdelay : (size < (this->delaytilde_04_del_reader != this->delaytilde_04_del_writer) ? this->delaytilde_04_del_reader != this->delaytilde_04_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ), this->delaytilde_04_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_04_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_04_del_buffer->getSize()) + this->delaytilde_04_del_reader - ((size > this->delaytilde_04_del__maxdelay ? this->delaytilde_04_del__maxdelay : (size < (this->delaytilde_04_del_reader != this->delaytilde_04_del_writer) ? this->delaytilde_04_del_reader != this->delaytilde_04_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_04_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_04_del_wrap))
        );
    }
    
    void delaytilde_04_del_write(number v) {
        this->delaytilde_04_del_writer = this->delaytilde_04_del_reader;
        this->delaytilde_04_del_buffer[(Index)this->delaytilde_04_del_writer] = v;
    }
    
    number delaytilde_04_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_04_del__maxdelay : size);
        number val = this->delaytilde_04_del_read(effectiveSize, 0);
        this->delaytilde_04_del_write(v);
        this->delaytilde_04_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_04_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_04_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_04_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_04_del_init() {
        auto result = this->delaytilde_04_del_calcSizeInSamples();
        this->delaytilde_04_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_04_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_04_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_04_del_clear() {
        this->delaytilde_04_del_buffer->setZero();
    }
    
    void delaytilde_04_del_reset() {
        auto result = this->delaytilde_04_del_calcSizeInSamples();
        this->delaytilde_04_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_04_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_04_del_buffer);
        this->delaytilde_04_del_wrap = this->delaytilde_04_del_buffer->getSize() - 1;
        this->delaytilde_04_del_clear();
    
        if (this->delaytilde_04_del_reader >= this->delaytilde_04_del__maxdelay || this->delaytilde_04_del_writer >= this->delaytilde_04_del__maxdelay) {
            this->delaytilde_04_del_reader = 0;
            this->delaytilde_04_del_writer = 0;
        }
    }
    
    void delaytilde_04_del_dspsetup() {
        this->delaytilde_04_del_reset();
    }
    
    number delaytilde_04_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_04_del_size() {
        return this->delaytilde_04_del__maxdelay;
    }
    
    void delaytilde_04_dspsetup(bool force) {
        if ((bool)(this->delaytilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_04_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_04_lastDelay = -1;
        this->delaytilde_04_setupDone = true;
        this->delaytilde_04_del_dspsetup();
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
        sah_tilde_07_input = 0;
        sah_tilde_07_trig = -1;
        sah_tilde_07_thresh = 0;
        dspexpr_36_in1 = 0;
        dspexpr_36_in2 = 0;
        slide_tilde_04_x = 0;
        slide_tilde_04_up = 1000;
        slide_tilde_04_down = 1000;
        dspexpr_37_in1 = 0;
        dspexpr_37_in2 = 0;
        dspexpr_38_in1 = 0;
        dspexpr_38_in2 = 0;
        dspexpr_39_in1 = 0;
        dspexpr_39_in2 = 512;
        dspexpr_40_in1 = 0;
        dspexpr_40_in2 = 48000;
        dspexpr_41_in1 = 0;
        dspexpr_41_in2 = 0;
        dspexpr_42_in1 = 0;
        sah_tilde_08_input = 0;
        sah_tilde_08_trig = -1;
        sah_tilde_08_thresh = 0;
        delaytilde_04_delay = 512;
        dspexpr_44_in1 = 0;
        dspexpr_44_in2 = 0;
        expr_04_in1 = 0;
        expr_04_in2 = 512;
        expr_04_out1 = 0;
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
        sah_tilde_07_s_prev = 0;
        sah_tilde_07_s_value = 0;
        slide_tilde_04_prev = 0;
        sah_tilde_08_s_prev = 0;
        sah_tilde_08_s_value = 0;
        delaytilde_04_lastDelay = -1;
        delaytilde_04_crossfadeDelay = 0;
        delaytilde_04_ramp = 0;
        delaytilde_04_rampInSamples = 0;
        delaytilde_04_del__maxdelay = 0;
        delaytilde_04_del_sizemode = 0;
        delaytilde_04_del_wrap = 0;
        delaytilde_04_del_reader = 0;
        delaytilde_04_del_writer = 0;
        delaytilde_04_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_07_input;
        number sah_tilde_07_trig;
        number sah_tilde_07_thresh;
        number dspexpr_36_in1;
        number dspexpr_36_in2;
        number slide_tilde_04_x;
        number slide_tilde_04_up;
        number slide_tilde_04_down;
        number dspexpr_37_in1;
        number dspexpr_37_in2;
        number dspexpr_38_in1;
        number dspexpr_38_in2;
        number dspexpr_39_in1;
        number dspexpr_39_in2;
        number dspexpr_40_in1;
        number dspexpr_40_in2;
        number dspexpr_41_in1;
        number dspexpr_41_in2;
        number dspexpr_42_in1;
        number sah_tilde_08_input;
        number sah_tilde_08_trig;
        number sah_tilde_08_thresh;
        number delaytilde_04_delay;
        number dspexpr_44_in1;
        number dspexpr_44_in2;
        number expr_04_in1;
        number expr_04_in2;
        number expr_04_out1;
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
        number sah_tilde_07_s_prev;
        number sah_tilde_07_s_value;
        number slide_tilde_04_prev;
        number sah_tilde_08_s_prev;
        number sah_tilde_08_s_value;
        number delaytilde_04_lastDelay;
        number delaytilde_04_crossfadeDelay;
        number delaytilde_04_ramp;
        long delaytilde_04_rampInSamples;
        Float64BufferRef delaytilde_04_del_buffer;
        Index delaytilde_04_del__maxdelay;
        Int delaytilde_04_del_sizemode;
        Index delaytilde_04_del_wrap;
        Int delaytilde_04_del_reader;
        Int delaytilde_04_del_writer;
        bool delaytilde_04_setupDone;
        number stackprotect_count;
        DataRef delaytilde_04_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1207 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1207()
    {
    }
    
    ~RNBOSubpatcher_1207()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_05_perform(in1, this->signals[0], n);
        this->dspexpr_46_perform(this->signals[0], this->dspexpr_46_in2, this->signals[1], n);
        this->dspexpr_45_perform(this->signals[1], this->dspexpr_45_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_05_innerScala_init();
        this->ftom_tilde_05_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_05_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_05_base = this->ftom_tilde_05_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_05_innerFtoM_next(frequency[(Index)i], __ftom_tilde_05_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_46_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_45_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_05_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_05_innerFtoM_lastInValue && tuning == this->ftom_tilde_05_innerFtoM_lastTuning) {
            return this->ftom_tilde_05_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_05_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_05_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_05_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_05_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_05_innerFtoM_reset() {
        this->ftom_tilde_05_innerFtoM_lastInValue = 0;
        this->ftom_tilde_05_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_05_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_05_innerScala_mid(int v) {
        this->ftom_tilde_05_innerScala_kbmMid = v;
        this->ftom_tilde_05_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_05_innerScala_ref(int v) {
        this->ftom_tilde_05_innerScala_kbmRefNum = v;
        this->ftom_tilde_05_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_05_innerScala_base(number v) {
        this->ftom_tilde_05_innerScala_kbmRefFreq = v;
        this->ftom_tilde_05_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_05_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_05_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_05_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_05_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_05_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_05_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_05_innerScala_lastValid) && this->ftom_tilde_05_innerScala_lastNote == note) {
            return this->ftom_tilde_05_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_05_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_05_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_05_innerScala_refFreq);
        }
    
        this->ftom_tilde_05_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_05_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_05_innerScala_lastValid) && this->ftom_tilde_05_innerScala_lastFreq == hz) {
            return this->ftom_tilde_05_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_05_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_05_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_05_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_05_innerScala_octdegree(degree, this->ftom_tilde_05_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_05_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_05_innerScala_kbmValid[(Index)(i + this->ftom_tilde_05_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_05_innerScala_kbmSize + entry + this->ftom_tilde_05_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_05_innerScala_updateLast(out, hz);
        return this->ftom_tilde_05_innerScala_lastNote;
    }
    
    int ftom_tilde_05_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_05_innerScala_lastValid = false;
            this->ftom_tilde_05_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_05_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_05_innerScala_sclOctaveMul = last;
            this->ftom_tilde_05_innerScala_sclEntryCount = (int)(this->ftom_tilde_05_innerScala_sclExpMul->length);
            this->ftom_tilde_05_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_05_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_05_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_05_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_05_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_05_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_05_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_05_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_05_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_05_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_05_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_05_innerScala_kbmValid = kbm;
            this->ftom_tilde_05_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_05_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_05_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_05_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_05_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_05_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_05_innerScala_lastValid = true;
        this->ftom_tilde_05_innerScala_lastNote = note;
        this->ftom_tilde_05_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_05_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_05_innerScala_refFreq) / this->ftom_tilde_05_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_05_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_05_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_05_innerScala_refFreq);
            n = this->ftom_tilde_05_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_05_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_05_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_05_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_05_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_05_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_05_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_05_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_05_innerScala_kbmMin == this->ftom_tilde_05_innerScala_kbmMax && this->ftom_tilde_05_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_05_innerScala_kbmMin && note <= this->ftom_tilde_05_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_05_innerScala_kbmMid));
    
            if (this->ftom_tilde_05_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_05_innerScala_octdegree(degree, this->ftom_tilde_05_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_05_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_05_innerScala_kbmValid[(Index)(this->ftom_tilde_05_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_05_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_05_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_05_innerScala_octdegree(degree, this->ftom_tilde_05_innerScala_sclEntryCount);
        return this->ftom_tilde_05_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_05_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_05_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_05_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_05_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_05_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_05_innerScala_updateRefFreq() {
        this->ftom_tilde_05_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_05_innerScala_kbmRefNum - this->ftom_tilde_05_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_05_innerScala_refFreq = this->ftom_tilde_05_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_05_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_05_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_05_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_05_innerScala_kbmSize) {
                if (index < this->ftom_tilde_05_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_05_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_05_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_05_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_05_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_05_innerScala_refFreq = this->ftom_tilde_05_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_05_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_05_innerScala_reset() {
        this->ftom_tilde_05_innerScala_internal = true;
        this->ftom_tilde_05_innerScala_lastValid = false;
        this->ftom_tilde_05_innerScala_lastNote = 0;
        this->ftom_tilde_05_innerScala_lastFreq = 0;
        this->ftom_tilde_05_innerScala_sclEntryCount = 0;
        this->ftom_tilde_05_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_05_innerScala_sclExpMul = {};
        this->ftom_tilde_05_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_05_innerScala_kbmMid = 60;
        this->ftom_tilde_05_innerScala_kbmRefNum = 69;
        this->ftom_tilde_05_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_05_innerScala_kbmSize = 0;
        this->ftom_tilde_05_innerScala_kbmMin = 0;
        this->ftom_tilde_05_innerScala_kbmMax = 0;
        this->ftom_tilde_05_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_05_innerScala_kbmMapSize = 0;
        this->ftom_tilde_05_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_05_init() {
        this->ftom_tilde_05_innerScala_update(this->ftom_tilde_05_scale, this->ftom_tilde_05_map);
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
        dspexpr_45_in2 = 60;
        dspexpr_46_in1 = 0;
        dspexpr_46_in2 = 60;
        ftom_tilde_05_frequency = 0;
        ftom_tilde_05_base = 440;
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
        ftom_tilde_05_innerFtoM_lastInValue = 0;
        ftom_tilde_05_innerFtoM_lastOutValue = 0;
        ftom_tilde_05_innerFtoM_lastTuning = 0;
        ftom_tilde_05_innerScala_internal = true;
        ftom_tilde_05_innerScala_lastValid = false;
        ftom_tilde_05_innerScala_lastNote = 0;
        ftom_tilde_05_innerScala_lastFreq = 0;
        ftom_tilde_05_innerScala_sclEntryCount = 0;
        ftom_tilde_05_innerScala_sclOctaveMul = 1;
        ftom_tilde_05_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_05_innerScala_kbmMid = 60;
        ftom_tilde_05_innerScala_kbmRefNum = 69;
        ftom_tilde_05_innerScala_kbmRefFreq = 440;
        ftom_tilde_05_innerScala_kbmSize = 0;
        ftom_tilde_05_innerScala_kbmMin = 0;
        ftom_tilde_05_innerScala_kbmMax = 0;
        ftom_tilde_05_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_05_innerScala_kbmMapSize = 0;
        ftom_tilde_05_innerScala_refFreq = 261.63;
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
        number ftom_tilde_05_frequency;
        list ftom_tilde_05_scale;
        list ftom_tilde_05_map;
        number ftom_tilde_05_base;
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
        number ftom_tilde_05_innerFtoM_lastInValue;
        number ftom_tilde_05_innerFtoM_lastOutValue;
        number ftom_tilde_05_innerFtoM_lastTuning;
        bool ftom_tilde_05_innerScala_internal;
        const Index ftom_tilde_05_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_05_innerScala_lastValid;
        number ftom_tilde_05_innerScala_lastNote;
        number ftom_tilde_05_innerScala_lastFreq;
        int ftom_tilde_05_innerScala_sclEntryCount;
        number ftom_tilde_05_innerScala_sclOctaveMul;
        list ftom_tilde_05_innerScala_sclExpMul;
        list ftom_tilde_05_innerScala_kbmValid;
        int ftom_tilde_05_innerScala_kbmMid;
        int ftom_tilde_05_innerScala_kbmRefNum;
        number ftom_tilde_05_innerScala_kbmRefFreq;
        int ftom_tilde_05_innerScala_kbmSize;
        int ftom_tilde_05_innerScala_kbmMin;
        int ftom_tilde_05_innerScala_kbmMax;
        int ftom_tilde_05_innerScala_kbmOctaveDegree;
        Index ftom_tilde_05_innerScala_kbmMapSize;
        number ftom_tilde_05_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1208 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1208()
    {
    }
    
    ~RNBOSubpatcher_1208()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_54_perform(this->signals[0], n);
        this->dspexpr_47_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_09_perform(in1, this->signals[1], this->sah_tilde_09_thresh, out1, n);
        this->sah_tilde_10_perform(in2, this->signals[1], this->sah_tilde_10_thresh, this->signals[2], n);
        this->delaytilde_05_perform(this->delaytilde_05_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_52_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_53_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_51_perform(this->signals[1], this->dspexpr_51_in2, this->signals[3], n);
        this->dspexpr_50_perform(this->signals[3], this->dspexpr_50_in2, this->signals[1], n);
        this->dspexpr_49_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_55_perform(in4, this->dspexpr_55_in2, this->signals[0], n);
        this->dspexpr_48_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_05_perform(
            this->signals[1],
            this->slide_tilde_05_up,
            this->slide_tilde_05_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_05_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_05_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_05_del_buffer = new Float64Buffer(this->delaytilde_05_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_05_del_bufferobj = initDataRef("delaytilde_05_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_05_del_bufferobj->setIndex(1);
        this->delaytilde_05_del_buffer = new Float64Buffer(this->delaytilde_05_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_05_out1_bang_bang() {
        this->expr_05_in1_bang();
    }
    
    void eventinlet_05_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_05_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_05_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_51_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_05_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_05_del_buffer = this->delaytilde_05_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_05_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_05_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_05_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_55_in2_set(number v) {
        this->dspexpr_55_in2 = v;
    }
    
    void expr_05_out1_set(number v) {
        this->expr_05_out1 = v;
        this->dspexpr_55_in2_set(this->expr_05_out1);
    }
    
    void expr_05_in1_set(number in1) {
        this->expr_05_in1 = in1;
    
        this->expr_05_out1_set(
            (this->expr_05_in2 == 0 ? 0 : (this->expr_05_in2 == 0. ? 0. : this->expr_05_in1 / this->expr_05_in2))
        );//#map:calcgainpitch[4]//_obj-64:1
    }
    
    void dspexpr_51_in2_set(number v) {
        this->dspexpr_51_in2 = v;
    }
    
    void eventinlet_05_out1_number_set(number v) {
        this->expr_05_in1_set(v);
        this->dspexpr_51_in2_set(v);
    }
    
    void expr_05_in1_bang() {
        this->expr_05_out1_set(
            (this->expr_05_in2 == 0 ? 0 : (this->expr_05_in2 == 0. ? 0. : this->expr_05_in1 / this->expr_05_in2))
        );//#map:calcgainpitch[4]//_obj-64:1
    }
    
    void expr_05_in2_set(number v) {
        this->expr_05_in2 = v;
    }
    
    void dspexpr_54_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_47_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_09_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_09_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_10_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_10_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_05_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_05_crossfadeDelay = this->delaytilde_05_crossfadeDelay;
        auto __delaytilde_05_rampInSamples = this->delaytilde_05_rampInSamples;
        auto __delaytilde_05_ramp = this->delaytilde_05_ramp;
        auto __delaytilde_05_lastDelay = this->delaytilde_05_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_05_lastDelay == -1) {
                __delaytilde_05_lastDelay = 512;
            }
    
            if (__delaytilde_05_ramp > 0) {
                number factor = __delaytilde_05_ramp / __delaytilde_05_rampInSamples;
                output[(Index)i] = this->delaytilde_05_del_read(__delaytilde_05_crossfadeDelay, 0) * factor + this->delaytilde_05_del_read(__delaytilde_05_lastDelay, 0) * (1. - factor);
                __delaytilde_05_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_05_lastDelay) {
                    __delaytilde_05_ramp = __delaytilde_05_rampInSamples;
                    __delaytilde_05_crossfadeDelay = __delaytilde_05_lastDelay;
                    __delaytilde_05_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_05_del_read(__delaytilde_05_crossfadeDelay, 0);
                    __delaytilde_05_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_05_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_05_del_write(input[(Index)i]);
            this->delaytilde_05_del_step();
        }
    
        this->delaytilde_05_lastDelay = __delaytilde_05_lastDelay;
        this->delaytilde_05_ramp = __delaytilde_05_ramp;
        this->delaytilde_05_crossfadeDelay = __delaytilde_05_crossfadeDelay;
    }
    
    void dspexpr_52_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_53_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_51_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_50_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_49_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_55_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_48_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_05_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_05_prev = this->slide_tilde_05_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_05_prev;
            __slide_tilde_05_prev = __slide_tilde_05_prev + ((x[(Index)i] > __slide_tilde_05_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_05_prev;
        }
    
        this->slide_tilde_05_prev = __slide_tilde_05_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_09_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_09_s_prev <= thresh && trig > thresh)
            this->sah_tilde_09_s_value = x;
    
        this->sah_tilde_09_s_prev = trig;
        return this->sah_tilde_09_s_value;
    }
    
    void sah_tilde_09_s_reset() {
        this->sah_tilde_09_s_prev = 0.;
        this->sah_tilde_09_s_value = 0.;
    }
    
    number sah_tilde_10_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_10_s_prev <= thresh && trig > thresh)
            this->sah_tilde_10_s_value = x;
    
        this->sah_tilde_10_s_prev = trig;
        return this->sah_tilde_10_s_value;
    }
    
    void sah_tilde_10_s_reset() {
        this->sah_tilde_10_s_prev = 0.;
        this->sah_tilde_10_s_value = 0.;
    }
    
    void delaytilde_05_del_step() {
        this->delaytilde_05_del_reader++;
    
        if (this->delaytilde_05_del_reader >= (int)(this->delaytilde_05_del_buffer->getSize()))
            this->delaytilde_05_del_reader = 0;
    }
    
    number delaytilde_05_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_05_del_buffer->getSize()) + this->delaytilde_05_del_reader - ((size > this->delaytilde_05_del__maxdelay ? this->delaytilde_05_del__maxdelay : (size < (this->delaytilde_05_del_reader != this->delaytilde_05_del_writer) ? this->delaytilde_05_del_reader != this->delaytilde_05_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_05_del_buffer->getSize()) + this->delaytilde_05_del_reader - ((size > this->delaytilde_05_del__maxdelay ? this->delaytilde_05_del__maxdelay : (size < (1 + this->delaytilde_05_del_reader != this->delaytilde_05_del_writer) ? 1 + this->delaytilde_05_del_reader != this->delaytilde_05_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_05_del_buffer->getSize()) + this->delaytilde_05_del_reader - ((size > this->delaytilde_05_del__maxdelay ? this->delaytilde_05_del__maxdelay : (size < (1 + this->delaytilde_05_del_reader != this->delaytilde_05_del_writer) ? 1 + this->delaytilde_05_del_reader != this->delaytilde_05_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_05_del_buffer->getSize()) + this->delaytilde_05_del_reader - ((size > this->delaytilde_05_del__maxdelay ? this->delaytilde_05_del__maxdelay : (size < (this->delaytilde_05_del_reader != this->delaytilde_05_del_writer) ? this->delaytilde_05_del_reader != this->delaytilde_05_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ), this->delaytilde_05_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_05_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_05_del_buffer->getSize()) + this->delaytilde_05_del_reader - ((size > this->delaytilde_05_del__maxdelay ? this->delaytilde_05_del__maxdelay : (size < (this->delaytilde_05_del_reader != this->delaytilde_05_del_writer) ? this->delaytilde_05_del_reader != this->delaytilde_05_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_05_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_05_del_wrap))
        );
    }
    
    void delaytilde_05_del_write(number v) {
        this->delaytilde_05_del_writer = this->delaytilde_05_del_reader;
        this->delaytilde_05_del_buffer[(Index)this->delaytilde_05_del_writer] = v;
    }
    
    number delaytilde_05_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_05_del__maxdelay : size);
        number val = this->delaytilde_05_del_read(effectiveSize, 0);
        this->delaytilde_05_del_write(v);
        this->delaytilde_05_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_05_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_05_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_05_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_05_del_init() {
        auto result = this->delaytilde_05_del_calcSizeInSamples();
        this->delaytilde_05_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_05_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_05_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_05_del_clear() {
        this->delaytilde_05_del_buffer->setZero();
    }
    
    void delaytilde_05_del_reset() {
        auto result = this->delaytilde_05_del_calcSizeInSamples();
        this->delaytilde_05_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_05_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_05_del_buffer);
        this->delaytilde_05_del_wrap = this->delaytilde_05_del_buffer->getSize() - 1;
        this->delaytilde_05_del_clear();
    
        if (this->delaytilde_05_del_reader >= this->delaytilde_05_del__maxdelay || this->delaytilde_05_del_writer >= this->delaytilde_05_del__maxdelay) {
            this->delaytilde_05_del_reader = 0;
            this->delaytilde_05_del_writer = 0;
        }
    }
    
    void delaytilde_05_del_dspsetup() {
        this->delaytilde_05_del_reset();
    }
    
    number delaytilde_05_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_05_del_size() {
        return this->delaytilde_05_del__maxdelay;
    }
    
    void delaytilde_05_dspsetup(bool force) {
        if ((bool)(this->delaytilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_05_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_05_lastDelay = -1;
        this->delaytilde_05_setupDone = true;
        this->delaytilde_05_del_dspsetup();
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
        sah_tilde_09_input = 0;
        sah_tilde_09_trig = -1;
        sah_tilde_09_thresh = 0;
        dspexpr_47_in1 = 0;
        dspexpr_47_in2 = 0;
        slide_tilde_05_x = 0;
        slide_tilde_05_up = 1000;
        slide_tilde_05_down = 1000;
        dspexpr_48_in1 = 0;
        dspexpr_48_in2 = 0;
        dspexpr_49_in1 = 0;
        dspexpr_49_in2 = 0;
        dspexpr_50_in1 = 0;
        dspexpr_50_in2 = 512;
        dspexpr_51_in1 = 0;
        dspexpr_51_in2 = 48000;
        dspexpr_52_in1 = 0;
        dspexpr_52_in2 = 0;
        dspexpr_53_in1 = 0;
        sah_tilde_10_input = 0;
        sah_tilde_10_trig = -1;
        sah_tilde_10_thresh = 0;
        delaytilde_05_delay = 512;
        dspexpr_55_in1 = 0;
        dspexpr_55_in2 = 0;
        expr_05_in1 = 0;
        expr_05_in2 = 512;
        expr_05_out1 = 0;
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
        sah_tilde_09_s_prev = 0;
        sah_tilde_09_s_value = 0;
        slide_tilde_05_prev = 0;
        sah_tilde_10_s_prev = 0;
        sah_tilde_10_s_value = 0;
        delaytilde_05_lastDelay = -1;
        delaytilde_05_crossfadeDelay = 0;
        delaytilde_05_ramp = 0;
        delaytilde_05_rampInSamples = 0;
        delaytilde_05_del__maxdelay = 0;
        delaytilde_05_del_sizemode = 0;
        delaytilde_05_del_wrap = 0;
        delaytilde_05_del_reader = 0;
        delaytilde_05_del_writer = 0;
        delaytilde_05_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_09_input;
        number sah_tilde_09_trig;
        number sah_tilde_09_thresh;
        number dspexpr_47_in1;
        number dspexpr_47_in2;
        number slide_tilde_05_x;
        number slide_tilde_05_up;
        number slide_tilde_05_down;
        number dspexpr_48_in1;
        number dspexpr_48_in2;
        number dspexpr_49_in1;
        number dspexpr_49_in2;
        number dspexpr_50_in1;
        number dspexpr_50_in2;
        number dspexpr_51_in1;
        number dspexpr_51_in2;
        number dspexpr_52_in1;
        number dspexpr_52_in2;
        number dspexpr_53_in1;
        number sah_tilde_10_input;
        number sah_tilde_10_trig;
        number sah_tilde_10_thresh;
        number delaytilde_05_delay;
        number dspexpr_55_in1;
        number dspexpr_55_in2;
        number expr_05_in1;
        number expr_05_in2;
        number expr_05_out1;
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
        number sah_tilde_09_s_prev;
        number sah_tilde_09_s_value;
        number slide_tilde_05_prev;
        number sah_tilde_10_s_prev;
        number sah_tilde_10_s_value;
        number delaytilde_05_lastDelay;
        number delaytilde_05_crossfadeDelay;
        number delaytilde_05_ramp;
        long delaytilde_05_rampInSamples;
        Float64BufferRef delaytilde_05_del_buffer;
        Index delaytilde_05_del__maxdelay;
        Int delaytilde_05_del_sizemode;
        Index delaytilde_05_del_wrap;
        Int delaytilde_05_del_reader;
        Int delaytilde_05_del_writer;
        bool delaytilde_05_setupDone;
        number stackprotect_count;
        DataRef delaytilde_05_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1209 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1209()
    {
    }
    
    ~RNBOSubpatcher_1209()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_06_perform(in1, this->signals[0], n);
        this->dspexpr_57_perform(this->signals[0], this->dspexpr_57_in2, this->signals[1], n);
        this->dspexpr_56_perform(this->signals[1], this->dspexpr_56_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_06_innerScala_init();
        this->ftom_tilde_06_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_06_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_06_base = this->ftom_tilde_06_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_06_innerFtoM_next(frequency[(Index)i], __ftom_tilde_06_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_57_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_56_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_06_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_06_innerFtoM_lastInValue && tuning == this->ftom_tilde_06_innerFtoM_lastTuning) {
            return this->ftom_tilde_06_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_06_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_06_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_06_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_06_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_06_innerFtoM_reset() {
        this->ftom_tilde_06_innerFtoM_lastInValue = 0;
        this->ftom_tilde_06_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_06_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_06_innerScala_mid(int v) {
        this->ftom_tilde_06_innerScala_kbmMid = v;
        this->ftom_tilde_06_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_06_innerScala_ref(int v) {
        this->ftom_tilde_06_innerScala_kbmRefNum = v;
        this->ftom_tilde_06_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_06_innerScala_base(number v) {
        this->ftom_tilde_06_innerScala_kbmRefFreq = v;
        this->ftom_tilde_06_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_06_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_06_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_06_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_06_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_06_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_06_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_06_innerScala_lastValid) && this->ftom_tilde_06_innerScala_lastNote == note) {
            return this->ftom_tilde_06_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_06_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_06_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_06_innerScala_refFreq);
        }
    
        this->ftom_tilde_06_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_06_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_06_innerScala_lastValid) && this->ftom_tilde_06_innerScala_lastFreq == hz) {
            return this->ftom_tilde_06_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_06_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_06_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_06_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_06_innerScala_octdegree(degree, this->ftom_tilde_06_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_06_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_06_innerScala_kbmValid[(Index)(i + this->ftom_tilde_06_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_06_innerScala_kbmSize + entry + this->ftom_tilde_06_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_06_innerScala_updateLast(out, hz);
        return this->ftom_tilde_06_innerScala_lastNote;
    }
    
    int ftom_tilde_06_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_06_innerScala_lastValid = false;
            this->ftom_tilde_06_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_06_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_06_innerScala_sclOctaveMul = last;
            this->ftom_tilde_06_innerScala_sclEntryCount = (int)(this->ftom_tilde_06_innerScala_sclExpMul->length);
            this->ftom_tilde_06_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_06_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_06_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_06_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_06_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_06_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_06_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_06_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_06_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_06_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_06_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_06_innerScala_kbmValid = kbm;
            this->ftom_tilde_06_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_06_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_06_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_06_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_06_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_06_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_06_innerScala_lastValid = true;
        this->ftom_tilde_06_innerScala_lastNote = note;
        this->ftom_tilde_06_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_06_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_06_innerScala_refFreq) / this->ftom_tilde_06_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_06_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_06_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_06_innerScala_refFreq);
            n = this->ftom_tilde_06_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_06_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_06_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_06_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_06_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_06_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_06_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_06_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_06_innerScala_kbmMin == this->ftom_tilde_06_innerScala_kbmMax && this->ftom_tilde_06_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_06_innerScala_kbmMin && note <= this->ftom_tilde_06_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_06_innerScala_kbmMid));
    
            if (this->ftom_tilde_06_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_06_innerScala_octdegree(degree, this->ftom_tilde_06_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_06_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_06_innerScala_kbmValid[(Index)(this->ftom_tilde_06_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_06_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_06_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_06_innerScala_octdegree(degree, this->ftom_tilde_06_innerScala_sclEntryCount);
        return this->ftom_tilde_06_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_06_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_06_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_06_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_06_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_06_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_06_innerScala_updateRefFreq() {
        this->ftom_tilde_06_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_06_innerScala_kbmRefNum - this->ftom_tilde_06_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_06_innerScala_refFreq = this->ftom_tilde_06_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_06_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_06_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_06_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_06_innerScala_kbmSize) {
                if (index < this->ftom_tilde_06_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_06_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_06_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_06_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_06_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_06_innerScala_refFreq = this->ftom_tilde_06_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_06_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_06_innerScala_reset() {
        this->ftom_tilde_06_innerScala_internal = true;
        this->ftom_tilde_06_innerScala_lastValid = false;
        this->ftom_tilde_06_innerScala_lastNote = 0;
        this->ftom_tilde_06_innerScala_lastFreq = 0;
        this->ftom_tilde_06_innerScala_sclEntryCount = 0;
        this->ftom_tilde_06_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_06_innerScala_sclExpMul = {};
        this->ftom_tilde_06_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_06_innerScala_kbmMid = 60;
        this->ftom_tilde_06_innerScala_kbmRefNum = 69;
        this->ftom_tilde_06_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_06_innerScala_kbmSize = 0;
        this->ftom_tilde_06_innerScala_kbmMin = 0;
        this->ftom_tilde_06_innerScala_kbmMax = 0;
        this->ftom_tilde_06_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_06_innerScala_kbmMapSize = 0;
        this->ftom_tilde_06_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_06_init() {
        this->ftom_tilde_06_innerScala_update(this->ftom_tilde_06_scale, this->ftom_tilde_06_map);
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
        dspexpr_56_in2 = 60;
        dspexpr_57_in1 = 0;
        dspexpr_57_in2 = 60;
        ftom_tilde_06_frequency = 0;
        ftom_tilde_06_base = 440;
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
        ftom_tilde_06_innerFtoM_lastInValue = 0;
        ftom_tilde_06_innerFtoM_lastOutValue = 0;
        ftom_tilde_06_innerFtoM_lastTuning = 0;
        ftom_tilde_06_innerScala_internal = true;
        ftom_tilde_06_innerScala_lastValid = false;
        ftom_tilde_06_innerScala_lastNote = 0;
        ftom_tilde_06_innerScala_lastFreq = 0;
        ftom_tilde_06_innerScala_sclEntryCount = 0;
        ftom_tilde_06_innerScala_sclOctaveMul = 1;
        ftom_tilde_06_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_06_innerScala_kbmMid = 60;
        ftom_tilde_06_innerScala_kbmRefNum = 69;
        ftom_tilde_06_innerScala_kbmRefFreq = 440;
        ftom_tilde_06_innerScala_kbmSize = 0;
        ftom_tilde_06_innerScala_kbmMin = 0;
        ftom_tilde_06_innerScala_kbmMax = 0;
        ftom_tilde_06_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_06_innerScala_kbmMapSize = 0;
        ftom_tilde_06_innerScala_refFreq = 261.63;
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
        number ftom_tilde_06_frequency;
        list ftom_tilde_06_scale;
        list ftom_tilde_06_map;
        number ftom_tilde_06_base;
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
        number ftom_tilde_06_innerFtoM_lastInValue;
        number ftom_tilde_06_innerFtoM_lastOutValue;
        number ftom_tilde_06_innerFtoM_lastTuning;
        bool ftom_tilde_06_innerScala_internal;
        const Index ftom_tilde_06_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_06_innerScala_lastValid;
        number ftom_tilde_06_innerScala_lastNote;
        number ftom_tilde_06_innerScala_lastFreq;
        int ftom_tilde_06_innerScala_sclEntryCount;
        number ftom_tilde_06_innerScala_sclOctaveMul;
        list ftom_tilde_06_innerScala_sclExpMul;
        list ftom_tilde_06_innerScala_kbmValid;
        int ftom_tilde_06_innerScala_kbmMid;
        int ftom_tilde_06_innerScala_kbmRefNum;
        number ftom_tilde_06_innerScala_kbmRefFreq;
        int ftom_tilde_06_innerScala_kbmSize;
        int ftom_tilde_06_innerScala_kbmMin;
        int ftom_tilde_06_innerScala_kbmMax;
        int ftom_tilde_06_innerScala_kbmOctaveDegree;
        Index ftom_tilde_06_innerScala_kbmMapSize;
        number ftom_tilde_06_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1210 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1210()
    {
    }
    
    ~RNBOSubpatcher_1210()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_65_perform(this->signals[0], n);
        this->dspexpr_58_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_11_perform(in1, this->signals[1], this->sah_tilde_11_thresh, out1, n);
        this->sah_tilde_12_perform(in2, this->signals[1], this->sah_tilde_12_thresh, this->signals[2], n);
        this->delaytilde_06_perform(this->delaytilde_06_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_63_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_64_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_62_perform(this->signals[1], this->dspexpr_62_in2, this->signals[3], n);
        this->dspexpr_61_perform(this->signals[3], this->dspexpr_61_in2, this->signals[1], n);
        this->dspexpr_60_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_66_perform(in4, this->dspexpr_66_in2, this->signals[0], n);
        this->dspexpr_59_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_06_perform(
            this->signals[1],
            this->slide_tilde_06_up,
            this->slide_tilde_06_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_06_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_06_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_06_del_buffer = new Float64Buffer(this->delaytilde_06_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_06_del_bufferobj = initDataRef("delaytilde_06_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_06_del_bufferobj->setIndex(1);
        this->delaytilde_06_del_buffer = new Float64Buffer(this->delaytilde_06_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_06_out1_bang_bang() {
        this->expr_06_in1_bang();
    }
    
    void eventinlet_06_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_06_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_06_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_62_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_06_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_06_del_buffer = this->delaytilde_06_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_06_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_06_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_06_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_66_in2_set(number v) {
        this->dspexpr_66_in2 = v;
    }
    
    void expr_06_out1_set(number v) {
        this->expr_06_out1 = v;
        this->dspexpr_66_in2_set(this->expr_06_out1);
    }
    
    void expr_06_in1_set(number in1) {
        this->expr_06_in1 = in1;
    
        this->expr_06_out1_set(
            (this->expr_06_in2 == 0 ? 0 : (this->expr_06_in2 == 0. ? 0. : this->expr_06_in1 / this->expr_06_in2))
        );//#map:calcgainpitch[5]//_obj-64:1
    }
    
    void dspexpr_62_in2_set(number v) {
        this->dspexpr_62_in2 = v;
    }
    
    void eventinlet_06_out1_number_set(number v) {
        this->expr_06_in1_set(v);
        this->dspexpr_62_in2_set(v);
    }
    
    void expr_06_in1_bang() {
        this->expr_06_out1_set(
            (this->expr_06_in2 == 0 ? 0 : (this->expr_06_in2 == 0. ? 0. : this->expr_06_in1 / this->expr_06_in2))
        );//#map:calcgainpitch[5]//_obj-64:1
    }
    
    void expr_06_in2_set(number v) {
        this->expr_06_in2 = v;
    }
    
    void dspexpr_65_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_58_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_11_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_11_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_12_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_12_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_06_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_06_crossfadeDelay = this->delaytilde_06_crossfadeDelay;
        auto __delaytilde_06_rampInSamples = this->delaytilde_06_rampInSamples;
        auto __delaytilde_06_ramp = this->delaytilde_06_ramp;
        auto __delaytilde_06_lastDelay = this->delaytilde_06_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_06_lastDelay == -1) {
                __delaytilde_06_lastDelay = 512;
            }
    
            if (__delaytilde_06_ramp > 0) {
                number factor = __delaytilde_06_ramp / __delaytilde_06_rampInSamples;
                output[(Index)i] = this->delaytilde_06_del_read(__delaytilde_06_crossfadeDelay, 0) * factor + this->delaytilde_06_del_read(__delaytilde_06_lastDelay, 0) * (1. - factor);
                __delaytilde_06_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_06_lastDelay) {
                    __delaytilde_06_ramp = __delaytilde_06_rampInSamples;
                    __delaytilde_06_crossfadeDelay = __delaytilde_06_lastDelay;
                    __delaytilde_06_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_06_del_read(__delaytilde_06_crossfadeDelay, 0);
                    __delaytilde_06_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_06_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_06_del_write(input[(Index)i]);
            this->delaytilde_06_del_step();
        }
    
        this->delaytilde_06_lastDelay = __delaytilde_06_lastDelay;
        this->delaytilde_06_ramp = __delaytilde_06_ramp;
        this->delaytilde_06_crossfadeDelay = __delaytilde_06_crossfadeDelay;
    }
    
    void dspexpr_63_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_64_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_62_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_61_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_60_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_66_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_59_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_06_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_06_prev = this->slide_tilde_06_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_06_prev;
            __slide_tilde_06_prev = __slide_tilde_06_prev + ((x[(Index)i] > __slide_tilde_06_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_06_prev;
        }
    
        this->slide_tilde_06_prev = __slide_tilde_06_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_11_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_11_s_prev <= thresh && trig > thresh)
            this->sah_tilde_11_s_value = x;
    
        this->sah_tilde_11_s_prev = trig;
        return this->sah_tilde_11_s_value;
    }
    
    void sah_tilde_11_s_reset() {
        this->sah_tilde_11_s_prev = 0.;
        this->sah_tilde_11_s_value = 0.;
    }
    
    number sah_tilde_12_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_12_s_prev <= thresh && trig > thresh)
            this->sah_tilde_12_s_value = x;
    
        this->sah_tilde_12_s_prev = trig;
        return this->sah_tilde_12_s_value;
    }
    
    void sah_tilde_12_s_reset() {
        this->sah_tilde_12_s_prev = 0.;
        this->sah_tilde_12_s_value = 0.;
    }
    
    void delaytilde_06_del_step() {
        this->delaytilde_06_del_reader++;
    
        if (this->delaytilde_06_del_reader >= (int)(this->delaytilde_06_del_buffer->getSize()))
            this->delaytilde_06_del_reader = 0;
    }
    
    number delaytilde_06_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_06_del_buffer->getSize()) + this->delaytilde_06_del_reader - ((size > this->delaytilde_06_del__maxdelay ? this->delaytilde_06_del__maxdelay : (size < (this->delaytilde_06_del_reader != this->delaytilde_06_del_writer) ? this->delaytilde_06_del_reader != this->delaytilde_06_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_06_del_buffer->getSize()) + this->delaytilde_06_del_reader - ((size > this->delaytilde_06_del__maxdelay ? this->delaytilde_06_del__maxdelay : (size < (1 + this->delaytilde_06_del_reader != this->delaytilde_06_del_writer) ? 1 + this->delaytilde_06_del_reader != this->delaytilde_06_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_06_del_buffer->getSize()) + this->delaytilde_06_del_reader - ((size > this->delaytilde_06_del__maxdelay ? this->delaytilde_06_del__maxdelay : (size < (1 + this->delaytilde_06_del_reader != this->delaytilde_06_del_writer) ? 1 + this->delaytilde_06_del_reader != this->delaytilde_06_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_06_del_buffer->getSize()) + this->delaytilde_06_del_reader - ((size > this->delaytilde_06_del__maxdelay ? this->delaytilde_06_del__maxdelay : (size < (this->delaytilde_06_del_reader != this->delaytilde_06_del_writer) ? this->delaytilde_06_del_reader != this->delaytilde_06_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ), this->delaytilde_06_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_06_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_06_del_buffer->getSize()) + this->delaytilde_06_del_reader - ((size > this->delaytilde_06_del__maxdelay ? this->delaytilde_06_del__maxdelay : (size < (this->delaytilde_06_del_reader != this->delaytilde_06_del_writer) ? this->delaytilde_06_del_reader != this->delaytilde_06_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_06_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_06_del_wrap))
        );
    }
    
    void delaytilde_06_del_write(number v) {
        this->delaytilde_06_del_writer = this->delaytilde_06_del_reader;
        this->delaytilde_06_del_buffer[(Index)this->delaytilde_06_del_writer] = v;
    }
    
    number delaytilde_06_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_06_del__maxdelay : size);
        number val = this->delaytilde_06_del_read(effectiveSize, 0);
        this->delaytilde_06_del_write(v);
        this->delaytilde_06_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_06_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_06_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_06_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_06_del_init() {
        auto result = this->delaytilde_06_del_calcSizeInSamples();
        this->delaytilde_06_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_06_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_06_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_06_del_clear() {
        this->delaytilde_06_del_buffer->setZero();
    }
    
    void delaytilde_06_del_reset() {
        auto result = this->delaytilde_06_del_calcSizeInSamples();
        this->delaytilde_06_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_06_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_06_del_buffer);
        this->delaytilde_06_del_wrap = this->delaytilde_06_del_buffer->getSize() - 1;
        this->delaytilde_06_del_clear();
    
        if (this->delaytilde_06_del_reader >= this->delaytilde_06_del__maxdelay || this->delaytilde_06_del_writer >= this->delaytilde_06_del__maxdelay) {
            this->delaytilde_06_del_reader = 0;
            this->delaytilde_06_del_writer = 0;
        }
    }
    
    void delaytilde_06_del_dspsetup() {
        this->delaytilde_06_del_reset();
    }
    
    number delaytilde_06_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_06_del_size() {
        return this->delaytilde_06_del__maxdelay;
    }
    
    void delaytilde_06_dspsetup(bool force) {
        if ((bool)(this->delaytilde_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_06_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_06_lastDelay = -1;
        this->delaytilde_06_setupDone = true;
        this->delaytilde_06_del_dspsetup();
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
        sah_tilde_11_input = 0;
        sah_tilde_11_trig = -1;
        sah_tilde_11_thresh = 0;
        dspexpr_58_in1 = 0;
        dspexpr_58_in2 = 0;
        slide_tilde_06_x = 0;
        slide_tilde_06_up = 1000;
        slide_tilde_06_down = 1000;
        dspexpr_59_in1 = 0;
        dspexpr_59_in2 = 0;
        dspexpr_60_in1 = 0;
        dspexpr_60_in2 = 0;
        dspexpr_61_in1 = 0;
        dspexpr_61_in2 = 512;
        dspexpr_62_in1 = 0;
        dspexpr_62_in2 = 48000;
        dspexpr_63_in1 = 0;
        dspexpr_63_in2 = 0;
        dspexpr_64_in1 = 0;
        sah_tilde_12_input = 0;
        sah_tilde_12_trig = -1;
        sah_tilde_12_thresh = 0;
        delaytilde_06_delay = 512;
        dspexpr_66_in1 = 0;
        dspexpr_66_in2 = 0;
        expr_06_in1 = 0;
        expr_06_in2 = 512;
        expr_06_out1 = 0;
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
        sah_tilde_11_s_prev = 0;
        sah_tilde_11_s_value = 0;
        slide_tilde_06_prev = 0;
        sah_tilde_12_s_prev = 0;
        sah_tilde_12_s_value = 0;
        delaytilde_06_lastDelay = -1;
        delaytilde_06_crossfadeDelay = 0;
        delaytilde_06_ramp = 0;
        delaytilde_06_rampInSamples = 0;
        delaytilde_06_del__maxdelay = 0;
        delaytilde_06_del_sizemode = 0;
        delaytilde_06_del_wrap = 0;
        delaytilde_06_del_reader = 0;
        delaytilde_06_del_writer = 0;
        delaytilde_06_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_11_input;
        number sah_tilde_11_trig;
        number sah_tilde_11_thresh;
        number dspexpr_58_in1;
        number dspexpr_58_in2;
        number slide_tilde_06_x;
        number slide_tilde_06_up;
        number slide_tilde_06_down;
        number dspexpr_59_in1;
        number dspexpr_59_in2;
        number dspexpr_60_in1;
        number dspexpr_60_in2;
        number dspexpr_61_in1;
        number dspexpr_61_in2;
        number dspexpr_62_in1;
        number dspexpr_62_in2;
        number dspexpr_63_in1;
        number dspexpr_63_in2;
        number dspexpr_64_in1;
        number sah_tilde_12_input;
        number sah_tilde_12_trig;
        number sah_tilde_12_thresh;
        number delaytilde_06_delay;
        number dspexpr_66_in1;
        number dspexpr_66_in2;
        number expr_06_in1;
        number expr_06_in2;
        number expr_06_out1;
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
        number sah_tilde_11_s_prev;
        number sah_tilde_11_s_value;
        number slide_tilde_06_prev;
        number sah_tilde_12_s_prev;
        number sah_tilde_12_s_value;
        number delaytilde_06_lastDelay;
        number delaytilde_06_crossfadeDelay;
        number delaytilde_06_ramp;
        long delaytilde_06_rampInSamples;
        Float64BufferRef delaytilde_06_del_buffer;
        Index delaytilde_06_del__maxdelay;
        Int delaytilde_06_del_sizemode;
        Index delaytilde_06_del_wrap;
        Int delaytilde_06_del_reader;
        Int delaytilde_06_del_writer;
        bool delaytilde_06_setupDone;
        number stackprotect_count;
        DataRef delaytilde_06_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1211 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1211()
    {
    }
    
    ~RNBOSubpatcher_1211()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_07_perform(in1, this->signals[0], n);
        this->dspexpr_68_perform(this->signals[0], this->dspexpr_68_in2, this->signals[1], n);
        this->dspexpr_67_perform(this->signals[1], this->dspexpr_67_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_07_innerScala_init();
        this->ftom_tilde_07_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_07_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_07_base = this->ftom_tilde_07_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_07_innerFtoM_next(frequency[(Index)i], __ftom_tilde_07_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_68_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_67_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_07_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_07_innerFtoM_lastInValue && tuning == this->ftom_tilde_07_innerFtoM_lastTuning) {
            return this->ftom_tilde_07_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_07_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_07_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_07_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_07_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_07_innerFtoM_reset() {
        this->ftom_tilde_07_innerFtoM_lastInValue = 0;
        this->ftom_tilde_07_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_07_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_07_innerScala_mid(int v) {
        this->ftom_tilde_07_innerScala_kbmMid = v;
        this->ftom_tilde_07_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_07_innerScala_ref(int v) {
        this->ftom_tilde_07_innerScala_kbmRefNum = v;
        this->ftom_tilde_07_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_07_innerScala_base(number v) {
        this->ftom_tilde_07_innerScala_kbmRefFreq = v;
        this->ftom_tilde_07_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_07_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_07_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_07_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_07_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_07_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_07_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_07_innerScala_lastValid) && this->ftom_tilde_07_innerScala_lastNote == note) {
            return this->ftom_tilde_07_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_07_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_07_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_07_innerScala_refFreq);
        }
    
        this->ftom_tilde_07_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_07_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_07_innerScala_lastValid) && this->ftom_tilde_07_innerScala_lastFreq == hz) {
            return this->ftom_tilde_07_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_07_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_07_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_07_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_07_innerScala_octdegree(degree, this->ftom_tilde_07_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_07_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_07_innerScala_kbmValid[(Index)(i + this->ftom_tilde_07_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_07_innerScala_kbmSize + entry + this->ftom_tilde_07_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_07_innerScala_updateLast(out, hz);
        return this->ftom_tilde_07_innerScala_lastNote;
    }
    
    int ftom_tilde_07_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_07_innerScala_lastValid = false;
            this->ftom_tilde_07_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_07_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_07_innerScala_sclOctaveMul = last;
            this->ftom_tilde_07_innerScala_sclEntryCount = (int)(this->ftom_tilde_07_innerScala_sclExpMul->length);
            this->ftom_tilde_07_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_07_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_07_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_07_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_07_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_07_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_07_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_07_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_07_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_07_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_07_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_07_innerScala_kbmValid = kbm;
            this->ftom_tilde_07_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_07_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_07_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_07_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_07_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_07_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_07_innerScala_lastValid = true;
        this->ftom_tilde_07_innerScala_lastNote = note;
        this->ftom_tilde_07_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_07_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_07_innerScala_refFreq) / this->ftom_tilde_07_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_07_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_07_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_07_innerScala_refFreq);
            n = this->ftom_tilde_07_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_07_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_07_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_07_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_07_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_07_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_07_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_07_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_07_innerScala_kbmMin == this->ftom_tilde_07_innerScala_kbmMax && this->ftom_tilde_07_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_07_innerScala_kbmMin && note <= this->ftom_tilde_07_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_07_innerScala_kbmMid));
    
            if (this->ftom_tilde_07_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_07_innerScala_octdegree(degree, this->ftom_tilde_07_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_07_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_07_innerScala_kbmValid[(Index)(this->ftom_tilde_07_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_07_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_07_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_07_innerScala_octdegree(degree, this->ftom_tilde_07_innerScala_sclEntryCount);
        return this->ftom_tilde_07_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_07_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_07_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_07_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_07_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_07_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_07_innerScala_updateRefFreq() {
        this->ftom_tilde_07_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_07_innerScala_kbmRefNum - this->ftom_tilde_07_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_07_innerScala_refFreq = this->ftom_tilde_07_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_07_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_07_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_07_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_07_innerScala_kbmSize) {
                if (index < this->ftom_tilde_07_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_07_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_07_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_07_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_07_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_07_innerScala_refFreq = this->ftom_tilde_07_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_07_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_07_innerScala_reset() {
        this->ftom_tilde_07_innerScala_internal = true;
        this->ftom_tilde_07_innerScala_lastValid = false;
        this->ftom_tilde_07_innerScala_lastNote = 0;
        this->ftom_tilde_07_innerScala_lastFreq = 0;
        this->ftom_tilde_07_innerScala_sclEntryCount = 0;
        this->ftom_tilde_07_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_07_innerScala_sclExpMul = {};
        this->ftom_tilde_07_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_07_innerScala_kbmMid = 60;
        this->ftom_tilde_07_innerScala_kbmRefNum = 69;
        this->ftom_tilde_07_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_07_innerScala_kbmSize = 0;
        this->ftom_tilde_07_innerScala_kbmMin = 0;
        this->ftom_tilde_07_innerScala_kbmMax = 0;
        this->ftom_tilde_07_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_07_innerScala_kbmMapSize = 0;
        this->ftom_tilde_07_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_07_init() {
        this->ftom_tilde_07_innerScala_update(this->ftom_tilde_07_scale, this->ftom_tilde_07_map);
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
        dspexpr_67_in2 = 60;
        dspexpr_68_in1 = 0;
        dspexpr_68_in2 = 60;
        ftom_tilde_07_frequency = 0;
        ftom_tilde_07_base = 440;
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
        ftom_tilde_07_innerFtoM_lastInValue = 0;
        ftom_tilde_07_innerFtoM_lastOutValue = 0;
        ftom_tilde_07_innerFtoM_lastTuning = 0;
        ftom_tilde_07_innerScala_internal = true;
        ftom_tilde_07_innerScala_lastValid = false;
        ftom_tilde_07_innerScala_lastNote = 0;
        ftom_tilde_07_innerScala_lastFreq = 0;
        ftom_tilde_07_innerScala_sclEntryCount = 0;
        ftom_tilde_07_innerScala_sclOctaveMul = 1;
        ftom_tilde_07_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_07_innerScala_kbmMid = 60;
        ftom_tilde_07_innerScala_kbmRefNum = 69;
        ftom_tilde_07_innerScala_kbmRefFreq = 440;
        ftom_tilde_07_innerScala_kbmSize = 0;
        ftom_tilde_07_innerScala_kbmMin = 0;
        ftom_tilde_07_innerScala_kbmMax = 0;
        ftom_tilde_07_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_07_innerScala_kbmMapSize = 0;
        ftom_tilde_07_innerScala_refFreq = 261.63;
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
        number ftom_tilde_07_frequency;
        list ftom_tilde_07_scale;
        list ftom_tilde_07_map;
        number ftom_tilde_07_base;
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
        number ftom_tilde_07_innerFtoM_lastInValue;
        number ftom_tilde_07_innerFtoM_lastOutValue;
        number ftom_tilde_07_innerFtoM_lastTuning;
        bool ftom_tilde_07_innerScala_internal;
        const Index ftom_tilde_07_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_07_innerScala_lastValid;
        number ftom_tilde_07_innerScala_lastNote;
        number ftom_tilde_07_innerScala_lastFreq;
        int ftom_tilde_07_innerScala_sclEntryCount;
        number ftom_tilde_07_innerScala_sclOctaveMul;
        list ftom_tilde_07_innerScala_sclExpMul;
        list ftom_tilde_07_innerScala_kbmValid;
        int ftom_tilde_07_innerScala_kbmMid;
        int ftom_tilde_07_innerScala_kbmRefNum;
        number ftom_tilde_07_innerScala_kbmRefFreq;
        int ftom_tilde_07_innerScala_kbmSize;
        int ftom_tilde_07_innerScala_kbmMin;
        int ftom_tilde_07_innerScala_kbmMax;
        int ftom_tilde_07_innerScala_kbmOctaveDegree;
        Index ftom_tilde_07_innerScala_kbmMapSize;
        number ftom_tilde_07_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1212 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1212()
    {
    }
    
    ~RNBOSubpatcher_1212()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_76_perform(this->signals[0], n);
        this->dspexpr_69_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_13_perform(in1, this->signals[1], this->sah_tilde_13_thresh, out1, n);
        this->sah_tilde_14_perform(in2, this->signals[1], this->sah_tilde_14_thresh, this->signals[2], n);
        this->delaytilde_07_perform(this->delaytilde_07_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_74_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_75_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_73_perform(this->signals[1], this->dspexpr_73_in2, this->signals[3], n);
        this->dspexpr_72_perform(this->signals[3], this->dspexpr_72_in2, this->signals[1], n);
        this->dspexpr_71_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_77_perform(in4, this->dspexpr_77_in2, this->signals[0], n);
        this->dspexpr_70_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_07_perform(
            this->signals[1],
            this->slide_tilde_07_up,
            this->slide_tilde_07_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_07_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_07_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_07_del_buffer = new Float64Buffer(this->delaytilde_07_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_07_del_bufferobj = initDataRef("delaytilde_07_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_07_del_bufferobj->setIndex(1);
        this->delaytilde_07_del_buffer = new Float64Buffer(this->delaytilde_07_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_07_out1_bang_bang() {
        this->expr_07_in1_bang();
    }
    
    void eventinlet_07_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_07_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_07_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_73_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_07_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_07_del_buffer = this->delaytilde_07_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_07_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_07_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_07_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_77_in2_set(number v) {
        this->dspexpr_77_in2 = v;
    }
    
    void expr_07_out1_set(number v) {
        this->expr_07_out1 = v;
        this->dspexpr_77_in2_set(this->expr_07_out1);
    }
    
    void expr_07_in1_set(number in1) {
        this->expr_07_in1 = in1;
    
        this->expr_07_out1_set(
            (this->expr_07_in2 == 0 ? 0 : (this->expr_07_in2 == 0. ? 0. : this->expr_07_in1 / this->expr_07_in2))
        );//#map:calcgainpitch[6]//_obj-64:1
    }
    
    void dspexpr_73_in2_set(number v) {
        this->dspexpr_73_in2 = v;
    }
    
    void eventinlet_07_out1_number_set(number v) {
        this->expr_07_in1_set(v);
        this->dspexpr_73_in2_set(v);
    }
    
    void expr_07_in1_bang() {
        this->expr_07_out1_set(
            (this->expr_07_in2 == 0 ? 0 : (this->expr_07_in2 == 0. ? 0. : this->expr_07_in1 / this->expr_07_in2))
        );//#map:calcgainpitch[6]//_obj-64:1
    }
    
    void expr_07_in2_set(number v) {
        this->expr_07_in2 = v;
    }
    
    void dspexpr_76_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_69_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_13_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_13_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_14_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_14_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_07_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_07_crossfadeDelay = this->delaytilde_07_crossfadeDelay;
        auto __delaytilde_07_rampInSamples = this->delaytilde_07_rampInSamples;
        auto __delaytilde_07_ramp = this->delaytilde_07_ramp;
        auto __delaytilde_07_lastDelay = this->delaytilde_07_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_07_lastDelay == -1) {
                __delaytilde_07_lastDelay = 512;
            }
    
            if (__delaytilde_07_ramp > 0) {
                number factor = __delaytilde_07_ramp / __delaytilde_07_rampInSamples;
                output[(Index)i] = this->delaytilde_07_del_read(__delaytilde_07_crossfadeDelay, 0) * factor + this->delaytilde_07_del_read(__delaytilde_07_lastDelay, 0) * (1. - factor);
                __delaytilde_07_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_07_lastDelay) {
                    __delaytilde_07_ramp = __delaytilde_07_rampInSamples;
                    __delaytilde_07_crossfadeDelay = __delaytilde_07_lastDelay;
                    __delaytilde_07_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_07_del_read(__delaytilde_07_crossfadeDelay, 0);
                    __delaytilde_07_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_07_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_07_del_write(input[(Index)i]);
            this->delaytilde_07_del_step();
        }
    
        this->delaytilde_07_lastDelay = __delaytilde_07_lastDelay;
        this->delaytilde_07_ramp = __delaytilde_07_ramp;
        this->delaytilde_07_crossfadeDelay = __delaytilde_07_crossfadeDelay;
    }
    
    void dspexpr_74_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_75_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_73_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_72_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_71_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_77_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_70_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_07_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_07_prev = this->slide_tilde_07_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_07_prev;
            __slide_tilde_07_prev = __slide_tilde_07_prev + ((x[(Index)i] > __slide_tilde_07_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_07_prev;
        }
    
        this->slide_tilde_07_prev = __slide_tilde_07_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_13_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_13_s_prev <= thresh && trig > thresh)
            this->sah_tilde_13_s_value = x;
    
        this->sah_tilde_13_s_prev = trig;
        return this->sah_tilde_13_s_value;
    }
    
    void sah_tilde_13_s_reset() {
        this->sah_tilde_13_s_prev = 0.;
        this->sah_tilde_13_s_value = 0.;
    }
    
    number sah_tilde_14_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_14_s_prev <= thresh && trig > thresh)
            this->sah_tilde_14_s_value = x;
    
        this->sah_tilde_14_s_prev = trig;
        return this->sah_tilde_14_s_value;
    }
    
    void sah_tilde_14_s_reset() {
        this->sah_tilde_14_s_prev = 0.;
        this->sah_tilde_14_s_value = 0.;
    }
    
    void delaytilde_07_del_step() {
        this->delaytilde_07_del_reader++;
    
        if (this->delaytilde_07_del_reader >= (int)(this->delaytilde_07_del_buffer->getSize()))
            this->delaytilde_07_del_reader = 0;
    }
    
    number delaytilde_07_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_07_del_buffer->getSize()) + this->delaytilde_07_del_reader - ((size > this->delaytilde_07_del__maxdelay ? this->delaytilde_07_del__maxdelay : (size < (this->delaytilde_07_del_reader != this->delaytilde_07_del_writer) ? this->delaytilde_07_del_reader != this->delaytilde_07_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_07_del_buffer->getSize()) + this->delaytilde_07_del_reader - ((size > this->delaytilde_07_del__maxdelay ? this->delaytilde_07_del__maxdelay : (size < (1 + this->delaytilde_07_del_reader != this->delaytilde_07_del_writer) ? 1 + this->delaytilde_07_del_reader != this->delaytilde_07_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_07_del_buffer->getSize()) + this->delaytilde_07_del_reader - ((size > this->delaytilde_07_del__maxdelay ? this->delaytilde_07_del__maxdelay : (size < (1 + this->delaytilde_07_del_reader != this->delaytilde_07_del_writer) ? 1 + this->delaytilde_07_del_reader != this->delaytilde_07_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_07_del_buffer->getSize()) + this->delaytilde_07_del_reader - ((size > this->delaytilde_07_del__maxdelay ? this->delaytilde_07_del__maxdelay : (size < (this->delaytilde_07_del_reader != this->delaytilde_07_del_writer) ? this->delaytilde_07_del_reader != this->delaytilde_07_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ), this->delaytilde_07_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_07_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_07_del_buffer->getSize()) + this->delaytilde_07_del_reader - ((size > this->delaytilde_07_del__maxdelay ? this->delaytilde_07_del__maxdelay : (size < (this->delaytilde_07_del_reader != this->delaytilde_07_del_writer) ? this->delaytilde_07_del_reader != this->delaytilde_07_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_07_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_07_del_wrap))
        );
    }
    
    void delaytilde_07_del_write(number v) {
        this->delaytilde_07_del_writer = this->delaytilde_07_del_reader;
        this->delaytilde_07_del_buffer[(Index)this->delaytilde_07_del_writer] = v;
    }
    
    number delaytilde_07_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_07_del__maxdelay : size);
        number val = this->delaytilde_07_del_read(effectiveSize, 0);
        this->delaytilde_07_del_write(v);
        this->delaytilde_07_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_07_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_07_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_07_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_07_del_init() {
        auto result = this->delaytilde_07_del_calcSizeInSamples();
        this->delaytilde_07_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_07_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_07_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_07_del_clear() {
        this->delaytilde_07_del_buffer->setZero();
    }
    
    void delaytilde_07_del_reset() {
        auto result = this->delaytilde_07_del_calcSizeInSamples();
        this->delaytilde_07_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_07_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_07_del_buffer);
        this->delaytilde_07_del_wrap = this->delaytilde_07_del_buffer->getSize() - 1;
        this->delaytilde_07_del_clear();
    
        if (this->delaytilde_07_del_reader >= this->delaytilde_07_del__maxdelay || this->delaytilde_07_del_writer >= this->delaytilde_07_del__maxdelay) {
            this->delaytilde_07_del_reader = 0;
            this->delaytilde_07_del_writer = 0;
        }
    }
    
    void delaytilde_07_del_dspsetup() {
        this->delaytilde_07_del_reset();
    }
    
    number delaytilde_07_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_07_del_size() {
        return this->delaytilde_07_del__maxdelay;
    }
    
    void delaytilde_07_dspsetup(bool force) {
        if ((bool)(this->delaytilde_07_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_07_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_07_lastDelay = -1;
        this->delaytilde_07_setupDone = true;
        this->delaytilde_07_del_dspsetup();
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
        sah_tilde_13_input = 0;
        sah_tilde_13_trig = -1;
        sah_tilde_13_thresh = 0;
        dspexpr_69_in1 = 0;
        dspexpr_69_in2 = 0;
        slide_tilde_07_x = 0;
        slide_tilde_07_up = 1000;
        slide_tilde_07_down = 1000;
        dspexpr_70_in1 = 0;
        dspexpr_70_in2 = 0;
        dspexpr_71_in1 = 0;
        dspexpr_71_in2 = 0;
        dspexpr_72_in1 = 0;
        dspexpr_72_in2 = 512;
        dspexpr_73_in1 = 0;
        dspexpr_73_in2 = 48000;
        dspexpr_74_in1 = 0;
        dspexpr_74_in2 = 0;
        dspexpr_75_in1 = 0;
        sah_tilde_14_input = 0;
        sah_tilde_14_trig = -1;
        sah_tilde_14_thresh = 0;
        delaytilde_07_delay = 512;
        dspexpr_77_in1 = 0;
        dspexpr_77_in2 = 0;
        expr_07_in1 = 0;
        expr_07_in2 = 512;
        expr_07_out1 = 0;
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
        sah_tilde_13_s_prev = 0;
        sah_tilde_13_s_value = 0;
        slide_tilde_07_prev = 0;
        sah_tilde_14_s_prev = 0;
        sah_tilde_14_s_value = 0;
        delaytilde_07_lastDelay = -1;
        delaytilde_07_crossfadeDelay = 0;
        delaytilde_07_ramp = 0;
        delaytilde_07_rampInSamples = 0;
        delaytilde_07_del__maxdelay = 0;
        delaytilde_07_del_sizemode = 0;
        delaytilde_07_del_wrap = 0;
        delaytilde_07_del_reader = 0;
        delaytilde_07_del_writer = 0;
        delaytilde_07_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_13_input;
        number sah_tilde_13_trig;
        number sah_tilde_13_thresh;
        number dspexpr_69_in1;
        number dspexpr_69_in2;
        number slide_tilde_07_x;
        number slide_tilde_07_up;
        number slide_tilde_07_down;
        number dspexpr_70_in1;
        number dspexpr_70_in2;
        number dspexpr_71_in1;
        number dspexpr_71_in2;
        number dspexpr_72_in1;
        number dspexpr_72_in2;
        number dspexpr_73_in1;
        number dspexpr_73_in2;
        number dspexpr_74_in1;
        number dspexpr_74_in2;
        number dspexpr_75_in1;
        number sah_tilde_14_input;
        number sah_tilde_14_trig;
        number sah_tilde_14_thresh;
        number delaytilde_07_delay;
        number dspexpr_77_in1;
        number dspexpr_77_in2;
        number expr_07_in1;
        number expr_07_in2;
        number expr_07_out1;
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
        number sah_tilde_13_s_prev;
        number sah_tilde_13_s_value;
        number slide_tilde_07_prev;
        number sah_tilde_14_s_prev;
        number sah_tilde_14_s_value;
        number delaytilde_07_lastDelay;
        number delaytilde_07_crossfadeDelay;
        number delaytilde_07_ramp;
        long delaytilde_07_rampInSamples;
        Float64BufferRef delaytilde_07_del_buffer;
        Index delaytilde_07_del__maxdelay;
        Int delaytilde_07_del_sizemode;
        Index delaytilde_07_del_wrap;
        Int delaytilde_07_del_reader;
        Int delaytilde_07_del_writer;
        bool delaytilde_07_setupDone;
        number stackprotect_count;
        DataRef delaytilde_07_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1213 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1213()
    {
    }
    
    ~RNBOSubpatcher_1213()
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
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
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
        this->ftom_tilde_08_perform(in1, this->signals[0], n);
        this->dspexpr_79_perform(this->signals[0], this->dspexpr_79_in2, this->signals[1], n);
        this->dspexpr_78_perform(this->signals[1], this->dspexpr_78_in2, out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
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
        return 1;
    }
    
    void initializeObjects() {
        this->ftom_tilde_08_innerScala_init();
        this->ftom_tilde_08_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void ftom_tilde_08_perform(const Sample * frequency, Sample * out, Index n) {
        auto __ftom_tilde_08_base = this->ftom_tilde_08_base;
    
        for (Index i = 0; i < n; i++) {
            number v = this->ftom_tilde_08_innerFtoM_next(frequency[(Index)i], __ftom_tilde_08_base);
            out[(Index)i] = (v == -999.0 ? 0.0 : v);
        }
    }
    
    void dspexpr_79_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - 60;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_78_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)60;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number ftom_tilde_08_innerFtoM_next(number frequency, number tuning) {
        if (frequency <= 0.0) {
            return -999;
        }
    
        if (frequency == this->ftom_tilde_08_innerFtoM_lastInValue && tuning == this->ftom_tilde_08_innerFtoM_lastTuning) {
            return this->ftom_tilde_08_innerFtoM_lastOutValue;
        }
    
        this->ftom_tilde_08_innerFtoM_lastInValue = frequency;
        this->ftom_tilde_08_innerFtoM_lastTuning = tuning;
        this->ftom_tilde_08_innerFtoM_lastOutValue = (frequency == 0 || tuning == 0 ? 0 : 69. + 17.31234050465299 * rnbo_log(frequency / tuning));
        return this->ftom_tilde_08_innerFtoM_lastOutValue;
    }
    
    void ftom_tilde_08_innerFtoM_reset() {
        this->ftom_tilde_08_innerFtoM_lastInValue = 0;
        this->ftom_tilde_08_innerFtoM_lastOutValue = 0;
        this->ftom_tilde_08_innerFtoM_lastTuning = 0;
    }
    
    void ftom_tilde_08_innerScala_mid(int v) {
        this->ftom_tilde_08_innerScala_kbmMid = v;
        this->ftom_tilde_08_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_08_innerScala_ref(int v) {
        this->ftom_tilde_08_innerScala_kbmRefNum = v;
        this->ftom_tilde_08_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_08_innerScala_base(number v) {
        this->ftom_tilde_08_innerScala_kbmRefFreq = v;
        this->ftom_tilde_08_innerScala_updateRefFreq();
    }
    
    void ftom_tilde_08_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->ftom_tilde_08_innerScala_updateScale(sclValid);
    }
    
    void ftom_tilde_08_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->ftom_tilde_08_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->ftom_tilde_08_innerScala_updateMap(map);
        }
    }
    
    number ftom_tilde_08_innerScala_mtof(number note) {
        if ((bool)(this->ftom_tilde_08_innerScala_lastValid) && this->ftom_tilde_08_innerScala_lastNote == note) {
            return this->ftom_tilde_08_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->ftom_tilde_08_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->ftom_tilde_08_innerScala_applySCL(degoct[0], fract(note), this->ftom_tilde_08_innerScala_refFreq);
        }
    
        this->ftom_tilde_08_innerScala_updateLast(note, out);
        return out;
    }
    
    number ftom_tilde_08_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->ftom_tilde_08_innerScala_lastValid) && this->ftom_tilde_08_innerScala_lastFreq == hz) {
            return this->ftom_tilde_08_innerScala_lastNote;
        }
    
        array<number, 2> df = this->ftom_tilde_08_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->ftom_tilde_08_innerScala_kbmSize == 0) {
            out = this->ftom_tilde_08_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->ftom_tilde_08_innerScala_octdegree(degree, this->ftom_tilde_08_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->ftom_tilde_08_innerScala_kbmMapSize; i++) {
                if (index == this->ftom_tilde_08_innerScala_kbmValid[(Index)(i + this->ftom_tilde_08_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->ftom_tilde_08_innerScala_kbmSize + entry + this->ftom_tilde_08_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->ftom_tilde_08_innerScala_updateLast(out, hz);
        return this->ftom_tilde_08_innerScala_lastNote;
    }
    
    int ftom_tilde_08_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->ftom_tilde_08_innerScala_lastValid = false;
            this->ftom_tilde_08_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->ftom_tilde_08_innerScala_sclExpMul->push(last);
            }
    
            this->ftom_tilde_08_innerScala_sclOctaveMul = last;
            this->ftom_tilde_08_innerScala_sclEntryCount = (int)(this->ftom_tilde_08_innerScala_sclExpMul->length);
            this->ftom_tilde_08_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int ftom_tilde_08_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->ftom_tilde_08_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->ftom_tilde_08_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->ftom_tilde_08_innerScala_kbmSize = (int)(size);
            this->ftom_tilde_08_innerScala_kbmMin = (int)(kbm[1]);
            this->ftom_tilde_08_innerScala_kbmMax = (int)(kbm[2]);
            this->ftom_tilde_08_innerScala_kbmMid = (int)(kbm[3]);
            this->ftom_tilde_08_innerScala_kbmRefNum = (int)(kbm[4]);
            this->ftom_tilde_08_innerScala_kbmRefFreq = kbm[5];
            this->ftom_tilde_08_innerScala_kbmOctaveDegree = octave;
            this->ftom_tilde_08_innerScala_kbmValid = kbm;
            this->ftom_tilde_08_innerScala_kbmMapSize = (kbm->length - this->ftom_tilde_08_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->ftom_tilde_08_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->ftom_tilde_08_innerScala_KBM_MAP_OFFSET));
            this->ftom_tilde_08_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void ftom_tilde_08_innerScala_updateLast(number note, number freq) {
        this->ftom_tilde_08_innerScala_lastValid = true;
        this->ftom_tilde_08_innerScala_lastNote = note;
        this->ftom_tilde_08_innerScala_lastFreq = freq;
    }
    
    array<number, 2> ftom_tilde_08_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->ftom_tilde_08_innerScala_refFreq) / this->ftom_tilde_08_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->ftom_tilde_08_innerScala_sclEntryCount; i++) {
            number c = this->ftom_tilde_08_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->ftom_tilde_08_innerScala_refFreq);
            n = this->ftom_tilde_08_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->ftom_tilde_08_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->ftom_tilde_08_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->ftom_tilde_08_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->ftom_tilde_08_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->ftom_tilde_08_innerScala_sclEntryCount;
        return {deg, frac};
    }
    
    array<int, 2> ftom_tilde_08_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> ftom_tilde_08_innerScala_applyKBM(number note) {
        if ((this->ftom_tilde_08_innerScala_kbmMin == this->ftom_tilde_08_innerScala_kbmMax && this->ftom_tilde_08_innerScala_kbmMax == 0) || (note >= this->ftom_tilde_08_innerScala_kbmMin && note <= this->ftom_tilde_08_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->ftom_tilde_08_innerScala_kbmMid));
    
            if (this->ftom_tilde_08_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->ftom_tilde_08_innerScala_octdegree(degree, this->ftom_tilde_08_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->ftom_tilde_08_innerScala_kbmMapSize > index) {
                degree = (int)(this->ftom_tilde_08_innerScala_kbmValid[(Index)(this->ftom_tilde_08_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->ftom_tilde_08_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number ftom_tilde_08_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->ftom_tilde_08_innerScala_octdegree(degree, this->ftom_tilde_08_innerScala_sclEntryCount);
        return this->ftom_tilde_08_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number ftom_tilde_08_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->ftom_tilde_08_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->ftom_tilde_08_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->ftom_tilde_08_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->ftom_tilde_08_innerScala_sclOctaveMul);
    }
    
    void ftom_tilde_08_innerScala_updateRefFreq() {
        this->ftom_tilde_08_innerScala_lastValid = false;
        int refOffset = (int)(this->ftom_tilde_08_innerScala_kbmRefNum - this->ftom_tilde_08_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->ftom_tilde_08_innerScala_refFreq = this->ftom_tilde_08_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->ftom_tilde_08_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->ftom_tilde_08_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->ftom_tilde_08_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->ftom_tilde_08_innerScala_kbmSize) {
                if (index < this->ftom_tilde_08_innerScala_kbmMapSize) {
                    index = (int)(this->ftom_tilde_08_innerScala_kbmValid[(Index)((Index)(index) + this->ftom_tilde_08_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->ftom_tilde_08_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->ftom_tilde_08_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->ftom_tilde_08_innerScala_refFreq = this->ftom_tilde_08_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->ftom_tilde_08_innerScala_sclOctaveMul);
            }
        }
    }
    
    void ftom_tilde_08_innerScala_reset() {
        this->ftom_tilde_08_innerScala_internal = true;
        this->ftom_tilde_08_innerScala_lastValid = false;
        this->ftom_tilde_08_innerScala_lastNote = 0;
        this->ftom_tilde_08_innerScala_lastFreq = 0;
        this->ftom_tilde_08_innerScala_sclEntryCount = 0;
        this->ftom_tilde_08_innerScala_sclOctaveMul = 1;
        this->ftom_tilde_08_innerScala_sclExpMul = {};
        this->ftom_tilde_08_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->ftom_tilde_08_innerScala_kbmMid = 60;
        this->ftom_tilde_08_innerScala_kbmRefNum = 69;
        this->ftom_tilde_08_innerScala_kbmRefFreq = 440;
        this->ftom_tilde_08_innerScala_kbmSize = 0;
        this->ftom_tilde_08_innerScala_kbmMin = 0;
        this->ftom_tilde_08_innerScala_kbmMax = 0;
        this->ftom_tilde_08_innerScala_kbmOctaveDegree = 12;
        this->ftom_tilde_08_innerScala_kbmMapSize = 0;
        this->ftom_tilde_08_innerScala_refFreq = 261.63;
    }
    
    void ftom_tilde_08_init() {
        this->ftom_tilde_08_innerScala_update(this->ftom_tilde_08_scale, this->ftom_tilde_08_map);
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
        dspexpr_78_in2 = 60;
        dspexpr_79_in1 = 0;
        dspexpr_79_in2 = 60;
        ftom_tilde_08_frequency = 0;
        ftom_tilde_08_base = 440;
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
        ftom_tilde_08_innerFtoM_lastInValue = 0;
        ftom_tilde_08_innerFtoM_lastOutValue = 0;
        ftom_tilde_08_innerFtoM_lastTuning = 0;
        ftom_tilde_08_innerScala_internal = true;
        ftom_tilde_08_innerScala_lastValid = false;
        ftom_tilde_08_innerScala_lastNote = 0;
        ftom_tilde_08_innerScala_lastFreq = 0;
        ftom_tilde_08_innerScala_sclEntryCount = 0;
        ftom_tilde_08_innerScala_sclOctaveMul = 1;
        ftom_tilde_08_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        ftom_tilde_08_innerScala_kbmMid = 60;
        ftom_tilde_08_innerScala_kbmRefNum = 69;
        ftom_tilde_08_innerScala_kbmRefFreq = 440;
        ftom_tilde_08_innerScala_kbmSize = 0;
        ftom_tilde_08_innerScala_kbmMin = 0;
        ftom_tilde_08_innerScala_kbmMax = 0;
        ftom_tilde_08_innerScala_kbmOctaveDegree = 12;
        ftom_tilde_08_innerScala_kbmMapSize = 0;
        ftom_tilde_08_innerScala_refFreq = 261.63;
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
        number ftom_tilde_08_frequency;
        list ftom_tilde_08_scale;
        list ftom_tilde_08_map;
        number ftom_tilde_08_base;
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
        number ftom_tilde_08_innerFtoM_lastInValue;
        number ftom_tilde_08_innerFtoM_lastOutValue;
        number ftom_tilde_08_innerFtoM_lastTuning;
        bool ftom_tilde_08_innerScala_internal;
        const Index ftom_tilde_08_innerScala_KBM_MAP_OFFSET = 7;
        bool ftom_tilde_08_innerScala_lastValid;
        number ftom_tilde_08_innerScala_lastNote;
        number ftom_tilde_08_innerScala_lastFreq;
        int ftom_tilde_08_innerScala_sclEntryCount;
        number ftom_tilde_08_innerScala_sclOctaveMul;
        list ftom_tilde_08_innerScala_sclExpMul;
        list ftom_tilde_08_innerScala_kbmValid;
        int ftom_tilde_08_innerScala_kbmMid;
        int ftom_tilde_08_innerScala_kbmRefNum;
        number ftom_tilde_08_innerScala_kbmRefFreq;
        int ftom_tilde_08_innerScala_kbmSize;
        int ftom_tilde_08_innerScala_kbmMin;
        int ftom_tilde_08_innerScala_kbmMax;
        int ftom_tilde_08_innerScala_kbmOctaveDegree;
        Index ftom_tilde_08_innerScala_kbmMapSize;
        number ftom_tilde_08_innerScala_refFreq;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_1214 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_1214()
    {
    }
    
    ~RNBOSubpatcher_1214()
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
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    number wrap(number x, number low, number high) {
        number lo;
        number hi;
    
        if (low == high)
            return low;
    
        if (low > high) {
            hi = low;
            lo = high;
        } else {
            lo = low;
            hi = high;
        }
    
        number range = hi - lo;
    
        if (x >= lo && x < hi)
            return x;
    
        if (range <= 0.000000001)
            return lo;
    
        long numWraps = (long)(rnbo_trunc((x - lo) / range));
        numWraps = numWraps - ((x < lo ? 1 : 0));
        number result = x - range * numWraps;
    
        if (result >= hi)
            return result - range;
        else
            return result;
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        this->dspexpr_87_perform(this->signals[0], n);
        this->dspexpr_80_perform(in4, in3, this->signals[1], n);
        this->sah_tilde_15_perform(in1, this->signals[1], this->sah_tilde_15_thresh, out1, n);
        this->sah_tilde_16_perform(in2, this->signals[1], this->sah_tilde_16_thresh, this->signals[2], n);
        this->delaytilde_08_perform(this->delaytilde_08_delay, this->signals[2], this->signals[1], n);
        this->dspexpr_85_perform(this->signals[2], this->signals[1], this->signals[3], n);
        this->dspexpr_86_perform(this->signals[3], this->signals[1], n);
        this->dspexpr_84_perform(this->signals[1], this->dspexpr_84_in2, this->signals[3], n);
        this->dspexpr_83_perform(this->signals[3], this->dspexpr_83_in2, this->signals[1], n);
        this->dspexpr_82_perform(this->signals[1], this->signals[0], this->signals[3], n);
        this->dspexpr_88_perform(in4, this->dspexpr_88_in2, this->signals[0], n);
        this->dspexpr_81_perform(this->signals[3], this->signals[0], this->signals[1], n);
    
        this->slide_tilde_08_perform(
            this->signals[1],
            this->slide_tilde_08_up,
            this->slide_tilde_08_down,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->delaytilde_08_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            return "bogus";
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
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
        default:
            return value;
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
    
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            return addressOf(this->delaytilde_08_del_bufferobj);
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
    
        if (index == 1) {
            this->delaytilde_08_del_buffer = new Float64Buffer(this->delaytilde_08_del_bufferobj);
        }
    }
    
    void initialize() {
        this->delaytilde_08_del_bufferobj = initDataRef("delaytilde_08_del_bufferobj", true, nullptr);
        this->assign_defaults();
        this->setState();
        this->delaytilde_08_del_bufferobj->setIndex(1);
        this->delaytilde_08_del_buffer = new Float64Buffer(this->delaytilde_08_del_bufferobj);
    }
    
    protected:
    
    void eventinlet_08_out1_bang_bang() {
        this->expr_08_in1_bang();
    }
    
    void eventinlet_08_out1_list_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_08_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_08_in1_set(converted);
        }
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->dspexpr_84_in2_set(converted);
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
        return 4;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->delaytilde_08_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->delaytilde_08_del_buffer = this->delaytilde_08_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_08_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_08_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_08_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    void dspexpr_88_in2_set(number v) {
        this->dspexpr_88_in2 = v;
    }
    
    void expr_08_out1_set(number v) {
        this->expr_08_out1 = v;
        this->dspexpr_88_in2_set(this->expr_08_out1);
    }
    
    void expr_08_in1_set(number in1) {
        this->expr_08_in1 = in1;
    
        this->expr_08_out1_set(
            (this->expr_08_in2 == 0 ? 0 : (this->expr_08_in2 == 0. ? 0. : this->expr_08_in1 / this->expr_08_in2))
        );//#map:calcgainpitch[7]//_obj-64:1
    }
    
    void dspexpr_84_in2_set(number v) {
        this->dspexpr_84_in2 = v;
    }
    
    void eventinlet_08_out1_number_set(number v) {
        this->expr_08_in1_set(v);
        this->dspexpr_84_in2_set(v);
    }
    
    void expr_08_in1_bang() {
        this->expr_08_out1_set(
            (this->expr_08_in2 == 0 ? 0 : (this->expr_08_in2 == 0. ? 0. : this->expr_08_in1 / this->expr_08_in2))
        );//#map:calcgainpitch[7]//_obj-64:1
    }
    
    void expr_08_in2_set(number v) {
        this->expr_08_in2 = v;
    }
    
    void dspexpr_87_perform(Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = 6.283185307179586;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_80_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] == in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void sah_tilde_15_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_15_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void sah_tilde_16_perform(
        const Sample * input,
        const Sample * trig,
        number thresh,
        Sample * out,
        Index n
    ) {
        RNBO_UNUSED(thresh);
    
        for (Index i = 0; i < n; i++) {
            out[(Index)i] = this->sah_tilde_16_s_next(input[(Index)i], trig[(Index)i], 0);
        }
    }
    
    void delaytilde_08_perform(number delay, const Sample * input, Sample * output, Index n) {
        RNBO_UNUSED(delay);
        auto __delaytilde_08_crossfadeDelay = this->delaytilde_08_crossfadeDelay;
        auto __delaytilde_08_rampInSamples = this->delaytilde_08_rampInSamples;
        auto __delaytilde_08_ramp = this->delaytilde_08_ramp;
        auto __delaytilde_08_lastDelay = this->delaytilde_08_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_08_lastDelay == -1) {
                __delaytilde_08_lastDelay = 512;
            }
    
            if (__delaytilde_08_ramp > 0) {
                number factor = __delaytilde_08_ramp / __delaytilde_08_rampInSamples;
                output[(Index)i] = this->delaytilde_08_del_read(__delaytilde_08_crossfadeDelay, 0) * factor + this->delaytilde_08_del_read(__delaytilde_08_lastDelay, 0) * (1. - factor);
                __delaytilde_08_ramp--;
            } else {
                number effectiveDelay = 512;
    
                if (effectiveDelay != __delaytilde_08_lastDelay) {
                    __delaytilde_08_ramp = __delaytilde_08_rampInSamples;
                    __delaytilde_08_crossfadeDelay = __delaytilde_08_lastDelay;
                    __delaytilde_08_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_08_del_read(__delaytilde_08_crossfadeDelay, 0);
                    __delaytilde_08_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_08_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_08_del_write(input[(Index)i]);
            this->delaytilde_08_del_step();
        }
    
        this->delaytilde_08_lastDelay = __delaytilde_08_lastDelay;
        this->delaytilde_08_ramp = __delaytilde_08_ramp;
        this->delaytilde_08_crossfadeDelay = __delaytilde_08_crossfadeDelay;
    }
    
    void dspexpr_85_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_86_perform(const Sample * in1, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->wrap(in1[(Index)i], -3.14159265358979323846, 3.14159265358979323846);//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_84_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_83_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] / (number)512;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_82_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in2[(Index)i] == 0 ? 0 : (in2[(Index)i] == 0. ? 0. : in1[(Index)i] / in2[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_88_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_81_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void slide_tilde_08_perform(const Sample * x, number up, number down, Sample * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __slide_tilde_08_prev = this->slide_tilde_08_prev;
        number iup = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        number idown = this->safediv(1., this->maximum(1., rnbo_abs(1000)));
        Index i;
    
        for (i = 0; i < n; i++) {
            number temp = x[(Index)i] - __slide_tilde_08_prev;
            __slide_tilde_08_prev = __slide_tilde_08_prev + ((x[(Index)i] > __slide_tilde_08_prev ? iup : idown)) * temp;
            out1[(Index)i] = __slide_tilde_08_prev;
        }
    
        this->slide_tilde_08_prev = __slide_tilde_08_prev;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number sah_tilde_15_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_15_s_prev <= thresh && trig > thresh)
            this->sah_tilde_15_s_value = x;
    
        this->sah_tilde_15_s_prev = trig;
        return this->sah_tilde_15_s_value;
    }
    
    void sah_tilde_15_s_reset() {
        this->sah_tilde_15_s_prev = 0.;
        this->sah_tilde_15_s_value = 0.;
    }
    
    number sah_tilde_16_s_next(number x, number trig, number thresh) {
        if (this->sah_tilde_16_s_prev <= thresh && trig > thresh)
            this->sah_tilde_16_s_value = x;
    
        this->sah_tilde_16_s_prev = trig;
        return this->sah_tilde_16_s_value;
    }
    
    void sah_tilde_16_s_reset() {
        this->sah_tilde_16_s_prev = 0.;
        this->sah_tilde_16_s_value = 0.;
    }
    
    void delaytilde_08_del_step() {
        this->delaytilde_08_del_reader++;
    
        if (this->delaytilde_08_del_reader >= (int)(this->delaytilde_08_del_buffer->getSize()))
            this->delaytilde_08_del_reader = 0;
    }
    
    number delaytilde_08_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_08_del_buffer->getSize()) + this->delaytilde_08_del_reader - ((size > this->delaytilde_08_del__maxdelay ? this->delaytilde_08_del__maxdelay : (size < (this->delaytilde_08_del_reader != this->delaytilde_08_del_writer) ? this->delaytilde_08_del_reader != this->delaytilde_08_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_08_del_buffer->getSize()) + this->delaytilde_08_del_reader - ((size > this->delaytilde_08_del__maxdelay ? this->delaytilde_08_del__maxdelay : (size < (1 + this->delaytilde_08_del_reader != this->delaytilde_08_del_writer) ? 1 + this->delaytilde_08_del_reader != this->delaytilde_08_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_08_del_buffer->getSize()) + this->delaytilde_08_del_reader - ((size > this->delaytilde_08_del__maxdelay ? this->delaytilde_08_del__maxdelay : (size < (1 + this->delaytilde_08_del_reader != this->delaytilde_08_del_writer) ? 1 + this->delaytilde_08_del_reader != this->delaytilde_08_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index3 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index4 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_08_del_buffer->getSize()) + this->delaytilde_08_del_reader - ((size > this->delaytilde_08_del__maxdelay ? this->delaytilde_08_del__maxdelay : (size < (this->delaytilde_08_del_reader != this->delaytilde_08_del_writer) ? this->delaytilde_08_del_reader != this->delaytilde_08_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ), this->delaytilde_08_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((UBinOpInt)index2 & (UBinOpInt)this->delaytilde_08_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_08_del_buffer->getSize()) + this->delaytilde_08_del_reader - ((size > this->delaytilde_08_del__maxdelay ? this->delaytilde_08_del__maxdelay : (size < (this->delaytilde_08_del_reader != this->delaytilde_08_del_writer) ? this->delaytilde_08_del_reader != this->delaytilde_08_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_08_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((UBinOpInt)index1 & (UBinOpInt)this->delaytilde_08_del_wrap))
        );
    }
    
    void delaytilde_08_del_write(number v) {
        this->delaytilde_08_del_writer = this->delaytilde_08_del_reader;
        this->delaytilde_08_del_buffer[(Index)this->delaytilde_08_del_writer] = v;
    }
    
    number delaytilde_08_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_08_del__maxdelay : size);
        number val = this->delaytilde_08_del_read(effectiveSize, 0);
        this->delaytilde_08_del_write(v);
        this->delaytilde_08_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_08_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_08_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_08_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_08_del_init() {
        auto result = this->delaytilde_08_del_calcSizeInSamples();
        this->delaytilde_08_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_08_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_08_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_08_del_clear() {
        this->delaytilde_08_del_buffer->setZero();
    }
    
    void delaytilde_08_del_reset() {
        auto result = this->delaytilde_08_del_calcSizeInSamples();
        this->delaytilde_08_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_08_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_08_del_buffer);
        this->delaytilde_08_del_wrap = this->delaytilde_08_del_buffer->getSize() - 1;
        this->delaytilde_08_del_clear();
    
        if (this->delaytilde_08_del_reader >= this->delaytilde_08_del__maxdelay || this->delaytilde_08_del_writer >= this->delaytilde_08_del__maxdelay) {
            this->delaytilde_08_del_reader = 0;
            this->delaytilde_08_del_writer = 0;
        }
    }
    
    void delaytilde_08_del_dspsetup() {
        this->delaytilde_08_del_reset();
    }
    
    number delaytilde_08_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 512;
    }
    
    number delaytilde_08_del_size() {
        return this->delaytilde_08_del__maxdelay;
    }
    
    void delaytilde_08_dspsetup(bool force) {
        if ((bool)(this->delaytilde_08_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_08_rampInSamples = (long)(this->mstosamps(50));
        this->delaytilde_08_lastDelay = -1;
        this->delaytilde_08_setupDone = true;
        this->delaytilde_08_del_dspsetup();
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
        sah_tilde_15_input = 0;
        sah_tilde_15_trig = -1;
        sah_tilde_15_thresh = 0;
        dspexpr_80_in1 = 0;
        dspexpr_80_in2 = 0;
        slide_tilde_08_x = 0;
        slide_tilde_08_up = 1000;
        slide_tilde_08_down = 1000;
        dspexpr_81_in1 = 0;
        dspexpr_81_in2 = 0;
        dspexpr_82_in1 = 0;
        dspexpr_82_in2 = 0;
        dspexpr_83_in1 = 0;
        dspexpr_83_in2 = 512;
        dspexpr_84_in1 = 0;
        dspexpr_84_in2 = 48000;
        dspexpr_85_in1 = 0;
        dspexpr_85_in2 = 0;
        dspexpr_86_in1 = 0;
        sah_tilde_16_input = 0;
        sah_tilde_16_trig = -1;
        sah_tilde_16_thresh = 0;
        delaytilde_08_delay = 512;
        dspexpr_88_in1 = 0;
        dspexpr_88_in2 = 0;
        expr_08_in1 = 0;
        expr_08_in2 = 512;
        expr_08_out1 = 0;
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
        sah_tilde_15_s_prev = 0;
        sah_tilde_15_s_value = 0;
        slide_tilde_08_prev = 0;
        sah_tilde_16_s_prev = 0;
        sah_tilde_16_s_value = 0;
        delaytilde_08_lastDelay = -1;
        delaytilde_08_crossfadeDelay = 0;
        delaytilde_08_ramp = 0;
        delaytilde_08_rampInSamples = 0;
        delaytilde_08_del__maxdelay = 0;
        delaytilde_08_del_sizemode = 0;
        delaytilde_08_del_wrap = 0;
        delaytilde_08_del_reader = 0;
        delaytilde_08_del_writer = 0;
        delaytilde_08_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number sah_tilde_15_input;
        number sah_tilde_15_trig;
        number sah_tilde_15_thresh;
        number dspexpr_80_in1;
        number dspexpr_80_in2;
        number slide_tilde_08_x;
        number slide_tilde_08_up;
        number slide_tilde_08_down;
        number dspexpr_81_in1;
        number dspexpr_81_in2;
        number dspexpr_82_in1;
        number dspexpr_82_in2;
        number dspexpr_83_in1;
        number dspexpr_83_in2;
        number dspexpr_84_in1;
        number dspexpr_84_in2;
        number dspexpr_85_in1;
        number dspexpr_85_in2;
        number dspexpr_86_in1;
        number sah_tilde_16_input;
        number sah_tilde_16_trig;
        number sah_tilde_16_thresh;
        number delaytilde_08_delay;
        number dspexpr_88_in1;
        number dspexpr_88_in2;
        number expr_08_in1;
        number expr_08_in2;
        number expr_08_out1;
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
        number sah_tilde_15_s_prev;
        number sah_tilde_15_s_value;
        number slide_tilde_08_prev;
        number sah_tilde_16_s_prev;
        number sah_tilde_16_s_value;
        number delaytilde_08_lastDelay;
        number delaytilde_08_crossfadeDelay;
        number delaytilde_08_ramp;
        long delaytilde_08_rampInSamples;
        Float64BufferRef delaytilde_08_del_buffer;
        Index delaytilde_08_del__maxdelay;
        Int delaytilde_08_del_sizemode;
        Index delaytilde_08_del_wrap;
        Int delaytilde_08_del_reader;
        Int delaytilde_08_del_writer;
        bool delaytilde_08_setupDone;
        number stackprotect_count;
        DataRef delaytilde_08_del_bufferobj;
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
    delete this->p_09;
    delete this->p_10;
    delete this->p_11;
    delete this->p_12;
    delete this->p_13;
    delete this->p_14;
    delete this->p_15;
    delete this->p_16;
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
    getEngine()->flushClockEvents(this, -871642103, false);
    getEngine()->flushClockEvents(this, -1566058454, false);
    getEngine()->flushClockEvents(this, 2027079339, false);
    getEngine()->flushClockEvents(this, -162399448, false);
    getEngine()->flushClockEvents(this, -864228951, false);
    getEngine()->flushClockEvents(this, 1241259558, false);
    getEngine()->flushClockEvents(this, 539430055, false);
    getEngine()->flushClockEvents(this, 1480712303, false);
    getEngine()->flushClockEvents(this, 1943089061, false);
    getEngine()->flushClockEvents(this, -1410595987, false);
    getEngine()->flushClockEvents(this, -2112425490, false);
    getEngine()->flushClockEvents(this, -6936981, false);
    getEngine()->flushClockEvents(this, -708766484, false);
    getEngine()->flushClockEvents(this, 1396722025, false);
    getEngine()->flushClockEvents(this, 694892522, false);
    getEngine()->flushClockEvents(this, -1494586265, false);
    getEngine()->flushClockEvents(this, 2098551528, false);
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

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

inline number safesqrt(number num) {
    return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
}

number hann(number x) {
    x = (x > 1 ? 1 : (x < 0 ? 0 : x));
    return 0.5 * (1 - rnbo_cos(6.28318530717958647692 * x));
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
    SampleValue * out11 = (numOutputs >= 11 && outputs[10] ? outputs[10] : this->dummyBuffer);
    SampleValue * out12 = (numOutputs >= 12 && outputs[11] ? outputs[11] : this->dummyBuffer);
    SampleValue * out13 = (numOutputs >= 13 && outputs[12] ? outputs[12] : this->dummyBuffer);
    SampleValue * out14 = (numOutputs >= 14 && outputs[13] ? outputs[13] : this->dummyBuffer);
    SampleValue * out15 = (numOutputs >= 15 && outputs[14] ? outputs[14] : this->dummyBuffer);
    SampleValue * out16 = (numOutputs >= 16 && outputs[15] ? outputs[15] : this->dummyBuffer);
    SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);

    this->fftstream_tilde_01_perform(
        in1,
        this->fftstream_tilde_01_imagIn,
        this->signals[0],
        this->signals[1],
        this->signals[2],
        n
    );

    this->dspexpr_89_perform(this->signals[0], this->dspexpr_89_in2, this->signals[3], n);
    this->dspexpr_90_perform(this->signals[1], this->dspexpr_90_in2, this->signals[0], n);
    this->cartopol_tilde_01_perform(this->signals[3], this->signals[0], this->signals[1], this->signals[4], n);

    this->gen_01_perform(
        this->signals[1],
        this->signals[2],
        this->gen_01_framesize,
        this->signals[0],
        this->signals[3],
        this->signals[5],
        this->signals[6],
        this->signals[7],
        this->signals[8],
        this->signals[9],
        this->signals[10],
        n
    );

    this->p_01_perform(
        this->signals[1],
        this->signals[4],
        this->signals[2],
        this->signals[0],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_02_perform(this->signals[11], out2, n);
    this->numbertilde_01_perform(this->signals[12], this->signals[11], n);
    this->p_02_perform(this->signals[11], out1, n);

    this->p_04_perform(
        this->signals[1],
        this->signals[4],
        this->signals[2],
        this->signals[3],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_04_perform(this->signals[11], out4, n);
    this->numbertilde_03_perform(this->signals[12], this->signals[11], n);
    this->p_03_perform(this->signals[11], out3, n);

    this->p_06_perform(
        this->signals[1],
        this->signals[4],
        this->signals[2],
        this->signals[5],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_06_perform(this->signals[11], out6, n);
    this->numbertilde_05_perform(this->signals[12], this->signals[11], n);
    this->p_05_perform(this->signals[11], out5, n);
    this->signaladder_01_perform(this->signals[1], this->signals[2], this->signals[5], n);

    this->p_08_perform(
        this->signals[5],
        this->signals[4],
        this->signals[2],
        this->signals[6],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_08_perform(this->signals[11], out8, n);
    this->numbertilde_07_perform(this->signals[12], this->signals[11], n);
    this->p_07_perform(this->signals[11], out7, n);

    this->p_10_perform(
        this->signals[1],
        this->signals[4],
        this->signals[2],
        this->signals[7],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_10_perform(this->signals[11], out10, n);
    this->numbertilde_09_perform(this->signals[12], this->signals[11], n);
    this->p_09_perform(this->signals[11], out9, n);

    this->p_12_perform(
        this->signals[1],
        this->signals[4],
        this->signals[2],
        this->signals[8],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_12_perform(this->signals[11], out12, n);
    this->numbertilde_11_perform(this->signals[12], this->signals[11], n);
    this->p_11_perform(this->signals[11], out11, n);

    this->p_14_perform(
        this->signals[1],
        this->signals[4],
        this->signals[2],
        this->signals[9],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_14_perform(this->signals[11], out14, n);
    this->numbertilde_13_perform(this->signals[12], this->signals[11], n);
    this->p_13_perform(this->signals[11], out13, n);

    this->p_16_perform(
        this->signals[1],
        this->signals[4],
        this->signals[2],
        this->signals[10],
        this->signals[11],
        this->signals[12],
        n
    );

    this->numbertilde_16_perform(this->signals[11], out16, n);
    this->numbertilde_15_perform(this->signals[12], this->signals[11], n);
    this->p_15_perform(this->signals[11], out15, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 13; i++) {
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

    this->fftstream_tilde_01_dspsetup(forceDSPSetup);
    this->numbertilde_02_dspsetup(forceDSPSetup);
    this->numbertilde_01_dspsetup(forceDSPSetup);
    this->numbertilde_04_dspsetup(forceDSPSetup);
    this->numbertilde_03_dspsetup(forceDSPSetup);
    this->numbertilde_06_dspsetup(forceDSPSetup);
    this->numbertilde_05_dspsetup(forceDSPSetup);
    this->numbertilde_08_dspsetup(forceDSPSetup);
    this->numbertilde_07_dspsetup(forceDSPSetup);
    this->numbertilde_10_dspsetup(forceDSPSetup);
    this->numbertilde_09_dspsetup(forceDSPSetup);
    this->numbertilde_12_dspsetup(forceDSPSetup);
    this->numbertilde_11_dspsetup(forceDSPSetup);
    this->numbertilde_14_dspsetup(forceDSPSetup);
    this->numbertilde_13_dspsetup(forceDSPSetup);
    this->numbertilde_16_dspsetup(forceDSPSetup);
    this->numbertilde_15_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_02->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_03->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_04->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_05->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_06->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_07->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_08->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_09->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_10->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_11->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_12->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_13->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_14->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_15->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_16->prepareToProcess(sampleRate, maxBlockSize, force);

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
    }

    this->p_01->processDataViewUpdate(index, time);
    this->p_02->processDataViewUpdate(index, time);
    this->p_03->processDataViewUpdate(index, time);
    this->p_04->processDataViewUpdate(index, time);
    this->p_05->processDataViewUpdate(index, time);
    this->p_06->processDataViewUpdate(index, time);
    this->p_07->processDataViewUpdate(index, time);
    this->p_08->processDataViewUpdate(index, time);
    this->p_09->processDataViewUpdate(index, time);
    this->p_10->processDataViewUpdate(index, time);
    this->p_11->processDataViewUpdate(index, time);
    this->p_12->processDataViewUpdate(index, time);
    this->p_13->processDataViewUpdate(index, time);
    this->p_14->processDataViewUpdate(index, time);
    this->p_15->processDataViewUpdate(index, time);
    this->p_16->processDataViewUpdate(index, time);
}

void initialize() {
    this->RNBODefaultFftWindow = initDataRef("RNBODefaultFftWindow", false, nullptr);
    this->assign_defaults();
    this->setState();
    this->RNBODefaultFftWindow->setIndex(0);
    this->fftstream_tilde_01_win_buf = new Float32Buffer(this->RNBODefaultFftWindow);
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
    this->p_01 = new RNBOSubpatcher_1199();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_1200();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));
    this->p_03 = new RNBOSubpatcher_1201();
    this->p_03->setEngineAndPatcher(this->getEngine(), this);
    this->p_03->initialize();
    this->p_03->setParameterOffset(this->getParameterOffset(this->p_03));
    this->p_04 = new RNBOSubpatcher_1202();
    this->p_04->setEngineAndPatcher(this->getEngine(), this);
    this->p_04->initialize();
    this->p_04->setParameterOffset(this->getParameterOffset(this->p_04));
    this->p_05 = new RNBOSubpatcher_1203();
    this->p_05->setEngineAndPatcher(this->getEngine(), this);
    this->p_05->initialize();
    this->p_05->setParameterOffset(this->getParameterOffset(this->p_05));
    this->p_06 = new RNBOSubpatcher_1204();
    this->p_06->setEngineAndPatcher(this->getEngine(), this);
    this->p_06->initialize();
    this->p_06->setParameterOffset(this->getParameterOffset(this->p_06));
    this->p_07 = new RNBOSubpatcher_1205();
    this->p_07->setEngineAndPatcher(this->getEngine(), this);
    this->p_07->initialize();
    this->p_07->setParameterOffset(this->getParameterOffset(this->p_07));
    this->p_08 = new RNBOSubpatcher_1206();
    this->p_08->setEngineAndPatcher(this->getEngine(), this);
    this->p_08->initialize();
    this->p_08->setParameterOffset(this->getParameterOffset(this->p_08));
    this->p_09 = new RNBOSubpatcher_1207();
    this->p_09->setEngineAndPatcher(this->getEngine(), this);
    this->p_09->initialize();
    this->p_09->setParameterOffset(this->getParameterOffset(this->p_09));
    this->p_10 = new RNBOSubpatcher_1208();
    this->p_10->setEngineAndPatcher(this->getEngine(), this);
    this->p_10->initialize();
    this->p_10->setParameterOffset(this->getParameterOffset(this->p_10));
    this->p_11 = new RNBOSubpatcher_1209();
    this->p_11->setEngineAndPatcher(this->getEngine(), this);
    this->p_11->initialize();
    this->p_11->setParameterOffset(this->getParameterOffset(this->p_11));
    this->p_12 = new RNBOSubpatcher_1210();
    this->p_12->setEngineAndPatcher(this->getEngine(), this);
    this->p_12->initialize();
    this->p_12->setParameterOffset(this->getParameterOffset(this->p_12));
    this->p_13 = new RNBOSubpatcher_1211();
    this->p_13->setEngineAndPatcher(this->getEngine(), this);
    this->p_13->initialize();
    this->p_13->setParameterOffset(this->getParameterOffset(this->p_13));
    this->p_14 = new RNBOSubpatcher_1212();
    this->p_14->setEngineAndPatcher(this->getEngine(), this);
    this->p_14->initialize();
    this->p_14->setParameterOffset(this->getParameterOffset(this->p_14));
    this->p_15 = new RNBOSubpatcher_1213();
    this->p_15->setEngineAndPatcher(this->getEngine(), this);
    this->p_15->initialize();
    this->p_15->setParameterOffset(this->getParameterOffset(this->p_15));
    this->p_16 = new RNBOSubpatcher_1214();
    this->p_16->setEngineAndPatcher(this->getEngine(), this);
    this->p_16->initialize();
    this->p_16->setParameterOffset(this->getParameterOffset(this->p_16));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-32"));
    this->p_03->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-44"));
    this->p_04->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch[1]"));
    this->p_05->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-47"));
    this->p_06->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch[2]"));
    this->p_07->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-50"));
    this->p_08->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch[3]"));
    this->p_09->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-53"));
    this->p_10->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch[4]"));
    this->p_11->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-7"));
    this->p_12->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch[5]"));
    this->p_13->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-15"));
    this->p_14->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch[6]"));
    this->p_15->getPreset(getSubState(getSubState(preset, "__sps"), "p_obj-26"));
    this->p_16->getPreset(getSubState(getSubState(preset, "__sps"), "calcgainpitch[7]"));
}

void setPreset(MillisecondTime , PatcherStateInterface& ) {}

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
        this->p_09->processTempoEvent(time, tempo);
        this->p_10->processTempoEvent(time, tempo);
        this->p_11->processTempoEvent(time, tempo);
        this->p_12->processTempoEvent(time, tempo);
        this->p_13->processTempoEvent(time, tempo);
        this->p_14->processTempoEvent(time, tempo);
        this->p_15->processTempoEvent(time, tempo);
        this->p_16->processTempoEvent(time, tempo);
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
        this->p_09->processTransportEvent(time, state);
        this->p_10->processTransportEvent(time, state);
        this->p_11->processTransportEvent(time, state);
        this->p_12->processTransportEvent(time, state);
        this->p_13->processTransportEvent(time, state);
        this->p_14->processTransportEvent(time, state);
        this->p_15->processTransportEvent(time, state);
        this->p_16->processTransportEvent(time, state);
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
        this->p_09->processBeatTimeEvent(time, beattime);
        this->p_10->processBeatTimeEvent(time, beattime);
        this->p_11->processBeatTimeEvent(time, beattime);
        this->p_12->processBeatTimeEvent(time, beattime);
        this->p_13->processBeatTimeEvent(time, beattime);
        this->p_14->processBeatTimeEvent(time, beattime);
        this->p_15->processBeatTimeEvent(time, beattime);
        this->p_16->processBeatTimeEvent(time, beattime);
    }
}

void onSampleRateChanged(double samplerate) {
    this->samplerate_01_onSampleRateChanged(samplerate);
}

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
        this->p_09->processTimeSignatureEvent(time, numerator, denominator);
        this->p_10->processTimeSignatureEvent(time, numerator, denominator);
        this->p_11->processTimeSignatureEvent(time, numerator, denominator);
        this->p_12->processTimeSignatureEvent(time, numerator, denominator);
        this->p_13->processTimeSignatureEvent(time, numerator, denominator);
        this->p_14->processTimeSignatureEvent(time, numerator, denominator);
        this->p_15->processTimeSignatureEvent(time, numerator, denominator);
        this->p_16->processTimeSignatureEvent(time, numerator, denominator);
    }
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    RNBO_UNUSED(v);
    this->updateTime(time);

    switch (index) {
    case 0:
        this->button_01_bangval_bang();
        break;
    case 1:
        this->message_01_bangval_bang();
        break;
    default:
        index -= 2;

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

        index -= this->p_08->getNumParameters();

        if (index < this->p_09->getNumParameters())
            this->p_09->setParameterValue(index, v, time);

        index -= this->p_09->getNumParameters();

        if (index < this->p_10->getNumParameters())
            this->p_10->setParameterValue(index, v, time);

        index -= this->p_10->getNumParameters();

        if (index < this->p_11->getNumParameters())
            this->p_11->setParameterValue(index, v, time);

        index -= this->p_11->getNumParameters();

        if (index < this->p_12->getNumParameters())
            this->p_12->setParameterValue(index, v, time);

        index -= this->p_12->getNumParameters();

        if (index < this->p_13->getNumParameters())
            this->p_13->setParameterValue(index, v, time);

        index -= this->p_13->getNumParameters();

        if (index < this->p_14->getNumParameters())
            this->p_14->setParameterValue(index, v, time);

        index -= this->p_14->getNumParameters();

        if (index < this->p_15->getNumParameters())
            this->p_15->setParameterValue(index, v, time);

        index -= this->p_15->getNumParameters();

        if (index < this->p_16->getNumParameters())
            this->p_16->setParameterValue(index, v, time);

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
        index -= 2;

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

        index -= this->p_08->getNumParameters();

        if (index < this->p_09->getNumParameters())
            return this->p_09->getParameterValue(index);

        index -= this->p_09->getNumParameters();

        if (index < this->p_10->getNumParameters())
            return this->p_10->getParameterValue(index);

        index -= this->p_10->getNumParameters();

        if (index < this->p_11->getNumParameters())
            return this->p_11->getParameterValue(index);

        index -= this->p_11->getNumParameters();

        if (index < this->p_12->getNumParameters())
            return this->p_12->getParameterValue(index);

        index -= this->p_12->getNumParameters();

        if (index < this->p_13->getNumParameters())
            return this->p_13->getParameterValue(index);

        index -= this->p_13->getNumParameters();

        if (index < this->p_14->getNumParameters())
            return this->p_14->getParameterValue(index);

        index -= this->p_14->getNumParameters();

        if (index < this->p_15->getNumParameters())
            return this->p_15->getParameterValue(index);

        index -= this->p_15->getNumParameters();

        if (index < this->p_16->getNumParameters())
            return this->p_16->getParameterValue(index);

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
    return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters() + this->p_10->getNumParameters() + this->p_11->getNumParameters() + this->p_12->getNumParameters() + this->p_13->getNumParameters() + this->p_14->getNumParameters() + this->p_15->getNumParameters() + this->p_16->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "button_01_bangval";
    case 1:
        return "message_01_bangval";
    default:
        index -= 2;

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

        index -= this->p_08->getNumParameters();

        if (index < this->p_09->getNumParameters())
            return this->p_09->getParameterName(index);

        index -= this->p_09->getNumParameters();

        if (index < this->p_10->getNumParameters())
            return this->p_10->getParameterName(index);

        index -= this->p_10->getNumParameters();

        if (index < this->p_11->getNumParameters())
            return this->p_11->getParameterName(index);

        index -= this->p_11->getNumParameters();

        if (index < this->p_12->getNumParameters())
            return this->p_12->getParameterName(index);

        index -= this->p_12->getNumParameters();

        if (index < this->p_13->getNumParameters())
            return this->p_13->getParameterName(index);

        index -= this->p_13->getNumParameters();

        if (index < this->p_14->getNumParameters())
            return this->p_14->getParameterName(index);

        index -= this->p_14->getNumParameters();

        if (index < this->p_15->getNumParameters())
            return this->p_15->getParameterName(index);

        index -= this->p_15->getNumParameters();

        if (index < this->p_16->getNumParameters())
            return this->p_16->getParameterName(index);

        return "bogus";
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "button_obj-43/bangval";
    case 1:
        return "message_obj-29/bangval";
    default:
        index -= 2;

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

        index -= this->p_08->getNumParameters();

        if (index < this->p_09->getNumParameters())
            return this->p_09->getParameterId(index);

        index -= this->p_09->getNumParameters();

        if (index < this->p_10->getNumParameters())
            return this->p_10->getParameterId(index);

        index -= this->p_10->getNumParameters();

        if (index < this->p_11->getNumParameters())
            return this->p_11->getParameterId(index);

        index -= this->p_11->getNumParameters();

        if (index < this->p_12->getNumParameters())
            return this->p_12->getParameterId(index);

        index -= this->p_12->getNumParameters();

        if (index < this->p_13->getNumParameters())
            return this->p_13->getParameterId(index);

        index -= this->p_13->getNumParameters();

        if (index < this->p_14->getNumParameters())
            return this->p_14->getParameterId(index);

        index -= this->p_14->getNumParameters();

        if (index < this->p_15->getNumParameters())
            return this->p_15->getParameterId(index);

        index -= this->p_15->getNumParameters();

        if (index < this->p_16->getNumParameters())
            return this->p_16->getParameterId(index);

        return "bogus";
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
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
        case 1:
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
        default:
            index -= 2;

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

            index -= this->p_08->getNumParameters();

            if (index < this->p_09->getNumParameters())
                this->p_09->getParameterInfo(index, info);

            index -= this->p_09->getNumParameters();

            if (index < this->p_10->getNumParameters())
                this->p_10->getParameterInfo(index, info);

            index -= this->p_10->getNumParameters();

            if (index < this->p_11->getNumParameters())
                this->p_11->getParameterInfo(index, info);

            index -= this->p_11->getNumParameters();

            if (index < this->p_12->getNumParameters())
                this->p_12->getParameterInfo(index, info);

            index -= this->p_12->getNumParameters();

            if (index < this->p_13->getNumParameters())
                this->p_13->getParameterInfo(index, info);

            index -= this->p_13->getNumParameters();

            if (index < this->p_14->getNumParameters())
                this->p_14->getParameterInfo(index, info);

            index -= this->p_14->getNumParameters();

            if (index < this->p_15->getNumParameters())
                this->p_15->getParameterInfo(index, info);

            index -= this->p_15->getNumParameters();

            if (index < this->p_16->getNumParameters())
                this->p_16->getParameterInfo(index, info);

            break;
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->p_01)
        return 2;

    if (subpatcher == this->p_02)
        return 2 + this->p_01->getNumParameters();

    if (subpatcher == this->p_03)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters();

    if (subpatcher == this->p_04)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters();

    if (subpatcher == this->p_05)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters();

    if (subpatcher == this->p_06)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters();

    if (subpatcher == this->p_07)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters();

    if (subpatcher == this->p_08)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters();

    if (subpatcher == this->p_09)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters();

    if (subpatcher == this->p_10)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters();

    if (subpatcher == this->p_11)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters() + this->p_10->getNumParameters();

    if (subpatcher == this->p_12)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters() + this->p_10->getNumParameters() + this->p_11->getNumParameters();

    if (subpatcher == this->p_13)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters() + this->p_10->getNumParameters() + this->p_11->getNumParameters() + this->p_12->getNumParameters();

    if (subpatcher == this->p_14)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters() + this->p_10->getNumParameters() + this->p_11->getNumParameters() + this->p_12->getNumParameters() + this->p_13->getNumParameters();

    if (subpatcher == this->p_15)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters() + this->p_10->getNumParameters() + this->p_11->getNumParameters() + this->p_12->getNumParameters() + this->p_13->getNumParameters() + this->p_14->getNumParameters();

    if (subpatcher == this->p_16)
        return 2 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters() + this->p_09->getNumParameters() + this->p_10->getNumParameters() + this->p_11->getNumParameters() + this->p_12->getNumParameters() + this->p_13->getNumParameters() + this->p_14->getNumParameters() + this->p_15->getNumParameters();

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
        index -= 2;

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

        index -= this->p_08->getNumParameters();

        if (index < this->p_09->getNumParameters())
            return this->p_09->convertToNormalizedParameterValue(index, value);

        index -= this->p_09->getNumParameters();

        if (index < this->p_10->getNumParameters())
            return this->p_10->convertToNormalizedParameterValue(index, value);

        index -= this->p_10->getNumParameters();

        if (index < this->p_11->getNumParameters())
            return this->p_11->convertToNormalizedParameterValue(index, value);

        index -= this->p_11->getNumParameters();

        if (index < this->p_12->getNumParameters())
            return this->p_12->convertToNormalizedParameterValue(index, value);

        index -= this->p_12->getNumParameters();

        if (index < this->p_13->getNumParameters())
            return this->p_13->convertToNormalizedParameterValue(index, value);

        index -= this->p_13->getNumParameters();

        if (index < this->p_14->getNumParameters())
            return this->p_14->convertToNormalizedParameterValue(index, value);

        index -= this->p_14->getNumParameters();

        if (index < this->p_15->getNumParameters())
            return this->p_15->convertToNormalizedParameterValue(index, value);

        index -= this->p_15->getNumParameters();

        if (index < this->p_16->getNumParameters())
            return this->p_16->convertToNormalizedParameterValue(index, value);

        return value;
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    default:
        index -= 2;

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

        index -= this->p_08->getNumParameters();

        if (index < this->p_09->getNumParameters())
            return this->p_09->convertFromNormalizedParameterValue(index, value);

        index -= this->p_09->getNumParameters();

        if (index < this->p_10->getNumParameters())
            return this->p_10->convertFromNormalizedParameterValue(index, value);

        index -= this->p_10->getNumParameters();

        if (index < this->p_11->getNumParameters())
            return this->p_11->convertFromNormalizedParameterValue(index, value);

        index -= this->p_11->getNumParameters();

        if (index < this->p_12->getNumParameters())
            return this->p_12->convertFromNormalizedParameterValue(index, value);

        index -= this->p_12->getNumParameters();

        if (index < this->p_13->getNumParameters())
            return this->p_13->convertFromNormalizedParameterValue(index, value);

        index -= this->p_13->getNumParameters();

        if (index < this->p_14->getNumParameters())
            return this->p_14->convertFromNormalizedParameterValue(index, value);

        index -= this->p_14->getNumParameters();

        if (index < this->p_15->getNumParameters())
            return this->p_15->convertFromNormalizedParameterValue(index, value);

        index -= this->p_15->getNumParameters();

        if (index < this->p_16->getNumParameters())
            return this->p_16->convertFromNormalizedParameterValue(index, value);

        return value;
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    default:
        index -= 2;

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

        index -= this->p_08->getNumParameters();

        if (index < this->p_09->getNumParameters())
            return this->p_09->constrainParameterValue(index, value);

        index -= this->p_09->getNumParameters();

        if (index < this->p_10->getNumParameters())
            return this->p_10->constrainParameterValue(index, value);

        index -= this->p_10->getNumParameters();

        if (index < this->p_11->getNumParameters())
            return this->p_11->constrainParameterValue(index, value);

        index -= this->p_11->getNumParameters();

        if (index < this->p_12->getNumParameters())
            return this->p_12->constrainParameterValue(index, value);

        index -= this->p_12->getNumParameters();

        if (index < this->p_13->getNumParameters())
            return this->p_13->constrainParameterValue(index, value);

        index -= this->p_13->getNumParameters();

        if (index < this->p_14->getNumParameters())
            return this->p_14->constrainParameterValue(index, value);

        index -= this->p_14->getNumParameters();

        if (index < this->p_15->getNumParameters())
            return this->p_15->constrainParameterValue(index, value);

        index -= this->p_15->getNumParameters();

        if (index < this->p_16->getNumParameters())
            return this->p_16->constrainParameterValue(index, value);

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

void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
    RNBO_UNUSED(hasValue);
    this->updateTime(time);

    switch (index) {
    case -871642103:
        this->loadbang_01_startupbang_bang();
        break;
    case -1566058454:
        this->numbertilde_02_value_set(value);
        break;
    case 2027079339:
        this->numbertilde_01_value_set(value);
        break;
    case -162399448:
        this->numbertilde_04_value_set(value);
        break;
    case -864228951:
        this->numbertilde_03_value_set(value);
        break;
    case 1241259558:
        this->numbertilde_06_value_set(value);
        break;
    case 539430055:
        this->numbertilde_05_value_set(value);
        break;
    case 1480712303:
        this->numbertilde_08_value_set(value);
        break;
    case 1943089061:
        this->numbertilde_07_value_set(value);
        break;
    case -1410595987:
        this->numbertilde_10_value_set(value);
        break;
    case -2112425490:
        this->numbertilde_09_value_set(value);
        break;
    case -6936981:
        this->numbertilde_12_value_set(value);
        break;
    case -708766484:
        this->numbertilde_11_value_set(value);
        break;
    case 1396722025:
        this->numbertilde_14_value_set(value);
        break;
    case 694892522:
        this->numbertilde_13_value_set(value);
        break;
    case -1494586265:
        this->numbertilde_16_value_set(value);
        break;
    case 2098551528:
        this->numbertilde_15_value_set(value);
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
        if (TAG("number~_obj-9") == objectId)
            this->numbertilde_01_sig_number_set(payload);

        if (TAG("number~_obj-11") == objectId)
            this->numbertilde_02_sig_number_set(payload);

        if (TAG("number~_obj-19") == objectId)
            this->numbertilde_03_sig_number_set(payload);

        if (TAG("number~_obj-12") == objectId)
            this->numbertilde_04_sig_number_set(payload);

        if (TAG("number~_obj-24") == objectId)
            this->numbertilde_05_sig_number_set(payload);

        if (TAG("number~_obj-23") == objectId)
            this->numbertilde_06_sig_number_set(payload);

        if (TAG("number~_obj-31") == objectId)
            this->numbertilde_07_sig_number_set(payload);

        if (TAG("number~_obj-30") == objectId)
            this->numbertilde_08_sig_number_set(payload);

        if (TAG("number~_obj-35") == objectId)
            this->numbertilde_09_sig_number_set(payload);

        if (TAG("number~_obj-34") == objectId)
            this->numbertilde_10_sig_number_set(payload);

        if (TAG("number~_obj-14") == objectId)
            this->numbertilde_11_sig_number_set(payload);

        if (TAG("number~_obj-13") == objectId)
            this->numbertilde_12_sig_number_set(payload);

        if (TAG("number~_obj-25") == objectId)
            this->numbertilde_13_sig_number_set(payload);

        if (TAG("number~_obj-22") == objectId)
            this->numbertilde_14_sig_number_set(payload);

        if (TAG("number~_obj-37") == objectId)
            this->numbertilde_15_sig_number_set(payload);

        if (TAG("number~_obj-36") == objectId)
            this->numbertilde_16_sig_number_set(payload);

        break;
    case TAG("mode"):
        if (TAG("number~_obj-9") == objectId)
            this->numbertilde_01_mode_set(payload);

        if (TAG("number~_obj-11") == objectId)
            this->numbertilde_02_mode_set(payload);

        if (TAG("number~_obj-19") == objectId)
            this->numbertilde_03_mode_set(payload);

        if (TAG("number~_obj-12") == objectId)
            this->numbertilde_04_mode_set(payload);

        if (TAG("number~_obj-24") == objectId)
            this->numbertilde_05_mode_set(payload);

        if (TAG("number~_obj-23") == objectId)
            this->numbertilde_06_mode_set(payload);

        if (TAG("number~_obj-31") == objectId)
            this->numbertilde_07_mode_set(payload);

        if (TAG("number~_obj-30") == objectId)
            this->numbertilde_08_mode_set(payload);

        if (TAG("number~_obj-35") == objectId)
            this->numbertilde_09_mode_set(payload);

        if (TAG("number~_obj-34") == objectId)
            this->numbertilde_10_mode_set(payload);

        if (TAG("number~_obj-14") == objectId)
            this->numbertilde_11_mode_set(payload);

        if (TAG("number~_obj-13") == objectId)
            this->numbertilde_12_mode_set(payload);

        if (TAG("number~_obj-25") == objectId)
            this->numbertilde_13_mode_set(payload);

        if (TAG("number~_obj-22") == objectId)
            this->numbertilde_14_mode_set(payload);

        if (TAG("number~_obj-37") == objectId)
            this->numbertilde_15_mode_set(payload);

        if (TAG("number~_obj-36") == objectId)
            this->numbertilde_16_mode_set(payload);

        break;
    }

    this->p_01->processNumMessage(tag, objectId, time, payload);
    this->p_02->processNumMessage(tag, objectId, time, payload);
    this->p_03->processNumMessage(tag, objectId, time, payload);
    this->p_04->processNumMessage(tag, objectId, time, payload);
    this->p_05->processNumMessage(tag, objectId, time, payload);
    this->p_06->processNumMessage(tag, objectId, time, payload);
    this->p_07->processNumMessage(tag, objectId, time, payload);
    this->p_08->processNumMessage(tag, objectId, time, payload);
    this->p_09->processNumMessage(tag, objectId, time, payload);
    this->p_10->processNumMessage(tag, objectId, time, payload);
    this->p_11->processNumMessage(tag, objectId, time, payload);
    this->p_12->processNumMessage(tag, objectId, time, payload);
    this->p_13->processNumMessage(tag, objectId, time, payload);
    this->p_14->processNumMessage(tag, objectId, time, payload);
    this->p_15->processNumMessage(tag, objectId, time, payload);
    this->p_16->processNumMessage(tag, objectId, time, payload);
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
        if (TAG("number~_obj-9") == objectId)
            this->numbertilde_01_sig_list_set(payload);

        if (TAG("number~_obj-11") == objectId)
            this->numbertilde_02_sig_list_set(payload);

        if (TAG("number~_obj-19") == objectId)
            this->numbertilde_03_sig_list_set(payload);

        if (TAG("number~_obj-12") == objectId)
            this->numbertilde_04_sig_list_set(payload);

        if (TAG("number~_obj-24") == objectId)
            this->numbertilde_05_sig_list_set(payload);

        if (TAG("number~_obj-23") == objectId)
            this->numbertilde_06_sig_list_set(payload);

        if (TAG("number~_obj-31") == objectId)
            this->numbertilde_07_sig_list_set(payload);

        if (TAG("number~_obj-30") == objectId)
            this->numbertilde_08_sig_list_set(payload);

        if (TAG("number~_obj-35") == objectId)
            this->numbertilde_09_sig_list_set(payload);

        if (TAG("number~_obj-34") == objectId)
            this->numbertilde_10_sig_list_set(payload);

        if (TAG("number~_obj-14") == objectId)
            this->numbertilde_11_sig_list_set(payload);

        if (TAG("number~_obj-13") == objectId)
            this->numbertilde_12_sig_list_set(payload);

        if (TAG("number~_obj-25") == objectId)
            this->numbertilde_13_sig_list_set(payload);

        if (TAG("number~_obj-22") == objectId)
            this->numbertilde_14_sig_list_set(payload);

        if (TAG("number~_obj-37") == objectId)
            this->numbertilde_15_sig_list_set(payload);

        if (TAG("number~_obj-36") == objectId)
            this->numbertilde_16_sig_list_set(payload);

        break;
    }

    this->p_01->processListMessage(tag, objectId, time, payload);
    this->p_02->processListMessage(tag, objectId, time, payload);
    this->p_03->processListMessage(tag, objectId, time, payload);
    this->p_04->processListMessage(tag, objectId, time, payload);
    this->p_05->processListMessage(tag, objectId, time, payload);
    this->p_06->processListMessage(tag, objectId, time, payload);
    this->p_07->processListMessage(tag, objectId, time, payload);
    this->p_08->processListMessage(tag, objectId, time, payload);
    this->p_09->processListMessage(tag, objectId, time, payload);
    this->p_10->processListMessage(tag, objectId, time, payload);
    this->p_11->processListMessage(tag, objectId, time, payload);
    this->p_12->processListMessage(tag, objectId, time, payload);
    this->p_13->processListMessage(tag, objectId, time, payload);
    this->p_14->processListMessage(tag, objectId, time, payload);
    this->p_15->processListMessage(tag, objectId, time, payload);
    this->p_16->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    this->updateTime(time);

    switch (tag) {
    case TAG("startupbang"):
        if (TAG("loadbang_obj-41") == objectId)
            this->loadbang_01_startupbang_bang();

        break;
    }

    this->p_01->processBangMessage(tag, objectId, time);
    this->p_02->processBangMessage(tag, objectId, time);
    this->p_03->processBangMessage(tag, objectId, time);
    this->p_04->processBangMessage(tag, objectId, time);
    this->p_05->processBangMessage(tag, objectId, time);
    this->p_06->processBangMessage(tag, objectId, time);
    this->p_07->processBangMessage(tag, objectId, time);
    this->p_08->processBangMessage(tag, objectId, time);
    this->p_09->processBangMessage(tag, objectId, time);
    this->p_10->processBangMessage(tag, objectId, time);
    this->p_11->processBangMessage(tag, objectId, time);
    this->p_12->processBangMessage(tag, objectId, time);
    this->p_13->processBangMessage(tag, objectId, time);
    this->p_14->processBangMessage(tag, objectId, time);
    this->p_15->processBangMessage(tag, objectId, time);
    this->p_16->processBangMessage(tag, objectId, time);
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {
    case TAG("monitor"):
        return "monitor";
    case TAG("number~_obj-9"):
        return "number~_obj-9";
    case TAG("assign"):
        return "assign";
    case TAG("setup"):
        return "setup";
    case TAG("number~_obj-11"):
        return "number~_obj-11";
    case TAG("number~_obj-19"):
        return "number~_obj-19";
    case TAG("number~_obj-12"):
        return "number~_obj-12";
    case TAG("number~_obj-24"):
        return "number~_obj-24";
    case TAG("number~_obj-23"):
        return "number~_obj-23";
    case TAG("number~_obj-31"):
        return "number~_obj-31";
    case TAG("number~_obj-30"):
        return "number~_obj-30";
    case TAG("number~_obj-35"):
        return "number~_obj-35";
    case TAG("number~_obj-34"):
        return "number~_obj-34";
    case TAG("number~_obj-14"):
        return "number~_obj-14";
    case TAG("number~_obj-13"):
        return "number~_obj-13";
    case TAG("number~_obj-25"):
        return "number~_obj-25";
    case TAG("number~_obj-22"):
        return "number~_obj-22";
    case TAG("number~_obj-37"):
        return "number~_obj-37";
    case TAG("number~_obj-36"):
        return "number~_obj-36";
    case TAG("sig"):
        return "sig";
    case TAG("mode"):
        return "mode";
    case TAG("startupbang"):
        return "startupbang";
    case TAG("loadbang_obj-41"):
        return "loadbang_obj-41";
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

    auto subpatchResult_8 = this->p_09->resolveTag(tag);

    if (subpatchResult_8)
        return subpatchResult_8;

    auto subpatchResult_9 = this->p_10->resolveTag(tag);

    if (subpatchResult_9)
        return subpatchResult_9;

    auto subpatchResult_10 = this->p_11->resolveTag(tag);

    if (subpatchResult_10)
        return subpatchResult_10;

    auto subpatchResult_11 = this->p_12->resolveTag(tag);

    if (subpatchResult_11)
        return subpatchResult_11;

    auto subpatchResult_12 = this->p_13->resolveTag(tag);

    if (subpatchResult_12)
        return subpatchResult_12;

    auto subpatchResult_13 = this->p_14->resolveTag(tag);

    if (subpatchResult_13)
        return subpatchResult_13;

    auto subpatchResult_14 = this->p_15->resolveTag(tag);

    if (subpatchResult_14)
        return subpatchResult_14;

    auto subpatchResult_15 = this->p_16->resolveTag(tag);

    if (subpatchResult_15)
        return subpatchResult_15;

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

void button_01_bangval_bang() {
    this->sendParameter(0, true);
    this->message_01_bangval_bang();
    this->samplerate_01_input_bang();
}

void loadbang_01_startupbang_bang() {
    this->loadbang_01_output_bang();
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

void numbertilde_02_value_set(number ) {}

void numbertilde_01_value_set(number ) {}

void numbertilde_04_value_set(number ) {}

void numbertilde_03_value_set(number ) {}

void numbertilde_06_value_set(number ) {}

void numbertilde_05_value_set(number ) {}

void numbertilde_08_value_set(number ) {}

void numbertilde_07_value_set(number ) {}

void numbertilde_10_value_set(number ) {}

void numbertilde_09_value_set(number ) {}

void numbertilde_12_value_set(number ) {}

void numbertilde_11_value_set(number ) {}

void numbertilde_14_value_set(number ) {}

void numbertilde_13_value_set(number ) {}

void numbertilde_16_value_set(number ) {}

void numbertilde_15_value_set(number ) {}

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
    return 16;
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
    this->p_09->allocateDataRefs();
    this->p_10->allocateDataRefs();
    this->p_11->allocateDataRefs();
    this->p_12->allocateDataRefs();
    this->p_13->allocateDataRefs();
    this->p_14->allocateDataRefs();
    this->p_15->allocateDataRefs();
    this->p_16->allocateDataRefs();
    this->fftstream_tilde_01_win_buf = this->fftstream_tilde_01_win_buf->allocateIfNeeded();

    if (this->RNBODefaultFftWindow->hasRequestedSize()) {
        if (this->RNBODefaultFftWindow->wantsFill())
            this->zeroDataRef(this->RNBODefaultFftWindow);

        this->getEngine()->sendDataRefUpdated(0);
    }
}

void initializeObjects() {
    this->numbertilde_01_init();
    this->numbertilde_02_init();
    this->numbertilde_03_init();
    this->numbertilde_04_init();
    this->numbertilde_05_init();
    this->numbertilde_06_init();
    this->numbertilde_07_init();
    this->gen_01_last_21_init();
    this->gen_01_index_20_init();
    this->gen_01_magnitude_19_init();
    this->gen_01_last_18_init();
    this->gen_01_index_17_init();
    this->gen_01_magnitude_16_init();
    this->gen_01_last_15_init();
    this->gen_01_index_14_init();
    this->gen_01_magnitude_13_init();
    this->gen_01_last_12_init();
    this->gen_01_index_11_init();
    this->gen_01_magnitude_10_init();
    this->gen_01_last_9_init();
    this->gen_01_index_8_init();
    this->gen_01_magnitude_7_init();
    this->gen_01_last_6_init();
    this->gen_01_index_5_init();
    this->gen_01_magnitude_4_init();
    this->gen_01_last_3_init();
    this->gen_01_index_2_init();
    this->gen_01_magnitude_1_init();
    this->gen_01_last_init();
    this->gen_01_index_init();
    this->gen_01_magnitude_init();
    this->numbertilde_08_init();
    this->numbertilde_09_init();
    this->numbertilde_10_init();
    this->numbertilde_11_init();
    this->numbertilde_12_init();
    this->numbertilde_13_init();
    this->numbertilde_14_init();
    this->numbertilde_15_init();
    this->numbertilde_16_init();
    this->p_01->initializeObjects();
    this->p_02->initializeObjects();
    this->p_03->initializeObjects();
    this->p_04->initializeObjects();
    this->p_05->initializeObjects();
    this->p_06->initializeObjects();
    this->p_07->initializeObjects();
    this->p_08->initializeObjects();
    this->p_09->initializeObjects();
    this->p_10->initializeObjects();
    this->p_11->initializeObjects();
    this->p_12->initializeObjects();
    this->p_13->initializeObjects();
    this->p_14->initializeObjects();
    this->p_15->initializeObjects();
    this->p_16->initializeObjects();
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
    this->p_09->startup();
    this->p_10->startup();
    this->p_11->startup();
    this->p_12->startup();
    this->p_13->startup();
    this->p_14->startup();
    this->p_15->startup();
    this->p_16->startup();
    this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    this->processParamInitEvents();
}

void gen_01_framesize_set(number v) {
    this->gen_01_framesize = v;
}

void message_01_out_set(const list& v) {
    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->gen_01_framesize_set(converted);
    }
}

void message_01_bangval_bang() {
    this->sendParameter(1, true);
    this->message_01_out_set({512});
}

void p_16_in5_number_set(number v) {
    this->p_16->updateTime(this->_currentTime);
    this->p_16->eventinlet_08_out1_number_set(v);
}

void p_14_in5_number_set(number v) {
    this->p_14->updateTime(this->_currentTime);
    this->p_14->eventinlet_07_out1_number_set(v);
}

void p_12_in5_number_set(number v) {
    this->p_12->updateTime(this->_currentTime);
    this->p_12->eventinlet_06_out1_number_set(v);
}

void p_10_in5_number_set(number v) {
    this->p_10->updateTime(this->_currentTime);
    this->p_10->eventinlet_05_out1_number_set(v);
}

void p_08_in5_number_set(number v) {
    this->p_08->updateTime(this->_currentTime);
    this->p_08->eventinlet_04_out1_number_set(v);
}

void p_06_in5_number_set(number v) {
    this->p_06->updateTime(this->_currentTime);
    this->p_06->eventinlet_03_out1_number_set(v);
}

void p_04_in5_number_set(number v) {
    this->p_04->updateTime(this->_currentTime);
    this->p_04->eventinlet_02_out1_number_set(v);
}

void p_01_in5_number_set(number v) {
    this->p_01->updateTime(this->_currentTime);
    this->p_01->eventinlet_01_out1_number_set(v);
}

void samplerate_01_samplerate_set(number v) {
    this->p_16_in5_number_set(v);
    this->p_14_in5_number_set(v);
    this->p_12_in5_number_set(v);
    this->p_10_in5_number_set(v);
    this->p_08_in5_number_set(v);
    this->p_06_in5_number_set(v);
    this->p_04_in5_number_set(v);
    this->p_01_in5_number_set(v);
}

void samplerate_01_input_bang() {
    this->samplerate_01_samplerate_set(this->samplerate());
}

void loadbang_01_output_bang() {
    this->message_01_bangval_bang();
    this->samplerate_01_input_bang();
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
    number invFftSize = (number)1 / (number)512;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        SampleIndex i = (SampleIndex)(__fftstream_tilde_01_datapos);
        list out = list(0, 0, i);
        number windowFactor = 1;
        windowFactor = this->hann(i * invFftSize);

        if (__fftstream_tilde_01_datapos < 512) {
            __fftstream_tilde_01_inWorkspace[(Index)(2 * i)] = windowFactor * realIn[(Index)i0];
            out[0] = __fftstream_tilde_01_outWorkspace[(Index)(2 * i)];
            __fftstream_tilde_01_inWorkspace[(Index)(2 * i + 1)] = windowFactor * 0;
            out[1] = __fftstream_tilde_01_outWorkspace[(Index)(2 * i + 1)];

            if (i == 512 - 1) {
                this->fftstream_tilde_01_fft_next(__fftstream_tilde_01_inWorkspace, 512);

                for (SampleIndex j = 0; j < 512 * 2; j++) {
                    __fftstream_tilde_01_outWorkspace[(Index)j] = __fftstream_tilde_01_inWorkspace[(Index)j];
                }
            }

            __fftstream_tilde_01_datapos = (__fftstream_tilde_01_datapos + 1) % 512;
        }

        out1[(Index)i0] = out[0];
        out2[(Index)i0] = out[1];
        out3[(Index)i0] = out[2];
    }

    this->fftstream_tilde_01_datapos = __fftstream_tilde_01_datapos;
}

void dspexpr_89_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    RNBO_UNUSED(in2);
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] / (number)256;//#map:_###_obj_###_:1
    }
}

void dspexpr_90_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    RNBO_UNUSED(in2);
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] / (number)256;//#map:_###_obj_###_:1
    }
}

void cartopol_tilde_01_perform(const Sample * x, const Sample * y, Sample * out1, Sample * out2, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = this->safesqrt(x[(Index)i] * x[(Index)i] + y[(Index)i] * y[(Index)i]);
        out2[(Index)i] = rnbo_atan2(y[(Index)i], x[(Index)i]);
    }
}

void gen_01_perform(
    const Sample * in1,
    const Sample * in2,
    number framesize,
    Sample * out1,
    Sample * out2,
    Sample * out3,
    Sample * out4,
    Sample * out5,
    Sample * out6,
    Sample * out7,
    Sample * out8,
    Index n
) {
    auto __gen_01_last_21_value = this->gen_01_last_21_value;
    auto __gen_01_index_20_value = this->gen_01_index_20_value;
    auto __gen_01_magnitude_19_value = this->gen_01_magnitude_19_value;
    auto __gen_01_last_18_value = this->gen_01_last_18_value;
    auto __gen_01_index_17_value = this->gen_01_index_17_value;
    auto __gen_01_magnitude_16_value = this->gen_01_magnitude_16_value;
    auto __gen_01_last_15_value = this->gen_01_last_15_value;
    auto __gen_01_index_14_value = this->gen_01_index_14_value;
    auto __gen_01_magnitude_13_value = this->gen_01_magnitude_13_value;
    auto __gen_01_last_12_value = this->gen_01_last_12_value;
    auto __gen_01_index_11_value = this->gen_01_index_11_value;
    auto __gen_01_magnitude_10_value = this->gen_01_magnitude_10_value;
    auto __gen_01_last_9_value = this->gen_01_last_9_value;
    auto __gen_01_index_8_value = this->gen_01_index_8_value;
    auto __gen_01_magnitude_7_value = this->gen_01_magnitude_7_value;
    auto __gen_01_last_6_value = this->gen_01_last_6_value;
    auto __gen_01_index_5_value = this->gen_01_index_5_value;
    auto __gen_01_magnitude_4_value = this->gen_01_magnitude_4_value;
    auto __gen_01_last_3_value = this->gen_01_last_3_value;
    auto __gen_01_index_2_value = this->gen_01_index_2_value;
    auto __gen_01_magnitude_1_value = this->gen_01_magnitude_1_value;
    auto __gen_01_last_value = this->gen_01_last_value;
    auto __gen_01_index_value = this->gen_01_index_value;
    auto __gen_01_magnitude_value = this->gen_01_magnitude_value;
    Index i;

    for (i = 0; i < n; i++) {
        number setparam_22_0 = framesize;
        number expr_23_1 = 0;
        number framesize2_2 = framesize / (number)2;

        if (in1[(Index)i] > __gen_01_magnitude_value) {
            if (in2[(Index)i] < framesize2_2) {
                __gen_01_magnitude_value = in1[(Index)i];
                __gen_01_index_value = in2[(Index)i];
            }
        } else {
            expr_23_1 = in1[(Index)i];
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_value = __gen_01_index_value;
            __gen_01_index_value = 0;
            __gen_01_magnitude_value = 0;
        }

        number expr_24_3 = __gen_01_last_value;
        number gen_25_4 = expr_24_3;
        number gen_26_5 = expr_23_1;
        out1[(Index)i] = gen_25_4;
        number expr_27_6 = 0;
        number framesize_28_7 = framesize / (number)2;

        if (gen_26_5 > __gen_01_magnitude_1_value) {
            if (in2[(Index)i] < framesize_28_7) {
                __gen_01_magnitude_1_value = gen_26_5;
                __gen_01_index_2_value = in2[(Index)i];
            }
        } else {
            expr_27_6 = gen_26_5;
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_3_value = __gen_01_index_2_value;
            __gen_01_index_2_value = 0;
            __gen_01_magnitude_1_value = 0;
        }

        number expr_29_8 = __gen_01_last_3_value;
        number gen_30_9 = expr_27_6;
        number gen_31_10 = expr_29_8;
        out2[(Index)i] = gen_31_10;
        number expr_32_11 = 0;
        number framesize_33_12 = framesize / (number)2;

        if (0 > __gen_01_magnitude_4_value) {
            if (in2[(Index)i] < framesize_33_12) {
                __gen_01_magnitude_4_value = 0;
                __gen_01_index_5_value = in2[(Index)i];
            }
        } else {
            expr_32_11 = 0;
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_6_value = __gen_01_index_5_value;
            __gen_01_index_5_value = 0;
            __gen_01_magnitude_4_value = 0;
        }

        number expr_34_13 = __gen_01_last_6_value;
        number gen_35_14 = expr_34_13;
        number gen_36_15 = expr_32_11;
        out8[(Index)i] = gen_35_14;
        number expr_37_16 = 0;
        number framesize_38_17 = framesize / (number)2;

        if (gen_30_9 > __gen_01_magnitude_7_value) {
            if (in2[(Index)i] < framesize_38_17) {
                __gen_01_magnitude_7_value = gen_30_9;
                __gen_01_index_8_value = in2[(Index)i];
            }
        } else {
            expr_37_16 = gen_30_9;
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_9_value = __gen_01_index_8_value;
            __gen_01_index_8_value = 0;
            __gen_01_magnitude_7_value = 0;
        }

        number expr_39_18 = __gen_01_last_9_value;
        number gen_40_19 = expr_39_18;
        number gen_41_20 = expr_37_16;
        out3[(Index)i] = gen_40_19;
        number expr_42_21 = 0;
        number framesize_43_22 = framesize / (number)2;

        if (gen_41_20 > __gen_01_magnitude_10_value) {
            if (in2[(Index)i] < framesize_43_22) {
                __gen_01_magnitude_10_value = gen_41_20;
                __gen_01_index_11_value = in2[(Index)i];
            }
        } else {
            expr_42_21 = gen_41_20;
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_12_value = __gen_01_index_11_value;
            __gen_01_index_11_value = 0;
            __gen_01_magnitude_10_value = 0;
        }

        number expr_44_23 = __gen_01_last_12_value;
        number gen_45_24 = expr_42_21;
        number gen_46_25 = expr_44_23;
        out4[(Index)i] = gen_46_25;
        number expr_47_26 = 0;
        number framesize_48_27 = framesize / (number)2;

        if (gen_45_24 > __gen_01_magnitude_13_value) {
            if (in2[(Index)i] < framesize_48_27) {
                __gen_01_magnitude_13_value = gen_45_24;
                __gen_01_index_14_value = in2[(Index)i];
            }
        } else {
            expr_47_26 = gen_45_24;
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_15_value = __gen_01_index_14_value;
            __gen_01_index_14_value = 0;
            __gen_01_magnitude_13_value = 0;
        }

        number expr_49_28 = __gen_01_last_15_value;
        number gen_50_29 = expr_47_26;
        number gen_51_30 = expr_49_28;
        out5[(Index)i] = gen_51_30;
        number expr_52_31 = 0;
        number framesize_53_32 = framesize / (number)2;

        if (gen_50_29 > __gen_01_magnitude_16_value) {
            if (in2[(Index)i] < framesize_53_32) {
                __gen_01_magnitude_16_value = gen_50_29;
                __gen_01_index_17_value = in2[(Index)i];
            }
        } else {
            expr_52_31 = gen_50_29;
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_18_value = __gen_01_index_17_value;
            __gen_01_index_17_value = 0;
            __gen_01_magnitude_16_value = 0;
        }

        number expr_54_33 = __gen_01_last_18_value;
        number gen_55_34 = expr_52_31;
        number gen_56_35 = expr_54_33;
        out6[(Index)i] = gen_56_35;
        number expr_57_36 = 0;
        number framesize_58_37 = framesize / (number)2;

        if (gen_55_34 > __gen_01_magnitude_19_value) {
            if (in2[(Index)i] < framesize_58_37) {
                __gen_01_magnitude_19_value = gen_55_34;
                __gen_01_index_20_value = in2[(Index)i];
            }
        } else {
            expr_57_36 = gen_55_34;
        }

        if (in2[(Index)i] == framesize - 1) {
            __gen_01_last_21_value = __gen_01_index_20_value;
            __gen_01_index_20_value = 0;
            __gen_01_magnitude_19_value = 0;
        }

        number expr_59_38 = __gen_01_last_21_value;
        number gen_60_39 = expr_59_38;
        number gen_61_40 = expr_57_36;
        out7[(Index)i] = gen_60_39;
    }

    this->gen_01_magnitude_value = __gen_01_magnitude_value;
    this->gen_01_index_value = __gen_01_index_value;
    this->gen_01_last_value = __gen_01_last_value;
    this->gen_01_magnitude_1_value = __gen_01_magnitude_1_value;
    this->gen_01_index_2_value = __gen_01_index_2_value;
    this->gen_01_last_3_value = __gen_01_last_3_value;
    this->gen_01_magnitude_4_value = __gen_01_magnitude_4_value;
    this->gen_01_index_5_value = __gen_01_index_5_value;
    this->gen_01_last_6_value = __gen_01_last_6_value;
    this->gen_01_magnitude_7_value = __gen_01_magnitude_7_value;
    this->gen_01_index_8_value = __gen_01_index_8_value;
    this->gen_01_last_9_value = __gen_01_last_9_value;
    this->gen_01_magnitude_10_value = __gen_01_magnitude_10_value;
    this->gen_01_index_11_value = __gen_01_index_11_value;
    this->gen_01_last_12_value = __gen_01_last_12_value;
    this->gen_01_magnitude_13_value = __gen_01_magnitude_13_value;
    this->gen_01_index_14_value = __gen_01_index_14_value;
    this->gen_01_last_15_value = __gen_01_last_15_value;
    this->gen_01_magnitude_16_value = __gen_01_magnitude_16_value;
    this->gen_01_index_17_value = __gen_01_index_17_value;
    this->gen_01_last_18_value = __gen_01_last_18_value;
    this->gen_01_magnitude_19_value = __gen_01_magnitude_19_value;
    this->gen_01_index_20_value = __gen_01_index_20_value;
    this->gen_01_last_21_value = __gen_01_last_21_value;
}

void p_01_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_01->process(ins, 4, outs, 2, n);
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
            -1566058454,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_02_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-11"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_02_currentInterval = __numbertilde_02_currentInterval;
    this->numbertilde_02_lastValue = __numbertilde_02_lastValue;
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
            2027079339,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_01_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-9"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_01_currentInterval = __numbertilde_01_currentInterval;
    this->numbertilde_01_lastValue = __numbertilde_01_lastValue;
}

void p_02_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_02->process(ins, 1, outs, 1, n);
}

void p_04_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_04->process(ins, 4, outs, 2, n);
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
            -162399448,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_04_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-12"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_04_currentInterval = __numbertilde_04_currentInterval;
    this->numbertilde_04_lastValue = __numbertilde_04_lastValue;
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
            -864228951,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_03_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-19"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_03_currentInterval = __numbertilde_03_currentInterval;
    this->numbertilde_03_lastValue = __numbertilde_03_lastValue;
}

void p_03_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_03->process(ins, 1, outs, 1, n);
}

void p_06_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_06->process(ins, 4, outs, 2, n);
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
            1241259558,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_06_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-23"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_06_currentInterval = __numbertilde_06_currentInterval;
    this->numbertilde_06_lastValue = __numbertilde_06_lastValue;
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
            539430055,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_05_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-24"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_05_currentInterval = __numbertilde_05_currentInterval;
    this->numbertilde_05_lastValue = __numbertilde_05_lastValue;
}

void p_05_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_05->process(ins, 1, outs, 1, n);
}

void signaladder_01_perform(const Sample * in1, const Sample * in2, Sample * out, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out[(Index)i] = in1[(Index)i] + in2[(Index)i];
    }
}

void p_08_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_08->process(ins, 4, outs, 2, n);
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
            1480712303,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_08_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-30"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_08_currentInterval = __numbertilde_08_currentInterval;
    this->numbertilde_08_lastValue = __numbertilde_08_lastValue;
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
            1943089061,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_07_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-31"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_07_currentInterval = __numbertilde_07_currentInterval;
    this->numbertilde_07_lastValue = __numbertilde_07_lastValue;
}

void p_07_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_07->process(ins, 1, outs, 1, n);
}

void p_10_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_10->process(ins, 4, outs, 2, n);
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
            -1410595987,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_10_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-34"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_10_currentInterval = __numbertilde_10_currentInterval;
    this->numbertilde_10_lastValue = __numbertilde_10_lastValue;
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
            -2112425490,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_09_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-35"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_09_currentInterval = __numbertilde_09_currentInterval;
    this->numbertilde_09_lastValue = __numbertilde_09_lastValue;
}

void p_09_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_09->process(ins, 1, outs, 1, n);
}

void p_12_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_12->process(ins, 4, outs, 2, n);
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
            -6936981,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_12_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-13"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_12_currentInterval = __numbertilde_12_currentInterval;
    this->numbertilde_12_lastValue = __numbertilde_12_lastValue;
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
            -708766484,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_11_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-14"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_11_currentInterval = __numbertilde_11_currentInterval;
    this->numbertilde_11_lastValue = __numbertilde_11_lastValue;
}

void p_11_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_11->process(ins, 1, outs, 1, n);
}

void p_14_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_14->process(ins, 4, outs, 2, n);
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
            1396722025,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_14_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-22"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_14_currentInterval = __numbertilde_14_currentInterval;
    this->numbertilde_14_lastValue = __numbertilde_14_lastValue;
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
            694892522,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_13_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-25"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_13_currentInterval = __numbertilde_13_currentInterval;
    this->numbertilde_13_lastValue = __numbertilde_13_lastValue;
}

void p_13_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_13->process(ins, 1, outs, 1, n);
}

void p_16_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * out1,
    Sample * out2,
    Index n
) {
    SampleArray<4> ins = {in1, in2, in3, in4};
    SampleArray<2> outs = {out1, out2};
    this->p_16->process(ins, 4, outs, 2, n);
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
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-36"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_16_currentInterval = __numbertilde_16_currentInterval;
    this->numbertilde_16_lastValue = __numbertilde_16_lastValue;
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
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-37"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_15_currentInterval = __numbertilde_15_currentInterval;
    this->numbertilde_15_lastValue = __numbertilde_15_lastValue;
}

void p_15_perform(Sample * in1, Sample * out1, Index n) {
    // subpatcher: voct
    SampleArray<1> ins = {in1};

    SampleArray<1> outs = {out1};
    this->p_15->process(ins, 1, outs, 1, n);
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-9"), 1, this->_currentTime);
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

    for (int i = 0; i < 512 * 2; i++) {
        this->fftstream_tilde_01_inWorkspace[(Index)i] = 0;
        this->fftstream_tilde_01_outWorkspace[(Index)i] = 0;
    }

    int safeframesize = (int)(512);

    {
        safeframesize = nextpoweroftwo(512);
    }

    int framepos = (int)(0 % safeframesize);

    if (framepos < 512) {
        this->fftstream_tilde_01_datapos = framepos;
    } else {
        this->fftstream_tilde_01_datapos = 0;
    }

    this->fftstream_tilde_01_setupDone = true;
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-11"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-19"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-12"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-24"), 1, this->_currentTime);
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

void samplerate_01_onSampleRateChanged(number samplerate) {
    this->samplerate_01_samplerate_set(samplerate);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-23"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-31"), 1, this->_currentTime);
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

number gen_01_last_21_getvalue() {
    return this->gen_01_last_21_value;
}

void gen_01_last_21_setvalue(number val) {
    this->gen_01_last_21_value = val;
}

void gen_01_last_21_reset() {
    this->gen_01_last_21_value = 0;
}

void gen_01_last_21_init() {
    this->gen_01_last_21_value = 0;
}

number gen_01_index_20_getvalue() {
    return this->gen_01_index_20_value;
}

void gen_01_index_20_setvalue(number val) {
    this->gen_01_index_20_value = val;
}

void gen_01_index_20_reset() {
    this->gen_01_index_20_value = 0;
}

void gen_01_index_20_init() {
    this->gen_01_index_20_value = 0;
}

number gen_01_magnitude_19_getvalue() {
    return this->gen_01_magnitude_19_value;
}

void gen_01_magnitude_19_setvalue(number val) {
    this->gen_01_magnitude_19_value = val;
}

void gen_01_magnitude_19_reset() {
    this->gen_01_magnitude_19_value = 0;
}

void gen_01_magnitude_19_init() {
    this->gen_01_magnitude_19_value = 0;
}

number gen_01_last_18_getvalue() {
    return this->gen_01_last_18_value;
}

void gen_01_last_18_setvalue(number val) {
    this->gen_01_last_18_value = val;
}

void gen_01_last_18_reset() {
    this->gen_01_last_18_value = 0;
}

void gen_01_last_18_init() {
    this->gen_01_last_18_value = 0;
}

number gen_01_index_17_getvalue() {
    return this->gen_01_index_17_value;
}

void gen_01_index_17_setvalue(number val) {
    this->gen_01_index_17_value = val;
}

void gen_01_index_17_reset() {
    this->gen_01_index_17_value = 0;
}

void gen_01_index_17_init() {
    this->gen_01_index_17_value = 0;
}

number gen_01_magnitude_16_getvalue() {
    return this->gen_01_magnitude_16_value;
}

void gen_01_magnitude_16_setvalue(number val) {
    this->gen_01_magnitude_16_value = val;
}

void gen_01_magnitude_16_reset() {
    this->gen_01_magnitude_16_value = 0;
}

void gen_01_magnitude_16_init() {
    this->gen_01_magnitude_16_value = 0;
}

number gen_01_last_15_getvalue() {
    return this->gen_01_last_15_value;
}

void gen_01_last_15_setvalue(number val) {
    this->gen_01_last_15_value = val;
}

void gen_01_last_15_reset() {
    this->gen_01_last_15_value = 0;
}

void gen_01_last_15_init() {
    this->gen_01_last_15_value = 0;
}

number gen_01_index_14_getvalue() {
    return this->gen_01_index_14_value;
}

void gen_01_index_14_setvalue(number val) {
    this->gen_01_index_14_value = val;
}

void gen_01_index_14_reset() {
    this->gen_01_index_14_value = 0;
}

void gen_01_index_14_init() {
    this->gen_01_index_14_value = 0;
}

number gen_01_magnitude_13_getvalue() {
    return this->gen_01_magnitude_13_value;
}

void gen_01_magnitude_13_setvalue(number val) {
    this->gen_01_magnitude_13_value = val;
}

void gen_01_magnitude_13_reset() {
    this->gen_01_magnitude_13_value = 0;
}

void gen_01_magnitude_13_init() {
    this->gen_01_magnitude_13_value = 0;
}

number gen_01_last_12_getvalue() {
    return this->gen_01_last_12_value;
}

void gen_01_last_12_setvalue(number val) {
    this->gen_01_last_12_value = val;
}

void gen_01_last_12_reset() {
    this->gen_01_last_12_value = 0;
}

void gen_01_last_12_init() {
    this->gen_01_last_12_value = 0;
}

number gen_01_index_11_getvalue() {
    return this->gen_01_index_11_value;
}

void gen_01_index_11_setvalue(number val) {
    this->gen_01_index_11_value = val;
}

void gen_01_index_11_reset() {
    this->gen_01_index_11_value = 0;
}

void gen_01_index_11_init() {
    this->gen_01_index_11_value = 0;
}

number gen_01_magnitude_10_getvalue() {
    return this->gen_01_magnitude_10_value;
}

void gen_01_magnitude_10_setvalue(number val) {
    this->gen_01_magnitude_10_value = val;
}

void gen_01_magnitude_10_reset() {
    this->gen_01_magnitude_10_value = 0;
}

void gen_01_magnitude_10_init() {
    this->gen_01_magnitude_10_value = 0;
}

number gen_01_last_9_getvalue() {
    return this->gen_01_last_9_value;
}

void gen_01_last_9_setvalue(number val) {
    this->gen_01_last_9_value = val;
}

void gen_01_last_9_reset() {
    this->gen_01_last_9_value = 0;
}

void gen_01_last_9_init() {
    this->gen_01_last_9_value = 0;
}

number gen_01_index_8_getvalue() {
    return this->gen_01_index_8_value;
}

void gen_01_index_8_setvalue(number val) {
    this->gen_01_index_8_value = val;
}

void gen_01_index_8_reset() {
    this->gen_01_index_8_value = 0;
}

void gen_01_index_8_init() {
    this->gen_01_index_8_value = 0;
}

number gen_01_magnitude_7_getvalue() {
    return this->gen_01_magnitude_7_value;
}

void gen_01_magnitude_7_setvalue(number val) {
    this->gen_01_magnitude_7_value = val;
}

void gen_01_magnitude_7_reset() {
    this->gen_01_magnitude_7_value = 0;
}

void gen_01_magnitude_7_init() {
    this->gen_01_magnitude_7_value = 0;
}

number gen_01_last_6_getvalue() {
    return this->gen_01_last_6_value;
}

void gen_01_last_6_setvalue(number val) {
    this->gen_01_last_6_value = val;
}

void gen_01_last_6_reset() {
    this->gen_01_last_6_value = 0;
}

void gen_01_last_6_init() {
    this->gen_01_last_6_value = 0;
}

number gen_01_index_5_getvalue() {
    return this->gen_01_index_5_value;
}

void gen_01_index_5_setvalue(number val) {
    this->gen_01_index_5_value = val;
}

void gen_01_index_5_reset() {
    this->gen_01_index_5_value = 0;
}

void gen_01_index_5_init() {
    this->gen_01_index_5_value = 0;
}

number gen_01_magnitude_4_getvalue() {
    return this->gen_01_magnitude_4_value;
}

void gen_01_magnitude_4_setvalue(number val) {
    this->gen_01_magnitude_4_value = val;
}

void gen_01_magnitude_4_reset() {
    this->gen_01_magnitude_4_value = 0;
}

void gen_01_magnitude_4_init() {
    this->gen_01_magnitude_4_value = 0;
}

number gen_01_last_3_getvalue() {
    return this->gen_01_last_3_value;
}

void gen_01_last_3_setvalue(number val) {
    this->gen_01_last_3_value = val;
}

void gen_01_last_3_reset() {
    this->gen_01_last_3_value = 0;
}

void gen_01_last_3_init() {
    this->gen_01_last_3_value = 0;
}

number gen_01_index_2_getvalue() {
    return this->gen_01_index_2_value;
}

void gen_01_index_2_setvalue(number val) {
    this->gen_01_index_2_value = val;
}

void gen_01_index_2_reset() {
    this->gen_01_index_2_value = 0;
}

void gen_01_index_2_init() {
    this->gen_01_index_2_value = 0;
}

number gen_01_magnitude_1_getvalue() {
    return this->gen_01_magnitude_1_value;
}

void gen_01_magnitude_1_setvalue(number val) {
    this->gen_01_magnitude_1_value = val;
}

void gen_01_magnitude_1_reset() {
    this->gen_01_magnitude_1_value = 0;
}

void gen_01_magnitude_1_init() {
    this->gen_01_magnitude_1_value = 0;
}

number gen_01_last_getvalue() {
    return this->gen_01_last_value;
}

void gen_01_last_setvalue(number val) {
    this->gen_01_last_value = val;
}

void gen_01_last_reset() {
    this->gen_01_last_value = 0;
}

void gen_01_last_init() {
    this->gen_01_last_value = 0;
}

number gen_01_index_getvalue() {
    return this->gen_01_index_value;
}

void gen_01_index_setvalue(number val) {
    this->gen_01_index_value = val;
}

void gen_01_index_reset() {
    this->gen_01_index_value = 0;
}

void gen_01_index_init() {
    this->gen_01_index_value = 0;
}

number gen_01_magnitude_getvalue() {
    return this->gen_01_magnitude_value;
}

void gen_01_magnitude_setvalue(number val) {
    this->gen_01_magnitude_value = val;
}

void gen_01_magnitude_reset() {
    this->gen_01_magnitude_value = 0;
}

void gen_01_magnitude_init() {
    this->gen_01_magnitude_value = 0;
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-30"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-35"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-34"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-14"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-13"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-25"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-22"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-37"), 1, this->_currentTime);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-36"), 1, this->_currentTime);
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
    p_01_target = 0;
    dspexpr_89_in1 = 0;
    dspexpr_89_in2 = 256;
    p_02_target = 0;
    numbertilde_01_input_number = 0;
    numbertilde_01_ramp = 0;
    cartopol_tilde_01_x = 0;
    cartopol_tilde_01_y = 0;
    fftstream_tilde_01_realIn = 0;
    fftstream_tilde_01_imagIn = 0;
    dspexpr_90_in1 = 0;
    dspexpr_90_in2 = 256;
    numbertilde_02_input_number = 0;
    numbertilde_02_ramp = 0;
    p_03_target = 0;
    numbertilde_03_input_number = 0;
    numbertilde_03_ramp = 0;
    p_04_target = 0;
    numbertilde_04_input_number = 0;
    numbertilde_04_ramp = 0;
    p_05_target = 0;
    numbertilde_05_input_number = 0;
    numbertilde_05_ramp = 0;
    numbertilde_06_input_number = 0;
    numbertilde_06_ramp = 0;
    p_06_target = 0;
    p_07_target = 0;
    numbertilde_07_input_number = 0;
    numbertilde_07_ramp = 0;
    gen_01_in1 = 0;
    gen_01_in2 = 0;
    gen_01_framesize = 512;
    numbertilde_08_input_number = 0;
    numbertilde_08_ramp = 0;
    p_08_target = 0;
    p_09_target = 0;
    numbertilde_09_input_number = 0;
    numbertilde_09_ramp = 0;
    numbertilde_10_input_number = 0;
    numbertilde_10_ramp = 0;
    p_10_target = 0;
    p_11_target = 0;
    numbertilde_11_input_number = 0;
    numbertilde_11_ramp = 0;
    numbertilde_12_input_number = 0;
    numbertilde_12_ramp = 0;
    p_12_target = 0;
    p_13_target = 0;
    numbertilde_13_input_number = 0;
    numbertilde_13_ramp = 0;
    numbertilde_14_input_number = 0;
    numbertilde_14_ramp = 0;
    p_14_target = 0;
    p_15_target = 0;
    numbertilde_15_input_number = 0;
    numbertilde_15_ramp = 0;
    numbertilde_16_input_number = 0;
    numbertilde_16_ramp = 0;
    p_16_target = 0;
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
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
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
    fftstream_tilde_01_datapos = 0;
    fftstream_tilde_01_fft_lastsize = 0;
    fftstream_tilde_01_fft_levels = -1;
    fftstream_tilde_01_setupDone = false;
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
    gen_01_last_21_value = 0;
    gen_01_index_20_value = 0;
    gen_01_magnitude_19_value = 0;
    gen_01_last_18_value = 0;
    gen_01_index_17_value = 0;
    gen_01_magnitude_16_value = 0;
    gen_01_last_15_value = 0;
    gen_01_index_14_value = 0;
    gen_01_magnitude_13_value = 0;
    gen_01_last_12_value = 0;
    gen_01_index_11_value = 0;
    gen_01_magnitude_10_value = 0;
    gen_01_last_9_value = 0;
    gen_01_index_8_value = 0;
    gen_01_magnitude_7_value = 0;
    gen_01_last_6_value = 0;
    gen_01_index_5_value = 0;
    gen_01_magnitude_4_value = 0;
    gen_01_last_3_value = 0;
    gen_01_index_2_value = 0;
    gen_01_magnitude_1_value = 0;
    gen_01_last_value = 0;
    gen_01_index_value = 0;
    gen_01_magnitude_value = 0;
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

    number p_01_target;
    number dspexpr_89_in1;
    number dspexpr_89_in2;
    number p_02_target;
    number numbertilde_01_input_number;
    number numbertilde_01_ramp;
    number cartopol_tilde_01_x;
    number cartopol_tilde_01_y;
    number fftstream_tilde_01_realIn;
    number fftstream_tilde_01_imagIn;
    number dspexpr_90_in1;
    number dspexpr_90_in2;
    number numbertilde_02_input_number;
    number numbertilde_02_ramp;
    number p_03_target;
    number numbertilde_03_input_number;
    number numbertilde_03_ramp;
    number p_04_target;
    number numbertilde_04_input_number;
    number numbertilde_04_ramp;
    number p_05_target;
    number numbertilde_05_input_number;
    number numbertilde_05_ramp;
    number numbertilde_06_input_number;
    number numbertilde_06_ramp;
    number p_06_target;
    number p_07_target;
    number numbertilde_07_input_number;
    number numbertilde_07_ramp;
    number gen_01_in1;
    number gen_01_in2;
    number gen_01_framesize;
    number numbertilde_08_input_number;
    number numbertilde_08_ramp;
    number p_08_target;
    number p_09_target;
    number numbertilde_09_input_number;
    number numbertilde_09_ramp;
    number numbertilde_10_input_number;
    number numbertilde_10_ramp;
    number p_10_target;
    number p_11_target;
    number numbertilde_11_input_number;
    number numbertilde_11_ramp;
    number numbertilde_12_input_number;
    number numbertilde_12_ramp;
    number p_12_target;
    number p_13_target;
    number numbertilde_13_input_number;
    number numbertilde_13_ramp;
    number numbertilde_14_input_number;
    number numbertilde_14_ramp;
    number p_14_target;
    number p_15_target;
    number numbertilde_15_input_number;
    number numbertilde_15_ramp;
    number numbertilde_16_input_number;
    number numbertilde_16_ramp;
    number p_16_target;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[13];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
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
    SampleValue fftstream_tilde_01_inWorkspace[1024] = { };
    SampleValue fftstream_tilde_01_outWorkspace[1024] = { };
    Float32BufferRef fftstream_tilde_01_win_buf;
    SampleIndex fftstream_tilde_01_datapos;
    int fftstream_tilde_01_fft_lastsize;
    list fftstream_tilde_01_fft_costab;
    list fftstream_tilde_01_fft_sintab;
    int fftstream_tilde_01_fft_levels;
    bool fftstream_tilde_01_setupDone;
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
    number gen_01_last_21_value;
    number gen_01_index_20_value;
    number gen_01_magnitude_19_value;
    number gen_01_last_18_value;
    number gen_01_index_17_value;
    number gen_01_magnitude_16_value;
    number gen_01_last_15_value;
    number gen_01_index_14_value;
    number gen_01_magnitude_13_value;
    number gen_01_last_12_value;
    number gen_01_index_11_value;
    number gen_01_magnitude_10_value;
    number gen_01_last_9_value;
    number gen_01_index_8_value;
    number gen_01_magnitude_7_value;
    number gen_01_last_6_value;
    number gen_01_index_5_value;
    number gen_01_magnitude_4_value;
    number gen_01_last_3_value;
    number gen_01_index_2_value;
    number gen_01_magnitude_1_value;
    number gen_01_last_value;
    number gen_01_index_value;
    number gen_01_magnitude_value;
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
    RNBOSubpatcher_1199* p_01;
    RNBOSubpatcher_1200* p_02;
    RNBOSubpatcher_1201* p_03;
    RNBOSubpatcher_1202* p_04;
    RNBOSubpatcher_1203* p_05;
    RNBOSubpatcher_1204* p_06;
    RNBOSubpatcher_1205* p_07;
    RNBOSubpatcher_1206* p_08;
    RNBOSubpatcher_1207* p_09;
    RNBOSubpatcher_1208* p_10;
    RNBOSubpatcher_1209* p_11;
    RNBOSubpatcher_1210* p_12;
    RNBOSubpatcher_1211* p_13;
    RNBOSubpatcher_1212* p_14;
    RNBOSubpatcher_1213* p_15;
    RNBOSubpatcher_1214* p_16;

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

