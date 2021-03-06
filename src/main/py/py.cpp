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

#include "py.h"

#include <Python.h>

#include "../util/util.h"

namespace {
Log const LOG(__FILE__);
}

PyRef& PyRef::operator=(PyObject * const ptr) {
    CARSTAIRS_REFRESH(PyRef, ptr);
}

PyRef& PyRef::operator=(PyRef const& that) {
    Py_XINCREF(that._ptr);
    CARSTAIRS_REFRESH(PyRef, that._ptr);
}

PyRef::~PyRef() {
    if (_ptr) {
        CARSTAIRS_TRACE("DECREF: %p * %zd", _ptr, Py_REFCNT(_ptr));
        Py_DECREF(_ptr);
    }
}
