#pragma once 

#include "Percussa.h"



#include <algorithm>


extern const char* percussaParamsName[Percussa::sspLast];

extern const char* percussaParamsText[Percussa::sspLast];

inline float constrainFloat(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

inline float sspCv2Pitch(float r) {
    // from SSP SDK
    static constexpr float p1 = 0.02325f; // first C note
    static constexpr float p2 = 0.21187f; // second C note
    static constexpr float scale = 12.0f / (p2 - p1);
    float arg = r;
    arg = arg - p1;
    arg = arg * scale;
    return arg;
}
