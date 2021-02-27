#pragma once

static constexpr unsigned MAX_SCALES = 87;
static constexpr unsigned MAX_TONICS = 12;

class Quantizer {
public:
    Quantizer() { ; }

    ~Quantizer() { ; }

    void quantize(unsigned root_, unsigned scale_, int &oct, unsigned &semi);

    const char *getScaleName(unsigned i);
    const char *getTonicName(unsigned i);
};



