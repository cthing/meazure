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
/// @brief Application-specific event messages.

#pragma once


/// Application-specific event messages.
///
enum MeaMessages {
    MeaDataChangeMsg        = (WM_USER + 0x100),    ///< Indicates that the displayed data has changed.
    MeaFieldEnterMsg        = (WM_USER + 0x101),    ///< A text field has gained the keyboard focus.
    MeaFieldArrowMsg        = (WM_USER + 0x102),    ///< An arrow key has been pressed while a field has the focus.
    MeaFieldFocusMsg        = (WM_USER + 0x103),    ///< A text field has lost keyboard focus.
    MeaShowCalPrefsMsg      = (WM_USER + 0x104),    ///< Request to display the calibration preferences.
    MeaPrefsApplyMsg        = (WM_USER + 0x105),    ///< Notificaiton that the preferences should be applied.
    MeaGetPositionMsg       = (WM_USER + 0x106),    ///< Request for the current radio tool's position.
    MeaCaliperPositionMsg   = (WM_USER + 0x107),    ///< Calibration calipers have been moved.
    MeaHPTimerMsg           = (WM_USER + 0x108),    ///< High priority timer has expired.
    MeaMasterResetMsg       = (WM_USER + 0x109)     ///< Master reset has been requested.
};
