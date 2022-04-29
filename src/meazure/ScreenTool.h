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
 /// @brief Screen information tool header file.

#pragma once

#include "Tool.h"


/// Screen information tool. This tool provides size and resolution
/// information for each display screen attached to the system.
///
class MeaScreenTool : public MeaTool {

public:
    static const CString kToolName;     ///< "ScreenTool"

    /// Constructs a new instance of the Screen information tool. To use
    /// a newly constructed Screen info tool, the Enable() method must be called.
    ///
    /// @param mgr  [in] Tool manager parent of the Screen info tool.
    ///
    explicit MeaScreenTool(MeaToolMgr* mgr);

    /// Disables and then destroys the Screen information tool.
    ///
    virtual ~MeaScreenTool();

    /// This class uses this method to call the SetPosition method
    /// to set the information about the current display screen.
    ///
    /// @param profile  [in] Not used.
    ///
    virtual void LoadProfile(MeaProfile& profile) override;

    /// Enables the display of the tool. This method must be called
    /// to properly set up the tool for use.
    ///
    virtual void Enable() override;

    /// Disables the tool. This method must be called to properly
    /// tear down the tool.
    ///
    virtual void Disable() override;

    /// Updates the display of the tool's information.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason) override;

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("GridTool").
    ///
    virtual CString GetToolName() const override;

    /// Provides the tool a position so that it can determine which
    /// display screen's information to provide.
    ///
    /// @param pos      [in] Position on a display screen, in pixels
    ///
    void SetPosition(const POINT& pos);

    /// Returns the tool's current position which, for the Screen tool,
    /// is always the position (0, 0).
    ///
    /// @return The position (0, 0)
    ///
    virtual const POINT& GetPosition() const override;

private:
    CRect m_rect;       ///< Rectangle representing the current display screen
    FSIZE m_res;        ///< Resolution of the current display screen
    bool m_anyOSRes;    ///< Indicates if any of the screen resolutions are not calibrated (i.e. the OS reported resolution is used)
    CString m_name;     ///< Name of the display screen
};
