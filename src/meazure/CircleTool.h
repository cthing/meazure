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
/// @brief Circle measurement tool header file.

#pragma once

#include "RadioTool.h"
#include "CrossHair.h"
#include "Circle.h"
#include "Line.h"
#include "DataWin.h"


/// Circle measurement tool. This tool provides a circle and its radius.
/// Two crosshairs are provided: one at the center of the circle (vertex)
/// and one where the radius intersects the perimeter (point 1). Each
/// crosshair can be arbitrarily positioned, which will result in moving
/// and resizing the circle.
///
class MeaCircleTool : public MeaRadioTool, public MeaCrossHairCallback
{
public:
    static const CString kToolName;     ///< "CircleTool"

    /// Constructs a new instance of a circle measurement tool. To use
    /// a newly constructed Circle tool, the Enable() method must be called.
    /// Enable() will call the Create() method the first time it is called
    /// if Create() has not previously been called.
    ///
    /// @param mgr  [in] Tool manager parent of the Circle tool.
    ///
    explicit MeaCircleTool(MeaToolMgr* mgr);

    /// Hides and then destroys the Circle tool.
    ///
    virtual ~MeaCircleTool();


    /// Displays the Circle tool. If the Create() method has not been
    /// previously called, it will be called by Enable.
    ///
    virtual void Enable();

    /// Hides the Circle tool.
    ///
    virtual void Disable();


    /// Requests that the tool update the display of its position data.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason);


    /// Indicates whether the tool can define a rectangular region that
    /// can then be read for a screen capture. For the Circle tool, the
    /// region is the circle's bounding square.
    ///
    /// @return <b>true</b> if the tool defines a non-empty rectangular region.
    ///
    virtual bool HasRegion();

    /// Returns the rectangle defined by the tool. This rectangle can then
    /// be read for a screen capture.
    ///
    /// @return The circle's bounding square.
    ///
    virtual RECT GetRegion();


    /// Persists the state of the tool to the specified profile object.
    ///
    /// @param profile  [in] The destination for the state information.
    ///                 Typically an MeaFileProfile or MeaRegistryProfile
    ///                 object.
    ///
    virtual void SaveProfile(MeaProfile& profile);

    /// Restores the state of the tool from the specified profile object.
    ///
    /// @param profile  [in] The source for the state information. Typically
    ///                 an MeaFileProfile or MeaRegistryProfile object.
    ///
    virtual void LoadProfile(MeaProfile& profile);


    /// Displays the tool's crosshairs. If the Enable() method has not
    /// been previously called, this method has no effect.
    ///
    virtual void EnableCrosshairs();

    /// Hides the tool's crosshairs.
    ///
    virtual void DisableCrosshairs();


    /// Visually flashes the tool. The tool crosshairs are cycled
    /// between a hilite color and their normal color a number of times.
    /// Flashing the tool helps the user find it on the screen.
    ///
    virtual void Flash();

    /// Visually strobes the tool. The tool crosshairs are cycled
    /// between a hilite color and their normal color once. In addition,
    /// the data windows attached to the crosshairs are cycled between
    /// hidden and visible. Strobing the tool is done to visually confirm
    /// region capture and position recording.
    ///
    virtual void Strobe();


    /// Sets the position of the tool. This method sets the position
    /// one location component at a time. For example, the x position
    /// of the center crosshair is specified by calling this method
    /// with the new position and specifying the MeaXVField for the
    /// <i>which</i> parameter.
    ///
    /// @param which    [in] Position component
    /// @param pixels   [in] Position to set for the component, in pixels
    ///
    virtual void    SetPosition(MeaFields which, int pixels);

    /// Sets the position of the tool. This method is called by the
    /// position log manager to play back a tool's position that was
    /// previously recorded using the GetPosition method.
    ///
    /// @param points   [in] Map of positions for the tool's crosshairs.
    ///
    virtual void    SetPosition(const PointMap& points);

    /// Returns the tool's current position. The current position is
    /// the position of the last crosshair entered or moved. The default
    /// position is the location of the center of the circle.
    ///
    /// @return Current position
    ///
    virtual const POINT&    GetPosition() const;

    /// Records the position of the tool. This method is called by the
    /// position log manager to record a position.
    ///
    /// @param position     [in] The tool's current position is recorded into
    ///                     the position log manager's position object.
    ///
    virtual void    GetPosition(MeaPositionLogMgr::Position& position) const;

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("CircleTool").
    ///
    virtual CString GetToolName() const;

    /// Returns a resource ID for the string used to label this
    /// tool on the data display.
    ///
    /// @return Resource ID for the label.
    ///
    virtual UINT    GetLabelId() const;

    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The crosshair and line colors are updated as
    /// well as the crosshair and data window opacities.
    ///
    virtual void ColorsChanged();


    /// Called when the pointer enters one of the tool's crosshairs.
    /// The crosshair entered is identified by the MeaCrossHairCallback::CHInfo
    /// structure passed to the method.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHEnter(const CHInfo *info);

    /// Called when the pointer leaves one of the tool's crosshairs.
    /// The crosshair exited is identified by the MeaCrossHairCallback::CHInfo
    /// structure passed to the method.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHLeave(const CHInfo *info);

    /// Called when the user attempts to drag one of the tool's
    /// crosshairs. The crosshair to be moved is identified by the
    /// MeaCrossHairCallback::CHInfo structure passed to the method.
    /// The info parameter indicates where the crosshair is to be
    /// moved and the tool actually moves the crosshair to the new
    /// position. This allows the tool to perform vertical or horizontal
    /// locking and move the entire tool depending on which keyboard
    /// modifiers are pressed during the drag.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHMove(const CHInfo *info);

private:
    static const UINT   kPerimeterId;       ///< ID for the perimeter crosshair
    static const UINT   kCenterId;          ///< ID for the center crosshair


    /// Creates the tool's graphical components. The components include
    /// the crosshairs, lines and data windows. The Enable() method must
    /// be called to make the tool visible.
    ///
    /// @return <b>true</b> if the window is successfully created.
    ///
    bool    Create();

    /// Sets the position of the tool's crosshairs based on the current
    /// values of #m_center, and #m_perimeter.
    ///
    void    SetPosition();

    CPoint          m_center;           ///< Location of the center of the circle
    CPoint          m_perimeter;        ///< A point on the perimeter of the circle
    CPoint          *m_curPos;          ///< Points to #m_center or #m_perimeter depending on which point the user is moving
    CPoint          m_anchorCenter;     ///< Location for vertical / horizontal lock when Shift is held while dragging the center crosshair
    CPoint          m_anchorPerimeter;  ///< Location for vertical / horizontal lock when Shift is held while dragging the perimeter crosshair
    MeaCrossHair    m_centerCH;         ///< Crosshair for the center point
    MeaCrossHair    m_perimeterCH;      ///< Crosshair for the perimeter point
    MeaCircle       m_circle;           ///< Circle graphic object
    MeaLine         m_line;             ///< Radial line
    MeaDataWin      m_dataWinCenter;    ///< Data window tooltip for the center point
    MeaDataWin      m_dataWinPerimeter; ///< Data window tooltip for the perimeter point
};
