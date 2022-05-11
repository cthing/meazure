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
#include "Units.h"
#include "UnitsLabels.h"


//*************************************************************************
// MeaUnits
//*************************************************************************


const double MeaUnits::kPI = 3.14159265359;


MeaUnits::MeaUnits(LPCTSTR unitsStr) : m_unitsStr(unitsStr) {}

MeaUnits::~MeaUnits() {}

void MeaUnits::SaveProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        SavePrecision(profile);
    }
}

void MeaUnits::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        LoadPrecision(profile);
    }
}

void MeaUnits::SavePrecision(MeaProfile& profile) {
    unsigned int i;

    for (i = 0; i < m_displayPrecisions.size(); i++) {
        CString tag;

        tag.Format(_T("Precision-%s-%s"),
                    static_cast<LPCTSTR>(m_displayPrecisionNames[i]),
                    static_cast<LPCTSTR>(m_unitsStr));
        profile.WriteInt(tag, m_displayPrecisions[i]);
    }
}

void MeaUnits::LoadPrecision(MeaProfile& profile) {
    unsigned int i;

    for (i = 0; i < m_displayPrecisions.size(); i++) {
        CString tag;

        tag.Format(_T("Precision-%s-%s"),
                    static_cast<LPCTSTR>(m_displayPrecisionNames[i]),
                    static_cast<LPCTSTR>(m_unitsStr));
        m_displayPrecisions[i] = profile.ReadInt(tag, m_displayPrecisions[i]);
    }
}

void MeaUnits::MasterReset() {
    RestoreDefaultPrecisions();
}


//*************************************************************************
// MeaAngularUnits
//*************************************************************************


MeaAngularUnits::MeaAngularUnits(MeaAngularUnitsId unitsId, LPCTSTR unitsStr) :
    MeaUnits(unitsStr), m_unitsId(unitsId) {
    AddPrecisionName(_T("angle"));      // MeaA
}

MeaAngularUnits::~MeaAngularUnits() {}

CString MeaAngularUnits::Format(MeaAngularMeasurementId id, double value) const {
    CString vstr;
    vstr.Format(_T("%0.*f"), GetDisplayPrecisions()[id], value);
    return vstr;
}


//*************************************************************************
// MeaDegreeUnits
//*************************************************************************

MeaDegreeUnits::MeaDegreeUnits() : MeaAngularUnits(MeaDegreesId, _T("deg")) {
    AddPrecision(1);        // MeaA
}

MeaDegreeUnits::~MeaDegreeUnits() {}

double MeaDegreeUnits::ConvertAngle(double angle) const {
    return 360.0 * angle / (2.0 * MeaUnits::kPI);
}


//*************************************************************************
// MeaRadianUnits
//*************************************************************************

MeaRadianUnits::MeaRadianUnits() : MeaAngularUnits(MeaRadiansId, _T("rad")) {
    AddPrecision(3);        // MeaA
}

MeaRadianUnits::~MeaRadianUnits() {}

double MeaRadianUnits::ConvertAngle(double angle) const {
    return angle;
}


//*************************************************************************
// MeaLinearUnits
//*************************************************************************

bool MeaLinearUnits::m_invertY = false;
POINT MeaLinearUnits::m_originOffset = { 0, 0 };


MeaLinearUnits::MeaLinearUnits(MeaLinearUnitsId unitsId, LPCTSTR unitsStr, const MeaScreenProvider& screenProvider) :
    MeaUnits(unitsStr), m_unitsId(unitsId), m_screenProvider(screenProvider), m_majorTickCount(10) {
    AddPrecisionName(_T("x"));      // MeaX
    AddPrecisionName(_T("y"));      // MeaY
    AddPrecisionName(_T("w"));      // MeaW
    AddPrecisionName(_T("h"));      // MeaH
    AddPrecisionName(_T("d"));      // MeaD
    AddPrecisionName(_T("area"));   // MeaAr
    AddPrecisionName(_T("rx"));     // MeaRx
    AddPrecisionName(_T("ry"));     // MeaRy
}

MeaLinearUnits::~MeaLinearUnits() {}

bool MeaLinearUnits::RequiresRes() const {
    return true;
}

CString MeaLinearUnits::Format(MeaLinearMeasurementId id, double value) const {
    CString vstr;
    vstr.Format(_T("%0.*f"), GetDisplayPrecisions()[id], value);
    return vstr;
}

FPOINT MeaLinearUnits::ConvertCoord(const POINT& pos) const {
    FPOINT fpos;
    FSIZE fromPixels = FromPixels(m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter(pos)));

    fpos.x = fromPixels.cx * (pos.x - m_originOffset.x);

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            fpos.y = fromPixels.cy * (m_screenProvider.GetVirtualRect().Height() - 1 - pos.y);
        } else {
            fpos.y = fromPixels.cy * (m_originOffset.y - pos.y);
        }
    } else {
        fpos.y = fromPixels.cy * (pos.y - m_originOffset.y);
    }

    return fpos;
}

double MeaLinearUnits::ConvertCoord(MeaConvertDir dir, const CWnd* wnd, int pos) const {
    FSIZE fromPixels = FromPixels(m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter(wnd)));

    if (dir == MeaConvertX) {
        return fromPixels.cx * (pos - m_originOffset.x);
    }

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            return fromPixels.cy * (m_screenProvider.GetVirtualRect().Height() - 1 - pos);
        }
        return fromPixels.cy * (m_originOffset.y - pos);
    }
    return fromPixels.cy * (pos - m_originOffset.y);
}

double MeaLinearUnits::UnconvertCoord(MeaConvertDir dir, const CWnd* wnd, double pos) const {
    FSIZE fromPixels = FromPixels(m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter(wnd)));

    if (dir == MeaConvertX) {
        return pos / fromPixels.cx + m_originOffset.x;
    }

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            return  (m_screenProvider.GetVirtualRect().Height() - 1) - pos / fromPixels.cy;
        }
        return  m_originOffset.y - pos / fromPixels.cy;
    }
    return pos / fromPixels.cy + m_originOffset.y;
}

POINT MeaLinearUnits::UnconvertCoord(const FPOINT& pos) const {
    POINT point;
    FSIZE fromPixels = FromPixels(FindResFromCoord(pos));

    point.x = static_cast<long>(pos.x / fromPixels.cx + m_originOffset.x);

    if (m_invertY) {
        if ((m_originOffset.x == 0) && (m_originOffset.y == 0)) {
            point.y = static_cast<long>((m_screenProvider.GetVirtualRect().Height() - 1) - pos.y / fromPixels.cy);
        } else {
            point.y = static_cast<long>(m_originOffset.y - pos.y / fromPixels.cy);
        }
    } else {
        point.y = static_cast<long>(pos.y / fromPixels.cy + m_originOffset.y);
    }

    return point;
}

bool MeaLinearUnits::UnconvertCoord(MeaConvertDir dir, const CWnd* wnd, double pos, int& c1, int& c2) const {
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

FPOINT MeaLinearUnits::ConvertPos(const POINT& pos) const {
    FSIZE fromPixels = FromPixels(m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter(pos)));
    FPOINT pt;
    pt.x = fromPixels.cx * pos.x;
    pt.y = fromPixels.cy * pos.y;
    return pt;
}

POINT MeaLinearUnits::UnconvertPos(const FPOINT& pos) const {
    FSIZE fromPixels = FromPixels(FindResFromPos(pos));
    POINT pt;
    pt.x = static_cast<long>(pos.x / fromPixels.cx);
    pt.y = static_cast<long>(pos.y / fromPixels.cy);
    return pt;
}

FSIZE MeaLinearUnits::ConvertRes(const FSIZE& res) const {
    FSIZE pt;
    pt.cx = 1.0 / GetResFromPixels(res).cx;
    pt.cy = 1.0 / GetResFromPixels(res).cy;
    return pt;
}

FSIZE MeaLinearUnits::GetResFromPixels(const FSIZE& res) const {
    return FromPixels(res);
}

const FSIZE& MeaLinearUnits::FindResFromCoord(const FPOINT& pos) const {
    if (m_screenProvider.GetNumScreens() != 1) {
        MeaScreenProvider::ScreenIter iter;

        for (iter = m_screenProvider.GetScreenIter(); !m_screenProvider.AtEnd(iter); ++iter) {
            const CRect& srect = m_screenProvider.GetScreenRect(iter);
            FPOINT tl = ConvertCoord(srect.TopLeft());
            FPOINT br = ConvertCoord(srect.BottomRight());

            if ((tl.x <= pos.x) && (br.x > pos.x) && (tl.y <= pos.y) && (br.y > pos.y)) {
                return m_screenProvider.GetScreenRes(iter);
            }
        }
    }

    return m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter());
}

const FSIZE& MeaLinearUnits::FindResFromPos(const FPOINT& pos) const {
    if (m_screenProvider.GetNumScreens() != 1) {
        MeaScreenProvider::ScreenIter iter;

        for (iter = m_screenProvider.GetScreenIter(); !m_screenProvider.AtEnd(iter); ++iter) {
            const CRect& srect = m_screenProvider.GetScreenRect(iter);
            FPOINT tl = ConvertPos(srect.TopLeft());
            FPOINT br = ConvertPos(srect.BottomRight());

            if ((tl.x <= pos.x) && (br.x > pos.x) && (tl.y <= pos.y) && (br.y > pos.y)) {
                return m_screenProvider.GetScreenRes(iter);
            }
        }
    }

    return m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter());
}

SIZE MeaLinearUnits::ConvertToPixels(const FSIZE& res, double value, int minPixels) const {
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

MeaPixelUnits::MeaPixelUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaPixelsId, _T("px"), screenProvider) {
    AddPrecision(0);        // MeaX
    AddPrecision(0);        // MeaY
    AddPrecision(0);        // MeaW
    AddPrecision(0);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(0);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}

MeaPixelUnits::~MeaPixelUnits() {}

bool MeaPixelUnits::RequiresRes() const {
    return false;
}

FSIZE MeaPixelUnits::GetResFromPixels(const FSIZE& res) const {
    FSIZE resFactor;
    resFactor.cx = 1.0 / res.cx;
    resFactor.cy = 1.0 / res.cy;
    return resFactor;
}

FSIZE MeaPixelUnits::FromPixels(const FSIZE& /*res*/) const {
    FSIZE fromPixels;
    fromPixels.cx = 1.0;
    fromPixels.cy = 1.0;
    return fromPixels;
}


//*************************************************************************
// MeaPointUnits
//*************************************************************************

MeaPointUnits::MeaPointUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaPointsId, _T("pt"), screenProvider) {
    AddPrecision(1);        // MeaX
    AddPrecision(1);        // MeaY
    AddPrecision(1);        // MeaW
    AddPrecision(1);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(1);        // MeaAr
    AddPrecision(2);        // MeaRx
    AddPrecision(2);        // MeaRy
}

MeaPointUnits::~MeaPointUnits() {}

FSIZE MeaPointUnits::FromPixels(const FSIZE& res) const {
    FSIZE fromPixels;
    fromPixels.cx = 72.0 / res.cx;
    fromPixels.cy = 72.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaPicaUnits
//*************************************************************************

MeaPicaUnits::MeaPicaUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaPicasId, _T("pc"), screenProvider) {
    AddPrecision(2);        // MeaX
    AddPrecision(2);        // MeaY
    AddPrecision(2);        // MeaW
    AddPrecision(2);        // MeaH
    AddPrecision(2);        // MeaD
    AddPrecision(2);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}

MeaPicaUnits::~MeaPicaUnits() {}

FSIZE MeaPicaUnits::FromPixels(const FSIZE& res) const {
    FSIZE fromPixels;
    fromPixels.cx = 6.0 / res.cx;
    fromPixels.cy = 6.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaTwipUnits
//*************************************************************************

MeaTwipUnits::MeaTwipUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaTwipsId, _T("tp"), screenProvider) {
    AddPrecision(0);        // MeaX
    AddPrecision(0);        // MeaY
    AddPrecision(0);        // MeaW
    AddPrecision(0);        // MeaH
    AddPrecision(0);        // MeaD
    AddPrecision(0);        // MeaAr
    AddPrecision(4);        // MeaRx
    AddPrecision(4);        // MeaRy
}

MeaTwipUnits::~MeaTwipUnits() {}

FSIZE MeaTwipUnits::FromPixels(const FSIZE& res) const {
    FSIZE fromPixels;
    fromPixels.cx = 1440.0 / res.cx;
    fromPixels.cy = 1440.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaInchUnits
//*************************************************************************

MeaInchUnits::MeaInchUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaInchesId, _T("in"), screenProvider) {
    AddPrecision(3);        // MeaX
    AddPrecision(3);        // MeaY
    AddPrecision(3);        // MeaW
    AddPrecision(3);        // MeaH
    AddPrecision(3);        // MeaD
    AddPrecision(3);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}

MeaInchUnits::~MeaInchUnits() {}

FSIZE MeaInchUnits::FromPixels(const FSIZE& res) const {
    FSIZE fromPixels;
    fromPixels.cx = 1.0 / res.cx;
    fromPixels.cy = 1.0 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaCentimeterUnits
//*************************************************************************

MeaCentimeterUnits::MeaCentimeterUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaCentimetersId, _T("cm"), screenProvider) {
    AddPrecision(2);        // MeaX
    AddPrecision(2);        // MeaY
    AddPrecision(2);        // MeaW
    AddPrecision(2);        // MeaH
    AddPrecision(2);        // MeaD
    AddPrecision(2);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}

MeaCentimeterUnits::~MeaCentimeterUnits() {}

FSIZE MeaCentimeterUnits::FromPixels(const FSIZE& res) const {
    FSIZE fromPixels;
    fromPixels.cx = 2.54 / res.cx;
    fromPixels.cy = 2.54 / res.cy;
    return fromPixels;
}


//*************************************************************************
// MeaMillimeterUnits
//*************************************************************************

MeaMillimeterUnits::MeaMillimeterUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaMillimetersId, _T("mm"), screenProvider) {
    AddPrecision(1);        // MeaX
    AddPrecision(1);        // MeaY
    AddPrecision(1);        // MeaW
    AddPrecision(1);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(1);        // MeaAr
    AddPrecision(2);        // MeaRx
    AddPrecision(2);        // MeaRy
}

MeaMillimeterUnits::~MeaMillimeterUnits() {}

FSIZE MeaMillimeterUnits::FromPixels(const FSIZE& res) const {
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


MeaCustomUnits::MeaCustomUnits(const MeaScreenProvider& screenProvider) :
    MeaLinearUnits(MeaCustomId, _T("custom"), screenProvider),
    m_scaleBasis(kDefScaleBasis),
    m_scaleFactor(kDefScaleFactor) {
    AddPrecision(0);        // MeaX
    AddPrecision(0);        // MeaY
    AddPrecision(0);        // MeaW
    AddPrecision(0);        // MeaH
    AddPrecision(1);        // MeaD
    AddPrecision(0);        // MeaAr
    AddPrecision(1);        // MeaRx
    AddPrecision(1);        // MeaRy
}

MeaCustomUnits::~MeaCustomUnits() {}

void MeaCustomUnits::SaveProfile(MeaProfile& profile) {
    profile.WriteStr(_T("CustomName"), m_name);
    profile.WriteStr(_T("CustomAbbrev"), m_abbrev);

    profile.WriteStr(_T("CustomScaleBasis"), GetScaleBasisStr());
    profile.WriteDbl(_T("CustomScaleFactor"), m_scaleFactor);

    SavePrecision(profile);
}

void MeaCustomUnits::LoadProfile(MeaProfile& profile) {
    SetName(profile.ReadStr(_T("CustomName"), m_name));
    SetAbbrev(profile.ReadStr(_T("CustomAbbrev"), m_abbrev));

    SetScaleBasis(profile.ReadStr(_T("CustomScaleBasis"), _T("")));
    SetScaleFactor(profile.ReadDbl(_T("CustomScaleFactor"), m_scaleFactor));

    LoadPrecision(profile);
}

void MeaCustomUnits::MasterReset() {
    MeaLinearUnits::MasterReset();

    SetName(_T(""));
    SetAbbrev(_T(""));
    SetScaleFactor(kDefScaleFactor);
    SetScaleBasis(kDefScaleBasis);
}

bool MeaCustomUnits::RequiresRes() const {
    return (m_scaleBasis != PixelBasis);
}

FSIZE MeaCustomUnits::FromPixels(const FSIZE& res) const {
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

void MeaCustomUnits::SetName(const CString& name) {
    m_name = name;
    m_name.TrimLeft();
    m_name.TrimRight();
}

void MeaCustomUnits::SetAbbrev(const CString& abbrev) {
    m_abbrev = abbrev;
    m_abbrev.TrimLeft();
    m_abbrev.TrimRight();

    MeaLinearUnitsLabel::ChangeLabel(MeaCustomId, m_abbrev);
}

CString MeaCustomUnits::GetScaleBasisStr() const {
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

void MeaCustomUnits::SetScaleBasis(CString scaleBasisStr) {
    if (scaleBasisStr == _T("px")) {
        SetScaleBasis(PixelBasis);
    } else if (scaleBasisStr == _T("in")) {
        SetScaleBasis(InchBasis);
    } else if (scaleBasisStr == _T("cm")) {
        SetScaleBasis(CentimeterBasis);
    }
}
