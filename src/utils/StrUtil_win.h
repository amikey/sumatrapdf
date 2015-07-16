/* Copyright 2015 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

// note: include BaseUtil.h instead of including directly

namespace str {

size_t Len(const WCHAR *s);
WCHAR * Dup(const WCHAR *s);
void ReplacePtr(WCHAR **s, const WCHAR *snew);

WCHAR * Join(const WCHAR *s1, const WCHAR *s2, const WCHAR *s3=nullptr);
bool Eq(const WCHAR *s1, const WCHAR *s2);
bool EqI(const WCHAR *s1, const WCHAR *s2);
bool EqIS(const WCHAR *s1, const WCHAR *s2);
bool EqN(const WCHAR *s1, const WCHAR *s2, size_t len);
bool EqNI(const WCHAR *s1, const WCHAR *s2, size_t len);
bool StartsWithI(const WCHAR *str, const WCHAR *txt);
bool EndsWith(const WCHAR *txt, const WCHAR *end);
bool EndsWithI(const WCHAR *txt, const WCHAR *end);
WCHAR * DupN(const WCHAR *s, size_t lenCch);
void ToLower(WCHAR *s);
WCHAR * ToWideChar(const char *src, UINT CodePage, int cbSrcLen=-1);
inline const WCHAR * FindChar(const WCHAR *str, const WCHAR c) {
    return wcschr(str, c);
}
inline WCHAR * FindChar(WCHAR *str, const WCHAR c) {
    return wcschr(str, c);
}
inline const WCHAR * FindCharLast(const WCHAR *str, const WCHAR c) {
    return wcsrchr(str, c);
}
inline WCHAR * FindCharLast(WCHAR *str, const WCHAR c) {
    return wcsrchr(str, c);
}
inline const WCHAR * Find(const WCHAR *str, const WCHAR *find) {
    return wcsstr(str, find);
}
const WCHAR * FindI(const WCHAR *str, const WCHAR *find);
bool    BufFmtV(WCHAR *buf, size_t bufCchSize, const WCHAR *fmt, va_list args);
WCHAR * FmtV(const WCHAR *fmt, va_list args);
WCHAR * Format(const WCHAR *fmt, ...);
inline bool IsWs(WCHAR c) { return iswspace(c); }

inline bool IsDigit(WCHAR c) {
    return '0' <= c && c <= '9';
}

inline bool IsNonCharacter(WCHAR c) {
    return c >= 0xFFFE || (c & ~1) == 0xDFFE || (0xFDD0 <= c && c <= 0xFDEF);
}

size_t  TrimWS(WCHAR *s, TrimOpt opt=TrimBoth);

size_t  TransChars(WCHAR *str, const WCHAR *oldChars, const WCHAR *newChars);
WCHAR * Replace(const WCHAR *s, const WCHAR *toReplace, const WCHAR *replaceWith);

size_t  NormalizeWS(WCHAR *str);
size_t  RemoveChars(WCHAR *str, const WCHAR *toRemove);
size_t  BufSet(WCHAR *dst, size_t dstCchSize, const WCHAR *src);
size_t  BufAppend(WCHAR *dst, size_t dstCchSize, const WCHAR *s);

WCHAR * FormatFloatWithThousandSep(double number, LCID locale=LOCALE_USER_DEFAULT);
WCHAR * FormatNumWithThousandSep(size_t num, LCID locale=LOCALE_USER_DEFAULT);
WCHAR * FormatRomanNumeral(int number);

int     CmpNatural(const WCHAR *a, const WCHAR *b);

const WCHAR *   Parse(const WCHAR *str, const WCHAR *format, ...);
size_t Utf8ToWcharBuf(const char *s, size_t sLen, WCHAR *bufOut, size_t cchBufOutSize);
size_t WcharToUtf8Buf(const WCHAR *s, char *bufOut, size_t cbBufOutSize);
char *  ToMultiByte(const WCHAR *txt, UINT CodePage, int cchTxtLen=-1);
char *  ToMultiByte(const char *src, UINT CodePageSrc, UINT CodePageDest);

namespace conv {

inline WCHAR *  FromCodePage(const char *src, UINT cp) { return ToWideChar(src, cp); }
inline char *   ToCodePage(const WCHAR *src, UINT cp) { return ToMultiByte(src, cp); }

inline WCHAR *  FromUtf8(const char *src, size_t cbSrcLen) { return ToWideChar(src, CP_UTF8, (int)cbSrcLen); }
inline WCHAR *  FromUtf8(const char *src) { return ToWideChar(src, CP_UTF8); }
inline char *   ToUtf8(const WCHAR *src, size_t cchSrcLen) { return ToMultiByte(src, CP_UTF8, (int)cchSrcLen); }
inline char *   ToUtf8(const WCHAR *src) { return ToMultiByte(src, CP_UTF8); }
inline WCHAR *  FromAnsi(const char *src, size_t cbSrcLen=(size_t)-1) { return ToWideChar(src, CP_ACP, (int)cbSrcLen); }
inline char *   ToAnsi(const WCHAR *src) { return ToMultiByte(src, CP_ACP); }
size_t ToCodePageBuf(char *buf, int cbBufSize, const WCHAR *s, UINT cp);
size_t FromCodePageBuf(WCHAR *buf, int cchBufSize, const char *s, UINT cp);

} // namespace str::conv

} // namespace str

namespace url {

bool IsAbsolute(const WCHAR *url);
void DecodeInPlace(WCHAR *url);
WCHAR *GetFullPath(const WCHAR *url);
WCHAR *GetFileName(const WCHAR *url);

} // namespace url

namespace seqstrings {
int          StrToIdx(const char *strings, const WCHAR *toFind);

} // namespace seqstrings
