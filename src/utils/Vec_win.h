/* Copyright 2015 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

// note: include BaseUtil.h instead of including directly

// WStrVec owns the strings in the list
class WStrVec : public Vec<WCHAR *>
{
public:
    WStrVec() : Vec() { }
    WStrVec(const WStrVec& orig) : Vec(orig) {
        // make sure not to share string pointers between StrVecs
        for (size_t i = 0; i < len; i++) {
            if (At(i))
                At(i) = str::Dup(At(i));
        }
    }
    ~WStrVec() {
        FreeVecMembers(*this);
    }

    WStrVec& operator=(const WStrVec& that) {
        if (this != &that) {
            FreeVecMembers(*this);
            Vec::operator=(that);
            for (size_t i = 0; i < that.len; i++) {
                if (At(i))
                    At(i) = str::Dup(At(i));
            }
        }
        return *this;
    }

    void Reset() {
        // FreeVecMembers calls Vec::Reset()
        FreeVecMembers(*this);
    }

    WCHAR *Join(const WCHAR *joint=nullptr) {
        str::Str<WCHAR> tmp(256);
        size_t jointLen = str::Len(joint);
        for (size_t i = 0; i < len; i++) {
            WCHAR *s = At(i);
            if (i > 0 && jointLen > 0)
                tmp.Append(joint, jointLen);
            tmp.Append(s);
        }
        return tmp.StealData();
    }

    int Find(const WCHAR *s, size_t startAt=0) const {
        for (size_t i = startAt; i < len; i++) {
            WCHAR *item = At(i);
            if (str::Eq(s, item))
                return (int)i;
        }
        return -1;
    }

    bool Contains(const WCHAR *s) const {
        return -1 != Find(s);
    }

    int FindI(const WCHAR *s, size_t startAt=0) const {
        for (size_t i = startAt; i < len; i++) {
            WCHAR *item = At(i);
            if (str::EqI(s, item))
                return (int)i;
        }
        return -1;
    }

    /* splits a string into several substrings, separated by the separator
       (optionally collapsing several consecutive separators into one);
       e.g. splitting "a,b,,c," by "," results in the list "a", "b", "", "c", ""
       (resp. "a", "b", "c" if separators are collapsed) */
    size_t Split(const WCHAR *s, const WCHAR *separator, bool collapse=false) {
        size_t start = len;
        const WCHAR *next;

        while ((next = str::Find(s, separator)) != nullptr) {
            if (!collapse || next > s)
                Append(str::DupN(s, next - s));
            s = next + str::Len(separator);
        }
        if (!collapse || *s)
            Append(str::Dup(s));

        return len - start;
    }

    void Sort() { Vec::Sort(cmpAscii); }
    void SortNatural() { Vec::Sort(cmpNatural); }

private:
    static int cmpNatural(const void *a, const void *b) {
        return str::CmpNatural(*(const WCHAR **)a, *(const WCHAR **)b);
    }

    static int cmpAscii(const void *a, const void *b) {
        return wcscmp(*(const WCHAR **)a, *(const WCHAR **)b);
    }
};

// WStrList is a subset of WStrVec that's optimized for appending and searching
// WStrList owns the strings it contains and frees them at destruction
class WStrList {
    struct Item {
        WCHAR *string;
        uint32_t hash;

        explicit Item(WCHAR *string=nullptr, uint32_t hash=0) : string(string), hash(hash) { }
    };

    Vec<Item> items;
    size_t count;
    Allocator *allocator;

    // variation of MurmurHash2 which deals with strings that are
    // mostly ASCII and should be treated case independently
    // TODO: I'm guessing would be much faster when done as MurmuserHash2I()
    // with lower-casing done in-line, without the need to allocate memory for the copy
    static uint32_t GetQuickHashI(const WCHAR *str) {
        size_t len = str::Len(str);
        ScopedMem<char> data(AllocArray<char>(len));
        WCHAR c;
        for (char *dst = data; (c = *str++) != 0; dst++) {
            *dst = (c & 0xFF80) ? 0x80 : 'A' <= c && c <= 'Z' ? (char)(c + 'a' - 'A') : (char)c;
        }
        return MurmurHash2(data, len);
    }

public:
    explicit WStrList(size_t capHint=0, Allocator *allocator=nullptr) :
        items(capHint, allocator), count(0), allocator(allocator) { }

    ~WStrList() {
        for (Item& item : items) {
            Allocator::Free(allocator, item.string);
        }
    }

    const WCHAR *At(size_t idx) const {
        return items.At(idx).string;
    }

    const WCHAR *Last() const {
        return items.Last().string;
    }

    size_t Count() const {
        return count;
    }

    // str must have been allocated by allocator and is owned by StrList
    void Append(WCHAR *str) {
        items.Append(Item(str, GetQuickHashI(str)));
        count++;
    }

    int Find(const WCHAR *str, size_t startAt=0) const {
        uint32_t hash = GetQuickHashI(str);
        Item *item = items.LendData();
        for (size_t i = startAt; i < count; i++) {
            if (item[i].hash == hash && str::Eq(item[i].string, str))
                return (int)i;
        }
        return -1;
    }

    int FindI(const WCHAR *str, size_t startAt=0) const {
        uint32_t hash = GetQuickHashI(str);
        Item *item = items.LendData();
        for (size_t i = startAt; i < count; i++) {
            if (item[i].hash == hash && str::EqI(item[i].string, str))
                return (int)i;
        }
        return -1;
    }

    bool Contains(const WCHAR *str) const {
        return -1 != Find(str);
    }
};
