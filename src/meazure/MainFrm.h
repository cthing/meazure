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

#include "ChildView.h"
#include "Profile.h"
#include "StatusBar.h"


/// The frame class for the application. The application frame is the
/// root window for the application's window hierarchy.
///
class CMainFrame : public CFrameWnd
{
public:
    static const bool kDefAlwaysVisible;    ///< Indicates whether the app's
                                            ///< window is always the topmost
                                            ///< window by default.

    /// Constructs the application's frame window.
    ///
    CMainFrame();

    /// Destroys the application's frame window.
    virtual ~CMainFrame();

#ifdef _DEBUG
    /// Verifies that the frame window is valid.
    ///
    virtual void AssertValid() const;
    
    /// Dumps the state of the frame window.
    ///
    /// @param dc   [in] Device context for debugging output.
    ///
    virtual void Dump(CDumpContext& dc) const;
#endif

    /// Returns the view window associated with the frame.
    /// Since this application is a single document style
    /// program, there is only one view held by the frame.
    ///
    /// @return The application's view window.
    ///
    CChildView* GetView() { return &m_wndView; }

    /// Returns the keyboard accelerator table for the application.
    ///
    /// @return Handle for the application keyboard accelerator table.
    ///
    HACCEL  GetAccelTable() { return m_hAccelTable; }

    /// Sets the window to either the topmost (i.e. always visible)
    /// position or a normal stacking order.
    ///
    void    UpdateVisibility();

    /// Sets the filename specified on the command line, if any.
    ///
    /// @param fileName     [in] Filename specified on the application
    ///                     command line, or the empty string if no filename
    ///                     was specified.
    ///
    void    SetCommandLineFile(const CString& fileName) { m_cmdLineFile = fileName; }

    
    /// Persists the frame's state to the specified profile object
    /// and initiates the persisting of the application view. Ultimately,
    /// this method causes the persistence of the entire state of the
    /// application.
    ///
    /// @param profile      [in] Profile object into which the state
    ///                     of the application is persisted.
    ///
    void    SaveProfile(MeaProfile& profile);
    
    /// Restores the frame's state from the specified profile object
    /// and initiates the restoring of the application view. Ultimately,
    /// this method causes the restoration of the entire state of the
    /// application.
    ///
    /// @param profile      [in] Profile object from which the state
    ///                     of the application is restored.
    ///
    void    LoadProfile(MeaProfile& profile);

    /// Called at startup right before the app windows are shown.
    /// Shows/hides the portions of the view according to the registry
    /// profile.
    ///
    void    InitView();

    /// Indicates whether this is the first time the application has
    /// been run.
    ///
    /// @return <b>true</b> if this is the first time the application
    ///         has been run.
    ///
    bool    IsNewInstall() const { return m_newInstall; }

protected: 
    DECLARE_DYNAMIC(CMainFrame)

    //{{AFX_VIRTUAL(CMainFrame)
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    //}}AFX_VIRTUAL

    /// @fn PreCreateWindow(CREATESTRUCT& cs)
    /// Called before the window is created to set the appearance
    /// of the application including setting the program's icon.
    /// @param cs       [in] Window creation structure.

    /// @fn OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
    /// Forwards command messages to the view.
    /// @param nID              [in] ID of control issuing the message.
    /// @param nCode            [in] Message code.
    /// @param pExtra           [in] Pointer to extra data.
    /// @param pHandlerInfo     [in] Handler information structure.

protected:
    static UINT m_indicators[];         ///< Status bar indicators. Only one field is used.

    CToolBar        m_wndToolBar;       ///< Application toolbar.
    MeaStatusBar    m_wndStatusBar;     ///< Application status bar.
    CChildView      m_wndView;          ///< Application view window.

    bool    m_alwaysVisible;            ///< Indicates if the application window is always top window on the desktop.
    bool    m_profileToolbarVisible;    ///< Indicates if the persisted preference is for the toolbar to be displayed.
    bool    m_profileStatusbarVisible;  ///< Indicates if the persisted preference if for the status bar to be displayed.
    bool    m_toolbarVisible;           ///< Indicates if the toolbar is currently visible.
    bool    m_statusbarVisible;         ///< Indicates if the status bar is currently visible.

    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnAlwaysVisible();
    afx_msg void OnUpdateAlwaysVisible(CCmdUI* pCmdUI);
    afx_msg void OnEndSession(BOOL bEnding);
    afx_msg void OnClose();
    afx_msg void OnViewToolbar();
    afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
    afx_msg void OnViewStatusbar();
    afx_msg void OnUpdateViewStatusbar(CCmdUI* pCmdUI);
    //}}AFX_MSG

    /// @fn OnCreate(LPCREATESTRUCT lpCreateStruct)
    /// Creates the main frame window, the toolbar and the view.
    /// @param lpCreateStruct   [in] Window creation parameters structure.
    /// @return 0 if creation successful.

    /// @fn OnSetFocus(CWnd *pOldWnd)
    /// Called when the frame gets the focus and forwards it on to the view.
    /// @param pOldWnd      [in] Window that had the focus previously.

    /// @fn OnAlwaysVisible()
    /// Called when the Always Visible item is selected on the View menu.
    
    /// @fn OnUpdateAlwaysVisible(CCmdUI* pCmdUI)
    /// Called right before the View menu is displayed to update the
    /// Always Visible menu item.
    /// @param pCmdUI       [in] UI command class to update the menu item.
    
    /// @fn OnEndSession(BOOL bEnding)
    /// Called when the windows session is ending. Saves the state of the
    /// application to the registry.
    /// @param bEnding      [in] TRUE if the windows session is ending.
    
    /// @fn OnClose()
    /// Called when the application window is closing. Saves the state of
    /// the application to the registry.
    
    /// @fn OnViewToolbar()
    /// Changes the window geometry according to whether the toolbar is
    /// displayed.
    
    /// @fn OnUpdateViewToolbar(CCmdUI* pCmdUI)
    /// Updates the view toolbar menu item.
    /// @param pCmdUI       [in] UI command class to update the menu item.

    /// @fn OnViewStatusbar()
    /// Called when the status bar view menu item is selected. Toggles
    /// view / hide of the status bar.
    
    /// @fn OnUpdateViewStatusbar(CCmdUI* pCmdUI)
    /// Updates the view status bar menu item.
    /// @param pCmdUI       [in] UI command class to update the menu item.

    /// Called when the Search / Index item is selected on the
    /// Help menu.
    afx_msg void OnHelpSearch();

    /// Called when the Search / Index item is selected on the
    /// Help menu.
    afx_msg void OnHelpIndex();

    /// Called before the Help menu is displayed. Tests to see
    /// if the help file exists.
    /// @param pCmdUI   [in] UI command class to update the menu item.
    afx_msg void OnUpdateHelpMenu(CCmdUI* pCmdUI);

    /// Called to open the Meazure GitHub releases web page.
    afx_msg void OnMeazureReleases();

    /// Called to open the Meazure GitHub issue web page.
    ///
    afx_msg void OnReportIssue();

    /// Called when the Apply button is pressed on the preferences
    /// dialog.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns 0.
    afx_msg LRESULT OnPrefsApply(WPARAM wParam, LPARAM lParam);

    /// Called to display the preferences dialog from the
    /// resolution calibration warning dialogs.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns 0.
    afx_msg LRESULT OnShowCalPrefs(WPARAM wParam, LPARAM lParam);

    /// Called when a second copy of the app is launched with
    /// a file specified on the command line. This method receives
    /// the pathname of the file and loads it.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] COPYDATASTRUCT
    /// @return Always returns TRUE.
    afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);

    /// Called when a master reset has been requested from the
    /// advanced page on the preferences dialog.
    /// @param wParam   [in] Not used.
    /// @param lParam   [in] Not used.
    /// @return Always returns TRUE.
    afx_msg LRESULT OnMasterReset(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    /// Selects whether to show the toolbar.
    /// @param enable       [in] Indicates whether the toolbar should be visible.
    void    ViewToolbar(bool enable);

    /// Selects whether to show the status bar.
    /// @param enable       [in] Indicates whether the status bar should be visible.
    void    ViewStatusbar(bool enable);

    /// Opens the specified URL in a web browser.
    /// 
    /// @param urlId  String resource ID for the URL 
    /// @return true if the browser was opened.
    bool    OpenUrl(int urlId);

    bool    m_newInstall;           ///< Indicates if this is the first time the app has been run.
    int     m_toolbarHeight;        ///< Height of the toolbar, in pixels.
    int     m_statusbarHeight;      ///< Height of the status bar, in pixels.
    int     m_appWidth;             ///< Total width of the application, in pixels.
    CString m_cmdLineFile;          ///< Filename specified on the command line, if any.
};


//{{AFX_INSERT_LOCATION}}
