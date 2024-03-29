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
 /// @brief Point measurement tool header file.

#pragma once

#include "RadioTool.h"
#include <meazure/graphics/CrossHair.h>
#include <meazure/ui/DataWin.h>
#include <meazure/position/Position.h>


/// Point measurement tool. This tool provides a crosshair that can be
/// placed arbitrarily on the screen to measure a location.
///
class MeaPointTool : public MeaRadioTool, public MeaCrossHairCallback {

public:
    static constexpr PCTSTR kToolName { _T("PointTool") };

    /// Constructs a new instance of a Point measurement tool. To use
    /// a newly constructed Point tool, the Enable() method must be called.
    /// Enable() will call the Create() method the first time it is called
    /// if Create() has not previously been called.
    ///
    /// @param mgr              [in] Tool manager parent of the Line tool
    /// @param screenProvider   [in] Screen information provider
    /// @param unitsProvider    [in] Units information provider
    ///
    explicit MeaPointTool(MeaToolMgr& mgr, const MeaScreenProvider& screenProvider,
                          const MeaUnitsProvider& unitsProvider);

    /// Hides and then destroys the Point tool.
    //
    virtual ~MeaPointTool();

    /// Displays the Point tool. If the Create() method has not been
    /// previously called, it will be called by Enable.
    ///
    virtual void Enable() override;

    /// Hides the Point tool.
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

    /// Displays the tool's crosshair. If the Enable() method has not
    /// been previously called, this method has no effect.
    ///
    virtual void EnableCrosshairs() override;

    /// Hides the tool's crosshair.
    ///
    virtual void DisableCrosshairs() override;

    /// Visually flashes the tool. The tool crosshair is cycled
    /// between a highlight color and its normal color a number of times.
    /// Flashing the tool helps the user find it on the screen.
    ///
    virtual void Flash() override;

    /// Visually strobes the tool. The tool crosshair is cycled
    /// between a highlight color and its normal color once. In addition,
    /// the data window attached to the crosshair is cycled between
    /// hidden and visible. Strobing the tool is done to visually confirm
    /// position recording.
    ///
    virtual void Strobe() override;

    /// Sets the position of the tool. This method sets the position
    /// one location component at a time. For example, the x position
    /// of the crosshair is specified by calling this method with the
    /// new position and specifying the MeaX1Field for the <i>which</i>
    /// parameter.
    ///
    /// @param which    [in] Position component
    /// @param pixels   [in] Position to set for the component, in pixels
    ///
    virtual void SetPosition(MeaDataFieldId which, int pixels) override;

    /// Sets the position of the tool. This method is called by the
    /// position log manager to play back a tool's position that was
    /// previously recorded using the RecordPosition method.
    ///
    /// @param points   [in] Map of positions for the tool's crosshairs.
    ///
    virtual void SetPosition(const PointMap& points) override;

    /// Returns the position of the crosshair.
    ///
    /// @return Current position of the tool's crosshair.
    ///
    virtual const POINT& GetPosition() const override;

    /// Records the position of the tool crosshair. This method is called
    /// by the position log manager to record a position.
    ///
    /// @param position     [in] The tool's crosshair position is recorded
    ///                     into the position log manager's position object.
    ///
    virtual void RecordPosition(MeaPosition& position) const override;

    /// Increments the specified position field.
    /// 
    /// @param which Field to increment
    ///
    virtual void IncPosition(MeaDataFieldId which) override;

    /// Decrements the specified position field.
    /// 
    /// @param which Field to decrement
    ///
    virtual void DecPosition(MeaDataFieldId which) override;

    /// Returns the name of the tool. Each tool has a unique name
    /// which is used to identify the tool in profiles and position
    /// logs.
    ///
    /// @return Always "PointTool". The memory for the string is owned by the tool.
    ///
    virtual PCTSTR GetToolName() const override;

    /// Returns a resource ID for the string used to label this
    /// tool on the data display.
    ///
    /// @return Resource ID for the label.
    ///
    virtual UINT GetLabelId() const override;

    /// Called to notify the tool that the colors in the MeaColors class
    /// have been changed. The crosshair is redrawn in the new color and
    /// the crosshair and data window opacities are updated.
    ///
    virtual void ColorsChanged() override;

    /// Called when the pointer the tool's crosshair.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHEnter(const CrossHairInfo* info) override;

    /// Called when the pointer leaves the tool's crosshair.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHLeave(const CrossHairInfo* info) override;

    /// Called when the user attempts to drag the tool's crosshair. The
    /// info parameter indicates where the crosshair is to be moved and
    /// the tool actually moves the crosshair to the new position. This
    /// allows the tool to perform vertical or horizontal locking and
    /// move the entire tool depending on which keyboard modifiers are
    /// pressed during the drag.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHMove(const CrossHairInfo* info) override;

private:
    /// Creates the tool's crosshair and data window. The Enable()
    /// method must be called to make the tool visible.
    ///
    /// @return <b>true</b> if the window is successfully created.
    ///
    bool Create();

    /// Sets the position of the tool's crosshair based on the current
    /// value of #m_center.
    ///
    void SetPosition();


    CPoint m_center;            ///< Position of the crosshair
    CPoint m_anchorPoint;       ///< Location for vertical / horizontal lock when Shift is held while dragging
    MeaCrossHair m_crossHair;   ///< Crosshair for the tool
    MeaDataWin m_dataWin;       ///< Data window tooltip for the tool
};
