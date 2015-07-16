/* Copyright 2015 the SumatraPDF project authors (see AUTHORS file).
   License: Simplified BSD (see COPYING.BSD) */

#ifndef BaseUtil_win_h
#define BaseUtil_win_h

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

// this tells Visual Studio's STL to not use exceptions and try/catch
// (otherwise we would have to compile with /EHsc)
#define _HAS_EXCEPTIONS 0

#include <windows.h>
#include <unknwn.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <commctrl.h>
#include <windowsx.h>
#include <winsafer.h>
#include <gdiplus.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include <malloc.h>
#include <io.h>

/* In debug mode, VS 2010 instrumentations complains about GetRValue() etc.
This adds equivalent functions that don't have this problem and ugly
substitutions to make sure we don't use Get*Value() in the future */

static inline BYTE GetRValueSafe(COLORREF rgb)
{
    rgb = rgb & 0xff;
    return (BYTE)rgb;
}

static inline BYTE GetGValueSafe(COLORREF rgb)
{
    rgb = (rgb >> 8) & 0xff;
    return (BYTE)rgb;
}

static inline BYTE GetBValueSafe(COLORREF rgb)
{
    rgb = (rgb >> 16) & 0xff;
    return (BYTE)rgb;
}

#undef GetRValue
#define GetRValue UseGetRValueSafeInstead
#undef GetGValue
#define GetGValue UseGetGValueSafeInstead
#undef GetBValue
#define GetBValue UseGetBValueSafeInstead

#endif
