
#ifndef _UTIL_UTIL_H_
#define _UTIL_UTIL_H_



// Undefine macros first, just in case. Some third-party includes have their own
// version.

#undef DISALLOW_ASSIGN
#define DISALLOW_ASSIGN(TypeName) \
void operator=(const TypeName&)

// A macro to disallow the evil copy constructor and operator= functions
// This should be used in the private: declarations for a class.
#undef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
TypeName(const TypeName&);                    \
DISALLOW_ASSIGN(TypeName)

// Alternative, less-accurate legacy name.
#undef DISALLOW_EVIL_CONSTRUCTORS
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) \
DISALLOW_COPY_AND_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#undef DISALLOW_IMPLICIT_CONSTRUCTORS
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
TypeName();                                    \
DISALLOW_EVIL_CONSTRUCTORS(TypeName)


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

