/* Copyright 2015 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

// note: include BaseUtil.h instead of including directly

/* Simple but also optimized for small sizes vector/array class that can
store pointer types or POD types
(http://stackoverflow.com/questions/146452/what-are-pod-types-in-c).

We always pad the elements with a single 0 value. This makes
Vec<char> and Vec<WCHAR> a C-compatible string. Although it's
not useful for other types, the code is simpler if we always do it
(rather than have it an optional behavior).
*/
template <typename T>
class Vec {
protected:
    static const size_t PADDING = 1;

    size_t      len;
    size_t      cap;
    size_t      capacityHint;
    T *         els;
    T           buf[16];
    Allocator * allocator;

    bool EnsureCapTry(size_t needed) {
        if (cap >= needed)
            return true;

        size_t newCap = cap * 2;
        if (needed > newCap)
            newCap = needed;
        if (newCap < capacityHint)
            newCap = capacityHint;

        size_t newElCount = newCap + PADDING;
        if (newElCount >= SIZE_MAX / sizeof(T))
            return false;
        if (newElCount > INT_MAX) // limitation of Vec::Find
            return false;

        size_t allocSize = newElCount * sizeof(T);
        size_t newPadding = allocSize - len * sizeof(T);
        T *newEls;
        if (buf == els)
            newEls = (T *)Allocator::Dup(allocator, buf, len * sizeof(T), newPadding);
        else
            newEls = (T *)Allocator::Realloc(allocator, els, allocSize);
        if (!newEls)
            return false;
        els = newEls;
        memset(els + len, 0, newPadding);
        cap = newCap;
        return true;
    }

    void EnsureCapCrash(size_t needed) {
        bool ok = EnsureCapTry(needed);
        CrashAlwaysIf(!ok);
    }

    T* MakeSpaceAt(size_t idx, size_t count, bool allowFailure=false) {
        size_t newLen = std::max(len, idx) + count;
        if (!allowFailure)
            EnsureCapCrash(newLen);
        else if (!EnsureCapTry(newLen))
            return nullptr;
        T* res = &(els[idx]);
        if (len > idx) {
            T* src = els + idx;
            T* dst = els + idx + count;
            memmove(dst, src, (len - idx) * sizeof(T));
        }
        len = newLen;
        return res;
    }

    void FreeEls() {
        if (els != buf)
            Allocator::Free(allocator, els);
    }

public:
    // allocator is not owned by Vec and must outlive it
    explicit Vec(size_t capHint=0, Allocator *allocator=nullptr) :
        capacityHint(capHint), allocator(allocator)
    {
        els = buf;
        Reset();
    }

    ~Vec() {
        FreeEls();
    }

    // ensure that a Vec never shares its els buffer with another after a clone/copy
    // note: we don't inherit allocator as it's not needed for our use cases
    Vec(const Vec& orig) : capacityHint(0), allocator(nullptr) {
        els = buf;
        Reset();
        EnsureCapCrash(orig.cap);
        // use memcpy, as Vec only supports POD types
        memcpy(els, orig.els, sizeof(T) * (len = orig.len));
    }

    Vec& operator=(const Vec& that) {
        if (this != &that) {
            EnsureCapCrash(that.cap);
            // use memcpy, as Vec only supports POD types
            memcpy(els, that.els, sizeof(T) * (len = that.len));
            memset(els + len, 0, sizeof(T) * (cap - len));
        }
        return *this;
    }

    T& operator[](size_t idx) {
        CrashIf(idx >= len);
        return els[idx];
    }

    void Reset() {
        len = 0;
        cap = dimof(buf) - PADDING;
        FreeEls();
        els = buf;
        memset(buf, 0, sizeof(buf));
    }

    // use &At() if you need a pointer to the element (e.g. if T is a struct)
    T& At(size_t idx) const {
        CrashIf(idx >= len);
        return els[idx];
    }

    T *AtPtr(size_t idx) const {
        CrashIf(idx >= len);
        CrashIf(&els[idx] != &At(idx));
        return &els[idx];
    }

    size_t Count() const {
        return len;
    }

    size_t Size() const {
        return len;
    }

    void InsertAt(size_t idx, const T& el) {
        MakeSpaceAt(idx, 1)[0] = el;
    }

    void Append(const T& el) {
        InsertAt(len, el);
    }

    void Append(const T* src, size_t count) {
        if (0 == count)
            return;
        T* dst = MakeSpaceAt(len, count);
        memcpy(dst, src, count * sizeof(T));
    }

    // returns false on allocation failure instead of crashing
    bool AppendChecked(const T* src, size_t count) {
        if (0 == count)
            return true;
        T* dst = MakeSpaceAt(len, count, true);
        if (dst)
            memcpy(dst, src, count * sizeof(T));
        return dst != nullptr;
    }

    // appends count blank (i.e. zeroed-out) elements at the end
    T* AppendBlanks(size_t count) {
        return MakeSpaceAt(len, count);
    }

    void RemoveAt(size_t idx, size_t count=1) {
        if (len > idx + count) {
            T *dst = els + idx;
            T *src = els + idx + count;
            memmove(dst, src, (len - idx - count) * sizeof(T));
        }
        len -= count;
        memset(els + len, 0, count * sizeof(T));
    }

    // This is a fast version of RemoveAt() which replaces the element we're
    // removing with the last element, copying less memory.
    // It can only be used if order of elements doesn't matter and elements
    // can be copied via memcpy()
    // TODO: could be extend to take number of elements to remove
    void RemoveAtFast(size_t idx) {
        CrashIf(idx >= len);
        if (idx >= len) return;
        T *toRemove = els + idx;
        T *last = els + len - 1;
        if (toRemove != last)
            memcpy(toRemove, last, sizeof(T));
        memset(last, 0, sizeof(T));
        --len;
    }

    void Push(T el) {
        Append(el);
    }

    T Pop() {
        CrashIf(0 == len);
        T el = At(len - 1);
        RemoveAtFast(len - 1);
        return el;
    }

    T PopAt(size_t idx) {
        CrashIf(idx >= len);
        T el = At(idx);
        RemoveAt(idx);
        return el;
    }

    T& Last() const {
        CrashIf(0 == len);
        return At(len - 1);
    }

    // perf hack for using as a buffer: client can get accumulated data
    // without duplicate allocation. Note: since Vec over-allocates, this
    // is likely to use more memory than strictly necessary, but in most cases
    // it doesn't matter
    T *StealData() {
        T* res = els;
        if (els == buf)
            res = (T *)Allocator::Dup(allocator, buf, (len + PADDING) * sizeof(T));
        els = buf;
        Reset();
        return res;
    }

    T *LendData() const {
        return els;
    }

    int Find(T el, size_t startAt=0) const {
        for (size_t i = startAt; i < len; i++) {
            if (els[i] == el)
                return (int)i;
        }
        return -1;
    }

    bool Contains(T el) const {
        return -1 != Find(el);
    }

    // returns true if removed
    bool Remove(T el) {
        int i = Find(el);
        if (-1 == i)
            return false;
        RemoveAt(i);
        return true;
    }

    void Sort(int (*cmpFunc)(const void *a, const void *b)) {
        qsort(els, len, sizeof(T), cmpFunc);
    }

    void Reverse() {
        for (size_t i = 0; i < len / 2; i++) {
            std::swap(els[i], els[len - i - 1]);
        }
    }

    T& FindEl(const std::function<bool (T&)> check) {
        for (size_t i = 0; i < len; i++) {
            if (check(els[i]))
                return els[i];
        }
        return els[len]; // nullptr-sentinel
    }

    // cf. http://www.cprogramming.com/c++11/c++11-ranged-for-loop.html
    class Iter {
        Vec<T> *vec;
        size_t pos;

    public:
        Iter(Vec<T> *vec, size_t pos) : vec(vec), pos(pos) { }

        bool operator!=(const Iter& other) const {
            return pos != other.pos;
        }
        T& operator*() const {
            return vec->At(pos);
        }
        Iter& operator++() {
            pos++;
            return *this;
        }
    };

    Iter begin() {
        return Iter(this, 0);
    }
    Iter end() {
        return Iter(this, len);
    }
};

// only suitable for T that are pointers that were malloc()ed
template <typename T>
inline void FreeVecMembers(Vec<T>& v)
{
    for (T& el : v) {
        free(el);
    }
    v.Reset();
}

// only suitable for T that are pointers to C++ objects
template <typename T>
inline void DeleteVecMembers(Vec<T>& v)
{
    for (T& el : v) {
        delete el;
    }
    v.Reset();
}

namespace str {

template <typename T>
class Str : public Vec<T> {
public:
    explicit Str(size_t capHint=0, Allocator *allocator=nullptr) : Vec<T>(capHint, allocator) { }

    void Append(T c)
    {
        Vec<T>::InsertAt(Vec<T>::len, c);
    }

    void Append(const T* src, size_t size=-1)
    {
        if ((size_t)-1 == size)
            size = Len(src);
        Vec<T>::Append(src, size);
    }

    void AppendFmt(const T* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        T *res = FmtV(fmt, args);
        AppendAndFree(res);
        va_end(args);
    }

    void AppendAndFree(T* s)
    {
        if (s)
            Append(s);
        free(s);
    }

    // returns true if was replaced
    bool Replace(const T *toReplace, const T *replaceWith)
    {
        // fast path: nothing to replace
        if (!str::Find(Vec<T>::els, toReplace))
            return false;
        char *newStr = str::Replace(Vec<T>::els, toReplace, replaceWith);
        Vec<T>::Reset();
        AppendAndFree(newStr);
        return true;
    }

    void Set(const T* s)
    {
        Vec<T>::Reset();
        Append(s);
    }

    T *Get() const
    {
        return Vec<T>::els;
    }
};

} // namespace str

#ifdef _WIN32
#include "Vec_win.h"
#endif


