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
/// @brief Angle measurement tool header file.

#pragma once

#include "RadioTool.h"
#include "CrossHair.h"
#include "Line.h"
#include "DataWin.h"


/// Angle measurement tool. This tool measures the acute or obtuse
/// angle between two intersecting lines. There are three crosshairs
/// associated with the tool: one at the intersection of the two
/// lines (vertex), and one at the end of each line (point 1 and point 2).
/// Each crosshair can be arbitrarily positioned. An angle bisector
/// is displayed to indicate on which side of the intersection the
/// angle is being measured.
///
class MeaAngleTool : public MeaRadioTool, public MeaCrossHairCallback {

public:
    static const CString kToolName;     ///< "AngleTool"

    /// Constructs a new instance of an angle measurement tool. To use
    /// a newly constructed Angle tool, the Enable() method must be called.
    /// Enable() will call the Create() method the first time it is called
    /// if Create() has not previously been called.
    ///
    /// @param mgr              [in] Tool manager parent of the Angle tool.
    /// @param screenProvider   [in] Screen information provider
    ///
    explicit MeaAngleTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider);

    /// Hides and then destroys the Angle tool.
    ///
    virtual ~MeaAngleTool();

    /// Displays the Angle tool. If the Create() method has not been
    /// previously called, it will be called by Enable.
    ///
    virtual void Enable() override;

    /// Hides the Angle tool.
    ///
    virtual void Disable() override;

    /// Requests that the tool update the display of its position data.
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

    /// Displays the tool's crosshairs. If the Enable() method has not
    /// been previously called, this method has no effect.
    ///
    virtual void EnableCrosshairs() override;

    /// Hides the tool's crosshairs.
    ///
    virtual void DisableCrosshairs() override;

    /// Visually flashes the tool. The tool crosshairs are cycled
    /// between a highlight color and their normal color a number of times.
    /// Flashing the tool helps the user find it on the screen.
    ///
    virtual void Flash() override;

    /// Visually strobes the tool. The tool crosshairs are cycled
    /// between a highlight color and their normal color once. In addition,
    /// the data windows attached to the crosshairs are cycled between
    /// hidden and visible. Strobing the tool is done to visually confirm
    /// position recording.
    ///
    virtual void Strobe() override;

    /// Sets the position of the tool. This method sets the position
    /// one location component at a time. For example, the x position
    /// of the point 1 crosshair is specified by calling this method
    /// with the new position and specifying the MeaX1Field for the
    /// <i>which</i> parameter.
    ///
    /// @param which    [in] Position component
    /// @param pixels   [in] Position to set for the component, in pixels
    ///
    virtual void SetPosition(MeaFields which, int pixels) override;

    /// Sets the position of the tool. This method is called by the
    /// position log manager to play back a tool's position that was
    /// previously recorded using the GetPosition method.
    ///
    /// @param points   [in] Map of positions for the tool's crosshairs.
    ///
    virtual void SetPosition(const PointMap& points) override;

    /// Returns the tool's current position. The current position is
    /// the position of the last crosshair entered or moved. The default
    /// position is the location of the point 1.
    ///
    /// @return Current position
    ///
    virtual const POINT& GetPosition() const override;

    /// Records the position of the tool. This method is called by the
    /// position log manager to record a position.
    ///
    /// @param position     [in] The tool's current position is recorded into
    ///                     the position log manager's position object.
    ///
    virtual void GetPosition(MeaPositionLogMgr::Position& position) const override;

    /// Increments the specified position field.
    /// 
    /// @param which Field to increment
    ///
    virtual void IncPosition(MeaFields which) override;

    /// Decrements the specified position field.
    /// 
    /// @param which Field to decrement
    ///
    virtual void DecPosition(MeaFields which) override;

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Name of the tool ("AngleTool").
    ///
    virtual CString GetToolName() const override;

    /// Returns a resource ID for the string used to label this
    /// tool on the data display.
    ///
    /// @return Resource ID for the label.
    ///
    virtual UINT GetLabelId() const override;

    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The crosshair and line colors are updated as
    /// well as the crosshair and data window opacities.
    ///
    virtual void ColorsChanged() override;

    /// Called when the pointer enters one of the tool's crosshairs.
    /// The crosshair entered is identified by the MeaCrossHairCallback::CHInfo
    /// structure passed to the method.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHEnter(const CHInfo* info) override;

    /// Called when the pointer leaves one of the tool's crosshairs.
    /// The crosshair exited is identified by the MeaCrossHairCallback::CHInfo
    /// structure passed to the method.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHLeave(const CHInfo* info) override;

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
    virtual void OnCHMove(const CHInfo* info) override;

private:
    static constexpr double kLengthB { 50.0 };      ///< Length of the angle bisector line in pixels
    static constexpr UINT kPoint1Id { 1 };          ///< ID for the point 1 crosshair
    static constexpr UINT kPoint2Id { 2 };          ///< ID for the point 2 crosshair
    static constexpr UINT kVertexId { 3 };          ///< ID for the vertex crosshair


    /// Creates the tool's graphical components. The components include
    /// the crosshairs, lines and data windows. The Enable() method must
    /// be called to make the tool visible.
    ///
    /// @return <b>true</b> if the window is successfully created.
    ///
    bool Create();

    /// Sets the position of the tool's crosshairs based on the current
    /// values of #m_point1, #m_point2, and #m_vertex.
    ///
    void SetPosition();

    /// Updates the position of the angle bisector to reflect the positions
    /// of the angle lines.
    ///
    void UpdateBisector();


    CPoint m_point1;            ///< Location of the line 1 end point
    CPoint m_point2;            ///< Location of the line 2 end point
    CPoint m_vertex;            ///< Location of the intersection of line 1 and line 2
    CPoint m_vertexAnchor;      ///< Location for vertical / horizontal lock when Shift is held while dragging
    CPoint* m_curPos;           ///< Points to #m_point1, #m_point2, or #m_vertex depending on point the user is moving
    MeaCrossHair m_point1CH;    ///< Crosshair for the line 1 end point
    MeaCrossHair m_point2CH;    ///< Crosshair for the line 2 end point
    MeaCrossHair m_vertexCH;    ///< Crosshair for the line 1 / line 2 intersection point
    MeaLine m_line1;            ///< Line from vertex to point 1
    MeaLine m_line2;            ///< Line from vertex to point 2
    MeaLine m_lineB;            ///< Line forming the angle bisector
    MeaDataWin m_dataWin1;      ///< Data window tooltip for point 1
    MeaDataWin m_dataWin2;      ///< Data window tooltip for point 2
    MeaDataWin m_dataWinV;      ///< Data window tooltip for the vertex
};
