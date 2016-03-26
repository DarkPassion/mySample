
#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_

/**
 * disable copy constructor of class,
 * to avoid the memory leak or corrupt by copy instance.
 */
#define disable_default_copy(className)\
private:\
/** \
* disable the copy constructor and operator=, donot allow directly copy. \
*/ \
className(const className&); \
className& operator= (const className&)

#define  freep(p) \
    if(p) { \
        delete p; \
        p = NULL; \
    }

#define freepa (pa) \
    if (pa) { \
        delete[] pa; \
        pa = NULL; \
    }


template <class T>
class Imp_AutoFree
{
private:
    T** _ptr;
    bool _is_array;
    
public:
    Imp_AutoFree(T** ptr, bool is_array);
    
    ~Imp_AutoFree();
};



#endif

