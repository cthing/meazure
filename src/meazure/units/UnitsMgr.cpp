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
#include "UnitsMgr.h"
#include <meazure/resource.h>
#include <meazure/Messages.h>
#include <meazure/ui/ScreenMgr.h>
#include <meazure/utilities/StringUtils.h>
#include <cassert>


 //*************************************************************************
 // MeaResWarnDlg
 //*************************************************************************


class MeaResWarnDlg : public CDialog {

public:
    MeaResWarnDlg();

    virtual BOOL OnInitDialog();
};


MeaResWarnDlg::MeaResWarnDlg() : CDialog(IDD_RES_WARNING) {}

BOOL MeaResWarnDlg::OnInitDialog() {
    CStatic* wnd = static_cast<CStatic*>(GetDlgItem(IDC_MEA_WARNING));
    wnd->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION));

    return TRUE;
}


//*************************************************************************
// MeaDimWarnDlg
//*************************************************************************


class MeaDimWarnDlg : public CDialog {

public:
    MeaDimWarnDlg();

    virtual BOOL OnInitDialog();
};


MeaDimWarnDlg::MeaDimWarnDlg() : CDialog(IDD_DIM_WARNING) {}

BOOL MeaDimWarnDlg::OnInitDialog() {
    CStatic* wnd = static_cast<CStatic*>(GetDlgItem(IDC_MEA_WARNING));
    wnd->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION));

    return TRUE;
}


//*************************************************************************
// MeaUnitsMgr
//*************************************************************************


const double MeaUnitsMgr::kTickIncrements[] = { 10.0, 25.0, 50.0, 75.0, 100.0 };
const int MeaUnitsMgr::kNumTickIncrements = sizeof(kTickIncrements) / sizeof(*kTickIncrements);
const int MeaUnitsMgr::kMinSepPixels = 5;
const double MeaUnitsMgr::kMinSepInches = 0.1;


MeaUnitsMgr::MeaUnitsMgr(token) :
    MeaSingleton_T<MeaUnitsMgr>(),
    m_pixelUnits(MeaScreenMgr::Instance()),
    m_pointUnits(MeaScreenMgr::Instance()),
    m_twipUnits(MeaScreenMgr::Instance()),
    m_inchUnits(MeaScreenMgr::Instance()),
    m_cmUnits(MeaScreenMgr::Instance()),
    m_mmUnits(MeaScreenMgr::Instance()),
    m_picaUnits(MeaScreenMgr::Instance()),
    m_customUnits(MeaScreenMgr::Instance(), &MeaLinearUnitsLabel::ChangeLabel),
    m_currentLinearUnits(&m_pixelUnits),
    m_currentAngularUnits(&m_degreeUnits),
    m_haveWarned(kDefHaveWarned),
    m_majorTickCount(10) {
    m_linearUnitsMap[m_pixelUnits.GetUnitsId()] = &m_pixelUnits;
    m_linearUnitsMap[m_pointUnits.GetUnitsId()] = &m_pointUnits;
    m_linearUnitsMap[m_twipUnits.GetUnitsId()] = &m_twipUnits;
    m_linearUnitsMap[m_inchUnits.GetUnitsId()] = &m_inchUnits;
    m_linearUnitsMap[m_cmUnits.GetUnitsId()] = &m_cmUnits;
    m_linearUnitsMap[m_mmUnits.GetUnitsId()] = &m_mmUnits;
    m_linearUnitsMap[m_picaUnits.GetUnitsId()] = &m_picaUnits;
    m_linearUnitsMap[m_customUnits.GetUnitsId()] = &m_customUnits;

    m_angularUnitsMap[m_degreeUnits.GetUnitsId()] = &m_degreeUnits;
    m_angularUnitsMap[m_radianUnits.GetUnitsId()] = &m_radianUnits;
}

MeaUnitsMgr::~MeaUnitsMgr() {
    m_currentLinearUnits = nullptr;
    m_currentAngularUnits = nullptr;
}

void MeaUnitsMgr::SaveProfile(MeaProfile& profile) {
    profile.WriteStr(_T("LinearUnits"), GetLinearUnitsStr());
    profile.WriteStr(_T("AngularUnits"), GetAngularUnitsStr());
    profile.WriteBool(_T("InvertY"), IsInvertY());

    MeaFPoint pt = ConvertPos(GetOrigin());
    profile.WriteStr(_T("OriginX"), MeaStringUtils::DblToStr(pt.x));
    profile.WriteStr(_T("OriginY"), MeaStringUtils::DblToStr(pt.y));

    if (!profile.UserInitiated()) {
        profile.WriteBool(_T("HaveWarned"), m_haveWarned);
    }

    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->SaveProfile(profile);
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->SaveProfile(profile);
    }
}

void MeaUnitsMgr::LoadProfile(MeaProfile& profile) {
    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->LoadProfile(profile);
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->LoadProfile(profile);
    }

    if (!profile.UserInitiated()) {
        m_haveWarned = profile.ReadBool(_T("HaveWarned"), m_haveWarned);
    }

    if (profile.GetVersion() == 1) {
        SetLinearUnits(static_cast<MeaLinearUnitsId>(profile.ReadInt(_T("Units"), MeaPixelsId)));
    } else {
        SetLinearUnits(profile.ReadStr(_T("LinearUnits"), kDefLinearUnits));
    }
    SetAngularUnits(profile.ReadStr(_T("AngularUnits"), kDefAngularUnits));
    SetInvertY(profile.ReadBool(_T("InvertY"), kDefInvertY));

    MeaFPoint pt;
    pt.x = profile.ReadDbl(_T("OriginX"), 0.0);
    pt.y = profile.ReadDbl(_T("OriginY"), 0.0);
    SetOrigin(UnconvertPos(pt));
}

void MeaUnitsMgr::MasterReset() {
    m_haveWarned = kDefHaveWarned;

    SetLinearUnits(kDefLinearUnits);
    SetAngularUnits(kDefAngularUnits);

    CPoint origin;

    SetInvertY(kDefInvertY);
    SetOrigin(origin);

    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->MasterReset();
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->MasterReset();
    }
}

void MeaUnitsMgr::SetLinearUnits(MeaLinearUnitsId unitsId) {
    // Set the new units.
    //
    m_currentLinearUnits = (*m_linearUnitsMap.find(unitsId)).second;

    // Update all units labels to reflect new units.
    //
    for (auto label : m_linearLabelsList) {
        label->SetUnits(unitsId);
    }

    // Determine whether we need to warn the user
    // to calibrate the screen resolution. The criteria
    // for warning the user is:
    //
    // - Resolution is not already set manually
    // - User has not already been warned
    // - The screen size has changed since last run
    // - The current units requires use of the resolution
    //
    if (m_currentLinearUnits->RequiresRes()) {
        MeaScreenMgr& mgr = MeaScreenMgr::Instance();

        if (!m_haveWarned && (mgr.SizeChanged() || mgr.AnyOSRes())) {
            MeaResWarnDlg dlg;

            m_haveWarned = true;
            if (dlg.DoModal() == IDOK) {
                AfxGetMainWnd()->PostMessage(MeaShowCalPrefsMsg);
            }
        } else if (mgr.SizeChanged() && m_haveWarned) {
            MeaDimWarnDlg dlg;

            m_haveWarned = true;
            if (dlg.DoModal() == IDOK) {
                AfxGetMainWnd()->PostMessage(MeaShowCalPrefsMsg);
            }
        }
    }
}

void MeaUnitsMgr::SetLinearUnits(LPCTSTR unitsStr) {
    for (const auto& unitsEntry : m_linearUnitsMap) {
        if (unitsEntry.second->GetUnitsStr() == unitsStr) {
            SetLinearUnits(unitsEntry.first);
        }
    }
}

void MeaUnitsMgr::SetAngularUnits(MeaAngularUnitsId unitsId) {
    // Set the new units.
    //
    m_currentAngularUnits = (*m_angularUnitsMap.find(unitsId)).second;

    // Update angle labels to reflect new units.
    //
    for (auto label : m_angularLabelsList) {
        label->SetUnits(unitsId);
    }
}

void MeaUnitsMgr::SetAngularUnits(LPCTSTR unitsStr) {
    for (const auto& unitsEntry : m_angularUnitsMap) {
        if (unitsEntry.second->GetUnitsStr() == unitsStr) {
            SetAngularUnits(unitsEntry.first);
        }
    }
}

MeaFSize MeaUnitsMgr::GetWidthHeight(const POINT& p1, const POINT& p2) const {
    CPoint np1(p1), np2(p2);

    if (np1.x < np2.x) {
        np2.x++;
    } else {
        np1.x++;
    }

    if (np1.y < np2.y) {
        np2.y++;
    } else {
        np1.y++;
    }

    MeaFPoint cp1 = ConvertCoord(np1);
    MeaFPoint cp2 = ConvertCoord(np2);

    return MeaFSize(fabs(cp1.x - cp2.x), fabs(cp1.y - cp2.y));
}

MeaFSize MeaUnitsMgr::GetMinorIncr(const RECT& rect) const {
    MeaScreenMgr& smgr = MeaScreenMgr::Instance();

    // We want to ensure a minimum resolution-independent
    // separation between the minor ticks. Start by converting
    // the resolution-independent minimum separation to pixels.
    //
    MeaFSize res = smgr.GetScreenRes(smgr.GetScreenIter(rect));
    SIZE sepPixels = ConvertToPixels(MeaInchesId, res, kMinSepInches, kMinSepPixels);

    // Convert the minimum tick separation to the current units.
    //
    MeaFSize sepUnits = m_currentLinearUnits->FromPixels(res) * sepPixels;

    // The object is to find a standard minor increment (e.g. 10, 25)
    // that is the closest to the minimum increment but larger than
    // or equal to it. To achieve this, use the log of the separation
    // to normalize the separation value into a standard range
    // (i.e. 10-100).
    //
    MeaFSize sepPower = log10(sepUnits);
    MeaFSize delta = 1.0 - floor(sepPower);

    sepPower += delta;

    MeaFSize increment;
    int idx;
    int count = kNumTickIncrements - 1;

    for (idx = 0; (idx < count) && (log10(kTickIncrements[idx]) < sepPower.cx); idx++) {
        ;
    }
    increment.cx = kTickIncrements[idx] * pow(10.0, -delta.cx);

    for (idx = 0; (idx < count) && (log10(kTickIncrements[idx]) < sepPower.cy); idx++) {
        ;
    }
    increment.cy = kTickIncrements[idx] * pow(10.0, -delta.cy);

    return increment;
}

MeaLengthUnitsLabel* MeaUnitsMgr::NewLengthUnitsLabel() {
    MeaLengthUnitsLabel* label = new MeaLengthUnitsLabel();
    m_linearLabelsList.push_back(label);
    return label;
}

MeaAngularUnitsLabel* MeaUnitsMgr::NewAngleUnitsLabel() {
    MeaAngularUnitsLabel* label = new MeaAngularUnitsLabel();
    m_angularLabelsList.push_back(label);
    return label;
}

MeaAreaUnitsLabel* MeaUnitsMgr::NewAreaUnitsLabel() {
    MeaAreaUnitsLabel* label = new MeaAreaUnitsLabel();
    m_linearLabelsList.push_back(label);
    return label;
}

MeaResUnitsLabel* MeaUnitsMgr::NewResUnitsLabel() {
    MeaResUnitsLabel* label = new MeaResUnitsLabel();
    m_linearLabelsList.push_back(label);
    return label;
}

void MeaUnitsMgr::DeleteUnitsLabel(MeaUnitsLabel* label) {
    assert(label != nullptr);

    // Cannot tell which list the label is from so attempt
    // to remove it from both. It is a benign op under
    // these circumstances as far as the list is concerned.
    //
    m_linearLabelsList.remove(static_cast<MeaLinearUnitsLabel*>(label));
    m_angularLabelsList.remove(static_cast<MeaAngularUnitsLabel*>(label));

    delete label;
}
