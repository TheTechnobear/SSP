#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"

#include "algos/Algos.h"

std::shared_ptr<Algo> PluginProcessor::createAlgo(unsigned a) {
    switch (a) {
        case A_DISPLAY      :
            return std::make_shared<AgDisplay>();
        case A_CONSTANT     :
            return std::make_shared<AgConstant>();
        case A_P_ADDER      :
            return std::make_shared<AgPrecAdder>();
        case A_MIN_MAX      :
            return std::make_shared<AgMinMax>();
        case A_SWITCH       :
            return std::make_shared<AgSwitch>();
        case A_COMPARATOR_W :
            return std::make_shared<AgComparatorW>();
        case A_TRANSPOSE    :
            return std::make_shared<AgTranspose>();
        case A_COMPARATOR_N :
            return std::make_shared<AgComparatorN>();
        case A_DELAY        :
            return std::make_shared<AgDelay>();
        case A_LOGIC_AND    :
            return std::make_shared<AgLogicAnd>();
        case A_LOGIC_OR     :
            return std::make_shared<AgLogicOr>();
        case A_LOGIC_XOR    :
            return std::make_shared<AgLogicXor>();
        case A_MAP_VV       :
            return std::make_shared<AgMapVV>();
        case A_MAP_NV       :
            return std::make_shared<AgMapNV>();
        case A_MAP_NN       :
            return std::make_shared<AgMapNN>();
        case A_COMPARATOR   :
            return std::make_shared<AgComparator>();
        case A_COUNTER      :
            return std::make_shared<AgCounter>();
        case A_S_DELAY      :
            return std::make_shared<AgSDelay>();
        default:
            assert(false);
    }
    return std::make_shared<AgDisplay>();
}


PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();

    algoDisplayOrder_.push_back(A_DISPLAY);
    algoDisplayOrder_.push_back(A_TRANSPOSE);
    algoDisplayOrder_.push_back(A_COMPARATOR);
    algoDisplayOrder_.push_back(A_COMPARATOR_W);
    algoDisplayOrder_.push_back(A_COMPARATOR_N);
    algoDisplayOrder_.push_back(A_MAP_VV);
    algoDisplayOrder_.push_back(A_MAP_NV);
    algoDisplayOrder_.push_back(A_MAP_NN);
    algoDisplayOrder_.push_back(A_MIN_MAX);
    algoDisplayOrder_.push_back(A_LOGIC_AND);
    algoDisplayOrder_.push_back(A_LOGIC_OR);
    algoDisplayOrder_.push_back(A_LOGIC_XOR);
    algoDisplayOrder_.push_back(A_SWITCH);
    algoDisplayOrder_.push_back(A_DELAY);
    algoDisplayOrder_.push_back(A_S_DELAY);
    algoDisplayOrder_.push_back(A_P_ADDER);
    algoDisplayOrder_.push_back(A_COUNTER);
    algoDisplayOrder_.push_back(A_CONSTANT);

    assert(algoDisplayOrder_.size() == A_MAX);

    for (auto e = 0; e < MAX_ENG; e++) {
        algo_[e] = createAlgo(A_DISPLAY);
    }
}

PluginProcessor::~PluginProcessor() {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);
    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "X 1",
        "Y 1",
        "Z 1",
        "X 2",
        "Y 2",
        "Z 2",
        "X 3",
        "Y 3",
        "Z 3",
        "X 4",
        "Y 4",
        "Z 4"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "A 1",
        "B 1",
        "A 2",
        "B 2",
        "A 3",
        "B 3",
        "A 4",
        "B 4"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    Algo::setSampleRate(sampleRate);
    outBufs_.setSize(2 * MAX_ENG, samplesPerBlock);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned n = buffer.getNumSamples();

    for (auto e = 0; e < MAX_ENG; e++) {
        unsigned sigi = e * 3;
        unsigned sigo = e * 2;

        bool outEnabledA = outputEnabled[O_A_1 + sigo];
        bool outEnabledB = outputEnabled[+O_B_1 + sigo];

        bool inEnabledX = inputEnabled[I_X_1 + sigi];
        bool inEnabledY = inputEnabled[I_Y_1 + sigi];
        bool inEnabledZ = inputEnabled[I_Z_1 + sigi];

        float *outA = outEnabledA ? outBufs_.getWritePointer(sigo) : nullptr;
        float *outB = outEnabledB ? outBufs_.getWritePointer(sigo + 1) : nullptr;

        const float *inX = inEnabledX ? buffer.getReadPointer(I_X_1 + sigi) : nullptr;
        const float *inY = inEnabledY ? buffer.getReadPointer(I_Y_1 + sigi) : nullptr;
        const float *inZ = inEnabledZ ? buffer.getReadPointer(I_Z_1 + sigi) : nullptr;

        algo_[e]->process(inX, inY, inZ, outA, outB, n);

        if (outEnabledA) {
            buffer.copyFrom(O_A_1 + sigo, 0, outBufs_, sigo, 0, n);
        } else {
            buffer.applyGain(O_A_1 + sigo, 0, n, 0.0f);
        }

        if (outEnabledB) {
            buffer.copyFrom(O_B_1 + sigo, 0, outBufs_, sigo + 1, 0, n);
        } else {
            buffer.applyGain(O_B_1 + sigo, 0, n, 0.0f);
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this,new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


void PluginProcessor::writeToXml(XmlElement &xml) {
    for (auto e = 0; e < MAX_ENG; e++) {
        auto axml = xml.createNewChildElement("algo" + String(e + 1));
        axml->setAttribute("algo", int(algo_[e]->type()));
        algo_[e]->writeToXml(*axml);
    }
}

void PluginProcessor::readFromXml(XmlElement &xml) {
    for (auto e = 0; e < MAX_ENG; e++) {
        auto axml = xml.getChildByName("algo" + String(e + 1));
        if (axml) {
            unsigned t = axml->getIntAttribute("algo", 0) % A_MAX;
            algo_[e] = createAlgo(t);
            algo_[e]->readFromXml(*axml);
        } else {
            // Logger::writeToLog("algo1 not found");
            algo_[e] = createAlgo(A_DISPLAY);
        }
    }
}


void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    static const char *xmlTag = JucePlugin_Name;
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    auto pXML = getXmlFromBinary(data, sizeInBytes);
    if (pXML) {
        readFromXml(*pXML);
    }
}
