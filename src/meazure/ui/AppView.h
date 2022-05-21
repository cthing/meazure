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
/// @brief Header file for the application view.

#pragma once

#include "DataDisplay.h"
#include "Magnifier.h"
#include <meazure/prefs/Preferences.h>
#include <meazure/profile/Profile.h>
#include <meazure/tools/Tool.h>
#include <meazure/utilities/Timer.h>


/// The view class for the application. There is only a single view in this application and it is owned directly
/// by the application frame window.
///
class AppView : public CWnd {

public:
    static constexpr bool kDefToolbarVisible { true };      ///< Indicates if the toolbar is visible by default.
    static constexpr bool kDefStatusbarVisible { true };    ///< Indicates if the status bar is visible by default.
    static constexpr bool kDefToolInfoVisible { true };     ///< Indicates if the measurement information section is
                                                            ///< visible by default.
    static constexpr bool kDefScreenInfoVisible { true };   ///< Indicates if the screen information section is
                                                            ///< visible by default.
    static constexpr bool kDefMagnifierVisible { true };    ///< Indicates if the magnifier is visible by default.


    /// Constructs the view.
    ///
    AppView();

    /// Destroys the view.
    ///
    virtual ~AppView();

    /// A number of tools (e.g. pointer tool, magnifier) need to track
    /// the pointer position. This method is called by the application
    /// class when the system mouse hook is called. The method forwards
    /// the mouse hook information to the tools that are interested.
    ///
    /// @param wParam   [in] Message id.
    /// @param lParam   [in] MOUSEHOOKSTRUCT.
    ///
    void OnMouseHook(WPARAM wParam, LPARAM lParam);

    /// Persists the state of the view to the specified profile object.
    /// In addition, this method calls the SaveProfile method on all
    /// objects it contains.
    ///
    /// @param profile      [in] Profile object into which the state
    ///                     is persisted.
    ///
    void SaveProfile(MeaProfile& profile);

    /// Restores the state of the view from the specified profile object.
    /// In addition, this method calls the LoadProfile method on all
    /// objects it contains.
    ///
    /// @param profile      [in] Profile object from which the state
    ///                     is restored.
    ///
    void LoadProfile(MeaProfile& profile);

    /// Called when the application master reset is initiated. This method
    /// restores the view to its default state and calls the MasterReset
    /// method on all contained objects.
    ///
    void MasterReset();

    /// This method handles the processing of any files specified on the
    /// command-line or specified in preferences. The following processing
    /// is performed:
    /// <ol>
    ///     <li>If a profile file is specified on the command-line, that file
    ///         is loaded.</li>
    ///     <li>If no file is specified on the command-line, or the file is not
    ///         a profile, the preferences are checked to see if a startup profile
    ///         file has been specified. If it has, that file is loaded.</li>
    ///     <li>If a position log file is specified on the command-line, that file
    ///         is loaded.</li>
    /// </ol>
    ///
    /// @param cmdLineFile      [in] File pathname specified on the command-line.
    ///
    void LoadStartupFile(const CString& cmdLineFile) const;

    /// Called at startup right before the application windows are shown. Shows/hides
    /// the portions of the view according to the registry profile.
    ///
    void InitView();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

    virtual BOOL PreTranslateMessage(MSG* pMsg) override;

protected:
    /// Called when new units are selected from the Units menu.
    /// @param nID      [in] ID of the Units menu item that was selected.
    /// 
    afx_msg void OnUnits(UINT nID);

    /// Called when the degrees or radians units are selected.
    /// @param nID      [in] ID of the Units menu angle item that was selected.
    /// 
    afx_msg void OnAngles(UINT nID);

    /// Called when a radio tool is selected.
    /// @param nID      [in] ID of the tool menu item.
    /// 
    afx_msg void OnRadioTool(UINT nID);

    /// Called when a new color display format is requested.
    /// @param nID      [in] ID of the color format menu item.
    /// 
    afx_msg void OnColorFmt(UINT nID);

    /// Called when data is entered into a field in the data display or
    /// a spin button is used. Tells the current tool to set a new position.
    /// @param wParam   [in] New data value in pixels.
    /// @param lParam   [in] Data field that we modified.
    /// @return Always returns 0.
    /// 
    afx_msg LRESULT OnDataChange(WPARAM wParam, LPARAM lParam);

    /// Called when the Apply button is pressed on the preferences dialog.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns 0.
    /// 
    afx_msg LRESULT OnPrefsApply(WPARAM wParam, LPARAM lParam);

    /// Called when the show calibration preferences message is received.
    /// This message is issued by the resolution calibration warning
    /// dialogs and the calibration warning button.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns 0.
    /// 
    afx_msg LRESULT OnShowCalPrefs(WPARAM wParam, LPARAM lParam);

    /// Called when the magnifier wants the current position.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] POINT structure to fill with current position.
    /// @return Always returns TRUE.
    /// 
    afx_msg LRESULT OnGetPosition(WPARAM wParam, LPARAM lParam);

    /// Called when the region snapshot timer expires.
    /// @param wParam   [in] Not used
    /// @param lParam   [in] Not used.
    /// @return Always returns 0.
    /// 
    afx_msg LRESULT OnHPTimer(WPARAM wParam, LPARAM lParam);

    /// Creates the view and the data display. Also enables the
    /// appropriate tools.
    /// @param lpCreateStruct   [in] Window creation structure.
    /// @return 0 if the creation was successful.
    /// 
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    /// Updates the radio button state on the Units menu right
    /// before the menu is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateUnits(CCmdUI* pCmdUI);

    /// Updates the radio button state on the Units menu right
    /// before the menu is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateAngles(CCmdUI* pCmdUI);

    /// Called right before the Tool menu is displayed to update
    /// its radio state.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateRadioTool(CCmdUI* pCmdUI);

    /// Disables all tools including the magnifier and destroys the window.
    /// 
    afx_msg void OnDestroy();

    /// Wires the application Edit menu Copy item into the data display text
    /// field that has the keyboard focus so that its contents are copied to
    /// the clipboard.
    /// 
    afx_msg void OnEditCopy();

    /// Wires the application Edit menu Cut item into the data display text
    /// field that has the keyboard focus so that its contents are cut into
    /// the clipboard.
    /// 
    afx_msg void OnEditCut();

    /// Wires the application Edit menu Paste item into the data display text
    /// field that has the keyboard focus so that the clipboard contents are
    /// pasted into the field.
    /// 
    afx_msg void OnEditPaste();

    /// Wires the application Edit menu Delete item into the data display text
    /// field that has the keyboard focus so that its contents are deleted.
    /// 
    afx_msg void OnEditClear();

    /// Updates the application Edit menu Copy item so that its state reflects
    /// the Copy state of the data display text field that has the keyboard focus.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);

    /// Updates the application Edit menu Cut item so that its state reflects
    /// the Cut state of the data display text field that has the keyboard focus.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);

    /// Updates the application Edit menu Paste item so that its state reflects
    /// the Paste state of the data display text field that has the keyboard focus.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);

    /// Updates the application Edit menu Delete item so that its state reflects
    /// the Delete state of the data display text field that has the keyboard focus.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);

    /// Called when the Invert Y menu item is selected. The y-axis inversion is
    /// toggled.
    /// 
    afx_msg void OnInvertY();

    /// Updates the state of the Invert Y menu item before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateInvertY(CCmdUI* pCmdUI);

    /// Called when the Set Origin menu item is selected. The origin is repositioned
    /// to the current point.
    /// 
    afx_msg void OnOrigin();

    /// Called to reset the origin to its default location (0, 0). The default origin
    /// location is (0, 0) at the upper left corner of the screen. In y-axis inversion
    /// is in effect, the origin is located at (0, 0) in the lower left corner of the
    /// screen.
    /// 
    afx_msg void OnResetOrigin();

    /// Called when the ruler menu item is selected to toggle the display of the
    /// screen measurement rulers.
    /// 
    afx_msg void OnRuler();

    /// Updates the state of the ruler menu item before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateRuler(CCmdUI* pCmdUI);

    /// Called when the Preferences menu item is selected to bring up the preferences
    /// dialog.
    /// 
    afx_msg void OnPreferences();

    /// Called when the magnifier menu item is selected to toggle between showing and
    /// hiding the magnifier.
    /// 
    afx_msg void OnMagnifier();

    /// Updates the state of the magnifier item on the view menu before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateMagnifier(CCmdUI* pCmdUI);

    /// Called when the magnifier grid menu item is selected to toggle the display of
    /// the pixel grid on the magnifier window.
    /// 
    afx_msg void OnShowMagGrid();

    /// Updates the state of the magnifier grid menu item before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateShowMagGrid(CCmdUI* pCmdUI);

    /// Called when the Screen Info view menu item is selected. Toggles the display
    /// of the screen information section.
    /// 
    afx_msg void OnScreenInfo();

    /// Updates the state of the Screen Info view menu item before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateScreenInfo(CCmdUI* pCmdUI);

    /// Updates the state of the pixel color format menu item before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateColorFmt(CCmdUI* pCmdUI);

    /// Called to toggle between paused and running states on the magnifier window.
    /// 
    afx_msg void OnRunState();

    /// Updates the state of the pause button on the magnifier window and on the
    /// menu.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateRunState(CCmdUI* pCmdUI);

    /// Called to toggle the display of the screen grid.
    /// 
    afx_msg void OnScreenGrid();

    /// Updates the state of the screen grid menu item before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateScreenGrid(CCmdUI* pCmdUI);

    /// Called when the screen grid spacing menu item is selected. Brings up the
    /// screen grid spacing dialog.
    /// 
    afx_msg void OnScreenGridSpacing();

    /// Saves the current application configuration to the user selected profile file.
    /// 
    afx_msg void OnSaveProfile();

    /// Loads the user specified profile file to set the application configuration.
    /// 
    afx_msg void OnLoadProfile();

    /// Enabled when the Rectangle, Circle or Window tool is selected. Copies the area of
    /// the screen bounded by the tool to the Clipboard.
    /// 
    afx_msg void OnCopyRegion();

    /// If a tool reports that it has a region that can be copied, the Copy Region menu
    /// item is enabled. Otherwise the menu item is disabled.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateCopyRegion(CCmdUI* pCmdUI);

    /// Called when the Tool Info view menu item is selected. Toggles the display of
    /// the measurement section.
    /// 
    afx_msg void OnToolInfo();

    /// Updates the state of the Tool Info menu item before it is displayed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateToolInfo(CCmdUI* pCmdUI);

    /// Called to toggle the application window between a collapsed presentation
    /// and an expanded presentation. Note that the expanded state maintains the
    /// the display state of individual sections of the application so that expanding
    /// the application does not display sections that have been explicitly hidden.
    /// 
    afx_msg void OnCollapse();

    /// Updates the state of the collapse menu item to reflect whether the application is
    /// currently in a collapsed presentation.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateCollapse(CCmdUI* pCmdUI);

    /// Called to flash the current radio tool's crosshair(s).
    /// 
    afx_msg void OnFindCrosshair();

    /// Updates the flash menu item to reflect whether the tool has crosshairs that
    /// can be flashed.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateFindCrosshair(CCmdUI* pCmdUI);

    /// Called to record the current radio tool's position.
    afx_msg void OnRecordPosition();

    /// Called to delete all recorded positions.
    /// 
    afx_msg void OnDeletePositions();

    /// Updates the state of the delete positions menu item to reflect whether there
    /// are any positions that can be deleted.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateDeletePositions(CCmdUI* pCmdUI);

    /// Called to load a position log file.
    /// 
    afx_msg void OnLoadPositions();

    /// Called to save a position log file.
    /// 
    afx_msg void OnSavePositions();

    /// Called to save a position log file with a new pathname.
    /// 
    afx_msg void OnSavePositionsAs();

    /// Called to update the state of the position save menu items. If there are
    /// no positions currently in memory, the menu items are disabled.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateSavePositions(CCmdUI* pCmdUI);

    /// Called to bring up the position management dialog.
    /// 
    afx_msg void OnManagePositions();

    /// Called to increase magnification on the magnifier window.
    /// 
    afx_msg void OnZoomIn();

    /// Updates the state of the Zoom In menu item before it is displayed.
    /// If the magnifier is at maximum zoom, the Zoom In menu item is disabled.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);

    /// Called to decrease magnification on the magnifier window.
    /// 
    afx_msg void OnZoomOut();

    /// Updates the state of the Zoom Out menu item before it is displayed.
    /// If the magnifier is at minimum zoom, the Zoom Out menu item is disabled.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);

    /// Called to toggle between showing and hiding the tool crosshair(s).
    /// 
    afx_msg void OnHideCrosshairs();

    /// Updates the state of the Hide Crosshairs menu item to reflect whether
    /// the tool crosshair(s) are shown or hidden.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateHideCrosshairs(CCmdUI* pCmdUI);

    /// Called when the user selects the Define Custom units menu item.
    /// This method brings up the preferences dialog with the Custom Units
    /// property page selected.
    /// 
    afx_msg void OnDefineCustomUnits();

    /// Updates the custom units menu item to show the name of the custom
    /// units, if custom units have been defined.
    /// @param pCmdUI   [in] UI command object for updating the menu item.
    /// 
    afx_msg void OnUpdateCustomUnits(CCmdUI* pCmdUI);

    /// If custom units have been defined, this method is called to select
    /// them for use.
    /// 
    afx_msg void OnCustomUnits();

    // Called to set position 1 to the current location of the cursor.
    //
    afx_msg void OnSetPosition1ToCursor();

    // Called to set position 2 to the current location of the cursor.
    //
    afx_msg void OnSetPosition2ToCursor();

    // Called to set position 3 to the current location of the cursor.
    //
    afx_msg void OnSetPosition3ToCursor();

    DECLARE_MESSAGE_MAP()

private:
    /// Determines the desired width of the application menu.
    ///
    /// @return Pixel width to fit all menu items in one row.
    ///
    int GetMenuWidth();

    /// Applies the preferences currently specified on the Preferences dialog.
    ///
    /// @param prefPage     [in] Index of preference page whose settings
    ///                     should be applied.
    ///
    void ApplyPreferences(int prefPage = MeaPreferences::kAllPages);

    /// If no sections are shown, nuke the application margins. Otherwise add application margins.
    ///
    void CheckMargins();

    /// Determines whether all sections of the display are hidden.
    ///
    /// @return <b>true</b> if all sections hidden.
    ///
    bool IsCollapsed() const;

    /// Indicates whether the toolbar is being shown.
    ///
    /// @return <b>true</b> if the toolbar is displayed.
    ///
    bool IsToolbarVisible() const {
        return GetParentFrame()->GetControlBar(AFX_IDW_TOOLBAR)->IsVisible() ? true : false;
    }

    /// Indicates whether the status bar is being shown.
    ///
    /// @return <b>true</b> if the status bar is displayed.
    ///
    bool IsStatusbarVisible() const {
        return GetParentFrame()->GetControlBar(AFX_IDW_STATUS_BAR)->IsVisible() ? true : false;
    }

    /// Indicates whether the specified key is being pressed.
    ///
    /// @param keyCode  ASCII character code or VK_* for a virtual key.
    /// 
    static bool IsKeyPressed(int keyCode) {
        return GetKeyState(keyCode) & 0x8000;
    }

    /// Displays or hides the measurement data section.
    ///
    /// @param enable       [in] <b>true</b> if the section should be shown.
    ///
    void ViewToolInfo(bool enable);

    /// Displays or hides the screen information section.
    ///
    /// @param enable       [in] <b>true</b> if the section should be shown.
    ///
    void ViewScreenInfo(bool enable);

    /// Displays or hides the magnifier section
    ///
    /// @param enable       [in] <b>true</b> if the section should be shown.
    ///
    void ViewMagnifier(bool enable);

    CSize m_margin;                 ///< Vertical and horizontal margins around major sections.
    bool m_enabled;                 ///< Used in determining margins when the application is collapsed.
    bool m_profileMagnifierEnabled; ///< Indicates if the stored user preference is to show the magnifier window.
    bool m_profileScreenEnabled;    ///< Indicates if the stored user preference is to show
                                    ///< the screen information section.
    bool m_profileRegionEnabled;    ///< Indicates if the stored user preference is to show
                                    ///< the measurement data window.
    MeaDataDisplay m_dataDisplay;   ///< Tool and screen measurement data display object.
    MeaMagnifier m_magnifier;       ///< Screen capture and magnifier object.
    MeaPreferences m_prefs;         ///< Application preferences.
    CString m_startupProfile;       ///< Pathname for the startup profile, if any.
    MeaTimer m_snapshotTimer;       ///< Timer used in copying a tool's region to the clipboard.
    int m_adjustHeight;             ///< Adjustment used when computing the height of the application, in pixels.
    bool m_expandToolbar;           ///< Indicates if the toolbar is displayed.
    bool m_expandStatusbar;         ///< Indicates if the status bar is displayed.
    bool m_expandMagnifier;         ///< Indicates if the magnifier is displayed.
    bool m_expandScreenInfo;        ///< Indicates if the screen information section is displayed.
    bool m_expandToolInfo;          ///< Indicates if the tool measurement section is displayed.
};
