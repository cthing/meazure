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
#include "CalibrationPrefs.h"
#include "Preferences.h"
#include <meazure/resource.h>
#include <meazure/utilities/NumericUtils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(MeaCalibrationPrefs, CPropertyPage)
    ON_MESSAGE(MeaCaliperPositionMsg, OnCaliperChange)
    ON_BN_CLICKED(IDC_AUTO_RES, OnAutoRes)
    ON_BN_CLICKED(IDC_MANUAL_RES, OnManRes)
    ON_BN_CLICKED(IDC_CAL_INCHES, OnCalInches)
    ON_BN_CLICKED(IDC_CAL_CM, OnCalCm)
    ON_EN_CHANGE(IDC_CAL_RX_FIELD, OnRxFieldChange)
    ON_EN_CHANGE(IDC_CAL_RY_FIELD, OnRyFieldChange)
    ON_EN_CHANGE(IDC_CAL_W_FIELD, OnWFieldChange)
    ON_EN_CHANGE(IDC_CAL_H_FIELD, OnHFieldChange)
END_MESSAGE_MAP()


MeaCalibrationPrefs::MeaCalibrationPrefs(const MeaScreenProvider& screenProvider) :
    CPropertyPage(IDD_PREF_CALIBRATION), m_lockoutId(0), m_inDDX(false), m_screenProvider(screenProvider) {}

MeaCalibrationPrefs::~MeaCalibrationPrefs() {}

void MeaCalibrationPrefs::DoDataExchange(CDataExchange* pDX) {
    m_inDDX = true;

    CPropertyPage::DoDataExchange(pDX);

    UpdateCurrentScreen();
    Screen& screen = GetScreen();

    // Perform DDX for all items except the locked out item.
    //
    if (m_lockoutId != IDC_AUTO_RES) {
        DDX_Radio(pDX, IDC_AUTO_RES, screen.m_resMode);
    }
    if (m_lockoutId != IDC_CAL_INCHES) {
        DDX_Radio(pDX, IDC_CAL_INCHES, screen.m_resUnits);
    }
    if (m_lockoutId != IDC_CAL_RX_FIELD) {
        DDX_ResText(pDX, IDC_CAL_RX_FIELD, screen.m_res.cx);
    }
    if (m_lockoutId != IDC_CAL_RY_FIELD) {
        DDX_ResText(pDX, IDC_CAL_RY_FIELD, screen.m_res.cy);
    }
    if (m_lockoutId != IDC_CAL_W_FIELD) {
        DDX_DimText(pDX, IDC_CAL_W_FIELD, screen.m_size.cx);
    }
    if (m_lockoutId != IDC_CAL_H_FIELD) {
        DDX_DimText(pDX, IDC_CAL_H_FIELD, screen.m_size.cy);
    }

    m_inDDX = false;
}

bool MeaCalibrationPrefs::LockedUpdateData(UINT lockoutId, bool bSaveAndValidate) {
    m_lockoutId = lockoutId;
    bool ret = UpdateData(bSaveAndValidate) ? true : false;
    m_lockoutId = 0;

    return ret;
}

BOOL MeaCalibrationPrefs::OnInitDialog() {
    CPropertyPage::OnInitDialog();

    // Set the styles on the fields
    //
    m_rxField.SetValueType(MeaNumberField::PosValues);
    m_ryField.SetValueType(MeaNumberField::PosValues);
    m_wField.SetValueType(MeaNumberField::PosValues);
    m_hField.SetValueType(MeaNumberField::PosValues);

    // Replace the CEdit controls with our field control.
    //
    m_rxField.SubclassDlgItem(IDC_CAL_RX_FIELD, this);
    m_ryField.SubclassDlgItem(IDC_CAL_RY_FIELD, this);
    m_wField.SubclassDlgItem(IDC_CAL_W_FIELD, this);
    m_hField.SubclassDlgItem(IDC_CAL_H_FIELD, this);

    // Replace the CProgressCtrl controls with the ruler slider.
    //
    m_rxSlider.SubclassDlgItem(IDC_CAL_RX_SLIDER, this);
    m_rySlider.SubclassDlgItem(IDC_CAL_RY_SLIDER, this);

    return TRUE;
}

BOOL MeaCalibrationPrefs::OnSetActive() {
    CPropertyPage::OnSetActive();

    UpdateCurrentScreen();
    UpdateView();

    return TRUE;
}

BOOL MeaCalibrationPrefs::OnKillActive() {
    return Validate() ? CPropertyPage::OnKillActive() : FALSE;
}

void MeaCalibrationPrefs::DDX_ResText(CDataExchange* pDX, int nIDC, double& resValue) {
    if (pDX->m_bSaveAndValidate) {
        double value;
        DDX_Text(pDX, nIDC, value);
        resValue = IsMetric() ? value * 2.54 : value;
    } else {
        double value = IsMetric() ? resValue / 2.54 : resValue;
        DDX_Text(pDX, nIDC, value);
    }
}

void MeaCalibrationPrefs::DDX_DimText(CDataExchange* pDX, int nIDC, double& dimValue) {
    if (pDX->m_bSaveAndValidate) {
        double value;
        DDX_Text(pDX, nIDC, value);
        dimValue = IsMetric() ? value / 2.54 : value;
    } else {
        double value = IsMetric() ? dimValue * 2.54 : dimValue;
        DDX_Text(pDX, nIDC, value);
    }
}

bool MeaCalibrationPrefs::Validate() {
    const CRect& screenRect = m_screenProvider.GetScreenRect(m_currentIter);
    Screen& screen = GetScreen();
    bool mr;
    MeaFSize r;

    m_screenProvider.GetScreenRes(m_currentIter, mr, r);

    if (screen.m_resMode == AutoRes) {
        if (!ValidateField(IDC_CAL_RX_FIELD)) {
            screen.m_res.cx = r.cx;
            UpdateData(FALSE);
        }
        if (!ValidateField(IDC_CAL_RY_FIELD)) {
            screen.m_res.cy = r.cy;
            UpdateData(FALSE);
        }
        if (!ValidateField(IDC_CAL_W_FIELD)) {
            screen.m_size.cx = screenRect.Width() / screen.m_res.cx;
            UpdateData(FALSE);
        }
        if (!ValidateField(IDC_CAL_H_FIELD)) {
            screen.m_size.cy = screenRect.Height() / screen.m_res.cy;
            UpdateData(FALSE);
        }
    } else {
        if (!ValidateField(IDC_CAL_RX_FIELD) ||
                !ValidateField(IDC_CAL_RY_FIELD) ||
                !ValidateField(IDC_CAL_W_FIELD) ||
                !ValidateField(IDC_CAL_H_FIELD)) {
            return false;
        }
    }
    return true;
}

bool MeaCalibrationPrefs::ValidateField(UINT id) {
    double v = 0.0;

    if (GetFieldValue(id, v)) {
        return true;
    }

    if (GetScreen().m_resMode == ManualRes) {
        AfxMessageBox(IDS_MEA_FIELD_VALUE, MB_OK | MB_ICONSTOP);
        CEdit* edit = static_cast<CEdit*>(GetDlgItem(id));
        edit->SetFocus();
        edit->SetSel(0, -1);
    }

    return false;
}

void MeaCalibrationPrefs::SetResMode(ResolutionMode resMode) {
    Screen& screen = GetScreen();

    screen.m_resMode = resMode;
    bool enable = (screen.m_resMode == ManualRes);

    GetDlgItem(IDC_CAL_INSTR)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_INSTR2)->EnableWindow(enable);

    GetDlgItem(IDC_CAL_INCHES)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_CM)->EnableWindow(enable);

    GetDlgItem(IDC_CAL_RX_LABEL)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_RX_FIELD)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_RY_LABEL)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_RY_FIELD)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_RES_LABEL)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_W_LABEL)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_W_FIELD)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_H_LABEL)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_H_FIELD)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_DIM_LABEL)->EnableWindow(enable);

    GetDlgItem(IDC_CAL_RX_SLIDER)->EnableWindow(enable);
    GetDlgItem(IDC_CAL_RY_SLIDER)->EnableWindow(enable);
}

void MeaCalibrationPrefs::SetResUnits(ResolutionUnits resUnits) {
    Screen& screen = GetScreen();

    screen.m_resUnits = resUnits;

    CString resLabel, dimLabel, instrLabel;

    if (screen.m_resUnits == UseInches) {
        resLabel.LoadString(IDS_MEA_RES_INCHES);
        dimLabel.LoadString(IDS_MEA_INCHES);
        instrLabel.LoadString(IDS_MEA_IN_INSTR);
    } else {
        resLabel.LoadString(IDS_MEA_RES_CM);
        dimLabel.LoadString(IDS_MEA_CM);
        instrLabel.LoadString(IDS_MEA_CM_INSTR);
    }

    GetDlgItem(IDC_CAL_RES_LABEL)->SetWindowText(resLabel);
    GetDlgItem(IDC_CAL_DIM_LABEL)->SetWindowText(dimLabel);
    GetDlgItem(IDC_CAL_INSTR2)->SetWindowText(instrLabel);
}

void MeaCalibrationPrefs::UpdateSliders() {
    Screen& screen = GetScreen();

    UpdateSlider(IDC_CAL_RX_SLIDER, screen.m_res.cx);
    UpdateSlider(IDC_CAL_RY_SLIDER, screen.m_res.cy);
}

void MeaCalibrationPrefs::UpdateSlider(UINT sliderId, double res) {
    MeaRulerSlider* slider = static_cast<MeaRulerSlider*>(GetDlgItem(sliderId));
    if (slider != nullptr) {
        int resValue = static_cast<int>(IsMetric() ? 2.0 * res / 2.54 : res);
        slider->SetSliderPos(resValue);
    }
}

void MeaCalibrationPrefs::OnAutoRes() {
    OnChange();
    SetResMode(AutoRes);
}

void MeaCalibrationPrefs::OnManRes() {
    OnChange();
    SetResMode(ManualRes);
}

void MeaCalibrationPrefs::OnCalInches() {
    SetResUnits(UseInches);

    UpdateData(FALSE);
    UpdateSliders();
}

void MeaCalibrationPrefs::OnCalCm() {
    SetResUnits(UseCentimeters);

    UpdateData(FALSE);
    UpdateSliders();
}

void MeaCalibrationPrefs::OnRxFieldChange() {
    Screen& screen = GetScreen();

    if (GetFieldValue(IDC_CAL_RX_FIELD, screen.m_res.cx)) {
        const CRect& screenRect = m_screenProvider.GetScreenRect(m_currentIter);

        OnChange();
        UpdateSlider(IDC_CAL_RX_SLIDER, screen.m_res.cx);
        screen.m_size.cx = screenRect.Width() / screen.m_res.cx;
        LockedUpdateData(IDC_CAL_RX_FIELD, false);
    }
}

void MeaCalibrationPrefs::OnRyFieldChange() {
    Screen& screen = GetScreen();

    if (GetFieldValue(IDC_CAL_RY_FIELD, screen.m_res.cy)) {
        const CRect& screenRect = m_screenProvider.GetScreenRect(m_currentIter);

        OnChange();
        UpdateSlider(IDC_CAL_RY_SLIDER, screen.m_res.cy);
        screen.m_size.cy = screenRect.Height() / screen.m_res.cy;
        LockedUpdateData(IDC_CAL_RY_SLIDER, false);
    }
}

void MeaCalibrationPrefs::OnWFieldChange() {
    Screen& screen = GetScreen();

    if (GetFieldValue(IDC_CAL_W_FIELD, screen.m_size.cx)) {
        const CRect& screenRect = m_screenProvider.GetScreenRect(m_currentIter);

        OnChange();
        screen.m_res.cx = screenRect.Width() / screen.m_size.cx;
        UpdateSlider(IDC_CAL_RX_SLIDER, screen.m_res.cx);
        LockedUpdateData(IDC_CAL_RX_SLIDER, false);
    }
}

void MeaCalibrationPrefs::OnHFieldChange() {
    Screen& screen = GetScreen();

    if (GetFieldValue(IDC_CAL_H_FIELD, screen.m_size.cy)) {
        const CRect& screenRect = m_screenProvider.GetScreenRect(m_currentIter);

        OnChange();
        screen.m_res.cy = screenRect.Height() / screen.m_size.cy;
        UpdateSlider(IDC_CAL_RY_SLIDER, screen.m_res.cy);
        LockedUpdateData(IDC_CAL_RY_SLIDER, false);
    }
}

bool MeaCalibrationPrefs::GetFieldValue(UINT fieldId, double& value) {
    CWnd* field = GetDlgItem(fieldId);

    if (field != nullptr) {
        CString str;

        field->GetWindowText(str);
        if (!str.IsEmpty()) {
            double v = _tcstod(str, nullptr);
            if (IsMetric()) {
                if ((fieldId == IDC_CAL_H_FIELD) || (fieldId == IDC_CAL_W_FIELD)) {
                    v /= 2.54;
                } else {
                    v *= 2.54;
                }
            }
            if ((v > DBL_EPSILON) && !MeaNumericUtils::IsEqualF(value, v)) {
                value = v;
                return true;
            }
        }
    }

    return false;
}

void MeaCalibrationPrefs::OnChange() {
    SetModified(TRUE);
}

LRESULT MeaCalibrationPrefs::OnCaliperChange(WPARAM pos, LPARAM id) {
    Screen& screen = GetScreen();
    const CRect& screenRect = m_screenProvider.GetScreenRect(m_currentIter);
    double resValue;

    OnChange();

    resValue = IsMetric() ? pos * 2.54 / 2.0 : pos;
    if (id == IDC_CAL_RX_SLIDER) {
        screen.m_res.cx = resValue;
        screen.m_size.cx = screenRect.Width() / screen.m_res.cx;
    } else {
        screen.m_res.cy = resValue;
        screen.m_size.cy = screenRect.Height() / screen.m_res.cy;
    }

    UpdateData(FALSE);

    return TRUE;
}

void MeaCalibrationPrefs::OnMove(int /*x*/, int /*y*/) {
    UpdateCurrentScreen();
}

void MeaCalibrationPrefs::UpdateView() {
    Screen& screen = GetScreen();

    SetResMode(static_cast<ResolutionMode>(screen.m_resMode));
    SetResUnits(static_cast<ResolutionUnits>(screen.m_resUnits));

    const CRect& rect = m_screenProvider.GetScreenRect(m_currentIter);

    screen.m_size.cx = rect.Width() / screen.m_res.cx;
    screen.m_size.cy = rect.Height() / screen.m_res.cy;

    UpdateData(FALSE);
    UpdateSliders();
}

void MeaCalibrationPrefs::UpdateCurrentScreen() {
    if (GetSafeHwnd() == nullptr) {
        return;
    }

    RECT winRect;
    GetWindowRect(&winRect);

    m_currentIter = m_screenProvider.GetScreenIter(winRect);

    CWnd* frame = GetDlgItem(IDC_CALRES_TITLE);
    if (frame != nullptr) {
        CString title;
        title.Format(IDS_MEA_CALRES_TITLE, static_cast<PCTSTR>(m_screenProvider.GetScreenName(m_currentIter)));
        frame->SetWindowText(title);

        MeaPreferences* sheet = static_cast<MeaPreferences*>(GetParent());
        assert(sheet != nullptr);

        if (sheet->GetActivePage() == this) {
            UpdateView();
        }
    }
}

BOOL MeaCalibrationPrefs::UpdateData(BOOL bSaveAndValidate) {
    return m_inDDX ? TRUE : CPropertyPage::UpdateData(bSaveAndValidate);
}
