#include "plugin.h"

#include <ladspa.h>

#include "../dizzYM.h"
#include "port.h"

#ifdef DEBUG_dizzYM
#include <iostream>
#endif

static LADSPA_Handle instantiate(const LADSPA_Descriptor *Descriptor, unsigned long SampleRate) {
#ifdef DEBUG_dizzYM
    std::cerr << "[dizzYM] Instantiating." << std::endl;
#endif
    return new dizzYM((int) SampleRate);
}

static void activate(LADSPA_Handle Instance) {
#ifdef DEBUG_dizzYM
    std::cerr << "[dizzYM] Activating." << std::endl;
#endif
    ((dizzYM *) Instance)->reset();
}

static int get_midi_controller_for_port(LADSPA_Handle, unsigned long Port) {
    return PORT_INFOS[Port]->_controllers;
}

static void cleanup(LADSPA_Handle Instance) {
    delete (dizzYM *) Instance;
#ifdef DEBUG_dizzYM
    std::cerr << "[dizzYM] Cleaned up." << std::endl;
#endif
}

Descriptor::Descriptor() {
    _PortDescriptors = new LADSPA_PortDescriptor[PortCount];
    _PortNames = new const char *[PortCount];
    _PortRangeHints = new LADSPA_PortRangeHint[PortCount];
    for (unsigned i = 0; i < PortCount; ++i) {
        _PortDescriptors[i] = PORT_INFOS[i]->_descriptor;
        _PortNames[i] = PORT_INFOS[i]->_name;
        _PortRangeHints[i] = PORT_INFOS[i]->_rangeHint; // Copy.
    }
    _ladspaDescriptor = { //
        0,// UniqueID
        "dizzYM",// Label
        LADSPA_PROPERTY_HARD_RT_CAPABLE,// Properties
        "YM2149",// Name
        "Andrzej Cichocki",// Maker
        "Andrzej Cichocki",// Copyright
        PortCount,//
        _PortDescriptors,//
        _PortNames,//
        _PortRangeHints,//
        0,// ImplementationData
        instantiate,//
        dizzYM::connect_port,//
        activate,//
        dizzYM::run,//
        0,// run_adding
        0,// set_run_adding_gain
        0,// deactivate
        cleanup,//
    };
    _dssiDescriptor = { //
        1,// API version, must be 1.
        &_ladspaDescriptor,//
        0,// configure()
        0,// get_program()
        0,// select_program()
        get_midi_controller_for_port,//
        dizzYM::run_synth,//
        0,// run_synth_adding()
        0,// run_multiple_synths()
        0,// run_multiple_synths_adding()
    };
}

Descriptor::~Descriptor() {
    delete[] _PortDescriptors;
    delete[] _PortNames;
    delete[] _PortRangeHints;
}

static Descriptor descriptor;

extern "C" {

const DSSI_Descriptor *dssi_descriptor(unsigned long index) {
    if (index == 0) {
        return &descriptor._dssiDescriptor;
    }
    else {
        return 0;
    }
}

}
