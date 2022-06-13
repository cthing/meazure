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

#include "pch.h"
#include "AppFrame.h"
#include "resource.h"
#include "profile/RegistryProfile.h"
#include "prefs/Preferences.h"
#include "ui/Layout.h"
#include "ui/ScreenMgr.h"
#include "utilities/Registry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(AppFrame, CFrameWnd)


BEGIN_MESSAGE_MAP(AppFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_MEA_ONTOP, OnAlwaysVisible)
    ON_UPDATE_COMMAND_UI(ID_MEA_ONTOP, OnUpdateAlwaysVisible)
    ON_WM_ENDSESSION()
    ON_WM_CLOSE()
    ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
    ON_COMMAND(ID_VIEW_STATUSBAR, OnViewStatusbar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_STATUSBAR, OnUpdateViewStatusbar)
    // Global help commands
    ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
    ON_UPDATE_COMMAND_UI(ID_HELP_FINDER, OnUpdateHelpMenu)
    ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
    ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
    ON_UPDATE_COMMAND_UI(ID_CONTEXT_HELP, OnUpdateHelpMenu)
    ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
    ON_COMMAND(ID_HELP_MEAZURERELEASES, OnMeazureReleases)
    ON_COMMAND(ID_HELP_REPORTANISSUE, OnReportIssue)
    ON_MESSAGE(MeaPrefsApplyMsg, OnPrefsApply)
    ON_MESSAGE(MeaShowCalPrefsMsg, OnShowCalPrefs)
    ON_MESSAGE(WM_COPYDATA, OnCopyData)
    ON_MESSAGE(MeaMasterResetMsg, OnMasterReset)
END_MESSAGE_MAP()


UINT AppFrame::m_indicators[] { ID_SEPARATOR };


AppFrame::AppFrame() :
    CFrameWnd(),
    m_alwaysVisible(kDefAlwaysVisible),
    m_profileToolbarVisible(AppView::kDefToolbarVisible),
    m_profileStatusbarVisible(AppView::kDefStatusbarVisible),
    m_toolbarVisible(AppView::kDefToolbarVisible),
    m_statusbarVisible(AppView::kDefStatusbarVisible),
    m_newInstall(true), m_toolbarHeight(0),
    m_statusbarHeight(0), m_appWidth(0) {}

AppFrame::~AppFrame() {}

int AppFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | CBRS_TOP |
                               CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    m_wndToolBar.SetButtonStyle(0, TBBS_CHECKGROUP);
    m_wndToolBar.SetButtonStyle(1, TBBS_CHECKGROUP);
    m_wndToolBar.SetButtonStyle(2, TBBS_CHECKGROUP);
    m_wndToolBar.SetButtonStyle(3, TBBS_CHECKGROUP);
    m_wndToolBar.SetButtonStyle(4, TBBS_CHECKGROUP);
    m_wndToolBar.SetButtonStyle(5, TBBS_CHECKGROUP);
    m_wndToolBar.SetButtonStyle(6, TBBS_CHECKGROUP);
    m_wndToolBar.SetButtonStyle(7, TBBS_SEPARATOR);
    m_wndToolBar.SetButtonStyle(8, TBBS_CHECKBOX);
    m_wndToolBar.SetButtonStyle(9, TBBS_CHECKBOX);

    if (!m_wndStatusBar.Create(this) ||
            !m_wndStatusBar.SetIndicators(m_indicators,
                                          sizeof(m_indicators) / sizeof(UINT))) {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }
    if (!m_wndView.CreateEx(WS_EX_CONTROLPARENT, nullptr, nullptr, AFX_WS_DEFAULT_VIEW,
                            CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr)) {
        TRACE0("Failed to create view window\n");
        return -1;
    }

    CRect toolbarRect;
    CRect statusbarRect;
    CRect frameRect;

    m_wndToolBar.GetWindowRect(toolbarRect);
    m_toolbarHeight = toolbarRect.Height();
    m_wndStatusBar.GetWindowRect(statusbarRect);
    m_statusbarHeight = statusbarRect.Height();
    GetWindowRect(frameRect);
    m_appWidth = frameRect.Width();

    MeaLayout::SetWindowSize(*this, m_appWidth,
        frameRect.Height() + m_toolbarHeight + m_statusbarHeight);

    // Restore the state of the program from the registry.
    //
    MeaRegistry registry;
    MeaRegistryProfile profile(registry);
    LoadProfile(profile);

    // Tell everyone to initialize its view
    //
    m_wndView.InitView();
    InitView();

    // Show the application.
    //
    UpdateVisibility();
    ShowWindow(SW_SHOW);
    UpdateWindow();

    // Load the startup file, if any.
    //
    m_wndView.LoadStartupFile(m_cmdLineFile);

    return 0;
}

BOOL AppFrame::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(0, 0, 0, AfxGetApp()->LoadIcon(IDR_MAINFRAME));
    return TRUE;
}

void AppFrame::SaveProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(&wp);

        profile.WriteInt(_T("WindowFlags"), wp.flags);
        profile.WriteInt(_T("WindowShowCmd"), wp.showCmd);
        profile.WriteInt(_T("WindowLeft"), wp.rcNormalPosition.left);
        profile.WriteInt(_T("WindowTop"), wp.rcNormalPosition.top);
        profile.WriteBool(_T("AlwaysVisible"), m_alwaysVisible);
        profile.WriteBool(_T("ToolBar"), m_toolbarVisible);
        profile.WriteBool(_T("StatusBar"), m_statusbarVisible);
        profile.WriteBool(_T("NewInstall"), false);
    }

    GetView()->SaveProfile(profile);
}

void AppFrame::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        CPoint noPos(MeaScreenMgr::Instance().GetOffScreen());

        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(&wp);

        CRect appRect(wp.rcNormalPosition);

        int flags = profile.ReadInt(_T("WindowFlags"), -1);
        if (flags != -1) {
            wp.flags = flags;
        }

        int x = profile.ReadInt(_T("WindowLeft"), noPos.x);
        if (x != noPos.x) {
            wp.rcNormalPosition.left = x;
            wp.rcNormalPosition.right = wp.rcNormalPosition.left + appRect.Width() - 1;
        }

        int y = profile.ReadInt(_T("WindowTop"), noPos.y);
        if (y != noPos.y) {
            wp.rcNormalPosition.top = y;
            wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + appRect.Height() - 1;
        }

        m_profileToolbarVisible = profile.ReadBool(_T("ToolBar"), m_toolbarVisible);
        m_profileStatusbarVisible = profile.ReadBool(_T("StatusBar"), m_statusbarVisible);

        m_newInstall = profile.ReadBool(_T("NewInstall"), m_newInstall);

        m_alwaysVisible = profile.ReadBool(_T("AlwaysVisible"), m_alwaysVisible);

        // Make sure the application is somewhere visible on the screen
        //
        wp.rcNormalPosition = MeaScreenMgr::Instance().EnsureVisible(wp.rcNormalPosition);

        wp.showCmd = SW_HIDE;   // Don't show the application until we are ready
        SetWindowPlacement(&wp);
    }

    GetView()->LoadProfile(profile);
}

LRESULT AppFrame::OnMasterReset(WPARAM, LPARAM) {
    m_alwaysVisible = kDefAlwaysVisible;
    UpdateVisibility();

    ViewToolbar(AppView::kDefToolbarVisible);
    ViewStatusbar(AppView::kDefStatusbarVisible);

    GetView()->MasterReset();

    return TRUE;
}

void AppFrame::InitView() {
    if (!m_profileToolbarVisible) {
        OnViewToolbar();
    }
    if (!m_profileStatusbarVisible) {
        OnViewStatusbar();
    }
}

#ifdef _DEBUG
void AppFrame::AssertValid() const {
    CFrameWnd::AssertValid();
}

void AppFrame::Dump(CDumpContext& dc) const {
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

LRESULT AppFrame::OnCopyData(WPARAM /*wParam*/, LPARAM lParam) {
    COPYDATASTRUCT* cds = reinterpret_cast<COPYDATASTRUCT*>(lParam);

    if (cds->cbData != ID_MEA_COPYDATA) {
        return FALSE;
    }

    CString cmdLineFile(reinterpret_cast<PCTSTR>(cds->lpData));
    m_wndView.LoadStartupFile(cmdLineFile);

    return TRUE;
}

void AppFrame::OnSetFocus(CWnd* /* pOldWnd */) {
    // forward focus to the view window
    m_wndView.SetFocus();
}

BOOL AppFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    // let the view have first crack at the command
    if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }

    // otherwise, do default handling
    return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void AppFrame::OnAlwaysVisible() {
    m_alwaysVisible = !m_alwaysVisible;
    UpdateVisibility();
}

void AppFrame::OnUpdateAlwaysVisible(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_alwaysVisible);
}

void AppFrame::UpdateVisibility() {
    SetWindowPos(m_alwaysVisible ? &wndTopMost : &wndNoTopMost,
        0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

LRESULT AppFrame::OnPrefsApply(WPARAM wParam, LPARAM lParam) {
    GetView()->SendMessage(MeaPrefsApplyMsg, wParam, lParam);
    return 0;
}

LRESULT AppFrame::OnShowCalPrefs(WPARAM wParam, LPARAM lParam) {
    GetView()->PostMessage(MeaShowCalPrefsMsg, wParam, lParam);
    return 0;
}

void AppFrame::OnUpdateHelpMenu(CCmdUI* pCmdUI) {
    pCmdUI->Enable(GetFileAttributes(AfxGetApp()->m_pszHelpFilePath) != 0xffffffff);
}

void AppFrame::OnMeazureReleases() {
    OpenUrl(IDS_MEA_RELEASES_URL);
}

void AppFrame::OnReportIssue() {
    OpenUrl(IDS_MEA_ISSUE_URL);
}

void AppFrame::OnEndSession(BOOL bEnding) {
    if (bEnding) {
        MeaRegistry registry;
        MeaRegistryProfile profile(registry);
        SaveProfile(profile);
    }

    CFrameWnd::OnEndSession(bEnding);
}

void AppFrame::OnClose() {
    MeaRegistry registry;
    MeaRegistryProfile profile(registry);
    SaveProfile(profile);

    CFrameWnd::OnClose();
}

void AppFrame::ViewToolbar(bool enable) {
    if ((m_toolbarVisible && !enable) || (!m_toolbarVisible && enable)) {
        m_toolbarVisible = enable;

        ShowControlBar(&m_wndToolBar, m_toolbarVisible, FALSE);

        CRect rect;
        GetWindowRect(rect);

        int height = rect.Height() + (m_toolbarVisible ? m_toolbarHeight : -m_toolbarHeight);
        MeaLayout::SetWindowSize(*this, m_appWidth, height);
    }
}

void AppFrame::OnViewToolbar() {
    ViewToolbar(!m_toolbarVisible);
}

void AppFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_toolbarVisible);
}

void AppFrame::ViewStatusbar(bool enable) {
    if ((m_statusbarVisible && !enable) || (!m_statusbarVisible && enable)) {
        m_statusbarVisible = enable;

        ShowControlBar(&m_wndStatusBar, m_statusbarVisible, FALSE);

        CRect rect;
        GetWindowRect(rect);

        int height = rect.Height() + (m_statusbarVisible ? m_statusbarHeight : -m_statusbarHeight);
        MeaLayout::SetWindowSize(*this, m_appWidth, height);
    }
}

void AppFrame::OnViewStatusbar() {
    ViewStatusbar(!m_statusbarVisible);
}

void AppFrame::OnUpdateViewStatusbar(CCmdUI* pCmdUI) {
    pCmdUI->SetCheck(m_statusbarVisible);
}

bool AppFrame::OpenUrl(int urlId) {
    CString url;
    url.LoadStringA(urlId);
    HINSTANCE h = ShellExecute(nullptr, _T("open"), url, nullptr, nullptr, SW_SHOWNORMAL);

    if (reinterpret_cast<INT_PTR>(h) <= 32) {
        CString msg;
        msg.Format(IDS_MEA_NOEXEC, static_cast<PCTSTR>(url));
        MessageBox(msg, nullptr, MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}
