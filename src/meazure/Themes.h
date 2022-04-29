/*
 * Copyright 2001 C Thing Software
 *
 * This file is part of Meazure.
 *
 * Meazure is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Meazure is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Meazure.  If not, see <http://www.gnu.org/licenses/>.
 */

/// @file
/// @brief Stub module that dynamically supports the display Themes API found
///     on Windows XP and beyond. Exactly one source file must include this
///     header with the macro COMPILE_THEME_STUBS defined.

#pragma once

#undef IsThemeActive
#undef DrawThemeEdge


/// Defines a handle to a section of theme data for class.
/// HTHEME is already declared if WINVER >= 0x0500 in uxtheme.h
/// This is for components built with an older version number.
///
typedef HANDLE HTHEME;


#ifndef BP_PUSHBUTTON
#include "vssym32.h"
#endif // !BP_PUSHBUTTON


// Define COMPILE_THEME_STUBS to compile the stubs;
// otherwise, you get the declarations.
//
#ifdef COMPILE_THEME_STUBS

// Implement API stubs.
//
BOOL(WINAPI* g_pfnIsThemeActive)(void) { nullptr };
HTHEME(WINAPI* g_pfnOpenThemeData)(HWND, LPCWSTR) { nullptr };
HRESULT(WINAPI* g_pfnCloseThemeData)(HTHEME) { nullptr };
HRESULT(WINAPI* g_pfnDrawThemeBackground)(HTHEME, HDC, int, int, const RECT*, const RECT*) { nullptr };
BOOL(WINAPI* g_pfnDrawThemeEdge)(HTHEME, HDC, int, int, const RECT*, UINT, UINT, RECT*) { nullptr };
BOOL g_fThemeInitDone { FALSE };

BOOL InitThemeStubs(void) {
    HMODULE hUXTheme;

    if (g_fThemeInitDone) {
        return g_pfnIsThemeActive != nullptr;
    }

    hUXTheme = GetModuleHandle(TEXT("UXTHEME"));
    if ((hUXTheme != nullptr) &&
        (*(FARPROC*)&g_pfnIsThemeActive = GetProcAddress(hUXTheme, "IsThemeActive")) != nullptr &&
        (*(FARPROC*)&g_pfnOpenThemeData = GetProcAddress(hUXTheme, "OpenThemeData")) != nullptr &&
        (*(FARPROC*)&g_pfnCloseThemeData = GetProcAddress(hUXTheme, "CloseThemeData")) != nullptr &&
        (*(FARPROC*)&g_pfnDrawThemeBackground = GetProcAddress(hUXTheme, "DrawThemeBackground")) != nullptr &&
        (*(FARPROC*)&g_pfnDrawThemeEdge = GetProcAddress(hUXTheme, "DrawThemeEdge")) != nullptr
       ) {
        g_fThemeInitDone = TRUE;
        return TRUE;
    } else {
        g_pfnIsThemeActive = nullptr;
        g_pfnOpenThemeData = nullptr;
        g_pfnCloseThemeData = nullptr;
        g_pfnDrawThemeBackground = nullptr;
        g_pfnDrawThemeEdge = nullptr;
        g_fThemeInitDone = TRUE;
        return FALSE;
    }
}

BOOL WINAPI
xIsThemeActive() {
    if (InitThemeStubs()) {
        return g_pfnIsThemeActive();
    }
    return FALSE;
}

HTHEME WINAPI
xOpenThemeData(HWND hwnd, LPCWSTR pszClassList) {
    if (InitThemeStubs()) {
        return g_pfnOpenThemeData(hwnd, pszClassList);
    }
    return nullptr;
}

HRESULT WINAPI
xCloseThemeData(HTHEME hTheme) {
    if (InitThemeStubs()) {
        return g_pfnCloseThemeData(hTheme);
    }
    return S_OK;
}

HRESULT WINAPI
xDrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect) {
    if (InitThemeStubs()) {
        return g_pfnDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
    }
    return S_OK;
}

BOOL WINAPI
xDrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pDestRect, UINT uEdge, UINT uFlags, RECT* pContentRect) {
    if (InitThemeStubs()) {
        return g_pfnDrawThemeEdge(hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect);
    }
    return TRUE;
}

#undef COMPILE_THEME_STUBS

#else   // COMPILE_THEME_STUBS

/// Indicates whether a theme is currently active.
extern BOOL WINAPI      xIsThemeActive(void);

/// Opens a display theme.
extern HTHEME WINAPI    xOpenThemeData(HWND, LPCWSTR);

/// Closes the currently open theme.
extern HRESULT WINAPI   xCloseThemeData(HTHEME);

/// Draws a background appropriate for the theme.
extern HRESULT WINAPI   xDrawThemeBackground(HTHEME, HDC, int, int, const RECT*, const RECT*);

/// Draws an edge into the specified DC that is appropriate for the theme.
extern BOOL WINAPI      xDrawThemeEdge(HTHEME, HDC, int, int, const RECT*, UINT, UINT, RECT*);

#endif  // COMPILE_THEME_STUBS


/// Indicates whether a theme is active.
#define IsThemeActive       xIsThemeActive

/// Opens a display theme.
#define OpenThemeData       xOpenThemeData

/// Closes a theme.
#define CloseThemeData      xCloseThemeData

/// Draws a background appropriate for the display theme.
#define DrawThemeBackground xDrawThemeBackground

/// Draws a control edge appropriate for the display theme.
#define DrawThemeEdge       xDrawThemeEdge
