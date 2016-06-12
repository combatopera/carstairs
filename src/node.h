#pragma once

#include "dssi/plugin.h"
#include "state.h"
#include "util/buf.h"

class Maskable {

public:

    virtual ~Maskable() {
    }

    virtual void catchUp(DSSI::cursor) = 0;

};

template<typename T> class Node: public Maskable {

    DSSI::cursor _cursor = -1;

    inline void catchUpImpl(DSSI::cursor newCursor, bool makeData) {
        if (_cursor < newCursor) {
            _buf.setLimit(sizex(newCursor - _cursor));
            renderImpl(); // TODO LATER: Only actually make the data in makeData case.
            _cursor = newCursor;
        }
    }

public:

    Node(char const *label, State const& state)
            : _buf(label), _state(state) {
        // Nothing else.
    }

    virtual ~Node() {
        // Do nothing.
    }

    void start() {
        _cursor = 0;
        startImpl();
    }

    void catchUp(DSSI::cursor newCursor) {
        catchUpImpl(newCursor, false);
    }

    View<T> render(DSSI::cursor newCursor) {
        catchUpImpl(newCursor, true);
        return _buf; // TODO: Enforce return of current buf when nothing to render.
    }

    DSSI::cursor cursor() {
        return _cursor;
    }

protected:

    Buffer<T> _buf;

    State const& _state;

    virtual void startImpl() = 0;

    virtual void renderImpl() = 0;

};

#define CARSTAIRS_NODE_INSTANTIATE(T) \
    template Node<T>::Node(char const *, State const&); \
    template Node<T>::~Node(); \
    template View<T> Node<T>::render(DSSI::cursor);
