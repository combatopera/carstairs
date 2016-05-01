#pragma once

#include <ladspa.h>
#include <stddef.h>

template<typename T> class Node {

    unsigned long _cursor;

    size_t _capacity;

public:

    Node();

    virtual ~Node();

    virtual T *render(unsigned long newCursor);

protected:

    T *_buffer;

    virtual void renderImpl(unsigned long n) = 0;

};

class Tone: public Node<int> {

    void renderImpl(unsigned long n);

};

class PCM: public Node<LADSPA_Data> {

    Tone _tone;

    void renderImpl(unsigned long n);

};
