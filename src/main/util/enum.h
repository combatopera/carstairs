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

#include "buf.h"

template<typename T> class Values {

    T * const _first;

public:

    sizex const length;

    Values(T *first, sizex ordCursor);

    T *operator[](sizex i) const {
        return _first + i; // Assume the objects are right next to each other.
    }

};

#define CARSTAIRS_ENUM_INSTANTIATE(T) \
    template T *Values<T>::operator[](sizex) const; \
    template Values<T>::Values(T *, sizex);
