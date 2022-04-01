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
/// @brief Line measurement tool header file.

#pragma once

#include "RadioTool.h"
#include "CrossHair.h"
#include "Line.h"
#include "DataWin.h"


/// Line measurement tool. This tool provides a line to measure the
/// distance between two arbitrary points on the screen. Two crosshairs
/// are provided one at the either end of the line (points 1 and 2).
/// Each crosshair can be arbitrarily positioned, which results in
/// moving the line connecting the crosshairs.
///
class MeaLineTool : public MeaRadioTool, public MeaCrossHairCallback
{
public:
    static const CString kToolName;     ///< "LineTool"

    /// Constructs a new instance of a line measurement tool. To use
    /// a newly constructed Line tool, the Enable() method must be called.
    /// Enable() will call the Create() method the first time it is called
    /// if Create() has not previously been called.
    ///
    /// @param mgr  [in] Tool manager parent of the Line tool.
    ///
    explicit MeaLineTool(MeaToolMgr* mgr);

    /// Hides and then destroys the Line tool.
    //
    virtual ~MeaLineTool();


    /// Displays the Line tool. If the Create() method has not been
    /// previously called, it will be called by Enable.
    ///
    virtual void Enable();

    /// Hides the Line tool.
    ///
    virtual void Disable();


    /// Requests that the tool update the display of its position data.
    ///
    /// @param reason   [in] Reason why the update has been requested
    ///
    virtual void Update(MeaUpdateReason reason);


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
    /// between a highlight color and their normal color a number of times.
    /// Flashing the tool helps the user find it on the screen.
    ///
    virtual void Flash();

    /// Visually strobes the tool. The tool crosshairs are cycled
    /// between a highlight color and their normal color once. In addition,
    /// the data windows attached to the crosshairs are cycled between
    /// hidden and visible. Strobing the tool is done to visually confirm
    /// region capture and position recording.
    ///
    virtual void Strobe();


    /// Sets the position of the tool. This method sets the position
    /// one location component at a time. For example, the x position
    /// of the point 1 crosshair is specified by calling this method
    /// with the new position and specifying the MeaX1Field for the
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
    /// position is the location of point 1.
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

    /// Increments the specified position field.
    /// 
    /// @param which Field to increment
    ///
    virtual void    IncPosition(MeaFields which);

    /// Decrements the specified position field.
    /// 
    /// @param which Field to decrement
    ///
    virtual void    DecPosition(MeaFields which);


    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("LineTool").
    ///
    virtual CString GetToolName() const;

    /// Returns a resource ID for the string used to label this
    /// tool on the data display.
    ///
    /// @return Resource ID for the label.
    ///
    virtual UINT    GetLabelId() const;


    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The crosshairs and line colors are updated as
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
    static const UINT   kPoint1Id;      ///< ID for the point 1 crosshair
    static const UINT   kPoint2Id;      ///< ID for the point 2 crosshair


    /// Creates the tool's graphical components. The components include
    /// the crosshairs, line and data windows. The Enable() method must
    /// be called to make the tool visible.
    ///
    /// @return <b>true</b> if the window is successfully created.
    ///
    bool    Create();

    /// Sets the position of the tool's crosshairs based on the current
    /// values of #m_point1, and #m_point2.
    ///
    void    SetPosition();

    CPoint          m_point1;       ///< Location of one end point of the line
    CPoint          m_point2;       ///< Location of one end point of the line
    CPoint          *m_curPos;      ///< Points to #m_point1 or #m_point2 depending on which point the user is moving
    MeaCrossHair    m_point1CH;     ///< Crosshair for point 1
    MeaCrossHair    m_point2CH;     ///< Crosshair for point 2
    MeaLine         m_line;         ///< Line connecting point 1 and point 2
    MeaDataWin      m_dataWin1;     ///< Data window tooltip for point 1
    MeaDataWin      m_dataWin2;     ///< Data window tooltip for point 2
};
