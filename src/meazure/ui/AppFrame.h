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
/// @brief Header file for the application frame class.

#pragma once

#include "AppView.h"
#include <meazure/profile/Profile.h>
#include "StatusBar.h"


/// The frame class for the application. The application frame is the
/// root window for the application's window hierarchy.
///
class AppFrame : public CFrameWnd {

public:
    static constexpr bool kDefAlwaysVisible { true };   ///< Indicates whether the application's window is always the
                                                        ///< topmost window by default.

    /// Constructs the application's frame window.
    ///
    AppFrame();

    /// Destroys the application's frame window.
    virtual ~AppFrame();

#ifdef _DEBUG
    /// Verifies that the frame window is valid.
    ///
    virtual void AssertValid() const override;

    /// Dumps the state of the frame window.
    ///
    /// @param dc   [in] Device context for debugging output.
    ///
    virtual void Dump(CDumpContext& dc) const override;
#endif

    /// Returns the view window associated with the frame.
    /// Since this application is a single document style
    /// program, there is only one view held by the frame.
    ///
    /// @return The application's view window.
    ///
    AppView* GetView() { return &m_wndView; }

    /// Returns the keyboard accelerator table for the application.
    ///
    /// @return Handle for the application keyboard accelerator table.
    ///
    HACCEL GetAccelTable() { return m_hAccelTable; }

    /// Sets the window to either the topmost (i.e. always visible)
    /// position or a normal stacking order.
    ///
    void UpdateVisibility();

    /// Sets the filename specified on the command line, if any.
    ///
    /// @param fileName     [in] Filename specified on the application
    ///                     command line, or the empty string if no filename
    ///                     was specified.
    ///
    void SetCommandLineFile(const CString& fileName) { m_cmdLineFile = fileName; }

    /// Persists the frame's state to the specified profile object
    /// and initiates the persisting of the application view. Ultimately,
    /// this method causes the persistence of the entire state of the
    /// application.
    ///
    /// @param profile      [in] Profile object into which the state
    ///                     of the application is persisted.
    ///
    void SaveProfile(MeaProfile& profile);

    /// Restores the frame's state from the specified profile object
    /// and initiates the restoring of the application view. Ultimately,
    /// this method causes the restoration of the entire state of the
    /// application.
    ///
    /// @param profile      [in] Profile object from which the state
    ///                     of the application is restored.
    ///
    void LoadProfile(MeaProfile& profile);

    /// Called at startup right before the application windows are shown.
    /// Shows/hides the portions of the view according to the registry
    /// profile.
    ///
    void InitView();

    /// Indicates whether this is the first time the application has
    /// been run.
    ///
    /// @return <b>true</b> if this is the first time the application
    ///         has been run.
    ///
    bool IsNewInstall() const { return m_newInstall; }

    /// Forwards command messages to the view.
    /// @param nID              [in] ID of control issuing the message.
    /// @param nCode            [in] Message code.
    /// @param pExtra           [in] Pointer to extra data.
    /// @param pHandlerInfo     [in] Handler information structure.
    /// 
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) override;

protected:
    DECLARE_DYNAMIC(AppFrame)

    static UINT m_indicators[];         ///< Status bar indicators. Only one field is used.

    
    /// Called before the window is created to set the appearance
    /// of the application including setting the program's icon.
    /// @param cs       [in] Window creation structure.
    /// 
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;

    /// Creates the main frame window, the toolbar and the view.
    /// @param lpCreateStruct   [in] Window creation parameters structure.
    /// @return 0 if creation successful.
    /// 
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    /// Called when the frame gets the focus and forwards it on to the view.
    /// @param pOldWnd      [in] Window that had the focus previously.
    /// 
    afx_msg void OnSetFocus(CWnd* pOldWnd);

    /// Called when the Always Visible item is selected on the View menu.
    ///
    afx_msg void OnAlwaysVisible();

    /// Called right before the View menu is displayed to update the
    /// Always Visible menu item.
    /// @param pCmdUI       [in] UI command class to update the menu item.
    /// 
    afx_msg void OnUpdateAlwaysVisible(CCmdUI* pCmdUI);

    /// Called when the windows session is ending. Saves the state of the
    /// application to the registry.
    /// @param bEnding      [in] TRUE if the windows session is ending.
    /// 
    afx_msg void OnEndSession(BOOL bEnding);

    /// Called when the application window is closing. Saves the state of
    /// the application to the registry.
    /// 
    afx_msg void OnClose();

    /// Changes the window geometry according to whether the toolbar is
    /// displayed.
    /// 
    afx_msg void OnViewToolbar();

    /// Updates the view toolbar menu item.
    /// @param pCmdUI       [in] UI command class to update the menu item.
    /// 
    afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);

    /// Called when the status bar view menu item is selected. Toggles
    /// view / hide of the status bar.
    /// 
    afx_msg void OnViewStatusbar();

    /// Updates the view status bar menu item.
    /// @param pCmdUI       [in] UI command class to update the menu item.
    /// 
    afx_msg void OnUpdateViewStatusbar(CCmdUI* pCmdUI);

    /// Called before the Help menu is displayed. Tests to see
    /// if the help file exists.
    /// @param pCmdUI   [in] UI command class to update the menu item.
    /// 
    afx_msg void OnUpdateHelpMenu(CCmdUI* pCmdUI);

    /// Called to open the Meazure GitHub releases web page.
    ///
    afx_msg void OnMeazureReleases();

    /// Called to open the Meazure GitHub issue web page.
    ///
    afx_msg void OnReportIssue();

    /// Called when the Apply button is pressed on the preferences
    /// dialog.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns 0.
    /// 
    afx_msg LRESULT OnPrefsApply(WPARAM wParam, LPARAM lParam);

    /// Called to display the preferences dialog from the
    /// resolution calibration warning dialogs.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns 0.
    /// 
    afx_msg LRESULT OnShowCalPrefs(WPARAM wParam, LPARAM lParam);

    /// Called when a second copy of the application is launched with
    /// a file specified on the command line. This method receives
    /// the pathname of the file and loads it.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] COPYDATASTRUCT
    /// @return Always returns TRUE.
    /// 
    afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);

    /// Called when a master reset has been requested from the
    /// advanced page on the preferences dialog.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns TRUE.
    /// 
    afx_msg LRESULT OnMasterReset(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()


    CToolBar  m_wndToolBar;         ///< Application toolbar.
    MeaStatusBar m_wndStatusBar;    ///< Application status bar.
    AppView m_wndView;              ///< Application view window.
    bool m_alwaysVisible;           ///< Indicates if the application window is always top window on the desktop.
    bool m_profileToolbarVisible;   ///< Indicates if the persisted preference is for the toolbar to be displayed.
    bool m_profileStatusbarVisible; ///< Indicates if the persisted preference if for the status bar to be displayed.
    bool m_toolbarVisible;          ///< Indicates if the toolbar is currently visible.
    bool m_statusbarVisible;        ///< Indicates if the status bar is currently visible.

private:
    /// Creates the application toolbar.
    /// 
    /// @return <b>true</b> if toolbar successfully created.
    ///  
    bool CreateToolbar();

    /// Selects whether to show the toolbar.
    /// @param enable       [in] Indicates whether the toolbar should be visible.
    /// 
    void ViewToolbar(bool enable);

    /// Selects whether to show the status bar.
    /// @param enable       [in] Indicates whether the status bar should be visible.
    /// 
    void ViewStatusbar(bool enable);

    /// Opens the specified URL in a web browser.
    /// 
    /// @param urlId  String resource ID for the URL 
    /// @return true if the browser was opened.
    /// 
    bool OpenUrl(int urlId);


    bool m_newInstall;          ///< Indicates if this is the first time the application has been run.
    int m_toolbarHeight;        ///< Height of the toolbar, in pixels.
    int m_statusbarHeight;      ///< Height of the status bar, in pixels.
    int m_appWidth;             ///< Total width of the application, in pixels.
    CString m_cmdLineFile;      ///< Filename specified on the command line, if any.
};
