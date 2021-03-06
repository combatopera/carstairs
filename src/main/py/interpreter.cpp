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

#include "interpreter.h"

#include <Python.h>
#include <cassert>

#include "../util/util.h"

namespace {
Log const LOG(__FILE__);
}

Interpreter::Interpreter(Python const& python, std::function<void()> const& init) {
    CARSTAIRS_DEBUG("Creating new sub-interpreter.");
    PyThreadState * const main = python;
    PyEval_AcquireThread(main);
    _state = Py_NewInterpreter();
    assert(_state);
    assert(main != _state);
    assert(PyThreadState_Get() == _state);
    init();
    PyEval_ReleaseThread(_state);
    CARSTAIRS_DEBUG("Created: %p", _state);
}

Interpreter::~Interpreter() {
    CARSTAIRS_DEBUG("Ending sub-interpreter: %p", _state);
    PyEval_AcquireThread(_state);
    CARSTAIRS_TRACE("Calling: Py_EndInterpreter(%p)", _state);
    Py_EndInterpreter(_state);
    CARSTAIRS_TRACE("Calling: PyEval_ReleaseLock");
    PyEval_ReleaseLock();
    CARSTAIRS_TRACE("Sub-interpreter ended.");
}
