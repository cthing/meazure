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

#include "stdafx.h"
#include <htmlhelp.h>
#include "Meazure.h"
#include "MainFrm.h"
#include "Colors.h"
#include "VersionInfo.h"
#include "PositionLogMgr.h"
#include "ProfileMgr.h"
#include "ToolMgr.h"
#include "ScreenMgr.h"
#include "CommandLineInfo.h"
#include "Hooks/Hooks.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Shared global variables. Used by the Mouse Hook
// DLL to route pointer movement messages back to this application.

#pragma data_seg("MeaMainShared")
volatile HWND g_meaMainWnd { nullptr };
#pragma data_seg()

#pragma comment(linker, "/section:MeaMainShared,rws")


// The application class object.

CMeazureApp theApp;


BEGIN_MESSAGE_MAP(CMeazureApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_THREAD_MESSAGE(MEA_MOUSE_MSG, OnMouseHook)
END_MESSAGE_MAP()


CMeazureApp::CMeazureApp() : CWinApp() {}

BOOL CMeazureApp::InitInstance() {
    // Parse the command line to see if we are being started with
    // a profile.
    //
    MeaCommandLineInfo cmdLineInfo;
    ParseCommandLine(cmdLineInfo);

    // Ensure that only one instance of the application is running.
    //
    if (InterlockedExchange64(reinterpret_cast<LONG64*>(const_cast<HWND*>(&g_meaMainWnd)),
                              reinterpret_cast<LONG64>(g_meaMainWnd)) != 0) {

        // Bring the window forward if obscured.

        ::SetForegroundWindow(g_meaMainWnd);

        // Make the window visible if minimized.

        if (IsIconic(g_meaMainWnd)) {
            ShowWindow(g_meaMainWnd, SW_RESTORE);
        }

        // Load the command line file, if any

        if (!cmdLineInfo.m_strFileName.IsEmpty()) {
            COPYDATASTRUCT cds;

            cds.cbData = ID_MEA_COPYDATA;
            cds.dwData = static_cast<DWORD>(cmdLineInfo.m_strFileName.GetLength() + 1) * sizeof(TCHAR);
            cds.lpData = reinterpret_cast<LPVOID>(const_cast<LPTSTR>(static_cast<LPCTSTR>(cmdLineInfo.m_strFileName)));
            ::SendMessage(g_meaMainWnd, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&cds));
        }

        return FALSE;
    }

    // Tell the installer that the program is running so that it cannot
    // be uninstalled out from under it.
    //
    CreateMutex(nullptr, FALSE, "MeazureInstallerMutex");

    // Initialize colors
    //
    MeaColors::Initialize();

    // App appearance
    //
    InitCommonControls();       // To support XP visual styles, if available
    CWinApp::InitInstance();

    // Enable HTML Help
    //
    EnableHtmlHelp();

    // Set the base key for saving/restoring state in the registry
    //
    SetRegistryKey(_T("C Thing Software"));

    // To create the main window, this code creates a new frame window
    // object and then sets it as the application's main window object.
    //
    CMainFrame* pFrame = new CMainFrame;
    m_pMainWnd = pFrame;

    // Tell frame about command line file, if any
    //
    pFrame->SetCommandLineFile(cmdLineInfo.m_strFileName);

    // Create and load the frame with its resources
    //
    pFrame->LoadFrame(IDR_MAINFRAME,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        nullptr, nullptr);

    InterlockedExchange64(reinterpret_cast<LONG64*>(const_cast<HWND*>(&g_meaMainWnd)), reinterpret_cast<LONG64>(pFrame->m_hWnd));

    // Set the help file pathname by replacing the old WinHelp
    // .hlp with the new HTML Help .chm.
    TCHAR* cptr = const_cast<TCHAR*>(_tcsstr(m_pszHelpFilePath, _T(".hlp")));
    if (cptr == nullptr) {
        cptr = const_cast<TCHAR*>(_tcsstr(m_pszHelpFilePath, _T(".HLP")));
    }
    if (cptr != nullptr) {
        _tcscpy_s(cptr, 5, ".chm");
    }

    return TRUE;
}

void CMeazureApp::OnMouseHook(WPARAM wParam, LPARAM lParam) {
    static_cast<CMainFrame*>(m_pMainWnd)->GetView()->OnMouseHook(wParam, lParam);
}

BOOL CMeazureApp::SaveAllModified() {
    return MeaPositionLogMgr::Instance().SaveIfModified();
}

void CMeazureApp::WinHelp(DWORD dwData, UINT nCmd) {
    switch (nCmd) {
    case HELP_CONTEXT:
        TRACE("Context: %s, 0x%X\n", m_pszHelpFilePath, dwData);
        ::HtmlHelp(*AfxGetMainWnd(), m_pszHelpFilePath, HH_HELP_CONTEXT, dwData);
        break;
    case HELP_INDEX:
        TRACE("Index: %s, 0x%X\n", m_pszHelpFilePath, dwData);
        ::HtmlHelp(*AfxGetMainWnd(), m_pszHelpFilePath, HH_DISPLAY_INDEX, dwData);
        break;
    case HELP_COMMAND:      // Using this for Search
    {
        HH_FTS_QUERY q;     // A blank query is required

        q.cbStruct = sizeof(q);    // Sizeof structure in bytes.
        q.fUniCodeStrings = FALSE;        // TRUE if all strings are Unicode.
        q.pszSearchQuery = nullptr;      // String containing the search query.
        q.iProximity = HH_FTS_DEFAULT_PROXIMITY; // Word proximity - only one option
        q.fStemmedSearch = FALSE;        // TRUE for StemmedSearch only.
        q.fTitleOnly = FALSE;        // TRUE for Title search only.
        q.fExecute = FALSE;        // TRUE to initiate the search.
        q.pszWindow = nullptr;      // Window to display in
        TRACE("Search: %s, 0x%X\n", m_pszHelpFilePath, dwData);
        ::HtmlHelp(*AfxGetMainWnd(), m_pszHelpFilePath, HH_DISPLAY_SEARCH, reinterpret_cast<DWORD_PTR>(&q));
    }
    break;
    case HELP_FINDER:
    default:
        TRACE("Contents: %s, 0x%X\n", m_pszHelpFilePath, dwData);
        ::HtmlHelp(*AfxGetMainWnd(), m_pszHelpFilePath, HH_DISPLAY_TOC, dwData);
        break;
    }
}


//*************************************************************************
// AboutDlg
//*************************************************************************


/// Application About dialog.
///
class CAboutDlg : public CDialog {

public:
    /// Constructs the About dialog. To display the dialog, call the
    /// DoModal method.
    ///
    CAboutDlg();

    /// Called when the dialog is about to be displayed.
    ///
    /// @return TRUE to continue the display of the dialog.
    ///
    virtual BOOL OnInitDialog() override;

    enum { IDD = IDD_ABOUTBOX };

protected:
    /// Called to perform Dynamic Data Exchange (DDX) for the dialog.
    /// @param pDX  [in] DDX operation object.
    /// 
    virtual void DoDataExchange(CDataExchange* pDX) override;

    /// Called when the home page URL text link is clicked. Brings up
    /// a browser pointed at the home page.
    /// 
    afx_msg void OnHomeUrl();

    /// Called when the financial contribution button is pressed.
    ///
    afx_msg void OnContribute();

    /// Called when a control on the dialog needs to have its color set.
    /// This method sets the appropriate colors for the links.
    /// @param pDC          [in] DC for the control.
    /// @param pWnd         [in] Window for the control.
    /// @param nCtlColor    [in] Control type.
    /// @return Brush with which to draw the control.
    /// 
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    /// Called to set the cursor on the dialog.
    /// @param pWnd     [in] Window on which to set the cursor.
    /// @param nHitTest [in] What are we testing.
    /// @param message  [in] Message for cursor.
    /// @return 0 to continue processing.
    /// 
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    DECLARE_MESSAGE_MAP()

private:
    /// Opens the specified URL in a web browser.
    /// 
    /// @param urlId  String resource ID for the URL 
    /// @return true if the browser was opened.
    bool OpenUrl(int urlId);

    static bool m_visited;              ///< Indicates if the home page has been visited.
    static bool m_emailed;              ///< Indicates if the email link has been visited.
    static COLORREF m_unvisitedColor;   ///< Link color before it has been visited.
    static COLORREF m_visitedColor;     ///< Link color after it has been visited.

    CFont m_titleFont;                  ///< Font used for the dialog title.
    CFont m_linkFont;                   ///< Font used for the link text.
    HCURSOR m_linkCursor;               ///< Hand cursor to indicate a link.
};


bool CAboutDlg::m_visited { false };
bool CAboutDlg::m_emailed { false };
COLORREF CAboutDlg::m_unvisitedColor { RGB(0, 0, 255) }; // Blue
COLORREF CAboutDlg::m_visitedColor { RGB(128, 0, 128) }; // Purple


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    ON_BN_CLICKED(IDC_HOME_URL, OnHomeUrl)
    ON_BN_CLICKED(IDC_CONTRIBUTE, OnContribute)
    ON_WM_CTLCOLOR()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD), m_linkCursor(nullptr) {}

BOOL CAboutDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    //
    // Set title font and append the version number
    //
    LOGFONT lf;
    CString str;

    CStatic* versionStr = static_cast<CStatic*>(GetDlgItem(IDC_PROGRAM_VERSION));
    CStatic* buildStr = static_cast<CStatic*>(GetDlgItem(IDC_PROGRAM_BUILD));

    versionStr->GetWindowText(str);
    str += g_versionInfo.GetProductVersion();
    versionStr->SetWindowText(str);

    CString bstr;
    buildStr->GetWindowText(str);
    bstr.Format(_T("%d"), g_versionInfo.GetProductBuild());
    str += bstr;
    buildStr->SetWindowText(str);

    //
    // Set the links font
    //
    CStatic* url = static_cast<CStatic*>(GetDlgItem(IDC_HOME_URL));
    CFont* font = url->GetFont();
    font->GetLogFont(&lf);
    lf.lfUnderline = TRUE;
    m_linkFont.CreateFontIndirect(&lf);
    url->SetFont(&m_linkFont);

    m_linkCursor = AfxGetApp()->LoadCursor(IDC_HAND_CUR);

    return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    if (pWnd->GetDlgCtrlID() == IDC_HOME_URL) {
        pDC->SetTextColor(m_visited ? m_visitedColor : m_unvisitedColor);
        pDC->SetBkMode(TRANSPARENT);
    }

    return hbr;
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
    CPoint point;
    CRect  rect;

    //
    // Test if cursor is over control
    //
    if (nHitTest == HTCLIENT) {
        GetCursorPos(&point);
        GetDlgItem(IDC_HOME_URL)->GetWindowRect(&rect);
        if (rect.PtInRect(point)) {
            ::SetCursor(m_linkCursor);
            return(TRUE);
        }
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CAboutDlg::OnHomeUrl() {
    if (OpenUrl(IDS_MEA_CTHING_URL)) {
        m_visited = true;

        Invalidate();
        UpdateWindow();
    }
}

void CAboutDlg::OnContribute() {
    OpenUrl(IDS_MEA_CONTRIBUTE_URL);
}

bool CAboutDlg::OpenUrl(int urlId) {
    CString url;
    url.LoadString(urlId);
    HINSTANCE h = ShellExecute(nullptr, _T("open"), url, nullptr, nullptr, SW_SHOWNORMAL);

    if (reinterpret_cast<INT_PTR>(h) <= 32) {
        CString msg;
        msg.Format(IDS_MEA_NOEXEC, static_cast<LPCTSTR>(url));
        MessageBox(msg, nullptr, MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

void CMeazureApp::OnAppAbout() {
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}
