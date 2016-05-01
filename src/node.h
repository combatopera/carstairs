#pragma once

#include "state.h"
#include "util.h"

template<typename T> class Node {

public:

    Node(State *state);

    virtual ~Node();

    virtual View<T> render(unsigned long newCursor);

protected:

    Buffer<T> _buf;

    State *_state;

    unsigned long _cursor;

    virtual void renderImpl() = 0;

};

#define NODE_INSTANTIATE(T) \
    template Node<T>::Node(State *); \
    template Node<T>::~Node();
