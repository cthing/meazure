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

#include "stdafx.h"
#define MEA_HOOKSAPI  __declspec(dllexport)
#include "hooks.h"
#include <crtdbg.h>


// Shared global variables

#pragma data_seg("MeaHooksShared")
DWORD   g_meaHookThreadId = 0;
HHOOK   g_meaMouseHook = NULL;
#pragma data_seg()

#pragma comment(linker, "/section:MeaHooksShared,rws")


// Globals not exported

HINSTANCE g_hInstDll = NULL;


/// Entry point for the DLL.
///
/// @param hModule  [in] Instance handle for the DLL.
/// @param reason   [in] Reason for calling the entry point.
/// @return Always returns TRUE.
///
BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD  reason, LPVOID /* lpReserved */)
{
    switch(reason) {
    case DLL_PROCESS_ATTACH:
        g_hInstDll = hModule;
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }

    return TRUE;
}

/// Callback for the mouse hook.
///
/// @param nCode    [in] Hook code.
/// @param wParam   [in] Message identifier.
/// @param lParam   [in] Mouse information structure.
/// @return Resolution of chain execution.
///
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        MOUSEHOOKSTRUCT *mhs = (MOUSEHOOKSTRUCT*)lParam;
        LPARAM coords = MAKELPARAM(mhs->pt.x, mhs->pt.y);
        PostThreadMessage(g_meaHookThreadId, MEA_MOUSE_MSG, 0, coords);
    }
    return CallNextHookEx(g_meaMouseHook, nCode, wParam, lParam);
}

bool MeaEnableMouseHook()
{
    _ASSERT(g_meaMouseHook == NULL);

    g_meaHookThreadId = GetCurrentThreadId();
    g_meaMouseHook = SetWindowsHookEx(WH_MOUSE, MouseHookProc, g_hInstDll, 0);

    return (g_meaMouseHook != NULL);
}

bool MeaDisableMouseHook()
{
    _ASSERT(g_meaMouseHook != NULL);

    // Turn off the hook
    bool ret = UnhookWindowsHookEx(g_meaMouseHook) ? true : false;
    g_meaMouseHook = NULL;

    // Flush any pending hook messages from the queue
    MSG msg;
    while (PeekMessage(&msg, NULL,  MEA_MOUSE_MSG, MEA_MOUSE_MSG, PM_REMOVE)) {
    }
    
    return ret;
}
