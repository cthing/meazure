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
/// @brief Window measurement tool header file.

#pragma once

#include "RadioTool.h"
#include "Rectangle.h"
#include "DataWin.h"


/// Window measurement tool. This tool tracks the mouse pointer and attempts
/// to identify the deepest window in the window hierarchy under the pointer.
/// the dimensions of that window are displayed. The tool consists of a
/// rectangle that is used to highlight the window selected for measurement. In
/// addition, a data window is attached to the rectangle to provide a measurement
/// information popup.
///
class MeaWindowTool : public MeaRadioTool {

public:
    static const CString kToolName;         ///< "WindowTool"

    /// Constructs a new instance of a Window measurement tool. To use
    /// a newly constructed Window tool, the Enable() method must be called.
    /// Enable() will call the Create() method the first time it is called
    /// if Create() has not previously been called.
    ///
    /// @param mgr              [in] Tool manager parent of the Window tool.
    /// @param screenProvider   [in] Screen information provider
    ///
    explicit MeaWindowTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider);

    /// Hides and then destroys the Window tool.
    //
    virtual ~MeaWindowTool();

    /// Displays the Window tool. If the Create() method has not been
    /// previously called, it will be called by Enable.
    ///
    virtual void Enable() override;

    /// Hides the Window tool.
    ///
    virtual void Disable() override;

    /// Requests that the tool update the display of the window data.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason) override;

    /// Indicates whether the tool uses crosshairs. The Window tool
    /// does not use crosshairs.
    ///
    /// @return For this class this method always returns <b>false</b>.
    ///
    virtual bool HasCrosshairs() const override;

    /// Visually strobes the tool. The tool data window attached to the
    /// window rectangle is cycled between hidden and visible. Strobing
    /// the tool is done to visually confirm region capture and position
    /// recording.
    ///
    virtual void Strobe() override;

    /// Indicates whether the tool can define a rectangular region that
    /// can then be read for a screen capture. The Window tool defines
    /// such a rectangular area.
    ///
    /// @return <b>true</b> if the tool defines a non-empty rectangular region.
    ///
    virtual bool HasRegion() override;

    /// Returns the rectangle defined by the tool. This rectangle can then
    /// be read for a screen capture.
    ///
    /// @return The rectangle formed by the tool.
    ///
    virtual RECT GetRegion() override;

    /// Called by the OS when the mouse pointer is moved. Via this method
    /// the window under the pointer is identified.
    ///
    /// @param wParam   [in] OS Message ID
    /// @param lParam   [in] MOUSEHOOKSTRUCT
    ///
    virtual void OnMouseHook(WPARAM wParam, LPARAM lParam) override;

    /// Returns the position of the mouse pointer.
    ///
    /// @return Position of the mouse pointer.
    ///
    virtual const POINT& GetPosition() const override;

    /// Records the position of the window rectangle. This method is called
    /// by the position log manager to record a position.
    ///
    /// @param position     [in] The position of the window rectangle is recorded
    ///                     into the position log manager's position object.
    ///
    virtual void GetPosition(MeaPositionLogMgr::Position& position) const override;

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("WindowTool").
    ///
    virtual CString GetToolName() const override;

    /// Returns a resource ID for the string used to label this
    /// tool on the data display.
    ///
    /// @return Resource ID for the label.
    ///
    virtual UINT GetLabelId() const override;

    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The window rectangle color and the data window
    /// opacity are updated.
    ///
    virtual void ColorsChanged() override;

private:
    /// Called by the win32 EnumChildWindows function when it enumerates
    /// child windows. Also tests to see if the child window is under the
    /// cursor.
    ///
    /// @param hwnd     [in] Child window to test
    /// @param lParam   [in] <b>this</b> pointer
    ///
    /// @return TRUE if enumeration should continue.
    ///
    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);

    /// Creates the tool's graphical components. The components include
    /// the window rectangle and the data window. The Enable() method must
    /// be called to make the tool visible.
    ///
    void Create();

    /// Attempts to locate a window under the mouse pointer. The window
    /// hierarchy is searched to identify the deepest window in the hierarchy.
    ///
    /// @return <b>true</b> if the pointer has moved over a new window.
    ///
    bool FindWindow();


    CPoint m_pointerPos;        ///< Cached position of the mouse pointer, in pixels
    CPoint m_point1;            ///< Upper left position of the window rectangle
    CPoint m_point2;            ///< Lower right position of the window rectangle
    MeaRectangle m_rectangle;   ///< Window highlight rectangle
    HWND m_currentWnd;          ///< Window currently highlighted
    HWND m_hiliteWnd;           ///< Window to highlight
    MeaDataWin m_dataWin;       ///< Data window tooltip
};
