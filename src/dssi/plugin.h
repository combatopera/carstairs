#pragma once

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <dssi.h>
#include <ladspa.h>
#include <climits>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

#include "../config.h"
#include "../module.h"
#include "../py/main.h"
#include "../util/buf.h"
#include "../util/enum.h"
#include "port.h"

namespace DSSI {

typedef unsigned long cursor;

cursor const CURSOR_MAX = ULONG_MAX;

}

class ProgramInfo {

protected:

    DSSI_Program_Descriptor _descriptor;

public:

    static DSSI::cursor const BANK = 0;

    ProgramInfo(sizex index, char const *name)
            : _descriptor {BANK, index, name} {
    }

    virtual ~ProgramInfo() {
    }

    sizex index() const {
        return sizex(_descriptor.Program);
    }

    DSSI_Program_Descriptor const& descriptor() const {
        return _descriptor;
    }

    virtual bool isReal() const = 0;

};

class DefaultProgramInfo: public ProgramInfo {

    std::string const _label;

public:

    DefaultProgramInfo(sizex index, char const *label)
            : ProgramInfo(index, 0), _label((boost::format("%02X %s") % index % label).str()) {
        _descriptor.Name = _label.c_str();
    }

    bool isReal() const {
        return false;
    }

};

class ProgramInfoImpl: public ProgramInfo {

    boost::filesystem::path const _path;

    std::string const _name;

public:

    ProgramInfoImpl(sizex index, boost::filesystem::path const& tmpPath, std::string const& tmpName)
            : ProgramInfo(index, 0), _path(tmpPath), _name(tmpName) { // Copies, necessarily.
        _descriptor.Name = _name.c_str(); // Use our copy of the name!
    }

    bool isReal() const {
        return true;
    }

    std::time_t lastWriteTime() const {
        return boost::filesystem::last_write_time(_path);
    }

};

class ProgramInfos {

    std::vector<std::unique_ptr<ProgramInfo>> _infos;

    void addOrLog(boost::filesystem::path const&, std::string const&);

public:

    ProgramInfos(Config const&, Module const&, Python const&);

    DSSI_Program_Descriptor const *descriptorOrNull(sizex index) const {
        return index < _infos.size() ? &_infos[index].get()->descriptor() : 0;
    }

    sizex size() const {
        return sizex(_infos.size());
    }

    ProgramInfo& operator[](sizex i) const {
        return *_infos[i].get();
    }

};

class Descriptors {

    LADSPA_PortDescriptor * const _PortDescriptors;

    char const ** const _PortNames;

    LADSPA_PortRangeHint * const _PortRangeHints;

    LADSPA_Descriptor _ladspaDescriptor;

    DSSI_Descriptor _dssiDescriptor;

    Descriptors(Descriptors const&); // Make cppcheck happy.

public:

    DSSI_Descriptor const& dssiDescriptor() const {
        return _dssiDescriptor;
    }

    Descriptors(Config const&, Values<PortInfo_t const> const&);

    ~Descriptors();

};
