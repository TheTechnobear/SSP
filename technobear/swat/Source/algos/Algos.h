#pragma once

enum AlgoDef {
    A_DISPLAY,      // AgDisplay    - Display
    A_CONSTANT,     // AgConstant   - Simple
    A_P_ADDER,      // AgPrecAdder  - Simple
    A_MIN_MAX,      // AgMinMax     - Simple
    A_SWITCH,       // AgSwitch     - Simple
    A_COMPARATOR_W, // AgComparatorW- Simple
    A_TRANSPOSE,    // AgTranspose  - Musical 
    A_COMPARATOR_N, // AgComparatorN- Musical 
    A_DELAY,        // AgDelay      - Delay
    A_LOGIC_AND,    // AgLogicAnd   - Logic
    A_LOGIC_OR,     // AgLogicOr    - Logic
    A_LOGIC_XOR,    // AgLogicXor   - Logic
    A_MAP_VV,       // AgMapVV      - Simple
    A_MAP_NV,       // AgMapNV      - Musical
    A_MAP_NN,       // AgMapNN      - Musical
    A_COMPARATOR,   // AgComparator - Simple
    A_COUNTER,      // AgCounter    - Simple
    A_S_DELAY,       // AgSDelay     - Delay
    A_MAX
};

#include "AgSimple.h"
#include "AgDisplay.h"
#include "AgMusical.h"
#include "AgDelay.h"
#include "AgLogic.h"
