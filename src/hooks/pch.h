/*
 *
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
/// @brief Standard system and project specific include files that are used
///     frequently, but changed infrequently.

#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN                 // Exclude rarely-used stuff from Windows headers
#include <windows.h>
