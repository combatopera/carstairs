#include "node.h"

#include <ladspa.h>

#include "state.h"
#include "util.h"

template<typename T> Node<T>::Node(const char *label, State *state)
        : _buf(label), _state(state), _cursor(0) {
    // Nothing else.
}

template<typename T> Node<T>::~Node() {
    // Do nothing.
}

template<typename T> View<T> Node<T>::render(unsigned long newCursor) {
    if (_cursor < newCursor) {
        _buf.setLimit(newCursor - _cursor);
        renderImpl();
        _cursor = newCursor;
    }
    return _buf;
}

NODE_INSTANTIATE(int)
NODE_INSTANTIATE(LADSPA_Data)
