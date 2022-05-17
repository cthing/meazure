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

#include <meazure/pch.h>
#include "CustomUnitsPrefs.h"
#include "Preferences.h"
#include <meazure/utilities/MeaAssert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(MeaCustomUnitsPrefs, CPropertyPage)


BEGIN_MESSAGE_MAP(MeaCustomUnitsPrefs, CPropertyPage)
    ON_BN_CLICKED(IDC_CUST_CLEAR, OnClear)
    ON_EN_CHANGE(IDC_CUST_CM_FIELD, OnCentimeterChange)
    ON_EN_CHANGE(IDC_CUST_NAME_FIELD, OnNameChange)
    ON_EN_CHANGE(IDC_CUST_ABBREV_FIELD, OnAbbrevChange)
    ON_BN_CLICKED(IDC_CUST_PREC, OnPrecision)
    ON_BN_CLICKED(IDC_CUST_CM, OnCentimeterBasis)
    ON_BN_CLICKED(IDC_CUST_IN, OnInchBasis)
    ON_BN_CLICKED(IDC_CUST_PX, OnPixelBasis)
    ON_EN_CHANGE(IDC_CUST_IN_FIELD, OnInchChange)
    ON_EN_CHANGE(IDC_CUST_PX_FIELD, OnPixelChange)
END_MESSAGE_MAP()


MeaCustomUnitsPrefs::MeaCustomUnitsPrefs() :
    CPropertyPage(MeaCustomUnitsPrefs::IDD),
    m_lockout(false),
    m_nameField(nullptr),
    m_abbrevField(nullptr),
    m_precisionButton(nullptr),
    m_basisLabel(nullptr),
    m_pxButton(nullptr),
    m_inButton(nullptr),
    m_cmButton(nullptr),
    m_pxLabel(nullptr),
    m_inLabel(nullptr),
    m_cmLabel(nullptr),
    m_pxAbbrev(nullptr),
    m_inAbbrev(nullptr),
    m_cmAbbrev(nullptr),
    m_abbrev(_T("")),
    m_name(_T("")),
    m_scaleBasis(MeaCustomUnits::PixelBasis),
    m_pxScaleFactor(1.0),
    m_inScaleFactor(1.0),
    m_cmScaleFactor(1.0) {}

MeaCustomUnitsPrefs::~MeaCustomUnitsPrefs() {
    m_nameField = nullptr;
    m_abbrevField = nullptr;
    m_precisionButton = nullptr;
    m_basisLabel = nullptr;

    m_pxButton = nullptr;
    m_inButton = nullptr;
    m_cmButton = nullptr;

    m_pxLabel = nullptr;
    m_inLabel = nullptr;
    m_cmLabel = nullptr;

    m_pxAbbrev = nullptr;
    m_inAbbrev = nullptr;
    m_cmAbbrev = nullptr;
}

void MeaCustomUnitsPrefs::DoDataExchange(CDataExchange* pDX) {
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CUST_ABBREV_FIELD, m_abbrev);
    DDV_MaxChars(pDX, m_abbrev, 2);
    DDX_Text(pDX, IDC_CUST_NAME_FIELD, m_name);
    DDV_MaxChars(pDX, m_name, 16);
    DDX_Radio(pDX, IDC_CUST_PX, m_scaleBasis);
    DDX_Text(pDX, IDC_CUST_PX_FIELD, m_pxScaleFactor);
    DDX_Text(pDX, IDC_CUST_IN_FIELD, m_inScaleFactor);
    DDX_Text(pDX, IDC_CUST_CM_FIELD, m_cmScaleFactor);
}

BOOL MeaCustomUnitsPrefs::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    // Replace the CEdit controls with our numeric field controls.
    //
    m_pxScaleField.SubclassDlgItem(IDC_CUST_PX_FIELD, this);
    m_inScaleField.SubclassDlgItem(IDC_CUST_IN_FIELD, this);
    m_cmScaleField.SubclassDlgItem(IDC_CUST_CM_FIELD, this);

    m_pxScaleField.SetValueType(MeaNumberField::PosValues);
    m_inScaleField.SetValueType(MeaNumberField::PosValues);
    m_cmScaleField.SetValueType(MeaNumberField::PosValues);

    // Get the name and abbreviation field
    //
    m_nameField = GetDlgItem(IDC_CUST_NAME_FIELD);
    m_abbrevField = GetDlgItem(IDC_CUST_ABBREV_FIELD);

    // Get the buttons
    //
    m_precisionButton = GetDlgItem(IDC_CUST_PREC);
    m_pxButton = static_cast<CButton*>(GetDlgItem(IDC_CUST_PX));
    m_inButton = static_cast<CButton*>(GetDlgItem(IDC_CUST_IN));
    m_cmButton = static_cast<CButton*>(GetDlgItem(IDC_CUST_CM));

    // Get the labels
    //
    m_basisLabel = GetDlgItem(IDC_CUST_BASIS_LABEL);

    m_pxLabel = GetDlgItem(IDC_CUST_PX_LABEL);
    m_inLabel = GetDlgItem(IDC_CUST_IN_LABEL);
    m_cmLabel = GetDlgItem(IDC_CUST_CM_LABEL);

    m_pxAbbrev = GetDlgItem(IDC_CUST_PX_ABBREV);
    m_inAbbrev = GetDlgItem(IDC_CUST_IN_ABBREV);
    m_cmAbbrev = GetDlgItem(IDC_CUST_CM_ABBREV);

    EnableScaleFactor();

    return TRUE;
}


void MeaCustomUnitsPrefs::SetCheck(UINT buttonId) {
    MeaAssert(m_pxButton != nullptr);
    m_pxButton->SetCheck((buttonId == IDC_CUST_PX) ? 1 : 0);
    MeaAssert(m_inButton != nullptr);
    m_inButton->SetCheck((buttonId == IDC_CUST_IN) ? 1 : 0);
    MeaAssert(m_cmButton != nullptr);
    m_cmButton->SetCheck((buttonId == IDC_CUST_CM) ? 1 : 0);
}


BOOL MeaCustomUnitsPrefs::OnKillActive() {
    if (!Validate()) {
        return FALSE;
    }

    return CPropertyPage::OnKillActive();
}


BOOL MeaCustomUnitsPrefs::OnSetActive() {
    if (m_abbrevField != nullptr) {
        m_lockout = true;
        OnAbbrevChange();
        m_lockout = false;
    }

    return CPropertyPage::OnSetActive();
}


BOOL MeaCustomUnitsPrefs::OnApply() {
    if (!Validate()) {
        return FALSE;
    }

    return CPropertyPage::OnApply();
}


bool MeaCustomUnitsPrefs::Validate() {
    CString name;
    CString abbrev;

    MeaAssert(m_nameField != nullptr);
    m_nameField->GetWindowText(name);
    MeaAssert(m_abbrevField != nullptr);
    m_abbrevField->GetWindowText(abbrev);

    // The name and abbreviation fields must either both be empty
    // or contain text for the page to be valid.
    //
    if (name.IsEmpty() ^ abbrev.IsEmpty()) {
        AfxMessageBox(IDS_MEA_CUSTOM_NAME_WARNING, MB_OK | MB_ICONSTOP);
        return false;
    }

    double pxScale = 0, inScale = 0, cmScale = 0;

    m_pxScaleField.GetValue(pxScale);
    m_inScaleField.GetValue(inScale);
    m_cmScaleField.GetValue(cmScale);

    MeaNumberField* field = nullptr;

    if (pxScale <= 0.0) {
        field = &m_pxScaleField;
    } else if (inScale <= 0.0) {
        field = &m_inScaleField;
    } else if (cmScale <= 0.0) {
        field = &m_cmScaleField;
    }

    if (field != nullptr) {
        AfxMessageBox(IDS_MEA_CUSTOM_FACTOR_WARNING, MB_OK | MB_ICONSTOP);
        field->SetSel(0, -1);
        return false;
    }

    return true;
}


void MeaCustomUnitsPrefs::GetScaleFactor(MeaCustomUnits::ScaleBasis& scaleBasis, double& scaleFactor) const {
    scaleBasis = static_cast<MeaCustomUnits::ScaleBasis>(m_scaleBasis);

    switch (m_scaleBasis) {
    case MeaCustomUnits::PixelBasis:
        scaleFactor = m_pxScaleFactor;
        break;
    case MeaCustomUnits::InchBasis:
        scaleFactor = m_inScaleFactor;
        break;
    case MeaCustomUnits::CentimeterBasis:
        scaleFactor = m_cmScaleFactor;
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void MeaCustomUnitsPrefs::SetScaleFactor(MeaCustomUnits::ScaleBasis scaleBasis, double scaleFactor) {
    m_scaleBasis = scaleBasis;

    m_pxScaleFactor = 1.0;
    m_inScaleFactor = 1.0;
    m_cmScaleFactor = 1.0;

    switch (m_scaleBasis) {
    case MeaCustomUnits::PixelBasis:
        m_pxScaleFactor = scaleFactor;
        break;
    case MeaCustomUnits::InchBasis:
        m_inScaleFactor = scaleFactor;
        break;
    case MeaCustomUnits::CentimeterBasis:
        m_cmScaleFactor = scaleFactor;
        break;
    default:
        MeaAssert(false);
        break;
    }
}


void MeaCustomUnitsPrefs::EnableScaleFactor() {
    CString name;
    CString abbrev;

    MeaAssert(m_nameField != nullptr);
    m_nameField->GetWindowText(name);
    name.TrimLeft();
    name.TrimRight();

    MeaAssert(m_abbrevField != nullptr);
    m_abbrevField->GetWindowText(abbrev);
    abbrev.TrimLeft();
    abbrev.TrimRight();

    bool notEmpty = !name.IsEmpty() && !abbrev.IsEmpty();

    MeaAssert((m_basisLabel != nullptr) && (m_precisionButton != nullptr));
    m_basisLabel->EnableWindow(notEmpty);
    m_precisionButton->EnableWindow(notEmpty);

    MeaAssert((m_pxButton != nullptr) &&
              (m_pxLabel != nullptr) &&
              (m_pxAbbrev != nullptr));
    m_pxButton->EnableWindow(notEmpty);
    m_pxScaleField.EnableWindow(notEmpty);
    m_pxLabel->EnableWindow(notEmpty);
    m_pxAbbrev->EnableWindow(notEmpty);

    MeaAssert((m_inButton != nullptr) &&
              (m_inLabel != nullptr) &&
              (m_inAbbrev != nullptr));
    m_inButton->EnableWindow(notEmpty);
    m_inScaleField.EnableWindow(notEmpty);
    m_inLabel->EnableWindow(notEmpty);
    m_inAbbrev->EnableWindow(notEmpty);

    MeaAssert((m_cmButton != nullptr) &&
              (m_cmLabel != nullptr) &&
              (m_cmAbbrev != nullptr));
    m_cmButton->EnableWindow(notEmpty);
    m_cmScaleField.EnableWindow(notEmpty);
    m_cmLabel->EnableWindow(notEmpty);
    m_cmAbbrev->EnableWindow(notEmpty);
}


void MeaCustomUnitsPrefs::OnChange() {
    if (!m_lockout) {
        SetModified(TRUE);
    }
}


void MeaCustomUnitsPrefs::OnPixelChange() {
    if (!m_lockout) {
        SetCheck(IDC_CUST_PX);
    }
    OnChange();
}


void MeaCustomUnitsPrefs::OnInchChange() {
    if (!m_lockout) {
        SetCheck(IDC_CUST_IN);
    }
    OnChange();
}


void MeaCustomUnitsPrefs::OnCentimeterChange() {
    if (!m_lockout) {
        SetCheck(IDC_CUST_CM);
    }
    OnChange();
}


void MeaCustomUnitsPrefs::OnNameChange() {
    EnableScaleFactor();
    OnChange();
}


void MeaCustomUnitsPrefs::OnAbbrevChange() {
    CString label;

    MeaAssert(m_abbrevField != nullptr);
    m_abbrevField->GetWindowText(label);
    label.TrimLeft();
    label.TrimRight();

    if (label.IsEmpty()) {
        label.LoadString(IDS_MEA_CUSTOM_LABEL);
    }

    MeaAssert(m_pxAbbrev != nullptr);
    m_pxAbbrev->SetWindowText(label);
    MeaAssert(m_inAbbrev != nullptr);
    m_inAbbrev->SetWindowText(label);
    MeaAssert(m_cmAbbrev != nullptr);
    m_cmAbbrev->SetWindowText(label);

    EnableScaleFactor();

    OnChange();
}


void MeaCustomUnitsPrefs::OnClear() {
    CString name;
    CString abbrev;

    MeaAssert(m_nameField != nullptr);
    m_nameField->GetWindowText(name);
    MeaAssert(m_abbrevField != nullptr);
    m_abbrevField->GetWindowText(abbrev);

    if (!name.IsEmpty() || !abbrev.IsEmpty()) {
        m_name.Empty();
        m_abbrev.Empty();

        UpdateData(FALSE);
        OnAbbrevChange();
    }
}


void MeaCustomUnitsPrefs::OnPrecision() {
    MeaPreferences* sheet = static_cast<MeaPreferences*>(GetParent());
    MeaAssert(sheet != nullptr);

    sheet->ShowCustomPrecision();
}


void MeaCustomUnitsPrefs::OnPixelBasis() {
    OnChange();
}


void MeaCustomUnitsPrefs::OnInchBasis() {
    OnChange();
}


void MeaCustomUnitsPrefs::OnCentimeterBasis() {
    OnChange();
}
