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
/// @brief Cursor position measurement tool header file.

#pragma once

#include "RadioTool.h"
#include "DataWin.h"


/// Cursor position measurement tool. This tool measures the position of
/// the mouse pointer.
///
class MeaCursorTool : public MeaRadioTool
{
public:
    static const CString kToolName;     ///< "CursorTool"

    /// Constructs a new instance of a cursor position measurement tool.
    /// To use a newly constructed Cursor tool, the Enable() method must
    /// be called. Enable() will call the Create() method the first time
    /// it is called if Create() has not previously been called.
    ///
    /// @param mgr  [in] Tool manager parent of the Cursor tool.
    ///
    explicit MeaCursorTool(MeaToolMgr* mgr);

    /// Destroys the Cursor tool.
    ///
    virtual ~MeaCursorTool();


    /// Enables the Cursor tool's data window. If the Create() method has
    /// not been previously called, it will be called by Enable.
    ///
    virtual void Enable() override;

    /// Disables the Cursor tool's data window.
    ///
    virtual void Disable() override;

    
    /// Requests that the tool update the display of its position data.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason) override;


    /// Indicates whether the tool has crosshairs.
    ///
    /// @return Always <b>false</b> because this tool only consists
    ///         of a data window attached to the mouse pointer.
    ///
    virtual bool HasCrosshairs() const override;

    /// Visually strobes the tool. The tool's data window is cycled between
    /// hidden and visible. Strobing the tool is done to visually confirm
    /// position recording.
    ///
    virtual void Strobe() override;


    /// Returns the position of the mouse pointer.
    ///
    /// @return Position of the mouse pointer.
    ///
    virtual const POINT&    GetPosition() const override;

    /// Records the position of the mouse pointer. This method is called
    /// by the position log manager to record a position.
    ///
    /// @param position     [in] The position of the mouse pointer is recorded
    ///                     into the position log manager's position object.
    ///
    virtual void    GetPosition(MeaPositionLogMgr::Position& position) const override;


    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("CursorTool").
    ///
    virtual CString GetToolName() const override;

    /// Returns a resource ID for the string used to label this
    /// tool on the data display.
    ///
    /// @return Resource ID for the label.
    ///
    virtual UINT    GetLabelId() const override;


    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The data window opacity is updated.
    ///
    virtual void ColorsChanged() override;


    /// Called by the OS when the mouse pointer is moved.
    ///
    /// @param wParam   [in] OS Message ID
    /// @param lParam   [in] MOUSEHOOKSTRUCT
    ///
    virtual void OnMouseHook(WPARAM wParam, LPARAM lParam) override;

private:
    /// Creates the tool's data window. The Enable() method must
    /// be called to make the data window visible.
    ///
    void    Create();

    CPoint      m_cursorPos;    ///< Cached position of the mouse pointer
    MeaDataWin  m_dataWin;      ///< Data window tooltip attached to the mouse pointer
    int         m_xDist;        ///< Half the width of the cursor
    int         m_yDist;        ///< Half the height of the cursor
};
