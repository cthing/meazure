/*
 * Copyright 2022 C Thing Software
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
/// @brief Represents a single monitor attached to the system.

#pragma once

#include "PositionLogWriter.h"
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ScreenProvider.h>
#include <meazure/xml/XMLParser.h>
#include <meazure/utilities/Geometry.h>


/// Represents a single monitor attached to the system. There is an instance of this class per monitor.
///
class MeaPositionScreen {

public:
    /// Constructs a screen object.
    ///
    MeaPositionScreen() : m_primary(false), m_manualRes(false) {}

    /// Constructs deep copy screen object from the specified
    /// iterator over the list of screens.
    ///
    /// @param screenIter     [in] Iterator from which to construct a copy of the screen.
    /// @param unitsProvider  [in] Units information and conversion provider
    /// @param screenProvider [in] Screen information provider
    ///
    explicit MeaPositionScreen(const MeaScreenProvider::ScreenIter& screenIter, const MeaUnitsProvider& unitsProvider,
                               const MeaScreenProvider& screenProvider);

    /// Constructs a deep copy of the specified screen.
    ///
    /// @param screen       [in] Screen object to be copied.
    ///
    MeaPositionScreen(const MeaPositionScreen& screen) { Copy(screen); }

    /// Loads screen elements of the log file.
    ///
    /// @param screenNode   [in] screen node in the DOM.
    ///
    void Load(const MeaXMLNode* screenNode);

    /// Saves the screen information
    ///
    /// @param writer   [in] Provides ability to write a position to the log.
    /// @param indent   [in] Output indentation level.
    /// @throw CFileException if there was a problem saving the file
    ///
    void Save(MeaPositionLogWriter& writer, int indent) const;

    /// Assignment operator for a screen object. Makes
    /// a deep copy of the object.
    ///
    /// @param screen       [in] Screen option to assign to this.
    ///
    /// @return This screen object.
    ///
    MeaPositionScreen& operator=(const MeaPositionScreen& screen) {
        return Copy(screen);
    }

    /// Tests for equality between the specified screen object and this.
    ///
    /// @param screen       [in] Screen object to test against this.
    ///
    /// @return <b>true</b> if the specified screen object and this are equal.
    ///
    bool operator==(const MeaPositionScreen& screen) const {
        return ((m_primary == screen.m_primary) &&
                MeaNumericUtils::IsEqualF(m_rect.top, screen.m_rect.top) &&
                MeaNumericUtils::IsEqualF(m_rect.bottom, screen.m_rect.bottom) &&
                MeaNumericUtils::IsEqualF(m_rect.left, screen.m_rect.left) &&
                MeaNumericUtils::IsEqualF(m_rect.right, screen.m_rect.right) &&
                MeaNumericUtils::IsEqualF(m_res.cx, screen.m_res.cx) &&
                MeaNumericUtils::IsEqualF(m_res.cy, screen.m_res.cy) &&
                (m_manualRes == screen.m_manualRes) &&
                (m_desc == screen.m_desc));
    }

    /// Tests for inequality between the specified screen object and this.
    ///
    /// @param screen       [in] Screen object to test against this.
    ///
    /// @return <b>true</b> if the specified screen object and this are not equal.
    ///
    bool operator!=(const MeaPositionScreen& screen) const { return !(*this == screen); }

    /// Indicates whether this screen is the primary display.
    /// 
    /// @return <b>true</b> if this screen is the primary display.
    /// 
    bool IsPrimary() const { return m_primary; }

    /// Provides the screen rectangle expressed in the units that were in effect at the time the
    /// screen object was created.
    /// 
    /// @return Screen rectangle
    /// 
    const MeaFRect& GetRect() const { return m_rect; }

    /// Provides the screen resolution expressed in the units that were in effect at the time the
    /// screen object was created.
    /// 
    /// @return Screen resolution.
    ///  
    const MeaFSize& GetRes() const { return m_res; }

    /// Indicates whether the resolution calibrated manually.
    /// 
    /// @return <b>true</b> if the screen resolution has been manually calibrated.
    /// 
    bool IsManualRes() const { return m_manualRes; }

    /// Description of the screen.
    /// 
    /// @return Screen description.
    const CString& GetDesc() const { return m_desc; }

private:
    /// Makes a deep copy of the specified screen object.
    ///
    /// @param screen       [in] Screen object to copy.
    ///
    /// @return Copy of the specified screen object.
    ///
    MeaPositionScreen& Copy(const MeaPositionScreen& screen) {
        if (&screen != this) {
            m_primary = screen.m_primary;
            m_rect = screen.m_rect;
            m_res = screen.m_res;
            m_manualRes = screen.m_manualRes;
            m_desc = screen.m_desc;
        }
        return *this;
    }


    bool m_primary;     ///< Is this the primary screen
    MeaFRect m_rect;       ///< Screen rectangle expressed in the units that were
                        ///< in effect at the time the screen object was created.
    MeaFSize m_res;     ///< Screen resolution expressed in the units that were
                        ///< in effect at the time the screen object was created.
    bool m_manualRes;   ///< Is the resolution calibrated manually.
    CString m_desc;     ///< Description of the screen
};
