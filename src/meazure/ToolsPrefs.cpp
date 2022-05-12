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
#include "ToolsPrefs.h"
#include "ColorDialog.h"
#include "Colors.h"
#include "LayeredWindows.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(MeaToolsPrefs, CPropertyPage)
    ON_BN_CLICKED(IDC_TOOLS_LINE_BTN, OnChangeLineColor)
    ON_BN_CLICKED(IDC_TOOLS_LINE_DEF, OnDefLineColor)
    ON_BN_CLICKED(IDC_TOOLS_CHBORDER_BTN, OnChangeBorderColor)
    ON_BN_CLICKED(IDC_TOOLS_CHBORDER_DEF, OnDefBorderColor)
    ON_BN_CLICKED(IDC_TOOLS_CHBACK_BTN, OnChangeBackColor)
    ON_BN_CLICKED(IDC_TOOLS_CHBACK_DEF, OnDefBackColor)
    ON_BN_CLICKED(IDC_TOOLS_CHHILITE_BTN, OnChangeHiliteColor)
    ON_BN_CLICKED(IDC_TOOLS_CHHILITE_DEF, OnDefHiliteColor)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_MEA_DATA_WIN, OnChange)
    ON_BN_CLICKED(IDC_MEA_ORIGIN_MARKER, OnChange)
END_MESSAGE_MAP()


MeaToolsPrefs::MeaToolsPrefs(const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider) :
    CPropertyPage(MeaToolsPrefs::IDD),
    m_screenProvider(screenProvider),
    m_lineColor(0),
    m_backColor(0),
    m_borderColor(0),
    m_hiliteColor(0),
    m_showDataWin(FALSE),
    m_opacity(0),
    m_originMarker(FALSE),
    m_normalCH(screenProvider, unitsProvider),
    m_hiliteCH(screenProvider, unitsProvider),
    m_dataWin(screenProvider, unitsProvider) {}

MeaToolsPrefs::~MeaToolsPrefs() {}

void MeaToolsPrefs::DoDataExchange(CDataExchange* pDX) {
    CPropertyPage::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_MEA_DATA_WIN, m_showDataWin);
    DDX_Slider(pDX, IDC_CH_OPACITY, m_opacity);
    DDX_Check(pDX, IDC_MEA_ORIGIN_MARKER, m_originMarker);
}

BOOL MeaToolsPrefs::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    CStatic* sample;
    CRect rect;
    BYTE alpha = static_cast<BYTE>(255 * m_opacity / 100);

    // Create the line samples
    //
    sample = static_cast<CStatic*>(GetDlgItem(IDC_LINE_SAMPLE));
    ::SetWindowLong(sample->m_hWnd, GWL_STYLE, SS_SIMPLE);
    sample->GetWindowRect(rect);
    ScreenToClient(rect);
    rect.DeflateRect(0, 3);

    m_line.Create(m_lineColor, rect, this);
    m_line.Show();

    // Create the crosshair samples
    //
    sample = static_cast<CStatic*>(GetDlgItem(IDC_CHNORM_SAMPLE));
    ::SetWindowLong(sample->m_hWnd, GWL_STYLE, SS_SIMPLE);
    sample->GetWindowRect(rect);
    ScreenToClient(rect);

    m_normalCH.Create(m_borderColor, m_backColor, m_backColor,
                        255, nullptr, this);
    m_normalCH.SetPosition(rect.CenterPoint());
    if (HaveLayeredWindows()) {
        m_normalCH.SetOpacity(alpha);
    }
    m_normalCH.Show();

    sample = static_cast<CStatic*>(GetDlgItem(IDC_CHHI_SAMPLE));
    ::SetWindowLong(sample->m_hWnd, GWL_STYLE, SS_SIMPLE);
    sample->GetWindowRect(rect);
    ScreenToClient(rect);

    m_hiliteCH.Create(m_borderColor, m_hiliteColor, m_hiliteColor,
                        255, nullptr, this);
    m_hiliteCH.SetPosition(rect.CenterPoint());
    if (HaveLayeredWindows()) {
        m_hiliteCH.SetOpacity(alpha);
    }
    m_hiliteCH.Show();

    // Create the data window sample.
    //
    sample = static_cast<CStatic*>(GetDlgItem(IDC_DATAWIN_SAMPLE));
    ::SetWindowLong(sample->m_hWnd, GWL_STYLE, SS_SIMPLE);
    sample->GetWindowRect(rect);
    ScreenToClient(rect);

    m_dataWin.Create(255, IDS_MEA_X1, IDS_MEA_Y1, MeaDataWin::kNoLabelId, MeaDataWin::kNoLabelId, IDS_MEA_LENGTH,
                     MeaDataWin::kNoLabelId, this);
    if (HaveLayeredWindows()) {
        m_dataWin.SetOpacity(alpha);
    }
    m_dataWin.Show();
    m_dataWin.SetWindowPos(nullptr, rect.left, rect.top - 2, rect.Width(),
                            MeaLayout::GetWindowHeight(m_dataWin) - 2, SWP_NOZORDER);

    // Configure the opacity slider.
    //
    CSliderCtrl* slider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_CH_OPACITY));
    slider->SetRange(20, 100, TRUE);

    if (!HaveLayeredWindows()) {
        slider->EnableWindow(FALSE);
        GetDlgItem(IDC_CH_OPACITY_LABEL1)->EnableWindow(FALSE);
        GetDlgItem(IDC_CH_OPACITY_LABEL2)->EnableWindow(FALSE);
    }

    return TRUE;
}

void MeaToolsPrefs::OnChangeLineColor() {
    MeaColorDialog dlg(IDS_MEA_LINEDLG_TITLE);

    dlg.m_cc.Flags |= CC_RGBINIT;
    dlg.m_cc.rgbResult = m_lineColor;

    if (dlg.DoModal() == IDOK) {
        m_lineColor = dlg.GetColor();
        m_line.SetColor(m_lineColor);
        SetModified(TRUE);
    }
}

void MeaToolsPrefs::OnChangeBorderColor() {
    MeaColorDialog dlg(IDS_MEA_BORDERDLG_TITLE);

    dlg.m_cc.Flags |= CC_RGBINIT;
    dlg.m_cc.rgbResult = m_borderColor;

    if (dlg.DoModal() == IDOK) {
        m_borderColor = dlg.GetColor();
        m_normalCH.SetColors(m_borderColor, m_backColor, m_backColor);
        m_hiliteCH.SetColors(m_borderColor, m_hiliteColor, m_hiliteColor);
        SetModified(TRUE);
    }
}

void MeaToolsPrefs::OnChangeBackColor() {
    MeaColorDialog dlg(IDS_MEA_BACKDLG_TITLE);

    dlg.m_cc.Flags |= CC_RGBINIT;
    dlg.m_cc.rgbResult = m_backColor;

    if (dlg.DoModal() == IDOK) {
        m_backColor = dlg.GetColor();
        m_normalCH.SetColors(m_borderColor, m_backColor, m_backColor);
        SetModified(TRUE);
    }
}

void MeaToolsPrefs::OnChangeHiliteColor() {
    MeaColorDialog dlg(IDS_MEA_HILITEDLG_TITLE);

    dlg.m_cc.Flags |= CC_RGBINIT;
    dlg.m_cc.rgbResult = m_hiliteColor;

    if (dlg.DoModal() == IDOK) {
        m_hiliteColor = dlg.GetColor();
        m_hiliteCH.SetColors(m_borderColor, m_hiliteColor, m_hiliteColor);
        SetModified(TRUE);
    }
}

void MeaToolsPrefs::OnDefLineColor() {
    m_lineColor = MeaColors::GetDef(MeaColors::LineFore);
    m_line.SetColor(m_lineColor);
    SetModified(TRUE);
}

void MeaToolsPrefs::OnDefBorderColor() {
    m_borderColor = MeaColors::GetDef(MeaColors::CrossHairBorder);
    m_normalCH.SetColors(m_borderColor, m_backColor, m_backColor);
    m_hiliteCH.SetColors(m_borderColor, m_hiliteColor, m_hiliteColor);
    SetModified(TRUE);
}

void MeaToolsPrefs::OnDefBackColor() {
    m_backColor = MeaColors::GetDef(MeaColors::CrossHairBack);
    m_normalCH.SetColors(m_borderColor, m_backColor, m_backColor);
    SetModified(TRUE);
}

void MeaToolsPrefs::OnDefHiliteColor() {
    m_hiliteColor = MeaColors::GetDef(MeaColors::CrossHairHilite);
    m_hiliteCH.SetColors(m_borderColor, m_hiliteColor, m_hiliteColor);
    SetModified(TRUE);
}

void MeaToolsPrefs::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
    CSliderCtrl* slider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
    int pos = slider->GetPos();

    BYTE alpha = static_cast<BYTE>(255 * pos / 100);
    m_normalCH.SetOpacity(alpha);
    m_hiliteCH.SetOpacity(alpha);
    m_dataWin.SetOpacity(alpha);

    if (pos != m_opacity) {
        SetModified(TRUE);
    }

    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void MeaToolsPrefs::OnChange() {
    SetModified(TRUE);
}
