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

class RNBOSubpatcher_98 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_98()
    {
    }
    
    ~RNBOSubpatcher_98()
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
        getEngine()->flushClockEvents(this, -871642103, false);
    }
    
    inline number safemod(number f, number m) {
        if (m != 0) {
            if (m < 0) {
                m = -m;
            }
    
            if (f >= m) {
                if (f >= m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f -= m;
                }
            } else if (f <= -m) {
                if (f <= -m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f += m;
                }
            }
        } else {
            f = 0.0;
        }
    
        return f;
    }
    
    inline number safesqrt(number num) {
        return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index bufferbindchannel(const Index channel, const Index maxChannels, const int channelmode) {
        if (channelmode == 0 || channelmode == 4) {
            return maxChannels - 1 - channel % maxChannels;
        } else if (channelmode == 3) {
            return (maxChannels == 0 ? 0 : channel % maxChannels);
        } else {
            return (channel > maxChannels - 1 ? maxChannels - 1 : (channel < 0 ? 0 : channel));
        }
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
    
    SampleIndex bufferbindindex(
        const SampleValue index,
        const SampleIndex start,
        const SampleIndex end,
        const int boundmode
    ) {
        if (boundmode == 0 || boundmode == 4) {
            if (index >= start && index < end)
                return index;
            else {
                SampleIndex length = (SampleIndex)(end - start);
                number index1 = index - start;
                number wrap = (index1 < 0 ? length * 2 - 1 + this->safemod(index1 + 1, length * 2) : this->safemod(index1, length * 2));
                return (start + wrap >= length ? length * 2 - wrap - 1 : wrap);
            }
        } else if (boundmode == 3) {
            return this->wrap(index, start, end);
        } else {
            return (index > end - 1 ? end - 1 : (index < start ? start : index));
        }
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
    
    template <typename T> inline SampleValue bufferreadsample(
        T& buffer,
        const SampleValue sampleIndex,
        const Index channel,
        const SampleIndex start,
        const SampleIndex end,
        const int interp
    ) {
        if (sampleIndex < 0.0) {
            return 0.0;
        }
    
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(sampleIndex));
    
        if (interp == 0) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->linearinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else if (interp == 1) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->cubicinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 2) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->splineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 3) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->cosineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else {
            return buffer->getSample(channel, index1);
        }
    }
    
    number hann(number x) {
        x = (x > 1 ? 1 : (x < 0 ? 0 : x));
        return 0.5 * (1 - rnbo_cos(6.28318530717958647692 * x));
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "start")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "len")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "rate")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "pan")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "size")) {
            return 5;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        this->dspexpr_02_perform(in2, this->dspexpr_02_in2, this->signals[0], n);
        this->dspexpr_06_perform(in3, this->dspexpr_06_in2, this->signals[1], n);
        this->dspexpr_07_perform(in4, this->dspexpr_07_in2, this->signals[2], n);
        this->dspexpr_08_perform(in5, this->dspexpr_08_in2, this->signals[3], n);
        this->dspexpr_05_perform(this->signals[3], this->dspexpr_05_in2, this->signals[4], n);
        this->dspexpr_04_perform(this->signals[4], this->dspexpr_04_in2, this->signals[3], n);
    
        this->granulator_01_perform(
            this->signals[0],
            this->signals[1],
            this->signals[2],
            this->granulator_01_bchan,
            this->signals[3],
            in1,
            this->signals[4],
            this->signals[5],
            n
        );
    
        this->dspexpr_01_perform(this->signals[4], this->dspexpr_01_in2, out1, n);
        this->dspexpr_03_perform(this->signals[5], this->dspexpr_03_in2, out2, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 6; i++) {
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
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_01_getPresetValue(getSubState(preset, "start"));
        this->param_02_getPresetValue(getSubState(preset, "len"));
        this->param_03_getPresetValue(getSubState(preset, "gain"));
        this->param_04_getPresetValue(getSubState(preset, "rate"));
        this->param_05_getPresetValue(getSubState(preset, "pan"));
        this->param_06_getPresetValue(getSubState(preset, "size"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double samplerate) {
        this->granulator_01_onSampleRateChanged(samplerate);
        this->samplerate_01_onSampleRateChanged(samplerate);
    }
    
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
        return 6;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "start";
        case 1:
            return "len";
        case 2:
            return "gain";
        case 3:
            return "rate";
        case 4:
            return "pan";
        case 5:
            return "size";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer1/start";
        case 1:
            return "layer1/len";
        case 2:
            return "layer1/gain";
        case 3:
            return "layer1/rate";
        case 4:
            return "layer1/pan";
        case 5:
            return "layer1/size";
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
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Start";
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
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Length";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
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
                info->displayName = "Gain";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
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
                info->displayName = "Rate";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = -1;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Pan";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 180;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Size";
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
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 180 ? 180 : value));
                ParameterValue normalizedValue = (value - 0) / (180 - 0);
                return normalizedValue;
            }
        case 3:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        case 4:
            {
                value = (value < -1 ? -1 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - -1) / (1 - -1);
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
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (180 - 0);
                }
            }
        case 3:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -1 + value * (1 - -1);
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
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -871642103:
            this->loadbang_01_startupbang_bang();
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("startupbang"):
            if (TAG("layer1/loadbang_obj-42") == objectId)
                this->loadbang_01_startupbang_bang();
    
            break;
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("startupbang"):
            return "startupbang";
        case TAG("layer1/loadbang_obj-42"):
            return "layer1/loadbang_obj-42";
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
            this->granulator_01_buffer = new Float32Buffer(this->getPatcher()->layer1);
            this->granulator_01_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->granulator_01_buffer = new Float32Buffer(this->getPatcher()->layer1);
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
    
        this->dspexpr_02_in2_set(v);
    }
    
    void param_02_value_set(number v) {
        v = this->param_02_value_constrain(v);
        this->param_02_value = v;
        this->sendParameter(1, false);
    
        if (this->param_02_value != this->param_02_lastValue) {
            this->getEngine()->presetTouched();
            this->param_02_lastValue = this->param_02_value;
        }
    
        this->dspexpr_06_in2_set(v);
    }
    
    void param_03_value_set(number v) {
        v = this->param_03_value_constrain(v);
        this->param_03_value = v;
        this->sendParameter(2, false);
    
        if (this->param_03_value != this->param_03_lastValue) {
            this->getEngine()->presetTouched();
            this->param_03_lastValue = this->param_03_value;
        }
    
        this->dspexpr_03_in2_set(v);
        this->dspexpr_01_in2_set(v);
    }
    
    void param_04_value_set(number v) {
        v = this->param_04_value_constrain(v);
        this->param_04_value = v;
        this->sendParameter(3, false);
    
        if (this->param_04_value != this->param_04_lastValue) {
            this->getEngine()->presetTouched();
            this->param_04_lastValue = this->param_04_value;
        }
    
        this->dspexpr_07_in2_set(v);
    }
    
    void param_05_value_set(number v) {
        v = this->param_05_value_constrain(v);
        this->param_05_value = v;
        this->sendParameter(4, false);
    
        if (this->param_05_value != this->param_05_lastValue) {
            this->getEngine()->presetTouched();
            this->param_05_lastValue = this->param_05_value;
        }
    
        this->dspexpr_08_in2_set(v);
    }
    
    void param_06_value_set(number v) {
        v = this->param_06_value_constrain(v);
        this->param_06_value = v;
        this->sendParameter(5, false);
    
        if (this->param_06_value != this->param_06_lastValue) {
            this->getEngine()->presetTouched();
            this->param_06_lastValue = this->param_06_value;
        }
    
        this->expr_01_in1_set(v);
    }
    
    void loadbang_01_startupbang_bang() {
        this->loadbang_01_output_bang();
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
        return 5;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->granulator_01_edgedetect_init();
        this->granulator_01_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    
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
    }
    
    void allocateDataRefs() {
        this->granulator_01_buffer = this->granulator_01_buffer->allocateIfNeeded();
    }
    
    static number param_01_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_02_in2_set(number v) {
        this->dspexpr_02_in2 = v;
    }
    
    static number param_02_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_06_in2_set(number v) {
        this->dspexpr_06_in2 = v;
    }
    
    static number param_03_value_constrain(number v) {
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
    
    void dspexpr_01_in2_set(number v) {
        this->dspexpr_01_in2 = v;
    }
    
    static number param_04_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void dspexpr_07_in2_set(number v) {
        this->dspexpr_07_in2 = v;
    }
    
    static number param_05_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < -1 ? -1 : v));
        return v;
    }
    
    void dspexpr_08_in2_set(number v) {
        this->dspexpr_08_in2 = v;
    }
    
    static number param_06_value_constrain(number v) {
        v = (v > 180 ? 180 : (v < 0 ? 0 : v));
        return v;
    }
    
    void eventoutlet_01_in1_number_set(number v) {
        this->getPatcher()->p_01_out3_number_set(v);
    }
    
    void expr_01_out1_set(number v) {
        this->expr_01_out1 = v;
        this->eventoutlet_01_in1_number_set(this->expr_01_out1);
    }
    
    void expr_01_in1_set(number in1) {
        this->expr_01_in1 = in1;
        this->expr_01_out1_set(this->expr_01_in1 * this->expr_01_in2);//#map:layer1/*_obj-43:1
    }
    
    void expr_01_in2_set(number v) {
        this->expr_01_in2 = v;
    }
    
    void expr_02_out1_set(number v) {
        this->expr_02_out1 = v;
        this->expr_01_in2_set(this->expr_02_out1);
    }
    
    void expr_02_in1_set(number in1) {
        this->expr_02_in1 = in1;
    
        this->expr_02_out1_set(
            (this->expr_02_in2 == 0 ? 0 : (this->expr_02_in2 == 0. ? 0. : this->expr_02_in1 / this->expr_02_in2))
        );//#map:layer1//_obj-44:1
    }
    
    void samplerate_01_samplerate_set(number v) {
        this->expr_02_in1_set(v);
    }
    
    void samplerate_01_input_bang() {
        this->samplerate_01_samplerate_set(this->samplerate());
    }
    
    void loadbang_01_output_bang() {
        this->samplerate_01_input_bang();
    }
    
    void dspexpr_02_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_06_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_07_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_08_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_05_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_04_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void granulator_01_perform(
        const Sample * start,
        const Sample * length,
        const Sample * rate,
        number bchan,
        const Sample * panpos,
        const Sample * trigger,
        Sample * out1,
        Sample * out2,
        Index n
    ) {
        RNBO_UNUSED(bchan);
        auto __granulator_01_grainPan = this->granulator_01_grainPan;
        auto __granulator_01_lengthMul = this->granulator_01_lengthMul;
        auto __granulator_01_startMul = this->granulator_01_startMul;
        auto __granulator_01_limitLast = this->granulator_01_limitLast;
        auto __granulator_01_limit = this->granulator_01_limit;
        number o = 0;
        Index i;
    
        for (i = 0; i < n; i++) {
            bool trig = (bool)(this->granulator_01_edgedetect_next(trigger[(Index)i]) > 0.0);
            int limit = (int)((__granulator_01_limit > 32 ? 32 : (__granulator_01_limit < 0 ? 0 : __granulator_01_limit)));
            __granulator_01_limitLast = limit;
            out1[(Index)i] = 0;
            out2[(Index)i] = 0;
            number s = start[(Index)i];
            number r = rate[(Index)i];
            number l = length[(Index)i];
    
            for (Index j = 0; j < 32; j++) {
                number thistrig = 0;
    
                if ((bool)(trig) && ((bool)(this->granulator_01_grains_inactive(j)) && j < limit)) {
                    trig = false;
                    thistrig = 1;
    
                    {
                        s *= __granulator_01_startMul;
                    }
    
                    {
                        l *= __granulator_01_lengthMul;
                    }
    
                    __granulator_01_grainPan[(Index)j][0] = this->granulator_01_panfunc_next(panpos[(Index)i], 0);
                    __granulator_01_grainPan[(Index)j][1] = this->granulator_01_panfunc_next(panpos[(Index)i], 1);
                }
    
                if (thistrig != 0 || (bool)(!(bool)(this->granulator_01_grains_inactive(j)))) {
                    array<number, 3> ret = this->granulator_01_grains_next(j, this->granulator_01_buffer, thistrig, s, l, r, 0);
                    number v = ret[0] * this->hann(ret[1]);
                    out1[(Index)i] += v * __granulator_01_grainPan[(Index)j][0];
                    out2[(Index)i] += v * __granulator_01_grainPan[(Index)j][1];
                }
            }
        }
    
        this->granulator_01_limitLast = __granulator_01_limitLast;
    }
    
    void dspexpr_01_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
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
    
    number granulator_01_edgedetect_next(number val) {
        if ((0 == 0 && val <= 0) || (0 == 1 && val > 0)) {
            this->granulator_01_edgedetect_active = false;
        } else if ((bool)(!(bool)(this->granulator_01_edgedetect_active))) {
            this->granulator_01_edgedetect_active = true;
            return 1.0;
        }
    
        return 0.0;
    }
    
    void granulator_01_edgedetect_init() {}
    
    void granulator_01_edgedetect_reset() {
        this->granulator_01_edgedetect_active = false;
    }
    
    number granulator_01_panfunc_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    {
                        if (pos >= 0) {
                            pos = this->safemod(pos, 2);
                        } else {
                            pos = this->safemod(2 + this->safemod(pos, 2), 2);
                        }
    
                        if (channel == 0 && pos >= nchan_1)
                            pos = pos - 2;
                    }
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        {
                            return this->safesqrt(1.0 - rnbo_abs(pos));
                        }
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void granulator_01_panfunc_reset() {}
    
    bool granulator_01_grains1_inactive() {
        return !(bool)(this->granulator_01_grains1_active);
    }
    
    void granulator_01_grains1_reset() {
        this->granulator_01_grains1_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains1_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains1_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains1_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains1_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains1_curRate = 0;
                this->granulator_01_grains1_active = false;
            } else {
                this->granulator_01_grains1_curLen = length;
                this->granulator_01_grains1_active = true;
                this->granulator_01_grains1_backwards = rate < 0;
                this->granulator_01_grains1_sampleOffset = 0;
                this->granulator_01_grains1_curStart = start;
    
                if ((bool)(this->granulator_01_grains1_backwards)) {
                    this->granulator_01_grains1_curStart += this->granulator_01_grains1_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains1_active)) {
            if (this->granulator_01_grains1_sampleOffset >= this->granulator_01_grains1_curLen) {
                bangDone = true;
                this->granulator_01_grains1_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains1_curStart;
    
                if ((bool)(this->granulator_01_grains1_backwards)) {
                    sampleIndex -= this->granulator_01_grains1_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains1_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains1_sampleOffset / this->granulator_01_grains1_curLen > 1 ? 1 : (this->granulator_01_grains1_sampleOffset / this->granulator_01_grains1_curLen < 0 ? 0 : this->granulator_01_grains1_sampleOffset / this->granulator_01_grains1_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains1_sampleOffset += this->granulator_01_grains1_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains2_inactive() {
        return !(bool)(this->granulator_01_grains2_active);
    }
    
    void granulator_01_grains2_reset() {
        this->granulator_01_grains2_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains2_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains2_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains2_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains2_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains2_curRate = 0;
                this->granulator_01_grains2_active = false;
            } else {
                this->granulator_01_grains2_curLen = length;
                this->granulator_01_grains2_active = true;
                this->granulator_01_grains2_backwards = rate < 0;
                this->granulator_01_grains2_sampleOffset = 0;
                this->granulator_01_grains2_curStart = start;
    
                if ((bool)(this->granulator_01_grains2_backwards)) {
                    this->granulator_01_grains2_curStart += this->granulator_01_grains2_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains2_active)) {
            if (this->granulator_01_grains2_sampleOffset >= this->granulator_01_grains2_curLen) {
                bangDone = true;
                this->granulator_01_grains2_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains2_curStart;
    
                if ((bool)(this->granulator_01_grains2_backwards)) {
                    sampleIndex -= this->granulator_01_grains2_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains2_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains2_sampleOffset / this->granulator_01_grains2_curLen > 1 ? 1 : (this->granulator_01_grains2_sampleOffset / this->granulator_01_grains2_curLen < 0 ? 0 : this->granulator_01_grains2_sampleOffset / this->granulator_01_grains2_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains2_sampleOffset += this->granulator_01_grains2_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains3_inactive() {
        return !(bool)(this->granulator_01_grains3_active);
    }
    
    void granulator_01_grains3_reset() {
        this->granulator_01_grains3_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains3_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains3_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains3_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains3_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains3_curRate = 0;
                this->granulator_01_grains3_active = false;
            } else {
                this->granulator_01_grains3_curLen = length;
                this->granulator_01_grains3_active = true;
                this->granulator_01_grains3_backwards = rate < 0;
                this->granulator_01_grains3_sampleOffset = 0;
                this->granulator_01_grains3_curStart = start;
    
                if ((bool)(this->granulator_01_grains3_backwards)) {
                    this->granulator_01_grains3_curStart += this->granulator_01_grains3_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains3_active)) {
            if (this->granulator_01_grains3_sampleOffset >= this->granulator_01_grains3_curLen) {
                bangDone = true;
                this->granulator_01_grains3_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains3_curStart;
    
                if ((bool)(this->granulator_01_grains3_backwards)) {
                    sampleIndex -= this->granulator_01_grains3_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains3_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains3_sampleOffset / this->granulator_01_grains3_curLen > 1 ? 1 : (this->granulator_01_grains3_sampleOffset / this->granulator_01_grains3_curLen < 0 ? 0 : this->granulator_01_grains3_sampleOffset / this->granulator_01_grains3_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains3_sampleOffset += this->granulator_01_grains3_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains4_inactive() {
        return !(bool)(this->granulator_01_grains4_active);
    }
    
    void granulator_01_grains4_reset() {
        this->granulator_01_grains4_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains4_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains4_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains4_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains4_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains4_curRate = 0;
                this->granulator_01_grains4_active = false;
            } else {
                this->granulator_01_grains4_curLen = length;
                this->granulator_01_grains4_active = true;
                this->granulator_01_grains4_backwards = rate < 0;
                this->granulator_01_grains4_sampleOffset = 0;
                this->granulator_01_grains4_curStart = start;
    
                if ((bool)(this->granulator_01_grains4_backwards)) {
                    this->granulator_01_grains4_curStart += this->granulator_01_grains4_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains4_active)) {
            if (this->granulator_01_grains4_sampleOffset >= this->granulator_01_grains4_curLen) {
                bangDone = true;
                this->granulator_01_grains4_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains4_curStart;
    
                if ((bool)(this->granulator_01_grains4_backwards)) {
                    sampleIndex -= this->granulator_01_grains4_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains4_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains4_sampleOffset / this->granulator_01_grains4_curLen > 1 ? 1 : (this->granulator_01_grains4_sampleOffset / this->granulator_01_grains4_curLen < 0 ? 0 : this->granulator_01_grains4_sampleOffset / this->granulator_01_grains4_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains4_sampleOffset += this->granulator_01_grains4_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains5_inactive() {
        return !(bool)(this->granulator_01_grains5_active);
    }
    
    void granulator_01_grains5_reset() {
        this->granulator_01_grains5_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains5_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains5_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains5_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains5_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains5_curRate = 0;
                this->granulator_01_grains5_active = false;
            } else {
                this->granulator_01_grains5_curLen = length;
                this->granulator_01_grains5_active = true;
                this->granulator_01_grains5_backwards = rate < 0;
                this->granulator_01_grains5_sampleOffset = 0;
                this->granulator_01_grains5_curStart = start;
    
                if ((bool)(this->granulator_01_grains5_backwards)) {
                    this->granulator_01_grains5_curStart += this->granulator_01_grains5_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains5_active)) {
            if (this->granulator_01_grains5_sampleOffset >= this->granulator_01_grains5_curLen) {
                bangDone = true;
                this->granulator_01_grains5_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains5_curStart;
    
                if ((bool)(this->granulator_01_grains5_backwards)) {
                    sampleIndex -= this->granulator_01_grains5_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains5_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains5_sampleOffset / this->granulator_01_grains5_curLen > 1 ? 1 : (this->granulator_01_grains5_sampleOffset / this->granulator_01_grains5_curLen < 0 ? 0 : this->granulator_01_grains5_sampleOffset / this->granulator_01_grains5_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains5_sampleOffset += this->granulator_01_grains5_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains6_inactive() {
        return !(bool)(this->granulator_01_grains6_active);
    }
    
    void granulator_01_grains6_reset() {
        this->granulator_01_grains6_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains6_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains6_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains6_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains6_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains6_curRate = 0;
                this->granulator_01_grains6_active = false;
            } else {
                this->granulator_01_grains6_curLen = length;
                this->granulator_01_grains6_active = true;
                this->granulator_01_grains6_backwards = rate < 0;
                this->granulator_01_grains6_sampleOffset = 0;
                this->granulator_01_grains6_curStart = start;
    
                if ((bool)(this->granulator_01_grains6_backwards)) {
                    this->granulator_01_grains6_curStart += this->granulator_01_grains6_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains6_active)) {
            if (this->granulator_01_grains6_sampleOffset >= this->granulator_01_grains6_curLen) {
                bangDone = true;
                this->granulator_01_grains6_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains6_curStart;
    
                if ((bool)(this->granulator_01_grains6_backwards)) {
                    sampleIndex -= this->granulator_01_grains6_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains6_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains6_sampleOffset / this->granulator_01_grains6_curLen > 1 ? 1 : (this->granulator_01_grains6_sampleOffset / this->granulator_01_grains6_curLen < 0 ? 0 : this->granulator_01_grains6_sampleOffset / this->granulator_01_grains6_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains6_sampleOffset += this->granulator_01_grains6_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains7_inactive() {
        return !(bool)(this->granulator_01_grains7_active);
    }
    
    void granulator_01_grains7_reset() {
        this->granulator_01_grains7_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains7_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains7_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains7_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains7_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains7_curRate = 0;
                this->granulator_01_grains7_active = false;
            } else {
                this->granulator_01_grains7_curLen = length;
                this->granulator_01_grains7_active = true;
                this->granulator_01_grains7_backwards = rate < 0;
                this->granulator_01_grains7_sampleOffset = 0;
                this->granulator_01_grains7_curStart = start;
    
                if ((bool)(this->granulator_01_grains7_backwards)) {
                    this->granulator_01_grains7_curStart += this->granulator_01_grains7_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains7_active)) {
            if (this->granulator_01_grains7_sampleOffset >= this->granulator_01_grains7_curLen) {
                bangDone = true;
                this->granulator_01_grains7_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains7_curStart;
    
                if ((bool)(this->granulator_01_grains7_backwards)) {
                    sampleIndex -= this->granulator_01_grains7_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains7_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains7_sampleOffset / this->granulator_01_grains7_curLen > 1 ? 1 : (this->granulator_01_grains7_sampleOffset / this->granulator_01_grains7_curLen < 0 ? 0 : this->granulator_01_grains7_sampleOffset / this->granulator_01_grains7_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains7_sampleOffset += this->granulator_01_grains7_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains8_inactive() {
        return !(bool)(this->granulator_01_grains8_active);
    }
    
    void granulator_01_grains8_reset() {
        this->granulator_01_grains8_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains8_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains8_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains8_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains8_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains8_curRate = 0;
                this->granulator_01_grains8_active = false;
            } else {
                this->granulator_01_grains8_curLen = length;
                this->granulator_01_grains8_active = true;
                this->granulator_01_grains8_backwards = rate < 0;
                this->granulator_01_grains8_sampleOffset = 0;
                this->granulator_01_grains8_curStart = start;
    
                if ((bool)(this->granulator_01_grains8_backwards)) {
                    this->granulator_01_grains8_curStart += this->granulator_01_grains8_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains8_active)) {
            if (this->granulator_01_grains8_sampleOffset >= this->granulator_01_grains8_curLen) {
                bangDone = true;
                this->granulator_01_grains8_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains8_curStart;
    
                if ((bool)(this->granulator_01_grains8_backwards)) {
                    sampleIndex -= this->granulator_01_grains8_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains8_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains8_sampleOffset / this->granulator_01_grains8_curLen > 1 ? 1 : (this->granulator_01_grains8_sampleOffset / this->granulator_01_grains8_curLen < 0 ? 0 : this->granulator_01_grains8_sampleOffset / this->granulator_01_grains8_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains8_sampleOffset += this->granulator_01_grains8_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains9_inactive() {
        return !(bool)(this->granulator_01_grains9_active);
    }
    
    void granulator_01_grains9_reset() {
        this->granulator_01_grains9_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains9_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains9_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains9_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains9_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains9_curRate = 0;
                this->granulator_01_grains9_active = false;
            } else {
                this->granulator_01_grains9_curLen = length;
                this->granulator_01_grains9_active = true;
                this->granulator_01_grains9_backwards = rate < 0;
                this->granulator_01_grains9_sampleOffset = 0;
                this->granulator_01_grains9_curStart = start;
    
                if ((bool)(this->granulator_01_grains9_backwards)) {
                    this->granulator_01_grains9_curStart += this->granulator_01_grains9_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains9_active)) {
            if (this->granulator_01_grains9_sampleOffset >= this->granulator_01_grains9_curLen) {
                bangDone = true;
                this->granulator_01_grains9_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains9_curStart;
    
                if ((bool)(this->granulator_01_grains9_backwards)) {
                    sampleIndex -= this->granulator_01_grains9_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains9_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains9_sampleOffset / this->granulator_01_grains9_curLen > 1 ? 1 : (this->granulator_01_grains9_sampleOffset / this->granulator_01_grains9_curLen < 0 ? 0 : this->granulator_01_grains9_sampleOffset / this->granulator_01_grains9_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains9_sampleOffset += this->granulator_01_grains9_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains10_inactive() {
        return !(bool)(this->granulator_01_grains10_active);
    }
    
    void granulator_01_grains10_reset() {
        this->granulator_01_grains10_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains10_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains10_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains10_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains10_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains10_curRate = 0;
                this->granulator_01_grains10_active = false;
            } else {
                this->granulator_01_grains10_curLen = length;
                this->granulator_01_grains10_active = true;
                this->granulator_01_grains10_backwards = rate < 0;
                this->granulator_01_grains10_sampleOffset = 0;
                this->granulator_01_grains10_curStart = start;
    
                if ((bool)(this->granulator_01_grains10_backwards)) {
                    this->granulator_01_grains10_curStart += this->granulator_01_grains10_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains10_active)) {
            if (this->granulator_01_grains10_sampleOffset >= this->granulator_01_grains10_curLen) {
                bangDone = true;
                this->granulator_01_grains10_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains10_curStart;
    
                if ((bool)(this->granulator_01_grains10_backwards)) {
                    sampleIndex -= this->granulator_01_grains10_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains10_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains10_sampleOffset / this->granulator_01_grains10_curLen > 1 ? 1 : (this->granulator_01_grains10_sampleOffset / this->granulator_01_grains10_curLen < 0 ? 0 : this->granulator_01_grains10_sampleOffset / this->granulator_01_grains10_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains10_sampleOffset += this->granulator_01_grains10_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains11_inactive() {
        return !(bool)(this->granulator_01_grains11_active);
    }
    
    void granulator_01_grains11_reset() {
        this->granulator_01_grains11_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains11_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains11_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains11_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains11_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains11_curRate = 0;
                this->granulator_01_grains11_active = false;
            } else {
                this->granulator_01_grains11_curLen = length;
                this->granulator_01_grains11_active = true;
                this->granulator_01_grains11_backwards = rate < 0;
                this->granulator_01_grains11_sampleOffset = 0;
                this->granulator_01_grains11_curStart = start;
    
                if ((bool)(this->granulator_01_grains11_backwards)) {
                    this->granulator_01_grains11_curStart += this->granulator_01_grains11_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains11_active)) {
            if (this->granulator_01_grains11_sampleOffset >= this->granulator_01_grains11_curLen) {
                bangDone = true;
                this->granulator_01_grains11_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains11_curStart;
    
                if ((bool)(this->granulator_01_grains11_backwards)) {
                    sampleIndex -= this->granulator_01_grains11_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains11_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains11_sampleOffset / this->granulator_01_grains11_curLen > 1 ? 1 : (this->granulator_01_grains11_sampleOffset / this->granulator_01_grains11_curLen < 0 ? 0 : this->granulator_01_grains11_sampleOffset / this->granulator_01_grains11_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains11_sampleOffset += this->granulator_01_grains11_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains12_inactive() {
        return !(bool)(this->granulator_01_grains12_active);
    }
    
    void granulator_01_grains12_reset() {
        this->granulator_01_grains12_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains12_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains12_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains12_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains12_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains12_curRate = 0;
                this->granulator_01_grains12_active = false;
            } else {
                this->granulator_01_grains12_curLen = length;
                this->granulator_01_grains12_active = true;
                this->granulator_01_grains12_backwards = rate < 0;
                this->granulator_01_grains12_sampleOffset = 0;
                this->granulator_01_grains12_curStart = start;
    
                if ((bool)(this->granulator_01_grains12_backwards)) {
                    this->granulator_01_grains12_curStart += this->granulator_01_grains12_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains12_active)) {
            if (this->granulator_01_grains12_sampleOffset >= this->granulator_01_grains12_curLen) {
                bangDone = true;
                this->granulator_01_grains12_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains12_curStart;
    
                if ((bool)(this->granulator_01_grains12_backwards)) {
                    sampleIndex -= this->granulator_01_grains12_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains12_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains12_sampleOffset / this->granulator_01_grains12_curLen > 1 ? 1 : (this->granulator_01_grains12_sampleOffset / this->granulator_01_grains12_curLen < 0 ? 0 : this->granulator_01_grains12_sampleOffset / this->granulator_01_grains12_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains12_sampleOffset += this->granulator_01_grains12_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains13_inactive() {
        return !(bool)(this->granulator_01_grains13_active);
    }
    
    void granulator_01_grains13_reset() {
        this->granulator_01_grains13_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains13_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains13_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains13_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains13_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains13_curRate = 0;
                this->granulator_01_grains13_active = false;
            } else {
                this->granulator_01_grains13_curLen = length;
                this->granulator_01_grains13_active = true;
                this->granulator_01_grains13_backwards = rate < 0;
                this->granulator_01_grains13_sampleOffset = 0;
                this->granulator_01_grains13_curStart = start;
    
                if ((bool)(this->granulator_01_grains13_backwards)) {
                    this->granulator_01_grains13_curStart += this->granulator_01_grains13_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains13_active)) {
            if (this->granulator_01_grains13_sampleOffset >= this->granulator_01_grains13_curLen) {
                bangDone = true;
                this->granulator_01_grains13_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains13_curStart;
    
                if ((bool)(this->granulator_01_grains13_backwards)) {
                    sampleIndex -= this->granulator_01_grains13_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains13_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains13_sampleOffset / this->granulator_01_grains13_curLen > 1 ? 1 : (this->granulator_01_grains13_sampleOffset / this->granulator_01_grains13_curLen < 0 ? 0 : this->granulator_01_grains13_sampleOffset / this->granulator_01_grains13_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains13_sampleOffset += this->granulator_01_grains13_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains14_inactive() {
        return !(bool)(this->granulator_01_grains14_active);
    }
    
    void granulator_01_grains14_reset() {
        this->granulator_01_grains14_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains14_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains14_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains14_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains14_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains14_curRate = 0;
                this->granulator_01_grains14_active = false;
            } else {
                this->granulator_01_grains14_curLen = length;
                this->granulator_01_grains14_active = true;
                this->granulator_01_grains14_backwards = rate < 0;
                this->granulator_01_grains14_sampleOffset = 0;
                this->granulator_01_grains14_curStart = start;
    
                if ((bool)(this->granulator_01_grains14_backwards)) {
                    this->granulator_01_grains14_curStart += this->granulator_01_grains14_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains14_active)) {
            if (this->granulator_01_grains14_sampleOffset >= this->granulator_01_grains14_curLen) {
                bangDone = true;
                this->granulator_01_grains14_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains14_curStart;
    
                if ((bool)(this->granulator_01_grains14_backwards)) {
                    sampleIndex -= this->granulator_01_grains14_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains14_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains14_sampleOffset / this->granulator_01_grains14_curLen > 1 ? 1 : (this->granulator_01_grains14_sampleOffset / this->granulator_01_grains14_curLen < 0 ? 0 : this->granulator_01_grains14_sampleOffset / this->granulator_01_grains14_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains14_sampleOffset += this->granulator_01_grains14_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains15_inactive() {
        return !(bool)(this->granulator_01_grains15_active);
    }
    
    void granulator_01_grains15_reset() {
        this->granulator_01_grains15_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains15_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains15_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains15_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains15_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains15_curRate = 0;
                this->granulator_01_grains15_active = false;
            } else {
                this->granulator_01_grains15_curLen = length;
                this->granulator_01_grains15_active = true;
                this->granulator_01_grains15_backwards = rate < 0;
                this->granulator_01_grains15_sampleOffset = 0;
                this->granulator_01_grains15_curStart = start;
    
                if ((bool)(this->granulator_01_grains15_backwards)) {
                    this->granulator_01_grains15_curStart += this->granulator_01_grains15_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains15_active)) {
            if (this->granulator_01_grains15_sampleOffset >= this->granulator_01_grains15_curLen) {
                bangDone = true;
                this->granulator_01_grains15_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains15_curStart;
    
                if ((bool)(this->granulator_01_grains15_backwards)) {
                    sampleIndex -= this->granulator_01_grains15_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains15_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains15_sampleOffset / this->granulator_01_grains15_curLen > 1 ? 1 : (this->granulator_01_grains15_sampleOffset / this->granulator_01_grains15_curLen < 0 ? 0 : this->granulator_01_grains15_sampleOffset / this->granulator_01_grains15_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains15_sampleOffset += this->granulator_01_grains15_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains16_inactive() {
        return !(bool)(this->granulator_01_grains16_active);
    }
    
    void granulator_01_grains16_reset() {
        this->granulator_01_grains16_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains16_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains16_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains16_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains16_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains16_curRate = 0;
                this->granulator_01_grains16_active = false;
            } else {
                this->granulator_01_grains16_curLen = length;
                this->granulator_01_grains16_active = true;
                this->granulator_01_grains16_backwards = rate < 0;
                this->granulator_01_grains16_sampleOffset = 0;
                this->granulator_01_grains16_curStart = start;
    
                if ((bool)(this->granulator_01_grains16_backwards)) {
                    this->granulator_01_grains16_curStart += this->granulator_01_grains16_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains16_active)) {
            if (this->granulator_01_grains16_sampleOffset >= this->granulator_01_grains16_curLen) {
                bangDone = true;
                this->granulator_01_grains16_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains16_curStart;
    
                if ((bool)(this->granulator_01_grains16_backwards)) {
                    sampleIndex -= this->granulator_01_grains16_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains16_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains16_sampleOffset / this->granulator_01_grains16_curLen > 1 ? 1 : (this->granulator_01_grains16_sampleOffset / this->granulator_01_grains16_curLen < 0 ? 0 : this->granulator_01_grains16_sampleOffset / this->granulator_01_grains16_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains16_sampleOffset += this->granulator_01_grains16_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains17_inactive() {
        return !(bool)(this->granulator_01_grains17_active);
    }
    
    void granulator_01_grains17_reset() {
        this->granulator_01_grains17_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains17_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains17_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains17_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains17_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains17_curRate = 0;
                this->granulator_01_grains17_active = false;
            } else {
                this->granulator_01_grains17_curLen = length;
                this->granulator_01_grains17_active = true;
                this->granulator_01_grains17_backwards = rate < 0;
                this->granulator_01_grains17_sampleOffset = 0;
                this->granulator_01_grains17_curStart = start;
    
                if ((bool)(this->granulator_01_grains17_backwards)) {
                    this->granulator_01_grains17_curStart += this->granulator_01_grains17_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains17_active)) {
            if (this->granulator_01_grains17_sampleOffset >= this->granulator_01_grains17_curLen) {
                bangDone = true;
                this->granulator_01_grains17_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains17_curStart;
    
                if ((bool)(this->granulator_01_grains17_backwards)) {
                    sampleIndex -= this->granulator_01_grains17_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains17_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains17_sampleOffset / this->granulator_01_grains17_curLen > 1 ? 1 : (this->granulator_01_grains17_sampleOffset / this->granulator_01_grains17_curLen < 0 ? 0 : this->granulator_01_grains17_sampleOffset / this->granulator_01_grains17_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains17_sampleOffset += this->granulator_01_grains17_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains18_inactive() {
        return !(bool)(this->granulator_01_grains18_active);
    }
    
    void granulator_01_grains18_reset() {
        this->granulator_01_grains18_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains18_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains18_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains18_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains18_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains18_curRate = 0;
                this->granulator_01_grains18_active = false;
            } else {
                this->granulator_01_grains18_curLen = length;
                this->granulator_01_grains18_active = true;
                this->granulator_01_grains18_backwards = rate < 0;
                this->granulator_01_grains18_sampleOffset = 0;
                this->granulator_01_grains18_curStart = start;
    
                if ((bool)(this->granulator_01_grains18_backwards)) {
                    this->granulator_01_grains18_curStart += this->granulator_01_grains18_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains18_active)) {
            if (this->granulator_01_grains18_sampleOffset >= this->granulator_01_grains18_curLen) {
                bangDone = true;
                this->granulator_01_grains18_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains18_curStart;
    
                if ((bool)(this->granulator_01_grains18_backwards)) {
                    sampleIndex -= this->granulator_01_grains18_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains18_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains18_sampleOffset / this->granulator_01_grains18_curLen > 1 ? 1 : (this->granulator_01_grains18_sampleOffset / this->granulator_01_grains18_curLen < 0 ? 0 : this->granulator_01_grains18_sampleOffset / this->granulator_01_grains18_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains18_sampleOffset += this->granulator_01_grains18_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains19_inactive() {
        return !(bool)(this->granulator_01_grains19_active);
    }
    
    void granulator_01_grains19_reset() {
        this->granulator_01_grains19_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains19_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains19_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains19_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains19_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains19_curRate = 0;
                this->granulator_01_grains19_active = false;
            } else {
                this->granulator_01_grains19_curLen = length;
                this->granulator_01_grains19_active = true;
                this->granulator_01_grains19_backwards = rate < 0;
                this->granulator_01_grains19_sampleOffset = 0;
                this->granulator_01_grains19_curStart = start;
    
                if ((bool)(this->granulator_01_grains19_backwards)) {
                    this->granulator_01_grains19_curStart += this->granulator_01_grains19_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains19_active)) {
            if (this->granulator_01_grains19_sampleOffset >= this->granulator_01_grains19_curLen) {
                bangDone = true;
                this->granulator_01_grains19_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains19_curStart;
    
                if ((bool)(this->granulator_01_grains19_backwards)) {
                    sampleIndex -= this->granulator_01_grains19_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains19_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains19_sampleOffset / this->granulator_01_grains19_curLen > 1 ? 1 : (this->granulator_01_grains19_sampleOffset / this->granulator_01_grains19_curLen < 0 ? 0 : this->granulator_01_grains19_sampleOffset / this->granulator_01_grains19_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains19_sampleOffset += this->granulator_01_grains19_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains20_inactive() {
        return !(bool)(this->granulator_01_grains20_active);
    }
    
    void granulator_01_grains20_reset() {
        this->granulator_01_grains20_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains20_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains20_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains20_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains20_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains20_curRate = 0;
                this->granulator_01_grains20_active = false;
            } else {
                this->granulator_01_grains20_curLen = length;
                this->granulator_01_grains20_active = true;
                this->granulator_01_grains20_backwards = rate < 0;
                this->granulator_01_grains20_sampleOffset = 0;
                this->granulator_01_grains20_curStart = start;
    
                if ((bool)(this->granulator_01_grains20_backwards)) {
                    this->granulator_01_grains20_curStart += this->granulator_01_grains20_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains20_active)) {
            if (this->granulator_01_grains20_sampleOffset >= this->granulator_01_grains20_curLen) {
                bangDone = true;
                this->granulator_01_grains20_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains20_curStart;
    
                if ((bool)(this->granulator_01_grains20_backwards)) {
                    sampleIndex -= this->granulator_01_grains20_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains20_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains20_sampleOffset / this->granulator_01_grains20_curLen > 1 ? 1 : (this->granulator_01_grains20_sampleOffset / this->granulator_01_grains20_curLen < 0 ? 0 : this->granulator_01_grains20_sampleOffset / this->granulator_01_grains20_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains20_sampleOffset += this->granulator_01_grains20_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains21_inactive() {
        return !(bool)(this->granulator_01_grains21_active);
    }
    
    void granulator_01_grains21_reset() {
        this->granulator_01_grains21_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains21_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains21_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains21_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains21_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains21_curRate = 0;
                this->granulator_01_grains21_active = false;
            } else {
                this->granulator_01_grains21_curLen = length;
                this->granulator_01_grains21_active = true;
                this->granulator_01_grains21_backwards = rate < 0;
                this->granulator_01_grains21_sampleOffset = 0;
                this->granulator_01_grains21_curStart = start;
    
                if ((bool)(this->granulator_01_grains21_backwards)) {
                    this->granulator_01_grains21_curStart += this->granulator_01_grains21_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains21_active)) {
            if (this->granulator_01_grains21_sampleOffset >= this->granulator_01_grains21_curLen) {
                bangDone = true;
                this->granulator_01_grains21_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains21_curStart;
    
                if ((bool)(this->granulator_01_grains21_backwards)) {
                    sampleIndex -= this->granulator_01_grains21_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains21_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains21_sampleOffset / this->granulator_01_grains21_curLen > 1 ? 1 : (this->granulator_01_grains21_sampleOffset / this->granulator_01_grains21_curLen < 0 ? 0 : this->granulator_01_grains21_sampleOffset / this->granulator_01_grains21_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains21_sampleOffset += this->granulator_01_grains21_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains22_inactive() {
        return !(bool)(this->granulator_01_grains22_active);
    }
    
    void granulator_01_grains22_reset() {
        this->granulator_01_grains22_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains22_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains22_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains22_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains22_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains22_curRate = 0;
                this->granulator_01_grains22_active = false;
            } else {
                this->granulator_01_grains22_curLen = length;
                this->granulator_01_grains22_active = true;
                this->granulator_01_grains22_backwards = rate < 0;
                this->granulator_01_grains22_sampleOffset = 0;
                this->granulator_01_grains22_curStart = start;
    
                if ((bool)(this->granulator_01_grains22_backwards)) {
                    this->granulator_01_grains22_curStart += this->granulator_01_grains22_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains22_active)) {
            if (this->granulator_01_grains22_sampleOffset >= this->granulator_01_grains22_curLen) {
                bangDone = true;
                this->granulator_01_grains22_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains22_curStart;
    
                if ((bool)(this->granulator_01_grains22_backwards)) {
                    sampleIndex -= this->granulator_01_grains22_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains22_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains22_sampleOffset / this->granulator_01_grains22_curLen > 1 ? 1 : (this->granulator_01_grains22_sampleOffset / this->granulator_01_grains22_curLen < 0 ? 0 : this->granulator_01_grains22_sampleOffset / this->granulator_01_grains22_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains22_sampleOffset += this->granulator_01_grains22_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains23_inactive() {
        return !(bool)(this->granulator_01_grains23_active);
    }
    
    void granulator_01_grains23_reset() {
        this->granulator_01_grains23_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains23_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains23_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains23_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains23_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains23_curRate = 0;
                this->granulator_01_grains23_active = false;
            } else {
                this->granulator_01_grains23_curLen = length;
                this->granulator_01_grains23_active = true;
                this->granulator_01_grains23_backwards = rate < 0;
                this->granulator_01_grains23_sampleOffset = 0;
                this->granulator_01_grains23_curStart = start;
    
                if ((bool)(this->granulator_01_grains23_backwards)) {
                    this->granulator_01_grains23_curStart += this->granulator_01_grains23_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains23_active)) {
            if (this->granulator_01_grains23_sampleOffset >= this->granulator_01_grains23_curLen) {
                bangDone = true;
                this->granulator_01_grains23_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains23_curStart;
    
                if ((bool)(this->granulator_01_grains23_backwards)) {
                    sampleIndex -= this->granulator_01_grains23_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains23_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains23_sampleOffset / this->granulator_01_grains23_curLen > 1 ? 1 : (this->granulator_01_grains23_sampleOffset / this->granulator_01_grains23_curLen < 0 ? 0 : this->granulator_01_grains23_sampleOffset / this->granulator_01_grains23_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains23_sampleOffset += this->granulator_01_grains23_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains24_inactive() {
        return !(bool)(this->granulator_01_grains24_active);
    }
    
    void granulator_01_grains24_reset() {
        this->granulator_01_grains24_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains24_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains24_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains24_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains24_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains24_curRate = 0;
                this->granulator_01_grains24_active = false;
            } else {
                this->granulator_01_grains24_curLen = length;
                this->granulator_01_grains24_active = true;
                this->granulator_01_grains24_backwards = rate < 0;
                this->granulator_01_grains24_sampleOffset = 0;
                this->granulator_01_grains24_curStart = start;
    
                if ((bool)(this->granulator_01_grains24_backwards)) {
                    this->granulator_01_grains24_curStart += this->granulator_01_grains24_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains24_active)) {
            if (this->granulator_01_grains24_sampleOffset >= this->granulator_01_grains24_curLen) {
                bangDone = true;
                this->granulator_01_grains24_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains24_curStart;
    
                if ((bool)(this->granulator_01_grains24_backwards)) {
                    sampleIndex -= this->granulator_01_grains24_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains24_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains24_sampleOffset / this->granulator_01_grains24_curLen > 1 ? 1 : (this->granulator_01_grains24_sampleOffset / this->granulator_01_grains24_curLen < 0 ? 0 : this->granulator_01_grains24_sampleOffset / this->granulator_01_grains24_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains24_sampleOffset += this->granulator_01_grains24_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains25_inactive() {
        return !(bool)(this->granulator_01_grains25_active);
    }
    
    void granulator_01_grains25_reset() {
        this->granulator_01_grains25_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains25_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains25_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains25_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains25_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains25_curRate = 0;
                this->granulator_01_grains25_active = false;
            } else {
                this->granulator_01_grains25_curLen = length;
                this->granulator_01_grains25_active = true;
                this->granulator_01_grains25_backwards = rate < 0;
                this->granulator_01_grains25_sampleOffset = 0;
                this->granulator_01_grains25_curStart = start;
    
                if ((bool)(this->granulator_01_grains25_backwards)) {
                    this->granulator_01_grains25_curStart += this->granulator_01_grains25_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains25_active)) {
            if (this->granulator_01_grains25_sampleOffset >= this->granulator_01_grains25_curLen) {
                bangDone = true;
                this->granulator_01_grains25_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains25_curStart;
    
                if ((bool)(this->granulator_01_grains25_backwards)) {
                    sampleIndex -= this->granulator_01_grains25_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains25_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains25_sampleOffset / this->granulator_01_grains25_curLen > 1 ? 1 : (this->granulator_01_grains25_sampleOffset / this->granulator_01_grains25_curLen < 0 ? 0 : this->granulator_01_grains25_sampleOffset / this->granulator_01_grains25_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains25_sampleOffset += this->granulator_01_grains25_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains26_inactive() {
        return !(bool)(this->granulator_01_grains26_active);
    }
    
    void granulator_01_grains26_reset() {
        this->granulator_01_grains26_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains26_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains26_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains26_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains26_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains26_curRate = 0;
                this->granulator_01_grains26_active = false;
            } else {
                this->granulator_01_grains26_curLen = length;
                this->granulator_01_grains26_active = true;
                this->granulator_01_grains26_backwards = rate < 0;
                this->granulator_01_grains26_sampleOffset = 0;
                this->granulator_01_grains26_curStart = start;
    
                if ((bool)(this->granulator_01_grains26_backwards)) {
                    this->granulator_01_grains26_curStart += this->granulator_01_grains26_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains26_active)) {
            if (this->granulator_01_grains26_sampleOffset >= this->granulator_01_grains26_curLen) {
                bangDone = true;
                this->granulator_01_grains26_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains26_curStart;
    
                if ((bool)(this->granulator_01_grains26_backwards)) {
                    sampleIndex -= this->granulator_01_grains26_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains26_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains26_sampleOffset / this->granulator_01_grains26_curLen > 1 ? 1 : (this->granulator_01_grains26_sampleOffset / this->granulator_01_grains26_curLen < 0 ? 0 : this->granulator_01_grains26_sampleOffset / this->granulator_01_grains26_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains26_sampleOffset += this->granulator_01_grains26_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains27_inactive() {
        return !(bool)(this->granulator_01_grains27_active);
    }
    
    void granulator_01_grains27_reset() {
        this->granulator_01_grains27_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains27_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains27_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains27_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains27_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains27_curRate = 0;
                this->granulator_01_grains27_active = false;
            } else {
                this->granulator_01_grains27_curLen = length;
                this->granulator_01_grains27_active = true;
                this->granulator_01_grains27_backwards = rate < 0;
                this->granulator_01_grains27_sampleOffset = 0;
                this->granulator_01_grains27_curStart = start;
    
                if ((bool)(this->granulator_01_grains27_backwards)) {
                    this->granulator_01_grains27_curStart += this->granulator_01_grains27_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains27_active)) {
            if (this->granulator_01_grains27_sampleOffset >= this->granulator_01_grains27_curLen) {
                bangDone = true;
                this->granulator_01_grains27_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains27_curStart;
    
                if ((bool)(this->granulator_01_grains27_backwards)) {
                    sampleIndex -= this->granulator_01_grains27_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains27_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains27_sampleOffset / this->granulator_01_grains27_curLen > 1 ? 1 : (this->granulator_01_grains27_sampleOffset / this->granulator_01_grains27_curLen < 0 ? 0 : this->granulator_01_grains27_sampleOffset / this->granulator_01_grains27_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains27_sampleOffset += this->granulator_01_grains27_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains28_inactive() {
        return !(bool)(this->granulator_01_grains28_active);
    }
    
    void granulator_01_grains28_reset() {
        this->granulator_01_grains28_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains28_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains28_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains28_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains28_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains28_curRate = 0;
                this->granulator_01_grains28_active = false;
            } else {
                this->granulator_01_grains28_curLen = length;
                this->granulator_01_grains28_active = true;
                this->granulator_01_grains28_backwards = rate < 0;
                this->granulator_01_grains28_sampleOffset = 0;
                this->granulator_01_grains28_curStart = start;
    
                if ((bool)(this->granulator_01_grains28_backwards)) {
                    this->granulator_01_grains28_curStart += this->granulator_01_grains28_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains28_active)) {
            if (this->granulator_01_grains28_sampleOffset >= this->granulator_01_grains28_curLen) {
                bangDone = true;
                this->granulator_01_grains28_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains28_curStart;
    
                if ((bool)(this->granulator_01_grains28_backwards)) {
                    sampleIndex -= this->granulator_01_grains28_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains28_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains28_sampleOffset / this->granulator_01_grains28_curLen > 1 ? 1 : (this->granulator_01_grains28_sampleOffset / this->granulator_01_grains28_curLen < 0 ? 0 : this->granulator_01_grains28_sampleOffset / this->granulator_01_grains28_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains28_sampleOffset += this->granulator_01_grains28_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains29_inactive() {
        return !(bool)(this->granulator_01_grains29_active);
    }
    
    void granulator_01_grains29_reset() {
        this->granulator_01_grains29_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains29_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains29_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains29_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains29_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains29_curRate = 0;
                this->granulator_01_grains29_active = false;
            } else {
                this->granulator_01_grains29_curLen = length;
                this->granulator_01_grains29_active = true;
                this->granulator_01_grains29_backwards = rate < 0;
                this->granulator_01_grains29_sampleOffset = 0;
                this->granulator_01_grains29_curStart = start;
    
                if ((bool)(this->granulator_01_grains29_backwards)) {
                    this->granulator_01_grains29_curStart += this->granulator_01_grains29_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains29_active)) {
            if (this->granulator_01_grains29_sampleOffset >= this->granulator_01_grains29_curLen) {
                bangDone = true;
                this->granulator_01_grains29_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains29_curStart;
    
                if ((bool)(this->granulator_01_grains29_backwards)) {
                    sampleIndex -= this->granulator_01_grains29_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains29_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains29_sampleOffset / this->granulator_01_grains29_curLen > 1 ? 1 : (this->granulator_01_grains29_sampleOffset / this->granulator_01_grains29_curLen < 0 ? 0 : this->granulator_01_grains29_sampleOffset / this->granulator_01_grains29_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains29_sampleOffset += this->granulator_01_grains29_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains30_inactive() {
        return !(bool)(this->granulator_01_grains30_active);
    }
    
    void granulator_01_grains30_reset() {
        this->granulator_01_grains30_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains30_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains30_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains30_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains30_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains30_curRate = 0;
                this->granulator_01_grains30_active = false;
            } else {
                this->granulator_01_grains30_curLen = length;
                this->granulator_01_grains30_active = true;
                this->granulator_01_grains30_backwards = rate < 0;
                this->granulator_01_grains30_sampleOffset = 0;
                this->granulator_01_grains30_curStart = start;
    
                if ((bool)(this->granulator_01_grains30_backwards)) {
                    this->granulator_01_grains30_curStart += this->granulator_01_grains30_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains30_active)) {
            if (this->granulator_01_grains30_sampleOffset >= this->granulator_01_grains30_curLen) {
                bangDone = true;
                this->granulator_01_grains30_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains30_curStart;
    
                if ((bool)(this->granulator_01_grains30_backwards)) {
                    sampleIndex -= this->granulator_01_grains30_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains30_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains30_sampleOffset / this->granulator_01_grains30_curLen > 1 ? 1 : (this->granulator_01_grains30_sampleOffset / this->granulator_01_grains30_curLen < 0 ? 0 : this->granulator_01_grains30_sampleOffset / this->granulator_01_grains30_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains30_sampleOffset += this->granulator_01_grains30_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains31_inactive() {
        return !(bool)(this->granulator_01_grains31_active);
    }
    
    void granulator_01_grains31_reset() {
        this->granulator_01_grains31_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains31_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains31_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains31_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains31_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains31_curRate = 0;
                this->granulator_01_grains31_active = false;
            } else {
                this->granulator_01_grains31_curLen = length;
                this->granulator_01_grains31_active = true;
                this->granulator_01_grains31_backwards = rate < 0;
                this->granulator_01_grains31_sampleOffset = 0;
                this->granulator_01_grains31_curStart = start;
    
                if ((bool)(this->granulator_01_grains31_backwards)) {
                    this->granulator_01_grains31_curStart += this->granulator_01_grains31_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains31_active)) {
            if (this->granulator_01_grains31_sampleOffset >= this->granulator_01_grains31_curLen) {
                bangDone = true;
                this->granulator_01_grains31_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains31_curStart;
    
                if ((bool)(this->granulator_01_grains31_backwards)) {
                    sampleIndex -= this->granulator_01_grains31_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains31_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains31_sampleOffset / this->granulator_01_grains31_curLen > 1 ? 1 : (this->granulator_01_grains31_sampleOffset / this->granulator_01_grains31_curLen < 0 ? 0 : this->granulator_01_grains31_sampleOffset / this->granulator_01_grains31_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains31_sampleOffset += this->granulator_01_grains31_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains32_inactive() {
        return !(bool)(this->granulator_01_grains32_active);
    }
    
    void granulator_01_grains32_reset() {
        this->granulator_01_grains32_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains32_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_01_grains32_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_01_grains32_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_01_grains32_active)) {
                    bangDone = true;
                }
    
                this->granulator_01_grains32_curRate = 0;
                this->granulator_01_grains32_active = false;
            } else {
                this->granulator_01_grains32_curLen = length;
                this->granulator_01_grains32_active = true;
                this->granulator_01_grains32_backwards = rate < 0;
                this->granulator_01_grains32_sampleOffset = 0;
                this->granulator_01_grains32_curStart = start;
    
                if ((bool)(this->granulator_01_grains32_backwards)) {
                    this->granulator_01_grains32_curStart += this->granulator_01_grains32_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_01_grains32_active)) {
            if (this->granulator_01_grains32_sampleOffset >= this->granulator_01_grains32_curLen) {
                bangDone = true;
                this->granulator_01_grains32_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_01_grains32_curStart;
    
                if ((bool)(this->granulator_01_grains32_backwards)) {
                    sampleIndex -= this->granulator_01_grains32_sampleOffset;
                } else {
                    sampleIndex += this->granulator_01_grains32_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_01_grains32_sampleOffset / this->granulator_01_grains32_curLen > 1 ? 1 : (this->granulator_01_grains32_sampleOffset / this->granulator_01_grains32_curLen < 0 ? 0 : this->granulator_01_grains32_sampleOffset / this->granulator_01_grains32_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_01_grains32_sampleOffset += this->granulator_01_grains32_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_01_grains_inactive(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_01_grains1_inactive();
        case 1:
            return this->granulator_01_grains2_inactive();
        case 2:
            return this->granulator_01_grains3_inactive();
        case 3:
            return this->granulator_01_grains4_inactive();
        case 4:
            return this->granulator_01_grains5_inactive();
        case 5:
            return this->granulator_01_grains6_inactive();
        case 6:
            return this->granulator_01_grains7_inactive();
        case 7:
            return this->granulator_01_grains8_inactive();
        case 8:
            return this->granulator_01_grains9_inactive();
        case 9:
            return this->granulator_01_grains10_inactive();
        case 10:
            return this->granulator_01_grains11_inactive();
        case 11:
            return this->granulator_01_grains12_inactive();
        case 12:
            return this->granulator_01_grains13_inactive();
        case 13:
            return this->granulator_01_grains14_inactive();
        case 14:
            return this->granulator_01_grains15_inactive();
        case 15:
            return this->granulator_01_grains16_inactive();
        case 16:
            return this->granulator_01_grains17_inactive();
        case 17:
            return this->granulator_01_grains18_inactive();
        case 18:
            return this->granulator_01_grains19_inactive();
        case 19:
            return this->granulator_01_grains20_inactive();
        case 20:
            return this->granulator_01_grains21_inactive();
        case 21:
            return this->granulator_01_grains22_inactive();
        case 22:
            return this->granulator_01_grains23_inactive();
        case 23:
            return this->granulator_01_grains24_inactive();
        case 24:
            return this->granulator_01_grains25_inactive();
        case 25:
            return this->granulator_01_grains26_inactive();
        case 26:
            return this->granulator_01_grains27_inactive();
        case 27:
            return this->granulator_01_grains28_inactive();
        case 28:
            return this->granulator_01_grains29_inactive();
        case 29:
            return this->granulator_01_grains30_inactive();
        case 30:
            return this->granulator_01_grains31_inactive();
        default:
            return this->granulator_01_grains32_inactive();
        }
    
        return 0;
    }
    
    void granulator_01_grains_reset(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_01_grains1_reset();
        case 1:
            return this->granulator_01_grains2_reset();
        case 2:
            return this->granulator_01_grains3_reset();
        case 3:
            return this->granulator_01_grains4_reset();
        case 4:
            return this->granulator_01_grains5_reset();
        case 5:
            return this->granulator_01_grains6_reset();
        case 6:
            return this->granulator_01_grains7_reset();
        case 7:
            return this->granulator_01_grains8_reset();
        case 8:
            return this->granulator_01_grains9_reset();
        case 9:
            return this->granulator_01_grains10_reset();
        case 10:
            return this->granulator_01_grains11_reset();
        case 11:
            return this->granulator_01_grains12_reset();
        case 12:
            return this->granulator_01_grains13_reset();
        case 13:
            return this->granulator_01_grains14_reset();
        case 14:
            return this->granulator_01_grains15_reset();
        case 15:
            return this->granulator_01_grains16_reset();
        case 16:
            return this->granulator_01_grains17_reset();
        case 17:
            return this->granulator_01_grains18_reset();
        case 18:
            return this->granulator_01_grains19_reset();
        case 19:
            return this->granulator_01_grains20_reset();
        case 20:
            return this->granulator_01_grains21_reset();
        case 21:
            return this->granulator_01_grains22_reset();
        case 22:
            return this->granulator_01_grains23_reset();
        case 23:
            return this->granulator_01_grains24_reset();
        case 24:
            return this->granulator_01_grains25_reset();
        case 25:
            return this->granulator_01_grains26_reset();
        case 26:
            return this->granulator_01_grains27_reset();
        case 27:
            return this->granulator_01_grains28_reset();
        case 28:
            return this->granulator_01_grains29_reset();
        case 29:
            return this->granulator_01_grains30_reset();
        case 30:
            return this->granulator_01_grains31_reset();
        default:
            return this->granulator_01_grains32_reset();
        }
    }
    
    template <typename T> inline array<number, 3> granulator_01_grains_next(
        Index i,
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        switch ((int)i) {
        case 0:
            return this->granulator_01_grains1_next(buffer, trigger, start, length, rate, channel);
        case 1:
            return this->granulator_01_grains2_next(buffer, trigger, start, length, rate, channel);
        case 2:
            return this->granulator_01_grains3_next(buffer, trigger, start, length, rate, channel);
        case 3:
            return this->granulator_01_grains4_next(buffer, trigger, start, length, rate, channel);
        case 4:
            return this->granulator_01_grains5_next(buffer, trigger, start, length, rate, channel);
        case 5:
            return this->granulator_01_grains6_next(buffer, trigger, start, length, rate, channel);
        case 6:
            return this->granulator_01_grains7_next(buffer, trigger, start, length, rate, channel);
        case 7:
            return this->granulator_01_grains8_next(buffer, trigger, start, length, rate, channel);
        case 8:
            return this->granulator_01_grains9_next(buffer, trigger, start, length, rate, channel);
        case 9:
            return this->granulator_01_grains10_next(buffer, trigger, start, length, rate, channel);
        case 10:
            return this->granulator_01_grains11_next(buffer, trigger, start, length, rate, channel);
        case 11:
            return this->granulator_01_grains12_next(buffer, trigger, start, length, rate, channel);
        case 12:
            return this->granulator_01_grains13_next(buffer, trigger, start, length, rate, channel);
        case 13:
            return this->granulator_01_grains14_next(buffer, trigger, start, length, rate, channel);
        case 14:
            return this->granulator_01_grains15_next(buffer, trigger, start, length, rate, channel);
        case 15:
            return this->granulator_01_grains16_next(buffer, trigger, start, length, rate, channel);
        case 16:
            return this->granulator_01_grains17_next(buffer, trigger, start, length, rate, channel);
        case 17:
            return this->granulator_01_grains18_next(buffer, trigger, start, length, rate, channel);
        case 18:
            return this->granulator_01_grains19_next(buffer, trigger, start, length, rate, channel);
        case 19:
            return this->granulator_01_grains20_next(buffer, trigger, start, length, rate, channel);
        case 20:
            return this->granulator_01_grains21_next(buffer, trigger, start, length, rate, channel);
        case 21:
            return this->granulator_01_grains22_next(buffer, trigger, start, length, rate, channel);
        case 22:
            return this->granulator_01_grains23_next(buffer, trigger, start, length, rate, channel);
        case 23:
            return this->granulator_01_grains24_next(buffer, trigger, start, length, rate, channel);
        case 24:
            return this->granulator_01_grains25_next(buffer, trigger, start, length, rate, channel);
        case 25:
            return this->granulator_01_grains26_next(buffer, trigger, start, length, rate, channel);
        case 26:
            return this->granulator_01_grains27_next(buffer, trigger, start, length, rate, channel);
        case 27:
            return this->granulator_01_grains28_next(buffer, trigger, start, length, rate, channel);
        case 28:
            return this->granulator_01_grains29_next(buffer, trigger, start, length, rate, channel);
        case 29:
            return this->granulator_01_grains30_next(buffer, trigger, start, length, rate, channel);
        case 30:
            return this->granulator_01_grains31_next(buffer, trigger, start, length, rate, channel);
        default:
            return this->granulator_01_grains32_next(buffer, trigger, start, length, rate, channel);
        }
    
        return 0;
    }
    
    void granulator_01_init() {
        this->granulator_01_edgedetect_init();
    
        for (Index i = 0; i < 32; i++) {
            for (Index j = 0; j < 2; j++) {
                this->granulator_01_grainPan[(Index)i][(Index)j] = 1;
            }
        }
    }
    
    void granulator_01_bufferUpdated() {
        number bufSr = this->granulator_01_buffer->getSampleRate();
        number bufSize = this->granulator_01_buffer->getSize();
    
        for (Index i = 0; i < 32; i++) {
            this->granulator_01_grains_reset(i);
        }
    
        {
            this->granulator_01_startMul = bufSize;
        }
    
        {
            this->granulator_01_lengthMul = bufSize;
        }
    }
    
    void granulator_01_onSampleRateChanged(number samplerate) {
        RNBO_UNUSED(samplerate);
        this->granulator_01_bufferUpdated();
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
    
    void param_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_04_value;
    }
    
    void param_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_04_value_set(preset["value"]);
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
    
    void samplerate_01_onSampleRateChanged(number samplerate) {
        this->samplerate_01_samplerate_set(samplerate);
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
        granulator_01_start = 0;
        granulator_01_length = 2205;
        granulator_01_rate = 1;
        granulator_01_bchan = 0;
        granulator_01_limit = 32;
        granulator_01_panpos = 0.5;
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 0;
        param_01_value = 0;
        dspexpr_03_in1 = 0;
        dspexpr_03_in2 = 0;
        dspexpr_04_in1 = 0;
        dspexpr_04_in2 = 0.5;
        dspexpr_05_in1 = 0;
        dspexpr_05_in2 = 0.5;
        dspexpr_06_in1 = 0;
        dspexpr_06_in2 = 0;
        param_02_value = 1;
        dspexpr_07_in1 = 0;
        dspexpr_07_in2 = 0;
        param_03_value = 1;
        dspexpr_08_in1 = 0;
        dspexpr_08_in2 = 0;
        param_04_value = 1;
        param_05_value = 0;
        expr_01_in1 = 0;
        expr_01_in2 = 0;
        expr_01_out1 = 0;
        param_06_value = 1;
        expr_02_in1 = 0;
        expr_02_in2 = 1000;
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
        signals[4] = nullptr;
        signals[5] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        granulator_01_startMul = 1;
        granulator_01_lengthMul = 1;
        granulator_01_limitLast = 0;
        granulator_01_edgedetect_active = false;
        granulator_01_grains1_sampleOffset = 0;
        granulator_01_grains1_curRate = 1;
        granulator_01_grains1_curStart = 0;
        granulator_01_grains1_curLen = 1;
        granulator_01_grains1_active = false;
        granulator_01_grains1_backwards = false;
        granulator_01_grains2_sampleOffset = 0;
        granulator_01_grains2_curRate = 1;
        granulator_01_grains2_curStart = 0;
        granulator_01_grains2_curLen = 1;
        granulator_01_grains2_active = false;
        granulator_01_grains2_backwards = false;
        granulator_01_grains3_sampleOffset = 0;
        granulator_01_grains3_curRate = 1;
        granulator_01_grains3_curStart = 0;
        granulator_01_grains3_curLen = 1;
        granulator_01_grains3_active = false;
        granulator_01_grains3_backwards = false;
        granulator_01_grains4_sampleOffset = 0;
        granulator_01_grains4_curRate = 1;
        granulator_01_grains4_curStart = 0;
        granulator_01_grains4_curLen = 1;
        granulator_01_grains4_active = false;
        granulator_01_grains4_backwards = false;
        granulator_01_grains5_sampleOffset = 0;
        granulator_01_grains5_curRate = 1;
        granulator_01_grains5_curStart = 0;
        granulator_01_grains5_curLen = 1;
        granulator_01_grains5_active = false;
        granulator_01_grains5_backwards = false;
        granulator_01_grains6_sampleOffset = 0;
        granulator_01_grains6_curRate = 1;
        granulator_01_grains6_curStart = 0;
        granulator_01_grains6_curLen = 1;
        granulator_01_grains6_active = false;
        granulator_01_grains6_backwards = false;
        granulator_01_grains7_sampleOffset = 0;
        granulator_01_grains7_curRate = 1;
        granulator_01_grains7_curStart = 0;
        granulator_01_grains7_curLen = 1;
        granulator_01_grains7_active = false;
        granulator_01_grains7_backwards = false;
        granulator_01_grains8_sampleOffset = 0;
        granulator_01_grains8_curRate = 1;
        granulator_01_grains8_curStart = 0;
        granulator_01_grains8_curLen = 1;
        granulator_01_grains8_active = false;
        granulator_01_grains8_backwards = false;
        granulator_01_grains9_sampleOffset = 0;
        granulator_01_grains9_curRate = 1;
        granulator_01_grains9_curStart = 0;
        granulator_01_grains9_curLen = 1;
        granulator_01_grains9_active = false;
        granulator_01_grains9_backwards = false;
        granulator_01_grains10_sampleOffset = 0;
        granulator_01_grains10_curRate = 1;
        granulator_01_grains10_curStart = 0;
        granulator_01_grains10_curLen = 1;
        granulator_01_grains10_active = false;
        granulator_01_grains10_backwards = false;
        granulator_01_grains11_sampleOffset = 0;
        granulator_01_grains11_curRate = 1;
        granulator_01_grains11_curStart = 0;
        granulator_01_grains11_curLen = 1;
        granulator_01_grains11_active = false;
        granulator_01_grains11_backwards = false;
        granulator_01_grains12_sampleOffset = 0;
        granulator_01_grains12_curRate = 1;
        granulator_01_grains12_curStart = 0;
        granulator_01_grains12_curLen = 1;
        granulator_01_grains12_active = false;
        granulator_01_grains12_backwards = false;
        granulator_01_grains13_sampleOffset = 0;
        granulator_01_grains13_curRate = 1;
        granulator_01_grains13_curStart = 0;
        granulator_01_grains13_curLen = 1;
        granulator_01_grains13_active = false;
        granulator_01_grains13_backwards = false;
        granulator_01_grains14_sampleOffset = 0;
        granulator_01_grains14_curRate = 1;
        granulator_01_grains14_curStart = 0;
        granulator_01_grains14_curLen = 1;
        granulator_01_grains14_active = false;
        granulator_01_grains14_backwards = false;
        granulator_01_grains15_sampleOffset = 0;
        granulator_01_grains15_curRate = 1;
        granulator_01_grains15_curStart = 0;
        granulator_01_grains15_curLen = 1;
        granulator_01_grains15_active = false;
        granulator_01_grains15_backwards = false;
        granulator_01_grains16_sampleOffset = 0;
        granulator_01_grains16_curRate = 1;
        granulator_01_grains16_curStart = 0;
        granulator_01_grains16_curLen = 1;
        granulator_01_grains16_active = false;
        granulator_01_grains16_backwards = false;
        granulator_01_grains17_sampleOffset = 0;
        granulator_01_grains17_curRate = 1;
        granulator_01_grains17_curStart = 0;
        granulator_01_grains17_curLen = 1;
        granulator_01_grains17_active = false;
        granulator_01_grains17_backwards = false;
        granulator_01_grains18_sampleOffset = 0;
        granulator_01_grains18_curRate = 1;
        granulator_01_grains18_curStart = 0;
        granulator_01_grains18_curLen = 1;
        granulator_01_grains18_active = false;
        granulator_01_grains18_backwards = false;
        granulator_01_grains19_sampleOffset = 0;
        granulator_01_grains19_curRate = 1;
        granulator_01_grains19_curStart = 0;
        granulator_01_grains19_curLen = 1;
        granulator_01_grains19_active = false;
        granulator_01_grains19_backwards = false;
        granulator_01_grains20_sampleOffset = 0;
        granulator_01_grains20_curRate = 1;
        granulator_01_grains20_curStart = 0;
        granulator_01_grains20_curLen = 1;
        granulator_01_grains20_active = false;
        granulator_01_grains20_backwards = false;
        granulator_01_grains21_sampleOffset = 0;
        granulator_01_grains21_curRate = 1;
        granulator_01_grains21_curStart = 0;
        granulator_01_grains21_curLen = 1;
        granulator_01_grains21_active = false;
        granulator_01_grains21_backwards = false;
        granulator_01_grains22_sampleOffset = 0;
        granulator_01_grains22_curRate = 1;
        granulator_01_grains22_curStart = 0;
        granulator_01_grains22_curLen = 1;
        granulator_01_grains22_active = false;
        granulator_01_grains22_backwards = false;
        granulator_01_grains23_sampleOffset = 0;
        granulator_01_grains23_curRate = 1;
        granulator_01_grains23_curStart = 0;
        granulator_01_grains23_curLen = 1;
        granulator_01_grains23_active = false;
        granulator_01_grains23_backwards = false;
        granulator_01_grains24_sampleOffset = 0;
        granulator_01_grains24_curRate = 1;
        granulator_01_grains24_curStart = 0;
        granulator_01_grains24_curLen = 1;
        granulator_01_grains24_active = false;
        granulator_01_grains24_backwards = false;
        granulator_01_grains25_sampleOffset = 0;
        granulator_01_grains25_curRate = 1;
        granulator_01_grains25_curStart = 0;
        granulator_01_grains25_curLen = 1;
        granulator_01_grains25_active = false;
        granulator_01_grains25_backwards = false;
        granulator_01_grains26_sampleOffset = 0;
        granulator_01_grains26_curRate = 1;
        granulator_01_grains26_curStart = 0;
        granulator_01_grains26_curLen = 1;
        granulator_01_grains26_active = false;
        granulator_01_grains26_backwards = false;
        granulator_01_grains27_sampleOffset = 0;
        granulator_01_grains27_curRate = 1;
        granulator_01_grains27_curStart = 0;
        granulator_01_grains27_curLen = 1;
        granulator_01_grains27_active = false;
        granulator_01_grains27_backwards = false;
        granulator_01_grains28_sampleOffset = 0;
        granulator_01_grains28_curRate = 1;
        granulator_01_grains28_curStart = 0;
        granulator_01_grains28_curLen = 1;
        granulator_01_grains28_active = false;
        granulator_01_grains28_backwards = false;
        granulator_01_grains29_sampleOffset = 0;
        granulator_01_grains29_curRate = 1;
        granulator_01_grains29_curStart = 0;
        granulator_01_grains29_curLen = 1;
        granulator_01_grains29_active = false;
        granulator_01_grains29_backwards = false;
        granulator_01_grains30_sampleOffset = 0;
        granulator_01_grains30_curRate = 1;
        granulator_01_grains30_curStart = 0;
        granulator_01_grains30_curLen = 1;
        granulator_01_grains30_active = false;
        granulator_01_grains30_backwards = false;
        granulator_01_grains31_sampleOffset = 0;
        granulator_01_grains31_curRate = 1;
        granulator_01_grains31_curStart = 0;
        granulator_01_grains31_curLen = 1;
        granulator_01_grains31_active = false;
        granulator_01_grains31_backwards = false;
        granulator_01_grains32_sampleOffset = 0;
        granulator_01_grains32_curRate = 1;
        granulator_01_grains32_curStart = 0;
        granulator_01_grains32_curLen = 1;
        granulator_01_grains32_active = false;
        granulator_01_grains32_backwards = false;
        param_01_lastValue = 0;
        param_02_lastValue = 0;
        param_03_lastValue = 0;
        param_04_lastValue = 0;
        param_05_lastValue = 0;
        param_06_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number granulator_01_start;
        number granulator_01_length;
        number granulator_01_rate;
        number granulator_01_bchan;
        number granulator_01_limit;
        number granulator_01_panpos;
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number param_01_value;
        number dspexpr_03_in1;
        number dspexpr_03_in2;
        number dspexpr_04_in1;
        number dspexpr_04_in2;
        number dspexpr_05_in1;
        number dspexpr_05_in2;
        number dspexpr_06_in1;
        number dspexpr_06_in2;
        number param_02_value;
        number dspexpr_07_in1;
        number dspexpr_07_in2;
        number param_03_value;
        number dspexpr_08_in1;
        number dspexpr_08_in2;
        number param_04_value;
        number param_05_value;
        number expr_01_in1;
        number expr_01_in2;
        number expr_01_out1;
        number param_06_value;
        number expr_02_in1;
        number expr_02_in2;
        number expr_02_out1;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[6];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef granulator_01_buffer;
        number granulator_01_grainPan[32][2] = { };
        number granulator_01_startMul;
        number granulator_01_lengthMul;
        int granulator_01_limitLast;
        bool granulator_01_edgedetect_active;
        number granulator_01_grains1_sampleOffset;
        number granulator_01_grains1_curRate;
        number granulator_01_grains1_curStart;
        number granulator_01_grains1_curLen;
        bool granulator_01_grains1_active;
        bool granulator_01_grains1_backwards;
        number granulator_01_grains2_sampleOffset;
        number granulator_01_grains2_curRate;
        number granulator_01_grains2_curStart;
        number granulator_01_grains2_curLen;
        bool granulator_01_grains2_active;
        bool granulator_01_grains2_backwards;
        number granulator_01_grains3_sampleOffset;
        number granulator_01_grains3_curRate;
        number granulator_01_grains3_curStart;
        number granulator_01_grains3_curLen;
        bool granulator_01_grains3_active;
        bool granulator_01_grains3_backwards;
        number granulator_01_grains4_sampleOffset;
        number granulator_01_grains4_curRate;
        number granulator_01_grains4_curStart;
        number granulator_01_grains4_curLen;
        bool granulator_01_grains4_active;
        bool granulator_01_grains4_backwards;
        number granulator_01_grains5_sampleOffset;
        number granulator_01_grains5_curRate;
        number granulator_01_grains5_curStart;
        number granulator_01_grains5_curLen;
        bool granulator_01_grains5_active;
        bool granulator_01_grains5_backwards;
        number granulator_01_grains6_sampleOffset;
        number granulator_01_grains6_curRate;
        number granulator_01_grains6_curStart;
        number granulator_01_grains6_curLen;
        bool granulator_01_grains6_active;
        bool granulator_01_grains6_backwards;
        number granulator_01_grains7_sampleOffset;
        number granulator_01_grains7_curRate;
        number granulator_01_grains7_curStart;
        number granulator_01_grains7_curLen;
        bool granulator_01_grains7_active;
        bool granulator_01_grains7_backwards;
        number granulator_01_grains8_sampleOffset;
        number granulator_01_grains8_curRate;
        number granulator_01_grains8_curStart;
        number granulator_01_grains8_curLen;
        bool granulator_01_grains8_active;
        bool granulator_01_grains8_backwards;
        number granulator_01_grains9_sampleOffset;
        number granulator_01_grains9_curRate;
        number granulator_01_grains9_curStart;
        number granulator_01_grains9_curLen;
        bool granulator_01_grains9_active;
        bool granulator_01_grains9_backwards;
        number granulator_01_grains10_sampleOffset;
        number granulator_01_grains10_curRate;
        number granulator_01_grains10_curStart;
        number granulator_01_grains10_curLen;
        bool granulator_01_grains10_active;
        bool granulator_01_grains10_backwards;
        number granulator_01_grains11_sampleOffset;
        number granulator_01_grains11_curRate;
        number granulator_01_grains11_curStart;
        number granulator_01_grains11_curLen;
        bool granulator_01_grains11_active;
        bool granulator_01_grains11_backwards;
        number granulator_01_grains12_sampleOffset;
        number granulator_01_grains12_curRate;
        number granulator_01_grains12_curStart;
        number granulator_01_grains12_curLen;
        bool granulator_01_grains12_active;
        bool granulator_01_grains12_backwards;
        number granulator_01_grains13_sampleOffset;
        number granulator_01_grains13_curRate;
        number granulator_01_grains13_curStart;
        number granulator_01_grains13_curLen;
        bool granulator_01_grains13_active;
        bool granulator_01_grains13_backwards;
        number granulator_01_grains14_sampleOffset;
        number granulator_01_grains14_curRate;
        number granulator_01_grains14_curStart;
        number granulator_01_grains14_curLen;
        bool granulator_01_grains14_active;
        bool granulator_01_grains14_backwards;
        number granulator_01_grains15_sampleOffset;
        number granulator_01_grains15_curRate;
        number granulator_01_grains15_curStart;
        number granulator_01_grains15_curLen;
        bool granulator_01_grains15_active;
        bool granulator_01_grains15_backwards;
        number granulator_01_grains16_sampleOffset;
        number granulator_01_grains16_curRate;
        number granulator_01_grains16_curStart;
        number granulator_01_grains16_curLen;
        bool granulator_01_grains16_active;
        bool granulator_01_grains16_backwards;
        number granulator_01_grains17_sampleOffset;
        number granulator_01_grains17_curRate;
        number granulator_01_grains17_curStart;
        number granulator_01_grains17_curLen;
        bool granulator_01_grains17_active;
        bool granulator_01_grains17_backwards;
        number granulator_01_grains18_sampleOffset;
        number granulator_01_grains18_curRate;
        number granulator_01_grains18_curStart;
        number granulator_01_grains18_curLen;
        bool granulator_01_grains18_active;
        bool granulator_01_grains18_backwards;
        number granulator_01_grains19_sampleOffset;
        number granulator_01_grains19_curRate;
        number granulator_01_grains19_curStart;
        number granulator_01_grains19_curLen;
        bool granulator_01_grains19_active;
        bool granulator_01_grains19_backwards;
        number granulator_01_grains20_sampleOffset;
        number granulator_01_grains20_curRate;
        number granulator_01_grains20_curStart;
        number granulator_01_grains20_curLen;
        bool granulator_01_grains20_active;
        bool granulator_01_grains20_backwards;
        number granulator_01_grains21_sampleOffset;
        number granulator_01_grains21_curRate;
        number granulator_01_grains21_curStart;
        number granulator_01_grains21_curLen;
        bool granulator_01_grains21_active;
        bool granulator_01_grains21_backwards;
        number granulator_01_grains22_sampleOffset;
        number granulator_01_grains22_curRate;
        number granulator_01_grains22_curStart;
        number granulator_01_grains22_curLen;
        bool granulator_01_grains22_active;
        bool granulator_01_grains22_backwards;
        number granulator_01_grains23_sampleOffset;
        number granulator_01_grains23_curRate;
        number granulator_01_grains23_curStart;
        number granulator_01_grains23_curLen;
        bool granulator_01_grains23_active;
        bool granulator_01_grains23_backwards;
        number granulator_01_grains24_sampleOffset;
        number granulator_01_grains24_curRate;
        number granulator_01_grains24_curStart;
        number granulator_01_grains24_curLen;
        bool granulator_01_grains24_active;
        bool granulator_01_grains24_backwards;
        number granulator_01_grains25_sampleOffset;
        number granulator_01_grains25_curRate;
        number granulator_01_grains25_curStart;
        number granulator_01_grains25_curLen;
        bool granulator_01_grains25_active;
        bool granulator_01_grains25_backwards;
        number granulator_01_grains26_sampleOffset;
        number granulator_01_grains26_curRate;
        number granulator_01_grains26_curStart;
        number granulator_01_grains26_curLen;
        bool granulator_01_grains26_active;
        bool granulator_01_grains26_backwards;
        number granulator_01_grains27_sampleOffset;
        number granulator_01_grains27_curRate;
        number granulator_01_grains27_curStart;
        number granulator_01_grains27_curLen;
        bool granulator_01_grains27_active;
        bool granulator_01_grains27_backwards;
        number granulator_01_grains28_sampleOffset;
        number granulator_01_grains28_curRate;
        number granulator_01_grains28_curStart;
        number granulator_01_grains28_curLen;
        bool granulator_01_grains28_active;
        bool granulator_01_grains28_backwards;
        number granulator_01_grains29_sampleOffset;
        number granulator_01_grains29_curRate;
        number granulator_01_grains29_curStart;
        number granulator_01_grains29_curLen;
        bool granulator_01_grains29_active;
        bool granulator_01_grains29_backwards;
        number granulator_01_grains30_sampleOffset;
        number granulator_01_grains30_curRate;
        number granulator_01_grains30_curStart;
        number granulator_01_grains30_curLen;
        bool granulator_01_grains30_active;
        bool granulator_01_grains30_backwards;
        number granulator_01_grains31_sampleOffset;
        number granulator_01_grains31_curRate;
        number granulator_01_grains31_curStart;
        number granulator_01_grains31_curLen;
        bool granulator_01_grains31_active;
        bool granulator_01_grains31_backwards;
        number granulator_01_grains32_sampleOffset;
        number granulator_01_grains32_curRate;
        number granulator_01_grains32_curStart;
        number granulator_01_grains32_curLen;
        bool granulator_01_grains32_active;
        bool granulator_01_grains32_backwards;
        number param_01_lastValue;
        number param_02_lastValue;
        number param_03_lastValue;
        number param_04_lastValue;
        number param_05_lastValue;
        number param_06_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_99 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_99()
    {
    }
    
    ~RNBOSubpatcher_99()
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
        getEngine()->flushClockEvents(this, -871642103, false);
    }
    
    inline number safemod(number f, number m) {
        if (m != 0) {
            if (m < 0) {
                m = -m;
            }
    
            if (f >= m) {
                if (f >= m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f -= m;
                }
            } else if (f <= -m) {
                if (f <= -m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f += m;
                }
            }
        } else {
            f = 0.0;
        }
    
        return f;
    }
    
    inline number safesqrt(number num) {
        return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index bufferbindchannel(const Index channel, const Index maxChannels, const int channelmode) {
        if (channelmode == 0 || channelmode == 4) {
            return maxChannels - 1 - channel % maxChannels;
        } else if (channelmode == 3) {
            return (maxChannels == 0 ? 0 : channel % maxChannels);
        } else {
            return (channel > maxChannels - 1 ? maxChannels - 1 : (channel < 0 ? 0 : channel));
        }
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
    
    SampleIndex bufferbindindex(
        const SampleValue index,
        const SampleIndex start,
        const SampleIndex end,
        const int boundmode
    ) {
        if (boundmode == 0 || boundmode == 4) {
            if (index >= start && index < end)
                return index;
            else {
                SampleIndex length = (SampleIndex)(end - start);
                number index1 = index - start;
                number wrap = (index1 < 0 ? length * 2 - 1 + this->safemod(index1 + 1, length * 2) : this->safemod(index1, length * 2));
                return (start + wrap >= length ? length * 2 - wrap - 1 : wrap);
            }
        } else if (boundmode == 3) {
            return this->wrap(index, start, end);
        } else {
            return (index > end - 1 ? end - 1 : (index < start ? start : index));
        }
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
    
    template <typename T> inline SampleValue bufferreadsample(
        T& buffer,
        const SampleValue sampleIndex,
        const Index channel,
        const SampleIndex start,
        const SampleIndex end,
        const int interp
    ) {
        if (sampleIndex < 0.0) {
            return 0.0;
        }
    
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(sampleIndex));
    
        if (interp == 0) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->linearinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else if (interp == 1) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->cubicinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 2) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->splineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 3) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->cosineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else {
            return buffer->getSample(channel, index1);
        }
    }
    
    number hann(number x) {
        x = (x > 1 ? 1 : (x < 0 ? 0 : x));
        return 0.5 * (1 - rnbo_cos(6.28318530717958647692 * x));
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "start")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "len")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "rate")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "pan")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "size")) {
            return 5;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        this->dspexpr_10_perform(in2, this->dspexpr_10_in2, this->signals[0], n);
        this->dspexpr_14_perform(in3, this->dspexpr_14_in2, this->signals[1], n);
        this->dspexpr_15_perform(in4, this->dspexpr_15_in2, this->signals[2], n);
        this->dspexpr_16_perform(in5, this->dspexpr_16_in2, this->signals[3], n);
        this->dspexpr_13_perform(this->signals[3], this->dspexpr_13_in2, this->signals[4], n);
        this->dspexpr_12_perform(this->signals[4], this->dspexpr_12_in2, this->signals[3], n);
    
        this->granulator_02_perform(
            this->signals[0],
            this->signals[1],
            this->signals[2],
            this->granulator_02_bchan,
            this->signals[3],
            in1,
            this->signals[4],
            this->signals[5],
            n
        );
    
        this->dspexpr_09_perform(this->signals[4], this->dspexpr_09_in2, out1, n);
        this->dspexpr_11_perform(this->signals[5], this->dspexpr_11_in2, out2, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 6; i++) {
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
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_07_getPresetValue(getSubState(preset, "start"));
        this->param_08_getPresetValue(getSubState(preset, "len"));
        this->param_09_getPresetValue(getSubState(preset, "gain"));
        this->param_10_getPresetValue(getSubState(preset, "rate"));
        this->param_11_getPresetValue(getSubState(preset, "pan"));
        this->param_12_getPresetValue(getSubState(preset, "size"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double samplerate) {
        this->granulator_02_onSampleRateChanged(samplerate);
        this->samplerate_02_onSampleRateChanged(samplerate);
    }
    
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
        case 2:
            this->param_09_value_set(v);
            break;
        case 3:
            this->param_10_value_set(v);
            break;
        case 4:
            this->param_11_value_set(v);
            break;
        case 5:
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
            return this->param_07_value;
        case 1:
            return this->param_08_value;
        case 2:
            return this->param_09_value;
        case 3:
            return this->param_10_value;
        case 4:
            return this->param_11_value;
        case 5:
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
        return 6;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "start";
        case 1:
            return "len";
        case 2:
            return "gain";
        case 3:
            return "rate";
        case 4:
            return "pan";
        case 5:
            return "size";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer3/start";
        case 1:
            return "layer3/len";
        case 2:
            return "layer3/gain";
        case 3:
            return "layer3/rate";
        case 4:
            return "layer3/pan";
        case 5:
            return "layer3/size";
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
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Start";
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
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Length";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
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
                info->displayName = "Gain";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
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
                info->displayName = "Rate";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = -1;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Pan";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 180;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Size";
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
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 180 ? 180 : value));
                ParameterValue normalizedValue = (value - 0) / (180 - 0);
                return normalizedValue;
            }
        case 3:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        case 4:
            {
                value = (value < -1 ? -1 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - -1) / (1 - -1);
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
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (180 - 0);
                }
            }
        case 3:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -1 + value * (1 - -1);
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
        case 2:
            return this->param_09_value_constrain(value);
        case 3:
            return this->param_10_value_constrain(value);
        case 4:
            return this->param_11_value_constrain(value);
        case 5:
            return this->param_12_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -871642103:
            this->loadbang_02_startupbang_bang();
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("startupbang"):
            if (TAG("layer3/loadbang_obj-42") == objectId)
                this->loadbang_02_startupbang_bang();
    
            break;
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("startupbang"):
            return "startupbang";
        case TAG("layer3/loadbang_obj-42"):
            return "layer3/loadbang_obj-42";
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
            this->granulator_02_buffer = new Float32Buffer(this->getPatcher()->layer3);
            this->granulator_02_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->granulator_02_buffer = new Float32Buffer(this->getPatcher()->layer3);
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
    
        this->dspexpr_10_in2_set(v);
    }
    
    void param_08_value_set(number v) {
        v = this->param_08_value_constrain(v);
        this->param_08_value = v;
        this->sendParameter(1, false);
    
        if (this->param_08_value != this->param_08_lastValue) {
            this->getEngine()->presetTouched();
            this->param_08_lastValue = this->param_08_value;
        }
    
        this->dspexpr_14_in2_set(v);
    }
    
    void param_09_value_set(number v) {
        v = this->param_09_value_constrain(v);
        this->param_09_value = v;
        this->sendParameter(2, false);
    
        if (this->param_09_value != this->param_09_lastValue) {
            this->getEngine()->presetTouched();
            this->param_09_lastValue = this->param_09_value;
        }
    
        this->dspexpr_11_in2_set(v);
        this->dspexpr_09_in2_set(v);
    }
    
    void param_10_value_set(number v) {
        v = this->param_10_value_constrain(v);
        this->param_10_value = v;
        this->sendParameter(3, false);
    
        if (this->param_10_value != this->param_10_lastValue) {
            this->getEngine()->presetTouched();
            this->param_10_lastValue = this->param_10_value;
        }
    
        this->dspexpr_15_in2_set(v);
    }
    
    void param_11_value_set(number v) {
        v = this->param_11_value_constrain(v);
        this->param_11_value = v;
        this->sendParameter(4, false);
    
        if (this->param_11_value != this->param_11_lastValue) {
            this->getEngine()->presetTouched();
            this->param_11_lastValue = this->param_11_value;
        }
    
        this->dspexpr_16_in2_set(v);
    }
    
    void param_12_value_set(number v) {
        v = this->param_12_value_constrain(v);
        this->param_12_value = v;
        this->sendParameter(5, false);
    
        if (this->param_12_value != this->param_12_lastValue) {
            this->getEngine()->presetTouched();
            this->param_12_lastValue = this->param_12_value;
        }
    
        this->expr_03_in1_set(v);
    }
    
    void loadbang_02_startupbang_bang() {
        this->loadbang_02_output_bang();
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
        return 5;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->granulator_02_edgedetect_init();
        this->granulator_02_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    
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
    }
    
    void allocateDataRefs() {
        this->granulator_02_buffer = this->granulator_02_buffer->allocateIfNeeded();
    }
    
    static number param_07_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_10_in2_set(number v) {
        this->dspexpr_10_in2 = v;
    }
    
    static number param_08_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_14_in2_set(number v) {
        this->dspexpr_14_in2 = v;
    }
    
    static number param_09_value_constrain(number v) {
        v = (v > 10 ? 10 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)10 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_11_in2_set(number v) {
        this->dspexpr_11_in2 = v;
    }
    
    void dspexpr_09_in2_set(number v) {
        this->dspexpr_09_in2 = v;
    }
    
    static number param_10_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void dspexpr_15_in2_set(number v) {
        this->dspexpr_15_in2 = v;
    }
    
    static number param_11_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < -1 ? -1 : v));
        return v;
    }
    
    void dspexpr_16_in2_set(number v) {
        this->dspexpr_16_in2 = v;
    }
    
    static number param_12_value_constrain(number v) {
        v = (v > 180 ? 180 : (v < 0 ? 0 : v));
        return v;
    }
    
    void eventoutlet_02_in1_number_set(number v) {
        this->getPatcher()->p_02_out3_number_set(v);
    }
    
    void expr_03_out1_set(number v) {
        this->expr_03_out1 = v;
        this->eventoutlet_02_in1_number_set(this->expr_03_out1);
    }
    
    void expr_03_in1_set(number in1) {
        this->expr_03_in1 = in1;
        this->expr_03_out1_set(this->expr_03_in1 * this->expr_03_in2);//#map:layer3/*_obj-43:1
    }
    
    void expr_03_in2_set(number v) {
        this->expr_03_in2 = v;
    }
    
    void expr_04_out1_set(number v) {
        this->expr_04_out1 = v;
        this->expr_03_in2_set(this->expr_04_out1);
    }
    
    void expr_04_in1_set(number in1) {
        this->expr_04_in1 = in1;
    
        this->expr_04_out1_set(
            (this->expr_04_in2 == 0 ? 0 : (this->expr_04_in2 == 0. ? 0. : this->expr_04_in1 / this->expr_04_in2))
        );//#map:layer3//_obj-44:1
    }
    
    void samplerate_02_samplerate_set(number v) {
        this->expr_04_in1_set(v);
    }
    
    void samplerate_02_input_bang() {
        this->samplerate_02_samplerate_set(this->samplerate());
    }
    
    void loadbang_02_output_bang() {
        this->samplerate_02_input_bang();
    }
    
    void dspexpr_10_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_14_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_15_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_16_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_13_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_12_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void granulator_02_perform(
        const Sample * start,
        const Sample * length,
        const Sample * rate,
        number bchan,
        const Sample * panpos,
        const Sample * trigger,
        Sample * out1,
        Sample * out2,
        Index n
    ) {
        RNBO_UNUSED(bchan);
        auto __granulator_02_grainPan = this->granulator_02_grainPan;
        auto __granulator_02_lengthMul = this->granulator_02_lengthMul;
        auto __granulator_02_startMul = this->granulator_02_startMul;
        auto __granulator_02_limitLast = this->granulator_02_limitLast;
        auto __granulator_02_limit = this->granulator_02_limit;
        number o = 0;
        Index i;
    
        for (i = 0; i < n; i++) {
            bool trig = (bool)(this->granulator_02_edgedetect_next(trigger[(Index)i]) > 0.0);
            int limit = (int)((__granulator_02_limit > 32 ? 32 : (__granulator_02_limit < 0 ? 0 : __granulator_02_limit)));
            __granulator_02_limitLast = limit;
            out1[(Index)i] = 0;
            out2[(Index)i] = 0;
            number s = start[(Index)i];
            number r = rate[(Index)i];
            number l = length[(Index)i];
    
            for (Index j = 0; j < 32; j++) {
                number thistrig = 0;
    
                if ((bool)(trig) && ((bool)(this->granulator_02_grains_inactive(j)) && j < limit)) {
                    trig = false;
                    thistrig = 1;
    
                    {
                        s *= __granulator_02_startMul;
                    }
    
                    {
                        l *= __granulator_02_lengthMul;
                    }
    
                    __granulator_02_grainPan[(Index)j][0] = this->granulator_02_panfunc_next(panpos[(Index)i], 0);
                    __granulator_02_grainPan[(Index)j][1] = this->granulator_02_panfunc_next(panpos[(Index)i], 1);
                }
    
                if (thistrig != 0 || (bool)(!(bool)(this->granulator_02_grains_inactive(j)))) {
                    array<number, 3> ret = this->granulator_02_grains_next(j, this->granulator_02_buffer, thistrig, s, l, r, 0);
                    number v = ret[0] * this->hann(ret[1]);
                    out1[(Index)i] += v * __granulator_02_grainPan[(Index)j][0];
                    out2[(Index)i] += v * __granulator_02_grainPan[(Index)j][1];
                }
            }
        }
    
        this->granulator_02_limitLast = __granulator_02_limitLast;
    }
    
    void dspexpr_09_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_11_perform(const Sample * in1, number in2, Sample * out1, Index n) {
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
    
    number granulator_02_edgedetect_next(number val) {
        if ((0 == 0 && val <= 0) || (0 == 1 && val > 0)) {
            this->granulator_02_edgedetect_active = false;
        } else if ((bool)(!(bool)(this->granulator_02_edgedetect_active))) {
            this->granulator_02_edgedetect_active = true;
            return 1.0;
        }
    
        return 0.0;
    }
    
    void granulator_02_edgedetect_init() {}
    
    void granulator_02_edgedetect_reset() {
        this->granulator_02_edgedetect_active = false;
    }
    
    number granulator_02_panfunc_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    {
                        if (pos >= 0) {
                            pos = this->safemod(pos, 2);
                        } else {
                            pos = this->safemod(2 + this->safemod(pos, 2), 2);
                        }
    
                        if (channel == 0 && pos >= nchan_1)
                            pos = pos - 2;
                    }
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        {
                            return this->safesqrt(1.0 - rnbo_abs(pos));
                        }
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void granulator_02_panfunc_reset() {}
    
    bool granulator_02_grains1_inactive() {
        return !(bool)(this->granulator_02_grains1_active);
    }
    
    void granulator_02_grains1_reset() {
        this->granulator_02_grains1_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains1_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains1_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains1_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains1_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains1_curRate = 0;
                this->granulator_02_grains1_active = false;
            } else {
                this->granulator_02_grains1_curLen = length;
                this->granulator_02_grains1_active = true;
                this->granulator_02_grains1_backwards = rate < 0;
                this->granulator_02_grains1_sampleOffset = 0;
                this->granulator_02_grains1_curStart = start;
    
                if ((bool)(this->granulator_02_grains1_backwards)) {
                    this->granulator_02_grains1_curStart += this->granulator_02_grains1_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains1_active)) {
            if (this->granulator_02_grains1_sampleOffset >= this->granulator_02_grains1_curLen) {
                bangDone = true;
                this->granulator_02_grains1_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains1_curStart;
    
                if ((bool)(this->granulator_02_grains1_backwards)) {
                    sampleIndex -= this->granulator_02_grains1_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains1_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains1_sampleOffset / this->granulator_02_grains1_curLen > 1 ? 1 : (this->granulator_02_grains1_sampleOffset / this->granulator_02_grains1_curLen < 0 ? 0 : this->granulator_02_grains1_sampleOffset / this->granulator_02_grains1_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains1_sampleOffset += this->granulator_02_grains1_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains2_inactive() {
        return !(bool)(this->granulator_02_grains2_active);
    }
    
    void granulator_02_grains2_reset() {
        this->granulator_02_grains2_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains2_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains2_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains2_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains2_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains2_curRate = 0;
                this->granulator_02_grains2_active = false;
            } else {
                this->granulator_02_grains2_curLen = length;
                this->granulator_02_grains2_active = true;
                this->granulator_02_grains2_backwards = rate < 0;
                this->granulator_02_grains2_sampleOffset = 0;
                this->granulator_02_grains2_curStart = start;
    
                if ((bool)(this->granulator_02_grains2_backwards)) {
                    this->granulator_02_grains2_curStart += this->granulator_02_grains2_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains2_active)) {
            if (this->granulator_02_grains2_sampleOffset >= this->granulator_02_grains2_curLen) {
                bangDone = true;
                this->granulator_02_grains2_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains2_curStart;
    
                if ((bool)(this->granulator_02_grains2_backwards)) {
                    sampleIndex -= this->granulator_02_grains2_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains2_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains2_sampleOffset / this->granulator_02_grains2_curLen > 1 ? 1 : (this->granulator_02_grains2_sampleOffset / this->granulator_02_grains2_curLen < 0 ? 0 : this->granulator_02_grains2_sampleOffset / this->granulator_02_grains2_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains2_sampleOffset += this->granulator_02_grains2_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains3_inactive() {
        return !(bool)(this->granulator_02_grains3_active);
    }
    
    void granulator_02_grains3_reset() {
        this->granulator_02_grains3_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains3_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains3_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains3_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains3_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains3_curRate = 0;
                this->granulator_02_grains3_active = false;
            } else {
                this->granulator_02_grains3_curLen = length;
                this->granulator_02_grains3_active = true;
                this->granulator_02_grains3_backwards = rate < 0;
                this->granulator_02_grains3_sampleOffset = 0;
                this->granulator_02_grains3_curStart = start;
    
                if ((bool)(this->granulator_02_grains3_backwards)) {
                    this->granulator_02_grains3_curStart += this->granulator_02_grains3_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains3_active)) {
            if (this->granulator_02_grains3_sampleOffset >= this->granulator_02_grains3_curLen) {
                bangDone = true;
                this->granulator_02_grains3_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains3_curStart;
    
                if ((bool)(this->granulator_02_grains3_backwards)) {
                    sampleIndex -= this->granulator_02_grains3_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains3_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains3_sampleOffset / this->granulator_02_grains3_curLen > 1 ? 1 : (this->granulator_02_grains3_sampleOffset / this->granulator_02_grains3_curLen < 0 ? 0 : this->granulator_02_grains3_sampleOffset / this->granulator_02_grains3_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains3_sampleOffset += this->granulator_02_grains3_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains4_inactive() {
        return !(bool)(this->granulator_02_grains4_active);
    }
    
    void granulator_02_grains4_reset() {
        this->granulator_02_grains4_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains4_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains4_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains4_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains4_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains4_curRate = 0;
                this->granulator_02_grains4_active = false;
            } else {
                this->granulator_02_grains4_curLen = length;
                this->granulator_02_grains4_active = true;
                this->granulator_02_grains4_backwards = rate < 0;
                this->granulator_02_grains4_sampleOffset = 0;
                this->granulator_02_grains4_curStart = start;
    
                if ((bool)(this->granulator_02_grains4_backwards)) {
                    this->granulator_02_grains4_curStart += this->granulator_02_grains4_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains4_active)) {
            if (this->granulator_02_grains4_sampleOffset >= this->granulator_02_grains4_curLen) {
                bangDone = true;
                this->granulator_02_grains4_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains4_curStart;
    
                if ((bool)(this->granulator_02_grains4_backwards)) {
                    sampleIndex -= this->granulator_02_grains4_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains4_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains4_sampleOffset / this->granulator_02_grains4_curLen > 1 ? 1 : (this->granulator_02_grains4_sampleOffset / this->granulator_02_grains4_curLen < 0 ? 0 : this->granulator_02_grains4_sampleOffset / this->granulator_02_grains4_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains4_sampleOffset += this->granulator_02_grains4_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains5_inactive() {
        return !(bool)(this->granulator_02_grains5_active);
    }
    
    void granulator_02_grains5_reset() {
        this->granulator_02_grains5_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains5_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains5_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains5_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains5_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains5_curRate = 0;
                this->granulator_02_grains5_active = false;
            } else {
                this->granulator_02_grains5_curLen = length;
                this->granulator_02_grains5_active = true;
                this->granulator_02_grains5_backwards = rate < 0;
                this->granulator_02_grains5_sampleOffset = 0;
                this->granulator_02_grains5_curStart = start;
    
                if ((bool)(this->granulator_02_grains5_backwards)) {
                    this->granulator_02_grains5_curStart += this->granulator_02_grains5_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains5_active)) {
            if (this->granulator_02_grains5_sampleOffset >= this->granulator_02_grains5_curLen) {
                bangDone = true;
                this->granulator_02_grains5_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains5_curStart;
    
                if ((bool)(this->granulator_02_grains5_backwards)) {
                    sampleIndex -= this->granulator_02_grains5_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains5_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains5_sampleOffset / this->granulator_02_grains5_curLen > 1 ? 1 : (this->granulator_02_grains5_sampleOffset / this->granulator_02_grains5_curLen < 0 ? 0 : this->granulator_02_grains5_sampleOffset / this->granulator_02_grains5_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains5_sampleOffset += this->granulator_02_grains5_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains6_inactive() {
        return !(bool)(this->granulator_02_grains6_active);
    }
    
    void granulator_02_grains6_reset() {
        this->granulator_02_grains6_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains6_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains6_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains6_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains6_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains6_curRate = 0;
                this->granulator_02_grains6_active = false;
            } else {
                this->granulator_02_grains6_curLen = length;
                this->granulator_02_grains6_active = true;
                this->granulator_02_grains6_backwards = rate < 0;
                this->granulator_02_grains6_sampleOffset = 0;
                this->granulator_02_grains6_curStart = start;
    
                if ((bool)(this->granulator_02_grains6_backwards)) {
                    this->granulator_02_grains6_curStart += this->granulator_02_grains6_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains6_active)) {
            if (this->granulator_02_grains6_sampleOffset >= this->granulator_02_grains6_curLen) {
                bangDone = true;
                this->granulator_02_grains6_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains6_curStart;
    
                if ((bool)(this->granulator_02_grains6_backwards)) {
                    sampleIndex -= this->granulator_02_grains6_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains6_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains6_sampleOffset / this->granulator_02_grains6_curLen > 1 ? 1 : (this->granulator_02_grains6_sampleOffset / this->granulator_02_grains6_curLen < 0 ? 0 : this->granulator_02_grains6_sampleOffset / this->granulator_02_grains6_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains6_sampleOffset += this->granulator_02_grains6_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains7_inactive() {
        return !(bool)(this->granulator_02_grains7_active);
    }
    
    void granulator_02_grains7_reset() {
        this->granulator_02_grains7_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains7_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains7_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains7_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains7_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains7_curRate = 0;
                this->granulator_02_grains7_active = false;
            } else {
                this->granulator_02_grains7_curLen = length;
                this->granulator_02_grains7_active = true;
                this->granulator_02_grains7_backwards = rate < 0;
                this->granulator_02_grains7_sampleOffset = 0;
                this->granulator_02_grains7_curStart = start;
    
                if ((bool)(this->granulator_02_grains7_backwards)) {
                    this->granulator_02_grains7_curStart += this->granulator_02_grains7_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains7_active)) {
            if (this->granulator_02_grains7_sampleOffset >= this->granulator_02_grains7_curLen) {
                bangDone = true;
                this->granulator_02_grains7_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains7_curStart;
    
                if ((bool)(this->granulator_02_grains7_backwards)) {
                    sampleIndex -= this->granulator_02_grains7_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains7_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains7_sampleOffset / this->granulator_02_grains7_curLen > 1 ? 1 : (this->granulator_02_grains7_sampleOffset / this->granulator_02_grains7_curLen < 0 ? 0 : this->granulator_02_grains7_sampleOffset / this->granulator_02_grains7_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains7_sampleOffset += this->granulator_02_grains7_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains8_inactive() {
        return !(bool)(this->granulator_02_grains8_active);
    }
    
    void granulator_02_grains8_reset() {
        this->granulator_02_grains8_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains8_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains8_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains8_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains8_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains8_curRate = 0;
                this->granulator_02_grains8_active = false;
            } else {
                this->granulator_02_grains8_curLen = length;
                this->granulator_02_grains8_active = true;
                this->granulator_02_grains8_backwards = rate < 0;
                this->granulator_02_grains8_sampleOffset = 0;
                this->granulator_02_grains8_curStart = start;
    
                if ((bool)(this->granulator_02_grains8_backwards)) {
                    this->granulator_02_grains8_curStart += this->granulator_02_grains8_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains8_active)) {
            if (this->granulator_02_grains8_sampleOffset >= this->granulator_02_grains8_curLen) {
                bangDone = true;
                this->granulator_02_grains8_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains8_curStart;
    
                if ((bool)(this->granulator_02_grains8_backwards)) {
                    sampleIndex -= this->granulator_02_grains8_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains8_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains8_sampleOffset / this->granulator_02_grains8_curLen > 1 ? 1 : (this->granulator_02_grains8_sampleOffset / this->granulator_02_grains8_curLen < 0 ? 0 : this->granulator_02_grains8_sampleOffset / this->granulator_02_grains8_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains8_sampleOffset += this->granulator_02_grains8_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains9_inactive() {
        return !(bool)(this->granulator_02_grains9_active);
    }
    
    void granulator_02_grains9_reset() {
        this->granulator_02_grains9_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains9_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains9_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains9_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains9_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains9_curRate = 0;
                this->granulator_02_grains9_active = false;
            } else {
                this->granulator_02_grains9_curLen = length;
                this->granulator_02_grains9_active = true;
                this->granulator_02_grains9_backwards = rate < 0;
                this->granulator_02_grains9_sampleOffset = 0;
                this->granulator_02_grains9_curStart = start;
    
                if ((bool)(this->granulator_02_grains9_backwards)) {
                    this->granulator_02_grains9_curStart += this->granulator_02_grains9_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains9_active)) {
            if (this->granulator_02_grains9_sampleOffset >= this->granulator_02_grains9_curLen) {
                bangDone = true;
                this->granulator_02_grains9_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains9_curStart;
    
                if ((bool)(this->granulator_02_grains9_backwards)) {
                    sampleIndex -= this->granulator_02_grains9_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains9_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains9_sampleOffset / this->granulator_02_grains9_curLen > 1 ? 1 : (this->granulator_02_grains9_sampleOffset / this->granulator_02_grains9_curLen < 0 ? 0 : this->granulator_02_grains9_sampleOffset / this->granulator_02_grains9_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains9_sampleOffset += this->granulator_02_grains9_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains10_inactive() {
        return !(bool)(this->granulator_02_grains10_active);
    }
    
    void granulator_02_grains10_reset() {
        this->granulator_02_grains10_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains10_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains10_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains10_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains10_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains10_curRate = 0;
                this->granulator_02_grains10_active = false;
            } else {
                this->granulator_02_grains10_curLen = length;
                this->granulator_02_grains10_active = true;
                this->granulator_02_grains10_backwards = rate < 0;
                this->granulator_02_grains10_sampleOffset = 0;
                this->granulator_02_grains10_curStart = start;
    
                if ((bool)(this->granulator_02_grains10_backwards)) {
                    this->granulator_02_grains10_curStart += this->granulator_02_grains10_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains10_active)) {
            if (this->granulator_02_grains10_sampleOffset >= this->granulator_02_grains10_curLen) {
                bangDone = true;
                this->granulator_02_grains10_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains10_curStart;
    
                if ((bool)(this->granulator_02_grains10_backwards)) {
                    sampleIndex -= this->granulator_02_grains10_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains10_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains10_sampleOffset / this->granulator_02_grains10_curLen > 1 ? 1 : (this->granulator_02_grains10_sampleOffset / this->granulator_02_grains10_curLen < 0 ? 0 : this->granulator_02_grains10_sampleOffset / this->granulator_02_grains10_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains10_sampleOffset += this->granulator_02_grains10_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains11_inactive() {
        return !(bool)(this->granulator_02_grains11_active);
    }
    
    void granulator_02_grains11_reset() {
        this->granulator_02_grains11_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains11_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains11_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains11_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains11_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains11_curRate = 0;
                this->granulator_02_grains11_active = false;
            } else {
                this->granulator_02_grains11_curLen = length;
                this->granulator_02_grains11_active = true;
                this->granulator_02_grains11_backwards = rate < 0;
                this->granulator_02_grains11_sampleOffset = 0;
                this->granulator_02_grains11_curStart = start;
    
                if ((bool)(this->granulator_02_grains11_backwards)) {
                    this->granulator_02_grains11_curStart += this->granulator_02_grains11_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains11_active)) {
            if (this->granulator_02_grains11_sampleOffset >= this->granulator_02_grains11_curLen) {
                bangDone = true;
                this->granulator_02_grains11_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains11_curStart;
    
                if ((bool)(this->granulator_02_grains11_backwards)) {
                    sampleIndex -= this->granulator_02_grains11_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains11_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains11_sampleOffset / this->granulator_02_grains11_curLen > 1 ? 1 : (this->granulator_02_grains11_sampleOffset / this->granulator_02_grains11_curLen < 0 ? 0 : this->granulator_02_grains11_sampleOffset / this->granulator_02_grains11_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains11_sampleOffset += this->granulator_02_grains11_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains12_inactive() {
        return !(bool)(this->granulator_02_grains12_active);
    }
    
    void granulator_02_grains12_reset() {
        this->granulator_02_grains12_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains12_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains12_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains12_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains12_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains12_curRate = 0;
                this->granulator_02_grains12_active = false;
            } else {
                this->granulator_02_grains12_curLen = length;
                this->granulator_02_grains12_active = true;
                this->granulator_02_grains12_backwards = rate < 0;
                this->granulator_02_grains12_sampleOffset = 0;
                this->granulator_02_grains12_curStart = start;
    
                if ((bool)(this->granulator_02_grains12_backwards)) {
                    this->granulator_02_grains12_curStart += this->granulator_02_grains12_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains12_active)) {
            if (this->granulator_02_grains12_sampleOffset >= this->granulator_02_grains12_curLen) {
                bangDone = true;
                this->granulator_02_grains12_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains12_curStart;
    
                if ((bool)(this->granulator_02_grains12_backwards)) {
                    sampleIndex -= this->granulator_02_grains12_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains12_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains12_sampleOffset / this->granulator_02_grains12_curLen > 1 ? 1 : (this->granulator_02_grains12_sampleOffset / this->granulator_02_grains12_curLen < 0 ? 0 : this->granulator_02_grains12_sampleOffset / this->granulator_02_grains12_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains12_sampleOffset += this->granulator_02_grains12_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains13_inactive() {
        return !(bool)(this->granulator_02_grains13_active);
    }
    
    void granulator_02_grains13_reset() {
        this->granulator_02_grains13_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains13_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains13_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains13_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains13_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains13_curRate = 0;
                this->granulator_02_grains13_active = false;
            } else {
                this->granulator_02_grains13_curLen = length;
                this->granulator_02_grains13_active = true;
                this->granulator_02_grains13_backwards = rate < 0;
                this->granulator_02_grains13_sampleOffset = 0;
                this->granulator_02_grains13_curStart = start;
    
                if ((bool)(this->granulator_02_grains13_backwards)) {
                    this->granulator_02_grains13_curStart += this->granulator_02_grains13_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains13_active)) {
            if (this->granulator_02_grains13_sampleOffset >= this->granulator_02_grains13_curLen) {
                bangDone = true;
                this->granulator_02_grains13_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains13_curStart;
    
                if ((bool)(this->granulator_02_grains13_backwards)) {
                    sampleIndex -= this->granulator_02_grains13_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains13_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains13_sampleOffset / this->granulator_02_grains13_curLen > 1 ? 1 : (this->granulator_02_grains13_sampleOffset / this->granulator_02_grains13_curLen < 0 ? 0 : this->granulator_02_grains13_sampleOffset / this->granulator_02_grains13_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains13_sampleOffset += this->granulator_02_grains13_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains14_inactive() {
        return !(bool)(this->granulator_02_grains14_active);
    }
    
    void granulator_02_grains14_reset() {
        this->granulator_02_grains14_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains14_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains14_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains14_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains14_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains14_curRate = 0;
                this->granulator_02_grains14_active = false;
            } else {
                this->granulator_02_grains14_curLen = length;
                this->granulator_02_grains14_active = true;
                this->granulator_02_grains14_backwards = rate < 0;
                this->granulator_02_grains14_sampleOffset = 0;
                this->granulator_02_grains14_curStart = start;
    
                if ((bool)(this->granulator_02_grains14_backwards)) {
                    this->granulator_02_grains14_curStart += this->granulator_02_grains14_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains14_active)) {
            if (this->granulator_02_grains14_sampleOffset >= this->granulator_02_grains14_curLen) {
                bangDone = true;
                this->granulator_02_grains14_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains14_curStart;
    
                if ((bool)(this->granulator_02_grains14_backwards)) {
                    sampleIndex -= this->granulator_02_grains14_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains14_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains14_sampleOffset / this->granulator_02_grains14_curLen > 1 ? 1 : (this->granulator_02_grains14_sampleOffset / this->granulator_02_grains14_curLen < 0 ? 0 : this->granulator_02_grains14_sampleOffset / this->granulator_02_grains14_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains14_sampleOffset += this->granulator_02_grains14_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains15_inactive() {
        return !(bool)(this->granulator_02_grains15_active);
    }
    
    void granulator_02_grains15_reset() {
        this->granulator_02_grains15_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains15_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains15_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains15_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains15_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains15_curRate = 0;
                this->granulator_02_grains15_active = false;
            } else {
                this->granulator_02_grains15_curLen = length;
                this->granulator_02_grains15_active = true;
                this->granulator_02_grains15_backwards = rate < 0;
                this->granulator_02_grains15_sampleOffset = 0;
                this->granulator_02_grains15_curStart = start;
    
                if ((bool)(this->granulator_02_grains15_backwards)) {
                    this->granulator_02_grains15_curStart += this->granulator_02_grains15_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains15_active)) {
            if (this->granulator_02_grains15_sampleOffset >= this->granulator_02_grains15_curLen) {
                bangDone = true;
                this->granulator_02_grains15_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains15_curStart;
    
                if ((bool)(this->granulator_02_grains15_backwards)) {
                    sampleIndex -= this->granulator_02_grains15_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains15_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains15_sampleOffset / this->granulator_02_grains15_curLen > 1 ? 1 : (this->granulator_02_grains15_sampleOffset / this->granulator_02_grains15_curLen < 0 ? 0 : this->granulator_02_grains15_sampleOffset / this->granulator_02_grains15_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains15_sampleOffset += this->granulator_02_grains15_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains16_inactive() {
        return !(bool)(this->granulator_02_grains16_active);
    }
    
    void granulator_02_grains16_reset() {
        this->granulator_02_grains16_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains16_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains16_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains16_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains16_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains16_curRate = 0;
                this->granulator_02_grains16_active = false;
            } else {
                this->granulator_02_grains16_curLen = length;
                this->granulator_02_grains16_active = true;
                this->granulator_02_grains16_backwards = rate < 0;
                this->granulator_02_grains16_sampleOffset = 0;
                this->granulator_02_grains16_curStart = start;
    
                if ((bool)(this->granulator_02_grains16_backwards)) {
                    this->granulator_02_grains16_curStart += this->granulator_02_grains16_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains16_active)) {
            if (this->granulator_02_grains16_sampleOffset >= this->granulator_02_grains16_curLen) {
                bangDone = true;
                this->granulator_02_grains16_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains16_curStart;
    
                if ((bool)(this->granulator_02_grains16_backwards)) {
                    sampleIndex -= this->granulator_02_grains16_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains16_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains16_sampleOffset / this->granulator_02_grains16_curLen > 1 ? 1 : (this->granulator_02_grains16_sampleOffset / this->granulator_02_grains16_curLen < 0 ? 0 : this->granulator_02_grains16_sampleOffset / this->granulator_02_grains16_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains16_sampleOffset += this->granulator_02_grains16_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains17_inactive() {
        return !(bool)(this->granulator_02_grains17_active);
    }
    
    void granulator_02_grains17_reset() {
        this->granulator_02_grains17_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains17_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains17_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains17_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains17_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains17_curRate = 0;
                this->granulator_02_grains17_active = false;
            } else {
                this->granulator_02_grains17_curLen = length;
                this->granulator_02_grains17_active = true;
                this->granulator_02_grains17_backwards = rate < 0;
                this->granulator_02_grains17_sampleOffset = 0;
                this->granulator_02_grains17_curStart = start;
    
                if ((bool)(this->granulator_02_grains17_backwards)) {
                    this->granulator_02_grains17_curStart += this->granulator_02_grains17_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains17_active)) {
            if (this->granulator_02_grains17_sampleOffset >= this->granulator_02_grains17_curLen) {
                bangDone = true;
                this->granulator_02_grains17_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains17_curStart;
    
                if ((bool)(this->granulator_02_grains17_backwards)) {
                    sampleIndex -= this->granulator_02_grains17_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains17_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains17_sampleOffset / this->granulator_02_grains17_curLen > 1 ? 1 : (this->granulator_02_grains17_sampleOffset / this->granulator_02_grains17_curLen < 0 ? 0 : this->granulator_02_grains17_sampleOffset / this->granulator_02_grains17_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains17_sampleOffset += this->granulator_02_grains17_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains18_inactive() {
        return !(bool)(this->granulator_02_grains18_active);
    }
    
    void granulator_02_grains18_reset() {
        this->granulator_02_grains18_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains18_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains18_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains18_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains18_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains18_curRate = 0;
                this->granulator_02_grains18_active = false;
            } else {
                this->granulator_02_grains18_curLen = length;
                this->granulator_02_grains18_active = true;
                this->granulator_02_grains18_backwards = rate < 0;
                this->granulator_02_grains18_sampleOffset = 0;
                this->granulator_02_grains18_curStart = start;
    
                if ((bool)(this->granulator_02_grains18_backwards)) {
                    this->granulator_02_grains18_curStart += this->granulator_02_grains18_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains18_active)) {
            if (this->granulator_02_grains18_sampleOffset >= this->granulator_02_grains18_curLen) {
                bangDone = true;
                this->granulator_02_grains18_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains18_curStart;
    
                if ((bool)(this->granulator_02_grains18_backwards)) {
                    sampleIndex -= this->granulator_02_grains18_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains18_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains18_sampleOffset / this->granulator_02_grains18_curLen > 1 ? 1 : (this->granulator_02_grains18_sampleOffset / this->granulator_02_grains18_curLen < 0 ? 0 : this->granulator_02_grains18_sampleOffset / this->granulator_02_grains18_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains18_sampleOffset += this->granulator_02_grains18_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains19_inactive() {
        return !(bool)(this->granulator_02_grains19_active);
    }
    
    void granulator_02_grains19_reset() {
        this->granulator_02_grains19_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains19_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains19_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains19_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains19_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains19_curRate = 0;
                this->granulator_02_grains19_active = false;
            } else {
                this->granulator_02_grains19_curLen = length;
                this->granulator_02_grains19_active = true;
                this->granulator_02_grains19_backwards = rate < 0;
                this->granulator_02_grains19_sampleOffset = 0;
                this->granulator_02_grains19_curStart = start;
    
                if ((bool)(this->granulator_02_grains19_backwards)) {
                    this->granulator_02_grains19_curStart += this->granulator_02_grains19_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains19_active)) {
            if (this->granulator_02_grains19_sampleOffset >= this->granulator_02_grains19_curLen) {
                bangDone = true;
                this->granulator_02_grains19_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains19_curStart;
    
                if ((bool)(this->granulator_02_grains19_backwards)) {
                    sampleIndex -= this->granulator_02_grains19_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains19_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains19_sampleOffset / this->granulator_02_grains19_curLen > 1 ? 1 : (this->granulator_02_grains19_sampleOffset / this->granulator_02_grains19_curLen < 0 ? 0 : this->granulator_02_grains19_sampleOffset / this->granulator_02_grains19_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains19_sampleOffset += this->granulator_02_grains19_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains20_inactive() {
        return !(bool)(this->granulator_02_grains20_active);
    }
    
    void granulator_02_grains20_reset() {
        this->granulator_02_grains20_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains20_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains20_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains20_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains20_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains20_curRate = 0;
                this->granulator_02_grains20_active = false;
            } else {
                this->granulator_02_grains20_curLen = length;
                this->granulator_02_grains20_active = true;
                this->granulator_02_grains20_backwards = rate < 0;
                this->granulator_02_grains20_sampleOffset = 0;
                this->granulator_02_grains20_curStart = start;
    
                if ((bool)(this->granulator_02_grains20_backwards)) {
                    this->granulator_02_grains20_curStart += this->granulator_02_grains20_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains20_active)) {
            if (this->granulator_02_grains20_sampleOffset >= this->granulator_02_grains20_curLen) {
                bangDone = true;
                this->granulator_02_grains20_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains20_curStart;
    
                if ((bool)(this->granulator_02_grains20_backwards)) {
                    sampleIndex -= this->granulator_02_grains20_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains20_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains20_sampleOffset / this->granulator_02_grains20_curLen > 1 ? 1 : (this->granulator_02_grains20_sampleOffset / this->granulator_02_grains20_curLen < 0 ? 0 : this->granulator_02_grains20_sampleOffset / this->granulator_02_grains20_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains20_sampleOffset += this->granulator_02_grains20_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains21_inactive() {
        return !(bool)(this->granulator_02_grains21_active);
    }
    
    void granulator_02_grains21_reset() {
        this->granulator_02_grains21_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains21_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains21_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains21_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains21_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains21_curRate = 0;
                this->granulator_02_grains21_active = false;
            } else {
                this->granulator_02_grains21_curLen = length;
                this->granulator_02_grains21_active = true;
                this->granulator_02_grains21_backwards = rate < 0;
                this->granulator_02_grains21_sampleOffset = 0;
                this->granulator_02_grains21_curStart = start;
    
                if ((bool)(this->granulator_02_grains21_backwards)) {
                    this->granulator_02_grains21_curStart += this->granulator_02_grains21_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains21_active)) {
            if (this->granulator_02_grains21_sampleOffset >= this->granulator_02_grains21_curLen) {
                bangDone = true;
                this->granulator_02_grains21_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains21_curStart;
    
                if ((bool)(this->granulator_02_grains21_backwards)) {
                    sampleIndex -= this->granulator_02_grains21_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains21_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains21_sampleOffset / this->granulator_02_grains21_curLen > 1 ? 1 : (this->granulator_02_grains21_sampleOffset / this->granulator_02_grains21_curLen < 0 ? 0 : this->granulator_02_grains21_sampleOffset / this->granulator_02_grains21_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains21_sampleOffset += this->granulator_02_grains21_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains22_inactive() {
        return !(bool)(this->granulator_02_grains22_active);
    }
    
    void granulator_02_grains22_reset() {
        this->granulator_02_grains22_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains22_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains22_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains22_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains22_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains22_curRate = 0;
                this->granulator_02_grains22_active = false;
            } else {
                this->granulator_02_grains22_curLen = length;
                this->granulator_02_grains22_active = true;
                this->granulator_02_grains22_backwards = rate < 0;
                this->granulator_02_grains22_sampleOffset = 0;
                this->granulator_02_grains22_curStart = start;
    
                if ((bool)(this->granulator_02_grains22_backwards)) {
                    this->granulator_02_grains22_curStart += this->granulator_02_grains22_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains22_active)) {
            if (this->granulator_02_grains22_sampleOffset >= this->granulator_02_grains22_curLen) {
                bangDone = true;
                this->granulator_02_grains22_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains22_curStart;
    
                if ((bool)(this->granulator_02_grains22_backwards)) {
                    sampleIndex -= this->granulator_02_grains22_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains22_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains22_sampleOffset / this->granulator_02_grains22_curLen > 1 ? 1 : (this->granulator_02_grains22_sampleOffset / this->granulator_02_grains22_curLen < 0 ? 0 : this->granulator_02_grains22_sampleOffset / this->granulator_02_grains22_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains22_sampleOffset += this->granulator_02_grains22_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains23_inactive() {
        return !(bool)(this->granulator_02_grains23_active);
    }
    
    void granulator_02_grains23_reset() {
        this->granulator_02_grains23_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains23_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains23_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains23_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains23_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains23_curRate = 0;
                this->granulator_02_grains23_active = false;
            } else {
                this->granulator_02_grains23_curLen = length;
                this->granulator_02_grains23_active = true;
                this->granulator_02_grains23_backwards = rate < 0;
                this->granulator_02_grains23_sampleOffset = 0;
                this->granulator_02_grains23_curStart = start;
    
                if ((bool)(this->granulator_02_grains23_backwards)) {
                    this->granulator_02_grains23_curStart += this->granulator_02_grains23_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains23_active)) {
            if (this->granulator_02_grains23_sampleOffset >= this->granulator_02_grains23_curLen) {
                bangDone = true;
                this->granulator_02_grains23_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains23_curStart;
    
                if ((bool)(this->granulator_02_grains23_backwards)) {
                    sampleIndex -= this->granulator_02_grains23_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains23_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains23_sampleOffset / this->granulator_02_grains23_curLen > 1 ? 1 : (this->granulator_02_grains23_sampleOffset / this->granulator_02_grains23_curLen < 0 ? 0 : this->granulator_02_grains23_sampleOffset / this->granulator_02_grains23_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains23_sampleOffset += this->granulator_02_grains23_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains24_inactive() {
        return !(bool)(this->granulator_02_grains24_active);
    }
    
    void granulator_02_grains24_reset() {
        this->granulator_02_grains24_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains24_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains24_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains24_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains24_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains24_curRate = 0;
                this->granulator_02_grains24_active = false;
            } else {
                this->granulator_02_grains24_curLen = length;
                this->granulator_02_grains24_active = true;
                this->granulator_02_grains24_backwards = rate < 0;
                this->granulator_02_grains24_sampleOffset = 0;
                this->granulator_02_grains24_curStart = start;
    
                if ((bool)(this->granulator_02_grains24_backwards)) {
                    this->granulator_02_grains24_curStart += this->granulator_02_grains24_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains24_active)) {
            if (this->granulator_02_grains24_sampleOffset >= this->granulator_02_grains24_curLen) {
                bangDone = true;
                this->granulator_02_grains24_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains24_curStart;
    
                if ((bool)(this->granulator_02_grains24_backwards)) {
                    sampleIndex -= this->granulator_02_grains24_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains24_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains24_sampleOffset / this->granulator_02_grains24_curLen > 1 ? 1 : (this->granulator_02_grains24_sampleOffset / this->granulator_02_grains24_curLen < 0 ? 0 : this->granulator_02_grains24_sampleOffset / this->granulator_02_grains24_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains24_sampleOffset += this->granulator_02_grains24_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains25_inactive() {
        return !(bool)(this->granulator_02_grains25_active);
    }
    
    void granulator_02_grains25_reset() {
        this->granulator_02_grains25_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains25_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains25_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains25_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains25_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains25_curRate = 0;
                this->granulator_02_grains25_active = false;
            } else {
                this->granulator_02_grains25_curLen = length;
                this->granulator_02_grains25_active = true;
                this->granulator_02_grains25_backwards = rate < 0;
                this->granulator_02_grains25_sampleOffset = 0;
                this->granulator_02_grains25_curStart = start;
    
                if ((bool)(this->granulator_02_grains25_backwards)) {
                    this->granulator_02_grains25_curStart += this->granulator_02_grains25_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains25_active)) {
            if (this->granulator_02_grains25_sampleOffset >= this->granulator_02_grains25_curLen) {
                bangDone = true;
                this->granulator_02_grains25_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains25_curStart;
    
                if ((bool)(this->granulator_02_grains25_backwards)) {
                    sampleIndex -= this->granulator_02_grains25_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains25_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains25_sampleOffset / this->granulator_02_grains25_curLen > 1 ? 1 : (this->granulator_02_grains25_sampleOffset / this->granulator_02_grains25_curLen < 0 ? 0 : this->granulator_02_grains25_sampleOffset / this->granulator_02_grains25_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains25_sampleOffset += this->granulator_02_grains25_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains26_inactive() {
        return !(bool)(this->granulator_02_grains26_active);
    }
    
    void granulator_02_grains26_reset() {
        this->granulator_02_grains26_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains26_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains26_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains26_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains26_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains26_curRate = 0;
                this->granulator_02_grains26_active = false;
            } else {
                this->granulator_02_grains26_curLen = length;
                this->granulator_02_grains26_active = true;
                this->granulator_02_grains26_backwards = rate < 0;
                this->granulator_02_grains26_sampleOffset = 0;
                this->granulator_02_grains26_curStart = start;
    
                if ((bool)(this->granulator_02_grains26_backwards)) {
                    this->granulator_02_grains26_curStart += this->granulator_02_grains26_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains26_active)) {
            if (this->granulator_02_grains26_sampleOffset >= this->granulator_02_grains26_curLen) {
                bangDone = true;
                this->granulator_02_grains26_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains26_curStart;
    
                if ((bool)(this->granulator_02_grains26_backwards)) {
                    sampleIndex -= this->granulator_02_grains26_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains26_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains26_sampleOffset / this->granulator_02_grains26_curLen > 1 ? 1 : (this->granulator_02_grains26_sampleOffset / this->granulator_02_grains26_curLen < 0 ? 0 : this->granulator_02_grains26_sampleOffset / this->granulator_02_grains26_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains26_sampleOffset += this->granulator_02_grains26_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains27_inactive() {
        return !(bool)(this->granulator_02_grains27_active);
    }
    
    void granulator_02_grains27_reset() {
        this->granulator_02_grains27_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains27_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains27_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains27_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains27_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains27_curRate = 0;
                this->granulator_02_grains27_active = false;
            } else {
                this->granulator_02_grains27_curLen = length;
                this->granulator_02_grains27_active = true;
                this->granulator_02_grains27_backwards = rate < 0;
                this->granulator_02_grains27_sampleOffset = 0;
                this->granulator_02_grains27_curStart = start;
    
                if ((bool)(this->granulator_02_grains27_backwards)) {
                    this->granulator_02_grains27_curStart += this->granulator_02_grains27_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains27_active)) {
            if (this->granulator_02_grains27_sampleOffset >= this->granulator_02_grains27_curLen) {
                bangDone = true;
                this->granulator_02_grains27_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains27_curStart;
    
                if ((bool)(this->granulator_02_grains27_backwards)) {
                    sampleIndex -= this->granulator_02_grains27_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains27_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains27_sampleOffset / this->granulator_02_grains27_curLen > 1 ? 1 : (this->granulator_02_grains27_sampleOffset / this->granulator_02_grains27_curLen < 0 ? 0 : this->granulator_02_grains27_sampleOffset / this->granulator_02_grains27_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains27_sampleOffset += this->granulator_02_grains27_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains28_inactive() {
        return !(bool)(this->granulator_02_grains28_active);
    }
    
    void granulator_02_grains28_reset() {
        this->granulator_02_grains28_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains28_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains28_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains28_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains28_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains28_curRate = 0;
                this->granulator_02_grains28_active = false;
            } else {
                this->granulator_02_grains28_curLen = length;
                this->granulator_02_grains28_active = true;
                this->granulator_02_grains28_backwards = rate < 0;
                this->granulator_02_grains28_sampleOffset = 0;
                this->granulator_02_grains28_curStart = start;
    
                if ((bool)(this->granulator_02_grains28_backwards)) {
                    this->granulator_02_grains28_curStart += this->granulator_02_grains28_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains28_active)) {
            if (this->granulator_02_grains28_sampleOffset >= this->granulator_02_grains28_curLen) {
                bangDone = true;
                this->granulator_02_grains28_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains28_curStart;
    
                if ((bool)(this->granulator_02_grains28_backwards)) {
                    sampleIndex -= this->granulator_02_grains28_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains28_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains28_sampleOffset / this->granulator_02_grains28_curLen > 1 ? 1 : (this->granulator_02_grains28_sampleOffset / this->granulator_02_grains28_curLen < 0 ? 0 : this->granulator_02_grains28_sampleOffset / this->granulator_02_grains28_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains28_sampleOffset += this->granulator_02_grains28_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains29_inactive() {
        return !(bool)(this->granulator_02_grains29_active);
    }
    
    void granulator_02_grains29_reset() {
        this->granulator_02_grains29_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains29_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains29_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains29_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains29_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains29_curRate = 0;
                this->granulator_02_grains29_active = false;
            } else {
                this->granulator_02_grains29_curLen = length;
                this->granulator_02_grains29_active = true;
                this->granulator_02_grains29_backwards = rate < 0;
                this->granulator_02_grains29_sampleOffset = 0;
                this->granulator_02_grains29_curStart = start;
    
                if ((bool)(this->granulator_02_grains29_backwards)) {
                    this->granulator_02_grains29_curStart += this->granulator_02_grains29_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains29_active)) {
            if (this->granulator_02_grains29_sampleOffset >= this->granulator_02_grains29_curLen) {
                bangDone = true;
                this->granulator_02_grains29_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains29_curStart;
    
                if ((bool)(this->granulator_02_grains29_backwards)) {
                    sampleIndex -= this->granulator_02_grains29_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains29_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains29_sampleOffset / this->granulator_02_grains29_curLen > 1 ? 1 : (this->granulator_02_grains29_sampleOffset / this->granulator_02_grains29_curLen < 0 ? 0 : this->granulator_02_grains29_sampleOffset / this->granulator_02_grains29_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains29_sampleOffset += this->granulator_02_grains29_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains30_inactive() {
        return !(bool)(this->granulator_02_grains30_active);
    }
    
    void granulator_02_grains30_reset() {
        this->granulator_02_grains30_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains30_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains30_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains30_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains30_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains30_curRate = 0;
                this->granulator_02_grains30_active = false;
            } else {
                this->granulator_02_grains30_curLen = length;
                this->granulator_02_grains30_active = true;
                this->granulator_02_grains30_backwards = rate < 0;
                this->granulator_02_grains30_sampleOffset = 0;
                this->granulator_02_grains30_curStart = start;
    
                if ((bool)(this->granulator_02_grains30_backwards)) {
                    this->granulator_02_grains30_curStart += this->granulator_02_grains30_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains30_active)) {
            if (this->granulator_02_grains30_sampleOffset >= this->granulator_02_grains30_curLen) {
                bangDone = true;
                this->granulator_02_grains30_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains30_curStart;
    
                if ((bool)(this->granulator_02_grains30_backwards)) {
                    sampleIndex -= this->granulator_02_grains30_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains30_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains30_sampleOffset / this->granulator_02_grains30_curLen > 1 ? 1 : (this->granulator_02_grains30_sampleOffset / this->granulator_02_grains30_curLen < 0 ? 0 : this->granulator_02_grains30_sampleOffset / this->granulator_02_grains30_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains30_sampleOffset += this->granulator_02_grains30_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains31_inactive() {
        return !(bool)(this->granulator_02_grains31_active);
    }
    
    void granulator_02_grains31_reset() {
        this->granulator_02_grains31_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains31_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains31_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains31_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains31_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains31_curRate = 0;
                this->granulator_02_grains31_active = false;
            } else {
                this->granulator_02_grains31_curLen = length;
                this->granulator_02_grains31_active = true;
                this->granulator_02_grains31_backwards = rate < 0;
                this->granulator_02_grains31_sampleOffset = 0;
                this->granulator_02_grains31_curStart = start;
    
                if ((bool)(this->granulator_02_grains31_backwards)) {
                    this->granulator_02_grains31_curStart += this->granulator_02_grains31_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains31_active)) {
            if (this->granulator_02_grains31_sampleOffset >= this->granulator_02_grains31_curLen) {
                bangDone = true;
                this->granulator_02_grains31_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains31_curStart;
    
                if ((bool)(this->granulator_02_grains31_backwards)) {
                    sampleIndex -= this->granulator_02_grains31_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains31_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains31_sampleOffset / this->granulator_02_grains31_curLen > 1 ? 1 : (this->granulator_02_grains31_sampleOffset / this->granulator_02_grains31_curLen < 0 ? 0 : this->granulator_02_grains31_sampleOffset / this->granulator_02_grains31_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains31_sampleOffset += this->granulator_02_grains31_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains32_inactive() {
        return !(bool)(this->granulator_02_grains32_active);
    }
    
    void granulator_02_grains32_reset() {
        this->granulator_02_grains32_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains32_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_02_grains32_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_02_grains32_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_02_grains32_active)) {
                    bangDone = true;
                }
    
                this->granulator_02_grains32_curRate = 0;
                this->granulator_02_grains32_active = false;
            } else {
                this->granulator_02_grains32_curLen = length;
                this->granulator_02_grains32_active = true;
                this->granulator_02_grains32_backwards = rate < 0;
                this->granulator_02_grains32_sampleOffset = 0;
                this->granulator_02_grains32_curStart = start;
    
                if ((bool)(this->granulator_02_grains32_backwards)) {
                    this->granulator_02_grains32_curStart += this->granulator_02_grains32_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_02_grains32_active)) {
            if (this->granulator_02_grains32_sampleOffset >= this->granulator_02_grains32_curLen) {
                bangDone = true;
                this->granulator_02_grains32_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_02_grains32_curStart;
    
                if ((bool)(this->granulator_02_grains32_backwards)) {
                    sampleIndex -= this->granulator_02_grains32_sampleOffset;
                } else {
                    sampleIndex += this->granulator_02_grains32_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_02_grains32_sampleOffset / this->granulator_02_grains32_curLen > 1 ? 1 : (this->granulator_02_grains32_sampleOffset / this->granulator_02_grains32_curLen < 0 ? 0 : this->granulator_02_grains32_sampleOffset / this->granulator_02_grains32_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_02_grains32_sampleOffset += this->granulator_02_grains32_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_02_grains_inactive(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_02_grains1_inactive();
        case 1:
            return this->granulator_02_grains2_inactive();
        case 2:
            return this->granulator_02_grains3_inactive();
        case 3:
            return this->granulator_02_grains4_inactive();
        case 4:
            return this->granulator_02_grains5_inactive();
        case 5:
            return this->granulator_02_grains6_inactive();
        case 6:
            return this->granulator_02_grains7_inactive();
        case 7:
            return this->granulator_02_grains8_inactive();
        case 8:
            return this->granulator_02_grains9_inactive();
        case 9:
            return this->granulator_02_grains10_inactive();
        case 10:
            return this->granulator_02_grains11_inactive();
        case 11:
            return this->granulator_02_grains12_inactive();
        case 12:
            return this->granulator_02_grains13_inactive();
        case 13:
            return this->granulator_02_grains14_inactive();
        case 14:
            return this->granulator_02_grains15_inactive();
        case 15:
            return this->granulator_02_grains16_inactive();
        case 16:
            return this->granulator_02_grains17_inactive();
        case 17:
            return this->granulator_02_grains18_inactive();
        case 18:
            return this->granulator_02_grains19_inactive();
        case 19:
            return this->granulator_02_grains20_inactive();
        case 20:
            return this->granulator_02_grains21_inactive();
        case 21:
            return this->granulator_02_grains22_inactive();
        case 22:
            return this->granulator_02_grains23_inactive();
        case 23:
            return this->granulator_02_grains24_inactive();
        case 24:
            return this->granulator_02_grains25_inactive();
        case 25:
            return this->granulator_02_grains26_inactive();
        case 26:
            return this->granulator_02_grains27_inactive();
        case 27:
            return this->granulator_02_grains28_inactive();
        case 28:
            return this->granulator_02_grains29_inactive();
        case 29:
            return this->granulator_02_grains30_inactive();
        case 30:
            return this->granulator_02_grains31_inactive();
        default:
            return this->granulator_02_grains32_inactive();
        }
    
        return 0;
    }
    
    void granulator_02_grains_reset(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_02_grains1_reset();
        case 1:
            return this->granulator_02_grains2_reset();
        case 2:
            return this->granulator_02_grains3_reset();
        case 3:
            return this->granulator_02_grains4_reset();
        case 4:
            return this->granulator_02_grains5_reset();
        case 5:
            return this->granulator_02_grains6_reset();
        case 6:
            return this->granulator_02_grains7_reset();
        case 7:
            return this->granulator_02_grains8_reset();
        case 8:
            return this->granulator_02_grains9_reset();
        case 9:
            return this->granulator_02_grains10_reset();
        case 10:
            return this->granulator_02_grains11_reset();
        case 11:
            return this->granulator_02_grains12_reset();
        case 12:
            return this->granulator_02_grains13_reset();
        case 13:
            return this->granulator_02_grains14_reset();
        case 14:
            return this->granulator_02_grains15_reset();
        case 15:
            return this->granulator_02_grains16_reset();
        case 16:
            return this->granulator_02_grains17_reset();
        case 17:
            return this->granulator_02_grains18_reset();
        case 18:
            return this->granulator_02_grains19_reset();
        case 19:
            return this->granulator_02_grains20_reset();
        case 20:
            return this->granulator_02_grains21_reset();
        case 21:
            return this->granulator_02_grains22_reset();
        case 22:
            return this->granulator_02_grains23_reset();
        case 23:
            return this->granulator_02_grains24_reset();
        case 24:
            return this->granulator_02_grains25_reset();
        case 25:
            return this->granulator_02_grains26_reset();
        case 26:
            return this->granulator_02_grains27_reset();
        case 27:
            return this->granulator_02_grains28_reset();
        case 28:
            return this->granulator_02_grains29_reset();
        case 29:
            return this->granulator_02_grains30_reset();
        case 30:
            return this->granulator_02_grains31_reset();
        default:
            return this->granulator_02_grains32_reset();
        }
    }
    
    template <typename T> inline array<number, 3> granulator_02_grains_next(
        Index i,
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        switch ((int)i) {
        case 0:
            return this->granulator_02_grains1_next(buffer, trigger, start, length, rate, channel);
        case 1:
            return this->granulator_02_grains2_next(buffer, trigger, start, length, rate, channel);
        case 2:
            return this->granulator_02_grains3_next(buffer, trigger, start, length, rate, channel);
        case 3:
            return this->granulator_02_grains4_next(buffer, trigger, start, length, rate, channel);
        case 4:
            return this->granulator_02_grains5_next(buffer, trigger, start, length, rate, channel);
        case 5:
            return this->granulator_02_grains6_next(buffer, trigger, start, length, rate, channel);
        case 6:
            return this->granulator_02_grains7_next(buffer, trigger, start, length, rate, channel);
        case 7:
            return this->granulator_02_grains8_next(buffer, trigger, start, length, rate, channel);
        case 8:
            return this->granulator_02_grains9_next(buffer, trigger, start, length, rate, channel);
        case 9:
            return this->granulator_02_grains10_next(buffer, trigger, start, length, rate, channel);
        case 10:
            return this->granulator_02_grains11_next(buffer, trigger, start, length, rate, channel);
        case 11:
            return this->granulator_02_grains12_next(buffer, trigger, start, length, rate, channel);
        case 12:
            return this->granulator_02_grains13_next(buffer, trigger, start, length, rate, channel);
        case 13:
            return this->granulator_02_grains14_next(buffer, trigger, start, length, rate, channel);
        case 14:
            return this->granulator_02_grains15_next(buffer, trigger, start, length, rate, channel);
        case 15:
            return this->granulator_02_grains16_next(buffer, trigger, start, length, rate, channel);
        case 16:
            return this->granulator_02_grains17_next(buffer, trigger, start, length, rate, channel);
        case 17:
            return this->granulator_02_grains18_next(buffer, trigger, start, length, rate, channel);
        case 18:
            return this->granulator_02_grains19_next(buffer, trigger, start, length, rate, channel);
        case 19:
            return this->granulator_02_grains20_next(buffer, trigger, start, length, rate, channel);
        case 20:
            return this->granulator_02_grains21_next(buffer, trigger, start, length, rate, channel);
        case 21:
            return this->granulator_02_grains22_next(buffer, trigger, start, length, rate, channel);
        case 22:
            return this->granulator_02_grains23_next(buffer, trigger, start, length, rate, channel);
        case 23:
            return this->granulator_02_grains24_next(buffer, trigger, start, length, rate, channel);
        case 24:
            return this->granulator_02_grains25_next(buffer, trigger, start, length, rate, channel);
        case 25:
            return this->granulator_02_grains26_next(buffer, trigger, start, length, rate, channel);
        case 26:
            return this->granulator_02_grains27_next(buffer, trigger, start, length, rate, channel);
        case 27:
            return this->granulator_02_grains28_next(buffer, trigger, start, length, rate, channel);
        case 28:
            return this->granulator_02_grains29_next(buffer, trigger, start, length, rate, channel);
        case 29:
            return this->granulator_02_grains30_next(buffer, trigger, start, length, rate, channel);
        case 30:
            return this->granulator_02_grains31_next(buffer, trigger, start, length, rate, channel);
        default:
            return this->granulator_02_grains32_next(buffer, trigger, start, length, rate, channel);
        }
    
        return 0;
    }
    
    void granulator_02_init() {
        this->granulator_02_edgedetect_init();
    
        for (Index i = 0; i < 32; i++) {
            for (Index j = 0; j < 2; j++) {
                this->granulator_02_grainPan[(Index)i][(Index)j] = 1;
            }
        }
    }
    
    void granulator_02_bufferUpdated() {
        number bufSr = this->granulator_02_buffer->getSampleRate();
        number bufSize = this->granulator_02_buffer->getSize();
    
        for (Index i = 0; i < 32; i++) {
            this->granulator_02_grains_reset(i);
        }
    
        {
            this->granulator_02_startMul = bufSize;
        }
    
        {
            this->granulator_02_lengthMul = bufSize;
        }
    }
    
    void granulator_02_onSampleRateChanged(number samplerate) {
        RNBO_UNUSED(samplerate);
        this->granulator_02_bufferUpdated();
    }
    
    void param_07_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_07_value;
    }
    
    void param_07_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_07_value_set(preset["value"]);
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
    
    void param_11_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_11_value;
    }
    
    void param_11_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_11_value_set(preset["value"]);
    }
    
    void param_12_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_12_value;
    }
    
    void param_12_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_12_value_set(preset["value"]);
    }
    
    void samplerate_02_onSampleRateChanged(number samplerate) {
        this->samplerate_02_samplerate_set(samplerate);
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
        dspexpr_09_in1 = 0;
        dspexpr_09_in2 = 0;
        granulator_02_start = 0;
        granulator_02_length = 2205;
        granulator_02_rate = 1;
        granulator_02_bchan = 0;
        granulator_02_limit = 32;
        granulator_02_panpos = 0.5;
        dspexpr_10_in1 = 0;
        dspexpr_10_in2 = 0;
        param_07_value = 0;
        dspexpr_11_in1 = 0;
        dspexpr_11_in2 = 0;
        dspexpr_12_in1 = 0;
        dspexpr_12_in2 = 0.5;
        dspexpr_13_in1 = 0;
        dspexpr_13_in2 = 0.5;
        dspexpr_14_in1 = 0;
        dspexpr_14_in2 = 0;
        param_08_value = 1;
        dspexpr_15_in1 = 0;
        dspexpr_15_in2 = 0;
        param_09_value = 1;
        dspexpr_16_in1 = 0;
        dspexpr_16_in2 = 0;
        param_10_value = 1;
        param_11_value = 0;
        expr_03_in1 = 0;
        expr_03_in2 = 0;
        expr_03_out1 = 0;
        param_12_value = 1;
        expr_04_in1 = 0;
        expr_04_in2 = 1000;
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
        signals[4] = nullptr;
        signals[5] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        granulator_02_startMul = 1;
        granulator_02_lengthMul = 1;
        granulator_02_limitLast = 0;
        granulator_02_edgedetect_active = false;
        granulator_02_grains1_sampleOffset = 0;
        granulator_02_grains1_curRate = 1;
        granulator_02_grains1_curStart = 0;
        granulator_02_grains1_curLen = 1;
        granulator_02_grains1_active = false;
        granulator_02_grains1_backwards = false;
        granulator_02_grains2_sampleOffset = 0;
        granulator_02_grains2_curRate = 1;
        granulator_02_grains2_curStart = 0;
        granulator_02_grains2_curLen = 1;
        granulator_02_grains2_active = false;
        granulator_02_grains2_backwards = false;
        granulator_02_grains3_sampleOffset = 0;
        granulator_02_grains3_curRate = 1;
        granulator_02_grains3_curStart = 0;
        granulator_02_grains3_curLen = 1;
        granulator_02_grains3_active = false;
        granulator_02_grains3_backwards = false;
        granulator_02_grains4_sampleOffset = 0;
        granulator_02_grains4_curRate = 1;
        granulator_02_grains4_curStart = 0;
        granulator_02_grains4_curLen = 1;
        granulator_02_grains4_active = false;
        granulator_02_grains4_backwards = false;
        granulator_02_grains5_sampleOffset = 0;
        granulator_02_grains5_curRate = 1;
        granulator_02_grains5_curStart = 0;
        granulator_02_grains5_curLen = 1;
        granulator_02_grains5_active = false;
        granulator_02_grains5_backwards = false;
        granulator_02_grains6_sampleOffset = 0;
        granulator_02_grains6_curRate = 1;
        granulator_02_grains6_curStart = 0;
        granulator_02_grains6_curLen = 1;
        granulator_02_grains6_active = false;
        granulator_02_grains6_backwards = false;
        granulator_02_grains7_sampleOffset = 0;
        granulator_02_grains7_curRate = 1;
        granulator_02_grains7_curStart = 0;
        granulator_02_grains7_curLen = 1;
        granulator_02_grains7_active = false;
        granulator_02_grains7_backwards = false;
        granulator_02_grains8_sampleOffset = 0;
        granulator_02_grains8_curRate = 1;
        granulator_02_grains8_curStart = 0;
        granulator_02_grains8_curLen = 1;
        granulator_02_grains8_active = false;
        granulator_02_grains8_backwards = false;
        granulator_02_grains9_sampleOffset = 0;
        granulator_02_grains9_curRate = 1;
        granulator_02_grains9_curStart = 0;
        granulator_02_grains9_curLen = 1;
        granulator_02_grains9_active = false;
        granulator_02_grains9_backwards = false;
        granulator_02_grains10_sampleOffset = 0;
        granulator_02_grains10_curRate = 1;
        granulator_02_grains10_curStart = 0;
        granulator_02_grains10_curLen = 1;
        granulator_02_grains10_active = false;
        granulator_02_grains10_backwards = false;
        granulator_02_grains11_sampleOffset = 0;
        granulator_02_grains11_curRate = 1;
        granulator_02_grains11_curStart = 0;
        granulator_02_grains11_curLen = 1;
        granulator_02_grains11_active = false;
        granulator_02_grains11_backwards = false;
        granulator_02_grains12_sampleOffset = 0;
        granulator_02_grains12_curRate = 1;
        granulator_02_grains12_curStart = 0;
        granulator_02_grains12_curLen = 1;
        granulator_02_grains12_active = false;
        granulator_02_grains12_backwards = false;
        granulator_02_grains13_sampleOffset = 0;
        granulator_02_grains13_curRate = 1;
        granulator_02_grains13_curStart = 0;
        granulator_02_grains13_curLen = 1;
        granulator_02_grains13_active = false;
        granulator_02_grains13_backwards = false;
        granulator_02_grains14_sampleOffset = 0;
        granulator_02_grains14_curRate = 1;
        granulator_02_grains14_curStart = 0;
        granulator_02_grains14_curLen = 1;
        granulator_02_grains14_active = false;
        granulator_02_grains14_backwards = false;
        granulator_02_grains15_sampleOffset = 0;
        granulator_02_grains15_curRate = 1;
        granulator_02_grains15_curStart = 0;
        granulator_02_grains15_curLen = 1;
        granulator_02_grains15_active = false;
        granulator_02_grains15_backwards = false;
        granulator_02_grains16_sampleOffset = 0;
        granulator_02_grains16_curRate = 1;
        granulator_02_grains16_curStart = 0;
        granulator_02_grains16_curLen = 1;
        granulator_02_grains16_active = false;
        granulator_02_grains16_backwards = false;
        granulator_02_grains17_sampleOffset = 0;
        granulator_02_grains17_curRate = 1;
        granulator_02_grains17_curStart = 0;
        granulator_02_grains17_curLen = 1;
        granulator_02_grains17_active = false;
        granulator_02_grains17_backwards = false;
        granulator_02_grains18_sampleOffset = 0;
        granulator_02_grains18_curRate = 1;
        granulator_02_grains18_curStart = 0;
        granulator_02_grains18_curLen = 1;
        granulator_02_grains18_active = false;
        granulator_02_grains18_backwards = false;
        granulator_02_grains19_sampleOffset = 0;
        granulator_02_grains19_curRate = 1;
        granulator_02_grains19_curStart = 0;
        granulator_02_grains19_curLen = 1;
        granulator_02_grains19_active = false;
        granulator_02_grains19_backwards = false;
        granulator_02_grains20_sampleOffset = 0;
        granulator_02_grains20_curRate = 1;
        granulator_02_grains20_curStart = 0;
        granulator_02_grains20_curLen = 1;
        granulator_02_grains20_active = false;
        granulator_02_grains20_backwards = false;
        granulator_02_grains21_sampleOffset = 0;
        granulator_02_grains21_curRate = 1;
        granulator_02_grains21_curStart = 0;
        granulator_02_grains21_curLen = 1;
        granulator_02_grains21_active = false;
        granulator_02_grains21_backwards = false;
        granulator_02_grains22_sampleOffset = 0;
        granulator_02_grains22_curRate = 1;
        granulator_02_grains22_curStart = 0;
        granulator_02_grains22_curLen = 1;
        granulator_02_grains22_active = false;
        granulator_02_grains22_backwards = false;
        granulator_02_grains23_sampleOffset = 0;
        granulator_02_grains23_curRate = 1;
        granulator_02_grains23_curStart = 0;
        granulator_02_grains23_curLen = 1;
        granulator_02_grains23_active = false;
        granulator_02_grains23_backwards = false;
        granulator_02_grains24_sampleOffset = 0;
        granulator_02_grains24_curRate = 1;
        granulator_02_grains24_curStart = 0;
        granulator_02_grains24_curLen = 1;
        granulator_02_grains24_active = false;
        granulator_02_grains24_backwards = false;
        granulator_02_grains25_sampleOffset = 0;
        granulator_02_grains25_curRate = 1;
        granulator_02_grains25_curStart = 0;
        granulator_02_grains25_curLen = 1;
        granulator_02_grains25_active = false;
        granulator_02_grains25_backwards = false;
        granulator_02_grains26_sampleOffset = 0;
        granulator_02_grains26_curRate = 1;
        granulator_02_grains26_curStart = 0;
        granulator_02_grains26_curLen = 1;
        granulator_02_grains26_active = false;
        granulator_02_grains26_backwards = false;
        granulator_02_grains27_sampleOffset = 0;
        granulator_02_grains27_curRate = 1;
        granulator_02_grains27_curStart = 0;
        granulator_02_grains27_curLen = 1;
        granulator_02_grains27_active = false;
        granulator_02_grains27_backwards = false;
        granulator_02_grains28_sampleOffset = 0;
        granulator_02_grains28_curRate = 1;
        granulator_02_grains28_curStart = 0;
        granulator_02_grains28_curLen = 1;
        granulator_02_grains28_active = false;
        granulator_02_grains28_backwards = false;
        granulator_02_grains29_sampleOffset = 0;
        granulator_02_grains29_curRate = 1;
        granulator_02_grains29_curStart = 0;
        granulator_02_grains29_curLen = 1;
        granulator_02_grains29_active = false;
        granulator_02_grains29_backwards = false;
        granulator_02_grains30_sampleOffset = 0;
        granulator_02_grains30_curRate = 1;
        granulator_02_grains30_curStart = 0;
        granulator_02_grains30_curLen = 1;
        granulator_02_grains30_active = false;
        granulator_02_grains30_backwards = false;
        granulator_02_grains31_sampleOffset = 0;
        granulator_02_grains31_curRate = 1;
        granulator_02_grains31_curStart = 0;
        granulator_02_grains31_curLen = 1;
        granulator_02_grains31_active = false;
        granulator_02_grains31_backwards = false;
        granulator_02_grains32_sampleOffset = 0;
        granulator_02_grains32_curRate = 1;
        granulator_02_grains32_curStart = 0;
        granulator_02_grains32_curLen = 1;
        granulator_02_grains32_active = false;
        granulator_02_grains32_backwards = false;
        param_07_lastValue = 0;
        param_08_lastValue = 0;
        param_09_lastValue = 0;
        param_10_lastValue = 0;
        param_11_lastValue = 0;
        param_12_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_09_in1;
        number dspexpr_09_in2;
        number granulator_02_start;
        number granulator_02_length;
        number granulator_02_rate;
        number granulator_02_bchan;
        number granulator_02_limit;
        number granulator_02_panpos;
        number dspexpr_10_in1;
        number dspexpr_10_in2;
        number param_07_value;
        number dspexpr_11_in1;
        number dspexpr_11_in2;
        number dspexpr_12_in1;
        number dspexpr_12_in2;
        number dspexpr_13_in1;
        number dspexpr_13_in2;
        number dspexpr_14_in1;
        number dspexpr_14_in2;
        number param_08_value;
        number dspexpr_15_in1;
        number dspexpr_15_in2;
        number param_09_value;
        number dspexpr_16_in1;
        number dspexpr_16_in2;
        number param_10_value;
        number param_11_value;
        number expr_03_in1;
        number expr_03_in2;
        number expr_03_out1;
        number param_12_value;
        number expr_04_in1;
        number expr_04_in2;
        number expr_04_out1;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[6];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef granulator_02_buffer;
        number granulator_02_grainPan[32][2] = { };
        number granulator_02_startMul;
        number granulator_02_lengthMul;
        int granulator_02_limitLast;
        bool granulator_02_edgedetect_active;
        number granulator_02_grains1_sampleOffset;
        number granulator_02_grains1_curRate;
        number granulator_02_grains1_curStart;
        number granulator_02_grains1_curLen;
        bool granulator_02_grains1_active;
        bool granulator_02_grains1_backwards;
        number granulator_02_grains2_sampleOffset;
        number granulator_02_grains2_curRate;
        number granulator_02_grains2_curStart;
        number granulator_02_grains2_curLen;
        bool granulator_02_grains2_active;
        bool granulator_02_grains2_backwards;
        number granulator_02_grains3_sampleOffset;
        number granulator_02_grains3_curRate;
        number granulator_02_grains3_curStart;
        number granulator_02_grains3_curLen;
        bool granulator_02_grains3_active;
        bool granulator_02_grains3_backwards;
        number granulator_02_grains4_sampleOffset;
        number granulator_02_grains4_curRate;
        number granulator_02_grains4_curStart;
        number granulator_02_grains4_curLen;
        bool granulator_02_grains4_active;
        bool granulator_02_grains4_backwards;
        number granulator_02_grains5_sampleOffset;
        number granulator_02_grains5_curRate;
        number granulator_02_grains5_curStart;
        number granulator_02_grains5_curLen;
        bool granulator_02_grains5_active;
        bool granulator_02_grains5_backwards;
        number granulator_02_grains6_sampleOffset;
        number granulator_02_grains6_curRate;
        number granulator_02_grains6_curStart;
        number granulator_02_grains6_curLen;
        bool granulator_02_grains6_active;
        bool granulator_02_grains6_backwards;
        number granulator_02_grains7_sampleOffset;
        number granulator_02_grains7_curRate;
        number granulator_02_grains7_curStart;
        number granulator_02_grains7_curLen;
        bool granulator_02_grains7_active;
        bool granulator_02_grains7_backwards;
        number granulator_02_grains8_sampleOffset;
        number granulator_02_grains8_curRate;
        number granulator_02_grains8_curStart;
        number granulator_02_grains8_curLen;
        bool granulator_02_grains8_active;
        bool granulator_02_grains8_backwards;
        number granulator_02_grains9_sampleOffset;
        number granulator_02_grains9_curRate;
        number granulator_02_grains9_curStart;
        number granulator_02_grains9_curLen;
        bool granulator_02_grains9_active;
        bool granulator_02_grains9_backwards;
        number granulator_02_grains10_sampleOffset;
        number granulator_02_grains10_curRate;
        number granulator_02_grains10_curStart;
        number granulator_02_grains10_curLen;
        bool granulator_02_grains10_active;
        bool granulator_02_grains10_backwards;
        number granulator_02_grains11_sampleOffset;
        number granulator_02_grains11_curRate;
        number granulator_02_grains11_curStart;
        number granulator_02_grains11_curLen;
        bool granulator_02_grains11_active;
        bool granulator_02_grains11_backwards;
        number granulator_02_grains12_sampleOffset;
        number granulator_02_grains12_curRate;
        number granulator_02_grains12_curStart;
        number granulator_02_grains12_curLen;
        bool granulator_02_grains12_active;
        bool granulator_02_grains12_backwards;
        number granulator_02_grains13_sampleOffset;
        number granulator_02_grains13_curRate;
        number granulator_02_grains13_curStart;
        number granulator_02_grains13_curLen;
        bool granulator_02_grains13_active;
        bool granulator_02_grains13_backwards;
        number granulator_02_grains14_sampleOffset;
        number granulator_02_grains14_curRate;
        number granulator_02_grains14_curStart;
        number granulator_02_grains14_curLen;
        bool granulator_02_grains14_active;
        bool granulator_02_grains14_backwards;
        number granulator_02_grains15_sampleOffset;
        number granulator_02_grains15_curRate;
        number granulator_02_grains15_curStart;
        number granulator_02_grains15_curLen;
        bool granulator_02_grains15_active;
        bool granulator_02_grains15_backwards;
        number granulator_02_grains16_sampleOffset;
        number granulator_02_grains16_curRate;
        number granulator_02_grains16_curStart;
        number granulator_02_grains16_curLen;
        bool granulator_02_grains16_active;
        bool granulator_02_grains16_backwards;
        number granulator_02_grains17_sampleOffset;
        number granulator_02_grains17_curRate;
        number granulator_02_grains17_curStart;
        number granulator_02_grains17_curLen;
        bool granulator_02_grains17_active;
        bool granulator_02_grains17_backwards;
        number granulator_02_grains18_sampleOffset;
        number granulator_02_grains18_curRate;
        number granulator_02_grains18_curStart;
        number granulator_02_grains18_curLen;
        bool granulator_02_grains18_active;
        bool granulator_02_grains18_backwards;
        number granulator_02_grains19_sampleOffset;
        number granulator_02_grains19_curRate;
        number granulator_02_grains19_curStart;
        number granulator_02_grains19_curLen;
        bool granulator_02_grains19_active;
        bool granulator_02_grains19_backwards;
        number granulator_02_grains20_sampleOffset;
        number granulator_02_grains20_curRate;
        number granulator_02_grains20_curStart;
        number granulator_02_grains20_curLen;
        bool granulator_02_grains20_active;
        bool granulator_02_grains20_backwards;
        number granulator_02_grains21_sampleOffset;
        number granulator_02_grains21_curRate;
        number granulator_02_grains21_curStart;
        number granulator_02_grains21_curLen;
        bool granulator_02_grains21_active;
        bool granulator_02_grains21_backwards;
        number granulator_02_grains22_sampleOffset;
        number granulator_02_grains22_curRate;
        number granulator_02_grains22_curStart;
        number granulator_02_grains22_curLen;
        bool granulator_02_grains22_active;
        bool granulator_02_grains22_backwards;
        number granulator_02_grains23_sampleOffset;
        number granulator_02_grains23_curRate;
        number granulator_02_grains23_curStart;
        number granulator_02_grains23_curLen;
        bool granulator_02_grains23_active;
        bool granulator_02_grains23_backwards;
        number granulator_02_grains24_sampleOffset;
        number granulator_02_grains24_curRate;
        number granulator_02_grains24_curStart;
        number granulator_02_grains24_curLen;
        bool granulator_02_grains24_active;
        bool granulator_02_grains24_backwards;
        number granulator_02_grains25_sampleOffset;
        number granulator_02_grains25_curRate;
        number granulator_02_grains25_curStart;
        number granulator_02_grains25_curLen;
        bool granulator_02_grains25_active;
        bool granulator_02_grains25_backwards;
        number granulator_02_grains26_sampleOffset;
        number granulator_02_grains26_curRate;
        number granulator_02_grains26_curStart;
        number granulator_02_grains26_curLen;
        bool granulator_02_grains26_active;
        bool granulator_02_grains26_backwards;
        number granulator_02_grains27_sampleOffset;
        number granulator_02_grains27_curRate;
        number granulator_02_grains27_curStart;
        number granulator_02_grains27_curLen;
        bool granulator_02_grains27_active;
        bool granulator_02_grains27_backwards;
        number granulator_02_grains28_sampleOffset;
        number granulator_02_grains28_curRate;
        number granulator_02_grains28_curStart;
        number granulator_02_grains28_curLen;
        bool granulator_02_grains28_active;
        bool granulator_02_grains28_backwards;
        number granulator_02_grains29_sampleOffset;
        number granulator_02_grains29_curRate;
        number granulator_02_grains29_curStart;
        number granulator_02_grains29_curLen;
        bool granulator_02_grains29_active;
        bool granulator_02_grains29_backwards;
        number granulator_02_grains30_sampleOffset;
        number granulator_02_grains30_curRate;
        number granulator_02_grains30_curStart;
        number granulator_02_grains30_curLen;
        bool granulator_02_grains30_active;
        bool granulator_02_grains30_backwards;
        number granulator_02_grains31_sampleOffset;
        number granulator_02_grains31_curRate;
        number granulator_02_grains31_curStart;
        number granulator_02_grains31_curLen;
        bool granulator_02_grains31_active;
        bool granulator_02_grains31_backwards;
        number granulator_02_grains32_sampleOffset;
        number granulator_02_grains32_curRate;
        number granulator_02_grains32_curStart;
        number granulator_02_grains32_curLen;
        bool granulator_02_grains32_active;
        bool granulator_02_grains32_backwards;
        number param_07_lastValue;
        number param_08_lastValue;
        number param_09_lastValue;
        number param_10_lastValue;
        number param_11_lastValue;
        number param_12_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_100 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_100()
    {
    }
    
    ~RNBOSubpatcher_100()
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
        getEngine()->flushClockEvents(this, -871642103, false);
    }
    
    inline number safemod(number f, number m) {
        if (m != 0) {
            if (m < 0) {
                m = -m;
            }
    
            if (f >= m) {
                if (f >= m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f -= m;
                }
            } else if (f <= -m) {
                if (f <= -m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f += m;
                }
            }
        } else {
            f = 0.0;
        }
    
        return f;
    }
    
    inline number safesqrt(number num) {
        return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index bufferbindchannel(const Index channel, const Index maxChannels, const int channelmode) {
        if (channelmode == 0 || channelmode == 4) {
            return maxChannels - 1 - channel % maxChannels;
        } else if (channelmode == 3) {
            return (maxChannels == 0 ? 0 : channel % maxChannels);
        } else {
            return (channel > maxChannels - 1 ? maxChannels - 1 : (channel < 0 ? 0 : channel));
        }
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
    
    SampleIndex bufferbindindex(
        const SampleValue index,
        const SampleIndex start,
        const SampleIndex end,
        const int boundmode
    ) {
        if (boundmode == 0 || boundmode == 4) {
            if (index >= start && index < end)
                return index;
            else {
                SampleIndex length = (SampleIndex)(end - start);
                number index1 = index - start;
                number wrap = (index1 < 0 ? length * 2 - 1 + this->safemod(index1 + 1, length * 2) : this->safemod(index1, length * 2));
                return (start + wrap >= length ? length * 2 - wrap - 1 : wrap);
            }
        } else if (boundmode == 3) {
            return this->wrap(index, start, end);
        } else {
            return (index > end - 1 ? end - 1 : (index < start ? start : index));
        }
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
    
    template <typename T> inline SampleValue bufferreadsample(
        T& buffer,
        const SampleValue sampleIndex,
        const Index channel,
        const SampleIndex start,
        const SampleIndex end,
        const int interp
    ) {
        if (sampleIndex < 0.0) {
            return 0.0;
        }
    
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(sampleIndex));
    
        if (interp == 0) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->linearinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else if (interp == 1) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->cubicinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 2) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->splineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 3) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->cosineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else {
            return buffer->getSample(channel, index1);
        }
    }
    
    number hann(number x) {
        x = (x > 1 ? 1 : (x < 0 ? 0 : x));
        return 0.5 * (1 - rnbo_cos(6.28318530717958647692 * x));
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "start")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "len")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "rate")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "pan")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "size")) {
            return 5;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        this->dspexpr_18_perform(in2, this->dspexpr_18_in2, this->signals[0], n);
        this->dspexpr_22_perform(in3, this->dspexpr_22_in2, this->signals[1], n);
        this->dspexpr_23_perform(in4, this->dspexpr_23_in2, this->signals[2], n);
        this->dspexpr_24_perform(in5, this->dspexpr_24_in2, this->signals[3], n);
        this->dspexpr_21_perform(this->signals[3], this->dspexpr_21_in2, this->signals[4], n);
        this->dspexpr_20_perform(this->signals[4], this->dspexpr_20_in2, this->signals[3], n);
    
        this->granulator_03_perform(
            this->signals[0],
            this->signals[1],
            this->signals[2],
            this->granulator_03_bchan,
            this->signals[3],
            in1,
            this->signals[4],
            this->signals[5],
            n
        );
    
        this->dspexpr_17_perform(this->signals[4], this->dspexpr_17_in2, out1, n);
        this->dspexpr_19_perform(this->signals[5], this->dspexpr_19_in2, out2, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 6; i++) {
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
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_13_getPresetValue(getSubState(preset, "start"));
        this->param_14_getPresetValue(getSubState(preset, "len"));
        this->param_15_getPresetValue(getSubState(preset, "gain"));
        this->param_16_getPresetValue(getSubState(preset, "rate"));
        this->param_17_getPresetValue(getSubState(preset, "pan"));
        this->param_18_getPresetValue(getSubState(preset, "size"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double samplerate) {
        this->granulator_03_onSampleRateChanged(samplerate);
        this->samplerate_03_onSampleRateChanged(samplerate);
    }
    
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
        case 2:
            this->param_15_value_set(v);
            break;
        case 3:
            this->param_16_value_set(v);
            break;
        case 4:
            this->param_17_value_set(v);
            break;
        case 5:
            this->param_18_value_set(v);
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
        case 2:
            return this->param_15_value;
        case 3:
            return this->param_16_value;
        case 4:
            return this->param_17_value;
        case 5:
            return this->param_18_value;
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
        return 6;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "start";
        case 1:
            return "len";
        case 2:
            return "gain";
        case 3:
            return "rate";
        case 4:
            return "pan";
        case 5:
            return "size";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer2/start";
        case 1:
            return "layer2/len";
        case 2:
            return "layer2/gain";
        case 3:
            return "layer2/rate";
        case 4:
            return "layer2/pan";
        case 5:
            return "layer2/size";
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
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Start";
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
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Length";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
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
                info->displayName = "Gain";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
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
                info->displayName = "Rate";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = -1;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Pan";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 180;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Size";
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
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 180 ? 180 : value));
                ParameterValue normalizedValue = (value - 0) / (180 - 0);
                return normalizedValue;
            }
        case 3:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        case 4:
            {
                value = (value < -1 ? -1 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - -1) / (1 - -1);
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
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (180 - 0);
                }
            }
        case 3:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -1 + value * (1 - -1);
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
        case 2:
            return this->param_15_value_constrain(value);
        case 3:
            return this->param_16_value_constrain(value);
        case 4:
            return this->param_17_value_constrain(value);
        case 5:
            return this->param_18_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -871642103:
            this->loadbang_03_startupbang_bang();
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("startupbang"):
            if (TAG("layer2/loadbang_obj-42") == objectId)
                this->loadbang_03_startupbang_bang();
    
            break;
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("startupbang"):
            return "startupbang";
        case TAG("layer2/loadbang_obj-42"):
            return "layer2/loadbang_obj-42";
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
            this->granulator_03_buffer = new Float32Buffer(this->getPatcher()->layer2);
            this->granulator_03_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->granulator_03_buffer = new Float32Buffer(this->getPatcher()->layer2);
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
    
        this->dspexpr_18_in2_set(v);
    }
    
    void param_14_value_set(number v) {
        v = this->param_14_value_constrain(v);
        this->param_14_value = v;
        this->sendParameter(1, false);
    
        if (this->param_14_value != this->param_14_lastValue) {
            this->getEngine()->presetTouched();
            this->param_14_lastValue = this->param_14_value;
        }
    
        this->dspexpr_22_in2_set(v);
    }
    
    void param_15_value_set(number v) {
        v = this->param_15_value_constrain(v);
        this->param_15_value = v;
        this->sendParameter(2, false);
    
        if (this->param_15_value != this->param_15_lastValue) {
            this->getEngine()->presetTouched();
            this->param_15_lastValue = this->param_15_value;
        }
    
        this->dspexpr_19_in2_set(v);
        this->dspexpr_17_in2_set(v);
    }
    
    void param_16_value_set(number v) {
        v = this->param_16_value_constrain(v);
        this->param_16_value = v;
        this->sendParameter(3, false);
    
        if (this->param_16_value != this->param_16_lastValue) {
            this->getEngine()->presetTouched();
            this->param_16_lastValue = this->param_16_value;
        }
    
        this->dspexpr_23_in2_set(v);
    }
    
    void param_17_value_set(number v) {
        v = this->param_17_value_constrain(v);
        this->param_17_value = v;
        this->sendParameter(4, false);
    
        if (this->param_17_value != this->param_17_lastValue) {
            this->getEngine()->presetTouched();
            this->param_17_lastValue = this->param_17_value;
        }
    
        this->dspexpr_24_in2_set(v);
    }
    
    void param_18_value_set(number v) {
        v = this->param_18_value_constrain(v);
        this->param_18_value = v;
        this->sendParameter(5, false);
    
        if (this->param_18_value != this->param_18_lastValue) {
            this->getEngine()->presetTouched();
            this->param_18_lastValue = this->param_18_value;
        }
    
        this->expr_05_in1_set(v);
    }
    
    void loadbang_03_startupbang_bang() {
        this->loadbang_03_output_bang();
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
        return 5;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->granulator_03_edgedetect_init();
        this->granulator_03_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    
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
    }
    
    void allocateDataRefs() {
        this->granulator_03_buffer = this->granulator_03_buffer->allocateIfNeeded();
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
    
    void dspexpr_18_in2_set(number v) {
        this->dspexpr_18_in2 = v;
    }
    
    static number param_14_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_22_in2_set(number v) {
        this->dspexpr_22_in2 = v;
    }
    
    static number param_15_value_constrain(number v) {
        v = (v > 10 ? 10 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)10 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_19_in2_set(number v) {
        this->dspexpr_19_in2 = v;
    }
    
    void dspexpr_17_in2_set(number v) {
        this->dspexpr_17_in2 = v;
    }
    
    static number param_16_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void dspexpr_23_in2_set(number v) {
        this->dspexpr_23_in2 = v;
    }
    
    static number param_17_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < -1 ? -1 : v));
        return v;
    }
    
    void dspexpr_24_in2_set(number v) {
        this->dspexpr_24_in2 = v;
    }
    
    static number param_18_value_constrain(number v) {
        v = (v > 180 ? 180 : (v < 0 ? 0 : v));
        return v;
    }
    
    void eventoutlet_03_in1_number_set(number v) {
        this->getPatcher()->p_03_out3_number_set(v);
    }
    
    void expr_05_out1_set(number v) {
        this->expr_05_out1 = v;
        this->eventoutlet_03_in1_number_set(this->expr_05_out1);
    }
    
    void expr_05_in1_set(number in1) {
        this->expr_05_in1 = in1;
        this->expr_05_out1_set(this->expr_05_in1 * this->expr_05_in2);//#map:layer2/*_obj-43:1
    }
    
    void expr_05_in2_set(number v) {
        this->expr_05_in2 = v;
    }
    
    void expr_06_out1_set(number v) {
        this->expr_06_out1 = v;
        this->expr_05_in2_set(this->expr_06_out1);
    }
    
    void expr_06_in1_set(number in1) {
        this->expr_06_in1 = in1;
    
        this->expr_06_out1_set(
            (this->expr_06_in2 == 0 ? 0 : (this->expr_06_in2 == 0. ? 0. : this->expr_06_in1 / this->expr_06_in2))
        );//#map:layer2//_obj-44:1
    }
    
    void samplerate_03_samplerate_set(number v) {
        this->expr_06_in1_set(v);
    }
    
    void samplerate_03_input_bang() {
        this->samplerate_03_samplerate_set(this->samplerate());
    }
    
    void loadbang_03_output_bang() {
        this->samplerate_03_input_bang();
    }
    
    void dspexpr_18_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_22_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_23_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_24_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_21_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_20_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void granulator_03_perform(
        const Sample * start,
        const Sample * length,
        const Sample * rate,
        number bchan,
        const Sample * panpos,
        const Sample * trigger,
        Sample * out1,
        Sample * out2,
        Index n
    ) {
        RNBO_UNUSED(bchan);
        auto __granulator_03_grainPan = this->granulator_03_grainPan;
        auto __granulator_03_lengthMul = this->granulator_03_lengthMul;
        auto __granulator_03_startMul = this->granulator_03_startMul;
        auto __granulator_03_limitLast = this->granulator_03_limitLast;
        auto __granulator_03_limit = this->granulator_03_limit;
        number o = 0;
        Index i;
    
        for (i = 0; i < n; i++) {
            bool trig = (bool)(this->granulator_03_edgedetect_next(trigger[(Index)i]) > 0.0);
            int limit = (int)((__granulator_03_limit > 32 ? 32 : (__granulator_03_limit < 0 ? 0 : __granulator_03_limit)));
            __granulator_03_limitLast = limit;
            out1[(Index)i] = 0;
            out2[(Index)i] = 0;
            number s = start[(Index)i];
            number r = rate[(Index)i];
            number l = length[(Index)i];
    
            for (Index j = 0; j < 32; j++) {
                number thistrig = 0;
    
                if ((bool)(trig) && ((bool)(this->granulator_03_grains_inactive(j)) && j < limit)) {
                    trig = false;
                    thistrig = 1;
    
                    {
                        s *= __granulator_03_startMul;
                    }
    
                    {
                        l *= __granulator_03_lengthMul;
                    }
    
                    __granulator_03_grainPan[(Index)j][0] = this->granulator_03_panfunc_next(panpos[(Index)i], 0);
                    __granulator_03_grainPan[(Index)j][1] = this->granulator_03_panfunc_next(panpos[(Index)i], 1);
                }
    
                if (thistrig != 0 || (bool)(!(bool)(this->granulator_03_grains_inactive(j)))) {
                    array<number, 3> ret = this->granulator_03_grains_next(j, this->granulator_03_buffer, thistrig, s, l, r, 0);
                    number v = ret[0] * this->hann(ret[1]);
                    out1[(Index)i] += v * __granulator_03_grainPan[(Index)j][0];
                    out2[(Index)i] += v * __granulator_03_grainPan[(Index)j][1];
                }
            }
        }
    
        this->granulator_03_limitLast = __granulator_03_limitLast;
    }
    
    void dspexpr_17_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_19_perform(const Sample * in1, number in2, Sample * out1, Index n) {
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
    
    number granulator_03_edgedetect_next(number val) {
        if ((0 == 0 && val <= 0) || (0 == 1 && val > 0)) {
            this->granulator_03_edgedetect_active = false;
        } else if ((bool)(!(bool)(this->granulator_03_edgedetect_active))) {
            this->granulator_03_edgedetect_active = true;
            return 1.0;
        }
    
        return 0.0;
    }
    
    void granulator_03_edgedetect_init() {}
    
    void granulator_03_edgedetect_reset() {
        this->granulator_03_edgedetect_active = false;
    }
    
    number granulator_03_panfunc_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    {
                        if (pos >= 0) {
                            pos = this->safemod(pos, 2);
                        } else {
                            pos = this->safemod(2 + this->safemod(pos, 2), 2);
                        }
    
                        if (channel == 0 && pos >= nchan_1)
                            pos = pos - 2;
                    }
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        {
                            return this->safesqrt(1.0 - rnbo_abs(pos));
                        }
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void granulator_03_panfunc_reset() {}
    
    bool granulator_03_grains1_inactive() {
        return !(bool)(this->granulator_03_grains1_active);
    }
    
    void granulator_03_grains1_reset() {
        this->granulator_03_grains1_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains1_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains1_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains1_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains1_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains1_curRate = 0;
                this->granulator_03_grains1_active = false;
            } else {
                this->granulator_03_grains1_curLen = length;
                this->granulator_03_grains1_active = true;
                this->granulator_03_grains1_backwards = rate < 0;
                this->granulator_03_grains1_sampleOffset = 0;
                this->granulator_03_grains1_curStart = start;
    
                if ((bool)(this->granulator_03_grains1_backwards)) {
                    this->granulator_03_grains1_curStart += this->granulator_03_grains1_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains1_active)) {
            if (this->granulator_03_grains1_sampleOffset >= this->granulator_03_grains1_curLen) {
                bangDone = true;
                this->granulator_03_grains1_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains1_curStart;
    
                if ((bool)(this->granulator_03_grains1_backwards)) {
                    sampleIndex -= this->granulator_03_grains1_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains1_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains1_sampleOffset / this->granulator_03_grains1_curLen > 1 ? 1 : (this->granulator_03_grains1_sampleOffset / this->granulator_03_grains1_curLen < 0 ? 0 : this->granulator_03_grains1_sampleOffset / this->granulator_03_grains1_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains1_sampleOffset += this->granulator_03_grains1_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains2_inactive() {
        return !(bool)(this->granulator_03_grains2_active);
    }
    
    void granulator_03_grains2_reset() {
        this->granulator_03_grains2_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains2_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains2_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains2_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains2_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains2_curRate = 0;
                this->granulator_03_grains2_active = false;
            } else {
                this->granulator_03_grains2_curLen = length;
                this->granulator_03_grains2_active = true;
                this->granulator_03_grains2_backwards = rate < 0;
                this->granulator_03_grains2_sampleOffset = 0;
                this->granulator_03_grains2_curStart = start;
    
                if ((bool)(this->granulator_03_grains2_backwards)) {
                    this->granulator_03_grains2_curStart += this->granulator_03_grains2_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains2_active)) {
            if (this->granulator_03_grains2_sampleOffset >= this->granulator_03_grains2_curLen) {
                bangDone = true;
                this->granulator_03_grains2_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains2_curStart;
    
                if ((bool)(this->granulator_03_grains2_backwards)) {
                    sampleIndex -= this->granulator_03_grains2_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains2_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains2_sampleOffset / this->granulator_03_grains2_curLen > 1 ? 1 : (this->granulator_03_grains2_sampleOffset / this->granulator_03_grains2_curLen < 0 ? 0 : this->granulator_03_grains2_sampleOffset / this->granulator_03_grains2_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains2_sampleOffset += this->granulator_03_grains2_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains3_inactive() {
        return !(bool)(this->granulator_03_grains3_active);
    }
    
    void granulator_03_grains3_reset() {
        this->granulator_03_grains3_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains3_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains3_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains3_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains3_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains3_curRate = 0;
                this->granulator_03_grains3_active = false;
            } else {
                this->granulator_03_grains3_curLen = length;
                this->granulator_03_grains3_active = true;
                this->granulator_03_grains3_backwards = rate < 0;
                this->granulator_03_grains3_sampleOffset = 0;
                this->granulator_03_grains3_curStart = start;
    
                if ((bool)(this->granulator_03_grains3_backwards)) {
                    this->granulator_03_grains3_curStart += this->granulator_03_grains3_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains3_active)) {
            if (this->granulator_03_grains3_sampleOffset >= this->granulator_03_grains3_curLen) {
                bangDone = true;
                this->granulator_03_grains3_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains3_curStart;
    
                if ((bool)(this->granulator_03_grains3_backwards)) {
                    sampleIndex -= this->granulator_03_grains3_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains3_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains3_sampleOffset / this->granulator_03_grains3_curLen > 1 ? 1 : (this->granulator_03_grains3_sampleOffset / this->granulator_03_grains3_curLen < 0 ? 0 : this->granulator_03_grains3_sampleOffset / this->granulator_03_grains3_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains3_sampleOffset += this->granulator_03_grains3_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains4_inactive() {
        return !(bool)(this->granulator_03_grains4_active);
    }
    
    void granulator_03_grains4_reset() {
        this->granulator_03_grains4_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains4_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains4_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains4_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains4_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains4_curRate = 0;
                this->granulator_03_grains4_active = false;
            } else {
                this->granulator_03_grains4_curLen = length;
                this->granulator_03_grains4_active = true;
                this->granulator_03_grains4_backwards = rate < 0;
                this->granulator_03_grains4_sampleOffset = 0;
                this->granulator_03_grains4_curStart = start;
    
                if ((bool)(this->granulator_03_grains4_backwards)) {
                    this->granulator_03_grains4_curStart += this->granulator_03_grains4_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains4_active)) {
            if (this->granulator_03_grains4_sampleOffset >= this->granulator_03_grains4_curLen) {
                bangDone = true;
                this->granulator_03_grains4_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains4_curStart;
    
                if ((bool)(this->granulator_03_grains4_backwards)) {
                    sampleIndex -= this->granulator_03_grains4_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains4_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains4_sampleOffset / this->granulator_03_grains4_curLen > 1 ? 1 : (this->granulator_03_grains4_sampleOffset / this->granulator_03_grains4_curLen < 0 ? 0 : this->granulator_03_grains4_sampleOffset / this->granulator_03_grains4_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains4_sampleOffset += this->granulator_03_grains4_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains5_inactive() {
        return !(bool)(this->granulator_03_grains5_active);
    }
    
    void granulator_03_grains5_reset() {
        this->granulator_03_grains5_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains5_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains5_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains5_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains5_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains5_curRate = 0;
                this->granulator_03_grains5_active = false;
            } else {
                this->granulator_03_grains5_curLen = length;
                this->granulator_03_grains5_active = true;
                this->granulator_03_grains5_backwards = rate < 0;
                this->granulator_03_grains5_sampleOffset = 0;
                this->granulator_03_grains5_curStart = start;
    
                if ((bool)(this->granulator_03_grains5_backwards)) {
                    this->granulator_03_grains5_curStart += this->granulator_03_grains5_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains5_active)) {
            if (this->granulator_03_grains5_sampleOffset >= this->granulator_03_grains5_curLen) {
                bangDone = true;
                this->granulator_03_grains5_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains5_curStart;
    
                if ((bool)(this->granulator_03_grains5_backwards)) {
                    sampleIndex -= this->granulator_03_grains5_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains5_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains5_sampleOffset / this->granulator_03_grains5_curLen > 1 ? 1 : (this->granulator_03_grains5_sampleOffset / this->granulator_03_grains5_curLen < 0 ? 0 : this->granulator_03_grains5_sampleOffset / this->granulator_03_grains5_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains5_sampleOffset += this->granulator_03_grains5_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains6_inactive() {
        return !(bool)(this->granulator_03_grains6_active);
    }
    
    void granulator_03_grains6_reset() {
        this->granulator_03_grains6_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains6_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains6_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains6_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains6_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains6_curRate = 0;
                this->granulator_03_grains6_active = false;
            } else {
                this->granulator_03_grains6_curLen = length;
                this->granulator_03_grains6_active = true;
                this->granulator_03_grains6_backwards = rate < 0;
                this->granulator_03_grains6_sampleOffset = 0;
                this->granulator_03_grains6_curStart = start;
    
                if ((bool)(this->granulator_03_grains6_backwards)) {
                    this->granulator_03_grains6_curStart += this->granulator_03_grains6_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains6_active)) {
            if (this->granulator_03_grains6_sampleOffset >= this->granulator_03_grains6_curLen) {
                bangDone = true;
                this->granulator_03_grains6_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains6_curStart;
    
                if ((bool)(this->granulator_03_grains6_backwards)) {
                    sampleIndex -= this->granulator_03_grains6_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains6_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains6_sampleOffset / this->granulator_03_grains6_curLen > 1 ? 1 : (this->granulator_03_grains6_sampleOffset / this->granulator_03_grains6_curLen < 0 ? 0 : this->granulator_03_grains6_sampleOffset / this->granulator_03_grains6_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains6_sampleOffset += this->granulator_03_grains6_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains7_inactive() {
        return !(bool)(this->granulator_03_grains7_active);
    }
    
    void granulator_03_grains7_reset() {
        this->granulator_03_grains7_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains7_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains7_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains7_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains7_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains7_curRate = 0;
                this->granulator_03_grains7_active = false;
            } else {
                this->granulator_03_grains7_curLen = length;
                this->granulator_03_grains7_active = true;
                this->granulator_03_grains7_backwards = rate < 0;
                this->granulator_03_grains7_sampleOffset = 0;
                this->granulator_03_grains7_curStart = start;
    
                if ((bool)(this->granulator_03_grains7_backwards)) {
                    this->granulator_03_grains7_curStart += this->granulator_03_grains7_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains7_active)) {
            if (this->granulator_03_grains7_sampleOffset >= this->granulator_03_grains7_curLen) {
                bangDone = true;
                this->granulator_03_grains7_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains7_curStart;
    
                if ((bool)(this->granulator_03_grains7_backwards)) {
                    sampleIndex -= this->granulator_03_grains7_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains7_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains7_sampleOffset / this->granulator_03_grains7_curLen > 1 ? 1 : (this->granulator_03_grains7_sampleOffset / this->granulator_03_grains7_curLen < 0 ? 0 : this->granulator_03_grains7_sampleOffset / this->granulator_03_grains7_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains7_sampleOffset += this->granulator_03_grains7_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains8_inactive() {
        return !(bool)(this->granulator_03_grains8_active);
    }
    
    void granulator_03_grains8_reset() {
        this->granulator_03_grains8_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains8_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains8_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains8_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains8_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains8_curRate = 0;
                this->granulator_03_grains8_active = false;
            } else {
                this->granulator_03_grains8_curLen = length;
                this->granulator_03_grains8_active = true;
                this->granulator_03_grains8_backwards = rate < 0;
                this->granulator_03_grains8_sampleOffset = 0;
                this->granulator_03_grains8_curStart = start;
    
                if ((bool)(this->granulator_03_grains8_backwards)) {
                    this->granulator_03_grains8_curStart += this->granulator_03_grains8_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains8_active)) {
            if (this->granulator_03_grains8_sampleOffset >= this->granulator_03_grains8_curLen) {
                bangDone = true;
                this->granulator_03_grains8_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains8_curStart;
    
                if ((bool)(this->granulator_03_grains8_backwards)) {
                    sampleIndex -= this->granulator_03_grains8_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains8_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains8_sampleOffset / this->granulator_03_grains8_curLen > 1 ? 1 : (this->granulator_03_grains8_sampleOffset / this->granulator_03_grains8_curLen < 0 ? 0 : this->granulator_03_grains8_sampleOffset / this->granulator_03_grains8_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains8_sampleOffset += this->granulator_03_grains8_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains9_inactive() {
        return !(bool)(this->granulator_03_grains9_active);
    }
    
    void granulator_03_grains9_reset() {
        this->granulator_03_grains9_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains9_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains9_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains9_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains9_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains9_curRate = 0;
                this->granulator_03_grains9_active = false;
            } else {
                this->granulator_03_grains9_curLen = length;
                this->granulator_03_grains9_active = true;
                this->granulator_03_grains9_backwards = rate < 0;
                this->granulator_03_grains9_sampleOffset = 0;
                this->granulator_03_grains9_curStart = start;
    
                if ((bool)(this->granulator_03_grains9_backwards)) {
                    this->granulator_03_grains9_curStart += this->granulator_03_grains9_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains9_active)) {
            if (this->granulator_03_grains9_sampleOffset >= this->granulator_03_grains9_curLen) {
                bangDone = true;
                this->granulator_03_grains9_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains9_curStart;
    
                if ((bool)(this->granulator_03_grains9_backwards)) {
                    sampleIndex -= this->granulator_03_grains9_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains9_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains9_sampleOffset / this->granulator_03_grains9_curLen > 1 ? 1 : (this->granulator_03_grains9_sampleOffset / this->granulator_03_grains9_curLen < 0 ? 0 : this->granulator_03_grains9_sampleOffset / this->granulator_03_grains9_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains9_sampleOffset += this->granulator_03_grains9_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains10_inactive() {
        return !(bool)(this->granulator_03_grains10_active);
    }
    
    void granulator_03_grains10_reset() {
        this->granulator_03_grains10_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains10_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains10_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains10_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains10_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains10_curRate = 0;
                this->granulator_03_grains10_active = false;
            } else {
                this->granulator_03_grains10_curLen = length;
                this->granulator_03_grains10_active = true;
                this->granulator_03_grains10_backwards = rate < 0;
                this->granulator_03_grains10_sampleOffset = 0;
                this->granulator_03_grains10_curStart = start;
    
                if ((bool)(this->granulator_03_grains10_backwards)) {
                    this->granulator_03_grains10_curStart += this->granulator_03_grains10_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains10_active)) {
            if (this->granulator_03_grains10_sampleOffset >= this->granulator_03_grains10_curLen) {
                bangDone = true;
                this->granulator_03_grains10_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains10_curStart;
    
                if ((bool)(this->granulator_03_grains10_backwards)) {
                    sampleIndex -= this->granulator_03_grains10_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains10_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains10_sampleOffset / this->granulator_03_grains10_curLen > 1 ? 1 : (this->granulator_03_grains10_sampleOffset / this->granulator_03_grains10_curLen < 0 ? 0 : this->granulator_03_grains10_sampleOffset / this->granulator_03_grains10_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains10_sampleOffset += this->granulator_03_grains10_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains11_inactive() {
        return !(bool)(this->granulator_03_grains11_active);
    }
    
    void granulator_03_grains11_reset() {
        this->granulator_03_grains11_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains11_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains11_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains11_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains11_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains11_curRate = 0;
                this->granulator_03_grains11_active = false;
            } else {
                this->granulator_03_grains11_curLen = length;
                this->granulator_03_grains11_active = true;
                this->granulator_03_grains11_backwards = rate < 0;
                this->granulator_03_grains11_sampleOffset = 0;
                this->granulator_03_grains11_curStart = start;
    
                if ((bool)(this->granulator_03_grains11_backwards)) {
                    this->granulator_03_grains11_curStart += this->granulator_03_grains11_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains11_active)) {
            if (this->granulator_03_grains11_sampleOffset >= this->granulator_03_grains11_curLen) {
                bangDone = true;
                this->granulator_03_grains11_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains11_curStart;
    
                if ((bool)(this->granulator_03_grains11_backwards)) {
                    sampleIndex -= this->granulator_03_grains11_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains11_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains11_sampleOffset / this->granulator_03_grains11_curLen > 1 ? 1 : (this->granulator_03_grains11_sampleOffset / this->granulator_03_grains11_curLen < 0 ? 0 : this->granulator_03_grains11_sampleOffset / this->granulator_03_grains11_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains11_sampleOffset += this->granulator_03_grains11_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains12_inactive() {
        return !(bool)(this->granulator_03_grains12_active);
    }
    
    void granulator_03_grains12_reset() {
        this->granulator_03_grains12_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains12_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains12_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains12_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains12_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains12_curRate = 0;
                this->granulator_03_grains12_active = false;
            } else {
                this->granulator_03_grains12_curLen = length;
                this->granulator_03_grains12_active = true;
                this->granulator_03_grains12_backwards = rate < 0;
                this->granulator_03_grains12_sampleOffset = 0;
                this->granulator_03_grains12_curStart = start;
    
                if ((bool)(this->granulator_03_grains12_backwards)) {
                    this->granulator_03_grains12_curStart += this->granulator_03_grains12_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains12_active)) {
            if (this->granulator_03_grains12_sampleOffset >= this->granulator_03_grains12_curLen) {
                bangDone = true;
                this->granulator_03_grains12_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains12_curStart;
    
                if ((bool)(this->granulator_03_grains12_backwards)) {
                    sampleIndex -= this->granulator_03_grains12_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains12_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains12_sampleOffset / this->granulator_03_grains12_curLen > 1 ? 1 : (this->granulator_03_grains12_sampleOffset / this->granulator_03_grains12_curLen < 0 ? 0 : this->granulator_03_grains12_sampleOffset / this->granulator_03_grains12_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains12_sampleOffset += this->granulator_03_grains12_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains13_inactive() {
        return !(bool)(this->granulator_03_grains13_active);
    }
    
    void granulator_03_grains13_reset() {
        this->granulator_03_grains13_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains13_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains13_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains13_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains13_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains13_curRate = 0;
                this->granulator_03_grains13_active = false;
            } else {
                this->granulator_03_grains13_curLen = length;
                this->granulator_03_grains13_active = true;
                this->granulator_03_grains13_backwards = rate < 0;
                this->granulator_03_grains13_sampleOffset = 0;
                this->granulator_03_grains13_curStart = start;
    
                if ((bool)(this->granulator_03_grains13_backwards)) {
                    this->granulator_03_grains13_curStart += this->granulator_03_grains13_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains13_active)) {
            if (this->granulator_03_grains13_sampleOffset >= this->granulator_03_grains13_curLen) {
                bangDone = true;
                this->granulator_03_grains13_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains13_curStart;
    
                if ((bool)(this->granulator_03_grains13_backwards)) {
                    sampleIndex -= this->granulator_03_grains13_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains13_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains13_sampleOffset / this->granulator_03_grains13_curLen > 1 ? 1 : (this->granulator_03_grains13_sampleOffset / this->granulator_03_grains13_curLen < 0 ? 0 : this->granulator_03_grains13_sampleOffset / this->granulator_03_grains13_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains13_sampleOffset += this->granulator_03_grains13_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains14_inactive() {
        return !(bool)(this->granulator_03_grains14_active);
    }
    
    void granulator_03_grains14_reset() {
        this->granulator_03_grains14_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains14_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains14_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains14_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains14_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains14_curRate = 0;
                this->granulator_03_grains14_active = false;
            } else {
                this->granulator_03_grains14_curLen = length;
                this->granulator_03_grains14_active = true;
                this->granulator_03_grains14_backwards = rate < 0;
                this->granulator_03_grains14_sampleOffset = 0;
                this->granulator_03_grains14_curStart = start;
    
                if ((bool)(this->granulator_03_grains14_backwards)) {
                    this->granulator_03_grains14_curStart += this->granulator_03_grains14_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains14_active)) {
            if (this->granulator_03_grains14_sampleOffset >= this->granulator_03_grains14_curLen) {
                bangDone = true;
                this->granulator_03_grains14_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains14_curStart;
    
                if ((bool)(this->granulator_03_grains14_backwards)) {
                    sampleIndex -= this->granulator_03_grains14_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains14_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains14_sampleOffset / this->granulator_03_grains14_curLen > 1 ? 1 : (this->granulator_03_grains14_sampleOffset / this->granulator_03_grains14_curLen < 0 ? 0 : this->granulator_03_grains14_sampleOffset / this->granulator_03_grains14_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains14_sampleOffset += this->granulator_03_grains14_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains15_inactive() {
        return !(bool)(this->granulator_03_grains15_active);
    }
    
    void granulator_03_grains15_reset() {
        this->granulator_03_grains15_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains15_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains15_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains15_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains15_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains15_curRate = 0;
                this->granulator_03_grains15_active = false;
            } else {
                this->granulator_03_grains15_curLen = length;
                this->granulator_03_grains15_active = true;
                this->granulator_03_grains15_backwards = rate < 0;
                this->granulator_03_grains15_sampleOffset = 0;
                this->granulator_03_grains15_curStart = start;
    
                if ((bool)(this->granulator_03_grains15_backwards)) {
                    this->granulator_03_grains15_curStart += this->granulator_03_grains15_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains15_active)) {
            if (this->granulator_03_grains15_sampleOffset >= this->granulator_03_grains15_curLen) {
                bangDone = true;
                this->granulator_03_grains15_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains15_curStart;
    
                if ((bool)(this->granulator_03_grains15_backwards)) {
                    sampleIndex -= this->granulator_03_grains15_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains15_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains15_sampleOffset / this->granulator_03_grains15_curLen > 1 ? 1 : (this->granulator_03_grains15_sampleOffset / this->granulator_03_grains15_curLen < 0 ? 0 : this->granulator_03_grains15_sampleOffset / this->granulator_03_grains15_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains15_sampleOffset += this->granulator_03_grains15_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains16_inactive() {
        return !(bool)(this->granulator_03_grains16_active);
    }
    
    void granulator_03_grains16_reset() {
        this->granulator_03_grains16_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains16_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains16_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains16_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains16_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains16_curRate = 0;
                this->granulator_03_grains16_active = false;
            } else {
                this->granulator_03_grains16_curLen = length;
                this->granulator_03_grains16_active = true;
                this->granulator_03_grains16_backwards = rate < 0;
                this->granulator_03_grains16_sampleOffset = 0;
                this->granulator_03_grains16_curStart = start;
    
                if ((bool)(this->granulator_03_grains16_backwards)) {
                    this->granulator_03_grains16_curStart += this->granulator_03_grains16_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains16_active)) {
            if (this->granulator_03_grains16_sampleOffset >= this->granulator_03_grains16_curLen) {
                bangDone = true;
                this->granulator_03_grains16_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains16_curStart;
    
                if ((bool)(this->granulator_03_grains16_backwards)) {
                    sampleIndex -= this->granulator_03_grains16_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains16_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains16_sampleOffset / this->granulator_03_grains16_curLen > 1 ? 1 : (this->granulator_03_grains16_sampleOffset / this->granulator_03_grains16_curLen < 0 ? 0 : this->granulator_03_grains16_sampleOffset / this->granulator_03_grains16_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains16_sampleOffset += this->granulator_03_grains16_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains17_inactive() {
        return !(bool)(this->granulator_03_grains17_active);
    }
    
    void granulator_03_grains17_reset() {
        this->granulator_03_grains17_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains17_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains17_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains17_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains17_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains17_curRate = 0;
                this->granulator_03_grains17_active = false;
            } else {
                this->granulator_03_grains17_curLen = length;
                this->granulator_03_grains17_active = true;
                this->granulator_03_grains17_backwards = rate < 0;
                this->granulator_03_grains17_sampleOffset = 0;
                this->granulator_03_grains17_curStart = start;
    
                if ((bool)(this->granulator_03_grains17_backwards)) {
                    this->granulator_03_grains17_curStart += this->granulator_03_grains17_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains17_active)) {
            if (this->granulator_03_grains17_sampleOffset >= this->granulator_03_grains17_curLen) {
                bangDone = true;
                this->granulator_03_grains17_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains17_curStart;
    
                if ((bool)(this->granulator_03_grains17_backwards)) {
                    sampleIndex -= this->granulator_03_grains17_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains17_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains17_sampleOffset / this->granulator_03_grains17_curLen > 1 ? 1 : (this->granulator_03_grains17_sampleOffset / this->granulator_03_grains17_curLen < 0 ? 0 : this->granulator_03_grains17_sampleOffset / this->granulator_03_grains17_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains17_sampleOffset += this->granulator_03_grains17_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains18_inactive() {
        return !(bool)(this->granulator_03_grains18_active);
    }
    
    void granulator_03_grains18_reset() {
        this->granulator_03_grains18_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains18_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains18_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains18_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains18_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains18_curRate = 0;
                this->granulator_03_grains18_active = false;
            } else {
                this->granulator_03_grains18_curLen = length;
                this->granulator_03_grains18_active = true;
                this->granulator_03_grains18_backwards = rate < 0;
                this->granulator_03_grains18_sampleOffset = 0;
                this->granulator_03_grains18_curStart = start;
    
                if ((bool)(this->granulator_03_grains18_backwards)) {
                    this->granulator_03_grains18_curStart += this->granulator_03_grains18_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains18_active)) {
            if (this->granulator_03_grains18_sampleOffset >= this->granulator_03_grains18_curLen) {
                bangDone = true;
                this->granulator_03_grains18_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains18_curStart;
    
                if ((bool)(this->granulator_03_grains18_backwards)) {
                    sampleIndex -= this->granulator_03_grains18_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains18_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains18_sampleOffset / this->granulator_03_grains18_curLen > 1 ? 1 : (this->granulator_03_grains18_sampleOffset / this->granulator_03_grains18_curLen < 0 ? 0 : this->granulator_03_grains18_sampleOffset / this->granulator_03_grains18_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains18_sampleOffset += this->granulator_03_grains18_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains19_inactive() {
        return !(bool)(this->granulator_03_grains19_active);
    }
    
    void granulator_03_grains19_reset() {
        this->granulator_03_grains19_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains19_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains19_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains19_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains19_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains19_curRate = 0;
                this->granulator_03_grains19_active = false;
            } else {
                this->granulator_03_grains19_curLen = length;
                this->granulator_03_grains19_active = true;
                this->granulator_03_grains19_backwards = rate < 0;
                this->granulator_03_grains19_sampleOffset = 0;
                this->granulator_03_grains19_curStart = start;
    
                if ((bool)(this->granulator_03_grains19_backwards)) {
                    this->granulator_03_grains19_curStart += this->granulator_03_grains19_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains19_active)) {
            if (this->granulator_03_grains19_sampleOffset >= this->granulator_03_grains19_curLen) {
                bangDone = true;
                this->granulator_03_grains19_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains19_curStart;
    
                if ((bool)(this->granulator_03_grains19_backwards)) {
                    sampleIndex -= this->granulator_03_grains19_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains19_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains19_sampleOffset / this->granulator_03_grains19_curLen > 1 ? 1 : (this->granulator_03_grains19_sampleOffset / this->granulator_03_grains19_curLen < 0 ? 0 : this->granulator_03_grains19_sampleOffset / this->granulator_03_grains19_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains19_sampleOffset += this->granulator_03_grains19_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains20_inactive() {
        return !(bool)(this->granulator_03_grains20_active);
    }
    
    void granulator_03_grains20_reset() {
        this->granulator_03_grains20_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains20_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains20_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains20_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains20_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains20_curRate = 0;
                this->granulator_03_grains20_active = false;
            } else {
                this->granulator_03_grains20_curLen = length;
                this->granulator_03_grains20_active = true;
                this->granulator_03_grains20_backwards = rate < 0;
                this->granulator_03_grains20_sampleOffset = 0;
                this->granulator_03_grains20_curStart = start;
    
                if ((bool)(this->granulator_03_grains20_backwards)) {
                    this->granulator_03_grains20_curStart += this->granulator_03_grains20_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains20_active)) {
            if (this->granulator_03_grains20_sampleOffset >= this->granulator_03_grains20_curLen) {
                bangDone = true;
                this->granulator_03_grains20_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains20_curStart;
    
                if ((bool)(this->granulator_03_grains20_backwards)) {
                    sampleIndex -= this->granulator_03_grains20_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains20_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains20_sampleOffset / this->granulator_03_grains20_curLen > 1 ? 1 : (this->granulator_03_grains20_sampleOffset / this->granulator_03_grains20_curLen < 0 ? 0 : this->granulator_03_grains20_sampleOffset / this->granulator_03_grains20_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains20_sampleOffset += this->granulator_03_grains20_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains21_inactive() {
        return !(bool)(this->granulator_03_grains21_active);
    }
    
    void granulator_03_grains21_reset() {
        this->granulator_03_grains21_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains21_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains21_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains21_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains21_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains21_curRate = 0;
                this->granulator_03_grains21_active = false;
            } else {
                this->granulator_03_grains21_curLen = length;
                this->granulator_03_grains21_active = true;
                this->granulator_03_grains21_backwards = rate < 0;
                this->granulator_03_grains21_sampleOffset = 0;
                this->granulator_03_grains21_curStart = start;
    
                if ((bool)(this->granulator_03_grains21_backwards)) {
                    this->granulator_03_grains21_curStart += this->granulator_03_grains21_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains21_active)) {
            if (this->granulator_03_grains21_sampleOffset >= this->granulator_03_grains21_curLen) {
                bangDone = true;
                this->granulator_03_grains21_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains21_curStart;
    
                if ((bool)(this->granulator_03_grains21_backwards)) {
                    sampleIndex -= this->granulator_03_grains21_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains21_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains21_sampleOffset / this->granulator_03_grains21_curLen > 1 ? 1 : (this->granulator_03_grains21_sampleOffset / this->granulator_03_grains21_curLen < 0 ? 0 : this->granulator_03_grains21_sampleOffset / this->granulator_03_grains21_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains21_sampleOffset += this->granulator_03_grains21_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains22_inactive() {
        return !(bool)(this->granulator_03_grains22_active);
    }
    
    void granulator_03_grains22_reset() {
        this->granulator_03_grains22_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains22_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains22_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains22_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains22_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains22_curRate = 0;
                this->granulator_03_grains22_active = false;
            } else {
                this->granulator_03_grains22_curLen = length;
                this->granulator_03_grains22_active = true;
                this->granulator_03_grains22_backwards = rate < 0;
                this->granulator_03_grains22_sampleOffset = 0;
                this->granulator_03_grains22_curStart = start;
    
                if ((bool)(this->granulator_03_grains22_backwards)) {
                    this->granulator_03_grains22_curStart += this->granulator_03_grains22_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains22_active)) {
            if (this->granulator_03_grains22_sampleOffset >= this->granulator_03_grains22_curLen) {
                bangDone = true;
                this->granulator_03_grains22_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains22_curStart;
    
                if ((bool)(this->granulator_03_grains22_backwards)) {
                    sampleIndex -= this->granulator_03_grains22_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains22_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains22_sampleOffset / this->granulator_03_grains22_curLen > 1 ? 1 : (this->granulator_03_grains22_sampleOffset / this->granulator_03_grains22_curLen < 0 ? 0 : this->granulator_03_grains22_sampleOffset / this->granulator_03_grains22_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains22_sampleOffset += this->granulator_03_grains22_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains23_inactive() {
        return !(bool)(this->granulator_03_grains23_active);
    }
    
    void granulator_03_grains23_reset() {
        this->granulator_03_grains23_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains23_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains23_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains23_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains23_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains23_curRate = 0;
                this->granulator_03_grains23_active = false;
            } else {
                this->granulator_03_grains23_curLen = length;
                this->granulator_03_grains23_active = true;
                this->granulator_03_grains23_backwards = rate < 0;
                this->granulator_03_grains23_sampleOffset = 0;
                this->granulator_03_grains23_curStart = start;
    
                if ((bool)(this->granulator_03_grains23_backwards)) {
                    this->granulator_03_grains23_curStart += this->granulator_03_grains23_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains23_active)) {
            if (this->granulator_03_grains23_sampleOffset >= this->granulator_03_grains23_curLen) {
                bangDone = true;
                this->granulator_03_grains23_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains23_curStart;
    
                if ((bool)(this->granulator_03_grains23_backwards)) {
                    sampleIndex -= this->granulator_03_grains23_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains23_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains23_sampleOffset / this->granulator_03_grains23_curLen > 1 ? 1 : (this->granulator_03_grains23_sampleOffset / this->granulator_03_grains23_curLen < 0 ? 0 : this->granulator_03_grains23_sampleOffset / this->granulator_03_grains23_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains23_sampleOffset += this->granulator_03_grains23_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains24_inactive() {
        return !(bool)(this->granulator_03_grains24_active);
    }
    
    void granulator_03_grains24_reset() {
        this->granulator_03_grains24_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains24_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains24_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains24_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains24_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains24_curRate = 0;
                this->granulator_03_grains24_active = false;
            } else {
                this->granulator_03_grains24_curLen = length;
                this->granulator_03_grains24_active = true;
                this->granulator_03_grains24_backwards = rate < 0;
                this->granulator_03_grains24_sampleOffset = 0;
                this->granulator_03_grains24_curStart = start;
    
                if ((bool)(this->granulator_03_grains24_backwards)) {
                    this->granulator_03_grains24_curStart += this->granulator_03_grains24_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains24_active)) {
            if (this->granulator_03_grains24_sampleOffset >= this->granulator_03_grains24_curLen) {
                bangDone = true;
                this->granulator_03_grains24_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains24_curStart;
    
                if ((bool)(this->granulator_03_grains24_backwards)) {
                    sampleIndex -= this->granulator_03_grains24_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains24_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains24_sampleOffset / this->granulator_03_grains24_curLen > 1 ? 1 : (this->granulator_03_grains24_sampleOffset / this->granulator_03_grains24_curLen < 0 ? 0 : this->granulator_03_grains24_sampleOffset / this->granulator_03_grains24_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains24_sampleOffset += this->granulator_03_grains24_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains25_inactive() {
        return !(bool)(this->granulator_03_grains25_active);
    }
    
    void granulator_03_grains25_reset() {
        this->granulator_03_grains25_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains25_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains25_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains25_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains25_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains25_curRate = 0;
                this->granulator_03_grains25_active = false;
            } else {
                this->granulator_03_grains25_curLen = length;
                this->granulator_03_grains25_active = true;
                this->granulator_03_grains25_backwards = rate < 0;
                this->granulator_03_grains25_sampleOffset = 0;
                this->granulator_03_grains25_curStart = start;
    
                if ((bool)(this->granulator_03_grains25_backwards)) {
                    this->granulator_03_grains25_curStart += this->granulator_03_grains25_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains25_active)) {
            if (this->granulator_03_grains25_sampleOffset >= this->granulator_03_grains25_curLen) {
                bangDone = true;
                this->granulator_03_grains25_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains25_curStart;
    
                if ((bool)(this->granulator_03_grains25_backwards)) {
                    sampleIndex -= this->granulator_03_grains25_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains25_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains25_sampleOffset / this->granulator_03_grains25_curLen > 1 ? 1 : (this->granulator_03_grains25_sampleOffset / this->granulator_03_grains25_curLen < 0 ? 0 : this->granulator_03_grains25_sampleOffset / this->granulator_03_grains25_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains25_sampleOffset += this->granulator_03_grains25_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains26_inactive() {
        return !(bool)(this->granulator_03_grains26_active);
    }
    
    void granulator_03_grains26_reset() {
        this->granulator_03_grains26_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains26_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains26_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains26_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains26_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains26_curRate = 0;
                this->granulator_03_grains26_active = false;
            } else {
                this->granulator_03_grains26_curLen = length;
                this->granulator_03_grains26_active = true;
                this->granulator_03_grains26_backwards = rate < 0;
                this->granulator_03_grains26_sampleOffset = 0;
                this->granulator_03_grains26_curStart = start;
    
                if ((bool)(this->granulator_03_grains26_backwards)) {
                    this->granulator_03_grains26_curStart += this->granulator_03_grains26_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains26_active)) {
            if (this->granulator_03_grains26_sampleOffset >= this->granulator_03_grains26_curLen) {
                bangDone = true;
                this->granulator_03_grains26_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains26_curStart;
    
                if ((bool)(this->granulator_03_grains26_backwards)) {
                    sampleIndex -= this->granulator_03_grains26_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains26_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains26_sampleOffset / this->granulator_03_grains26_curLen > 1 ? 1 : (this->granulator_03_grains26_sampleOffset / this->granulator_03_grains26_curLen < 0 ? 0 : this->granulator_03_grains26_sampleOffset / this->granulator_03_grains26_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains26_sampleOffset += this->granulator_03_grains26_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains27_inactive() {
        return !(bool)(this->granulator_03_grains27_active);
    }
    
    void granulator_03_grains27_reset() {
        this->granulator_03_grains27_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains27_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains27_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains27_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains27_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains27_curRate = 0;
                this->granulator_03_grains27_active = false;
            } else {
                this->granulator_03_grains27_curLen = length;
                this->granulator_03_grains27_active = true;
                this->granulator_03_grains27_backwards = rate < 0;
                this->granulator_03_grains27_sampleOffset = 0;
                this->granulator_03_grains27_curStart = start;
    
                if ((bool)(this->granulator_03_grains27_backwards)) {
                    this->granulator_03_grains27_curStart += this->granulator_03_grains27_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains27_active)) {
            if (this->granulator_03_grains27_sampleOffset >= this->granulator_03_grains27_curLen) {
                bangDone = true;
                this->granulator_03_grains27_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains27_curStart;
    
                if ((bool)(this->granulator_03_grains27_backwards)) {
                    sampleIndex -= this->granulator_03_grains27_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains27_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains27_sampleOffset / this->granulator_03_grains27_curLen > 1 ? 1 : (this->granulator_03_grains27_sampleOffset / this->granulator_03_grains27_curLen < 0 ? 0 : this->granulator_03_grains27_sampleOffset / this->granulator_03_grains27_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains27_sampleOffset += this->granulator_03_grains27_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains28_inactive() {
        return !(bool)(this->granulator_03_grains28_active);
    }
    
    void granulator_03_grains28_reset() {
        this->granulator_03_grains28_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains28_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains28_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains28_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains28_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains28_curRate = 0;
                this->granulator_03_grains28_active = false;
            } else {
                this->granulator_03_grains28_curLen = length;
                this->granulator_03_grains28_active = true;
                this->granulator_03_grains28_backwards = rate < 0;
                this->granulator_03_grains28_sampleOffset = 0;
                this->granulator_03_grains28_curStart = start;
    
                if ((bool)(this->granulator_03_grains28_backwards)) {
                    this->granulator_03_grains28_curStart += this->granulator_03_grains28_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains28_active)) {
            if (this->granulator_03_grains28_sampleOffset >= this->granulator_03_grains28_curLen) {
                bangDone = true;
                this->granulator_03_grains28_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains28_curStart;
    
                if ((bool)(this->granulator_03_grains28_backwards)) {
                    sampleIndex -= this->granulator_03_grains28_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains28_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains28_sampleOffset / this->granulator_03_grains28_curLen > 1 ? 1 : (this->granulator_03_grains28_sampleOffset / this->granulator_03_grains28_curLen < 0 ? 0 : this->granulator_03_grains28_sampleOffset / this->granulator_03_grains28_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains28_sampleOffset += this->granulator_03_grains28_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains29_inactive() {
        return !(bool)(this->granulator_03_grains29_active);
    }
    
    void granulator_03_grains29_reset() {
        this->granulator_03_grains29_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains29_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains29_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains29_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains29_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains29_curRate = 0;
                this->granulator_03_grains29_active = false;
            } else {
                this->granulator_03_grains29_curLen = length;
                this->granulator_03_grains29_active = true;
                this->granulator_03_grains29_backwards = rate < 0;
                this->granulator_03_grains29_sampleOffset = 0;
                this->granulator_03_grains29_curStart = start;
    
                if ((bool)(this->granulator_03_grains29_backwards)) {
                    this->granulator_03_grains29_curStart += this->granulator_03_grains29_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains29_active)) {
            if (this->granulator_03_grains29_sampleOffset >= this->granulator_03_grains29_curLen) {
                bangDone = true;
                this->granulator_03_grains29_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains29_curStart;
    
                if ((bool)(this->granulator_03_grains29_backwards)) {
                    sampleIndex -= this->granulator_03_grains29_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains29_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains29_sampleOffset / this->granulator_03_grains29_curLen > 1 ? 1 : (this->granulator_03_grains29_sampleOffset / this->granulator_03_grains29_curLen < 0 ? 0 : this->granulator_03_grains29_sampleOffset / this->granulator_03_grains29_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains29_sampleOffset += this->granulator_03_grains29_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains30_inactive() {
        return !(bool)(this->granulator_03_grains30_active);
    }
    
    void granulator_03_grains30_reset() {
        this->granulator_03_grains30_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains30_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains30_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains30_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains30_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains30_curRate = 0;
                this->granulator_03_grains30_active = false;
            } else {
                this->granulator_03_grains30_curLen = length;
                this->granulator_03_grains30_active = true;
                this->granulator_03_grains30_backwards = rate < 0;
                this->granulator_03_grains30_sampleOffset = 0;
                this->granulator_03_grains30_curStart = start;
    
                if ((bool)(this->granulator_03_grains30_backwards)) {
                    this->granulator_03_grains30_curStart += this->granulator_03_grains30_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains30_active)) {
            if (this->granulator_03_grains30_sampleOffset >= this->granulator_03_grains30_curLen) {
                bangDone = true;
                this->granulator_03_grains30_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains30_curStart;
    
                if ((bool)(this->granulator_03_grains30_backwards)) {
                    sampleIndex -= this->granulator_03_grains30_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains30_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains30_sampleOffset / this->granulator_03_grains30_curLen > 1 ? 1 : (this->granulator_03_grains30_sampleOffset / this->granulator_03_grains30_curLen < 0 ? 0 : this->granulator_03_grains30_sampleOffset / this->granulator_03_grains30_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains30_sampleOffset += this->granulator_03_grains30_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains31_inactive() {
        return !(bool)(this->granulator_03_grains31_active);
    }
    
    void granulator_03_grains31_reset() {
        this->granulator_03_grains31_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains31_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains31_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains31_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains31_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains31_curRate = 0;
                this->granulator_03_grains31_active = false;
            } else {
                this->granulator_03_grains31_curLen = length;
                this->granulator_03_grains31_active = true;
                this->granulator_03_grains31_backwards = rate < 0;
                this->granulator_03_grains31_sampleOffset = 0;
                this->granulator_03_grains31_curStart = start;
    
                if ((bool)(this->granulator_03_grains31_backwards)) {
                    this->granulator_03_grains31_curStart += this->granulator_03_grains31_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains31_active)) {
            if (this->granulator_03_grains31_sampleOffset >= this->granulator_03_grains31_curLen) {
                bangDone = true;
                this->granulator_03_grains31_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains31_curStart;
    
                if ((bool)(this->granulator_03_grains31_backwards)) {
                    sampleIndex -= this->granulator_03_grains31_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains31_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains31_sampleOffset / this->granulator_03_grains31_curLen > 1 ? 1 : (this->granulator_03_grains31_sampleOffset / this->granulator_03_grains31_curLen < 0 ? 0 : this->granulator_03_grains31_sampleOffset / this->granulator_03_grains31_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains31_sampleOffset += this->granulator_03_grains31_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains32_inactive() {
        return !(bool)(this->granulator_03_grains32_active);
    }
    
    void granulator_03_grains32_reset() {
        this->granulator_03_grains32_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains32_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_03_grains32_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_03_grains32_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_03_grains32_active)) {
                    bangDone = true;
                }
    
                this->granulator_03_grains32_curRate = 0;
                this->granulator_03_grains32_active = false;
            } else {
                this->granulator_03_grains32_curLen = length;
                this->granulator_03_grains32_active = true;
                this->granulator_03_grains32_backwards = rate < 0;
                this->granulator_03_grains32_sampleOffset = 0;
                this->granulator_03_grains32_curStart = start;
    
                if ((bool)(this->granulator_03_grains32_backwards)) {
                    this->granulator_03_grains32_curStart += this->granulator_03_grains32_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_03_grains32_active)) {
            if (this->granulator_03_grains32_sampleOffset >= this->granulator_03_grains32_curLen) {
                bangDone = true;
                this->granulator_03_grains32_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_03_grains32_curStart;
    
                if ((bool)(this->granulator_03_grains32_backwards)) {
                    sampleIndex -= this->granulator_03_grains32_sampleOffset;
                } else {
                    sampleIndex += this->granulator_03_grains32_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_03_grains32_sampleOffset / this->granulator_03_grains32_curLen > 1 ? 1 : (this->granulator_03_grains32_sampleOffset / this->granulator_03_grains32_curLen < 0 ? 0 : this->granulator_03_grains32_sampleOffset / this->granulator_03_grains32_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_03_grains32_sampleOffset += this->granulator_03_grains32_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_03_grains_inactive(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_03_grains1_inactive();
        case 1:
            return this->granulator_03_grains2_inactive();
        case 2:
            return this->granulator_03_grains3_inactive();
        case 3:
            return this->granulator_03_grains4_inactive();
        case 4:
            return this->granulator_03_grains5_inactive();
        case 5:
            return this->granulator_03_grains6_inactive();
        case 6:
            return this->granulator_03_grains7_inactive();
        case 7:
            return this->granulator_03_grains8_inactive();
        case 8:
            return this->granulator_03_grains9_inactive();
        case 9:
            return this->granulator_03_grains10_inactive();
        case 10:
            return this->granulator_03_grains11_inactive();
        case 11:
            return this->granulator_03_grains12_inactive();
        case 12:
            return this->granulator_03_grains13_inactive();
        case 13:
            return this->granulator_03_grains14_inactive();
        case 14:
            return this->granulator_03_grains15_inactive();
        case 15:
            return this->granulator_03_grains16_inactive();
        case 16:
            return this->granulator_03_grains17_inactive();
        case 17:
            return this->granulator_03_grains18_inactive();
        case 18:
            return this->granulator_03_grains19_inactive();
        case 19:
            return this->granulator_03_grains20_inactive();
        case 20:
            return this->granulator_03_grains21_inactive();
        case 21:
            return this->granulator_03_grains22_inactive();
        case 22:
            return this->granulator_03_grains23_inactive();
        case 23:
            return this->granulator_03_grains24_inactive();
        case 24:
            return this->granulator_03_grains25_inactive();
        case 25:
            return this->granulator_03_grains26_inactive();
        case 26:
            return this->granulator_03_grains27_inactive();
        case 27:
            return this->granulator_03_grains28_inactive();
        case 28:
            return this->granulator_03_grains29_inactive();
        case 29:
            return this->granulator_03_grains30_inactive();
        case 30:
            return this->granulator_03_grains31_inactive();
        default:
            return this->granulator_03_grains32_inactive();
        }
    
        return 0;
    }
    
    void granulator_03_grains_reset(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_03_grains1_reset();
        case 1:
            return this->granulator_03_grains2_reset();
        case 2:
            return this->granulator_03_grains3_reset();
        case 3:
            return this->granulator_03_grains4_reset();
        case 4:
            return this->granulator_03_grains5_reset();
        case 5:
            return this->granulator_03_grains6_reset();
        case 6:
            return this->granulator_03_grains7_reset();
        case 7:
            return this->granulator_03_grains8_reset();
        case 8:
            return this->granulator_03_grains9_reset();
        case 9:
            return this->granulator_03_grains10_reset();
        case 10:
            return this->granulator_03_grains11_reset();
        case 11:
            return this->granulator_03_grains12_reset();
        case 12:
            return this->granulator_03_grains13_reset();
        case 13:
            return this->granulator_03_grains14_reset();
        case 14:
            return this->granulator_03_grains15_reset();
        case 15:
            return this->granulator_03_grains16_reset();
        case 16:
            return this->granulator_03_grains17_reset();
        case 17:
            return this->granulator_03_grains18_reset();
        case 18:
            return this->granulator_03_grains19_reset();
        case 19:
            return this->granulator_03_grains20_reset();
        case 20:
            return this->granulator_03_grains21_reset();
        case 21:
            return this->granulator_03_grains22_reset();
        case 22:
            return this->granulator_03_grains23_reset();
        case 23:
            return this->granulator_03_grains24_reset();
        case 24:
            return this->granulator_03_grains25_reset();
        case 25:
            return this->granulator_03_grains26_reset();
        case 26:
            return this->granulator_03_grains27_reset();
        case 27:
            return this->granulator_03_grains28_reset();
        case 28:
            return this->granulator_03_grains29_reset();
        case 29:
            return this->granulator_03_grains30_reset();
        case 30:
            return this->granulator_03_grains31_reset();
        default:
            return this->granulator_03_grains32_reset();
        }
    }
    
    template <typename T> inline array<number, 3> granulator_03_grains_next(
        Index i,
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        switch ((int)i) {
        case 0:
            return this->granulator_03_grains1_next(buffer, trigger, start, length, rate, channel);
        case 1:
            return this->granulator_03_grains2_next(buffer, trigger, start, length, rate, channel);
        case 2:
            return this->granulator_03_grains3_next(buffer, trigger, start, length, rate, channel);
        case 3:
            return this->granulator_03_grains4_next(buffer, trigger, start, length, rate, channel);
        case 4:
            return this->granulator_03_grains5_next(buffer, trigger, start, length, rate, channel);
        case 5:
            return this->granulator_03_grains6_next(buffer, trigger, start, length, rate, channel);
        case 6:
            return this->granulator_03_grains7_next(buffer, trigger, start, length, rate, channel);
        case 7:
            return this->granulator_03_grains8_next(buffer, trigger, start, length, rate, channel);
        case 8:
            return this->granulator_03_grains9_next(buffer, trigger, start, length, rate, channel);
        case 9:
            return this->granulator_03_grains10_next(buffer, trigger, start, length, rate, channel);
        case 10:
            return this->granulator_03_grains11_next(buffer, trigger, start, length, rate, channel);
        case 11:
            return this->granulator_03_grains12_next(buffer, trigger, start, length, rate, channel);
        case 12:
            return this->granulator_03_grains13_next(buffer, trigger, start, length, rate, channel);
        case 13:
            return this->granulator_03_grains14_next(buffer, trigger, start, length, rate, channel);
        case 14:
            return this->granulator_03_grains15_next(buffer, trigger, start, length, rate, channel);
        case 15:
            return this->granulator_03_grains16_next(buffer, trigger, start, length, rate, channel);
        case 16:
            return this->granulator_03_grains17_next(buffer, trigger, start, length, rate, channel);
        case 17:
            return this->granulator_03_grains18_next(buffer, trigger, start, length, rate, channel);
        case 18:
            return this->granulator_03_grains19_next(buffer, trigger, start, length, rate, channel);
        case 19:
            return this->granulator_03_grains20_next(buffer, trigger, start, length, rate, channel);
        case 20:
            return this->granulator_03_grains21_next(buffer, trigger, start, length, rate, channel);
        case 21:
            return this->granulator_03_grains22_next(buffer, trigger, start, length, rate, channel);
        case 22:
            return this->granulator_03_grains23_next(buffer, trigger, start, length, rate, channel);
        case 23:
            return this->granulator_03_grains24_next(buffer, trigger, start, length, rate, channel);
        case 24:
            return this->granulator_03_grains25_next(buffer, trigger, start, length, rate, channel);
        case 25:
            return this->granulator_03_grains26_next(buffer, trigger, start, length, rate, channel);
        case 26:
            return this->granulator_03_grains27_next(buffer, trigger, start, length, rate, channel);
        case 27:
            return this->granulator_03_grains28_next(buffer, trigger, start, length, rate, channel);
        case 28:
            return this->granulator_03_grains29_next(buffer, trigger, start, length, rate, channel);
        case 29:
            return this->granulator_03_grains30_next(buffer, trigger, start, length, rate, channel);
        case 30:
            return this->granulator_03_grains31_next(buffer, trigger, start, length, rate, channel);
        default:
            return this->granulator_03_grains32_next(buffer, trigger, start, length, rate, channel);
        }
    
        return 0;
    }
    
    void granulator_03_init() {
        this->granulator_03_edgedetect_init();
    
        for (Index i = 0; i < 32; i++) {
            for (Index j = 0; j < 2; j++) {
                this->granulator_03_grainPan[(Index)i][(Index)j] = 1;
            }
        }
    }
    
    void granulator_03_bufferUpdated() {
        number bufSr = this->granulator_03_buffer->getSampleRate();
        number bufSize = this->granulator_03_buffer->getSize();
    
        for (Index i = 0; i < 32; i++) {
            this->granulator_03_grains_reset(i);
        }
    
        {
            this->granulator_03_startMul = bufSize;
        }
    
        {
            this->granulator_03_lengthMul = bufSize;
        }
    }
    
    void granulator_03_onSampleRateChanged(number samplerate) {
        RNBO_UNUSED(samplerate);
        this->granulator_03_bufferUpdated();
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
    
    void param_18_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_18_value;
    }
    
    void param_18_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_18_value_set(preset["value"]);
    }
    
    void samplerate_03_onSampleRateChanged(number samplerate) {
        this->samplerate_03_samplerate_set(samplerate);
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
        dspexpr_17_in1 = 0;
        dspexpr_17_in2 = 0;
        granulator_03_start = 0;
        granulator_03_length = 2205;
        granulator_03_rate = 1;
        granulator_03_bchan = 0;
        granulator_03_limit = 32;
        granulator_03_panpos = 0.5;
        dspexpr_18_in1 = 0;
        dspexpr_18_in2 = 0;
        param_13_value = 0;
        dspexpr_19_in1 = 0;
        dspexpr_19_in2 = 0;
        dspexpr_20_in1 = 0;
        dspexpr_20_in2 = 0.5;
        dspexpr_21_in1 = 0;
        dspexpr_21_in2 = 0.5;
        dspexpr_22_in1 = 0;
        dspexpr_22_in2 = 0;
        param_14_value = 1;
        dspexpr_23_in1 = 0;
        dspexpr_23_in2 = 0;
        param_15_value = 1;
        dspexpr_24_in1 = 0;
        dspexpr_24_in2 = 0;
        param_16_value = 1;
        param_17_value = 0;
        expr_05_in1 = 0;
        expr_05_in2 = 0;
        expr_05_out1 = 0;
        param_18_value = 1;
        expr_06_in1 = 0;
        expr_06_in2 = 1000;
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
        signals[4] = nullptr;
        signals[5] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        granulator_03_startMul = 1;
        granulator_03_lengthMul = 1;
        granulator_03_limitLast = 0;
        granulator_03_edgedetect_active = false;
        granulator_03_grains1_sampleOffset = 0;
        granulator_03_grains1_curRate = 1;
        granulator_03_grains1_curStart = 0;
        granulator_03_grains1_curLen = 1;
        granulator_03_grains1_active = false;
        granulator_03_grains1_backwards = false;
        granulator_03_grains2_sampleOffset = 0;
        granulator_03_grains2_curRate = 1;
        granulator_03_grains2_curStart = 0;
        granulator_03_grains2_curLen = 1;
        granulator_03_grains2_active = false;
        granulator_03_grains2_backwards = false;
        granulator_03_grains3_sampleOffset = 0;
        granulator_03_grains3_curRate = 1;
        granulator_03_grains3_curStart = 0;
        granulator_03_grains3_curLen = 1;
        granulator_03_grains3_active = false;
        granulator_03_grains3_backwards = false;
        granulator_03_grains4_sampleOffset = 0;
        granulator_03_grains4_curRate = 1;
        granulator_03_grains4_curStart = 0;
        granulator_03_grains4_curLen = 1;
        granulator_03_grains4_active = false;
        granulator_03_grains4_backwards = false;
        granulator_03_grains5_sampleOffset = 0;
        granulator_03_grains5_curRate = 1;
        granulator_03_grains5_curStart = 0;
        granulator_03_grains5_curLen = 1;
        granulator_03_grains5_active = false;
        granulator_03_grains5_backwards = false;
        granulator_03_grains6_sampleOffset = 0;
        granulator_03_grains6_curRate = 1;
        granulator_03_grains6_curStart = 0;
        granulator_03_grains6_curLen = 1;
        granulator_03_grains6_active = false;
        granulator_03_grains6_backwards = false;
        granulator_03_grains7_sampleOffset = 0;
        granulator_03_grains7_curRate = 1;
        granulator_03_grains7_curStart = 0;
        granulator_03_grains7_curLen = 1;
        granulator_03_grains7_active = false;
        granulator_03_grains7_backwards = false;
        granulator_03_grains8_sampleOffset = 0;
        granulator_03_grains8_curRate = 1;
        granulator_03_grains8_curStart = 0;
        granulator_03_grains8_curLen = 1;
        granulator_03_grains8_active = false;
        granulator_03_grains8_backwards = false;
        granulator_03_grains9_sampleOffset = 0;
        granulator_03_grains9_curRate = 1;
        granulator_03_grains9_curStart = 0;
        granulator_03_grains9_curLen = 1;
        granulator_03_grains9_active = false;
        granulator_03_grains9_backwards = false;
        granulator_03_grains10_sampleOffset = 0;
        granulator_03_grains10_curRate = 1;
        granulator_03_grains10_curStart = 0;
        granulator_03_grains10_curLen = 1;
        granulator_03_grains10_active = false;
        granulator_03_grains10_backwards = false;
        granulator_03_grains11_sampleOffset = 0;
        granulator_03_grains11_curRate = 1;
        granulator_03_grains11_curStart = 0;
        granulator_03_grains11_curLen = 1;
        granulator_03_grains11_active = false;
        granulator_03_grains11_backwards = false;
        granulator_03_grains12_sampleOffset = 0;
        granulator_03_grains12_curRate = 1;
        granulator_03_grains12_curStart = 0;
        granulator_03_grains12_curLen = 1;
        granulator_03_grains12_active = false;
        granulator_03_grains12_backwards = false;
        granulator_03_grains13_sampleOffset = 0;
        granulator_03_grains13_curRate = 1;
        granulator_03_grains13_curStart = 0;
        granulator_03_grains13_curLen = 1;
        granulator_03_grains13_active = false;
        granulator_03_grains13_backwards = false;
        granulator_03_grains14_sampleOffset = 0;
        granulator_03_grains14_curRate = 1;
        granulator_03_grains14_curStart = 0;
        granulator_03_grains14_curLen = 1;
        granulator_03_grains14_active = false;
        granulator_03_grains14_backwards = false;
        granulator_03_grains15_sampleOffset = 0;
        granulator_03_grains15_curRate = 1;
        granulator_03_grains15_curStart = 0;
        granulator_03_grains15_curLen = 1;
        granulator_03_grains15_active = false;
        granulator_03_grains15_backwards = false;
        granulator_03_grains16_sampleOffset = 0;
        granulator_03_grains16_curRate = 1;
        granulator_03_grains16_curStart = 0;
        granulator_03_grains16_curLen = 1;
        granulator_03_grains16_active = false;
        granulator_03_grains16_backwards = false;
        granulator_03_grains17_sampleOffset = 0;
        granulator_03_grains17_curRate = 1;
        granulator_03_grains17_curStart = 0;
        granulator_03_grains17_curLen = 1;
        granulator_03_grains17_active = false;
        granulator_03_grains17_backwards = false;
        granulator_03_grains18_sampleOffset = 0;
        granulator_03_grains18_curRate = 1;
        granulator_03_grains18_curStart = 0;
        granulator_03_grains18_curLen = 1;
        granulator_03_grains18_active = false;
        granulator_03_grains18_backwards = false;
        granulator_03_grains19_sampleOffset = 0;
        granulator_03_grains19_curRate = 1;
        granulator_03_grains19_curStart = 0;
        granulator_03_grains19_curLen = 1;
        granulator_03_grains19_active = false;
        granulator_03_grains19_backwards = false;
        granulator_03_grains20_sampleOffset = 0;
        granulator_03_grains20_curRate = 1;
        granulator_03_grains20_curStart = 0;
        granulator_03_grains20_curLen = 1;
        granulator_03_grains20_active = false;
        granulator_03_grains20_backwards = false;
        granulator_03_grains21_sampleOffset = 0;
        granulator_03_grains21_curRate = 1;
        granulator_03_grains21_curStart = 0;
        granulator_03_grains21_curLen = 1;
        granulator_03_grains21_active = false;
        granulator_03_grains21_backwards = false;
        granulator_03_grains22_sampleOffset = 0;
        granulator_03_grains22_curRate = 1;
        granulator_03_grains22_curStart = 0;
        granulator_03_grains22_curLen = 1;
        granulator_03_grains22_active = false;
        granulator_03_grains22_backwards = false;
        granulator_03_grains23_sampleOffset = 0;
        granulator_03_grains23_curRate = 1;
        granulator_03_grains23_curStart = 0;
        granulator_03_grains23_curLen = 1;
        granulator_03_grains23_active = false;
        granulator_03_grains23_backwards = false;
        granulator_03_grains24_sampleOffset = 0;
        granulator_03_grains24_curRate = 1;
        granulator_03_grains24_curStart = 0;
        granulator_03_grains24_curLen = 1;
        granulator_03_grains24_active = false;
        granulator_03_grains24_backwards = false;
        granulator_03_grains25_sampleOffset = 0;
        granulator_03_grains25_curRate = 1;
        granulator_03_grains25_curStart = 0;
        granulator_03_grains25_curLen = 1;
        granulator_03_grains25_active = false;
        granulator_03_grains25_backwards = false;
        granulator_03_grains26_sampleOffset = 0;
        granulator_03_grains26_curRate = 1;
        granulator_03_grains26_curStart = 0;
        granulator_03_grains26_curLen = 1;
        granulator_03_grains26_active = false;
        granulator_03_grains26_backwards = false;
        granulator_03_grains27_sampleOffset = 0;
        granulator_03_grains27_curRate = 1;
        granulator_03_grains27_curStart = 0;
        granulator_03_grains27_curLen = 1;
        granulator_03_grains27_active = false;
        granulator_03_grains27_backwards = false;
        granulator_03_grains28_sampleOffset = 0;
        granulator_03_grains28_curRate = 1;
        granulator_03_grains28_curStart = 0;
        granulator_03_grains28_curLen = 1;
        granulator_03_grains28_active = false;
        granulator_03_grains28_backwards = false;
        granulator_03_grains29_sampleOffset = 0;
        granulator_03_grains29_curRate = 1;
        granulator_03_grains29_curStart = 0;
        granulator_03_grains29_curLen = 1;
        granulator_03_grains29_active = false;
        granulator_03_grains29_backwards = false;
        granulator_03_grains30_sampleOffset = 0;
        granulator_03_grains30_curRate = 1;
        granulator_03_grains30_curStart = 0;
        granulator_03_grains30_curLen = 1;
        granulator_03_grains30_active = false;
        granulator_03_grains30_backwards = false;
        granulator_03_grains31_sampleOffset = 0;
        granulator_03_grains31_curRate = 1;
        granulator_03_grains31_curStart = 0;
        granulator_03_grains31_curLen = 1;
        granulator_03_grains31_active = false;
        granulator_03_grains31_backwards = false;
        granulator_03_grains32_sampleOffset = 0;
        granulator_03_grains32_curRate = 1;
        granulator_03_grains32_curStart = 0;
        granulator_03_grains32_curLen = 1;
        granulator_03_grains32_active = false;
        granulator_03_grains32_backwards = false;
        param_13_lastValue = 0;
        param_14_lastValue = 0;
        param_15_lastValue = 0;
        param_16_lastValue = 0;
        param_17_lastValue = 0;
        param_18_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_17_in1;
        number dspexpr_17_in2;
        number granulator_03_start;
        number granulator_03_length;
        number granulator_03_rate;
        number granulator_03_bchan;
        number granulator_03_limit;
        number granulator_03_panpos;
        number dspexpr_18_in1;
        number dspexpr_18_in2;
        number param_13_value;
        number dspexpr_19_in1;
        number dspexpr_19_in2;
        number dspexpr_20_in1;
        number dspexpr_20_in2;
        number dspexpr_21_in1;
        number dspexpr_21_in2;
        number dspexpr_22_in1;
        number dspexpr_22_in2;
        number param_14_value;
        number dspexpr_23_in1;
        number dspexpr_23_in2;
        number param_15_value;
        number dspexpr_24_in1;
        number dspexpr_24_in2;
        number param_16_value;
        number param_17_value;
        number expr_05_in1;
        number expr_05_in2;
        number expr_05_out1;
        number param_18_value;
        number expr_06_in1;
        number expr_06_in2;
        number expr_06_out1;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[6];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef granulator_03_buffer;
        number granulator_03_grainPan[32][2] = { };
        number granulator_03_startMul;
        number granulator_03_lengthMul;
        int granulator_03_limitLast;
        bool granulator_03_edgedetect_active;
        number granulator_03_grains1_sampleOffset;
        number granulator_03_grains1_curRate;
        number granulator_03_grains1_curStart;
        number granulator_03_grains1_curLen;
        bool granulator_03_grains1_active;
        bool granulator_03_grains1_backwards;
        number granulator_03_grains2_sampleOffset;
        number granulator_03_grains2_curRate;
        number granulator_03_grains2_curStart;
        number granulator_03_grains2_curLen;
        bool granulator_03_grains2_active;
        bool granulator_03_grains2_backwards;
        number granulator_03_grains3_sampleOffset;
        number granulator_03_grains3_curRate;
        number granulator_03_grains3_curStart;
        number granulator_03_grains3_curLen;
        bool granulator_03_grains3_active;
        bool granulator_03_grains3_backwards;
        number granulator_03_grains4_sampleOffset;
        number granulator_03_grains4_curRate;
        number granulator_03_grains4_curStart;
        number granulator_03_grains4_curLen;
        bool granulator_03_grains4_active;
        bool granulator_03_grains4_backwards;
        number granulator_03_grains5_sampleOffset;
        number granulator_03_grains5_curRate;
        number granulator_03_grains5_curStart;
        number granulator_03_grains5_curLen;
        bool granulator_03_grains5_active;
        bool granulator_03_grains5_backwards;
        number granulator_03_grains6_sampleOffset;
        number granulator_03_grains6_curRate;
        number granulator_03_grains6_curStart;
        number granulator_03_grains6_curLen;
        bool granulator_03_grains6_active;
        bool granulator_03_grains6_backwards;
        number granulator_03_grains7_sampleOffset;
        number granulator_03_grains7_curRate;
        number granulator_03_grains7_curStart;
        number granulator_03_grains7_curLen;
        bool granulator_03_grains7_active;
        bool granulator_03_grains7_backwards;
        number granulator_03_grains8_sampleOffset;
        number granulator_03_grains8_curRate;
        number granulator_03_grains8_curStart;
        number granulator_03_grains8_curLen;
        bool granulator_03_grains8_active;
        bool granulator_03_grains8_backwards;
        number granulator_03_grains9_sampleOffset;
        number granulator_03_grains9_curRate;
        number granulator_03_grains9_curStart;
        number granulator_03_grains9_curLen;
        bool granulator_03_grains9_active;
        bool granulator_03_grains9_backwards;
        number granulator_03_grains10_sampleOffset;
        number granulator_03_grains10_curRate;
        number granulator_03_grains10_curStart;
        number granulator_03_grains10_curLen;
        bool granulator_03_grains10_active;
        bool granulator_03_grains10_backwards;
        number granulator_03_grains11_sampleOffset;
        number granulator_03_grains11_curRate;
        number granulator_03_grains11_curStart;
        number granulator_03_grains11_curLen;
        bool granulator_03_grains11_active;
        bool granulator_03_grains11_backwards;
        number granulator_03_grains12_sampleOffset;
        number granulator_03_grains12_curRate;
        number granulator_03_grains12_curStart;
        number granulator_03_grains12_curLen;
        bool granulator_03_grains12_active;
        bool granulator_03_grains12_backwards;
        number granulator_03_grains13_sampleOffset;
        number granulator_03_grains13_curRate;
        number granulator_03_grains13_curStart;
        number granulator_03_grains13_curLen;
        bool granulator_03_grains13_active;
        bool granulator_03_grains13_backwards;
        number granulator_03_grains14_sampleOffset;
        number granulator_03_grains14_curRate;
        number granulator_03_grains14_curStart;
        number granulator_03_grains14_curLen;
        bool granulator_03_grains14_active;
        bool granulator_03_grains14_backwards;
        number granulator_03_grains15_sampleOffset;
        number granulator_03_grains15_curRate;
        number granulator_03_grains15_curStart;
        number granulator_03_grains15_curLen;
        bool granulator_03_grains15_active;
        bool granulator_03_grains15_backwards;
        number granulator_03_grains16_sampleOffset;
        number granulator_03_grains16_curRate;
        number granulator_03_grains16_curStart;
        number granulator_03_grains16_curLen;
        bool granulator_03_grains16_active;
        bool granulator_03_grains16_backwards;
        number granulator_03_grains17_sampleOffset;
        number granulator_03_grains17_curRate;
        number granulator_03_grains17_curStart;
        number granulator_03_grains17_curLen;
        bool granulator_03_grains17_active;
        bool granulator_03_grains17_backwards;
        number granulator_03_grains18_sampleOffset;
        number granulator_03_grains18_curRate;
        number granulator_03_grains18_curStart;
        number granulator_03_grains18_curLen;
        bool granulator_03_grains18_active;
        bool granulator_03_grains18_backwards;
        number granulator_03_grains19_sampleOffset;
        number granulator_03_grains19_curRate;
        number granulator_03_grains19_curStart;
        number granulator_03_grains19_curLen;
        bool granulator_03_grains19_active;
        bool granulator_03_grains19_backwards;
        number granulator_03_grains20_sampleOffset;
        number granulator_03_grains20_curRate;
        number granulator_03_grains20_curStart;
        number granulator_03_grains20_curLen;
        bool granulator_03_grains20_active;
        bool granulator_03_grains20_backwards;
        number granulator_03_grains21_sampleOffset;
        number granulator_03_grains21_curRate;
        number granulator_03_grains21_curStart;
        number granulator_03_grains21_curLen;
        bool granulator_03_grains21_active;
        bool granulator_03_grains21_backwards;
        number granulator_03_grains22_sampleOffset;
        number granulator_03_grains22_curRate;
        number granulator_03_grains22_curStart;
        number granulator_03_grains22_curLen;
        bool granulator_03_grains22_active;
        bool granulator_03_grains22_backwards;
        number granulator_03_grains23_sampleOffset;
        number granulator_03_grains23_curRate;
        number granulator_03_grains23_curStart;
        number granulator_03_grains23_curLen;
        bool granulator_03_grains23_active;
        bool granulator_03_grains23_backwards;
        number granulator_03_grains24_sampleOffset;
        number granulator_03_grains24_curRate;
        number granulator_03_grains24_curStart;
        number granulator_03_grains24_curLen;
        bool granulator_03_grains24_active;
        bool granulator_03_grains24_backwards;
        number granulator_03_grains25_sampleOffset;
        number granulator_03_grains25_curRate;
        number granulator_03_grains25_curStart;
        number granulator_03_grains25_curLen;
        bool granulator_03_grains25_active;
        bool granulator_03_grains25_backwards;
        number granulator_03_grains26_sampleOffset;
        number granulator_03_grains26_curRate;
        number granulator_03_grains26_curStart;
        number granulator_03_grains26_curLen;
        bool granulator_03_grains26_active;
        bool granulator_03_grains26_backwards;
        number granulator_03_grains27_sampleOffset;
        number granulator_03_grains27_curRate;
        number granulator_03_grains27_curStart;
        number granulator_03_grains27_curLen;
        bool granulator_03_grains27_active;
        bool granulator_03_grains27_backwards;
        number granulator_03_grains28_sampleOffset;
        number granulator_03_grains28_curRate;
        number granulator_03_grains28_curStart;
        number granulator_03_grains28_curLen;
        bool granulator_03_grains28_active;
        bool granulator_03_grains28_backwards;
        number granulator_03_grains29_sampleOffset;
        number granulator_03_grains29_curRate;
        number granulator_03_grains29_curStart;
        number granulator_03_grains29_curLen;
        bool granulator_03_grains29_active;
        bool granulator_03_grains29_backwards;
        number granulator_03_grains30_sampleOffset;
        number granulator_03_grains30_curRate;
        number granulator_03_grains30_curStart;
        number granulator_03_grains30_curLen;
        bool granulator_03_grains30_active;
        bool granulator_03_grains30_backwards;
        number granulator_03_grains31_sampleOffset;
        number granulator_03_grains31_curRate;
        number granulator_03_grains31_curStart;
        number granulator_03_grains31_curLen;
        bool granulator_03_grains31_active;
        bool granulator_03_grains31_backwards;
        number granulator_03_grains32_sampleOffset;
        number granulator_03_grains32_curRate;
        number granulator_03_grains32_curStart;
        number granulator_03_grains32_curLen;
        bool granulator_03_grains32_active;
        bool granulator_03_grains32_backwards;
        number param_13_lastValue;
        number param_14_lastValue;
        number param_15_lastValue;
        number param_16_lastValue;
        number param_17_lastValue;
        number param_18_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_101 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_101()
    {
    }
    
    ~RNBOSubpatcher_101()
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
        getEngine()->flushClockEvents(this, -871642103, false);
    }
    
    inline number safemod(number f, number m) {
        if (m != 0) {
            if (m < 0) {
                m = -m;
            }
    
            if (f >= m) {
                if (f >= m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f -= m;
                }
            } else if (f <= -m) {
                if (f <= -m * 2.0) {
                    number d = f / m;
                    int i = (int)(rnbo_trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f += m;
                }
            }
        } else {
            f = 0.0;
        }
    
        return f;
    }
    
    inline number safesqrt(number num) {
        return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index bufferbindchannel(const Index channel, const Index maxChannels, const int channelmode) {
        if (channelmode == 0 || channelmode == 4) {
            return maxChannels - 1 - channel % maxChannels;
        } else if (channelmode == 3) {
            return (maxChannels == 0 ? 0 : channel % maxChannels);
        } else {
            return (channel > maxChannels - 1 ? maxChannels - 1 : (channel < 0 ? 0 : channel));
        }
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
    
    SampleIndex bufferbindindex(
        const SampleValue index,
        const SampleIndex start,
        const SampleIndex end,
        const int boundmode
    ) {
        if (boundmode == 0 || boundmode == 4) {
            if (index >= start && index < end)
                return index;
            else {
                SampleIndex length = (SampleIndex)(end - start);
                number index1 = index - start;
                number wrap = (index1 < 0 ? length * 2 - 1 + this->safemod(index1 + 1, length * 2) : this->safemod(index1, length * 2));
                return (start + wrap >= length ? length * 2 - wrap - 1 : wrap);
            }
        } else if (boundmode == 3) {
            return this->wrap(index, start, end);
        } else {
            return (index > end - 1 ? end - 1 : (index < start ? start : index));
        }
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
    
    template <typename T> inline SampleValue bufferreadsample(
        T& buffer,
        const SampleValue sampleIndex,
        const Index channel,
        const SampleIndex start,
        const SampleIndex end,
        const int interp
    ) {
        if (sampleIndex < 0.0) {
            return 0.0;
        }
    
        SampleIndex index1 = (SampleIndex)(rnbo_trunc(sampleIndex));
    
        if (interp == 0) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->linearinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else if (interp == 1) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->cubicinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 2) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            SampleIndex index3 = (SampleIndex)(index2 + 1);
    
            if (index3 > wrap)
                index3 = start;
    
            SampleIndex index4 = (SampleIndex)(index3 + 1);
    
            if (index4 > wrap)
                index4 = start;
    
            return this->splineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2),
                buffer->getSample(channel, index3),
                buffer->getSample(channel, index4)
            );
        } else if (interp == 3) {
            number frac = sampleIndex - index1;
            number wrap = end - 1;
            SampleIndex index2 = (SampleIndex)(index1 + 1);
    
            if (index2 > wrap)
                index2 = start;
    
            return this->cosineinterp(
                frac,
                buffer->getSample(channel, index1),
                buffer->getSample(channel, index2)
            );
        } else {
            return buffer->getSample(channel, index1);
        }
    }
    
    number hann(number x) {
        x = (x > 1 ? 1 : (x < 0 ? 0 : x));
        return 0.5 * (1 - rnbo_cos(6.28318530717958647692 * x));
    }
    
    ParameterIndex getParameterIndexForID(ConstCharPointer paramid) const {
        if (!stringCompare(paramid, "start")) {
            return 0;
        }
    
        if (!stringCompare(paramid, "len")) {
            return 1;
        }
    
        if (!stringCompare(paramid, "gain")) {
            return 2;
        }
    
        if (!stringCompare(paramid, "rate")) {
            return 3;
        }
    
        if (!stringCompare(paramid, "pan")) {
            return 4;
        }
    
        if (!stringCompare(paramid, "size")) {
            return 5;
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
        SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        this->dspexpr_26_perform(in2, this->dspexpr_26_in2, this->signals[0], n);
        this->dspexpr_30_perform(in3, this->dspexpr_30_in2, this->signals[1], n);
        this->dspexpr_31_perform(in4, this->dspexpr_31_in2, this->signals[2], n);
        this->dspexpr_32_perform(in5, this->dspexpr_32_in2, this->signals[3], n);
        this->dspexpr_29_perform(this->signals[3], this->dspexpr_29_in2, this->signals[4], n);
        this->dspexpr_28_perform(this->signals[4], this->dspexpr_28_in2, this->signals[3], n);
    
        this->granulator_04_perform(
            this->signals[0],
            this->signals[1],
            this->signals[2],
            this->granulator_04_bchan,
            this->signals[3],
            in1,
            this->signals[4],
            this->signals[5],
            n
        );
    
        this->dspexpr_25_perform(this->signals[4], this->dspexpr_25_in2, out1, n);
        this->dspexpr_27_perform(this->signals[5], this->dspexpr_27_in2, out2, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 6; i++) {
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
    
    void getPreset(PatcherStateInterface& preset) {
        this->param_19_getPresetValue(getSubState(preset, "start"));
        this->param_20_getPresetValue(getSubState(preset, "len"));
        this->param_21_getPresetValue(getSubState(preset, "gain"));
        this->param_22_getPresetValue(getSubState(preset, "rate"));
        this->param_23_getPresetValue(getSubState(preset, "pan"));
        this->param_24_getPresetValue(getSubState(preset, "size"));
    }
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double samplerate) {
        this->granulator_04_onSampleRateChanged(samplerate);
        this->samplerate_04_onSampleRateChanged(samplerate);
    }
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            this->param_19_value_set(v);
            break;
        case 1:
            this->param_20_value_set(v);
            break;
        case 2:
            this->param_21_value_set(v);
            break;
        case 3:
            this->param_22_value_set(v);
            break;
        case 4:
            this->param_23_value_set(v);
            break;
        case 5:
            this->param_24_value_set(v);
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
            return this->param_19_value;
        case 1:
            return this->param_20_value;
        case 2:
            return this->param_21_value;
        case 3:
            return this->param_22_value;
        case 4:
            return this->param_23_value;
        case 5:
            return this->param_24_value;
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
        return 6;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "start";
        case 1:
            return "len";
        case 2:
            return "gain";
        case 3:
            return "rate";
        case 4:
            return "pan";
        case 5:
            return "size";
        default:
            return "bogus";
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            return "layer4/start";
        case 1:
            return "layer4/len";
        case 2:
            return "layer4/gain";
        case 3:
            return "layer4/rate";
        case 4:
            return "layer4/pan";
        case 5:
            return "layer4/size";
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
                info->max = 1;
                info->exponent = 1;
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Start";
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
                info->steps = 101;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Length";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 2:
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
                info->displayName = "Gain";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 3:
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
                info->displayName = "Rate";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 4:
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = -1;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Pan";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
            case 5:
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = 0;
                info->max = 180;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = true;
                info->displayName = "Size";
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
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 10 ? 10 : value));
                ParameterValue normalizedValue = (value - 0) / (10 - 0);
    
                {
                    normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
                }
    
                return normalizedValue;
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 180 ? 180 : value));
                ParameterValue normalizedValue = (value - 0) / (180 - 0);
                return normalizedValue;
            }
        case 3:
            {
                value = (value < -4 ? -4 : (value > 4 ? 4 : value));
                ParameterValue normalizedValue = (value - -4) / (4 - -4);
                return normalizedValue;
            }
        case 4:
            {
                value = (value < -1 ? -1 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - -1) / (1 - -1);
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
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (1 - 0);
                }
            }
        case 2:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    value = this->applyStepsToNormalizedParameterValue(value, 101);
                }
    
                {
                    return 0 + value * (10 - 0);
                }
            }
        case 5:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (180 - 0);
                }
            }
        case 3:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -4 + value * (4 - -4);
                }
            }
        case 4:
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -1 + value * (1 - -1);
                }
            }
        default:
            return value;
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            return this->param_19_value_constrain(value);
        case 1:
            return this->param_20_value_constrain(value);
        case 2:
            return this->param_21_value_constrain(value);
        case 3:
            return this->param_22_value_constrain(value);
        case 4:
            return this->param_23_value_constrain(value);
        case 5:
            return this->param_24_value_constrain(value);
        default:
            return value;
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -871642103:
            this->loadbang_04_startupbang_bang();
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("startupbang"):
            if (TAG("layer4/loadbang_obj-42") == objectId)
                this->loadbang_04_startupbang_bang();
    
            break;
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("startupbang"):
            return "startupbang";
        case TAG("layer4/loadbang_obj-42"):
            return "layer4/loadbang_obj-42";
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
            this->granulator_04_buffer = new Float32Buffer(this->getPatcher()->layer4);
            this->granulator_04_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->granulator_04_buffer = new Float32Buffer(this->getPatcher()->layer4);
    }
    
    protected:
    
    void param_19_value_set(number v) {
        v = this->param_19_value_constrain(v);
        this->param_19_value = v;
        this->sendParameter(0, false);
    
        if (this->param_19_value != this->param_19_lastValue) {
            this->getEngine()->presetTouched();
            this->param_19_lastValue = this->param_19_value;
        }
    
        this->dspexpr_26_in2_set(v);
    }
    
    void param_20_value_set(number v) {
        v = this->param_20_value_constrain(v);
        this->param_20_value = v;
        this->sendParameter(1, false);
    
        if (this->param_20_value != this->param_20_lastValue) {
            this->getEngine()->presetTouched();
            this->param_20_lastValue = this->param_20_value;
        }
    
        this->dspexpr_30_in2_set(v);
    }
    
    void param_21_value_set(number v) {
        v = this->param_21_value_constrain(v);
        this->param_21_value = v;
        this->sendParameter(2, false);
    
        if (this->param_21_value != this->param_21_lastValue) {
            this->getEngine()->presetTouched();
            this->param_21_lastValue = this->param_21_value;
        }
    
        this->dspexpr_27_in2_set(v);
        this->dspexpr_25_in2_set(v);
    }
    
    void param_22_value_set(number v) {
        v = this->param_22_value_constrain(v);
        this->param_22_value = v;
        this->sendParameter(3, false);
    
        if (this->param_22_value != this->param_22_lastValue) {
            this->getEngine()->presetTouched();
            this->param_22_lastValue = this->param_22_value;
        }
    
        this->dspexpr_31_in2_set(v);
    }
    
    void param_23_value_set(number v) {
        v = this->param_23_value_constrain(v);
        this->param_23_value = v;
        this->sendParameter(4, false);
    
        if (this->param_23_value != this->param_23_lastValue) {
            this->getEngine()->presetTouched();
            this->param_23_lastValue = this->param_23_value;
        }
    
        this->dspexpr_32_in2_set(v);
    }
    
    void param_24_value_set(number v) {
        v = this->param_24_value_constrain(v);
        this->param_24_value = v;
        this->sendParameter(5, false);
    
        if (this->param_24_value != this->param_24_lastValue) {
            this->getEngine()->presetTouched();
            this->param_24_lastValue = this->param_24_value;
        }
    
        this->expr_07_in1_set(v);
    }
    
    void loadbang_04_startupbang_bang() {
        this->loadbang_04_output_bang();
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
        return 5;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->granulator_04_edgedetect_init();
        this->granulator_04_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    
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
    }
    
    void allocateDataRefs() {
        this->granulator_04_buffer = this->granulator_04_buffer->allocateIfNeeded();
    }
    
    static number param_19_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    
        {
            number oneStep = (number)1 / (number)100;
            number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
            number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
            v = numberOfSteps * oneStep;
        }
    
        return v;
    }
    
    void dspexpr_26_in2_set(number v) {
        this->dspexpr_26_in2 = v;
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
    
    void dspexpr_30_in2_set(number v) {
        this->dspexpr_30_in2 = v;
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
    
    void dspexpr_27_in2_set(number v) {
        this->dspexpr_27_in2 = v;
    }
    
    void dspexpr_25_in2_set(number v) {
        this->dspexpr_25_in2 = v;
    }
    
    static number param_22_value_constrain(number v) {
        v = (v > 4 ? 4 : (v < -4 ? -4 : v));
        return v;
    }
    
    void dspexpr_31_in2_set(number v) {
        this->dspexpr_31_in2 = v;
    }
    
    static number param_23_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < -1 ? -1 : v));
        return v;
    }
    
    void dspexpr_32_in2_set(number v) {
        this->dspexpr_32_in2 = v;
    }
    
    static number param_24_value_constrain(number v) {
        v = (v > 180 ? 180 : (v < 0 ? 0 : v));
        return v;
    }
    
    void eventoutlet_04_in1_number_set(number v) {
        this->getPatcher()->p_04_out3_number_set(v);
    }
    
    void expr_07_out1_set(number v) {
        this->expr_07_out1 = v;
        this->eventoutlet_04_in1_number_set(this->expr_07_out1);
    }
    
    void expr_07_in1_set(number in1) {
        this->expr_07_in1 = in1;
        this->expr_07_out1_set(this->expr_07_in1 * this->expr_07_in2);//#map:layer4/*_obj-43:1
    }
    
    void expr_07_in2_set(number v) {
        this->expr_07_in2 = v;
    }
    
    void expr_08_out1_set(number v) {
        this->expr_08_out1 = v;
        this->expr_07_in2_set(this->expr_08_out1);
    }
    
    void expr_08_in1_set(number in1) {
        this->expr_08_in1 = in1;
    
        this->expr_08_out1_set(
            (this->expr_08_in2 == 0 ? 0 : (this->expr_08_in2 == 0. ? 0. : this->expr_08_in1 / this->expr_08_in2))
        );//#map:layer4//_obj-44:1
    }
    
    void samplerate_04_samplerate_set(number v) {
        this->expr_08_in1_set(v);
    }
    
    void samplerate_04_input_bang() {
        this->samplerate_04_samplerate_set(this->samplerate());
    }
    
    void loadbang_04_output_bang() {
        this->samplerate_04_input_bang();
    }
    
    void dspexpr_26_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_30_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_31_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_32_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_29_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_28_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void granulator_04_perform(
        const Sample * start,
        const Sample * length,
        const Sample * rate,
        number bchan,
        const Sample * panpos,
        const Sample * trigger,
        Sample * out1,
        Sample * out2,
        Index n
    ) {
        RNBO_UNUSED(bchan);
        auto __granulator_04_grainPan = this->granulator_04_grainPan;
        auto __granulator_04_lengthMul = this->granulator_04_lengthMul;
        auto __granulator_04_startMul = this->granulator_04_startMul;
        auto __granulator_04_limitLast = this->granulator_04_limitLast;
        auto __granulator_04_limit = this->granulator_04_limit;
        number o = 0;
        Index i;
    
        for (i = 0; i < n; i++) {
            bool trig = (bool)(this->granulator_04_edgedetect_next(trigger[(Index)i]) > 0.0);
            int limit = (int)((__granulator_04_limit > 32 ? 32 : (__granulator_04_limit < 0 ? 0 : __granulator_04_limit)));
            __granulator_04_limitLast = limit;
            out1[(Index)i] = 0;
            out2[(Index)i] = 0;
            number s = start[(Index)i];
            number r = rate[(Index)i];
            number l = length[(Index)i];
    
            for (Index j = 0; j < 32; j++) {
                number thistrig = 0;
    
                if ((bool)(trig) && ((bool)(this->granulator_04_grains_inactive(j)) && j < limit)) {
                    trig = false;
                    thistrig = 1;
    
                    {
                        s *= __granulator_04_startMul;
                    }
    
                    {
                        l *= __granulator_04_lengthMul;
                    }
    
                    __granulator_04_grainPan[(Index)j][0] = this->granulator_04_panfunc_next(panpos[(Index)i], 0);
                    __granulator_04_grainPan[(Index)j][1] = this->granulator_04_panfunc_next(panpos[(Index)i], 1);
                }
    
                if (thistrig != 0 || (bool)(!(bool)(this->granulator_04_grains_inactive(j)))) {
                    array<number, 3> ret = this->granulator_04_grains_next(j, this->granulator_04_buffer, thistrig, s, l, r, 0);
                    number v = ret[0] * this->hann(ret[1]);
                    out1[(Index)i] += v * __granulator_04_grainPan[(Index)j][0];
                    out2[(Index)i] += v * __granulator_04_grainPan[(Index)j][1];
                }
            }
        }
    
        this->granulator_04_limitLast = __granulator_04_limitLast;
    }
    
    void dspexpr_25_perform(const Sample * in1, number in2, Sample * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_27_perform(const Sample * in1, number in2, Sample * out1, Index n) {
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
    
    number granulator_04_edgedetect_next(number val) {
        if ((0 == 0 && val <= 0) || (0 == 1 && val > 0)) {
            this->granulator_04_edgedetect_active = false;
        } else if ((bool)(!(bool)(this->granulator_04_edgedetect_active))) {
            this->granulator_04_edgedetect_active = true;
            return 1.0;
        }
    
        return 0.0;
    }
    
    void granulator_04_edgedetect_init() {}
    
    void granulator_04_edgedetect_reset() {
        this->granulator_04_edgedetect_active = false;
    }
    
    number granulator_04_panfunc_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    {
                        if (pos >= 0) {
                            pos = this->safemod(pos, 2);
                        } else {
                            pos = this->safemod(2 + this->safemod(pos, 2), 2);
                        }
    
                        if (channel == 0 && pos >= nchan_1)
                            pos = pos - 2;
                    }
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        {
                            return this->safesqrt(1.0 - rnbo_abs(pos));
                        }
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void granulator_04_panfunc_reset() {}
    
    bool granulator_04_grains1_inactive() {
        return !(bool)(this->granulator_04_grains1_active);
    }
    
    void granulator_04_grains1_reset() {
        this->granulator_04_grains1_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains1_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains1_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains1_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains1_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains1_curRate = 0;
                this->granulator_04_grains1_active = false;
            } else {
                this->granulator_04_grains1_curLen = length;
                this->granulator_04_grains1_active = true;
                this->granulator_04_grains1_backwards = rate < 0;
                this->granulator_04_grains1_sampleOffset = 0;
                this->granulator_04_grains1_curStart = start;
    
                if ((bool)(this->granulator_04_grains1_backwards)) {
                    this->granulator_04_grains1_curStart += this->granulator_04_grains1_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains1_active)) {
            if (this->granulator_04_grains1_sampleOffset >= this->granulator_04_grains1_curLen) {
                bangDone = true;
                this->granulator_04_grains1_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains1_curStart;
    
                if ((bool)(this->granulator_04_grains1_backwards)) {
                    sampleIndex -= this->granulator_04_grains1_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains1_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains1_sampleOffset / this->granulator_04_grains1_curLen > 1 ? 1 : (this->granulator_04_grains1_sampleOffset / this->granulator_04_grains1_curLen < 0 ? 0 : this->granulator_04_grains1_sampleOffset / this->granulator_04_grains1_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains1_sampleOffset += this->granulator_04_grains1_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains2_inactive() {
        return !(bool)(this->granulator_04_grains2_active);
    }
    
    void granulator_04_grains2_reset() {
        this->granulator_04_grains2_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains2_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains2_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains2_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains2_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains2_curRate = 0;
                this->granulator_04_grains2_active = false;
            } else {
                this->granulator_04_grains2_curLen = length;
                this->granulator_04_grains2_active = true;
                this->granulator_04_grains2_backwards = rate < 0;
                this->granulator_04_grains2_sampleOffset = 0;
                this->granulator_04_grains2_curStart = start;
    
                if ((bool)(this->granulator_04_grains2_backwards)) {
                    this->granulator_04_grains2_curStart += this->granulator_04_grains2_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains2_active)) {
            if (this->granulator_04_grains2_sampleOffset >= this->granulator_04_grains2_curLen) {
                bangDone = true;
                this->granulator_04_grains2_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains2_curStart;
    
                if ((bool)(this->granulator_04_grains2_backwards)) {
                    sampleIndex -= this->granulator_04_grains2_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains2_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains2_sampleOffset / this->granulator_04_grains2_curLen > 1 ? 1 : (this->granulator_04_grains2_sampleOffset / this->granulator_04_grains2_curLen < 0 ? 0 : this->granulator_04_grains2_sampleOffset / this->granulator_04_grains2_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains2_sampleOffset += this->granulator_04_grains2_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains3_inactive() {
        return !(bool)(this->granulator_04_grains3_active);
    }
    
    void granulator_04_grains3_reset() {
        this->granulator_04_grains3_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains3_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains3_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains3_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains3_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains3_curRate = 0;
                this->granulator_04_grains3_active = false;
            } else {
                this->granulator_04_grains3_curLen = length;
                this->granulator_04_grains3_active = true;
                this->granulator_04_grains3_backwards = rate < 0;
                this->granulator_04_grains3_sampleOffset = 0;
                this->granulator_04_grains3_curStart = start;
    
                if ((bool)(this->granulator_04_grains3_backwards)) {
                    this->granulator_04_grains3_curStart += this->granulator_04_grains3_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains3_active)) {
            if (this->granulator_04_grains3_sampleOffset >= this->granulator_04_grains3_curLen) {
                bangDone = true;
                this->granulator_04_grains3_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains3_curStart;
    
                if ((bool)(this->granulator_04_grains3_backwards)) {
                    sampleIndex -= this->granulator_04_grains3_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains3_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains3_sampleOffset / this->granulator_04_grains3_curLen > 1 ? 1 : (this->granulator_04_grains3_sampleOffset / this->granulator_04_grains3_curLen < 0 ? 0 : this->granulator_04_grains3_sampleOffset / this->granulator_04_grains3_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains3_sampleOffset += this->granulator_04_grains3_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains4_inactive() {
        return !(bool)(this->granulator_04_grains4_active);
    }
    
    void granulator_04_grains4_reset() {
        this->granulator_04_grains4_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains4_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains4_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains4_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains4_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains4_curRate = 0;
                this->granulator_04_grains4_active = false;
            } else {
                this->granulator_04_grains4_curLen = length;
                this->granulator_04_grains4_active = true;
                this->granulator_04_grains4_backwards = rate < 0;
                this->granulator_04_grains4_sampleOffset = 0;
                this->granulator_04_grains4_curStart = start;
    
                if ((bool)(this->granulator_04_grains4_backwards)) {
                    this->granulator_04_grains4_curStart += this->granulator_04_grains4_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains4_active)) {
            if (this->granulator_04_grains4_sampleOffset >= this->granulator_04_grains4_curLen) {
                bangDone = true;
                this->granulator_04_grains4_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains4_curStart;
    
                if ((bool)(this->granulator_04_grains4_backwards)) {
                    sampleIndex -= this->granulator_04_grains4_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains4_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains4_sampleOffset / this->granulator_04_grains4_curLen > 1 ? 1 : (this->granulator_04_grains4_sampleOffset / this->granulator_04_grains4_curLen < 0 ? 0 : this->granulator_04_grains4_sampleOffset / this->granulator_04_grains4_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains4_sampleOffset += this->granulator_04_grains4_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains5_inactive() {
        return !(bool)(this->granulator_04_grains5_active);
    }
    
    void granulator_04_grains5_reset() {
        this->granulator_04_grains5_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains5_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains5_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains5_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains5_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains5_curRate = 0;
                this->granulator_04_grains5_active = false;
            } else {
                this->granulator_04_grains5_curLen = length;
                this->granulator_04_grains5_active = true;
                this->granulator_04_grains5_backwards = rate < 0;
                this->granulator_04_grains5_sampleOffset = 0;
                this->granulator_04_grains5_curStart = start;
    
                if ((bool)(this->granulator_04_grains5_backwards)) {
                    this->granulator_04_grains5_curStart += this->granulator_04_grains5_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains5_active)) {
            if (this->granulator_04_grains5_sampleOffset >= this->granulator_04_grains5_curLen) {
                bangDone = true;
                this->granulator_04_grains5_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains5_curStart;
    
                if ((bool)(this->granulator_04_grains5_backwards)) {
                    sampleIndex -= this->granulator_04_grains5_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains5_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains5_sampleOffset / this->granulator_04_grains5_curLen > 1 ? 1 : (this->granulator_04_grains5_sampleOffset / this->granulator_04_grains5_curLen < 0 ? 0 : this->granulator_04_grains5_sampleOffset / this->granulator_04_grains5_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains5_sampleOffset += this->granulator_04_grains5_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains6_inactive() {
        return !(bool)(this->granulator_04_grains6_active);
    }
    
    void granulator_04_grains6_reset() {
        this->granulator_04_grains6_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains6_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains6_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains6_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains6_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains6_curRate = 0;
                this->granulator_04_grains6_active = false;
            } else {
                this->granulator_04_grains6_curLen = length;
                this->granulator_04_grains6_active = true;
                this->granulator_04_grains6_backwards = rate < 0;
                this->granulator_04_grains6_sampleOffset = 0;
                this->granulator_04_grains6_curStart = start;
    
                if ((bool)(this->granulator_04_grains6_backwards)) {
                    this->granulator_04_grains6_curStart += this->granulator_04_grains6_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains6_active)) {
            if (this->granulator_04_grains6_sampleOffset >= this->granulator_04_grains6_curLen) {
                bangDone = true;
                this->granulator_04_grains6_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains6_curStart;
    
                if ((bool)(this->granulator_04_grains6_backwards)) {
                    sampleIndex -= this->granulator_04_grains6_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains6_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains6_sampleOffset / this->granulator_04_grains6_curLen > 1 ? 1 : (this->granulator_04_grains6_sampleOffset / this->granulator_04_grains6_curLen < 0 ? 0 : this->granulator_04_grains6_sampleOffset / this->granulator_04_grains6_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains6_sampleOffset += this->granulator_04_grains6_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains7_inactive() {
        return !(bool)(this->granulator_04_grains7_active);
    }
    
    void granulator_04_grains7_reset() {
        this->granulator_04_grains7_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains7_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains7_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains7_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains7_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains7_curRate = 0;
                this->granulator_04_grains7_active = false;
            } else {
                this->granulator_04_grains7_curLen = length;
                this->granulator_04_grains7_active = true;
                this->granulator_04_grains7_backwards = rate < 0;
                this->granulator_04_grains7_sampleOffset = 0;
                this->granulator_04_grains7_curStart = start;
    
                if ((bool)(this->granulator_04_grains7_backwards)) {
                    this->granulator_04_grains7_curStart += this->granulator_04_grains7_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains7_active)) {
            if (this->granulator_04_grains7_sampleOffset >= this->granulator_04_grains7_curLen) {
                bangDone = true;
                this->granulator_04_grains7_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains7_curStart;
    
                if ((bool)(this->granulator_04_grains7_backwards)) {
                    sampleIndex -= this->granulator_04_grains7_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains7_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains7_sampleOffset / this->granulator_04_grains7_curLen > 1 ? 1 : (this->granulator_04_grains7_sampleOffset / this->granulator_04_grains7_curLen < 0 ? 0 : this->granulator_04_grains7_sampleOffset / this->granulator_04_grains7_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains7_sampleOffset += this->granulator_04_grains7_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains8_inactive() {
        return !(bool)(this->granulator_04_grains8_active);
    }
    
    void granulator_04_grains8_reset() {
        this->granulator_04_grains8_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains8_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains8_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains8_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains8_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains8_curRate = 0;
                this->granulator_04_grains8_active = false;
            } else {
                this->granulator_04_grains8_curLen = length;
                this->granulator_04_grains8_active = true;
                this->granulator_04_grains8_backwards = rate < 0;
                this->granulator_04_grains8_sampleOffset = 0;
                this->granulator_04_grains8_curStart = start;
    
                if ((bool)(this->granulator_04_grains8_backwards)) {
                    this->granulator_04_grains8_curStart += this->granulator_04_grains8_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains8_active)) {
            if (this->granulator_04_grains8_sampleOffset >= this->granulator_04_grains8_curLen) {
                bangDone = true;
                this->granulator_04_grains8_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains8_curStart;
    
                if ((bool)(this->granulator_04_grains8_backwards)) {
                    sampleIndex -= this->granulator_04_grains8_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains8_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains8_sampleOffset / this->granulator_04_grains8_curLen > 1 ? 1 : (this->granulator_04_grains8_sampleOffset / this->granulator_04_grains8_curLen < 0 ? 0 : this->granulator_04_grains8_sampleOffset / this->granulator_04_grains8_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains8_sampleOffset += this->granulator_04_grains8_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains9_inactive() {
        return !(bool)(this->granulator_04_grains9_active);
    }
    
    void granulator_04_grains9_reset() {
        this->granulator_04_grains9_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains9_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains9_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains9_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains9_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains9_curRate = 0;
                this->granulator_04_grains9_active = false;
            } else {
                this->granulator_04_grains9_curLen = length;
                this->granulator_04_grains9_active = true;
                this->granulator_04_grains9_backwards = rate < 0;
                this->granulator_04_grains9_sampleOffset = 0;
                this->granulator_04_grains9_curStart = start;
    
                if ((bool)(this->granulator_04_grains9_backwards)) {
                    this->granulator_04_grains9_curStart += this->granulator_04_grains9_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains9_active)) {
            if (this->granulator_04_grains9_sampleOffset >= this->granulator_04_grains9_curLen) {
                bangDone = true;
                this->granulator_04_grains9_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains9_curStart;
    
                if ((bool)(this->granulator_04_grains9_backwards)) {
                    sampleIndex -= this->granulator_04_grains9_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains9_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains9_sampleOffset / this->granulator_04_grains9_curLen > 1 ? 1 : (this->granulator_04_grains9_sampleOffset / this->granulator_04_grains9_curLen < 0 ? 0 : this->granulator_04_grains9_sampleOffset / this->granulator_04_grains9_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains9_sampleOffset += this->granulator_04_grains9_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains10_inactive() {
        return !(bool)(this->granulator_04_grains10_active);
    }
    
    void granulator_04_grains10_reset() {
        this->granulator_04_grains10_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains10_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains10_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains10_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains10_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains10_curRate = 0;
                this->granulator_04_grains10_active = false;
            } else {
                this->granulator_04_grains10_curLen = length;
                this->granulator_04_grains10_active = true;
                this->granulator_04_grains10_backwards = rate < 0;
                this->granulator_04_grains10_sampleOffset = 0;
                this->granulator_04_grains10_curStart = start;
    
                if ((bool)(this->granulator_04_grains10_backwards)) {
                    this->granulator_04_grains10_curStart += this->granulator_04_grains10_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains10_active)) {
            if (this->granulator_04_grains10_sampleOffset >= this->granulator_04_grains10_curLen) {
                bangDone = true;
                this->granulator_04_grains10_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains10_curStart;
    
                if ((bool)(this->granulator_04_grains10_backwards)) {
                    sampleIndex -= this->granulator_04_grains10_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains10_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains10_sampleOffset / this->granulator_04_grains10_curLen > 1 ? 1 : (this->granulator_04_grains10_sampleOffset / this->granulator_04_grains10_curLen < 0 ? 0 : this->granulator_04_grains10_sampleOffset / this->granulator_04_grains10_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains10_sampleOffset += this->granulator_04_grains10_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains11_inactive() {
        return !(bool)(this->granulator_04_grains11_active);
    }
    
    void granulator_04_grains11_reset() {
        this->granulator_04_grains11_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains11_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains11_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains11_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains11_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains11_curRate = 0;
                this->granulator_04_grains11_active = false;
            } else {
                this->granulator_04_grains11_curLen = length;
                this->granulator_04_grains11_active = true;
                this->granulator_04_grains11_backwards = rate < 0;
                this->granulator_04_grains11_sampleOffset = 0;
                this->granulator_04_grains11_curStart = start;
    
                if ((bool)(this->granulator_04_grains11_backwards)) {
                    this->granulator_04_grains11_curStart += this->granulator_04_grains11_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains11_active)) {
            if (this->granulator_04_grains11_sampleOffset >= this->granulator_04_grains11_curLen) {
                bangDone = true;
                this->granulator_04_grains11_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains11_curStart;
    
                if ((bool)(this->granulator_04_grains11_backwards)) {
                    sampleIndex -= this->granulator_04_grains11_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains11_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains11_sampleOffset / this->granulator_04_grains11_curLen > 1 ? 1 : (this->granulator_04_grains11_sampleOffset / this->granulator_04_grains11_curLen < 0 ? 0 : this->granulator_04_grains11_sampleOffset / this->granulator_04_grains11_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains11_sampleOffset += this->granulator_04_grains11_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains12_inactive() {
        return !(bool)(this->granulator_04_grains12_active);
    }
    
    void granulator_04_grains12_reset() {
        this->granulator_04_grains12_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains12_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains12_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains12_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains12_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains12_curRate = 0;
                this->granulator_04_grains12_active = false;
            } else {
                this->granulator_04_grains12_curLen = length;
                this->granulator_04_grains12_active = true;
                this->granulator_04_grains12_backwards = rate < 0;
                this->granulator_04_grains12_sampleOffset = 0;
                this->granulator_04_grains12_curStart = start;
    
                if ((bool)(this->granulator_04_grains12_backwards)) {
                    this->granulator_04_grains12_curStart += this->granulator_04_grains12_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains12_active)) {
            if (this->granulator_04_grains12_sampleOffset >= this->granulator_04_grains12_curLen) {
                bangDone = true;
                this->granulator_04_grains12_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains12_curStart;
    
                if ((bool)(this->granulator_04_grains12_backwards)) {
                    sampleIndex -= this->granulator_04_grains12_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains12_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains12_sampleOffset / this->granulator_04_grains12_curLen > 1 ? 1 : (this->granulator_04_grains12_sampleOffset / this->granulator_04_grains12_curLen < 0 ? 0 : this->granulator_04_grains12_sampleOffset / this->granulator_04_grains12_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains12_sampleOffset += this->granulator_04_grains12_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains13_inactive() {
        return !(bool)(this->granulator_04_grains13_active);
    }
    
    void granulator_04_grains13_reset() {
        this->granulator_04_grains13_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains13_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains13_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains13_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains13_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains13_curRate = 0;
                this->granulator_04_grains13_active = false;
            } else {
                this->granulator_04_grains13_curLen = length;
                this->granulator_04_grains13_active = true;
                this->granulator_04_grains13_backwards = rate < 0;
                this->granulator_04_grains13_sampleOffset = 0;
                this->granulator_04_grains13_curStart = start;
    
                if ((bool)(this->granulator_04_grains13_backwards)) {
                    this->granulator_04_grains13_curStart += this->granulator_04_grains13_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains13_active)) {
            if (this->granulator_04_grains13_sampleOffset >= this->granulator_04_grains13_curLen) {
                bangDone = true;
                this->granulator_04_grains13_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains13_curStart;
    
                if ((bool)(this->granulator_04_grains13_backwards)) {
                    sampleIndex -= this->granulator_04_grains13_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains13_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains13_sampleOffset / this->granulator_04_grains13_curLen > 1 ? 1 : (this->granulator_04_grains13_sampleOffset / this->granulator_04_grains13_curLen < 0 ? 0 : this->granulator_04_grains13_sampleOffset / this->granulator_04_grains13_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains13_sampleOffset += this->granulator_04_grains13_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains14_inactive() {
        return !(bool)(this->granulator_04_grains14_active);
    }
    
    void granulator_04_grains14_reset() {
        this->granulator_04_grains14_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains14_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains14_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains14_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains14_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains14_curRate = 0;
                this->granulator_04_grains14_active = false;
            } else {
                this->granulator_04_grains14_curLen = length;
                this->granulator_04_grains14_active = true;
                this->granulator_04_grains14_backwards = rate < 0;
                this->granulator_04_grains14_sampleOffset = 0;
                this->granulator_04_grains14_curStart = start;
    
                if ((bool)(this->granulator_04_grains14_backwards)) {
                    this->granulator_04_grains14_curStart += this->granulator_04_grains14_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains14_active)) {
            if (this->granulator_04_grains14_sampleOffset >= this->granulator_04_grains14_curLen) {
                bangDone = true;
                this->granulator_04_grains14_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains14_curStart;
    
                if ((bool)(this->granulator_04_grains14_backwards)) {
                    sampleIndex -= this->granulator_04_grains14_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains14_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains14_sampleOffset / this->granulator_04_grains14_curLen > 1 ? 1 : (this->granulator_04_grains14_sampleOffset / this->granulator_04_grains14_curLen < 0 ? 0 : this->granulator_04_grains14_sampleOffset / this->granulator_04_grains14_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains14_sampleOffset += this->granulator_04_grains14_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains15_inactive() {
        return !(bool)(this->granulator_04_grains15_active);
    }
    
    void granulator_04_grains15_reset() {
        this->granulator_04_grains15_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains15_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains15_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains15_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains15_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains15_curRate = 0;
                this->granulator_04_grains15_active = false;
            } else {
                this->granulator_04_grains15_curLen = length;
                this->granulator_04_grains15_active = true;
                this->granulator_04_grains15_backwards = rate < 0;
                this->granulator_04_grains15_sampleOffset = 0;
                this->granulator_04_grains15_curStart = start;
    
                if ((bool)(this->granulator_04_grains15_backwards)) {
                    this->granulator_04_grains15_curStart += this->granulator_04_grains15_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains15_active)) {
            if (this->granulator_04_grains15_sampleOffset >= this->granulator_04_grains15_curLen) {
                bangDone = true;
                this->granulator_04_grains15_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains15_curStart;
    
                if ((bool)(this->granulator_04_grains15_backwards)) {
                    sampleIndex -= this->granulator_04_grains15_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains15_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains15_sampleOffset / this->granulator_04_grains15_curLen > 1 ? 1 : (this->granulator_04_grains15_sampleOffset / this->granulator_04_grains15_curLen < 0 ? 0 : this->granulator_04_grains15_sampleOffset / this->granulator_04_grains15_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains15_sampleOffset += this->granulator_04_grains15_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains16_inactive() {
        return !(bool)(this->granulator_04_grains16_active);
    }
    
    void granulator_04_grains16_reset() {
        this->granulator_04_grains16_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains16_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains16_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains16_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains16_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains16_curRate = 0;
                this->granulator_04_grains16_active = false;
            } else {
                this->granulator_04_grains16_curLen = length;
                this->granulator_04_grains16_active = true;
                this->granulator_04_grains16_backwards = rate < 0;
                this->granulator_04_grains16_sampleOffset = 0;
                this->granulator_04_grains16_curStart = start;
    
                if ((bool)(this->granulator_04_grains16_backwards)) {
                    this->granulator_04_grains16_curStart += this->granulator_04_grains16_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains16_active)) {
            if (this->granulator_04_grains16_sampleOffset >= this->granulator_04_grains16_curLen) {
                bangDone = true;
                this->granulator_04_grains16_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains16_curStart;
    
                if ((bool)(this->granulator_04_grains16_backwards)) {
                    sampleIndex -= this->granulator_04_grains16_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains16_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains16_sampleOffset / this->granulator_04_grains16_curLen > 1 ? 1 : (this->granulator_04_grains16_sampleOffset / this->granulator_04_grains16_curLen < 0 ? 0 : this->granulator_04_grains16_sampleOffset / this->granulator_04_grains16_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains16_sampleOffset += this->granulator_04_grains16_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains17_inactive() {
        return !(bool)(this->granulator_04_grains17_active);
    }
    
    void granulator_04_grains17_reset() {
        this->granulator_04_grains17_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains17_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains17_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains17_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains17_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains17_curRate = 0;
                this->granulator_04_grains17_active = false;
            } else {
                this->granulator_04_grains17_curLen = length;
                this->granulator_04_grains17_active = true;
                this->granulator_04_grains17_backwards = rate < 0;
                this->granulator_04_grains17_sampleOffset = 0;
                this->granulator_04_grains17_curStart = start;
    
                if ((bool)(this->granulator_04_grains17_backwards)) {
                    this->granulator_04_grains17_curStart += this->granulator_04_grains17_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains17_active)) {
            if (this->granulator_04_grains17_sampleOffset >= this->granulator_04_grains17_curLen) {
                bangDone = true;
                this->granulator_04_grains17_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains17_curStart;
    
                if ((bool)(this->granulator_04_grains17_backwards)) {
                    sampleIndex -= this->granulator_04_grains17_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains17_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains17_sampleOffset / this->granulator_04_grains17_curLen > 1 ? 1 : (this->granulator_04_grains17_sampleOffset / this->granulator_04_grains17_curLen < 0 ? 0 : this->granulator_04_grains17_sampleOffset / this->granulator_04_grains17_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains17_sampleOffset += this->granulator_04_grains17_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains18_inactive() {
        return !(bool)(this->granulator_04_grains18_active);
    }
    
    void granulator_04_grains18_reset() {
        this->granulator_04_grains18_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains18_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains18_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains18_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains18_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains18_curRate = 0;
                this->granulator_04_grains18_active = false;
            } else {
                this->granulator_04_grains18_curLen = length;
                this->granulator_04_grains18_active = true;
                this->granulator_04_grains18_backwards = rate < 0;
                this->granulator_04_grains18_sampleOffset = 0;
                this->granulator_04_grains18_curStart = start;
    
                if ((bool)(this->granulator_04_grains18_backwards)) {
                    this->granulator_04_grains18_curStart += this->granulator_04_grains18_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains18_active)) {
            if (this->granulator_04_grains18_sampleOffset >= this->granulator_04_grains18_curLen) {
                bangDone = true;
                this->granulator_04_grains18_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains18_curStart;
    
                if ((bool)(this->granulator_04_grains18_backwards)) {
                    sampleIndex -= this->granulator_04_grains18_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains18_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains18_sampleOffset / this->granulator_04_grains18_curLen > 1 ? 1 : (this->granulator_04_grains18_sampleOffset / this->granulator_04_grains18_curLen < 0 ? 0 : this->granulator_04_grains18_sampleOffset / this->granulator_04_grains18_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains18_sampleOffset += this->granulator_04_grains18_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains19_inactive() {
        return !(bool)(this->granulator_04_grains19_active);
    }
    
    void granulator_04_grains19_reset() {
        this->granulator_04_grains19_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains19_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains19_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains19_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains19_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains19_curRate = 0;
                this->granulator_04_grains19_active = false;
            } else {
                this->granulator_04_grains19_curLen = length;
                this->granulator_04_grains19_active = true;
                this->granulator_04_grains19_backwards = rate < 0;
                this->granulator_04_grains19_sampleOffset = 0;
                this->granulator_04_grains19_curStart = start;
    
                if ((bool)(this->granulator_04_grains19_backwards)) {
                    this->granulator_04_grains19_curStart += this->granulator_04_grains19_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains19_active)) {
            if (this->granulator_04_grains19_sampleOffset >= this->granulator_04_grains19_curLen) {
                bangDone = true;
                this->granulator_04_grains19_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains19_curStart;
    
                if ((bool)(this->granulator_04_grains19_backwards)) {
                    sampleIndex -= this->granulator_04_grains19_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains19_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains19_sampleOffset / this->granulator_04_grains19_curLen > 1 ? 1 : (this->granulator_04_grains19_sampleOffset / this->granulator_04_grains19_curLen < 0 ? 0 : this->granulator_04_grains19_sampleOffset / this->granulator_04_grains19_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains19_sampleOffset += this->granulator_04_grains19_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains20_inactive() {
        return !(bool)(this->granulator_04_grains20_active);
    }
    
    void granulator_04_grains20_reset() {
        this->granulator_04_grains20_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains20_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains20_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains20_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains20_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains20_curRate = 0;
                this->granulator_04_grains20_active = false;
            } else {
                this->granulator_04_grains20_curLen = length;
                this->granulator_04_grains20_active = true;
                this->granulator_04_grains20_backwards = rate < 0;
                this->granulator_04_grains20_sampleOffset = 0;
                this->granulator_04_grains20_curStart = start;
    
                if ((bool)(this->granulator_04_grains20_backwards)) {
                    this->granulator_04_grains20_curStart += this->granulator_04_grains20_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains20_active)) {
            if (this->granulator_04_grains20_sampleOffset >= this->granulator_04_grains20_curLen) {
                bangDone = true;
                this->granulator_04_grains20_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains20_curStart;
    
                if ((bool)(this->granulator_04_grains20_backwards)) {
                    sampleIndex -= this->granulator_04_grains20_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains20_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains20_sampleOffset / this->granulator_04_grains20_curLen > 1 ? 1 : (this->granulator_04_grains20_sampleOffset / this->granulator_04_grains20_curLen < 0 ? 0 : this->granulator_04_grains20_sampleOffset / this->granulator_04_grains20_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains20_sampleOffset += this->granulator_04_grains20_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains21_inactive() {
        return !(bool)(this->granulator_04_grains21_active);
    }
    
    void granulator_04_grains21_reset() {
        this->granulator_04_grains21_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains21_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains21_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains21_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains21_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains21_curRate = 0;
                this->granulator_04_grains21_active = false;
            } else {
                this->granulator_04_grains21_curLen = length;
                this->granulator_04_grains21_active = true;
                this->granulator_04_grains21_backwards = rate < 0;
                this->granulator_04_grains21_sampleOffset = 0;
                this->granulator_04_grains21_curStart = start;
    
                if ((bool)(this->granulator_04_grains21_backwards)) {
                    this->granulator_04_grains21_curStart += this->granulator_04_grains21_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains21_active)) {
            if (this->granulator_04_grains21_sampleOffset >= this->granulator_04_grains21_curLen) {
                bangDone = true;
                this->granulator_04_grains21_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains21_curStart;
    
                if ((bool)(this->granulator_04_grains21_backwards)) {
                    sampleIndex -= this->granulator_04_grains21_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains21_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains21_sampleOffset / this->granulator_04_grains21_curLen > 1 ? 1 : (this->granulator_04_grains21_sampleOffset / this->granulator_04_grains21_curLen < 0 ? 0 : this->granulator_04_grains21_sampleOffset / this->granulator_04_grains21_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains21_sampleOffset += this->granulator_04_grains21_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains22_inactive() {
        return !(bool)(this->granulator_04_grains22_active);
    }
    
    void granulator_04_grains22_reset() {
        this->granulator_04_grains22_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains22_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains22_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains22_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains22_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains22_curRate = 0;
                this->granulator_04_grains22_active = false;
            } else {
                this->granulator_04_grains22_curLen = length;
                this->granulator_04_grains22_active = true;
                this->granulator_04_grains22_backwards = rate < 0;
                this->granulator_04_grains22_sampleOffset = 0;
                this->granulator_04_grains22_curStart = start;
    
                if ((bool)(this->granulator_04_grains22_backwards)) {
                    this->granulator_04_grains22_curStart += this->granulator_04_grains22_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains22_active)) {
            if (this->granulator_04_grains22_sampleOffset >= this->granulator_04_grains22_curLen) {
                bangDone = true;
                this->granulator_04_grains22_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains22_curStart;
    
                if ((bool)(this->granulator_04_grains22_backwards)) {
                    sampleIndex -= this->granulator_04_grains22_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains22_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains22_sampleOffset / this->granulator_04_grains22_curLen > 1 ? 1 : (this->granulator_04_grains22_sampleOffset / this->granulator_04_grains22_curLen < 0 ? 0 : this->granulator_04_grains22_sampleOffset / this->granulator_04_grains22_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains22_sampleOffset += this->granulator_04_grains22_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains23_inactive() {
        return !(bool)(this->granulator_04_grains23_active);
    }
    
    void granulator_04_grains23_reset() {
        this->granulator_04_grains23_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains23_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains23_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains23_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains23_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains23_curRate = 0;
                this->granulator_04_grains23_active = false;
            } else {
                this->granulator_04_grains23_curLen = length;
                this->granulator_04_grains23_active = true;
                this->granulator_04_grains23_backwards = rate < 0;
                this->granulator_04_grains23_sampleOffset = 0;
                this->granulator_04_grains23_curStart = start;
    
                if ((bool)(this->granulator_04_grains23_backwards)) {
                    this->granulator_04_grains23_curStart += this->granulator_04_grains23_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains23_active)) {
            if (this->granulator_04_grains23_sampleOffset >= this->granulator_04_grains23_curLen) {
                bangDone = true;
                this->granulator_04_grains23_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains23_curStart;
    
                if ((bool)(this->granulator_04_grains23_backwards)) {
                    sampleIndex -= this->granulator_04_grains23_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains23_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains23_sampleOffset / this->granulator_04_grains23_curLen > 1 ? 1 : (this->granulator_04_grains23_sampleOffset / this->granulator_04_grains23_curLen < 0 ? 0 : this->granulator_04_grains23_sampleOffset / this->granulator_04_grains23_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains23_sampleOffset += this->granulator_04_grains23_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains24_inactive() {
        return !(bool)(this->granulator_04_grains24_active);
    }
    
    void granulator_04_grains24_reset() {
        this->granulator_04_grains24_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains24_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains24_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains24_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains24_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains24_curRate = 0;
                this->granulator_04_grains24_active = false;
            } else {
                this->granulator_04_grains24_curLen = length;
                this->granulator_04_grains24_active = true;
                this->granulator_04_grains24_backwards = rate < 0;
                this->granulator_04_grains24_sampleOffset = 0;
                this->granulator_04_grains24_curStart = start;
    
                if ((bool)(this->granulator_04_grains24_backwards)) {
                    this->granulator_04_grains24_curStart += this->granulator_04_grains24_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains24_active)) {
            if (this->granulator_04_grains24_sampleOffset >= this->granulator_04_grains24_curLen) {
                bangDone = true;
                this->granulator_04_grains24_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains24_curStart;
    
                if ((bool)(this->granulator_04_grains24_backwards)) {
                    sampleIndex -= this->granulator_04_grains24_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains24_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains24_sampleOffset / this->granulator_04_grains24_curLen > 1 ? 1 : (this->granulator_04_grains24_sampleOffset / this->granulator_04_grains24_curLen < 0 ? 0 : this->granulator_04_grains24_sampleOffset / this->granulator_04_grains24_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains24_sampleOffset += this->granulator_04_grains24_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains25_inactive() {
        return !(bool)(this->granulator_04_grains25_active);
    }
    
    void granulator_04_grains25_reset() {
        this->granulator_04_grains25_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains25_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains25_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains25_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains25_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains25_curRate = 0;
                this->granulator_04_grains25_active = false;
            } else {
                this->granulator_04_grains25_curLen = length;
                this->granulator_04_grains25_active = true;
                this->granulator_04_grains25_backwards = rate < 0;
                this->granulator_04_grains25_sampleOffset = 0;
                this->granulator_04_grains25_curStart = start;
    
                if ((bool)(this->granulator_04_grains25_backwards)) {
                    this->granulator_04_grains25_curStart += this->granulator_04_grains25_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains25_active)) {
            if (this->granulator_04_grains25_sampleOffset >= this->granulator_04_grains25_curLen) {
                bangDone = true;
                this->granulator_04_grains25_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains25_curStart;
    
                if ((bool)(this->granulator_04_grains25_backwards)) {
                    sampleIndex -= this->granulator_04_grains25_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains25_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains25_sampleOffset / this->granulator_04_grains25_curLen > 1 ? 1 : (this->granulator_04_grains25_sampleOffset / this->granulator_04_grains25_curLen < 0 ? 0 : this->granulator_04_grains25_sampleOffset / this->granulator_04_grains25_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains25_sampleOffset += this->granulator_04_grains25_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains26_inactive() {
        return !(bool)(this->granulator_04_grains26_active);
    }
    
    void granulator_04_grains26_reset() {
        this->granulator_04_grains26_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains26_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains26_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains26_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains26_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains26_curRate = 0;
                this->granulator_04_grains26_active = false;
            } else {
                this->granulator_04_grains26_curLen = length;
                this->granulator_04_grains26_active = true;
                this->granulator_04_grains26_backwards = rate < 0;
                this->granulator_04_grains26_sampleOffset = 0;
                this->granulator_04_grains26_curStart = start;
    
                if ((bool)(this->granulator_04_grains26_backwards)) {
                    this->granulator_04_grains26_curStart += this->granulator_04_grains26_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains26_active)) {
            if (this->granulator_04_grains26_sampleOffset >= this->granulator_04_grains26_curLen) {
                bangDone = true;
                this->granulator_04_grains26_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains26_curStart;
    
                if ((bool)(this->granulator_04_grains26_backwards)) {
                    sampleIndex -= this->granulator_04_grains26_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains26_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains26_sampleOffset / this->granulator_04_grains26_curLen > 1 ? 1 : (this->granulator_04_grains26_sampleOffset / this->granulator_04_grains26_curLen < 0 ? 0 : this->granulator_04_grains26_sampleOffset / this->granulator_04_grains26_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains26_sampleOffset += this->granulator_04_grains26_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains27_inactive() {
        return !(bool)(this->granulator_04_grains27_active);
    }
    
    void granulator_04_grains27_reset() {
        this->granulator_04_grains27_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains27_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains27_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains27_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains27_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains27_curRate = 0;
                this->granulator_04_grains27_active = false;
            } else {
                this->granulator_04_grains27_curLen = length;
                this->granulator_04_grains27_active = true;
                this->granulator_04_grains27_backwards = rate < 0;
                this->granulator_04_grains27_sampleOffset = 0;
                this->granulator_04_grains27_curStart = start;
    
                if ((bool)(this->granulator_04_grains27_backwards)) {
                    this->granulator_04_grains27_curStart += this->granulator_04_grains27_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains27_active)) {
            if (this->granulator_04_grains27_sampleOffset >= this->granulator_04_grains27_curLen) {
                bangDone = true;
                this->granulator_04_grains27_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains27_curStart;
    
                if ((bool)(this->granulator_04_grains27_backwards)) {
                    sampleIndex -= this->granulator_04_grains27_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains27_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains27_sampleOffset / this->granulator_04_grains27_curLen > 1 ? 1 : (this->granulator_04_grains27_sampleOffset / this->granulator_04_grains27_curLen < 0 ? 0 : this->granulator_04_grains27_sampleOffset / this->granulator_04_grains27_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains27_sampleOffset += this->granulator_04_grains27_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains28_inactive() {
        return !(bool)(this->granulator_04_grains28_active);
    }
    
    void granulator_04_grains28_reset() {
        this->granulator_04_grains28_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains28_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains28_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains28_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains28_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains28_curRate = 0;
                this->granulator_04_grains28_active = false;
            } else {
                this->granulator_04_grains28_curLen = length;
                this->granulator_04_grains28_active = true;
                this->granulator_04_grains28_backwards = rate < 0;
                this->granulator_04_grains28_sampleOffset = 0;
                this->granulator_04_grains28_curStart = start;
    
                if ((bool)(this->granulator_04_grains28_backwards)) {
                    this->granulator_04_grains28_curStart += this->granulator_04_grains28_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains28_active)) {
            if (this->granulator_04_grains28_sampleOffset >= this->granulator_04_grains28_curLen) {
                bangDone = true;
                this->granulator_04_grains28_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains28_curStart;
    
                if ((bool)(this->granulator_04_grains28_backwards)) {
                    sampleIndex -= this->granulator_04_grains28_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains28_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains28_sampleOffset / this->granulator_04_grains28_curLen > 1 ? 1 : (this->granulator_04_grains28_sampleOffset / this->granulator_04_grains28_curLen < 0 ? 0 : this->granulator_04_grains28_sampleOffset / this->granulator_04_grains28_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains28_sampleOffset += this->granulator_04_grains28_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains29_inactive() {
        return !(bool)(this->granulator_04_grains29_active);
    }
    
    void granulator_04_grains29_reset() {
        this->granulator_04_grains29_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains29_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains29_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains29_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains29_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains29_curRate = 0;
                this->granulator_04_grains29_active = false;
            } else {
                this->granulator_04_grains29_curLen = length;
                this->granulator_04_grains29_active = true;
                this->granulator_04_grains29_backwards = rate < 0;
                this->granulator_04_grains29_sampleOffset = 0;
                this->granulator_04_grains29_curStart = start;
    
                if ((bool)(this->granulator_04_grains29_backwards)) {
                    this->granulator_04_grains29_curStart += this->granulator_04_grains29_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains29_active)) {
            if (this->granulator_04_grains29_sampleOffset >= this->granulator_04_grains29_curLen) {
                bangDone = true;
                this->granulator_04_grains29_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains29_curStart;
    
                if ((bool)(this->granulator_04_grains29_backwards)) {
                    sampleIndex -= this->granulator_04_grains29_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains29_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains29_sampleOffset / this->granulator_04_grains29_curLen > 1 ? 1 : (this->granulator_04_grains29_sampleOffset / this->granulator_04_grains29_curLen < 0 ? 0 : this->granulator_04_grains29_sampleOffset / this->granulator_04_grains29_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains29_sampleOffset += this->granulator_04_grains29_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains30_inactive() {
        return !(bool)(this->granulator_04_grains30_active);
    }
    
    void granulator_04_grains30_reset() {
        this->granulator_04_grains30_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains30_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains30_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains30_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains30_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains30_curRate = 0;
                this->granulator_04_grains30_active = false;
            } else {
                this->granulator_04_grains30_curLen = length;
                this->granulator_04_grains30_active = true;
                this->granulator_04_grains30_backwards = rate < 0;
                this->granulator_04_grains30_sampleOffset = 0;
                this->granulator_04_grains30_curStart = start;
    
                if ((bool)(this->granulator_04_grains30_backwards)) {
                    this->granulator_04_grains30_curStart += this->granulator_04_grains30_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains30_active)) {
            if (this->granulator_04_grains30_sampleOffset >= this->granulator_04_grains30_curLen) {
                bangDone = true;
                this->granulator_04_grains30_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains30_curStart;
    
                if ((bool)(this->granulator_04_grains30_backwards)) {
                    sampleIndex -= this->granulator_04_grains30_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains30_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains30_sampleOffset / this->granulator_04_grains30_curLen > 1 ? 1 : (this->granulator_04_grains30_sampleOffset / this->granulator_04_grains30_curLen < 0 ? 0 : this->granulator_04_grains30_sampleOffset / this->granulator_04_grains30_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains30_sampleOffset += this->granulator_04_grains30_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains31_inactive() {
        return !(bool)(this->granulator_04_grains31_active);
    }
    
    void granulator_04_grains31_reset() {
        this->granulator_04_grains31_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains31_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains31_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains31_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains31_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains31_curRate = 0;
                this->granulator_04_grains31_active = false;
            } else {
                this->granulator_04_grains31_curLen = length;
                this->granulator_04_grains31_active = true;
                this->granulator_04_grains31_backwards = rate < 0;
                this->granulator_04_grains31_sampleOffset = 0;
                this->granulator_04_grains31_curStart = start;
    
                if ((bool)(this->granulator_04_grains31_backwards)) {
                    this->granulator_04_grains31_curStart += this->granulator_04_grains31_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains31_active)) {
            if (this->granulator_04_grains31_sampleOffset >= this->granulator_04_grains31_curLen) {
                bangDone = true;
                this->granulator_04_grains31_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains31_curStart;
    
                if ((bool)(this->granulator_04_grains31_backwards)) {
                    sampleIndex -= this->granulator_04_grains31_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains31_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains31_sampleOffset / this->granulator_04_grains31_curLen > 1 ? 1 : (this->granulator_04_grains31_sampleOffset / this->granulator_04_grains31_curLen < 0 ? 0 : this->granulator_04_grains31_sampleOffset / this->granulator_04_grains31_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains31_sampleOffset += this->granulator_04_grains31_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains32_inactive() {
        return !(bool)(this->granulator_04_grains32_active);
    }
    
    void granulator_04_grains32_reset() {
        this->granulator_04_grains32_active = false;
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains32_next(
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        number bangDone = false;
        number v = 0;
        number ph = 0;
        const SampleIndex bufferSize = (const SampleIndex)(buffer->getSize());
    
        if (trigger > 0) {
            this->granulator_04_grains32_curRate = rnbo_abs(rate * buffer->getSampleRate() / this->samplerate());
    
            if (this->granulator_04_grains32_curRate == 0 || length <= 0) {
                if ((bool)(this->granulator_04_grains32_active)) {
                    bangDone = true;
                }
    
                this->granulator_04_grains32_curRate = 0;
                this->granulator_04_grains32_active = false;
            } else {
                this->granulator_04_grains32_curLen = length;
                this->granulator_04_grains32_active = true;
                this->granulator_04_grains32_backwards = rate < 0;
                this->granulator_04_grains32_sampleOffset = 0;
                this->granulator_04_grains32_curStart = start;
    
                if ((bool)(this->granulator_04_grains32_backwards)) {
                    this->granulator_04_grains32_curStart += this->granulator_04_grains32_curLen;
                }
    
                bangDone = false;
            }
        }
    
        if ((bool)(this->granulator_04_grains32_active)) {
            if (this->granulator_04_grains32_sampleOffset >= this->granulator_04_grains32_curLen) {
                bangDone = true;
                this->granulator_04_grains32_active = false;
                ph = 1.0;
            } else {
                number sampleIndex = this->granulator_04_grains32_curStart;
    
                if ((bool)(this->granulator_04_grains32_backwards)) {
                    sampleIndex -= this->granulator_04_grains32_sampleOffset;
                } else {
                    sampleIndex += this->granulator_04_grains32_sampleOffset;
                }
    
                const Index bufferChannels = (const Index)(buffer->getChannels());
                ph = (this->granulator_04_grains32_sampleOffset / this->granulator_04_grains32_curLen > 1 ? 1 : (this->granulator_04_grains32_sampleOffset / this->granulator_04_grains32_curLen < 0 ? 0 : this->granulator_04_grains32_sampleOffset / this->granulator_04_grains32_curLen));
    
                if (bufferSize == 0)
                    v = 0;
                else {
                    {
                        channel = this->bufferbindchannel(channel, bufferChannels, 1);
                        sampleIndex = this->bufferbindindex(sampleIndex, 0, bufferSize, 3);
                        v = this->bufferreadsample(buffer, sampleIndex, channel, 0, bufferSize, 0);
                    }
                }
            }
    
            this->granulator_04_grains32_sampleOffset += this->granulator_04_grains32_curRate;
        }
    
        return {v, ph, ((bool)(bangDone) ? 1 : 0)};
    }
    
    bool granulator_04_grains_inactive(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_04_grains1_inactive();
        case 1:
            return this->granulator_04_grains2_inactive();
        case 2:
            return this->granulator_04_grains3_inactive();
        case 3:
            return this->granulator_04_grains4_inactive();
        case 4:
            return this->granulator_04_grains5_inactive();
        case 5:
            return this->granulator_04_grains6_inactive();
        case 6:
            return this->granulator_04_grains7_inactive();
        case 7:
            return this->granulator_04_grains8_inactive();
        case 8:
            return this->granulator_04_grains9_inactive();
        case 9:
            return this->granulator_04_grains10_inactive();
        case 10:
            return this->granulator_04_grains11_inactive();
        case 11:
            return this->granulator_04_grains12_inactive();
        case 12:
            return this->granulator_04_grains13_inactive();
        case 13:
            return this->granulator_04_grains14_inactive();
        case 14:
            return this->granulator_04_grains15_inactive();
        case 15:
            return this->granulator_04_grains16_inactive();
        case 16:
            return this->granulator_04_grains17_inactive();
        case 17:
            return this->granulator_04_grains18_inactive();
        case 18:
            return this->granulator_04_grains19_inactive();
        case 19:
            return this->granulator_04_grains20_inactive();
        case 20:
            return this->granulator_04_grains21_inactive();
        case 21:
            return this->granulator_04_grains22_inactive();
        case 22:
            return this->granulator_04_grains23_inactive();
        case 23:
            return this->granulator_04_grains24_inactive();
        case 24:
            return this->granulator_04_grains25_inactive();
        case 25:
            return this->granulator_04_grains26_inactive();
        case 26:
            return this->granulator_04_grains27_inactive();
        case 27:
            return this->granulator_04_grains28_inactive();
        case 28:
            return this->granulator_04_grains29_inactive();
        case 29:
            return this->granulator_04_grains30_inactive();
        case 30:
            return this->granulator_04_grains31_inactive();
        default:
            return this->granulator_04_grains32_inactive();
        }
    
        return 0;
    }
    
    void granulator_04_grains_reset(Index i) {
        switch ((int)i) {
        case 0:
            return this->granulator_04_grains1_reset();
        case 1:
            return this->granulator_04_grains2_reset();
        case 2:
            return this->granulator_04_grains3_reset();
        case 3:
            return this->granulator_04_grains4_reset();
        case 4:
            return this->granulator_04_grains5_reset();
        case 5:
            return this->granulator_04_grains6_reset();
        case 6:
            return this->granulator_04_grains7_reset();
        case 7:
            return this->granulator_04_grains8_reset();
        case 8:
            return this->granulator_04_grains9_reset();
        case 9:
            return this->granulator_04_grains10_reset();
        case 10:
            return this->granulator_04_grains11_reset();
        case 11:
            return this->granulator_04_grains12_reset();
        case 12:
            return this->granulator_04_grains13_reset();
        case 13:
            return this->granulator_04_grains14_reset();
        case 14:
            return this->granulator_04_grains15_reset();
        case 15:
            return this->granulator_04_grains16_reset();
        case 16:
            return this->granulator_04_grains17_reset();
        case 17:
            return this->granulator_04_grains18_reset();
        case 18:
            return this->granulator_04_grains19_reset();
        case 19:
            return this->granulator_04_grains20_reset();
        case 20:
            return this->granulator_04_grains21_reset();
        case 21:
            return this->granulator_04_grains22_reset();
        case 22:
            return this->granulator_04_grains23_reset();
        case 23:
            return this->granulator_04_grains24_reset();
        case 24:
            return this->granulator_04_grains25_reset();
        case 25:
            return this->granulator_04_grains26_reset();
        case 26:
            return this->granulator_04_grains27_reset();
        case 27:
            return this->granulator_04_grains28_reset();
        case 28:
            return this->granulator_04_grains29_reset();
        case 29:
            return this->granulator_04_grains30_reset();
        case 30:
            return this->granulator_04_grains31_reset();
        default:
            return this->granulator_04_grains32_reset();
        }
    }
    
    template <typename T> inline array<number, 3> granulator_04_grains_next(
        Index i,
        T& buffer,
        number trigger,
        number start,
        number length,
        number rate,
        number channel
    ) {
        switch ((int)i) {
        case 0:
            return this->granulator_04_grains1_next(buffer, trigger, start, length, rate, channel);
        case 1:
            return this->granulator_04_grains2_next(buffer, trigger, start, length, rate, channel);
        case 2:
            return this->granulator_04_grains3_next(buffer, trigger, start, length, rate, channel);
        case 3:
            return this->granulator_04_grains4_next(buffer, trigger, start, length, rate, channel);
        case 4:
            return this->granulator_04_grains5_next(buffer, trigger, start, length, rate, channel);
        case 5:
            return this->granulator_04_grains6_next(buffer, trigger, start, length, rate, channel);
        case 6:
            return this->granulator_04_grains7_next(buffer, trigger, start, length, rate, channel);
        case 7:
            return this->granulator_04_grains8_next(buffer, trigger, start, length, rate, channel);
        case 8:
            return this->granulator_04_grains9_next(buffer, trigger, start, length, rate, channel);
        case 9:
            return this->granulator_04_grains10_next(buffer, trigger, start, length, rate, channel);
        case 10:
            return this->granulator_04_grains11_next(buffer, trigger, start, length, rate, channel);
        case 11:
            return this->granulator_04_grains12_next(buffer, trigger, start, length, rate, channel);
        case 12:
            return this->granulator_04_grains13_next(buffer, trigger, start, length, rate, channel);
        case 13:
            return this->granulator_04_grains14_next(buffer, trigger, start, length, rate, channel);
        case 14:
            return this->granulator_04_grains15_next(buffer, trigger, start, length, rate, channel);
        case 15:
            return this->granulator_04_grains16_next(buffer, trigger, start, length, rate, channel);
        case 16:
            return this->granulator_04_grains17_next(buffer, trigger, start, length, rate, channel);
        case 17:
            return this->granulator_04_grains18_next(buffer, trigger, start, length, rate, channel);
        case 18:
            return this->granulator_04_grains19_next(buffer, trigger, start, length, rate, channel);
        case 19:
            return this->granulator_04_grains20_next(buffer, trigger, start, length, rate, channel);
        case 20:
            return this->granulator_04_grains21_next(buffer, trigger, start, length, rate, channel);
        case 21:
            return this->granulator_04_grains22_next(buffer, trigger, start, length, rate, channel);
        case 22:
            return this->granulator_04_grains23_next(buffer, trigger, start, length, rate, channel);
        case 23:
            return this->granulator_04_grains24_next(buffer, trigger, start, length, rate, channel);
        case 24:
            return this->granulator_04_grains25_next(buffer, trigger, start, length, rate, channel);
        case 25:
            return this->granulator_04_grains26_next(buffer, trigger, start, length, rate, channel);
        case 26:
            return this->granulator_04_grains27_next(buffer, trigger, start, length, rate, channel);
        case 27:
            return this->granulator_04_grains28_next(buffer, trigger, start, length, rate, channel);
        case 28:
            return this->granulator_04_grains29_next(buffer, trigger, start, length, rate, channel);
        case 29:
            return this->granulator_04_grains30_next(buffer, trigger, start, length, rate, channel);
        case 30:
            return this->granulator_04_grains31_next(buffer, trigger, start, length, rate, channel);
        default:
            return this->granulator_04_grains32_next(buffer, trigger, start, length, rate, channel);
        }
    
        return 0;
    }
    
    void granulator_04_init() {
        this->granulator_04_edgedetect_init();
    
        for (Index i = 0; i < 32; i++) {
            for (Index j = 0; j < 2; j++) {
                this->granulator_04_grainPan[(Index)i][(Index)j] = 1;
            }
        }
    }
    
    void granulator_04_bufferUpdated() {
        number bufSr = this->granulator_04_buffer->getSampleRate();
        number bufSize = this->granulator_04_buffer->getSize();
    
        for (Index i = 0; i < 32; i++) {
            this->granulator_04_grains_reset(i);
        }
    
        {
            this->granulator_04_startMul = bufSize;
        }
    
        {
            this->granulator_04_lengthMul = bufSize;
        }
    }
    
    void granulator_04_onSampleRateChanged(number samplerate) {
        RNBO_UNUSED(samplerate);
        this->granulator_04_bufferUpdated();
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
    
    void samplerate_04_onSampleRateChanged(number samplerate) {
        this->samplerate_04_samplerate_set(samplerate);
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
        dspexpr_25_in1 = 0;
        dspexpr_25_in2 = 0;
        granulator_04_start = 0;
        granulator_04_length = 2205;
        granulator_04_rate = 1;
        granulator_04_bchan = 0;
        granulator_04_limit = 32;
        granulator_04_panpos = 0.5;
        dspexpr_26_in1 = 0;
        dspexpr_26_in2 = 0;
        param_19_value = 0;
        dspexpr_27_in1 = 0;
        dspexpr_27_in2 = 0;
        dspexpr_28_in1 = 0;
        dspexpr_28_in2 = 0.5;
        dspexpr_29_in1 = 0;
        dspexpr_29_in2 = 0.5;
        dspexpr_30_in1 = 0;
        dspexpr_30_in2 = 0;
        param_20_value = 1;
        dspexpr_31_in1 = 0;
        dspexpr_31_in2 = 0;
        param_21_value = 1;
        dspexpr_32_in1 = 0;
        dspexpr_32_in2 = 0;
        param_22_value = 1;
        param_23_value = 0;
        expr_07_in1 = 0;
        expr_07_in2 = 0;
        expr_07_out1 = 0;
        param_24_value = 1;
        expr_08_in1 = 0;
        expr_08_in2 = 1000;
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
        signals[4] = nullptr;
        signals[5] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        granulator_04_startMul = 1;
        granulator_04_lengthMul = 1;
        granulator_04_limitLast = 0;
        granulator_04_edgedetect_active = false;
        granulator_04_grains1_sampleOffset = 0;
        granulator_04_grains1_curRate = 1;
        granulator_04_grains1_curStart = 0;
        granulator_04_grains1_curLen = 1;
        granulator_04_grains1_active = false;
        granulator_04_grains1_backwards = false;
        granulator_04_grains2_sampleOffset = 0;
        granulator_04_grains2_curRate = 1;
        granulator_04_grains2_curStart = 0;
        granulator_04_grains2_curLen = 1;
        granulator_04_grains2_active = false;
        granulator_04_grains2_backwards = false;
        granulator_04_grains3_sampleOffset = 0;
        granulator_04_grains3_curRate = 1;
        granulator_04_grains3_curStart = 0;
        granulator_04_grains3_curLen = 1;
        granulator_04_grains3_active = false;
        granulator_04_grains3_backwards = false;
        granulator_04_grains4_sampleOffset = 0;
        granulator_04_grains4_curRate = 1;
        granulator_04_grains4_curStart = 0;
        granulator_04_grains4_curLen = 1;
        granulator_04_grains4_active = false;
        granulator_04_grains4_backwards = false;
        granulator_04_grains5_sampleOffset = 0;
        granulator_04_grains5_curRate = 1;
        granulator_04_grains5_curStart = 0;
        granulator_04_grains5_curLen = 1;
        granulator_04_grains5_active = false;
        granulator_04_grains5_backwards = false;
        granulator_04_grains6_sampleOffset = 0;
        granulator_04_grains6_curRate = 1;
        granulator_04_grains6_curStart = 0;
        granulator_04_grains6_curLen = 1;
        granulator_04_grains6_active = false;
        granulator_04_grains6_backwards = false;
        granulator_04_grains7_sampleOffset = 0;
        granulator_04_grains7_curRate = 1;
        granulator_04_grains7_curStart = 0;
        granulator_04_grains7_curLen = 1;
        granulator_04_grains7_active = false;
        granulator_04_grains7_backwards = false;
        granulator_04_grains8_sampleOffset = 0;
        granulator_04_grains8_curRate = 1;
        granulator_04_grains8_curStart = 0;
        granulator_04_grains8_curLen = 1;
        granulator_04_grains8_active = false;
        granulator_04_grains8_backwards = false;
        granulator_04_grains9_sampleOffset = 0;
        granulator_04_grains9_curRate = 1;
        granulator_04_grains9_curStart = 0;
        granulator_04_grains9_curLen = 1;
        granulator_04_grains9_active = false;
        granulator_04_grains9_backwards = false;
        granulator_04_grains10_sampleOffset = 0;
        granulator_04_grains10_curRate = 1;
        granulator_04_grains10_curStart = 0;
        granulator_04_grains10_curLen = 1;
        granulator_04_grains10_active = false;
        granulator_04_grains10_backwards = false;
        granulator_04_grains11_sampleOffset = 0;
        granulator_04_grains11_curRate = 1;
        granulator_04_grains11_curStart = 0;
        granulator_04_grains11_curLen = 1;
        granulator_04_grains11_active = false;
        granulator_04_grains11_backwards = false;
        granulator_04_grains12_sampleOffset = 0;
        granulator_04_grains12_curRate = 1;
        granulator_04_grains12_curStart = 0;
        granulator_04_grains12_curLen = 1;
        granulator_04_grains12_active = false;
        granulator_04_grains12_backwards = false;
        granulator_04_grains13_sampleOffset = 0;
        granulator_04_grains13_curRate = 1;
        granulator_04_grains13_curStart = 0;
        granulator_04_grains13_curLen = 1;
        granulator_04_grains13_active = false;
        granulator_04_grains13_backwards = false;
        granulator_04_grains14_sampleOffset = 0;
        granulator_04_grains14_curRate = 1;
        granulator_04_grains14_curStart = 0;
        granulator_04_grains14_curLen = 1;
        granulator_04_grains14_active = false;
        granulator_04_grains14_backwards = false;
        granulator_04_grains15_sampleOffset = 0;
        granulator_04_grains15_curRate = 1;
        granulator_04_grains15_curStart = 0;
        granulator_04_grains15_curLen = 1;
        granulator_04_grains15_active = false;
        granulator_04_grains15_backwards = false;
        granulator_04_grains16_sampleOffset = 0;
        granulator_04_grains16_curRate = 1;
        granulator_04_grains16_curStart = 0;
        granulator_04_grains16_curLen = 1;
        granulator_04_grains16_active = false;
        granulator_04_grains16_backwards = false;
        granulator_04_grains17_sampleOffset = 0;
        granulator_04_grains17_curRate = 1;
        granulator_04_grains17_curStart = 0;
        granulator_04_grains17_curLen = 1;
        granulator_04_grains17_active = false;
        granulator_04_grains17_backwards = false;
        granulator_04_grains18_sampleOffset = 0;
        granulator_04_grains18_curRate = 1;
        granulator_04_grains18_curStart = 0;
        granulator_04_grains18_curLen = 1;
        granulator_04_grains18_active = false;
        granulator_04_grains18_backwards = false;
        granulator_04_grains19_sampleOffset = 0;
        granulator_04_grains19_curRate = 1;
        granulator_04_grains19_curStart = 0;
        granulator_04_grains19_curLen = 1;
        granulator_04_grains19_active = false;
        granulator_04_grains19_backwards = false;
        granulator_04_grains20_sampleOffset = 0;
        granulator_04_grains20_curRate = 1;
        granulator_04_grains20_curStart = 0;
        granulator_04_grains20_curLen = 1;
        granulator_04_grains20_active = false;
        granulator_04_grains20_backwards = false;
        granulator_04_grains21_sampleOffset = 0;
        granulator_04_grains21_curRate = 1;
        granulator_04_grains21_curStart = 0;
        granulator_04_grains21_curLen = 1;
        granulator_04_grains21_active = false;
        granulator_04_grains21_backwards = false;
        granulator_04_grains22_sampleOffset = 0;
        granulator_04_grains22_curRate = 1;
        granulator_04_grains22_curStart = 0;
        granulator_04_grains22_curLen = 1;
        granulator_04_grains22_active = false;
        granulator_04_grains22_backwards = false;
        granulator_04_grains23_sampleOffset = 0;
        granulator_04_grains23_curRate = 1;
        granulator_04_grains23_curStart = 0;
        granulator_04_grains23_curLen = 1;
        granulator_04_grains23_active = false;
        granulator_04_grains23_backwards = false;
        granulator_04_grains24_sampleOffset = 0;
        granulator_04_grains24_curRate = 1;
        granulator_04_grains24_curStart = 0;
        granulator_04_grains24_curLen = 1;
        granulator_04_grains24_active = false;
        granulator_04_grains24_backwards = false;
        granulator_04_grains25_sampleOffset = 0;
        granulator_04_grains25_curRate = 1;
        granulator_04_grains25_curStart = 0;
        granulator_04_grains25_curLen = 1;
        granulator_04_grains25_active = false;
        granulator_04_grains25_backwards = false;
        granulator_04_grains26_sampleOffset = 0;
        granulator_04_grains26_curRate = 1;
        granulator_04_grains26_curStart = 0;
        granulator_04_grains26_curLen = 1;
        granulator_04_grains26_active = false;
        granulator_04_grains26_backwards = false;
        granulator_04_grains27_sampleOffset = 0;
        granulator_04_grains27_curRate = 1;
        granulator_04_grains27_curStart = 0;
        granulator_04_grains27_curLen = 1;
        granulator_04_grains27_active = false;
        granulator_04_grains27_backwards = false;
        granulator_04_grains28_sampleOffset = 0;
        granulator_04_grains28_curRate = 1;
        granulator_04_grains28_curStart = 0;
        granulator_04_grains28_curLen = 1;
        granulator_04_grains28_active = false;
        granulator_04_grains28_backwards = false;
        granulator_04_grains29_sampleOffset = 0;
        granulator_04_grains29_curRate = 1;
        granulator_04_grains29_curStart = 0;
        granulator_04_grains29_curLen = 1;
        granulator_04_grains29_active = false;
        granulator_04_grains29_backwards = false;
        granulator_04_grains30_sampleOffset = 0;
        granulator_04_grains30_curRate = 1;
        granulator_04_grains30_curStart = 0;
        granulator_04_grains30_curLen = 1;
        granulator_04_grains30_active = false;
        granulator_04_grains30_backwards = false;
        granulator_04_grains31_sampleOffset = 0;
        granulator_04_grains31_curRate = 1;
        granulator_04_grains31_curStart = 0;
        granulator_04_grains31_curLen = 1;
        granulator_04_grains31_active = false;
        granulator_04_grains31_backwards = false;
        granulator_04_grains32_sampleOffset = 0;
        granulator_04_grains32_curRate = 1;
        granulator_04_grains32_curStart = 0;
        granulator_04_grains32_curLen = 1;
        granulator_04_grains32_active = false;
        granulator_04_grains32_backwards = false;
        param_19_lastValue = 0;
        param_20_lastValue = 0;
        param_21_lastValue = 0;
        param_22_lastValue = 0;
        param_23_lastValue = 0;
        param_24_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_25_in1;
        number dspexpr_25_in2;
        number granulator_04_start;
        number granulator_04_length;
        number granulator_04_rate;
        number granulator_04_bchan;
        number granulator_04_limit;
        number granulator_04_panpos;
        number dspexpr_26_in1;
        number dspexpr_26_in2;
        number param_19_value;
        number dspexpr_27_in1;
        number dspexpr_27_in2;
        number dspexpr_28_in1;
        number dspexpr_28_in2;
        number dspexpr_29_in1;
        number dspexpr_29_in2;
        number dspexpr_30_in1;
        number dspexpr_30_in2;
        number param_20_value;
        number dspexpr_31_in1;
        number dspexpr_31_in2;
        number param_21_value;
        number dspexpr_32_in1;
        number dspexpr_32_in2;
        number param_22_value;
        number param_23_value;
        number expr_07_in1;
        number expr_07_in2;
        number expr_07_out1;
        number param_24_value;
        number expr_08_in1;
        number expr_08_in2;
        number expr_08_out1;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[6];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float32BufferRef granulator_04_buffer;
        number granulator_04_grainPan[32][2] = { };
        number granulator_04_startMul;
        number granulator_04_lengthMul;
        int granulator_04_limitLast;
        bool granulator_04_edgedetect_active;
        number granulator_04_grains1_sampleOffset;
        number granulator_04_grains1_curRate;
        number granulator_04_grains1_curStart;
        number granulator_04_grains1_curLen;
        bool granulator_04_grains1_active;
        bool granulator_04_grains1_backwards;
        number granulator_04_grains2_sampleOffset;
        number granulator_04_grains2_curRate;
        number granulator_04_grains2_curStart;
        number granulator_04_grains2_curLen;
        bool granulator_04_grains2_active;
        bool granulator_04_grains2_backwards;
        number granulator_04_grains3_sampleOffset;
        number granulator_04_grains3_curRate;
        number granulator_04_grains3_curStart;
        number granulator_04_grains3_curLen;
        bool granulator_04_grains3_active;
        bool granulator_04_grains3_backwards;
        number granulator_04_grains4_sampleOffset;
        number granulator_04_grains4_curRate;
        number granulator_04_grains4_curStart;
        number granulator_04_grains4_curLen;
        bool granulator_04_grains4_active;
        bool granulator_04_grains4_backwards;
        number granulator_04_grains5_sampleOffset;
        number granulator_04_grains5_curRate;
        number granulator_04_grains5_curStart;
        number granulator_04_grains5_curLen;
        bool granulator_04_grains5_active;
        bool granulator_04_grains5_backwards;
        number granulator_04_grains6_sampleOffset;
        number granulator_04_grains6_curRate;
        number granulator_04_grains6_curStart;
        number granulator_04_grains6_curLen;
        bool granulator_04_grains6_active;
        bool granulator_04_grains6_backwards;
        number granulator_04_grains7_sampleOffset;
        number granulator_04_grains7_curRate;
        number granulator_04_grains7_curStart;
        number granulator_04_grains7_curLen;
        bool granulator_04_grains7_active;
        bool granulator_04_grains7_backwards;
        number granulator_04_grains8_sampleOffset;
        number granulator_04_grains8_curRate;
        number granulator_04_grains8_curStart;
        number granulator_04_grains8_curLen;
        bool granulator_04_grains8_active;
        bool granulator_04_grains8_backwards;
        number granulator_04_grains9_sampleOffset;
        number granulator_04_grains9_curRate;
        number granulator_04_grains9_curStart;
        number granulator_04_grains9_curLen;
        bool granulator_04_grains9_active;
        bool granulator_04_grains9_backwards;
        number granulator_04_grains10_sampleOffset;
        number granulator_04_grains10_curRate;
        number granulator_04_grains10_curStart;
        number granulator_04_grains10_curLen;
        bool granulator_04_grains10_active;
        bool granulator_04_grains10_backwards;
        number granulator_04_grains11_sampleOffset;
        number granulator_04_grains11_curRate;
        number granulator_04_grains11_curStart;
        number granulator_04_grains11_curLen;
        bool granulator_04_grains11_active;
        bool granulator_04_grains11_backwards;
        number granulator_04_grains12_sampleOffset;
        number granulator_04_grains12_curRate;
        number granulator_04_grains12_curStart;
        number granulator_04_grains12_curLen;
        bool granulator_04_grains12_active;
        bool granulator_04_grains12_backwards;
        number granulator_04_grains13_sampleOffset;
        number granulator_04_grains13_curRate;
        number granulator_04_grains13_curStart;
        number granulator_04_grains13_curLen;
        bool granulator_04_grains13_active;
        bool granulator_04_grains13_backwards;
        number granulator_04_grains14_sampleOffset;
        number granulator_04_grains14_curRate;
        number granulator_04_grains14_curStart;
        number granulator_04_grains14_curLen;
        bool granulator_04_grains14_active;
        bool granulator_04_grains14_backwards;
        number granulator_04_grains15_sampleOffset;
        number granulator_04_grains15_curRate;
        number granulator_04_grains15_curStart;
        number granulator_04_grains15_curLen;
        bool granulator_04_grains15_active;
        bool granulator_04_grains15_backwards;
        number granulator_04_grains16_sampleOffset;
        number granulator_04_grains16_curRate;
        number granulator_04_grains16_curStart;
        number granulator_04_grains16_curLen;
        bool granulator_04_grains16_active;
        bool granulator_04_grains16_backwards;
        number granulator_04_grains17_sampleOffset;
        number granulator_04_grains17_curRate;
        number granulator_04_grains17_curStart;
        number granulator_04_grains17_curLen;
        bool granulator_04_grains17_active;
        bool granulator_04_grains17_backwards;
        number granulator_04_grains18_sampleOffset;
        number granulator_04_grains18_curRate;
        number granulator_04_grains18_curStart;
        number granulator_04_grains18_curLen;
        bool granulator_04_grains18_active;
        bool granulator_04_grains18_backwards;
        number granulator_04_grains19_sampleOffset;
        number granulator_04_grains19_curRate;
        number granulator_04_grains19_curStart;
        number granulator_04_grains19_curLen;
        bool granulator_04_grains19_active;
        bool granulator_04_grains19_backwards;
        number granulator_04_grains20_sampleOffset;
        number granulator_04_grains20_curRate;
        number granulator_04_grains20_curStart;
        number granulator_04_grains20_curLen;
        bool granulator_04_grains20_active;
        bool granulator_04_grains20_backwards;
        number granulator_04_grains21_sampleOffset;
        number granulator_04_grains21_curRate;
        number granulator_04_grains21_curStart;
        number granulator_04_grains21_curLen;
        bool granulator_04_grains21_active;
        bool granulator_04_grains21_backwards;
        number granulator_04_grains22_sampleOffset;
        number granulator_04_grains22_curRate;
        number granulator_04_grains22_curStart;
        number granulator_04_grains22_curLen;
        bool granulator_04_grains22_active;
        bool granulator_04_grains22_backwards;
        number granulator_04_grains23_sampleOffset;
        number granulator_04_grains23_curRate;
        number granulator_04_grains23_curStart;
        number granulator_04_grains23_curLen;
        bool granulator_04_grains23_active;
        bool granulator_04_grains23_backwards;
        number granulator_04_grains24_sampleOffset;
        number granulator_04_grains24_curRate;
        number granulator_04_grains24_curStart;
        number granulator_04_grains24_curLen;
        bool granulator_04_grains24_active;
        bool granulator_04_grains24_backwards;
        number granulator_04_grains25_sampleOffset;
        number granulator_04_grains25_curRate;
        number granulator_04_grains25_curStart;
        number granulator_04_grains25_curLen;
        bool granulator_04_grains25_active;
        bool granulator_04_grains25_backwards;
        number granulator_04_grains26_sampleOffset;
        number granulator_04_grains26_curRate;
        number granulator_04_grains26_curStart;
        number granulator_04_grains26_curLen;
        bool granulator_04_grains26_active;
        bool granulator_04_grains26_backwards;
        number granulator_04_grains27_sampleOffset;
        number granulator_04_grains27_curRate;
        number granulator_04_grains27_curStart;
        number granulator_04_grains27_curLen;
        bool granulator_04_grains27_active;
        bool granulator_04_grains27_backwards;
        number granulator_04_grains28_sampleOffset;
        number granulator_04_grains28_curRate;
        number granulator_04_grains28_curStart;
        number granulator_04_grains28_curLen;
        bool granulator_04_grains28_active;
        bool granulator_04_grains28_backwards;
        number granulator_04_grains29_sampleOffset;
        number granulator_04_grains29_curRate;
        number granulator_04_grains29_curStart;
        number granulator_04_grains29_curLen;
        bool granulator_04_grains29_active;
        bool granulator_04_grains29_backwards;
        number granulator_04_grains30_sampleOffset;
        number granulator_04_grains30_curRate;
        number granulator_04_grains30_curStart;
        number granulator_04_grains30_curLen;
        bool granulator_04_grains30_active;
        bool granulator_04_grains30_backwards;
        number granulator_04_grains31_sampleOffset;
        number granulator_04_grains31_curRate;
        number granulator_04_grains31_curStart;
        number granulator_04_grains31_curLen;
        bool granulator_04_grains31_active;
        bool granulator_04_grains31_backwards;
        number granulator_04_grains32_sampleOffset;
        number granulator_04_grains32_curRate;
        number granulator_04_grains32_curStart;
        number granulator_04_grains32_curLen;
        bool granulator_04_grains32_active;
        bool granulator_04_grains32_backwards;
        number param_19_lastValue;
        number param_20_lastValue;
        number param_21_lastValue;
        number param_22_lastValue;
        number param_23_lastValue;
        number param_24_lastValue;
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
    getEngine()->flushClockEvents(this, 2098551528, false);
    getEngine()->flushClockEvents(this, -1494586265, false);
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

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
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
    if (!stringCompare(paramid, "rec-layer")) {
        return 0;
    }

    if (!stringCompare(paramid, "rec-mon")) {
        return 1;
    }

    if (!stringCompare(paramid, "rec-mode")) {
        return 2;
    }

    if (!stringCompare(paramid, "rec-gain")) {
        return 3;
    }

    if (!stringCompare(paramid, "rec-loop")) {
        return 4;
    }

    if (!stringCompare(paramid, "rec-begin")) {
        return 5;
    }

    if (!stringCompare(paramid, "rec-end")) {
        return 6;
    }

    if (!stringCompare(paramid, "toggle_obj-40/value")) {
        return 7;
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
    SampleValue * out10 = (numOutputs >= 10 && outputs[9] ? outputs[9] : this->dummyBuffer);
    SampleValue * out11 = (numOutputs >= 11 && outputs[10] ? outputs[10] : this->dummyBuffer);
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
    SampleValue * in18 = (numInputs >= 18 && inputs[17] ? inputs[17] : this->zeroBuffer);
    SampleValue * in19 = (numInputs >= 19 && inputs[18] ? inputs[18] : this->zeroBuffer);
    SampleValue * in20 = (numInputs >= 20 && inputs[19] ? inputs[19] : this->zeroBuffer);
    SampleValue * in21 = (numInputs >= 21 && inputs[20] ? inputs[20] : this->zeroBuffer);
    SampleValue * in22 = (numInputs >= 22 && inputs[21] ? inputs[21] : this->zeroBuffer);
    SampleValue * in23 = (numInputs >= 23 && inputs[22] ? inputs[22] : this->zeroBuffer);
    SampleValue * in24 = (numInputs >= 24 && inputs[23] ? inputs[23] : this->zeroBuffer);
    this->dspexpr_43_perform(in1, this->dspexpr_43_in2, this->signals[0], n);
    this->dspexpr_37_perform(in1, this->dspexpr_37_in2, this->signals[1], n);
    this->p_01_perform(in2, in3, in4, in5, in6, out3, out4, n);
    this->dspexpr_36_perform(this->signals[1], out3, this->signals[2], n);
    this->dspexpr_42_perform(this->signals[1], out4, this->signals[3], n);
    this->dspexpr_38_perform(in22, this->dspexpr_38_in2, this->signals[1], n);

    this->recordtilde_01_perform(
        this->signals[1],
        this->recordtilde_01_begin,
        this->recordtilde_01_end,
        this->signals[0],
        out11,
        n
    );

    this->p_02_perform(in12, in13, in14, in15, in16, this->signals[0], out8, n);
    this->signalforwarder_01_perform(this->signals[0], out7, n);
    this->p_03_perform(in7, in8, in9, in10, in11, this->signals[1], out6, n);
    this->dspexpr_34_perform(this->signals[2], this->signals[1], this->signals[4], n);
    this->dspexpr_35_perform(this->signals[4], this->signals[0], this->signals[2], n);
    this->signalforwarder_02_perform(this->signals[1], out5, n);
    this->dspexpr_40_perform(this->signals[3], out6, this->signals[1], n);
    this->dspexpr_41_perform(this->signals[1], out8, this->signals[3], n);
    this->dspexpr_45_perform(in23, this->dspexpr_45_in2, this->signals[1], n);
    this->dspexpr_44_perform(this->signals[1], this->dspexpr_44_in2, this->signals[0], n);
    this->numbertilde_01_perform(this->signals[0], this->dummyBuffer, n);
    this->p_04_perform(in17, in18, in19, in20, in21, this->signals[0], out10, n);
    this->dspexpr_33_perform(this->signals[2], this->signals[0], out1, n);
    this->signalforwarder_03_perform(this->signals[0], out9, n);
    this->dspexpr_39_perform(this->signals[3], out10, out2, n);
    this->dspexpr_47_perform(in24, this->dspexpr_47_in2, this->signals[3], n);
    this->dspexpr_46_perform(this->signals[3], this->dspexpr_46_in2, this->signals[0], n);
    this->numbertilde_02_perform(this->signals[0], this->dummyBuffer, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 5; i++) {
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

    this->numbertilde_01_dspsetup(forceDSPSetup);
    this->numbertilde_02_dspsetup(forceDSPSetup);
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
        return addressOf(this->layer3);
        break;
    case 2:
        return addressOf(this->layer2);
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

    if (index == -1 || index == 0 || index == 2 || index == 1 || index == 3) {
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
    this->layer3 = initDataRef("layer3", false, nullptr);
    this->layer2 = initDataRef("layer2", false, nullptr);
    this->layer4 = initDataRef("layer4", false, nullptr);
    this->assign_defaults();
    this->setState();
    this->recordtilde_01_bufferobj->setIndex(-1);
    this->recordtilde_01_buffer = new Float32MultiBuffer(this->recordtilde_01_bufferobj);
    this->layer1->setIndex(0);
    this->layer3->setIndex(1);
    this->layer2->setIndex(2);
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
    this->p_01 = new RNBOSubpatcher_98();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_99();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));
    this->p_03 = new RNBOSubpatcher_100();
    this->p_03->setEngineAndPatcher(this->getEngine(), this);
    this->p_03->initialize();
    this->p_03->setParameterOffset(this->getParameterOffset(this->p_03));
    this->p_04 = new RNBOSubpatcher_101();
    this->p_04->setEngineAndPatcher(this->getEngine(), this);
    this->p_04->initialize();
    this->p_04->setParameterOffset(this->getParameterOffset(this->p_04));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_25_getPresetValue(getSubState(preset, "rec-layer"));
    this->param_26_getPresetValue(getSubState(preset, "rec-mon"));
    this->param_27_getPresetValue(getSubState(preset, "rec-mode"));
    this->param_28_getPresetValue(getSubState(preset, "rec-gain"));
    this->param_29_getPresetValue(getSubState(preset, "rec-loop"));
    this->param_30_getPresetValue(getSubState(preset, "rec-begin"));
    this->param_31_getPresetValue(getSubState(preset, "rec-end"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "layer1"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "layer3"));
    this->p_03->getPreset(getSubState(getSubState(preset, "__sps"), "layer2"));
    this->p_04->getPreset(getSubState(getSubState(preset, "__sps"), "layer4"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_25_setPresetValue(getSubState(preset, "rec-layer"));
    this->param_26_setPresetValue(getSubState(preset, "rec-mon"));
    this->param_27_setPresetValue(getSubState(preset, "rec-mode"));
    this->param_28_setPresetValue(getSubState(preset, "rec-gain"));
    this->param_29_setPresetValue(getSubState(preset, "rec-loop"));
    this->param_30_setPresetValue(getSubState(preset, "rec-begin"));
    this->param_31_setPresetValue(getSubState(preset, "rec-end"));
    this->p_01->param_01_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "start"));
    this->p_01->param_02_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "len"));
    this->p_01->param_03_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "gain"));
    this->p_01->param_04_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "rate"));
    this->p_01->param_05_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "pan"));
    this->p_01->param_06_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer1"), "size"));
    this->p_02->param_07_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "start"));
    this->p_02->param_08_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "len"));
    this->p_02->param_09_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "gain"));
    this->p_02->param_10_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "rate"));
    this->p_02->param_11_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "pan"));
    this->p_02->param_12_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer3"), "size"));
    this->p_03->param_13_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "start"));
    this->p_03->param_14_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "len"));
    this->p_03->param_15_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "gain"));
    this->p_03->param_16_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "rate"));
    this->p_03->param_17_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "pan"));
    this->p_03->param_18_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer2"), "size"));
    this->p_04->param_19_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "start"));
    this->p_04->param_20_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "len"));
    this->p_04->param_21_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "gain"));
    this->p_04->param_22_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "rate"));
    this->p_04->param_23_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "pan"));
    this->p_04->param_24_setPresetValue(getSubState(getSubState(getSubState(preset, "__sps"), "layer4"), "size"));
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
        this->param_25_value_set(v);
        break;
    case 1:
        this->param_26_value_set(v);
        break;
    case 2:
        this->param_27_value_set(v);
        break;
    case 3:
        this->param_28_value_set(v);
        break;
    case 4:
        this->param_29_value_set(v);
        break;
    case 5:
        this->param_30_value_set(v);
        break;
    case 6:
        this->param_31_value_set(v);
        break;
    case 7:
        this->toggle_01_value_set(v);
        break;
    default:
        index -= 8;

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
        return this->param_25_value;
    case 1:
        return this->param_26_value;
    case 2:
        return this->param_27_value;
    case 3:
        return this->param_28_value;
    case 4:
        return this->param_29_value;
    case 5:
        return this->param_30_value;
    case 6:
        return this->param_31_value;
    case 7:
        return this->toggle_01_value;
    default:
        index -= 8;

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
    return 8 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        return "rec-layer";
    case 1:
        return "rec-mon";
    case 2:
        return "rec-mode";
    case 3:
        return "rec-gain";
    case 4:
        return "rec-loop";
    case 5:
        return "rec-begin";
    case 6:
        return "rec-end";
    case 7:
        return "toggle_01_value";
    default:
        index -= 8;

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
        return "rec-layer";
    case 1:
        return "rec-mon";
    case 2:
        return "rec-mode";
    case 3:
        return "rec-gain";
    case 4:
        return "rec-loop";
    case 5:
        return "rec-begin";
    case 6:
        return "rec-end";
    case 7:
        return "toggle_obj-40/value";
    default:
        index -= 8;

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
            info->displayName = "Layer";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 1:
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
            info->displayName = "Monitor";
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
            info->displayName = "Record";
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
            info->displayName = "Gain";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 4:
            info->type = ParameterTypeNumber;
            info->initialValue = 1;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 2;
            static const char * eVal4[] = {"false", "true"};
            info->enumValues = eVal4;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Loop";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 5:
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
            info->displayName = "Begin";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 6:
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
            info->displayName = "End";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
        case 7:
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
            index -= 8;

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
        return 8;

    if (subpatcher == this->p_02)
        return 8 + this->p_01->getNumParameters();

    if (subpatcher == this->p_03)
        return 8 + this->p_01->getNumParameters() + this->p_02->getNumParameters();

    if (subpatcher == this->p_04)
        return 8 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters();

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
    case 7:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
    case 2:
    case 4:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 2);
            }

            return normalizedValue;
        }
    case 5:
    case 6:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
            }

            return normalizedValue;
        }
    case 0:
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
    case 1:
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 101);
            }

            return normalizedValue;
        }
    default:
        index -= 8;

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
    case 7:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (1 - 0);
            }
        }
    case 2:
    case 4:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                value = this->applyStepsToNormalizedParameterValue(value, 2);
            }

            {
                return 0 + value * (1 - 0);
            }
        }
    case 5:
    case 6:
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                value = this->applyStepsToNormalizedParameterValue(value, 101);
            }

            {
                return 0 + value * (1 - 0);
            }
        }
    case 0:
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
    case 1:
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
        index -= 8;

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
        return this->param_25_value_constrain(value);
    case 1:
        return this->param_26_value_constrain(value);
    case 2:
        return this->param_27_value_constrain(value);
    case 3:
        return this->param_28_value_constrain(value);
    case 4:
        return this->param_29_value_constrain(value);
    case 5:
        return this->param_30_value_constrain(value);
    case 6:
        return this->param_31_value_constrain(value);
    default:
        index -= 8;

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
        if (TAG("number~_obj-70") == objectId)
            this->numbertilde_01_sig_number_set(payload);

        if (TAG("number~_obj-71") == objectId)
            this->numbertilde_02_sig_number_set(payload);

        break;
    case TAG("mode"):
        if (TAG("number~_obj-70") == objectId)
            this->numbertilde_01_mode_set(payload);

        if (TAG("number~_obj-71") == objectId)
            this->numbertilde_02_mode_set(payload);

        break;
    }

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
    this->updateTime(time);

    switch (tag) {
    case TAG("sig"):
        if (TAG("number~_obj-70") == objectId)
            this->numbertilde_01_sig_list_set(payload);

        if (TAG("number~_obj-71") == objectId)
            this->numbertilde_02_sig_list_set(payload);

        break;
    }

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
    case TAG("monitor"):
        return "monitor";
    case TAG("number~_obj-70"):
        return "number~_obj-70";
    case TAG("assign"):
        return "assign";
    case TAG("setup"):
        return "setup";
    case TAG("number~_obj-71"):
        return "number~_obj-71";
    case TAG("sig"):
        return "sig";
    case TAG("mode"):
        return "mode";
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

void param_25_value_set(number v) {
    v = this->param_25_value_constrain(v);
    this->param_25_value = v;
    this->sendParameter(0, false);

    if (this->param_25_value != this->param_25_lastValue) {
        this->getEngine()->presetTouched();
        this->param_25_lastValue = this->param_25_value;
    }

    this->trigger_01_input_number_set(v);
}

void param_26_value_set(number v) {
    v = this->param_26_value_constrain(v);
    this->param_26_value = v;
    this->sendParameter(1, false);

    if (this->param_26_value != this->param_26_lastValue) {
        this->getEngine()->presetTouched();
        this->param_26_lastValue = this->param_26_value;
    }

    this->expr_10_in1_set(v);
}

void param_27_value_set(number v) {
    v = this->param_27_value_constrain(v);
    this->param_27_value = v;
    this->sendParameter(2, false);

    if (this->param_27_value != this->param_27_lastValue) {
        this->getEngine()->presetTouched();
        this->param_27_lastValue = this->param_27_value;
    }

    this->send_02_input_number_set(v);
}

void param_28_value_set(number v) {
    v = this->param_28_value_constrain(v);
    this->param_28_value = v;
    this->sendParameter(3, false);

    if (this->param_28_value != this->param_28_lastValue) {
        this->getEngine()->presetTouched();
        this->param_28_lastValue = this->param_28_value;
    }

    this->dspexpr_43_in2_set(v);
}

void param_29_value_set(number v) {
    v = this->param_29_value_constrain(v);
    this->param_29_value = v;
    this->sendParameter(4, false);

    if (this->param_29_value != this->param_29_lastValue) {
        this->getEngine()->presetTouched();
        this->param_29_lastValue = this->param_29_value;
    }

    this->recordtilde_01_loop_set(v);
}

void param_30_value_set(number v) {
    v = this->param_30_value_constrain(v);
    this->param_30_value = v;
    this->sendParameter(5, false);

    if (this->param_30_value != this->param_30_lastValue) {
        this->getEngine()->presetTouched();
        this->param_30_lastValue = this->param_30_value;
    }

    this->dspexpr_45_in2_set(v);
}

void param_31_value_set(number v) {
    v = this->param_31_value_constrain(v);
    this->param_31_value = v;
    this->sendParameter(6, false);

    if (this->param_31_value != this->param_31_lastValue) {
        this->getEngine()->presetTouched();
        this->param_31_lastValue = this->param_31_value;
    }

    this->dspexpr_47_in2_set(v);
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
    return 24;
}

Index getNumOutputChannels() const {
    return 11;
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

    if (this->layer3->hasRequestedSize()) {
        if (this->layer3->wantsFill())
            this->zeroDataRef(this->layer3);

        this->getEngine()->sendDataRefUpdated(1);
    }

    if (this->layer2->hasRequestedSize()) {
        if (this->layer2->wantsFill())
            this->zeroDataRef(this->layer2);

        this->getEngine()->sendDataRefUpdated(2);
    }

    if (this->layer4->hasRequestedSize()) {
        if (this->layer4->wantsFill())
            this->zeroDataRef(this->layer4);

        this->getEngine()->sendDataRefUpdated(3);
    }
}

void initializeObjects() {
    this->numbertilde_01_init();
    this->numbertilde_02_init();
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

    {
        this->scheduleParamInit(4, 0);
    }

    {
        this->scheduleParamInit(5, 0);
    }

    {
        this->scheduleParamInit(6, 0);
    }

    this->processParamInitEvents();
}

static number param_25_value_constrain(number v) {
    v = (v > 3 ? 3 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)3 / (number)3;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void send_01_input_number_set(number v) {
    this->send_01_input_number = v;
}

void trigger_01_out2_set(number v) {
    this->send_01_input_number_set(v);
}

void recordtilde_01_buffer_set(number v) {
    updateMultiRef(this, this->recordtilde_01_buffer, v);
}

void trigger_01_out1_set(number v) {
    this->recordtilde_01_buffer_set(v);
}

void trigger_01_input_number_set(number v) {
    this->trigger_01_out2_set(v);
    this->trigger_01_out1_set(v);
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

void dspexpr_37_in2_set(number v) {
    this->dspexpr_37_in2 = v;
}

void expr_09_out1_set(number v) {
    this->expr_09_out1 = v;
    this->dspexpr_37_in2_set(this->expr_09_out1);
}

void expr_09_in1_set(number in1) {
    this->expr_09_in1 = in1;
    this->expr_09_out1_set(this->expr_09_in1 * this->expr_09_in2);//#map:*_obj-51:1
}

void expr_10_out1_set(number v) {
    this->expr_10_out1 = v;
    this->expr_09_in1_set(this->expr_10_out1);
}

void expr_10_in1_set(number in1) {
    this->expr_10_in1 = in1;

    this->expr_10_out1_set(
        (this->expr_10_in2 == 0 ? 0 : (this->expr_10_in2 == 0. ? 0. : this->expr_10_in1 / this->expr_10_in2))
    );//#map:/_obj-49:1
}

static number param_27_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)1 / (number)1;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void dspexpr_38_in2_set(number v) {
    this->dspexpr_38_in2 = v;
}

void expr_09_in2_set(number v) {
    this->expr_09_in2 = v;
    this->expr_09_in1_set(this->expr_09_in1);
}

void toggle_01_value_set(number v) {
    this->toggle_01_value = v;
    this->sendParameter(7, false);
    this->dspexpr_38_in2_set(v);
    this->expr_09_in2_set(v);
}

void receive_01_output_number_set(number v) {
    this->receive_01_output_number = v;
    this->toggle_01_value_set(v);
}

void send_02_input_number_set(number v) {
    this->send_02_input_number = v;
    this->receive_01_output_number_set(v);
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

void dspexpr_43_in2_set(number v) {
    this->dspexpr_43_in2 = v;
}

static number param_29_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)1 / (number)1;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void recordtilde_01_loop_set(number v) {
    this->recordtilde_01_loop = v;
}

static number param_30_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)1 / (number)100;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void dspexpr_45_in2_set(number v) {
    this->dspexpr_45_in2 = v;
}

static number param_31_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)1 / (number)100;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void dspexpr_47_in2_set(number v) {
    this->dspexpr_47_in2 = v;
}

void p_01_out3_number_set(number ) {}

void p_02_out3_number_set(number ) {}

void p_03_out3_number_set(number ) {}

void p_04_out3_number_set(number ) {}

void dspexpr_43_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
    }
}

void dspexpr_37_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
    }
}

void p_01_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * in5,
    Sample * out1,
    Sample * out2,
    Index n
) {
    // subpatcher: layer1
    SampleArray<5> ins = {in1, in2, in3, in4, in5};

    SampleArray<2> outs = {out1, out2};
    this->p_01->process(ins, 5, outs, 2, n);
}

void dspexpr_36_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_42_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_38_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
    }
}

void recordtilde_01_perform(
    const Sample * record,
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
                    if ((bool)(record[(Index)i]) && __recordtilde_01_lastRecord != record[(Index)i]) {
                        __recordtilde_01_wIndex = loopBegin;
                    }
                }

                if (record[(Index)i] != 0 && __recordtilde_01_wIndex < loopEnd) {
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
                    __recordtilde_01_lastRecord = record[(Index)i];
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

void p_02_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * in5,
    Sample * out1,
    Sample * out2,
    Index n
) {
    // subpatcher: layer3
    SampleArray<5> ins = {in1, in2, in3, in4, in5};

    SampleArray<2> outs = {out1, out2};
    this->p_02->process(ins, 5, outs, 2, n);
}

void signalforwarder_01_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void p_03_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * in5,
    Sample * out1,
    Sample * out2,
    Index n
) {
    // subpatcher: layer2
    SampleArray<5> ins = {in1, in2, in3, in4, in5};

    SampleArray<2> outs = {out1, out2};
    this->p_03->process(ins, 5, outs, 2, n);
}

void dspexpr_34_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_35_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void signalforwarder_02_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void dspexpr_40_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_41_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_45_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
    }
}

void dspexpr_44_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    RNBO_UNUSED(in2);
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * 0;//#map:_###_obj_###_:1
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
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-70"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_01_currentInterval = __numbertilde_01_currentInterval;
    this->numbertilde_01_lastValue = __numbertilde_01_lastValue;
}

void p_04_perform(
    Sample * in1,
    Sample * in2,
    Sample * in3,
    Sample * in4,
    Sample * in5,
    Sample * out1,
    Sample * out2,
    Index n
) {
    // subpatcher: layer4
    SampleArray<5> ins = {in1, in2, in3, in4, in5};

    SampleArray<2> outs = {out1, out2};
    this->p_04->process(ins, 5, outs, 2, n);
}

void dspexpr_33_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void signalforwarder_03_perform(const Sample * input, Sample * output, Index n) {
    for (Index i = 0; i < n; i++) {
        output[(Index)i] = input[(Index)i];
    }
}

void dspexpr_39_perform(const Sample * in1, const Sample * in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_47_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
    }
}

void dspexpr_46_perform(const Sample * in1, number in2, Sample * out1, Index n) {
    RNBO_UNUSED(in2);
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * 0;//#map:_###_obj_###_:1
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
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-71"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_02_currentInterval = __numbertilde_02_currentInterval;
    this->numbertilde_02_lastValue = __numbertilde_02_lastValue;
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

void param_25_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_25_value;
}

void param_25_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_25_value_set(preset["value"]);
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

void param_29_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_29_value;
}

void param_29_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_29_value_set(preset["value"]);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-70"), 1, this->_currentTime);
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

void param_30_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_30_value;
}

void param_30_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_30_value_set(preset["value"]);
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
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-71"), 1, this->_currentTime);
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

void param_31_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_31_value;
}

void param_31_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_31_value_set(preset["value"]);
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
    dspexpr_33_in1 = 0;
    dspexpr_33_in2 = 0;
    dspexpr_34_in1 = 0;
    dspexpr_34_in2 = 0;
    dspexpr_35_in1 = 0;
    dspexpr_35_in2 = 0;
    dspexpr_36_in1 = 0;
    dspexpr_36_in2 = 0;
    dspexpr_37_in1 = 0;
    dspexpr_37_in2 = 0;
    expr_09_in1 = 0;
    expr_09_in2 = 0;
    expr_09_out1 = 0;
    expr_10_in1 = 0;
    expr_10_in2 = 100;
    expr_10_out1 = 0;
    toggle_01_value = 0;
    receive_01_output_number = 0;
    dspexpr_38_in1 = 0;
    dspexpr_38_in2 = 0;
    param_25_value = 0;
    recordtilde_01_record = 0;
    recordtilde_01_begin = 0;
    recordtilde_01_end = -1;
    recordtilde_01_loop = 0;
    send_01_input_number = 0;
    dspexpr_39_in1 = 0;
    dspexpr_39_in2 = 0;
    dspexpr_40_in1 = 0;
    dspexpr_40_in2 = 0;
    dspexpr_41_in1 = 0;
    dspexpr_41_in2 = 0;
    dspexpr_42_in1 = 0;
    dspexpr_42_in2 = 0;
    p_01_target = 0;
    dspexpr_43_in1 = 0;
    dspexpr_43_in2 = 0;
    param_26_value = 100;
    p_02_target = 0;
    param_27_value = 0;
    param_28_value = 1;
    send_02_input_number = 0;
    p_03_target = 0;
    param_29_value = 1;
    numbertilde_01_input_number = 0;
    numbertilde_01_ramp = 0;
    dspexpr_44_in1 = 0;
    dspexpr_44_in2 = 0;
    dspexpr_45_in1 = 0;
    dspexpr_45_in2 = 0;
    param_30_value = 0;
    p_04_target = 0;
    numbertilde_02_input_number = 0;
    numbertilde_02_ramp = 0;
    dspexpr_46_in1 = 0;
    dspexpr_46_in2 = 0;
    dspexpr_47_in1 = 0;
    dspexpr_47_in2 = 0;
    param_31_value = 1;
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
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    param_25_lastValue = 0;
    recordtilde_01_wIndex = 0;
    recordtilde_01_lastRecord = 0;
    param_26_lastValue = 0;
    param_27_lastValue = 0;
    param_28_lastValue = 0;
    param_29_lastValue = 0;
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
    param_30_lastValue = 0;
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
    param_31_lastValue = 0;
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

    number dspexpr_33_in1;
    number dspexpr_33_in2;
    number dspexpr_34_in1;
    number dspexpr_34_in2;
    number dspexpr_35_in1;
    number dspexpr_35_in2;
    number dspexpr_36_in1;
    number dspexpr_36_in2;
    number dspexpr_37_in1;
    number dspexpr_37_in2;
    number expr_09_in1;
    number expr_09_in2;
    number expr_09_out1;
    number expr_10_in1;
    number expr_10_in2;
    number expr_10_out1;
    number toggle_01_value;
    number receive_01_output_number;
    list receive_01_output_list;
    number dspexpr_38_in1;
    number dspexpr_38_in2;
    number param_25_value;
    number recordtilde_01_record;
    number recordtilde_01_begin;
    number recordtilde_01_end;
    number recordtilde_01_loop;
    number send_01_input_number;
    list send_01_input_list;
    number dspexpr_39_in1;
    number dspexpr_39_in2;
    number dspexpr_40_in1;
    number dspexpr_40_in2;
    number dspexpr_41_in1;
    number dspexpr_41_in2;
    number dspexpr_42_in1;
    number dspexpr_42_in2;
    number p_01_target;
    number dspexpr_43_in1;
    number dspexpr_43_in2;
    number param_26_value;
    number p_02_target;
    number param_27_value;
    number param_28_value;
    number send_02_input_number;
    list send_02_input_list;
    number p_03_target;
    number param_29_value;
    number numbertilde_01_input_number;
    number numbertilde_01_ramp;
    number dspexpr_44_in1;
    number dspexpr_44_in2;
    number dspexpr_45_in1;
    number dspexpr_45_in2;
    number param_30_value;
    number p_04_target;
    number numbertilde_02_input_number;
    number numbertilde_02_ramp;
    number dspexpr_46_in1;
    number dspexpr_46_in2;
    number dspexpr_47_in1;
    number dspexpr_47_in2;
    number param_31_value;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[5];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number param_25_lastValue;
    Float32MultiBufferRef recordtilde_01_buffer;
    SampleIndex recordtilde_01_wIndex;
    number recordtilde_01_lastRecord;
    number param_26_lastValue;
    number param_27_lastValue;
    number param_28_lastValue;
    number param_29_lastValue;
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
    number param_30_lastValue;
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
    number param_31_lastValue;
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
    DataRef layer3;
    DataRef layer2;
    DataRef layer4;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_98* p_01;
    RNBOSubpatcher_99* p_02;
    RNBOSubpatcher_100* p_03;
    RNBOSubpatcher_101* p_04;

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

