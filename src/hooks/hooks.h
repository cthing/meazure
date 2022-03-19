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
/// @brief Hooks to pick up mouse movement information.

#pragma once

// Export/import handling
#ifndef MEA_HOOKSAPI
#define MEA_HOOKSAPI __declspec(dllimport)
#endif

// Mouse hook message
#define MEA_MOUSE_MSG   (WM_USER + 1)

/// Enables mouse position reporting. This function installs the mouse hook
/// procedure that calls back to processes that are interested in the mouse
/// pointer position as it changes.
///
/// @return <code>true</code> if the mouse hook was installed successfully.
///
MEA_HOOKSAPI bool MeaEnableMouseHook(void);

/// Disables mouse position reporting. This function removes
/// the mouse hook procedure.
///
/// @return <b>true</b> if the mouse hook was removed successfully.
///
MEA_HOOKSAPI bool MeaDisableMouseHook(void);
