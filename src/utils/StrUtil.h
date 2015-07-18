/* Copyright 2015 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

// note: include BaseUtil.h instead of including directly

namespace str {

enum TrimOpt {
    TrimLeft,
    TrimRight,
    TrimBoth
};

size_t Len(const char *s);

char *  Dup(const char *s);

void ReplacePtr(char **s, const char *snew);
void ReplacePtr(const char **s, const char *snew);

char *  Join(const char *s1, const char *s2, const char *s3=nullptr);
char *  Join(const char *s1, const char *s2, const char *s3, Allocator *allocator);

bool Eq(const char *s1, const char *s2);
bool EqI(const char *s1, const char *s2);
bool EqIS(const char *s1, const char *s2);
bool EqN(const char *s1, const char *s2, size_t len);
bool EqNI(const char *s1, const char *s2, size_t len);

template <typename T>
inline bool IsEmpty(T *s) {
    return !s || (0 == *s);
}

template <typename T>
inline bool StartsWith(const T* str, const T* txt) {
    return EqN(str, txt, Len(txt));
}

bool StartsWithI(const char *str, const char *txt);
bool EndsWith(const char *txt, const char *end);
bool EndsWithI(const char *txt, const char *end);

static inline bool EqNIx(const char *s, size_t len, const char *s2) {
    return str::Len(s2) == len && str::StartsWithI(s, s2);
}

char *  DupN(const char *s, size_t lenCch);

void ToLower(char *s);

void    Utf8Encode(char *& dst, int c);

inline const char * FindChar(const char *str, const char c) {
    return strchr(str, c);
}
inline char * FindChar(char *str, const char c) {
    return strchr(str, c);
}

inline const char * FindCharLast(const char *str, const char c) {
    return strrchr(str, c);
}
inline char * FindCharLast(char *str, const char c) {
    return strrchr(str, c);
}

inline const char * Find(const char *str, const char *find) {
    return strstr(str, find);
}

const char * FindI(const char *str, const char *find);

bool    BufFmtV(char *buf, size_t bufCchSize, const char *fmt, va_list args);
char *  FmtV(const char *fmt, va_list args);
char *  Format(const char *fmt, ...);

inline bool IsWs(char c) { return ' ' == c || ('\t' <= c && c <= '\r'); }

// Note: I tried an optimization: return (unsigned)(c - '0') < 10;
// but it seems to mis-compile in release builds
inline bool IsDigit(char c) {
    return '0' <= c && c <= '9';
}

void    TrimWsEnd(char *s, char *&e);

size_t  TransChars(char *str, const char *oldChars, const char *newChars);
char *  Replace(const char *s, const char *toReplace, const char *replaceWith);

size_t  NormalizeWS(char *str);
size_t  NormalizeNewlinesInPlace(char *s, char *e);
size_t  NormalizeNewlinesInPlace(char *s);
size_t  RemoveChars(char *str, const char *toRemove);

size_t  BufSet(char *dst, size_t dstCchSize, const char *src);
size_t  BufAppend(char *dst, size_t dstCchSize, const char *s);

char *  MemToHex(const unsigned char *buf, size_t len);
bool    HexToMem(const char *s, unsigned char *buf, size_t bufLen);

const char  *   Parse(const char *str, const char *format, ...);
const char  *   Parse(const char *str, size_t len, const char *format, ...);

}  // namespace str

namespace url {

void DecodeInPlace(char *urlUtf8);

} // namespace url

namespace seqstrings {
void         SkipStr(char *& s);
void         SkipStr(const char *& s);
int          StrToIdx(const char *strings, const char *toFind);
const char * IdxToStr(const char *strings, int idx);

} // namespace seqstrings

#define _MemToHex(ptr) str::MemToHex((const unsigned char *)(ptr), sizeof(*ptr))
#define _HexToMem(txt, ptr) str::HexToMem(txt, (unsigned char *)(ptr), sizeof(*ptr))

#define UTF8_BOM    "\xEF\xBB\xBF"
#define UTF16_BOM   "\xFF\xFE"
#define UTF16BE_BOM "\xFE\xFF"

#if defined(OS_WIN)
#include "StrUtil_win.h"
#endif
