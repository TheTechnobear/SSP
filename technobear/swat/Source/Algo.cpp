#include "Algo.h"

#include "../JuceLibraryCode/JuceHeader.h"

// simple test, 
// a = x * y + z + x_+ z_ 
// y = x * y + z + y_+ z_ 
void Algo_Test::process(
    const float* x, const float* y, const float* z,
    float* a, float* b, 
    unsigned n) {

    if(a!=nullptr) {
        if(x) FloatVectorOperations::copy(a,x,n);
        else FloatVectorOperations::fill(a,0.0,n);
        if(y) FloatVectorOperations::multiply(a, y, n);
        if(z) FloatVectorOperations::add(a, z, n);
        FloatVectorOperations::add(a,x_,n);
        FloatVectorOperations::add(a,z_,n);
    }


    if(b!=nullptr) {
        if(x) FloatVectorOperations::copy(b,x,n);
        else FloatVectorOperations::fill(b,0.0,n);
        if(y) FloatVectorOperations::multiply(b, y, n);
        if(z) FloatVectorOperations::add(b, z, n);
        FloatVectorOperations::add(b,z_,n);
        FloatVectorOperations::add(b,y_,n);
    }
}

