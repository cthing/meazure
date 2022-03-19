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
#include "RulerPrefs.h"
#include "ColorDialog.h"
#include "Colors.h"
#include "LayeredWindows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(MeaRulerPrefs, CPropertyPage)


BEGIN_MESSAGE_MAP(MeaRulerPrefs, CPropertyPage)
    //{{AFX_MSG_MAP(MeaRulerPrefs)
    ON_BN_CLICKED(IDC_RULER_BACK_BTN, OnChangeBkColor)
    ON_BN_CLICKED(IDC_RULER_BORDER_BTN, OnChangeBorderColor)
    ON_BN_CLICKED(IDC_RULER_BACK_DEF, OnDefBkColor)
    ON_BN_CLICKED(IDC_RULER_BORDER_DEF, OnDefBorderColor)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


MeaRulerPrefs::MeaRulerPrefs() : CPropertyPage(MeaRulerPrefs::IDD),
    m_backColor(0), m_borderColor(0)
{
    m_psp.dwFlags &= ~(PSP_HASHELP);

    //{{AFX_DATA_INIT(MeaRulerPrefs)
    m_opacity = 0;
    //}}AFX_DATA_INIT
}


MeaRulerPrefs::~MeaRulerPrefs()
{
}


void MeaRulerPrefs::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(MeaRulerPrefs)
    DDX_Slider(pDX, IDC_RULER_OPACITY, m_opacity);
    //}}AFX_DATA_MAP
}


BOOL MeaRulerPrefs::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // Set the static control to serve as a ruler sample window
    // and create the ruler.
    //
    CStatic *rulerSample = static_cast<CStatic*>(GetDlgItem(IDC_RULER_SAMPLE));
    ::SetWindowLong(rulerSample->m_hWnd, GWL_STYLE, SS_SIMPLE);

    CRect sampleRect;
    CRect rulerRect;
    CRect horizRect;

    rulerSample->GetWindowRect(sampleRect);
    ScreenToClient(sampleRect);

    m_vRuler.Create(m_borderColor, m_backColor, 255, MeaRuler::Vertical, sampleRect, NULL, this);
    m_vRuler.GetClientRect(rulerRect);
    horizRect = sampleRect;
    horizRect.left += rulerRect.Width() - 1;
    m_hRuler.Create(m_borderColor, m_backColor, 255, MeaRuler::Horizontal, horizRect, NULL, this);

    m_vRuler.SetPosition(sampleRect.left);
    m_hRuler.SetPosition(sampleRect.top);

    if (HaveLayeredWindows()) {
        BYTE alpha = static_cast<BYTE>(255 * m_opacity / 100);
        m_vRuler.SetOpacity(alpha);
        m_hRuler.SetOpacity(alpha);
    }

    m_vRuler.Show();
    m_hRuler.Show();

    // Configure the opacity slider.
    //
    CSliderCtrl *slider = static_cast<CSliderCtrl*>(GetDlgItem(IDC_RULER_OPACITY));
    slider->SetRange(20, 100, TRUE);

    if (!HaveLayeredWindows()) {
        slider->EnableWindow(FALSE);
        GetDlgItem(IDC_RULER_OPACITY_LABEL1)->EnableWindow(FALSE);
        GetDlgItem(IDC_RULER_OPACITY_LABEL2)->EnableWindow(FALSE);
    }

    return TRUE;
}


void MeaRulerPrefs::OnChangeBkColor() 
{
    MeaColorDialog dlg(IDS_MEA_BACKDLG_TITLE);

    dlg.m_cc.Flags |= CC_RGBINIT;
    dlg.m_cc.rgbResult = m_backColor;
    
    if (dlg.DoModal() == IDOK) {
        m_backColor = dlg.GetColor();
        ColorsChanged();
    }
}


void MeaRulerPrefs::OnChangeBorderColor() 
{
    MeaColorDialog dlg(IDS_MEA_BORDERDLG_TITLE);

    dlg.m_cc.Flags |= CC_RGBINIT;
    dlg.m_cc.rgbResult = m_borderColor;

    if (dlg.DoModal() == IDOK) {
        m_borderColor = dlg.GetColor();
        ColorsChanged();
    }
}


void MeaRulerPrefs::OnDefBkColor() 
{
    m_backColor = MeaColors::GetDef(MeaColors::RulerBack);
    ColorsChanged();
}


void MeaRulerPrefs::OnDefBorderColor() 
{
    m_borderColor = MeaColors::GetDef(MeaColors::RulerBorder);
    ColorsChanged();
}


void MeaRulerPrefs::ColorsChanged()
{
    m_vRuler.SetColors(m_borderColor, m_backColor);
    m_hRuler.SetColors(m_borderColor, m_backColor);
    SetModified(TRUE);
}


void MeaRulerPrefs::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    CSliderCtrl *slider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
    int pos = slider->GetPos();

    BYTE alpha = static_cast<BYTE>(255 * pos / 100);
    m_vRuler.SetOpacity(alpha);
    m_hRuler.SetOpacity(alpha);

    if (pos != m_opacity) {
        SetModified(TRUE);
    }
    
    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
