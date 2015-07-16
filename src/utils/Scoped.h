/* Copyright 2015 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

// include BaseUtil.h instead of including directly

// auto-free memory for arbitrary malloc()ed memory of type T*
template <typename T>
class ScopedMem
{
    T *ptr;
public:
    ScopedMem() : ptr(nullptr) {}
    explicit ScopedMem(T* ptr) : ptr(ptr) {}
    ~ScopedMem() { free(ptr); }
    void Set(T *newPtr) {
        free(ptr);
        ptr = newPtr;
    }
    ScopedMem& operator=(T* newPtr) {
        free(ptr);
        ptr = newPtr;
        return *this;
    }
    T *Get() const { return ptr; }
    T *StealData() {
        T *tmp = ptr;
        ptr = nullptr;
        return tmp;
    }
    operator T*() const { return ptr; }
};

// deletes any object at the end of the scope
template <class T>
class ScopedPtr
{
    T *obj;
public:
    ScopedPtr() : obj(nullptr) {}
    explicit ScopedPtr(T* obj) : obj(obj) {}
    ~ScopedPtr() { delete obj; }
    T *Detach() {
        T *tmp = obj;
        obj = nullptr;
        return tmp;
    }
    operator T*() const { return obj; }
    T* operator->() const { return obj; }
    T* operator=(T* newObj) {
        delete obj;
        return (obj = newObj);
    }
};

#if defined(BUILD_FOR_WIN32)
#include "Scoped_win.h"
#endif

