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
/// @brief Header file for the tool base class.

#pragma once

#include <list>
#include <meazure/ui/DataDisplay.h>
#include <meazure/profile/Profile.h>
#include <meazure/ui/ScreenProvider.h>


/// Reason hint for the tool Update method.
///
enum class MeaUpdateReason {
    NormalUpdate,       ///< Normal request for displaying the tool's data
    UnitsChanged,       ///< Measurement units have changed
    OriginChanged,      ///< Coordinate system origin has moved
    DataWinArming,      ///< The data window is about to be displayed
    AllChanged          ///< The world has changed
};


class MeaToolMgr;


/// Abstract base class for all measurement tools. Both radio and non-radio
/// measurement tools inherit from this base class.
///
class MeaTool {

public:
    /// Base class constructor. Typically, to use a newly constructed tool,
    /// the Enable() method must be called.
    ///
    /// @param mgr            [in] Tool manager parent of the tool.
    /// @param screenProvider [in] Screen information provider
    /// @param unitsProvider  [in] Units information provider
    ///
    explicit MeaTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider);

    /// Destroys the tool.
    ///
    virtual ~MeaTool() = default;

    /// Typically used by derived classes to show the tool. In addition,
    /// the first time this method is called, it typically creates the
    /// components that comprise the tool.
    ///
    virtual void Enable();

    /// Typically used by derived classes to hide the tool.
    ///
    virtual void Disable();

    /// Used by derived classes to update the display of their data.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason);

    /// Indicates whether the derived tool is a radio tool and
    /// cannot be enabled until any other enabled Radio tool
    /// is disabled.
    ///
    /// @return Always <b>false</b> indicating that this class
    ///         and its derived classes are not radio tools unless
    ///         they have overridden this method.
    ///
    virtual bool IsRadioTool();

    /// Sets the position of the tool. This method sets the position
    /// one location component at a time.
    ///
    /// @param which    [in] Position component
    /// @param pixels   [in] Position to set for the component, in pixels
    ///
    virtual void SetPosition(MeaDataFieldId which, int pixels);

    /// Returns the tool's current position. The current position is
    /// the position of the last crosshair entered or moved. The default
    /// position is the location of point 1.
    ///
    /// @return Current position
    ///
    virtual const POINT& GetPosition() const = 0;

    /// Increments the specified position field. 
    /// 
    /// @param which Position field to increment
    ///
    virtual void IncPosition(MeaDataFieldId which);

    /// Decrements the specified position field. 
    /// 
    /// @param which Position field to decrement
    ///
    virtual void DecPosition(MeaDataFieldId which);

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool. Will never be nullptr and the memory for the string is owned by the tool.
    ///
    virtual PCTSTR GetToolName() const = 0;

    /// Persists the state of the tool to the specified profile object.
    /// This base class implementation does nothing.
    ///
    /// @param profile  [in] The destination for the state information.
    ///                 Typically an MeaFileProfile or MeaRegistryProfile
    ///                 object.
    ///
    virtual void SaveProfile(MeaProfile& profile);

    /// Restores the state of the tool from the specified profile object.
    /// This base class implementation does nothing.
    ///
    /// @param profile  [in] The source for the state information. Typically
    ///                 an MeaFileProfile or MeaRegistryProfile object.
    ///
    virtual void LoadProfile(MeaProfile& profile);

    /// Resets the tool to its default state.
    ///
    virtual void MasterReset();

    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. Typically a derived class redraws its components
    /// with the new colors and opacities.
    ///
    virtual void ColorsChanged();

    /// Indicates whether the tool is enabled. A tool is enabled when the
    /// Enable() method is called and is disabled when the Disable() method
    /// is called.
    ///
    /// @return <b>true</b> if the tool is enabled.
    ///
    bool IsEnabled() const { return m_enabled; }

protected:
    static const CPoint m_defaultPos;           ///< A default position used by tools that do not have a defined
                                                ///< position. This position is returned by the RecordPosition() method
    MeaToolMgr& m_mgr;                          ///< Tool manager
    const MeaScreenProvider& m_screenProvider;  ///< Screen information provider
    const MeaUnitsProvider& m_unitsProvider;    ///< Units information provider
    bool m_enabled;                             ///< Indicates whether the tool is enabled
};
