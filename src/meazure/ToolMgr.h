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
/// @brief Header file for the measurement tool manager.

#pragma once

#include <map>

#include "Profile.h"
#include "MeaAssert.h"
#include "ScreenTool.h"
#include "CursorTool.h"
#include "PointTool.h"
#include "LineTool.h"
#include "RectTool.h"
#include "CircleTool.h"
#include "AngleTool.h"
#include "WindowTool.h"
#include "RulerTool.h"
#include "GridTool.h"
#include "OriginTool.h"
#include "DataDisplay.h"
#include "Singleton.h"


/// Manages the measurement tools including selection,
/// enabling, disabling, and communicating various messages
/// to the tools.
///
/// The term "radio tool" refers to those measurement tools
/// that are mutually exclusive (i.e. only one radio tool can
/// be used at a time).
///
class MeaToolMgr : public MeaSingleton_T<MeaToolMgr>
{
public:
    static const bool   kDefCrosshairsEnabled;      ///< Crosshairs are enabled by default.


    /// Persists the state of the manager to the specified profile object.
    ///
    /// @param profile      [in] Profile object into which the state is
    ///                     to be persisted.
    ///
    void SaveProfile(MeaProfile& profile);

    /// Restores the state of the manager from the specified profile object.
    ///
    /// @param profile      [in] Profile object from which the state is
    ///                     to be restored.
    ///
    void LoadProfile(MeaProfile& profile);


    /// Resets the screen manager to its default state.
    ///
    void MasterReset();


    /// Sets the measurement data display object for use by each
    /// measurement tool.
    ///
    /// @param dataDisplay      [in] Display object
    ///
    void SetDataDisplay(MeaDataDisplay* dataDisplay) {
        m_dataDisplay = dataDisplay;
    }


    /// Makes the specified radio tool the current measurement tool.
    ///
    /// @param toolName     [in] Name of the radio tool to set.
    ///
    void SetRadioTool(const CString& toolName) {
        MeaTool* tool = FindTool(toolName);
        MeaAssert(tool->IsRadioTool());
        m_currentRadioTool = static_cast<MeaRadioTool*>(tool);
        ChangeRadioToolEnable();
    }

    /// Disables all radio tools. Among other things, disabling
    /// a tool hides it.
    ///
    void DisableRadioTools() {
        if (m_dataDisplay != NULL)
            m_dataDisplay->ClearRegionData();
        ToolsMap::const_iterator iter;
        for (iter = m_tools.begin(); iter != m_tools.end(); ++iter) {
            MeaTool *tool = (*iter).second;
            if (tool->IsRadioTool() && tool->IsEnabled()) {
                tool->Disable();
            }
        }
    }

    /// Returns the measurement tool corresponding to the
    /// specified tool name.
    ///
    /// @param toolName     [in] Name of the measurement tool.
    ///
    /// @return Measurement tool with the specified name.
    ///
    MeaTool* FindTool(const CString& toolName) const {
        ToolsMap::const_iterator iter = m_tools.find(toolName);
        MeaAssert(iter != m_tools.end());
        return (*iter).second;
    }

    /// Returns the name of the currently set radio tool.
    ///
    /// @return Name of the currently set radio tool.
    ///
    LPCTSTR GetToolName() const {
        return m_currentRadioTool->GetToolName();
    }

    /// Returns the ID of the currently set radio tool.
    ///
    /// @return ID of the currently set radio tool.
    ///
    UINT    GetLabelId() const {
        return m_currentRadioTool->GetLabelId();
    }

    /// Indicates if the specified tool is enabled.
    ///
    /// @return <b>true</b> if the specified tool is enabled.
    ///
    bool IsToolEnabled(const CString& toolName) const {
        return FindTool(toolName)->IsEnabled();
    }

    /// Enables the specified tool.
    ///
    /// @param toolName     [in] Name of tool to enable.
    ///
    void EnableTool(const CString& toolName) const {
        FindTool(toolName)->Enable();
    }

    /// Disables the specified tool.
    ///
    /// @param toolName     [in] Name of tool to disable.
    ///
    void DisableTool(const CString& toolName) const {
        FindTool(toolName)->Disable();
    }

    /// Enables the currently selected radio tool.
    ///
    void EnableRadioTool() const {
        m_currentRadioTool->Enable();
    }

    /// Disables the currently selected radio tool.
    ///
    void DisableRadioTool() const {
        m_currentRadioTool->Disable();
    }

    /// Disables all radio tools and enables the
    /// currently selected radio tool.
    ///
    void ChangeRadioToolEnable() {
        DisableRadioTools();
        EnableRadioTool();
        if (m_dataDisplay != NULL) {
            m_dataDisplay->SetRegionLabel(GetLabelId());
        }
    }
    
    /// If the specified tool is enabled, this method
    /// disables it, and vice versa.
    ///
    /// @param toolName     [in] Name of the tool to toggle.
    ///
    void ToggleEnableTool(const CString& toolName) const {
        MeaTool* tool = FindTool(toolName);
        if (tool->IsEnabled()) {
            tool->Disable();
        } else {
            tool->Enable();
        }
    }

    /// Tells all tools to update their visual display.
    ///
    /// @param reason       [in] Reason the update was requested.
    ///
    void UpdateTools(MeaUpdateReason reason = NormalUpdate) const {
        ToolsMap::const_iterator iter;
        for (iter = m_tools.begin(); iter != m_tools.end(); ++iter) {
            (*iter).second->Update(reason);
        }
    }

    /// Informs all tools that the color style sheet has changed.
    ///
    void ColorsChanged() const {
        ToolsMap::const_iterator iter;
        for (iter = m_tools.begin(); iter != m_tools.end(); ++iter) {
            (*iter).second->ColorsChanged();
        }
    }

    /// Indicates if the currently selected radio tool uses crosshairs.
    ///
    /// @return <b>true</b> if the currently selected radio tool uses
    ///         crosshairs.
    ///
    bool HasCrosshairs() const {
        return m_currentRadioTool->HasCrosshairs();
    }

    /// Indicates whether the display of crosshairs is enabled.
    ///
    /// @return <b>true</b> if the display of crosshairs is enabled.
    ///
    bool CrosshairsEnabled() const {
        return m_crosshairsEnabled;
    }

    /// Enables the display of crosshairs.
    ///
    void EnableCrosshairs() {
        m_crosshairsEnabled = true;
        m_currentRadioTool->EnableCrosshairs();
    }

    /// Disables the display of crosshairs.
    ///
    void DisableCrosshairs() {
        m_crosshairsEnabled = false;
        m_currentRadioTool->DisableCrosshairs();
    }

    /// Causes the current radio tool to flash. Typically flashing
    /// a tool causes its crosshairs to cycle a number of times
    /// between their normal visual state and a hilited state.
    ///
    void FlashTool() const { m_currentRadioTool->Flash(); }
    
    /// Causes the current radio tool to strobe. Typically strobing
    /// a tool causes its crosshairs to cycle once between their
    /// normal visual state and a hilited state.
    ///
    void StrobeTool() const { m_currentRadioTool->Strobe(); }


    /// Called by the OS when the mouse pointer is moved.
    ///
    /// @param wParam   [in] OS Message ID
    /// @param lParam   [in] MOUSEHOOKSTRUCT
    ///
    void OnMouseHook(WPARAM wParam, LPARAM lParam) {
        m_currentRadioTool->OnMouseHook(wParam, lParam);
    }


    /// Sets the position of the current radio tool. The
    /// position of a specific part of the tool is set.
    ///
    /// @param which        [in] Specifies the component of the tools position to set.
    /// @param pixels       [in] Position value for the component.
    ///
    void    SetPosition(MeaFields which, int pixels) {
        static_cast<MeaTool*>(m_currentRadioTool)->SetPosition(which, pixels);
    }
    
    /// Sets the position of the current radio tool.
    ///
    /// @param points   [in] Complete set of points for the tool.
    ///
    void    SetPosition(const MeaRadioTool::PointMap& points) {
        m_currentRadioTool->SetPosition(points);
    }

    /// Sets the specified position based on the current location of the cursor.
    /// 
    /// @param xfield  [in] X position to set
    /// @param yfield  [in] Y position to set
    /// 
    void    SetPositionToCursor(MeaFields xfield, MeaFields yfield);

    /// Returns a representative position from the current radio tool.
    ///
    /// @return The current radio tool returns a point that represents
    ///         its current position.
    ///
    const POINT&    GetPosition() const {
        return static_cast<MeaTool*>(m_currentRadioTool)->GetPosition();
    }

    /// Returns the current radio tool's position as a position object.
    /// The position object contains the complete set of points that
    /// describe the location of the tool.
    ///
    /// @param position     [out] Position representing the location of
    ///                     the current radio tool.
    ///
    void    GetPosition(MeaPositionLogMgr::Position& position) const {
        m_currentRadioTool->GetPosition(position);
    }

    /// Increments the specified position field. 
    /// 
    /// @param which Position field to increment
    ///
    void    IncPosition(MeaFields which) {
        m_currentRadioTool->IncPosition(which);
    }

    /// Decrements the specified position field. 
    /// 
    /// @param which Position field to decrement
    ///
    void    DecPosition(MeaFields which) {
        m_currentRadioTool->DecPosition(which);
    }

    /// Indicates if the current radio tool has a rectangular region
    /// that can be captured.
    ///
    /// @return <b>true</b> if the current radio tool has a rectangular
    ///         region that can be captured.
    ///
    bool HasRegion() const { return m_currentRadioTool->HasRegion(); }
    
    /// Obtains the current radio tools rectangular region.
    ///
    /// @return The current radio tools rectangular region.
    ///
    RECT GetRegion() const { return m_currentRadioTool->GetRegion(); }


    /// Sets the text in the status bar using the specified
    /// string resource ID.
    ///
    /// @param id       [in] String resource ID.
    ///
    void SetStatus(UINT id) const;


    /// Supervises enabling the appropriate fields in the
    /// measurement region portion of the UI.
    ///
    /// @param enableFields     [in] Bitmask indicating which fields
    ///                         on the UI should be enabled.
    /// @param editableFields   [in] Bitmask indicating which fields
    ///                         on the UI should be editable.
    ///
    void EnableRegionFields(UINT enableFields, UINT editableFields);

    /// Supervises enabling the appropriate fields in the
    /// screen information portion of the UI.
    ///
    /// @param enableFields     [in] Bitmask indicating which fields
    ///                         on the UI should be enabled.
    /// @param editableFields   [in] Bitmask indicating which fields
    ///                         on the UI should be editable.
    ///
    void EnableScreenFields(UINT enableFields, UINT editableFields);


    /// Updates the display of the screen size and resolution based
    /// on the screen containing the specified point. In the case
    /// where there are multiple monitors attached to the system, the
    /// screen information display needs to be updated to reflect the
    /// screen containing the point being manipulated.
    ///
    /// @param pos      [in] Point defining a screen.
    /// 
    void UpdateScreenInfo(const POINT& pos) {
        if (m_screenTool != NULL) {
            m_screenTool->SetPosition(pos);
        }
    }


    /// Displays the specified point in the X1/Y1 fields
    /// of the data display and on the rulers, if they
    /// are visible.
    ///
    /// @param point    [in] Point, in pixels.
    /// @param cpoint   [in] Point, in the current units.
    ///
    void ShowXY1(const POINT& point, const FPOINT& cpoint) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowXY1(point, cpoint);
        }
        m_rulerTool->SetIndicator(MeaRuler::Ind1, point);
    }

    /// Displays the specified point in the X2/Y2 fields
    /// of the data display and on the rulers, if they
    /// are visible.
    ///
    /// @param point    [in] Point, in pixels.
    /// @param cpoint   [in] Point, in the current units.
    ///
    void ShowXY2(const POINT& point, const FPOINT& cpoint) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowXY2(point, cpoint);
        }
        m_rulerTool->SetIndicator(MeaRuler::Ind2, point);
    }

    /// Displays the specified point in the XV/YV fields
    /// of the data display and on the rulers, if they
    /// are visible.
    ///
    /// @param point    [in] Point, in pixels.
    /// @param cpoint   [in] Point, in the current units.
    ///
    void ShowXYV(const POINT& point, const FPOINT& cpoint) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowXYV(point, cpoint);
        }
        m_rulerTool->SetIndicator(MeaRuler::Ind3, point);
    }

    /// Displays the specified size in the width and height
    /// fields of the data display.
    ///
    /// @param size     [in] Width and height, in the current units
    ///
    void ShowWH(const FSIZE& size) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowWH(size);
        }
    }

    /// Displays a distance calculated from the specified size
    /// in the distance field of the data display.
    ///
    /// @param size     [in] The distance is calculated from
    ///                 this parameter, in the current units
    ///
    void ShowDistance(const FSIZE& size) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowDistance(size);
        }
    }

    /// Displays the specified value in the distance field of
    /// the data display.
    ///
    /// @param dist     [in] The distance, in the current units
    ///
    void ShowDistance(double dist) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowDistance(dist);
        }
    }

    /// Displays the specified angle in the angle field of
    /// the data display.
    ///
    /// @param angle    [in] Angle value, in the current angular units.
    ///
    void ShowAngle(double angle) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowAngle(angle);
        }
    }

    /// Displays the aspect ratio of the specified size in the aspect ratio field of
    /// the data display.
    ///
    /// @param size    [in] Width and height of the bounding rectangle.
    ///
    void ShowAspect(const FSIZE& size) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowAspect(size);
        }
    }

    /// Displays the rectangular area calculated from the
    /// specified width and height in the area field of the
    /// data display.
    ///
    /// @param size     [in] Width and height from which the
    ///                 rectangular area is calculated, in current units.
    ///
    void ShowRectArea(const FSIZE& size) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowRectArea(size);
        }
    }

    /// Displays the area of a circle with the specified radius
    /// in the area field of the data display.
    ///
    /// @param radius       [in] Radius from which the circle's
    ///                     area is calculated, in the current units.
    ///
    void ShowCircleArea(double radius) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowCircleArea(radius);
        }
    }

    /// Displays the name of the current screen.
    ///
    /// @param name     [in] Name to be displayed.
    ///
    void ShowScreenName(const CString& name) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowScreenName(name);
        }
    }

    /// Displays the specified width and height
    /// in the appropriate screen fields.
    ///
    /// @param size     [in] Width and height, in the current units.
    ///
    void ShowScreenWH(const FSIZE& size) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowScreenWH(size);
        }
    }

    /// Displays the specified resolution in the
    /// appropriate screen fields.
    ///
    /// @param res      [in] Screen resolution, in the current units.
    ///
    void ShowScreenRes(const FSIZE& res) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowScreenRes(res);
        }
    }

    /// Displays or hides the screen calibration button
    ///
    /// @param show     [in] <b>true</b> if the screen resolution
    ///                 calibration button should be displayed.
    ///
    void ShowCalButton(bool show) {
        if (m_dataDisplay != NULL) {
            m_dataDisplay->ShowCalButton(show);
        }
    }

private:
    typedef std::map<CString, MeaTool*> ToolsMap;   ///< Maps a tool name to the tool object.

    MEA_SINGLETON_DECL(MeaToolMgr);     ///< Managers are singletons.

    /// Purposely undefined.
    MeaToolMgr(const MeaToolMgr&);

    /// Purposely undefined.
    MeaToolMgr& operator=(const MeaToolMgr&);
    
    MeaDataDisplay  *m_dataDisplay;         ///< Data display object.
    bool            m_crosshairsEnabled;    ///< Are crosshairs enabled.
    ToolsMap        m_tools;                ///< All measurement tools.
    MeaRadioTool    *m_currentRadioTool;    ///< Currently selected radio tool.

    MeaScreenTool   *m_screenTool;          ///< Screen information tool.
    MeaCursorTool   *m_cursorTool;          ///< Cursor position tool.
    MeaPointTool    *m_pointTool;           ///< Point position tool.
    MeaLineTool     *m_lineTool;            ///< Line measurement tool.
    MeaRectTool     *m_rectTool;            ///< Rectangle tool.
    MeaCircleTool   *m_circleTool;          ///< Circle measurement tool.
    MeaAngleTool    *m_angleTool;           ///< Angle measurement tool.
    MeaWindowTool   *m_windowTool;          ///< Window hierarchy measurement tool.
    MeaRulerTool    *m_rulerTool;           ///< Screen rulers.
    MeaGridTool     *m_gridTool;            ///< Screen grid.
    MeaOriginTool   *m_originTool;          ///< Origin marker.
};
