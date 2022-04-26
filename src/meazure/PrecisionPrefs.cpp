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
#include "PrecisionPrefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(MeaPrecisionPrefs, CPropertyPage)


BEGIN_MESSAGE_MAP(MeaPrecisionPrefs, CPropertyPage)
    ON_BN_CLICKED(IDC_PREC_CM, OnCm)
    ON_BN_CLICKED(IDC_PREC_CUSTOM, OnCustom)
    ON_BN_CLICKED(IDC_PREC_DEG, OnDeg)
    ON_BN_CLICKED(IDC_PREC_IN, OnIn)
    ON_BN_CLICKED(IDC_PREC_MM, OnMm)
    ON_BN_CLICKED(IDC_PREC_PC, OnPc)
    ON_BN_CLICKED(IDC_PREC_PT, OnPt)
    ON_BN_CLICKED(IDC_PREC_PX, OnPx)
    ON_BN_CLICKED(IDC_PREC_RAD, OnRad)
    ON_BN_CLICKED(IDC_PREC_TP, OnTp)
    ON_EN_KILLFOCUS(IDC_PREC_A_FIELD, OnKillFocusA)
    ON_EN_KILLFOCUS(IDC_PREC_AR_FIELD, OnKillFocusAr)
    ON_EN_KILLFOCUS(IDC_PREC_D_FIELD, OnKillFocusD)
    ON_EN_KILLFOCUS(IDC_PREC_H_FIELD, OnKillFocusH)
    ON_EN_KILLFOCUS(IDC_PREC_RX_FIELD, OnKillFocusRx)
    ON_EN_KILLFOCUS(IDC_PREC_RY_FIELD, OnKillFocusRy)
    ON_EN_KILLFOCUS(IDC_PREC_W_FIELD, OnKillFocusW)
    ON_EN_KILLFOCUS(IDC_PREC_X_FIELD, OnKillFocusX)
    ON_EN_KILLFOCUS(IDC_PREC_Y_FIELD, OnKillFocusY)
    ON_BN_CLICKED(IDC_PREC_DEF, OnDefault)
    ON_EN_CHANGE(IDC_PREC_A_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_AR_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_D_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_H_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_RX_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_RY_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_W_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_X_FIELD, OnChange)
    ON_EN_CHANGE(IDC_PREC_Y_FIELD, OnChange)
END_MESSAGE_MAP()


MeaPrecisionPrefs::MeaPrecisionPrefs() : CPropertyPage(MeaPrecisionPrefs::IDD),
    m_curLinearUnitsId(MeaPixelsId), m_curAngularUnitsId(MeaDegreesId),
    m_lockout(false)
{
    // Instantiate the fields.
    //
    m_linearFields[MeaX]    = new MeaNumberField();
    m_linearFields[MeaY]    = new MeaNumberField();
    m_linearFields[MeaW]    = new MeaNumberField();
    m_linearFields[MeaH]    = new MeaNumberField();
    m_linearFields[MeaD]    = new MeaNumberField();
    m_linearFields[MeaAr]   = new MeaNumberField();
    m_linearFields[MeaRx]   = new MeaNumberField();
    m_linearFields[MeaRy]   = new MeaNumberField();

    m_angularFields[MeaA]   = new MeaNumberField();
}


MeaPrecisionPrefs::~MeaPrecisionPrefs()
{
    try {
        for (const auto& field : m_linearFields) {
            delete field.second;
        }
        for (const auto& field : m_angularFields) {
            delete field.second;
        }

        m_linearFields.clear();
        m_angularFields.clear();
    }
    catch(...) {
        MeaAssert(false);
    }
}


void MeaPrecisionPrefs::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
}


BOOL MeaPrecisionPrefs::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // Obtain the label controls
    //
    m_linearLabels[MeaX]    = static_cast<CStatic*>(GetDlgItem(IDC_PREC_X_LABEL));
    m_linearLabels[MeaY]    = static_cast<CStatic*>(GetDlgItem(IDC_PREC_Y_LABEL));
    m_linearLabels[MeaW]    = static_cast<CStatic*>(GetDlgItem(IDC_PREC_W_LABEL));
    m_linearLabels[MeaH]    = static_cast<CStatic*>(GetDlgItem(IDC_PREC_H_LABEL));
    m_linearLabels[MeaD]    = static_cast<CStatic*>(GetDlgItem(IDC_PREC_D_LABEL));
    m_linearLabels[MeaAr]   = static_cast<CStatic*>(GetDlgItem(IDC_PREC_AR_LABEL));
    m_linearLabels[MeaRx]   = static_cast<CStatic*>(GetDlgItem(IDC_PREC_RX_LABEL));
    m_linearLabels[MeaRy]   = static_cast<CStatic*>(GetDlgItem(IDC_PREC_RY_LABEL));

    m_angularLabels[MeaA]   = static_cast<CStatic*>(GetDlgItem(IDC_PREC_A_LABEL));

    // Set the styles on the fields
    //
    for (const auto& field : m_linearFields) {
        field.second->SetValueType(MeaNumberField::IntValues | MeaNumberField::PosValues);
    }
    for (const auto& field : m_angularFields) {
        field.second->SetValueType(MeaNumberField::IntValues | MeaNumberField::PosValues);
    }

    // Replace the CEdit controls with our field control.
    //
    m_linearFields[MeaX]->SubclassDlgItem(IDC_PREC_X_FIELD, this);
    m_linearFields[MeaY]->SubclassDlgItem(IDC_PREC_Y_FIELD, this);
    m_linearFields[MeaW]->SubclassDlgItem(IDC_PREC_W_FIELD, this);
    m_linearFields[MeaH]->SubclassDlgItem(IDC_PREC_H_FIELD, this);
    m_linearFields[MeaD]->SubclassDlgItem(IDC_PREC_D_FIELD, this);
    m_linearFields[MeaAr]->SubclassDlgItem(IDC_PREC_AR_FIELD, this);
    m_linearFields[MeaRx]->SubclassDlgItem(IDC_PREC_RX_FIELD, this);
    m_linearFields[MeaRy]->SubclassDlgItem(IDC_PREC_RY_FIELD, this);

    m_angularFields[MeaDegreesId]->SubclassDlgItem(IDC_PREC_A_FIELD, this);

    // Limit the number of characters that can be entered.
    //
    int maxChars = (kMaxPlaces <= 0) ? 1 : (static_cast<int>(log10(static_cast<double>(kMaxPlaces))) + 1);

    for (const auto& field : m_linearFields) {
        field.second->SetLimitText(maxChars);
    }
    for (const auto& field : m_angularFields) {
        field.second->SetLimitText(maxChars);
    }

    // Obtain the spin controls
    //
    m_linearSpins[MeaX]     = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_X_SPIN));
    m_linearSpins[MeaY]     = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_Y_SPIN));
    m_linearSpins[MeaW]     = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_W_SPIN));
    m_linearSpins[MeaH]     = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_H_SPIN));
    m_linearSpins[MeaD]     = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_D_SPIN));
    m_linearSpins[MeaAr]    = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_AR_SPIN));
    m_linearSpins[MeaRx]    = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_RX_SPIN));
    m_linearSpins[MeaRy]    = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_RY_SPIN));

    m_angularSpins[MeaA]    = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_PREC_A_SPIN));

    // Set the spin control behavior.
    //
    for (const auto& spin : m_linearSpins) {
        spin.second->SetRange(0, kMaxPlaces);
    }
    for (const auto& spin : m_angularSpins) {
        spin.second->SetRange(0, kMaxPlaces);
    }

    // Set the initial units to the current units manager linear value.
    //
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    m_curAngularUnitsId = unitsMgr.GetAngularUnitsId();
    m_curLinearUnitsId  = unitsMgr.GetLinearUnitsId();

    switch (m_curLinearUnitsId) {
    case MeaPixelsId:
        SetCheck(IDC_PREC_PX);
        break;
    case MeaPointsId:
        SetCheck(IDC_PREC_PT);
        break;
    case MeaTwipsId:
        SetCheck(IDC_PREC_TP);
        break;
    case MeaInchesId:
        SetCheck(IDC_PREC_IN);
        break;
    case MeaCentimetersId:
        SetCheck(IDC_PREC_CM);
        break;
    case MeaMillimetersId:
        SetCheck(IDC_PREC_MM);
        break;
    case MeaPicasId:
        SetCheck(IDC_PREC_PC);
        break;
    case MeaCustomId:
        SetCheck(IDC_PREC_CUSTOM);
        break;
    }

    ShowLinearPrecisions();
    UpdateAngularDisplay();

    return TRUE;
}


void MeaPrecisionPrefs::SelectCustomUnits()
{
    m_curLinearUnitsId  = MeaCustomId;
    SetCheck(IDC_PREC_CUSTOM);
    ShowLinearPrecisions();
    UpdateAngularDisplay();
}


void MeaPrecisionPrefs::SetCheck(UINT buttonId) const
{
    static_cast<CButton*>(GetDlgItem(IDC_PREC_PX))->SetCheck((buttonId == IDC_PREC_PX) ? 1 : 0);
    static_cast<CButton*>(GetDlgItem(IDC_PREC_PT))->SetCheck((buttonId == IDC_PREC_PT) ? 1 : 0);
    static_cast<CButton*>(GetDlgItem(IDC_PREC_TP))->SetCheck((buttonId == IDC_PREC_TP) ? 1 : 0);
    static_cast<CButton*>(GetDlgItem(IDC_PREC_IN))->SetCheck((buttonId == IDC_PREC_IN) ? 1 : 0);
    static_cast<CButton*>(GetDlgItem(IDC_PREC_CM))->SetCheck((buttonId == IDC_PREC_CM) ? 1 : 0);
    static_cast<CButton*>(GetDlgItem(IDC_PREC_MM))->SetCheck((buttonId == IDC_PREC_MM) ? 1 : 0);
    static_cast<CButton*>(GetDlgItem(IDC_PREC_PC))->SetCheck((buttonId == IDC_PREC_PC) ? 1 : 0);
    static_cast<CButton*>(GetDlgItem(IDC_PREC_CUSTOM))->SetCheck((buttonId == IDC_PREC_CUSTOM) ? 1 : 0);
}


void MeaPrecisionPrefs::ShowLinearPrecisions()
{
    EnableLinearDisplay(true);
    EnableAngularDisplay(false);

    UpdateLinearDisplay();
}


void MeaPrecisionPrefs::ShowAngularPrecisions()
{
    EnableLinearDisplay(false);
    EnableAngularDisplay(true);

    UpdateAngularDisplay();
}


void MeaPrecisionPrefs::UpdateLinearDisplay()
{
    const MeaUnits::DisplayPrecisions& precisions = m_linearPrecisions[m_curLinearUnitsId];

    m_lockout = true;
    m_linearFields[MeaX]->SetValue(precisions[MeaX]);
    m_linearFields[MeaY]->SetValue(precisions[MeaY]);
    m_linearFields[MeaW]->SetValue(precisions[MeaW]);
    m_linearFields[MeaH]->SetValue(precisions[MeaH]);
    m_linearFields[MeaD]->SetValue(precisions[MeaD]);
    m_linearFields[MeaAr]->SetValue(precisions[MeaAr]);
    m_linearFields[MeaRx]->SetValue(precisions[MeaRx]);
    m_linearFields[MeaRy]->SetValue(precisions[MeaRy]);
    m_lockout = false;
}


void MeaPrecisionPrefs::UpdateAngularDisplay()
{
    const MeaUnits::DisplayPrecisions& precisions = m_angularPrecisions[m_curAngularUnitsId];

    m_lockout = true;
    m_angularFields[MeaA]->SetValue(precisions[MeaA]);
    m_lockout = false;
}


void MeaPrecisionPrefs::EnableLinearDisplay(bool enable)
{
    for (const auto& label : m_linearLabels) {
        label.second->EnableWindow(enable);
    }
    for (const auto& field : m_linearFields) {
        field.second->EnableWindow(enable);
    }
    for (const auto& spin : m_linearSpins) {
        spin.second->EnableWindow(enable);
    }
}


void MeaPrecisionPrefs::EnableAngularDisplay(bool enable)
{
    for (const auto& label : m_angularLabels) {
        label.second->EnableWindow(enable);
    }
    for (const auto& field : m_angularFields) {
        field.second->EnableWindow(enable);
    }
    for (const auto& spin : m_angularSpins) {
        spin.second->EnableWindow(enable);
    }
}


void MeaPrecisionPrefs::OnChange() 
{
    if (!m_lockout) {
        SetModified(TRUE);
    }
}


void MeaPrecisionPrefs::OnDefault() 
{
    MeaUnitsMgr& unitsMgr = MeaUnitsMgr::Instance();

    m_linearPrecisions[m_curLinearUnitsId] = unitsMgr.GetLinearDefaultPrecisions(m_curLinearUnitsId);
    m_angularPrecisions[m_curAngularUnitsId] = unitsMgr.GetAngularDefaultPrecisions(m_curAngularUnitsId);

    UpdateLinearDisplay();
    UpdateAngularDisplay();

    OnChange();
}


void MeaPrecisionPrefs::OnPx() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_PX))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaPixelsId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnTp() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_TP))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaTwipsId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnPt() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_PT))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaPointsId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnPc() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_PC))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaPicasId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnIn() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_IN))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaInchesId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnCm() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_CM))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaCentimetersId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnMm() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_MM))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaMillimetersId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnCustom() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_CUSTOM))->GetCheck() == 1) {
        m_curLinearUnitsId = MeaCustomId;
        ShowLinearPrecisions();
    }
}


void MeaPrecisionPrefs::OnDeg() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_DEG))->GetCheck() == 1) {
        m_curAngularUnitsId = MeaDegreesId;
        ShowAngularPrecisions();
    }
}


void MeaPrecisionPrefs::OnRad() 
{
    if (static_cast<CButton*>(GetDlgItem(IDC_PREC_RAD))->GetCheck() == 1) {
        m_curAngularUnitsId = MeaRadiansId;
        ShowAngularPrecisions();
    }
}


void MeaPrecisionPrefs::OnKillFocusA() 
{
    m_angularPrecisions[m_curAngularUnitsId][MeaA] = GetAngularValue(MeaA);
}


void MeaPrecisionPrefs::OnKillFocusX() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaX] = GetLinearValue(MeaX);
}


void MeaPrecisionPrefs::OnKillFocusY() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaY] = GetLinearValue(MeaY);
}


void MeaPrecisionPrefs::OnKillFocusW() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaW] = GetLinearValue(MeaW);
}


void MeaPrecisionPrefs::OnKillFocusH() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaH] = GetLinearValue(MeaH);
}


void MeaPrecisionPrefs::OnKillFocusD() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaD] = GetLinearValue(MeaD);
}


void MeaPrecisionPrefs::OnKillFocusAr() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaAr] = GetLinearValue(MeaAr);
}


void MeaPrecisionPrefs::OnKillFocusRx() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaRx] = GetLinearValue(MeaRx);
}


void MeaPrecisionPrefs::OnKillFocusRy() 
{
    m_linearPrecisions[m_curLinearUnitsId][MeaRy] = GetLinearValue(MeaRy);
}


int MeaPrecisionPrefs::GetValue(MeaNumberField* field)
{
    int value;
    bool invalid = false;

    field->GetValue(value);
    
    if (value < 0) {
        invalid = true;
        value = 0;
    }
    else if (value > kMaxPlaces) {
        invalid = true;
        value = kMaxPlaces;
    }

    if (invalid) {
        CString msg;
        msg.Format(IDS_MEA_PREC_VALUE, kMaxPlaces, value);
        AfxMessageBox(msg, MB_OK | MB_ICONSTOP);
        field->SetValue(value);
        field->SetFocus();
        field->SetSel(0, -1);
    }

    return value;
}
