/*
 * Copyright 2001, 2004, 2011 C Thing Software
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
#include "MeaAssert.h"
#include "Units.h"
#include "Layout.h"
#include "Messages.h"
#include "ScreenMgr.h"


//*************************************************************************
// MeaResWarnDlg
//*************************************************************************


class MeaResWarnDlg : public CDialog
{
public:
    MeaResWarnDlg();

    virtual BOOL OnInitDialog();
};


MeaResWarnDlg::MeaResWarnDlg() : CDialog(IDD_RES_WARNING)
{
}


BOOL MeaResWarnDlg::OnInitDialog()
{
    CStatic *wnd = static_cast<CStatic*>(GetDlgItem(IDC_MEA_WARNING));
    wnd->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION));

    return TRUE;
}


//*************************************************************************
// MeaDimWarnDlg
//*************************************************************************


class MeaDimWarnDlg : public CDialog
{
public:
    MeaDimWarnDlg();

    virtual BOOL OnInitDialog();
};


MeaDimWarnDlg::MeaDimWarnDlg() : CDialog(IDD_DIM_WARNING)
{
}


BOOL MeaDimWarnDlg::OnInitDialog()
{
    CStatic *wnd = static_cast<CStatic*>(GetDlgItem(IDC_MEA_WARNING));
    wnd->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION));

    return TRUE;
}

 
//*************************************************************************
// MeaUnitsMgr
//*************************************************************************


MEA_SINGLETON_DEF(MeaUnitsMgr);


// Defaults
const bool      MeaUnitsMgr::kDefHaveWarned     = false;
const LPCTSTR   MeaUnitsMgr::kDefLinearUnits    = _T("px");
const LPCTSTR   MeaUnitsMgr::kDefAngularUnits   = _T("deg");
const bool      MeaUnitsMgr::kDefInvertY        = false;


MeaUnitsMgr::MeaUnitsMgr() : MeaSingleton_T<MeaUnitsMgr>(),
    m_currentLinearUnits(&m_pixelUnits),
    m_currentAngularUnits(&m_degreeUnits),
    m_haveWarned(kDefHaveWarned)
{
    m_linearUnitsMap[m_pixelUnits.GetUnitsId()]     = &m_pixelUnits;
    m_linearUnitsMap[m_pointUnits.GetUnitsId()]     = &m_pointUnits;
    m_linearUnitsMap[m_twipUnits.GetUnitsId()]      = &m_twipUnits;
    m_linearUnitsMap[m_inchUnits.GetUnitsId()]      = &m_inchUnits;
    m_linearUnitsMap[m_cmUnits.GetUnitsId()]        = &m_cmUnits;
    m_linearUnitsMap[m_mmUnits.GetUnitsId()]        = &m_mmUnits;
    m_linearUnitsMap[m_picaUnits.GetUnitsId()]      = &m_picaUnits;
    m_linearUnitsMap[m_customUnits.GetUnitsId()]    = &m_customUnits;

    m_angularUnitsMap[m_degreeUnits.GetUnitsId()] = &m_degreeUnits;
    m_angularUnitsMap[m_radianUnits.GetUnitsId()] = &m_radianUnits;
}


MeaUnitsMgr::~MeaUnitsMgr()
{
    m_currentLinearUnits = NULL;
    m_currentAngularUnits = NULL;
}


void MeaUnitsMgr::SaveProfile(MeaProfile& profile)
{
    profile.WriteStr(_T("LinearUnits"), GetLinearUnitsStr());
    profile.WriteStr(_T("AngularUnits"), GetAngularUnitsStr());
    profile.WriteBool(_T("InvertY"), GetInvertY());

    FPOINT pt = ConvertPos(GetOrigin());
    profile.WriteStr(_T("OriginX"), MeaUtils::DblToStr(pt.x));
    profile.WriteStr(_T("OriginY"), MeaUtils::DblToStr(pt.y));

    if (!profile.UserInitiated()) {
        profile.WriteBool(_T("HaveWarned"), m_haveWarned);
    }

    LinearUnitsMap::const_iterator liter;
    for (liter = m_linearUnitsMap.begin(); liter != m_linearUnitsMap.end(); ++liter) {
        (*liter).second->SaveProfile(profile);
    }

    AngularUnitsMap::const_iterator aiter;
    for (aiter = m_angularUnitsMap.begin(); aiter != m_angularUnitsMap.end(); ++aiter) {
        (*aiter).second->SaveProfile(profile);
    }
}


void MeaUnitsMgr::LoadProfile(MeaProfile& profile)
{
    LinearUnitsMap::const_iterator liter;
    for (liter = m_linearUnitsMap.begin(); liter != m_linearUnitsMap.end(); ++liter) {
        (*liter).second->LoadProfile(profile);
    }

    AngularUnitsMap::const_iterator aiter;
    for (aiter = m_angularUnitsMap.begin(); aiter != m_angularUnitsMap.end(); ++aiter) {
        (*aiter).second->LoadProfile(profile);
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

    FPOINT pt;
    pt.x = profile.ReadDbl(_T("OriginX"), 0.0);
    pt.y = profile.ReadDbl(_T("OriginY"), 0.0);
    SetOrigin(UnconvertPos(pt));
}


void MeaUnitsMgr::MasterReset()
{
    m_haveWarned = kDefHaveWarned;
    
    SetLinearUnits(kDefLinearUnits);
    SetAngularUnits(kDefAngularUnits);

    POINT origin = { 0, 0 };

    SetInvertY(kDefInvertY);
    SetOrigin(origin);

    LinearUnitsMap::const_iterator liter;
    for (liter = m_linearUnitsMap.begin(); liter != m_linearUnitsMap.end(); ++liter) {
        (*liter).second->MasterReset();
    }

    AngularUnitsMap::const_iterator aiter;
    for (aiter = m_angularUnitsMap.begin(); aiter != m_angularUnitsMap.end(); ++aiter) {
        (*aiter).second->MasterReset();
    }
}


void MeaUnitsMgr::SetLinearUnits(MeaLinearUnitsId unitsId)
{
    // Set the new units.
    //
    m_currentLinearUnits = (*m_linearUnitsMap.find(unitsId)).second;

    // Update all units labels to reflect new units.
    //
    LinearLabelsList::const_iterator iter;
    
    for (iter = m_linearLabelsList.begin(); iter != m_linearLabelsList.end(); ++iter) {
        (*iter)->SetUnits(unitsId);
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


void MeaUnitsMgr::SetLinearUnits(LPCTSTR unitsStr)
{
    LinearUnitsMap::const_iterator iter;
    
    for (iter = m_linearUnitsMap.begin(); iter != m_linearUnitsMap.end(); ++iter) {
        if ((*iter).second->GetUnitsStr() == unitsStr) {
            SetLinearUnits((*iter).first);
        }
    }
}


void MeaUnitsMgr::SetAngularUnits(MeaAngularUnitsId unitsId)
{
    // Set the new units.
    //
    m_currentAngularUnits = (*m_angularUnitsMap.find(unitsId)).second;

    // Update angle labels to reflect new units.
    //
    AngularLabelsList::const_iterator iter;
    
    for (iter = m_angularLabelsList.begin(); iter != m_angularLabelsList.end(); ++iter) {
        (*iter)->SetUnits(unitsId);
    }
}


void MeaUnitsMgr::SetAngularUnits(LPCTSTR unitsStr)
{
    AngularUnitsMap::const_iterator iter;
    
    for (iter = m_angularUnitsMap.begin(); iter != m_angularUnitsMap.end(); ++iter) {
        if ((*iter).second->GetUnitsStr() == unitsStr) {
            SetAngularUnits((*iter).first);
        }
    }
}


FSIZE MeaUnitsMgr::GetWidthHeight(const POINT& p1, const POINT& p2) const
{
    FSIZE wh;
    POINT np1, np2;

    np1 = p1;
    np2 = p2;
    
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

    MeaUnitsMgr& units = MeaUnitsMgr::Instance();
    
    FPOINT cp1 = units.ConvertCoord(np1);
    FPOINT cp2 = units.ConvertCoord(np2);

    wh.cx = fabs(cp1.x - cp2.x);
    wh.cy = fabs(cp1.y - cp2.y);

    return wh;
}


MeaLengthUnitsLabel* MeaUnitsMgr::NewLengthUnitsLabel()
{
    MeaLengthUnitsLabel* label = new MeaLengthUnitsLabel();
    m_linearLabelsList.push_back(label);
    return label;
}

    
MeaAngularUnitsLabel* MeaUnitsMgr::NewAngleUnitsLabel()
{
    MeaAngularUnitsLabel* label = new MeaAngularUnitsLabel();
    m_angularLabelsList.push_back(label);
    return label;
}
    
    
MeaAreaUnitsLabel* MeaUnitsMgr::NewAreaUnitsLabel()
{
    MeaAreaUnitsLabel* label = new MeaAreaUnitsLabel();
    m_linearLabelsList.push_back(label);
    return label;
}
    
    
MeaResUnitsLabel* MeaUnitsMgr::NewResUnitsLabel()
{
    MeaResUnitsLabel* label = new MeaResUnitsLabel();
    m_linearLabelsList.push_back(label);
    return label;
}


void MeaUnitsMgr::DeleteUnitsLabel(MeaUnitsLabel *label)
{
    MeaAssert(label != NULL);
    
    // Cannot tell which list the label is from so attempt
    // to remove it from both. It is a benign op under
    // these circumstances as far as the list is concerned.
    //
    m_linearLabelsList.remove(static_cast<MeaLinearUnitsLabel*>(label));
    m_angularLabelsList.remove(static_cast<MeaAngularUnitsLabel*>(label));

    delete label;
}


/**************************************************************************
 * MeaUnitsLabel
 **************************************************************************/

MeaUnitsLabel::MeaUnitsLabel(UINT format, LabelMap& labels, int& width) :
    MeaLabel(),
    m_labelsRef(labels),
    m_widthRef(width),
    m_format(format)
{
}


MeaUnitsLabel::~MeaUnitsLabel()
{
}


bool MeaUnitsLabel::Create(UINT unitsId, DWORD style, const POINT& topLeft,
                           CWnd* parentWnd, UINT id)
{
    if (!MeaLabel::Create(ConstructLabel(unitsId), style, topLeft, parentWnd, id)) {
        return false;
    }

    return SetSize();
}

    
bool MeaUnitsLabel::SetSize()
{
    if (m_widthRef < 0) {
        CDC *dc = GetDC();
        if (dc == NULL) {
            return false;
        }

        if (dc->SelectStockObject(DEFAULT_GUI_FONT) == NULL) {
            ReleaseDC(dc);
            return false;
        }

        // Get the bounding box for each label setting the width to the max.
        //
        LabelMap::const_iterator iter;

        for (iter = m_labelsRef.begin(); iter != m_labelsRef.end(); ++iter) {
            CSize sz = dc->GetTextExtent(ConstructLabel((*iter).first));
            if (sz.cx > m_widthRef) {
                m_widthRef = sz.cx;
            }
        }

        ReleaseDC(dc);
    }

    CRect rect;

    GetWindowRect(&rect);
    MeaLayout::SetWindowSize(*this, m_widthRef, rect.Height());

    return true;
}


CString MeaUnitsLabel::ConstructLabel(int unitsId) const
{
    CString labelStr;

    labelStr.Format(m_format, static_cast<LPCTSTR>(m_labelsRef[unitsId]));
    return labelStr;
}


//*************************************************************************
// MeaLinearUnitsLabel
//*************************************************************************

MeaUnitsLabel::LabelMap MeaLinearUnitsLabel::m_labels;


MeaLinearUnitsLabel::MeaLinearUnitsLabel(UINT format, int& width) :
    MeaUnitsLabel(format, m_labels, width)
{
    if (m_labels.empty()) {
        m_labels[MeaPixelsId].LoadString(IDS_MEA_PIXELS);
        m_labels[MeaPointsId].LoadString(IDS_MEA_POINTS);
        m_labels[MeaTwipsId].LoadString(IDS_MEA_TWIPS);
        m_labels[MeaInchesId].LoadString(IDS_MEA_INCHES);
        m_labels[MeaCentimetersId].LoadString(IDS_MEA_CM);
        m_labels[MeaMillimetersId].LoadString(IDS_MEA_MM);
        m_labels[MeaPicasId].LoadString(IDS_MEA_PICAS);
        m_labels[MeaCustomId].LoadString(IDS_MEA_CUSTOM_UNITS);
    }
}


MeaLinearUnitsLabel::~MeaLinearUnitsLabel()
{
}


void MeaLinearUnitsLabel::SetUnits(MeaLinearUnitsId unitsId)
{
    // For pixels (i.e. resolution independent units), report
    // the resolution not as px/px but as px/in.
    //
    if (unitsId == MeaPixelsId && GetFormat() == IDS_MEA_RES_LABEL_FMT) {
        unitsId = MeaInchesId;
    }

    SetWindowText(ConstructLabel(unitsId));
}


//*************************************************************************
// MeaAngularUnitsLabel
//*************************************************************************

MeaUnitsLabel::LabelMap MeaAngularUnitsLabel::m_labels;
int                     MeaAngularUnitsLabel::m_labelWidth = -1;


MeaAngularUnitsLabel::MeaAngularUnitsLabel() :
    MeaUnitsLabel(IDS_MEA_DEF_LABEL_FMT, m_labels, m_labelWidth)
{
    if (m_labels.empty()) {
        m_labels[MeaDegreesId].LoadString(IDS_MEA_DEGREES);
        m_labels[MeaRadiansId].LoadString(IDS_MEA_RADIANS);
    }
}


MeaAngularUnitsLabel::~MeaAngularUnitsLabel()
{
}


void MeaAngularUnitsLabel::SetUnits(MeaAngularUnitsId anglesId)
{
    SetWindowText(ConstructLabel(anglesId));
}


//*************************************************************************
// MeaLengthUnitsLabel
//*************************************************************************

int MeaLengthUnitsLabel::m_labelWidth = -1;

MeaLengthUnitsLabel::MeaLengthUnitsLabel() :
    MeaLinearUnitsLabel(IDS_MEA_DEF_LABEL_FMT, m_labelWidth)
{
}


MeaLengthUnitsLabel::~MeaLengthUnitsLabel()
{
}


//*************************************************************************
// MeaAreaUnitsLabel
//*************************************************************************

int MeaAreaUnitsLabel::m_labelWidth = -1;


MeaAreaUnitsLabel::MeaAreaUnitsLabel() :
    MeaLinearUnitsLabel(IDS_MEA_AREA_LABEL_FMT, m_labelWidth)
{
}


MeaAreaUnitsLabel::~MeaAreaUnitsLabel()
{
}


//*************************************************************************
// MeaResUnitsLabel
//*************************************************************************

int MeaResUnitsLabel::m_labelWidth = -1;


MeaResUnitsLabel::MeaResUnitsLabel() :
    MeaLinearUnitsLabel(IDS_MEA_RES_LABEL_FMT, m_labelWidth)
{
}


MeaResUnitsLabel::~MeaResUnitsLabel()
{
}


//*************************************************************************
// MeaUnits
//*************************************************************************


const double MeaUnits::kPI = 3.14159265359;


MeaUnits::MeaUnits(LPCTSTR unitsStr) : m_unitsStr(unitsStr)
{
}


MeaUnits::~MeaUnits()
{
}


void MeaUnits::SaveProfile(MeaProfile& profile)
{
    if (!profile.UserInitiated()) {
        SavePrecision(profile);
    }
}


void MeaUnits::LoadProfile(MeaProfile& profile)
{
    if (!profile.UserInitiated()) {
        LoadPrecision(profile);
    }
}


void MeaUnits::SavePrecision(MeaProfile& profile)
{
    unsigned int i;

    for (i = 0; i < m_displayPrecisions.size(); i++) {
        CString tag;

        tag.Format(_T("Precision-%s-%s"),
                    static_cast<LPCTSTR>(m_displayPrecisionNames[i]),
                    static_cast<LPCTSTR>(m_unitsStr));
        profile.WriteInt(tag, m_displayPrecisions[i]);
    }
}


void MeaUnits::LoadPrecision(MeaProfile& profile)
{
    unsigned int i;

    for (i = 0; i < m_displayPrecisions.size(); i++) {
        CString tag;

        tag.Format(_T("Precision-%s-%s"),
                    static_cast<LPCTSTR>(m_displayPrecisionNames[i]),
                    static_cast<LPCTSTR>(m_unitsStr));
        m_displayPrecisions[i] = profile.ReadInt(tag, m_displayPrecisions[i]);
    }
}


void MeaUnits::MasterReset()
{
    RestoreDefaultPrecisions();
}


//*************************************************************************
// MeaAngularUnits
//*************************************************************************


MeaAngularUnits::MeaAngularUnits(MeaAngularUnitsId unitsId, LPCTSTR unitsStr):
    MeaUnits(unitsStr), m_unitsId(unitsId)
{
    AddPrecisionName(_T("angle"));      // MeaA
}


MeaAngularUnits::~MeaAngularUnits()
{
}


CString MeaAngularUnits::Format(MeaAngularMeasurementId id, double value) const
{
    CString vstr;
    vstr.Format(_T("%0.*f"), GetDisplayPrecisions()[id], value);
    return vstr;
}


//*************************************************************************
// MeaDegreeUnits
//*************************************************************************

MeaDegreeUnits::MeaDegreeUnits(): MeaAngularUnits(MeaDegreesId, _T("deg"))
{
    AddPrecision(1);        // MeaA
}


MeaDegreeUnits::~MeaDegreeUnits()
{
}


double MeaDegreeUnits::ConvertAngle(double angle) const {
    return 360.0 * angle / (2.0 * MeaUnits::kPI);
}


//*************************************************************************
// MeaRadianUnits
//*************************************************************************

MeaRadianUnits::MeaRadianUnits(): MeaAngularUnits(MeaRadiansId, _T("rad"))
{
    AddPrecision(3);        // MeaA
}


MeaRadianUnits::~MeaRadianUnits()
{
}


double MeaRadianUnits::ConvertAngle(double angle) const {
    return angle;
}


//*************************************************************************
// MeaLinearUnits
//*************************************************************************

const double    MeaLinearUnits::kTickIncrements[]   = { 10.0, 25.0, 50.0, 75.0, 100.0 };
const int       MeaLinearUnits::kNumTickIncrements  = sizeof(kTickIncrements) / sizeof(*kTickIncrements);
const int       MeaLinearUnits::kMinSepPixels       = 5;
const double    MeaLinearUnits::kMinSepInches       = 0.1;

bool            MeaLinearUnits::m_invertY           = MeaUnitsMgr::kDefInvertY;
POINT           MeaLinearUnits::m_originOffset      = { 0, 0 };


MeaLinearUnits::MeaLinearUnits(MeaLinearUnitsId unitsId, LPCTSTR unitsStr):
    MeaUnits(unitsStr), m_unitsId(unitsId), m_majorTickCount(10)
{
    AddPrecisionName(_T("x"));      // MeaX
    AddPrecisionName(_T("y"));      // MeaY
    AddPrecisionName(_T("w"));      // MeaW
    AddPrecisionName(_T("h"));      // MeaH
    AddPrecisionName(_T("d"));      // MeaD
    AddPrecisionName(_T("area"));   // MeaAr
    AddPrecisionName(_T("rx"));     // MeaRx
    AddPrecisionName(_T("ry"));     // MeaRy
}


MeaLinearUnits::~MeaLinearUnits()
{
}


bool MeaLinearUnits::RequiresRes() const
{
    return true;
}


CString MeaLinearUnits::Format(MeaLinearMeasurementId id, double value) const
{
    CString vstr;
    vstr.Format(_T("%0.*f"), GetDisplayPrecisions()[id], value);
    return vstr;
}


FPOINT MeaLinearUnits::ConvertCoord(const POINT& pos) const
{
    FPOINT fpos;
    MeaScreenMgr& smgr = MeaScreenMgr::Instance();
    FSIZE fromPixels = FromPixels(smgr.GetScreenRes(smgr.GetScreenIter(pos)));

    fpos.x = fromPixels.cx * (pos.x - m_originOffset.x);

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            fpos.y = fromPixels.cy * (smgr.GetVirtualRect().Height() - 1 - pos.y);
        } else {
            fpos.y = fromPixels.cy * (m_originOffset.y - pos.y);
        }
    } else {
        fpos.y = fromPixels.cy * (pos.y - m_originOffset.y);
    }

    return fpos;
}


double MeaLinearUnits::ConvertCoord(MeaConvertDir dir, const CWnd* wnd, int pos) const
{
    MeaScreenMgr& smgr = MeaScreenMgr::Instance();
    FSIZE fromPixels = FromPixels(smgr.GetScreenRes(smgr.GetScreenIter(wnd)));

    if (dir == MeaConvertX) {
        return fromPixels.cx * (pos - m_originOffset.x);
    }

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            return fromPixels.cy * (smgr.GetVirtualRect().Height() - 1 - pos);
        }
        return fromPixels.cy * (m_originOffset.y - pos);
    }
    return fromPixels.cy * (pos - m_originOffset.y);
}


double MeaLinearUnits::UnconvertCoord(MeaConvertDir dir, const CWnd* wnd, double pos) const
{
    MeaScreenMgr& smgr = MeaScreenMgr::Instance();
    FSIZE fromPixels = FromPixels(smgr.GetScreenRes(smgr.GetScreenIter(wnd)));

    if (dir == MeaConvertX) {
        return pos / fromPixels.cx + m_originOffset.x;
    }

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            return  (smgr.GetVirtualRect().Height() - 1) - pos / fromPixels.cy;
        }
        return  m_originOffset.y - pos / fromPixels.cy;
    }
    return pos / fromPixels.cy + m_originOffset.y;
}


POINT MeaLinearUnits::UnconvertCoord(const FPOINT& pos) const
{
    POINT point;
    FSIZE fromPixels = FromPixels(FindResFromCoord(pos));

    point.x = static_cast<long>(pos.x / fromPixels.cx + m_originOffset.x);

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            point.y = static_cast<long>((MeaScreenMgr::Instance().GetVirtualRect().Height() - 1) - pos.y / fromPixels.cy);
        } else {
            point.y = static_cast<long>(m_originOffset.y - pos.y / fromPixels.cy);
        }
    } else {
        point.y = static_cast<long>(pos.y / fromPixels.cy + m_originOffset.y);
    }

    return point;
}


bool MeaLinearUnits::UnconvertCoord(MeaConvertDir dir, const CWnd* wnd, double pos, int& c1, int& c2) const
{
    int p1 = static_cast<int>(UnconvertCoord(dir, wnd, pos));
    int p2 = p1 - 1;
    int p3 = p1 + 1;

    double l1 = ConvertCoord(dir, wnd, p1);
    double l2 = ConvertCoord(dir, wnd, p2);
    double l3 = ConvertCoord(dir, wnd, p3);
    double la;

    double d1 = fabs(pos - l1);
    double d2 = fabs(pos - l2);
    double d3 = fabs(pos - l3);

    if (d1 < d2) {
        if (d1 < d3) {
            c1 = p1;
            c2 = p3;
            la = l1;
        } else {
            c1 = p3;
            c2 = p1;
            la = l3;
        }
    } else {
        if (d2 < d3) {
            c1 = p2;
            c2 = p3;
            la = l2;
        } else {
            c1 = p3;
            c2 = p2;
            la = l3;
        }
    }

    return (Format(MeaX, pos) == Format(MeaX, la));
}


FPOINT MeaLinearUnits::ConvertPos(const POINT& pos) const
{
    MeaScreenMgr& mgr = MeaScreenMgr::Instance();
    FSIZE fromPixels = FromPixels(mgr.GetScreenRes(mgr.GetScreenIter(pos)));
    FPOINT pt;
    pt.x = fromPixels.cx * pos.x;
    pt.y = fromPixels.cy * pos.y;
    return pt;
}


POINT MeaLinearUnits::UnconvertPos(const FPOINT& pos) const
{
    FSIZE fromPixels = FromPixels(FindResFromPos(pos));
    POINT pt;
    pt.x = static_cast<long>(pos.x / fromPixels.cx);
    pt.y = static_cast<long>(pos.y / fromPixels.cy);
    return pt;
}


FSIZE MeaLinearUnits::ConvertRes(const FSIZE& res) const
{
    FSIZE pt;
    pt.cx = 1.0 / GetResFromPixels(res).cx;
    pt.cy = 1.0 / GetResFromPixels(res).cy;
    return pt;
}


FSIZE MeaLinearUnits::GetResFromPixels(const FSIZE& res) const
{
    return FromPixels(res);
}


const FSIZE& MeaLinearUnits::FindResFromCoord(const FPOINT& pos) const
{
    MeaScreenMgr& smgr = MeaScreenMgr::Instance();

    if (smgr.GetNumScreens() != 1) {
        MeaScreenMgr::ScreenIter iter;

        for (iter = smgr.GetScreenIter(); !smgr.AtEnd(iter); ++iter) {
            const CRect& srect = smgr.GetScreenRect(iter);
            FPOINT tl = ConvertCoord(srect.TopLeft());
            FPOINT br = ConvertCoord(srect.BottomRight());

            if ((tl.x <= pos.x) && (br.x > pos.x) && (tl.y <= pos.y) && (br.y > pos.y)) {
                return smgr.GetScreenRes(iter);
            }
        }
    }

    return smgr.GetScreenRes(smgr.GetScreenIter());
}


const FSIZE& MeaLinearUnits::FindResFromPos(const FPOINT& pos) const
{
    MeaScreenMgr& smgr = MeaScreenMgr::Instance();

    if (smgr.GetNumScreens() != 1) {
        MeaScreenMgr::ScreenIter iter;

        for (iter = smgr.GetScreenIter(); !smgr.AtEnd(iter); ++iter) {
            const CRect& srect = smgr.GetScreenRect(iter);
            FPOINT tl = ConvertPos(srect.TopLeft());
            FPOINT br = ConvertPos(srect.BottomRight());

            if ((tl.x <= pos.x) && (br.x > pos.x) && (tl.y <= pos.y) && (br.y > pos.y)) {
                return smgr.GetScreenRes(iter);
            }
        }
    }

    return smgr.GetScreenRes(smgr.GetScreenIter());
}


FSIZE MeaLinearUnits::GetMinorIncr(const RECT& rect) const
{
    MeaScreenMgr& smgr = MeaScreenMgr::Instance();
    MeaUnitsMgr& umgr = MeaUnitsMgr::Instance();

    // We want to ensure a minimum resolution-independent
    // separation between the minor ticks. Start by converting
    // the resolution-independent minimum separation to pixels.
    //
    FSIZE res = smgr.GetScreenRes(smgr.GetScreenIter(rect));
    SIZE sepPixels = umgr.ConvertToPixels(MeaInchesId, res, kMinSepInches, kMinSepPixels);

    // Convert the minimum tick separation to the current units.
    //
    FSIZE sepUnits = FromPixels(res) * sepPixels;

    // The object is to find a standard minor increment (e.g. 10, 25)
    // that is the closest to the minimum increment but larger than
    // or equal to it. To achieve this, use the log of the separation
    // to normalize the separation value into a standard range
    // (i.e. 10-100).
    //
    FSIZE sepPower = log10(sepUnits);
    FSIZE delta = 1.0 - floor(sepPower);

    sepPower += delta;

    FSIZE increment;
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


SIZE MeaLinearUnits::ConvertToPixels(const FSIZE& res, double value, int minPixels) const
{
    SIZE pixels;

    FSIZE fromPixels = FromPixels(res);
    
    pixels.cx = static_cast<int>(value / fromPixels.cx);
    pixels.cy = static_cast<int>(value / fromPixels.cy);

    // In case the resolution is small enough that the pixel
    // becomes too small, set the separation to the pixel minimum.
    //
    if (pixels.cx < minPixels) {
        pixels.cx = minPixels;
    }
    if (pixels.cy < minPixels) {
        pixels.cy = minPixels;
    }

    return pixels;
}


//*************************************************************************
// MeaPixelUnits
//*************************************************************************

MeaPixelUnits::MeaPixelUnits(): MeaLinearUnits(MeaPixelsId, _T("px"))
{
    AddPrecision(0);        // MeaX
    AddPrecision(0);        // MeaY
    AddPrecision(0);        // MeaW
    AddPrecision(0);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(0);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}


MeaPixelUnits::~MeaPixelUnits()
{
}


bool MeaPixelUnits::RequiresRes() const
{
    return false;
}


FSIZE MeaPixelUnits::GetResFromPixels(const FSIZE& res) const
{
    FSIZE resFactor;
    resFactor.cx = 1.0 / res.cx;
    resFactor.cy = 1.0 / res.cy;
    return resFactor;
}


FSIZE MeaPixelUnits::FromPixels(const FSIZE& /*res*/) const
{
    FSIZE fromPixels;
    fromPixels.cx = 1.0;
    fromPixels.cy = 1.0;
    return fromPixels;
}


//*************************************************************************
// MeaPointUnits
//*************************************************************************

MeaPointUnits::MeaPointUnits(): MeaLinearUnits(MeaPointsId, _T("pt"))
{
    AddPrecision(1);        // MeaX
    AddPrecision(1);        // MeaY
    AddPrecision(1);        // MeaW
    AddPrecision(1);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(1);        // MeaAr
    AddPrecision(2);        // MeaRx
    AddPrecision(2);        // MeaRy
}


MeaPointUnits::~MeaPointUnits()
{
}


FSIZE MeaPointUnits::FromPixels(const FSIZE& res) const
{
    FSIZE fromPixels;
    fromPixels.cx = 72.0 / res.cx;
    fromPixels.cy = 72.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaPicaUnits
//*************************************************************************

MeaPicaUnits::MeaPicaUnits(): MeaLinearUnits(MeaPicasId, _T("pc"))
{
    AddPrecision(2);        // MeaX
    AddPrecision(2);        // MeaY
    AddPrecision(2);        // MeaW
    AddPrecision(2);        // MeaH
    AddPrecision(2);        // MeaD
    AddPrecision(2);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}


MeaPicaUnits::~MeaPicaUnits()
{
}


FSIZE MeaPicaUnits::FromPixels(const FSIZE& res) const
{
    FSIZE fromPixels;
    fromPixels.cx = 6.0 / res.cx;
    fromPixels.cy = 6.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaTwipUnits
//*************************************************************************

MeaTwipUnits::MeaTwipUnits(): MeaLinearUnits(MeaTwipsId, _T("tp"))
{
    AddPrecision(0);        // MeaX
    AddPrecision(0);        // MeaY
    AddPrecision(0);        // MeaW
    AddPrecision(0);        // MeaH
    AddPrecision(0);        // MeaD
    AddPrecision(0);        // MeaAr
    AddPrecision(4);        // MeaRx
    AddPrecision(4);        // MeaRy
}


MeaTwipUnits::~MeaTwipUnits()
{
}


FSIZE MeaTwipUnits::FromPixels(const FSIZE& res) const
{
    FSIZE fromPixels;
    fromPixels.cx = 1440.0 / res.cx;
    fromPixels.cy = 1440.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaInchUnits
//*************************************************************************

MeaInchUnits::MeaInchUnits(): MeaLinearUnits(MeaInchesId, _T("in"))
{
    AddPrecision(3);        // MeaX
    AddPrecision(3);        // MeaY
    AddPrecision(3);        // MeaW
    AddPrecision(3);        // MeaH
    AddPrecision(3);        // MeaD
    AddPrecision(3);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}


MeaInchUnits::~MeaInchUnits()
{
}


FSIZE MeaInchUnits::FromPixels(const FSIZE& res) const
{
    FSIZE fromPixels;
    fromPixels.cx = 1.0 / res.cx;
    fromPixels.cy = 1.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaCentimeterUnits
//*************************************************************************

MeaCentimeterUnits::MeaCentimeterUnits(): MeaLinearUnits(MeaCentimetersId, _T("cm"))
{
    AddPrecision(2);        // MeaX
    AddPrecision(2);        // MeaY
    AddPrecision(2);        // MeaW
    AddPrecision(2);        // MeaH
    AddPrecision(2);        // MeaD
    AddPrecision(2);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}


MeaCentimeterUnits::~MeaCentimeterUnits()
{
}


FSIZE MeaCentimeterUnits::FromPixels(const FSIZE& res) const
{
    FSIZE fromPixels;
    fromPixels.cx = 2.54 / res.cx;
    fromPixels.cy = 2.54 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaMillimeterUnits
//*************************************************************************

MeaMillimeterUnits::MeaMillimeterUnits(): MeaLinearUnits(MeaMillimetersId, _T("mm"))
{
    AddPrecision(1);        // MeaX
    AddPrecision(1);        // MeaY
    AddPrecision(1);        // MeaW
    AddPrecision(1);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(1);        // MeaAr
    AddPrecision(2);        // MeaRx
    AddPrecision(2);        // MeaRy
}


MeaMillimeterUnits::~MeaMillimeterUnits()
{
}


FSIZE MeaMillimeterUnits::FromPixels(const FSIZE& res) const
{
    FSIZE fromPixels;
    fromPixels.cx = 25.4 / res.cx;
    fromPixels.cy = 25.4 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaCustomUnits
//*************************************************************************

const double MeaCustomUnits::kDefScaleFactor = 1.0;
const MeaCustomUnits::ScaleBasis MeaCustomUnits::kDefScaleBasis = MeaCustomUnits::PixelBasis;


MeaCustomUnits::MeaCustomUnits(): MeaLinearUnits(MeaCustomId, _T("custom")),
    m_scaleBasis(kDefScaleBasis), m_scaleFactor(kDefScaleFactor)
{
    AddPrecision(0);        // MeaX
    AddPrecision(0);        // MeaY
    AddPrecision(0);        // MeaW
    AddPrecision(0);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(0);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}


MeaCustomUnits::~MeaCustomUnits()
{
}


void MeaCustomUnits::SaveProfile(MeaProfile& profile)
{
    profile.WriteStr(_T("CustomName"), m_name);
    profile.WriteStr(_T("CustomAbbrev"), m_abbrev);

    profile.WriteStr(_T("CustomScaleBasis"), GetScaleBasisStr());
    profile.WriteDbl(_T("CustomScaleFactor"), m_scaleFactor);

    SavePrecision(profile);
}


void MeaCustomUnits::LoadProfile(MeaProfile& profile)
{
    SetName(profile.ReadStr(_T("CustomName"), m_name));
    SetAbbrev(profile.ReadStr(_T("CustomAbbrev"), m_abbrev));

    SetScaleBasis(profile.ReadStr(_T("CustomScaleBasis"), _T("")));
    SetScaleFactor(profile.ReadDbl(_T("CustomScaleFactor"), m_scaleFactor));

    LoadPrecision(profile);
}


void MeaCustomUnits::MasterReset()
{
    MeaLinearUnits::MasterReset();

    SetName(_T(""));
    SetAbbrev(_T(""));
    SetScaleFactor(kDefScaleFactor);
    SetScaleBasis(kDefScaleBasis);
}


bool MeaCustomUnits::RequiresRes() const
{
    return (m_scaleBasis != PixelBasis);
}


FSIZE MeaCustomUnits::FromPixels(const FSIZE& res) const
{
    FSIZE fromPixels;

    switch (m_scaleBasis) {
    default:
    case PixelBasis:
        fromPixels.cx = 1.0 / m_scaleFactor;
        fromPixels.cy = 1.0 / m_scaleFactor;
        break;
    case InchBasis:
        fromPixels.cx = 1.0 / (res.cx * m_scaleFactor);
        fromPixels.cy = 1.0 / (res.cy * m_scaleFactor);
        break;
    case CentimeterBasis:
        fromPixels.cx = 2.54 / (res.cx * m_scaleFactor);
        fromPixels.cy = 2.54 / (res.cy * m_scaleFactor);
        break;
    }

    return fromPixels;
}


void MeaCustomUnits::SetName(const CString& name)
{
    m_name = name;
    m_name.TrimLeft();
    m_name.TrimRight();
}


void MeaCustomUnits::SetAbbrev(const CString& abbrev)
{
    m_abbrev = abbrev;
    m_abbrev.TrimLeft();
    m_abbrev.TrimRight();

    MeaLinearUnitsLabel::ChangeLabel(MeaCustomId, m_abbrev);
}


CString MeaCustomUnits::GetScaleBasisStr() const
{
    switch (m_scaleBasis) {
    case PixelBasis:
        return _T("px");
    case InchBasis:
        return _T("in");
    case CentimeterBasis:
        return _T("cm");
    }

    return _T("");
}


void MeaCustomUnits::SetScaleBasis(CString scaleBasisStr)
{
    if (scaleBasisStr == _T("px")) {
        SetScaleBasis(PixelBasis);
    } else if (scaleBasisStr == _T("in")) {
        SetScaleBasis(InchBasis);
    } else if (scaleBasisStr == _T("cm")) {
        SetScaleBasis(CentimeterBasis);
    }
}
