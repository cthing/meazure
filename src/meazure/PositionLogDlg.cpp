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

#include "StdAfx.h"
#include "Resource.h"
#include "PositionLogDlg.h"
#include "PositionLogMgr.h"
#include "TimeStamp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(MeaPositionLogDlg, CDialog)
    //{{AFX_MSG_MAP(MeaPositionLogDlg)
    ON_BN_CLICKED(IDC_MEA_ADD_POSITION, OnAddPosition)
    ON_BN_CLICKED(IDC_MEA_DELETE_ALL_POSITIONS, OnDeletePositions)
    ON_BN_CLICKED(IDC_MEA_DELETE_POSITION, OnDeletePosition)
    ON_BN_CLICKED(IDC_MEA_REPLACE_POSITION, OnReplacePosition)
    ON_WM_HSCROLL()
    ON_EN_KILLFOCUS(IDC_MEA_POSITION_DESC, OnKillfocusPositionDesc)
    ON_BN_CLICKED(IDC_MEA_LOAD_POSITIONS, OnLoadPositions)
    ON_BN_CLICKED(IDC_MEA_SAVE_POSITIONS, OnSavePositions)
    ON_BN_CLICKED(IDC_MEA_SAVE_POSITIONS_AS, OnSavePositionsAs)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


MeaPositionLogDlg::MeaPositionLogDlg() : CDialog(), MeaPositionLogObserver()
{
}


MeaPositionLogDlg::~MeaPositionLogDlg()
{
}


BOOL MeaPositionLogDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    MeaPositionLogMgr& mgr = MeaPositionLogMgr::Instance();
    
    mgr.SetObserver(this);

    SetDlgTitle();
    
    SetScrollRange();
    SetScrollPos(0);

    mgr.ShowPosition(0);

    UpdatePositionInfo();
    UpdateEnable();

    return TRUE;
}


void MeaPositionLogDlg::SetDlgTitle()
{
    CString pathname = MeaPositionLogMgr::Instance().GetFilePathname();

    if (pathname.IsEmpty()) {
        pathname.LoadString(IDS_MEA_UNTITLED);
    } else {
        TCHAR filename[_MAX_FNAME];
        TCHAR ext[_MAX_EXT];
        _tsplitpath_s(pathname, NULL, 0, NULL, 0, filename, _MAX_FNAME, ext, _MAX_EXT);
        pathname = filename;
        pathname += ext;
    }

    CString title;
    title.Format(IDS_MEA_POSDLG_TITLE, static_cast<LPCTSTR>(pathname));
    
    SetWindowText(title);
}


BOOL MeaPositionLogDlg::PreTranslateMessage(MSG* pMsg)
{
    HACCEL hAccel = static_cast<CMainFrame*>(AfxGetMainWnd())->GetAccelTable();

    if (!(hAccel && ::TranslateAccelerator(AfxGetApp()->m_pMainWnd->m_hWnd, hAccel, pMsg))) {
        return CDialog::PreTranslateMessage(pMsg);
    } else {
        return TRUE;
    }
}


void MeaPositionLogDlg::OnLoadPositions() 
{
    MeaPositionLogMgr::Instance().Load();
}


void MeaPositionLogDlg::OnSavePositions() 
{
    MeaPositionLogMgr::Instance().Save(false);
}

void MeaPositionLogDlg::OnSavePositionsAs() 
{
    MeaPositionLogMgr::Instance().Save(true);
}


void MeaPositionLogDlg::OnCancel()
{
    DestroyWindow();
}


void MeaPositionLogDlg::PostNcDestroy()
{
    CDialog::PostNcDestroy();
    
    if (!MeaPositionLogMgr::IsDestroyed()) {
        MeaPositionLogMgr::Instance().SetObserver(NULL);
        MeaPositionLogMgr::Instance().ManageDlgDestroyed();
    }

    delete this;
}


void MeaPositionLogDlg::SetScrollPos(int pos) const
{
    static_cast<CScrollBar*>(GetDlgItem(IDC_MEA_POSITION_SCROLLBAR))->SetScrollPos(pos);
}


int MeaPositionLogDlg::GetScrollPos() const
{
    return static_cast<CScrollBar*>(GetDlgItem(IDC_MEA_POSITION_SCROLLBAR))->GetScrollPos();
}


void MeaPositionLogDlg::SetScrollRange() const
{
    MeaPositionLogMgr& mgr = MeaPositionLogMgr::Instance();

    SCROLLINFO info;
    info.cbSize = sizeof(SCROLLINFO);
    info.fMask = SIF_PAGE | SIF_RANGE;
    info.nMin = 0;
    info.nMax = mgr.HavePositions() ? mgr.NumPositions() - 1 : 0;
    info.nPage = 1;

    static_cast<CScrollBar*>(GetDlgItem(IDC_MEA_POSITION_SCROLLBAR))->SetScrollInfo(&info);
}


void MeaPositionLogDlg::LogLoaded()
{
    SetDlgTitle();

    SetScrollRange();
    SetScrollPos(0);
    
    MeaPositionLogMgr::Instance().ShowPosition(0);

    UpdatePositionInfo();
    UpdateEnable();
}


void MeaPositionLogDlg::LogSaved()
{
    SetDlgTitle();
}


void MeaPositionLogDlg::PositionAdded(int /* posIndex */)
{
    SetScrollRange();
    SetScrollPos(MeaPositionLogMgr::Instance().NumPositions() - 1);

    UpdatePositionInfo();
    UpdateEnable();
}


void MeaPositionLogDlg::PositionReplaced(int /* posIndex */)
{
    UpdatePositionInfo();
}


void MeaPositionLogDlg::PositionDeleted(int /* posIndex */)
{
    SetScrollRange();

    UpdatePositionInfo();
    UpdateEnable();
}


void MeaPositionLogDlg::PositionsDeleted()
{
    SetScrollRange();
    
    UpdateEnable();
}


void MeaPositionLogDlg::UpdatePositionInfo(int posIndex) const
{
    CStatic* numField       = static_cast<CStatic*>(GetDlgItem(IDC_MEA_POSITION_NUM));
    CStatic* recordedField  = static_cast<CStatic*>(GetDlgItem(IDC_MEA_POSITION_DATE));
    CEdit* descField        = static_cast<CEdit*>(GetDlgItem(IDC_MEA_POSITION_DESC));

    MeaPositionLogMgr& mgr = MeaPositionLogMgr::Instance();

    int count = mgr.NumPositions();

    if (count > 0) {
        if (posIndex < 0) {
            posIndex = GetScrollPos();
        }

        CString str;
        str.Format(_T("%d of %d"), posIndex + 1, count);
        numField->SetWindowText(str);

        const MeaPositionLogMgr::Position& position = mgr.GetPosition(posIndex);
        
        CTime ts(MeaParseTimeStamp(position.GetTimeStamp()));
        recordedField->SetWindowText(ts.Format(_T("%c")));
        
        descField->SetWindowText(position.GetDesc());
    }
}


void MeaPositionLogDlg::UpdateEnable() const
{
    CWnd* replaceButton     = GetDlgItem(IDC_MEA_REPLACE_POSITION);
    CWnd* deleteButton      = GetDlgItem(IDC_MEA_DELETE_POSITION);
    CWnd* deleteAllButton   = GetDlgItem(IDC_MEA_DELETE_ALL_POSITIONS);
    CWnd* saveButton        = GetDlgItem(IDC_MEA_SAVE_POSITIONS);
    CWnd* saveAsButton      = GetDlgItem(IDC_MEA_SAVE_POSITIONS_AS);

    CWnd* descLabel         = GetDlgItem(IDC_MEA_POSITION_DESC_LBL);
    CWnd* descField         = GetDlgItem(IDC_MEA_POSITION_DESC);

    CWnd* recordedLabel     = GetDlgItem(IDC_MEA_POSITION_DATE_LBL);
    CWnd* recordedField     = GetDlgItem(IDC_MEA_POSITION_DATE);

    CWnd* numLabel          = GetDlgItem(IDC_MEA_POSITION_NUM_LBL);
    CWnd* numField          = GetDlgItem(IDC_MEA_POSITION_NUM);

    CWnd* scrollbar         = GetDlgItem(IDC_MEA_POSITION_SCROLLBAR);

    bool havePositions = MeaPositionLogMgr::Instance().HavePositions();

    replaceButton->EnableWindow(havePositions);
    deleteButton->EnableWindow(havePositions);
    deleteAllButton->EnableWindow(havePositions);
    saveButton->EnableWindow(havePositions);
    saveAsButton->EnableWindow(havePositions);

    descLabel->EnableWindow(havePositions);
    descField->EnableWindow(havePositions);

    recordedLabel->EnableWindow(havePositions);
    recordedField->ShowWindow(havePositions ? SW_SHOW : SW_HIDE);

    numLabel->EnableWindow(havePositions);
    numField->ShowWindow(havePositions ? SW_SHOW : SW_HIDE);

    scrollbar->EnableWindow(havePositions);
}


void MeaPositionLogDlg::OnAddPosition() 
{
    MeaPositionLogMgr::Instance().RecordPosition();
}


void MeaPositionLogDlg::OnReplacePosition() 
{
    MeaPositionLogMgr::Instance().ReplacePosition(GetScrollPos());
}


void MeaPositionLogDlg::OnDeletePosition() 
{
    MeaPositionLogMgr::Instance().DeletePosition(GetScrollPos());   
}


void MeaPositionLogDlg::OnDeletePositions() 
{
    MeaPositionLogMgr::Instance().DeletePositions();
}


void MeaPositionLogDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // Get the minimum and maximum scroll-bar positions.
    //
    int minpos;
    int maxpos;
    pScrollBar->GetScrollRange(&minpos, &maxpos); 
    maxpos = pScrollBar->GetScrollLimit();

    // Get the current position of scroll box.
    //
    int curpos = pScrollBar->GetScrollPos();

    // Determine the new position of scroll box.
    //
    switch (nSBCode)
    {
    case SB_LEFT:           // Scroll to far left.
        curpos = minpos;
        break;
    case SB_RIGHT:          // Scroll to far right.
        curpos = maxpos;
        break;
    case SB_ENDSCROLL:      // End scroll.
        break;
    case SB_LINELEFT:       // Scroll left.
        if (curpos > minpos) {
            curpos--;
        }
        break;
    case SB_LINERIGHT:      // Scroll right.
        if (curpos < maxpos) {
            curpos++;
        }
        break;
    case SB_PAGELEFT:       // Scroll one page left.
        {
            // Get the page size.
            //
            SCROLLINFO info;
            pScrollBar->GetScrollInfo(&info, SIF_PAGE);
   
            if (curpos > minpos) {
                curpos = max(minpos, curpos - static_cast<int>(info.nPage));
            }
        }
        break;
    case SB_PAGERIGHT:      // Scroll one page right.
        {
            // Get the page size.
            //
            SCROLLINFO info;
            pScrollBar->GetScrollInfo(&info, SIF_PAGE);

            if (curpos < maxpos) {
                curpos = min(maxpos, curpos + static_cast<int>(info.nPage));
            }
        }
        break;
    case SB_THUMBPOSITION:  // Scroll to absolute position. nPos is the position
        curpos = nPos;      // of the scroll box at the end of the drag operation.
        break;
    case SB_THUMBTRACK:     // Drag scroll box to specified position. nPos is the
        curpos = nPos;      // position that the scroll box has been dragged to.
        break;
    default:
        break;
    }

    // Set the new position of the thumb (scroll box) and update
    // the position information display.
    //
    SetScrollPos(curpos);
    UpdatePositionInfo(curpos);

    // Set the tool and its position.
    //
    MeaPositionLogMgr::Instance().ShowPosition(curpos);

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void MeaPositionLogDlg::OnKillfocusPositionDesc() 
{
    MeaPositionLogMgr& mgr = MeaPositionLogMgr::Instance();

    if (mgr.HavePositions()) {
        int posIndex = GetScrollPos();

        MeaPositionLogMgr::Position& position = mgr.GetPosition(posIndex);
        CString origStr = position.GetDesc();

        CString newStr;
        CEdit* descField = static_cast<CEdit*>(GetDlgItem(IDC_MEA_POSITION_DESC));
        descField->GetWindowText(newStr);

        if (origStr != newStr) {
            position.SetDesc(newStr);
        }
    }   
}
