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
/// @brief Origin marker tool header file.

#pragma once

#include "Tool.h"
#include "Line.h"


/// Origin marker tool. Two thin lines are arranged to form axes indicators
/// on the screen at the location of the origin of the coordinate system. The
/// intersection of the lines indicates the origin and the direction of the
/// lines from the origin indicates the positive direction along the axis.
///
class MeaOriginTool : public MeaTool {

public:
    // Defaults
    static constexpr bool kShowMarker { true }; ///< Indicates whether to show the origin marker by default

    static const CString kToolName;             ///< "OriginTool"


    /// Constructs a new instance of an Origin marker tool. To use
    /// a newly constructed tool, the Enable() method must be called.
    /// Enable() will call the Create() method the first time it is called
    /// if Create() has not previously been called.
    ///
    /// @param mgr  [in] Tool manager parent of the Origin tool.
    ///
    explicit MeaOriginTool(MeaToolMgr* mgr);

    /// Hides and then destroys the Origin tool.
    //
    virtual ~MeaOriginTool();

    /// Displays the Origin tool. If the Create() method has not been
    /// previously called, it will be called by Enable.
    ///
    virtual void Enable() override;

    /// Hides the Origin tool.
    ///
    virtual void Disable() override;

    /// Requests that the tool update its position based on the current
    /// origin location.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason) override;

    /// Persists the state of the tool to the specified profile object.
    ///
    /// @param profile  [in] The destination for the state information.
    ///                 Typically an MeaFileProfile or MeaRegistryProfile
    ///                 object.
    ///
    virtual void SaveProfile(MeaProfile& profile) override;

    /// Restores the state of the tool from the specified profile object.
    ///
    /// @param profile  [in] The source for the state information. Typically
    ///                 an MeaFileProfile or MeaRegistryProfile object.
    ///
    virtual void LoadProfile(MeaProfile& profile) override;

    /// Resets the tool to its default state.
    ///
    virtual void MasterReset() override;

    /// Returns the tool's current position which, for the Origin tool,
    /// is location of the origin.
    ///
    /// @return The location of the origin, in pixels
    ///
    virtual const POINT& GetPosition() const override;

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("OriginTool").
    ///
    virtual CString GetToolName() const override;

    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The origin lines are redrawn in their new colors.
    ///
    virtual void ColorsChanged() override;

private:
    /// Creates the tool's graphical components. The Origin tool consists
    /// of two lines arranged to form a coordinate axes indicator. The
    /// Enable() method must be called to make the tool visible.
    ///
    /// @return <b>true</b> if the window is successfully created.
    ///
    bool Create();


    MeaLine m_xAxis;    ///< Line to depict the x-axis
    MeaLine m_yAxis;    ///< Line to depict the y-axis
};
