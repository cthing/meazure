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
/// @brief Header file for base class of tools whose use is mutually exclusive.

#pragma once

#include "Tool.h"
#include "PositionLogMgr.h"


/// Abstract base class for tools whose use is mutually exclusive. For
/// example, a Line tool cannot be used at the same time a Point tool is
/// being used. Thus, the Line and Point tools are mutually exclusive and
/// derive from MeaRadioTool.
///
class MeaRadioTool: public MeaTool
{
public:
    /// Map of points used to set the position of a tool.
    /// Typically the position log manager will fill a point
    /// map and pass it to the tool's SetPosition method.
    ///
    typedef std::map<CString, POINT> PointMap;


    /// Constructs a new instance of an radio based measurement tool.
    /// Typically, to use a newly constructed tool, the Enable() method
    /// must be called.
    ///
    /// @param mgr  [in] Tool manager parent of the tool.
    ///
    explicit MeaRadioTool(MeaToolMgr *mgr);

    /// Destroys the tool.
    ///
    virtual ~MeaRadioTool();


    /// Indicates that the derived tool is a Radio tool and
    /// cannot be enabled until any other enabled Radio tool
    /// is disabled.
    ///
    /// @return Always <b>true</b> indicating that this class
    ///         and its derived classes are Radio tools.
    ///
    virtual bool IsRadioTool();


    /// Returns a resource ID for the string used to label this
    /// tool on the data display.
    ///
    /// @return Resource ID for the label. This base class implementation
    ///         returns the resource ID for an empty string.
    ///
    virtual UINT GetLabelId() const;

    /// Indicates whether the derived tool uses crosshairs.
    ///
    /// @return This base class implementation always returns <b>true</b>.
    ///
    virtual bool HasCrosshairs() const;


    /// Displays the tool's crosshairs. If the Enable() method has not
    /// been previously called, this method has no effect. This base
    /// class implementation does nothing.
    ///
    virtual void EnableCrosshairs();

    /// Hides the tool's crosshairs. This base class implementation
    /// does nothing.
    ///
    virtual void DisableCrosshairs();


    /// Visually flashes the tool. Typically, if a tool uses crosshairs,
    /// they are cycled between a hilite color and their normal color a
    /// number of times. Flashing the tool helps the user find it on the
    /// screen. This base class implementation does nothing.
    ///
    virtual void Flash();

    /// Visually strobes the tool. Typically, if a tool uses crosshairs,
    /// they are cycled between a hilite color and their normal color once.
    /// In addition, if the tool has data windows attached to the crosshairs,
    /// they are cycled between hidden and visible. Strobing the tool is done
    /// to visually confirm region capture and position recording. This base
    /// class implementation does nothing.
    ///
    virtual void Strobe();


    /// Indicates whether the tool can define a rectangular region that
    /// can then be read for a screen capture.
    ///
    /// @return <b>true</b> if the tool defines a non-empty rectangular
    ///         region. This base class implementation always returns
    ///         <b>false</b>.
    ///
    virtual bool HasRegion();

    /// Returns the rectangle defined by the tool. This rectangle can then
    /// be read for a screen capture.
    ///
    /// @return The region rectangle. This base class implementation returns
    ///         an empty rectangle.
    ///
    virtual RECT GetRegion();


    /// Sets the position of the tool. This method is called by the
    /// position log manager to play back a tool's position that was
    /// previously recorded using the GetPosition method. This base
    /// class implementation does nothing.
    ///
    /// @param points   [in] Map of positions for the tool's crosshairs.
    ///
    virtual void SetPosition(const PointMap& points);

    /// Records the position of the tool. This method is called by the
    /// position log manager to record a position.
    ///
    /// @param position     [in] The tool's current position is recorded into
    ///                     the position log manager's position object.
    ///
    virtual void GetPosition(MeaPositionLogMgr::Position& position) const = 0;


    /// Called by the OS when the mouse pointer is moved. This base class
    /// implementation does nothing.
    ///
    /// @param wParam   [in] OS Message ID
    /// @param lParam   [in] MOUSEHOOKSTRUCT
    ///
    virtual void OnMouseHook(WPARAM wParam, LPARAM lParam);
};

