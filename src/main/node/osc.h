// Copyright 2016 Andrzej Cichocki

// This file is part of Carstairs.
//
// Carstairs is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Carstairs is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Carstairs.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "../node.h"
#include "../state.h"
#include "../util/buf.h"

class Shape {

protected:

#ifdef CARSTAIRS_TEST
public:
#endif

    Buffer<int> _data;

    sizex const _introLen, _last;

public:

    Shape(char const *label, sizex limit, sizex introLen = 0)
        : _data(label, limit), _introLen(introLen), _last(limit - 1) {
    }

    sizex next(sizex index) const {
        return _last == index ? _introLen : index + 1;
    }

    Buffer<int> const& data() const {
        return _data;
    }

};

class Osc: public Node<int> {

    sizex const _atomSize;

    Shape const *_shape;

    int const& _period;

    bool const _eagerStepSize;

#ifdef CARSTAIRS_TEST
public:
#endif

    sizex _indexInShape;

    int _progress, _stepSize;

private:

    void startImpl() {
        _indexInShape = 0;
        _progress = 0;
    }

    inline void updateStepSize() {
        _stepSize = _atomSize * _period;
    }

    void renderImpl() {
        auto const& shapeData = _shape->data();
        auto const n = _buf.limit();
        if (_eagerStepSize || !_progress) {
            updateStepSize();
        }
        if (_progress >= _stepSize) { // Start a new step.
            _indexInShape = _shape->next(_indexInShape);
            _progress = 0;
        }
        sizex endOfStep = _stepSize - _progress, i = 0;
        auto ptr = const_cast<int *>(_buf.begin());
        if (endOfStep <= n) {
            if (!_eagerStepSize) {
                updateStepSize();
            }
            do {
                auto const val = shapeData.at(_indexInShape);
                for (; i < endOfStep; ++i) {
                    *ptr++ = val;
                }
                _indexInShape = _shape->next(_indexInShape);
                endOfStep += _stepSize;
            } while (endOfStep <= n);
        }
        auto const val = shapeData.at(_indexInShape);
        for (; i < n; ++i) {
            *ptr++ = val;
        }
        _progress = _stepSize - (endOfStep - n);
    }

protected:

    void setShape(Shape const& shape) {
        _shape = &shape;
        startImpl();
    }

    Osc(sizex atomSize, State const& state, char const *label, Shape const& shape, int const& period, bool eagerStepSize)
        : Node(label, state), _atomSize(atomSize), _shape(&shape), _period(period), _eagerStepSize(eagerStepSize), //
          _indexInShape(), _progress(), _stepSize() {
    }

};
