#pragma once

#include <python3.4m/Python.h>
#include <cassert>

#include "config.h"
#include "state.h"
#include "util/util.h"

class Program: public Fire {

    char const * const _moduleName;

    PyThreadState *_parent, *_interpreter;

    PyObject *_module;

    float _rate;

    void load() {
        debug("Creating new sub-interpreter.");
        _interpreter = Py_NewInterpreter();
        assert(_interpreter);
        assert(_parent != _interpreter);
        assert(PyThreadState_Get() == _interpreter);
        _rate = 50; // Default.
        debug("Loading module: %s", _moduleName);
        _module = PyImport_ImportModule(_moduleName);
        if (_module) {
            auto const ratePtr = PyObject_GetAttrString(_module, "rate");
            if (ratePtr) {
                _rate = float(PyFloat_AsDouble(ratePtr));
                debug("Program rate: %.3f", _rate);
                Py_DECREF(ratePtr);
            }
        }
        else {
            debug("Failed to load: %s", _moduleName);
        }
    }

    void unload() {
        debug("Ending sub-interpreter.");
        if (_module) {
            Py_DECREF(_module);
            _module = 0;
        }
        Py_EndInterpreter(_interpreter);
    }

public:

    Program(Config const& config)
            : _moduleName(config._programModule) {
        debug("Initing Python.");
        Py_InitializeEx(0);
        _parent = PyThreadState_Get();
        assert(_parent);
        load();
    }

    void reload() {
        unload();
        load();
    }

    float rate() const {
        return _rate;
    }

    void fire(int, int, State&) const;

    ~Program() {
        unload();
        PyThreadState_Swap(_parent); // Otherwise Py_Finalize crashes.
        debug("Closing Python.");
        Py_Finalize();
    }

};
