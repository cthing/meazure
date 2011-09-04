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
/// @brief Produces an ISO 8601 compliant time stamp.

#pragma once

#include <time.h>


/// Creates an ISO 8601 compliant time stamp based on the specified
/// time value. The format of the timestamp is:
///
/// yyyy-mm-ddThh:mm:ssZ
///
/// @param t    [in] Number of seconds since the Epoch as defined
///             by the system time() function.
///
/// @return String containing the timestamp in 8601 format corresponding
///         to the specified time.
///
extern CString  MeaMakeTimeStamp(time_t t);

/// Parses an ISO 8601 compliant time stamp and returns the
/// corresponding time value. The format of the timestamp must be:
///
/// yyy-mm-ddThh:mm:ssZ
///
/// @param timeStr  [in] Time stamp string to parse.
///
/// @return Number of seconds since the Epoch as defined by the
///         system time() function.
///
extern time_t   MeaParseTimeStamp(const CString& timeStr);
