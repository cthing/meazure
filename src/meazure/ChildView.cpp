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
#include "Resource.h"
#include "MeaAssert.h"
#include "Layout.h"
#include "ChildView.h"
#include "Colors.h"
#include "ColorDialog.h"
#include "FileProfile.h"
#include "DataWin.h"
#include "ScreenTool.h"
#include "PositionLogMgr.h"
#include "ProfileMgr.h"
#include "ToolMgr.h"
#include "ScreenMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CHAR_1 49
#define CHAR_2 50
#define CHAR_3 51


BEGIN_MESSAGE_MAP(CChildView,CWnd )
    ON_COMMAND_RANGE(ID_MEA_UNITS_PIXELS, ID_MEA_UNITS_PICAS, OnUnits)
    ON_COMMAND_RANGE(ID_MEA_DEGREES, ID_MEA_RADIANS, OnAngles)
    ON_COMMAND_RANGE(ID_MEA_CURSOR, ID_MEA_WINDOW, OnRadioTool)
    ON_COMMAND_RANGE(ID_MEA_RGBFMT, ID_MEA_YIQFMT, OnColorFmt)
    ON_MESSAGE(MeaDataChangeMsg, OnDataChange)
    ON_MESSAGE(MeaPrefsApplyMsg, OnPrefsApply)
    ON_MESSAGE(MeaShowCalPrefsMsg, OnShowCalPrefs)
    ON_MESSAGE(MeaGetPositionMsg, OnGetPosition)
    ON_MESSAGE(MeaHPTimerMsg, OnHPTimer)
    //{{AFX_MSG_MAP(CChildView)
    ON_WM_CREATE()
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_CM, OnUpdateUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_DEGREES, OnUpdateAngles)
    ON_UPDATE_COMMAND_UI(ID_MEA_CURSOR, OnUpdateRadioTool)
    ON_WM_DESTROY()
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
    ON_COMMAND(ID_MEA_INVERTY, OnInvertY)
    ON_UPDATE_COMMAND_UI(ID_MEA_INVERTY, OnUpdateInvertY)
    ON_COMMAND(ID_MEA_ORIGIN, OnOrigin)
    ON_COMMAND(ID_MEA_RESET_ORIGIN, OnResetOrigin)
    ON_COMMAND(ID_MEA_RULER, OnRuler)
    ON_UPDATE_COMMAND_UI(ID_MEA_RULER, OnUpdateRuler)
    ON_COMMAND(ID_MEA_PREFERENCES, OnPreferences)
    ON_COMMAND(ID_MEA_MAGNIFIER, OnMagnifier)
    ON_UPDATE_COMMAND_UI(ID_MEA_MAGNIFIER, OnUpdateMagnifier)
    ON_COMMAND(ID_MEA_MAGGRID, OnShowMagGrid)
    ON_UPDATE_COMMAND_UI(ID_MEA_MAGGRID, OnUpdateShowMagGrid)
    ON_COMMAND(ID_MEA_SCREEN_INFO, OnScreenInfo)
    ON_UPDATE_COMMAND_UI(ID_MEA_SCREEN_INFO, OnUpdateScreenInfo)
    ON_UPDATE_COMMAND_UI(ID_MEA_RGBFMT, OnUpdateColorFmt)
    ON_COMMAND(ID_MEA_RUNSTATE, OnRunState)
    ON_UPDATE_COMMAND_UI(ID_MEA_RUNSTATE, OnUpdateRunState)
    ON_COMMAND(ID_MEA_SCREEN_GRID, OnScreenGrid)
    ON_UPDATE_COMMAND_UI(ID_MEA_SCREEN_GRID, OnUpdateScreenGrid)
    ON_COMMAND(ID_MEA_SCREEN_GRID_SPACING, OnScreenGridSpacing)
    ON_COMMAND(ID_MEA_SAVE_PROFILE, OnSaveProfile)
    ON_COMMAND(ID_MEA_LOAD_PROFILE, OnLoadProfile)
    ON_COMMAND(ID_MEA_COPY_RGN, OnCopyRegion)
    ON_UPDATE_COMMAND_UI(ID_MEA_COPY_RGN, OnUpdateCopyRegion)
    ON_COMMAND(ID_MEA_TOOL_INFO, OnToolInfo)
    ON_UPDATE_COMMAND_UI(ID_MEA_TOOL_INFO, OnUpdateToolInfo)
    ON_COMMAND(ID_MEA_COLLAPSE, OnCollapse)
    ON_UPDATE_COMMAND_UI(ID_MEA_COLLAPSE, OnUpdateCollapse)
    ON_COMMAND(ID_MEA_FIND, OnFindCrosshair)
    ON_UPDATE_COMMAND_UI(ID_MEA_FIND, OnUpdateFindCrosshair)
    ON_COMMAND(ID_MEA_RECORD, OnRecordPosition)
    ON_COMMAND(ID_MEA_DELETE_POSITIONS, OnDeletePositions)
    ON_UPDATE_COMMAND_UI(ID_MEA_DELETE_POSITIONS, OnUpdateDeletePositions)
    ON_COMMAND(ID_MEA_LOAD_POSITIONS, OnLoadPositions)
    ON_COMMAND(ID_MEA_SAVE_POSITIONS, OnSavePositions)
    ON_COMMAND(ID_MEA_SAVE_POSITIONS_AS, OnSavePositionsAs)
    ON_UPDATE_COMMAND_UI(ID_MEA_SAVE_POSITIONS, OnUpdateSavePositions)
    ON_COMMAND(ID_MEA_MANAGE_POSITIONS, OnManagePositions)
    ON_COMMAND(ID_MEA_ZOOM_IN, OnZoomIn)
    ON_UPDATE_COMMAND_UI(ID_MEA_ZOOM_IN, OnUpdateZoomIn)
    ON_COMMAND(ID_MEA_ZOOM_OUT, OnZoomOut)
    ON_UPDATE_COMMAND_UI(ID_MEA_ZOOM_OUT, OnUpdateZoomOut)
    ON_COMMAND(ID_MEA_HIDE_CROSSHAIR, OnHideCrosshairs)
    ON_UPDATE_COMMAND_UI(ID_MEA_HIDE_CROSSHAIR, OnUpdateHideCrosshairs)
    ON_COMMAND(ID_MEA_UNITS_DEF_CUSTOM, OnDefineCustomUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_CUSTOM, OnUpdateCustomUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_RADIANS, OnUpdateAngles)
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_INCHES, OnUpdateUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_MM, OnUpdateUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_PIXELS, OnUpdateUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_POINTS, OnUpdateUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_TWIPS, OnUpdateUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_LINE, OnUpdateRadioTool)
    ON_UPDATE_COMMAND_UI(ID_MEA_POINT, OnUpdateRadioTool)
    ON_UPDATE_COMMAND_UI(ID_MEA_RECT, OnUpdateRadioTool)
    ON_UPDATE_COMMAND_UI(ID_MEA_CIRCLE, OnUpdateRadioTool)
    ON_UPDATE_COMMAND_UI(ID_MEA_ANGLE, OnUpdateRadioTool)
    ON_UPDATE_COMMAND_UI(ID_MEA_WINDOW, OnUpdateRadioTool)
    ON_UPDATE_COMMAND_UI(ID_MEA_RGBHEXFMT, OnUpdateColorFmt)
    ON_UPDATE_COMMAND_UI(ID_MEA_CMYFMT, OnUpdateColorFmt)
    ON_UPDATE_COMMAND_UI(ID_MEA_CMYKFMT, OnUpdateColorFmt)
    ON_UPDATE_COMMAND_UI(ID_MEA_HSLFMT, OnUpdateColorFmt)
    ON_UPDATE_COMMAND_UI(ID_MEA_YCBCRFMT, OnUpdateColorFmt)
    ON_UPDATE_COMMAND_UI(ID_MEA_YIQFMT, OnUpdateColorFmt)
    ON_UPDATE_COMMAND_UI(ID_MEA_UNITS_PICAS, OnUpdateUnits)
    ON_UPDATE_COMMAND_UI(ID_MEA_SAVE_POSITIONS_AS, OnUpdateSavePositions)
    ON_COMMAND(ID_MEA_UNITS_CUSTOM, OnCustomUnits)
    ON_COMMAND(ID_MEA_POS1, OnSetPosition1ToCursor)
    ON_COMMAND(ID_MEA_POS2, OnSetPosition2ToCursor)
    ON_COMMAND(ID_MEA_POS3, OnSetPosition3ToCursor)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Defaults
const bool CChildView::kDefToolbarVisible = true;
const bool CChildView::kDefStatusbarVisible = true;
const bool CChildView::kDefToolInfoVisible = true;
const bool CChildView::kDefScreenInfoVisible = true;
const bool CChildView::kDefMagnifierVisible = true;


CChildView::CChildView() : CWnd(),
    m_margin(5,5),
    m_enabled(true),
    m_profileMagnifierEnabled(kDefMagnifierVisible),
    m_profileScreenEnabled(kDefScreenInfoVisible),
    m_profileRegionEnabled(kDefToolInfoVisible),
    m_prefs(IDS_MEA_PREFERENCES),
    m_adjustHeight(0),
    m_expandToolbar(kDefToolbarVisible),
    m_expandStatusbar(kDefStatusbarVisible),
    m_expandMagnifier(kDefMagnifierVisible),
    m_expandScreenInfo(kDefScreenInfoVisible),
    m_expandToolInfo(kDefToolInfoVisible)
{
    MeaToolMgr::Instance().SetDataDisplay(&m_dataDisplay);
}


CChildView::~CChildView()
{
    try {
        MeaToolMgr::Instance().SetDataDisplay(NULL);
    }
    catch(...) {
        MeaAssert(false);
    }
}


void CChildView::SaveProfile(MeaProfile& profile)
{
    MeaScreenMgr::Instance().SaveProfile(profile);
    MeaUnitsMgr::Instance().SaveProfile(profile);
    MeaColors::SaveProfile(profile);
    MeaColorDialog::SaveProfile(profile);
    MeaDataWin::SaveProfile(profile);
    MeaProfileMgr::Instance().SaveProfile(profile);
    MeaPositionLogMgr::Instance().SaveProfile(profile);
    m_magnifier.SaveProfile(profile);

    if (!profile.UserInitiated()) {
        profile.WriteBool(_T("RegionInfo"), m_dataDisplay.IsEnabled(MeaRegionSection));
        profile.WriteBool(_T("ScreenInfo"), m_dataDisplay.IsEnabled(MeaScreenSection));

        profile.WriteBool(_T("Magnifier"), m_magnifier.IsEnabled());

        profile.WriteBool(_T("ExpandToolbar"),      m_expandToolbar);
        profile.WriteBool(_T("ExpandStatusbar"),    m_expandStatusbar);
        profile.WriteBool(_T("ExpandMagnifier"),    m_expandMagnifier);
        profile.WriteBool(_T("ExpandScreenInfo"),   m_expandScreenInfo);
        profile.WriteBool(_T("ExpandToolInfo"),     m_expandToolInfo);
    
        profile.WriteStr(_T("StartupProfile"), m_startupProfile);
    }

    MeaToolMgr::Instance().SaveProfile(profile);
}


void CChildView::LoadProfile(MeaProfile& profile)
{
    MeaToolMgr& toolMgr = MeaToolMgr::Instance();

    MeaScreenMgr::Instance().LoadProfile(profile);
    MeaUnitsMgr::Instance().LoadProfile(profile);

    MeaColorDialog::LoadProfile(profile);
    MeaColors::LoadProfile(profile);
    
    MeaDataWin::LoadProfile(profile);
    
    MeaProfileMgr::Instance().LoadProfile(profile);
    MeaPositionLogMgr::Instance().LoadProfile(profile);
    
    m_magnifier.LoadProfile(profile);

    if (!profile.UserInitiated()) {
        toolMgr.ColorsChanged();

        m_profileRegionEnabled = profile.ReadBool(_T("RegionInfo"), m_dataDisplay.IsEnabled(MeaRegionSection));
        m_profileScreenEnabled = profile.ReadBool(_T("ScreenInfo"), m_dataDisplay.IsEnabled(MeaScreenSection));
        m_profileMagnifierEnabled = profile.ReadBool(_T("Magnifier"), m_magnifier.IsEnabled());

        m_expandToolbar     = profile.ReadBool(_T("ExpandToolbar"),     m_expandToolbar);
        m_expandStatusbar   = profile.ReadBool(_T("ExpandStatusbar"),   m_expandStatusbar);
        m_expandMagnifier   = profile.ReadBool(_T("ExpandMagnifier"),   m_expandMagnifier);
        m_expandScreenInfo  = profile.ReadBool(_T("ExpandScreenInfo"),  m_expandScreenInfo);
        m_expandToolInfo    = profile.ReadBool(_T("ExpandToolInfo"),    m_expandToolInfo);

        m_startupProfile    = profile.ReadStr(_T("StartupProfile"), m_startupProfile);
    }

    toolMgr.DisableRadioTools();
    toolMgr.LoadProfile(profile);
    toolMgr.ChangeRadioToolEnable();
    toolMgr.UpdateTools();
}


void CChildView::MasterReset()
{
    m_expandToolbar = kDefToolbarVisible;
    m_expandStatusbar = kDefStatusbarVisible;

    m_expandToolInfo = kDefToolInfoVisible;
    ViewToolInfo(m_expandToolInfo);

    m_expandScreenInfo = kDefScreenInfoVisible;
    ViewScreenInfo(m_expandScreenInfo);

    m_expandMagnifier = kDefMagnifierVisible;
    ViewMagnifier(m_expandMagnifier);

    m_startupProfile.Empty();

    MeaScreenMgr::Instance().MasterReset();
    MeaUnitsMgr::Instance().MasterReset();

    MeaColorDialog::MasterReset();
    MeaColors::MasterReset();

    MeaDataWin::MasterReset();

    MeaProfileMgr::Instance().MasterReset();
    MeaPositionLogMgr::Instance().MasterReset();

    m_magnifier.MasterReset();
    
    MeaToolMgr& toolMgr = MeaToolMgr::Instance();
    toolMgr.ColorsChanged();
    toolMgr.DisableRadioTools();
    toolMgr.MasterReset();
    toolMgr.ChangeRadioToolEnable();
    toolMgr.UpdateTools(AllChanged);
}


void CChildView::LoadStartupFile(const CString& cmdLineFile) const
{
    // Load any profile files.
    //
    if (!cmdLineFile.IsEmpty() && MeaProfileMgr::IsProfileFile(cmdLineFile))
        MeaProfileMgr::Instance().Load(cmdLineFile);
    else if (!m_startupProfile.IsEmpty())
        MeaProfileMgr::Instance().Load(m_startupProfile);

    // Load the position log file, if any.
    //
    if (!cmdLineFile.IsEmpty() && MeaPositionLogMgr::IsPositionFile(cmdLineFile)) {
        if (MeaPositionLogMgr::Instance().Load(cmdLineFile)) {
            MeaPositionLogMgr::Instance().ManagePositions();
        }
    }
}


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
    if (!CWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
        ::LoadCursor(NULL, IDC_ARROW), GetSysColorBrush(COLOR_BTNFACE), NULL);

    return TRUE;
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    if (!m_dataDisplay.Create(CPoint(m_margin.cx, m_margin.cy), this)) {
        return -1;
    }

    CRect rect, magRect, frameRect;

    m_dataDisplay.GetWindowRect(&rect);

    if (!m_magnifier.Create(CPoint(m_margin.cx,
            rect.Height() + 2 * m_margin.cy), rect.Width(), this)) {
        return -1;
    }
    m_magnifier.GetWindowRect(magRect);

    rect.UnionRect(rect, magRect);

    m_adjustHeight = rect.Height();

    DWORD style = GetStyle();
    DWORD exstyle = GetExStyle();
    ::AdjustWindowRectEx(&rect, style, FALSE, exstyle);

    m_adjustHeight = rect.Height() - m_adjustHeight;

    CFrameWnd *frame = GetParentFrame();
    
    style = frame->GetStyle();
    exstyle = frame->GetExStyle();
    ::AdjustWindowRectEx(&rect, style, TRUE, exstyle);

    frame->GetWindowRect(&frameRect);
    frameRect.right = frameRect.left + rect.Width() + 2 * m_margin.cx;
    frameRect.bottom = frameRect.top + rect.Height() + 2 * m_margin.cy;

    int deltaWidth = GetMenuWidth() - frameRect.Width();

    if (deltaWidth > 0) {
        frameRect.right += deltaWidth;

        m_margin.cx += deltaWidth / 2;

        MeaLayout::SetWindowPos(m_dataDisplay, m_margin.cx, m_margin.cy);
        ScreenToClient(magRect);
        MeaLayout::SetWindowPos(m_magnifier, m_margin.cx, magRect.top);
    }

    frame->MoveWindow(frameRect);

    m_snapshotTimer.Create(this);

    MeaToolMgr& toolMgr = MeaToolMgr::Instance();

    // Display screen information
    //
    toolMgr.EnableTool(MeaScreenTool::kToolName);

    // Enable the currently selected radio tool
    //
    toolMgr.ChangeRadioToolEnable();
    
    return 0;
}


int CChildView::GetMenuWidth()
{
    int width = 0;
    CMenu *menu = GetParentFrame()->GetMenu();
    int numItems = menu->GetMenuItemCount();

    for (int i = 0; i < numItems; i++) {
        CRect rect;

        GetMenuItemRect(*this, *menu, i, rect);
        width += rect.Width() + GetSystemMetrics(SM_CXFOCUSBORDER);
    }

    return width + GetSystemMetrics(SM_CXVSCROLL) + 1;
}


void CChildView::InitView()
{
    if (!m_profileRegionEnabled) {
        OnToolInfo();
    }

    if (!m_profileScreenEnabled) {
        OnScreenInfo();
    }

    if (!m_profileMagnifierEnabled) {
        OnMagnifier();
    }
}


void CChildView::CheckMargins()
{
    bool sectionsEnabled = m_dataDisplay.IsEnabled(MeaScreenSection) ||
                           m_dataDisplay.IsEnabled(MeaRegionSection) ||
                           m_magnifier.IsEnabled();
    CFrameWnd *frame = GetParentFrame();
    int frameHeight = MeaLayout::GetWindowHeight(*frame);
    int marginHeight = 2 * m_margin.cy + m_adjustHeight;
    
    if (sectionsEnabled) {
        if (!m_enabled) {
            MeaLayout::SetWindowHeight(*frame, frameHeight + marginHeight);
            m_enabled = true;
        }
    } else {
        if (m_enabled) {
            MeaLayout::SetWindowHeight(*frame, frameHeight - marginHeight);
            m_enabled = false;
        }
    }
}


void CChildView::OnSaveProfile() 
{
    MeaProfileMgr::Instance().Save();
}


void CChildView::OnLoadProfile() 
{
    MeaProfileMgr::Instance().Load();
}


BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
    bool pressed = false;

    if (pMsg->message == WM_KEYDOWN) {
        auto incPosition = [&pressed](int keyCode, MeaFields which) {
            if (MeaUtils::IsKeyPressed(keyCode)) {
                MeaToolMgr::Instance().IncPosition(which);
                pressed = true;
            }
        };

        auto decPosition = [&pressed](int keyCode, MeaFields which) {
            if (MeaUtils::IsKeyPressed(keyCode)) {
                MeaToolMgr::Instance().DecPosition(which);
                pressed = true;
            }
        };

        switch (pMsg->wParam) {
        case VK_DOWN:
            if (MeaUtils::IsKeyPressed(VK_CONTROL)) {
                incPosition(CHAR_1, MeaY1Field);
                incPosition(CHAR_2, MeaY2Field);
                incPosition(CHAR_3, MeaYVField);
            }
            break;
        case VK_UP:
            if (MeaUtils::IsKeyPressed(VK_CONTROL)) {
                decPosition(CHAR_1, MeaY1Field);
                decPosition(CHAR_2, MeaY2Field);
                decPosition(CHAR_3, MeaYVField);
            }
            break;
        case VK_LEFT:
            if (MeaUtils::IsKeyPressed(VK_CONTROL)) {
                decPosition(CHAR_1, MeaX1Field);
                decPosition(CHAR_2, MeaX2Field);
                decPosition(CHAR_3, MeaXVField);
            }
            break;
        case VK_RIGHT:
            if (MeaUtils::IsKeyPressed(VK_CONTROL)) {
                incPosition(CHAR_1, MeaX1Field);
                incPosition(CHAR_2, MeaX2Field);
                incPosition(CHAR_3, MeaXVField);
            }
            break;
        default:
            break;
        }
    }

    return pressed ? TRUE : CWnd::PreTranslateMessage(pMsg);
}


void CChildView::OnMouseHook(WPARAM wParam, LPARAM lParam)
{
    MeaToolMgr::Instance().OnMouseHook(wParam, lParam);
}


LRESULT CChildView::OnGetPosition(WPARAM /* wParam */, LPARAM lParam)
{
    POINT *curPos = reinterpret_cast<POINT*>(lParam);

    *curPos = MeaToolMgr::Instance().GetPosition();
    return TRUE;
}


void CChildView::OnUpdateUnits(CCmdUI* pCmdUI)
{
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    pCmdUI->Enable();
    
    switch (pCmdUI->m_nID) {
    case ID_MEA_UNITS_PIXELS:
        pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaPixelsId);
        break;
    case ID_MEA_UNITS_TWIPS:
        pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaTwipsId);
        break;
    case ID_MEA_UNITS_INCHES:
        pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaInchesId);
        break;
    case ID_MEA_UNITS_CM:
        pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaCentimetersId);
        break;
    case ID_MEA_UNITS_MM:
        pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaMillimetersId);
        break;
    case ID_MEA_UNITS_POINTS:
        pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaPointsId);
        break;
    case ID_MEA_UNITS_PICAS:
        pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaPicasId);
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void CChildView::OnUnits(UINT nID) 
{
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    switch (nID) {
    case ID_MEA_UNITS_PIXELS:
        unitsMgr.SetLinearUnits(MeaPixelsId);
        break;
    case ID_MEA_UNITS_TWIPS:
        unitsMgr.SetLinearUnits(MeaTwipsId);
        break;
    case ID_MEA_UNITS_INCHES:
        unitsMgr.SetLinearUnits(MeaInchesId);
        break;
    case ID_MEA_UNITS_CM:
        unitsMgr.SetLinearUnits(MeaCentimetersId);
        break;
    case ID_MEA_UNITS_MM:
        unitsMgr.SetLinearUnits(MeaMillimetersId);
        break;
    case ID_MEA_UNITS_POINTS:
        unitsMgr.SetLinearUnits(MeaPointsId);
        break;
    case ID_MEA_UNITS_PICAS:
        unitsMgr.SetLinearUnits(MeaPicasId);
        break;
    default:
        MeaAssert(false);
        break;
    }

    MeaToolMgr::Instance().UpdateTools(UnitsChanged);
}


void CChildView::OnUpdateAngles(CCmdUI* pCmdUI)
{
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    pCmdUI->Enable();
    
    switch (pCmdUI->m_nID) {
    case ID_MEA_DEGREES:
        pCmdUI->SetRadio(unitsMgr.GetAngularUnitsId() == MeaDegreesId);
        break;
    case ID_MEA_RADIANS:
        pCmdUI->SetRadio(unitsMgr.GetAngularUnitsId() == MeaRadiansId);
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void CChildView::OnAngles(UINT nID) 
{
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    switch (nID) {
    case ID_MEA_DEGREES:
        unitsMgr.SetAngularUnits(MeaDegreesId);
        break;
    case ID_MEA_RADIANS:
        unitsMgr.SetAngularUnits(MeaRadiansId);
        break;
    default:
        MeaAssert(false);
        break;
    }

    MeaToolMgr::Instance().UpdateTools(UnitsChanged);
}


void CChildView::OnUpdateCustomUnits(CCmdUI* pCmdUI) 
{
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();
    MeaCustomUnits* customUnits = unitsMgr.GetCustomUnits();

    pCmdUI->SetRadio(unitsMgr.GetLinearUnitsId() == MeaCustomId);
    
    if (customUnits->HaveCustomUnits()) {
        pCmdUI->SetText(customUnits->GetName());
        pCmdUI->Enable(TRUE);
    } else {
        CString label(reinterpret_cast<LPCSTR>(IDS_MEA_CUSTOM_MENU));
        pCmdUI->SetText(label);
        pCmdUI->Enable(FALSE);
    }
}


void CChildView::OnCustomUnits() 
{
    MeaUnitsMgr::Instance().SetLinearUnits(MeaCustomId);
    MeaToolMgr::Instance().UpdateTools(UnitsChanged);
}


void CChildView::OnDefineCustomUnits() 
{
    m_prefs.SetActivePage(&m_prefs.m_customPrefs);
    OnPreferences();
}


void CChildView::OnSetPosition1ToCursor()
{
    MeaToolMgr::Instance().SetPositionToCursor(MeaX1Field, MeaY1Field);
}


void CChildView::OnSetPosition2ToCursor()
{
    MeaToolMgr::Instance().SetPositionToCursor(MeaX2Field, MeaY2Field);
}


void CChildView::OnSetPosition3ToCursor()
{
    MeaToolMgr::Instance().SetPositionToCursor(MeaXVField, MeaYVField);
}


void CChildView::OnUpdateColorFmt(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable();
    
    switch (pCmdUI->m_nID) {
    case ID_MEA_RGBFMT:
        pCmdUI->SetRadio(m_magnifier.GetColorFmt() == MeaMagnifier::RGBFmt);
        break;
    case ID_MEA_RGBHEXFMT:
        pCmdUI->SetRadio(m_magnifier.GetColorFmt() == MeaMagnifier::RGBHexFmt);
        break;
    case ID_MEA_CMYFMT:
        pCmdUI->SetRadio(m_magnifier.GetColorFmt() == MeaMagnifier::CMYFmt);
        break;
    case ID_MEA_CMYKFMT:
        pCmdUI->SetRadio(m_magnifier.GetColorFmt() == MeaMagnifier::CMYKFmt);
        break;
    case ID_MEA_HSLFMT:
        pCmdUI->SetRadio(m_magnifier.GetColorFmt() == MeaMagnifier::HSLFmt);
        break;
    case ID_MEA_YCBCRFMT:
        pCmdUI->SetRadio(m_magnifier.GetColorFmt() == MeaMagnifier::YCbCrFmt);
        break;
    case ID_MEA_YIQFMT:
        pCmdUI->SetRadio(m_magnifier.GetColorFmt() == MeaMagnifier::YIQFmt);
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void CChildView::OnColorFmt(UINT nID)
{
    switch (nID) {
    case ID_MEA_RGBFMT:
        m_magnifier.SetColorFmt(MeaMagnifier::RGBFmt);
        break;
    case ID_MEA_RGBHEXFMT:
        m_magnifier.SetColorFmt(MeaMagnifier::RGBHexFmt);
        break;
    case ID_MEA_CMYFMT:
        m_magnifier.SetColorFmt(MeaMagnifier::CMYFmt);
        break;
    case ID_MEA_CMYKFMT:
        m_magnifier.SetColorFmt(MeaMagnifier::CMYKFmt);
        break;
    case ID_MEA_HSLFMT:
        m_magnifier.SetColorFmt(MeaMagnifier::HSLFmt);
        break;
    case ID_MEA_YCBCRFMT:
        m_magnifier.SetColorFmt(MeaMagnifier::YCbCrFmt);
        break;
    case ID_MEA_YIQFMT:
        m_magnifier.SetColorFmt(MeaMagnifier::YIQFmt);
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void CChildView::OnInvertY() 
{
    MeaUnitsMgr::Instance().SetInvertY(!MeaUnitsMgr::Instance().GetInvertY());
    MeaToolMgr::Instance().UpdateTools();
}


void CChildView::OnUpdateInvertY(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(MeaUnitsMgr::Instance().GetInvertY());
}


void CChildView::OnOrigin() 
{
    MeaUnitsMgr::Instance().SetOrigin(MeaToolMgr::Instance().GetPosition());
    MeaToolMgr::Instance().UpdateTools(OriginChanged);
}


void CChildView::OnResetOrigin() 
{
    CPoint point(0, 0);
    MeaUnitsMgr::Instance().SetOrigin(point);
    MeaToolMgr::Instance().UpdateTools();
}


void CChildView::OnRecordPosition() 
{
    MeaPositionLogMgr::Instance().RecordPosition();
}


void CChildView::OnManagePositions() 
{
    MeaPositionLogMgr::Instance().ManagePositions();
}


void CChildView::OnUpdateDeletePositions(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(MeaPositionLogMgr::Instance().HavePositions());
}


void CChildView::OnDeletePositions() 
{
    MeaPositionLogMgr::Instance().DeletePositions();
}


void CChildView::OnLoadPositions() 
{
    if (MeaPositionLogMgr::Instance().Load()) {
        MeaPositionLogMgr::Instance().ManagePositions();
    }
}


void CChildView::OnSavePositions() 
{
    MeaPositionLogMgr::Instance().Save(false);
}


void CChildView::OnSavePositionsAs() 
{
    MeaPositionLogMgr::Instance().Save(true);
}


void CChildView::OnUpdateSavePositions(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(MeaPositionLogMgr::Instance().HavePositions());
}


void CChildView::OnUpdateRadioTool(CCmdUI* pCmdUI) 
{
    switch (pCmdUI->m_nID) {
    case ID_MEA_CURSOR:
        pCmdUI->SetRadio(MeaToolMgr::Instance().GetToolName() == MeaCursorTool::kToolName);
        break;
    case ID_MEA_POINT:
        pCmdUI->SetRadio(MeaToolMgr::Instance().GetToolName() == MeaPointTool::kToolName);
        break;
    case ID_MEA_LINE:
        pCmdUI->SetRadio(MeaToolMgr::Instance().GetToolName() == MeaLineTool::kToolName);
        break;
    case ID_MEA_RECT:
        pCmdUI->SetRadio(MeaToolMgr::Instance().GetToolName() == MeaRectTool::kToolName);
        break;
    case ID_MEA_CIRCLE:
        pCmdUI->SetRadio(MeaToolMgr::Instance().GetToolName() == MeaCircleTool::kToolName);
        break;
    case ID_MEA_ANGLE:
        pCmdUI->SetRadio(MeaToolMgr::Instance().GetToolName() == MeaAngleTool::kToolName);
        break;
    case ID_MEA_WINDOW:
        pCmdUI->SetRadio(MeaToolMgr::Instance().GetToolName() == MeaWindowTool::kToolName);
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void CChildView::OnRadioTool(UINT nID) 
{
    switch (nID) {
    case ID_MEA_CURSOR:
        MeaToolMgr::Instance().SetRadioTool(MeaCursorTool::kToolName);
        break;
    case ID_MEA_POINT:
        MeaToolMgr::Instance().SetRadioTool(MeaPointTool::kToolName);
        break;
    case ID_MEA_LINE:
        MeaToolMgr::Instance().SetRadioTool(MeaLineTool::kToolName);
        break;
    case ID_MEA_RECT:
        MeaToolMgr::Instance().SetRadioTool(MeaRectTool::kToolName);
        break;
    case ID_MEA_CIRCLE:
        MeaToolMgr::Instance().SetRadioTool(MeaCircleTool::kToolName);
        break;
    case ID_MEA_ANGLE:
        MeaToolMgr::Instance().SetRadioTool(MeaAngleTool::kToolName);
        break;
    case ID_MEA_WINDOW:
        MeaToolMgr::Instance().SetRadioTool(MeaWindowTool::kToolName);
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void CChildView::OnRuler() 
{
    MeaToolMgr::Instance().ToggleEnableTool(MeaRulerTool::kToolName);
}


void CChildView::OnUpdateRuler(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(MeaToolMgr::Instance().IsToolEnabled(MeaRulerTool::kToolName));
}


void CChildView::ViewMagnifier(bool enable) 
{
    CFrameWnd *frame = GetParentFrame();

    int frameHeight = MeaLayout::GetWindowHeight(*frame);
    int magHeight   = m_magnifier.GetHeight() + m_margin.cy;

    if (enable && !m_magnifier.IsEnabled()) {
        m_magnifier.Enable();
        MeaLayout::SetWindowHeight(*frame, frameHeight + magHeight);
        CheckMargins();
    } else if (!enable && m_magnifier.IsEnabled()) {
        m_magnifier.Disable();
        MeaLayout::SetWindowHeight(*frame, frameHeight - magHeight);
        CheckMargins();
    }
}


void CChildView::OnMagnifier() 
{
    ViewMagnifier(!m_magnifier.IsEnabled());
}


void CChildView::OnUpdateMagnifier(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_magnifier.IsEnabled());
}


void CChildView::OnRunState() 
{
    if (m_magnifier.GetRunState() == MeaMagnifier::Run) {
        m_magnifier.SetRunState(MeaMagnifier::Freeze);
    } else {
        m_magnifier.SetRunState(MeaMagnifier::Run);
    }
}


void CChildView::OnUpdateRunState(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_magnifier.GetRunState() == MeaMagnifier::Freeze);
}


void CChildView::OnZoomIn() 
{
    m_magnifier.ZoomIn();
}


void CChildView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_magnifier.CanZoomIn());
}


void CChildView::OnZoomOut() 
{
    m_magnifier.ZoomOut();
}


void CChildView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_magnifier.CanZoomOut());
}


void CChildView::OnShowMagGrid() 
{
    m_magnifier.SetShowGrid(!m_magnifier.GetShowGrid());
}


void CChildView::OnUpdateShowMagGrid(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_magnifier.GetShowGrid());
}


void CChildView::OnScreenGrid() 
{
    MeaToolMgr::Instance().ToggleEnableTool(MeaGridTool::kToolName);
}


void CChildView::OnUpdateScreenGrid(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(MeaToolMgr::Instance().IsToolEnabled(MeaGridTool::kToolName));
}


void CChildView::OnScreenGridSpacing() 
{
    MeaToolMgr& toolMgr = MeaToolMgr::Instance();

    bool wasEnabled = toolMgr.IsToolEnabled(MeaGridTool::kToolName);
    if (!wasEnabled) {
        toolMgr.EnableTool(MeaGridTool::kToolName);
    }

    MeaGridDialog gridDialog(static_cast<MeaGridTool*>(toolMgr.FindTool(MeaGridTool::kToolName)));
    gridDialog.DoModal();

    if (!wasEnabled) {
        toolMgr.DisableTool(MeaGridTool::kToolName);
    }
}


void CChildView::OnHideCrosshairs() 
{
    MeaToolMgr& mgr = MeaToolMgr::Instance();

    if (mgr.CrosshairsEnabled()) {
        mgr.DisableCrosshairs();
    } else {
        mgr.EnableCrosshairs();
    }
}


void CChildView::OnUpdateHideCrosshairs(CCmdUI* pCmdUI) 
{
    MeaToolMgr& mgr = MeaToolMgr::Instance();

    pCmdUI->SetCheck(!mgr.CrosshairsEnabled());
    pCmdUI->Enable(mgr.HasCrosshairs());
}


void CChildView::OnFindCrosshair() 
{
    MeaToolMgr::Instance().FlashTool();
}


void CChildView::OnUpdateFindCrosshair(CCmdUI* pCmdUI) 
{
    MeaToolMgr& mgr = MeaToolMgr::Instance();

    pCmdUI->Enable(mgr.HasCrosshairs() && mgr.CrosshairsEnabled());
}


bool CChildView::IsCollapsed() const
{
    return (!IsToolbarVisible() && !IsStatusbarVisible() &&
        !m_magnifier.IsEnabled() && 
        !m_dataDisplay.IsEnabled(MeaScreenSection) && 
        !m_dataDisplay.IsEnabled(MeaRegionSection));
}


void CChildView::OnCollapse() 
{
    if (!IsCollapsed()) {
        m_expandToolbar = IsToolbarVisible();
        m_expandStatusbar = IsStatusbarVisible();
        m_expandMagnifier = m_magnifier.IsEnabled();    
        m_expandScreenInfo = m_dataDisplay.IsEnabled(MeaScreenSection);
        m_expandToolInfo = m_dataDisplay.IsEnabled(MeaRegionSection);
    }

    if (m_expandToolbar) {
        CFrameWnd *frame = GetParentFrame();
        frame->PostMessage(WM_COMMAND, ID_VIEW_TOOLBAR);
    }
    if (m_expandStatusbar) {
        CFrameWnd *frame = GetParentFrame();
        frame->PostMessage(WM_COMMAND, ID_VIEW_STATUSBAR);
    }
    if (m_expandMagnifier) {
        OnMagnifier();
    }
    if (m_expandScreenInfo) {
        OnScreenInfo();
    }
    if (m_expandToolInfo) {
        OnToolInfo();
    }
}


void CChildView::OnUpdateCollapse(CCmdUI* pCmdUI) 
{
    CString label;

    if (IsCollapsed()) {
        VERIFY(label.LoadString(IDS_MEA_EXPAND));
    } else {
        VERIFY(label.LoadString(IDS_MEA_COLLAPSE));
    }

    pCmdUI->SetText(label);
}


void CChildView::ViewToolInfo(bool enable) 
{
    CFrameWnd *frame = GetParentFrame();

    int frameHeight = MeaLayout::GetWindowHeight(*frame);
    int dispHeight = m_dataDisplay.GetHeight(MeaRegionSection);
    
    if (enable && !m_dataDisplay.IsEnabled(MeaRegionSection)) {
        m_dataDisplay.Enable(MeaRegionSection);
        MeaLayout::MoveWindowHeight(m_magnifier, dispHeight);
        MeaLayout::SetWindowHeight(*frame, frameHeight + dispHeight);
        CheckMargins();
    } else if (!enable && m_dataDisplay.IsEnabled(MeaRegionSection)) {
        m_dataDisplay.Disable(MeaRegionSection);
        MeaLayout::MoveWindowHeight(m_magnifier, -dispHeight);
        MeaLayout::SetWindowHeight(*frame, frameHeight - dispHeight);
        CheckMargins();
    }
}


void CChildView::OnToolInfo() 
{
    ViewToolInfo(!m_dataDisplay.IsEnabled(MeaRegionSection));
}


void CChildView::OnUpdateToolInfo(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_dataDisplay.IsEnabled(MeaRegionSection));
}


void CChildView::ViewScreenInfo(bool enable)
{
    CFrameWnd *frame = GetParentFrame();

    int frameHeight = MeaLayout::GetWindowHeight(*frame);
    int dispHeight = m_dataDisplay.GetHeight(MeaScreenSection);
    
    if (enable && !m_dataDisplay.IsEnabled(MeaScreenSection)) {
        m_dataDisplay.Enable(MeaScreenSection);
        MeaLayout::MoveWindowHeight(m_magnifier, dispHeight);
        MeaLayout::SetWindowHeight(*frame, frameHeight + dispHeight);
        CheckMargins();
    } else if (!enable && m_dataDisplay.IsEnabled(MeaScreenSection)) {
        m_dataDisplay.Disable(MeaScreenSection);
        MeaLayout::MoveWindowHeight(m_magnifier, -dispHeight);
        MeaLayout::SetWindowHeight(*frame, frameHeight - dispHeight);
        CheckMargins();
    }

}


void CChildView::OnScreenInfo() 
{
    ViewScreenInfo(!m_dataDisplay.IsEnabled(MeaScreenSection));
}


void CChildView::OnUpdateScreenInfo(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_dataDisplay.IsEnabled(MeaScreenSection));
}


LRESULT CChildView::OnDataChange(WPARAM wParam, LPARAM lParam)
{
    MeaToolMgr::Instance().SetPosition(static_cast<MeaFields>(lParam), wParam);
    return 0;
}


void CChildView::OnDestroy() 
{
    MeaToolMgr::Instance().DisableRadioTools();
    m_magnifier.Disable();
    CWnd::OnDestroy();
}


LRESULT CChildView::OnShowCalPrefs(WPARAM /* wParam */, LPARAM /* lParam */)
{
    m_prefs.SetActivePage(&m_prefs.m_calibrationPrefs);
    OnPreferences();
    return 0;
}


void CChildView::OnPreferences() 
{
    // If the preferences dialog is already displayed, leave it alone.
    //
    if (m_prefs.GetSafeHwnd() != NULL) {
        return;
    }

    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    // Tool preferences
    //
    m_prefs.m_toolsPrefs.m_backColor    = MeaColors::Get(MeaColors::CrossHairBack);
    m_prefs.m_toolsPrefs.m_borderColor  = MeaColors::Get(MeaColors::CrossHairBorder);
    m_prefs.m_toolsPrefs.m_hiliteColor  = MeaColors::Get(MeaColors::CrossHairHilite);
    m_prefs.m_toolsPrefs.m_lineColor    = MeaColors::Get(MeaColors::LineFore);
    m_prefs.m_toolsPrefs.m_opacity      = 100 * MeaColors::GetA(MeaColors::CrossHairOpacity) / 255;
    m_prefs.m_toolsPrefs.m_showDataWin  = MeaDataWin::IsArmed();
    m_prefs.m_toolsPrefs.m_originMarker = MeaToolMgr::Instance().IsToolEnabled(MeaOriginTool::kToolName);

    // Precision preferences
    //
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaPixelsId,      unitsMgr.GetLinearDisplayPrecisions(MeaPixelsId));
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaPointsId,      unitsMgr.GetLinearDisplayPrecisions(MeaPointsId));
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaTwipsId,       unitsMgr.GetLinearDisplayPrecisions(MeaTwipsId));
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaInchesId,      unitsMgr.GetLinearDisplayPrecisions(MeaInchesId));
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaCentimetersId, unitsMgr.GetLinearDisplayPrecisions(MeaCentimetersId));
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaMillimetersId, unitsMgr.GetLinearDisplayPrecisions(MeaMillimetersId));
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaPicasId,       unitsMgr.GetLinearDisplayPrecisions(MeaPicasId));
    m_prefs.m_precisionPrefs.SetLinearPrecisions(MeaCustomId,      unitsMgr.GetLinearDisplayPrecisions(MeaCustomId));

    m_prefs.m_precisionPrefs.SetAngularPrecisions(MeaDegreesId,    unitsMgr.GetAngularDisplayPrecisions(MeaDegreesId));
    m_prefs.m_precisionPrefs.SetAngularPrecisions(MeaRadiansId,    unitsMgr.GetAngularDisplayPrecisions(MeaRadiansId));

    // Custom units preferences
    //
    MeaCustomUnits* customUnits = unitsMgr.GetCustomUnits();
    m_prefs.m_customPrefs.m_name   = customUnits->GetName();
    m_prefs.m_customPrefs.m_abbrev = customUnits->GetAbbrev();
    m_prefs.m_customPrefs.SetScaleFactor(customUnits->GetScaleBasis(), customUnits->GetScaleFactor());

    // Ruler preferences
    //
    m_prefs.m_rulerPrefs.m_backColor    = MeaColors::Get(MeaColors::RulerBack);
    m_prefs.m_rulerPrefs.m_borderColor  = MeaColors::Get(MeaColors::RulerBorder);
    m_prefs.m_rulerPrefs.m_opacity      = 100 * MeaColors::GetA(MeaColors::RulerOpacity) / 255;

    // Calibration preferences
    //
    bool useManualRes;
    FSIZE manualRes;
    MeaScreenMgr::ScreenIter iter;
    MeaScreenMgr& screenMgr = MeaScreenMgr::Instance();

    m_prefs.m_calibrationPrefs.m_screens.clear();
    for (iter = screenMgr.GetScreenIter(); !screenMgr.AtEnd(iter); ++iter) {
        MeaCalibrationPrefs::Screen screen;

        screenMgr.GetScreenRes(iter, useManualRes, manualRes);
        screen.m_resMode = useManualRes ? MeaCalibrationPrefs::ManualRes : MeaCalibrationPrefs::AutoRes;
        screen.m_res = manualRes;
        screen.m_resUnits = screenMgr.GetCalInInches(iter) ? MeaCalibrationPrefs::UseInches : MeaCalibrationPrefs::UseCentimeters;

        m_prefs.m_calibrationPrefs.m_screens[iter] = screen;
    }

    // Advanced preferences
    //
    m_prefs.m_advancedPrefs.m_startupProfile = m_startupProfile;
    m_prefs.m_advancedPrefs.m_startupProfileDlg = MeaProfileMgr::Instance().CreateLoadDialog();

    int res = m_prefs.DoModal();
    if (res == IDOK) {
        ApplyPreferences();
    } else if (res == ID_MEA_MASTER_RESET) {
        GetParentFrame()->PostMessage(MeaMasterResetMsg);
    }
}


LRESULT CChildView::OnPrefsApply(WPARAM /* wParam */, LPARAM lParam)
{
    ApplyPreferences(lParam);
    return 0;
}


void CChildView::ApplyPreferences(int prefsPage)
{
    // Tool preferences
    //
    if (prefsPage == MeaPreferences::kAllPages || prefsPage == MeaPreferences::kToolsPage) {
        MeaColors::Set(MeaColors::LineFore,             m_prefs.m_toolsPrefs.m_lineColor);
        MeaColors::Set(MeaColors::CrossHairBack,        m_prefs.m_toolsPrefs.m_backColor);
        MeaColors::Set(MeaColors::CrossHairBorder,      m_prefs.m_toolsPrefs.m_borderColor);
        MeaColors::Set(MeaColors::CrossHairHilite,      m_prefs.m_toolsPrefs.m_hiliteColor);
        MeaColors::SetA(MeaColors::CrossHairOpacity,    static_cast<BYTE>(255 * m_prefs.m_toolsPrefs.m_opacity / 100));

        MeaDataWin::Arm(m_prefs.m_toolsPrefs.m_showDataWin == TRUE);

        if (m_prefs.m_toolsPrefs.m_originMarker)
            MeaToolMgr::Instance().EnableTool(MeaOriginTool::kToolName);
        else
            MeaToolMgr::Instance().DisableTool(MeaOriginTool::kToolName);

        MeaToolMgr::Instance().UpdateTools(DataWinArming);
    }

    // Precision preferences
    //
    if (prefsPage == MeaPreferences::kAllPages || prefsPage == MeaPreferences::kPrecisionPage) {
        MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

        unitsMgr.SetLinearDisplayPrecisions(MeaPixelsId,      m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaPixelsId));
        unitsMgr.SetLinearDisplayPrecisions(MeaPointsId,      m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaPointsId));
        unitsMgr.SetLinearDisplayPrecisions(MeaTwipsId,       m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaTwipsId));
        unitsMgr.SetLinearDisplayPrecisions(MeaInchesId,      m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaInchesId));
        unitsMgr.SetLinearDisplayPrecisions(MeaCentimetersId, m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaCentimetersId));
        unitsMgr.SetLinearDisplayPrecisions(MeaMillimetersId, m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaMillimetersId));
        unitsMgr.SetLinearDisplayPrecisions(MeaPicasId,       m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaPicasId));
        unitsMgr.SetLinearDisplayPrecisions(MeaCustomId,      m_prefs.m_precisionPrefs.GetLinearPrecisions(MeaCustomId));

        unitsMgr.SetAngularDisplayPrecisions(MeaDegreesId,    m_prefs.m_precisionPrefs.GetAngularPrecisions(MeaDegreesId));
        unitsMgr.SetAngularDisplayPrecisions(MeaRadiansId,    m_prefs.m_precisionPrefs.GetAngularPrecisions(MeaRadiansId));

        MeaToolMgr::Instance().UpdateTools(UnitsChanged);
    }

    // Custom units preferences
    //
    if (prefsPage == MeaPreferences::kAllPages || prefsPage == MeaPreferences::kCustomPage) {
        MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();
        MeaCustomUnits* customUnits = unitsMgr.GetCustomUnits();

        customUnits->SetName(m_prefs.m_customPrefs.m_name);
        customUnits->SetAbbrev(m_prefs.m_customPrefs.m_abbrev);

        MeaCustomUnits::ScaleBasis scaleBasis;
        double scaleFactor;

        m_prefs.m_customPrefs.GetScaleFactor(scaleBasis, scaleFactor);
        customUnits->SetScaleBasis(scaleBasis);
        customUnits->SetScaleFactor(scaleFactor);

        // If the custom units are currently selected but we removed them, switch
        // the units to pixels.
        //
        if (unitsMgr.GetLinearUnitsId() == MeaCustomId && !customUnits->HaveCustomUnits()) {
            unitsMgr.SetLinearUnits(MeaPixelsId);
            MeaToolMgr::Instance().UpdateTools(UnitsChanged);
        }
        // If we pressed Apply while on the custom units page or custom units are currently
        // set, show/update the custom units displayed.
        //
        else if (prefsPage == MeaPreferences::kCustomPage || unitsMgr.GetLinearUnitsId() == MeaCustomId) {
            unitsMgr.SetLinearUnits(MeaCustomId);
            MeaToolMgr::Instance().UpdateTools(UnitsChanged);
        }
    }

    // Ruler preferences
    //
    if (prefsPage == MeaPreferences::kAllPages || prefsPage == MeaPreferences::kRulerPage) {
        MeaColors::Set(MeaColors::RulerBack,        m_prefs.m_rulerPrefs.m_backColor);
        MeaColors::Set(MeaColors::RulerBorder,      m_prefs.m_rulerPrefs.m_borderColor);
        MeaColors::SetA(MeaColors::RulerOpacity,    static_cast<BYTE>(255 * m_prefs.m_rulerPrefs.m_opacity / 100));
    }

    if (prefsPage == MeaPreferences::kAllPages || prefsPage == MeaPreferences::kRulerPage || prefsPage == MeaPreferences::kToolsPage) {
        MeaToolMgr::Instance().ColorsChanged();
    }

    // Calibration preferences
    //
    if (prefsPage == MeaPreferences::kAllPages || prefsPage == MeaPreferences::kCalibrationPage) {
        MeaCalibrationPrefs::ScreenMap::const_iterator calIter;
        MeaScreenMgr& screenMgr = MeaScreenMgr::Instance();

        for (calIter = m_prefs.m_calibrationPrefs.m_screens.begin();
                calIter != m_prefs.m_calibrationPrefs.m_screens.end(); calIter++) {
            MeaScreenMgr::ScreenIter screenIter = (*calIter).first;
            const MeaCalibrationPrefs::Screen& screen = (*calIter).second;

            bool useManualRes = (screen.m_resMode == MeaCalibrationPrefs::AutoRes) ? false : true;
            FSIZE manualRes = screen.m_res;
            screenMgr.SetScreenRes(screenIter, useManualRes, &manualRes);

            bool calInInches = (screen.m_resUnits == MeaCalibrationPrefs::UseInches) ? true : false;
            screenMgr.SetCalInInches(screenIter, calInInches);
        }

        m_dataDisplay.ShowCalButton(screenMgr.AnyOSRes());
        MeaToolMgr::Instance().UpdateTools();
    }

    // Advanced preferences
    //
    if (prefsPage == MeaPreferences::kAllPages || prefsPage == MeaPreferences::kAdvancedPage) {
        m_startupProfile = m_prefs.m_advancedPrefs.m_startupProfile;
    }
}


void CChildView::OnCopyRegion() 
{
    MeaToolMgr& mgr = MeaToolMgr::Instance();

    if (mgr.HasRegion() && ::OpenClipboard(m_hWnd) != NULL) {
        // We must use a timer to allow the region tool to disable
        // before we copy the screen image to the clipboard. Otherwise
        // we'll capture the tool's outline and crosshairs as part of
        // the image.
        //
        mgr.DisableRadioTool();
        m_snapshotTimer.Start(10);
    }
}


LRESULT CChildView::OnHPTimer(WPARAM, LPARAM)
{
    // Get the screen region to copy.
    //
    CRect rect(MeaToolMgr::Instance().GetRegion());

    // Create a memory context and bitmap for it.
    //
    HDC screenDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    HDC memDC = ::CreateCompatibleDC(screenDC);

    HBITMAP screenBitmap = ::CreateCompatibleBitmap(screenDC, rect.Width(), rect.Height());
    ::SelectObject(memDC, screenBitmap);

    // Copy the screen region to the bitmap.
    //
    ::BitBlt(memDC, 0, 0, rect.Width(), rect.Height(),
        screenDC, rect.left, rect.top, SRCCOPY);

    // Put bitmap on the clipboard.
    //
    ::EmptyClipboard();
    ::SetClipboardData(CF_BITMAP, screenBitmap);
    ::CloseClipboard();

    // Cleanup
    //
    ::DeleteDC(screenDC);

    MeaToolMgr::Instance().EnableRadioTool();

    return 0;
}


void CChildView::OnUpdateCopyRegion(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(MeaToolMgr::Instance().HasRegion());
}


void CChildView::OnEditCopy() 
{
    MeaNumberField *dataField = m_dataDisplay.GetFieldFocus();
    MeaNumberField *colorField = m_magnifier.GetFieldFocus();

    if (dataField != NULL) {
        dataField->Copy();
    } else if (colorField != NULL) {
        colorField->Copy();
    }
}


void CChildView::OnEditCut() 
{
    MeaNumberField *dataField = m_dataDisplay.GetFieldFocus();

    if (dataField != NULL) {
        dataField->Cut();
    }
}


void CChildView::OnEditPaste() 
{
    MeaNumberField *dataField = m_dataDisplay.GetFieldFocus();

    if (dataField != NULL) {
        dataField->Paste();
    }
}


void CChildView::OnEditClear() 
{
    MeaNumberField *dataField = m_dataDisplay.GetFieldFocus();

    if (dataField != NULL) {
        dataField->Clear();
    }
}


void CChildView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
    MeaNumberField *dataField  = m_dataDisplay.GetFieldFocus();
    MeaNumberField *colorField = m_magnifier.GetFieldFocus();

    pCmdUI->Enable(((dataField != NULL) && dataField->IsTextSelected()) ||
                   ((colorField != NULL) && colorField->IsTextSelected()));
}


void CChildView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
    MeaNumberField *dataField = m_dataDisplay.GetFieldFocus();

    pCmdUI->Enable((dataField != NULL) && dataField->IsTextSelected());
}


void CChildView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
    MeaNumberField *dataField = m_dataDisplay.GetFieldFocus();

    pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT) && (dataField != NULL));
}


void CChildView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
    MeaNumberField *dataField = m_dataDisplay.GetFieldFocus();

    pCmdUI->Enable((dataField != NULL) && dataField->IsTextSelected());
}
