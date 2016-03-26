#include <stdlib.h>
#include "util.h"



template <class T>
Imp_AutoFree<T>::Imp_AutoFree(T** ptr, bool is_array)
{
    _ptr = ptr;
    _is_array = is_array;
}


template <class T>
Imp_AutoFree<T>::~Imp_AutoFree()
{
    if (_ptr == NULL || *_ptr == NULL) {
        return;
    }

    if (_is_array) {
        delete[] *_ptr;
    } else {
        delete *_ptr;
    }
    
    *_ptr = NULL;
    
}

