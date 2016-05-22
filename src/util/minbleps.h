#pragma once

#include <stddef.h>
#include <cassert>
#include <cmath>
#include <complex>

#include "../config.h"
#include "buf.h"
#include "util.h"

class MinBLEPs {

    int const _pcmRate, _naiveRate;

#ifdef DIZZYM_UNIT_TEST

public:

    Buffer<double> _BLI {"_BLI"};

    Buffer<std::complex<double>> _realCepstrum {"_realCepstrum"};

#endif

    int const _minBLEPCount;

    Buffer<float> _minBLEPs {"_minBLEPs"};

    DSSI::cursor _pcmRelX;

    unsigned _minBLEPIndex;

public:

    MinBLEPs(Config const&);

    DSSI::cursor pcmXToNaiveX(DSSI::cursor pcmX) const {
        // First naiveX that doesn't affect pcmX prior to that given:
        return DSSI::cursor(floor(double(pcmX - 1) / _pcmRate * _naiveRate) + 1);
    }

    size_t minBLEPSize(int minBLEPIndex) const {
        return (_minBLEPs.limit() - minBLEPIndex + _minBLEPCount - 1) / _minBLEPCount;
    }

    void pastePrepare(DSSI::cursor naiveX, DSSI::cursor pcmRef) {
        auto pcmMark = double(naiveX) / _naiveRate * _pcmRate;
        // If pcmX is 1 too big due to rounding error, we simply skip _minBLEPs[0] which is close to zero:
        auto pcmX = DSSI::cursor(ceil(pcmMark));
        assert(pcmRef <= pcmX);
        _pcmRelX = pcmX - pcmRef;
        auto distance = double(pcmX) - pcmMark;
        _minBLEPIndex = unsigned(round(distance * _minBLEPCount));
    }

    size_t targetLimit() const {
        return _pcmRelX + minBLEPSize(_minBLEPIndex);
    }

    void pastePerform(float amp, View<float> target) const {
        auto targetPtr = const_cast<float *>(target.begin(_pcmRelX));
        target.begin(targetLimit()); // Bounds check.
        // The target must be big enough for a minBLEP at maximum pcmX:
        for (index_t k = _minBLEPIndex, lim = _minBLEPs.limit(); k < lim; k += _minBLEPCount) {
            *targetPtr++ += amp * _minBLEPs.at(k);
        }
        for (auto end = target.end(); targetPtr != end;) {
            *targetPtr++ += amp;
        }
    }

};
