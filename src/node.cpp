#include "node.h"

#include <stdlib.h>

#include "util.h"

template<typename T>
Node<T>::Node()
        : _cursor(0) {
    _buffer = (T *) malloc(_capacity = 0);
}

template<typename T>
Node<T>::~Node() {
    free(_buffer);
}

template<typename T>
T *Node<T>::render(unsigned long newCursor) {
    if (_cursor < newCursor) {
        unsigned long blockSize = newCursor - _cursor;
        _cursor = newCursor;
        if (blockSize > _capacity) {
            _buffer = (T *) realloc(_buffer, _capacity = (blockSize * sizeof *_buffer));
        }
        renderImpl(_buffer,blockSize);
    }
    return _buffer;
}

Tone::Tone():Node() {

}

Tone::~Tone() {

}

int *Tone::render(unsigned long newCursor) {
    return Node::render(newCursor);
}

void Tone::renderImpl(int *buffer, unsigned long n) {
    zero(buffer, n);
}

PCM::PCM():Node() {

}

PCM::~PCM() {

}

LADSPA_Data *PCM::render(unsigned long newCursor) {
    return Node::render(newCursor);
}

void PCM::renderImpl(LADSPA_Data * buffer, unsigned long n) {
    zero(buffer, n);
}