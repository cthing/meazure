/*
 * Copyright 2001, 2004, 2011 C Thing Software
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
/// @brief Stub module that dynamically supports the Layered Windows API found
///     on Windows 2000 and beyond. Exactly one source file must include this
///     header with the macro COMPILE_LAYERED_WINDOW_STUBS defined.

#pragma once


#ifndef WS_EX_LAYERED
/// Style for a layered window.
#define WS_EX_LAYERED       0x00080000

/// Indicates color keyed transparency.
#define LWA_COLORKEY        0x00000001

/// Indicates alpha blended transparency.
#define LWA_ALPHA           0x00000002
#endif // WS_EX_LAYERED


#undef SetLayeredWindowAttributes
#undef UpdateLayeredWindow


// Define COMPILE_LAYERED_WINDOW_STUBS to compile the stubs;
// otherwise, you get the declarations.
//
#ifdef COMPILE_LAYERED_WINDOW_STUBS

// Implement API stubs.
//

BOOL (WINAPI* g_pfnSetLayeredWindowAttributes)(HWND, COLORREF, BYTE, DWORD) = NULL;
BOOL (WINAPI* g_pfnUpdateLayeredWindow)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD) = NULL;
BOOL g_fLayeredWindowInitDone = FALSE;
BOOL g_enableLayeredWindows = TRUE;

BOOL InitLayeredWindowStubs(void)
{
    HMODULE hUser32;

    if (g_fLayeredWindowInitDone)
        return g_pfnSetLayeredWindowAttributes != NULL;

    hUser32 = GetModuleHandle(TEXT("USER32"));
    
    if ((hUser32 != NULL) && g_enableLayeredWindows &&
        (*(FARPROC*)&g_pfnSetLayeredWindowAttributes = GetProcAddress(hUser32, "SetLayeredWindowAttributes")) != NULL &&
        (*(FARPROC*)&g_pfnUpdateLayeredWindow        = GetProcAddress(hUser32, "UpdateLayeredWindow")) != NULL
       ) {
        g_fLayeredWindowInitDone = TRUE;
        return TRUE;
    } else {
        g_pfnSetLayeredWindowAttributes = NULL;
        g_pfnUpdateLayeredWindow = NULL;
        g_fLayeredWindowInitDone = TRUE;
        return FALSE;
    }
}


BOOL HaveLayeredWindows(void)
{
    return InitLayeredWindowStubs();
}


BOOL WINAPI
xSetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
    if (InitLayeredWindowStubs()) {
        return g_pfnSetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags);
    }
    return TRUE;
}


BOOL WINAPI
xUpdateLayeredWindow(HWND hwnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
{
    if (InitLayeredWindowStubs()) {
        return g_pfnUpdateLayeredWindow(hwnd, hdcDst, pptDst, psize, hdcSrc, pptSrc, crKey, pblend, dwFlags);
    }
    return TRUE;
}


#undef COMPILE_LAYERED_WINDOW_STUBS

#else   // COMPILE_LAYERED_WINDOW_STUBS

/// Flag to use layered windows if they are present.
extern BOOL         g_enableLayeredWindows;

/// Indicates that layered windows are available.
/// @return TRUE if layered windows are available.
extern BOOL         HaveLayeredWindows(void);

/// Sets layered windows attributes. The parameters
/// are the same as those for the Windows API function
/// SetLayeredWindowAttributes.
extern BOOL WINAPI  xSetLayeredWindowAttributes(HWND, COLORREF, BYTE, DWORD);

/// Updates a layered window from a memory bitmap. The
/// parameters are the same as those for the Windows API
/// function UpdateLayeredWindow.
extern BOOL WINAPI  xUpdateLayeredWindow(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);

#endif  // COMPILE_LAYERED_WINDOW_STUBS


/// Replace the Windows API function with our function.
#define SetLayeredWindowAttributes  xSetLayeredWindowAttributes

/// Replace the Windows API function with our function.
#define UpdateLayeredWindow         xUpdateLayeredWindow
