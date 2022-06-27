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
 /// Compile the multiple monitor stub functions.
#define COMPILE_MULTIMON_STUBS 1
#include "ScreenMgr.h"
#include <meazure/resource.h>
#include <meazure/utilities/StringUtils.h>
#include <cassert>


/// Represents a single display screen.
///
class MeaScreenProvider::Screen {

public:
    /// Constructs a display screen object.
    ///
    /// @param rect     [in] Rectangle representing the screen, in pixels.
    ///                 In multiple monitor environments, the screen rectangle
    ///                 can have negative coordinates.
    ///
    Screen(LPCRECT rect) :
        m_rect(rect),
        m_useManualRes(MeaScreenMgr::kDefUseManualRes),
        m_calInInches(MeaScreenMgr::kDefCalInInches),
        m_primary(false) {
        m_center = m_rect.CenterPoint();
    }

    /// Destroys a display screen object.
    ///
    virtual ~Screen() = default;

    /// Returns the screen rectangle. Note that in multiple
    /// monitor environments, the screen rectangle can have
    /// negative coordinates.
    ///
    /// @return Screen rectangle, in pixels.
    ///
    const CRect& GetRect() const { return m_rect; }

    /// Determines the center of the screen.
    ///
    /// @return Center point of the screen.
    ///
    const CPoint& GetCenter() const { return m_center; }

    /// Sets the resolution of the screen, in pixels per inch.
    ///
    /// @param useManualRes     [in] Specifies whether the manually calibrated
    ///                         resolution should be used (true) or the operating
    ///                         system resolution (false).
    /// @param manualRes        [in] If non-nullptr, specifies the manually calibrated
    ///                         resolution.
    ///
    void SetScreenRes(bool useManualRes, const MeaFSize* manualRes = nullptr) {
        m_useManualRes = useManualRes;

        if (manualRes != nullptr) {
            m_manualRes = *manualRes;
        }

        if (m_manualRes.cx < DBL_EPSILON || m_manualRes.cy < DBL_EPSILON) {
            m_manualRes = m_osRes;
        }

        m_currentRes = m_useManualRes ? m_manualRes : m_osRes;
    }

    /// Obtains the currently set manual screen resolution, if set.
    ///
    /// @param useManualRes     [out] Indicates whether the manually calibrated
    ///                         resolution is used (true) or the operating
    ///                         system resolution (false).
    /// @param manualRes        [out] Manually calibrated resolution, in pixels
    ///                         per inch.
    ///
    void GetScreenRes(bool& useManualRes, MeaFSize& manualRes) const {
        useManualRes = m_useManualRes;
        manualRes = m_manualRes;
    }

    /// Obtains the current screen resolution. If the resolution has been
    /// manually calibrated, it is returned. Otherwise the operating system
    /// reported resolution is returned.
    ///
    /// @return Screen resolution, in pixels per inch.
    ///
    const MeaFSize& GetScreenRes() const { return m_currentRes; }

    /// Indicates if the screen resolution has been manually set.
    ///
    /// @return <b>true</b> if the screen resolution has been set manually.
    ///
    bool IsManualRes() const { return m_useManualRes; }

    /// In multiple monitor environments, one of the monitors is
    /// designated as the primary. This method designates the screen
    /// as the primary.
    ///
    /// @param primary      [in] Specify <b>true</b> if this screen
    ///                     is the primary.
    ///
    void SetPrimary(bool primary) { m_primary = primary; }

    /// Indicates if this is the primary screen.
    ///
    /// @return <b>true</b> if this is the primary screen.
    ///
    bool IsPrimary() const { return m_primary; }

    /// Specifies whether the resolution calibration has been performed
    /// in inches or centimeters.
    ///
    /// @param calInInches  [in] <b>true</b> if the calibration has been
    ///                     performed in inches.
    ///
    void SetCalInInches(bool calInInches) { m_calInInches = calInInches; }

    /// Indicates whether the resolution calibration has been performed
    /// in inches or centimeters.
    ///
    /// @return <b>true</b> if the calibration has been performed in inches.
    ///
    bool IsCalInInches() const { return m_calInInches; }

    /// Sets a descriptive name for the screen.
    ///
    /// @param name     [in] Name for the screen.
    ///
    void SetName(PCTSTR name) { m_name = name; }

    /// Returns the descriptive name for the screen.
    ///
    /// @return Name for the screen or the empty string if
    ///         there is no name.
    ///
    CString GetName() const { return m_name; }

private:
    /// Obtains the screen resolution reported by the operating system.
    ///
    /// @return Resolution in pixels per inch.
    ///
    static MeaFSize GetOSScreenRes() {
        MeaFSize res;

        HDC dc = ::GetDC(nullptr);
        if (dc != nullptr) {
            res.cx = static_cast<double>(::GetDeviceCaps(dc, LOGPIXELSX));
            res.cy = static_cast<double>(::GetDeviceCaps(dc, LOGPIXELSY));
            ::ReleaseDC(nullptr, dc);
        } else {
            res.cx = static_cast<double>(USER_DEFAULT_SCREEN_DPI);
            res.cy = static_cast<double>(USER_DEFAULT_SCREEN_DPI);
        }

        return res;
    }


    static inline const MeaFSize m_osRes = GetOSScreenRes();    ///< DPI reported by the operating system.

    CRect m_rect;           ///< Screen rectangle.
    CPoint m_center;        ///< Center point of the screen.
    MeaFSize m_currentRes;  ///< Current screen resolution, pixels per inch.
    MeaFSize m_manualRes;   ///< Manually calibrated resolution, pixels per inch.
    bool m_useManualRes;    ///< Indicates if manually calibrated resolution is used.
    bool m_calInInches;     ///< Indicates if calibration in inches or centimeters.
    bool m_primary;         ///< Indicates if this is the primary screen.
    CString m_name;         ///< Descriptive name for the screen.
};


MeaScreenMgr::MeaScreenMgr(token) :
    MeaSingleton_T<MeaScreenMgr>(), m_sizeChanged(false) {
    EnumDisplayMonitors(nullptr, nullptr, CreateScreens, reinterpret_cast<LPARAM>(this));
    assert(m_screens.size() > 0);

    m_virtualRect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    m_virtualRect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    m_virtualRect.bottom = m_virtualRect.top + GetSystemMetrics(SM_CYVIRTUALSCREEN);
    m_virtualRect.right = m_virtualRect.left + GetSystemMetrics(SM_CXVIRTUALSCREEN);
}

MeaScreenMgr::~MeaScreenMgr() {
    try {
        for (const auto& screenEntry : m_screens) {
            delete screenEntry.second;
        }
    } catch (...) {
        assert(false);
    }
}

void MeaScreenMgr::SaveProfile(MeaProfile& profile) const {
    if (!profile.UserInitiated()) {
        profile.WriteInt(_T("ScreenW"), m_virtualRect.Width());
        profile.WriteInt(_T("ScreenH"), m_virtualRect.Height());

        ScreenIter iter;
        int screenIdx { 0 };

        profile.WriteInt(_T("NumScreens"), GetNumScreens());
        for (const auto& screenEntry : m_screens) {
            CString tag;
            bool useManualRes;
            MeaFSize manualRes;

            screenEntry.second->GetScreenRes(useManualRes, manualRes);

            tag.Format(_T("Screen%d-"), screenIdx);
            profile.WriteInt(tag + _T("CenterX"), screenEntry.second->GetCenter().x);
            profile.WriteInt(tag + _T("CenterY"), screenEntry.second->GetCenter().y);
            profile.WriteBool(tag + _T("UseManualRes"), useManualRes);
            profile.WriteStr(tag + _T("ManualResX"), MeaStringUtils::DblToStr(manualRes.cx));
            profile.WriteStr(tag + _T("ManualResY"), MeaStringUtils::DblToStr(manualRes.cy));
            profile.WriteBool(tag + _T("CalInInches"), screenEntry.second->IsCalInInches());

            screenIdx++;
        }
    }
}

void MeaScreenMgr::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        int w = profile.ReadInt(_T("ScreenW"), m_virtualRect.Width());
        int h = profile.ReadInt(_T("ScreenH"), m_virtualRect.Height());
        int numScreens = profile.ReadInt(_T("NumScreens"), 0);
        m_sizeChanged = ((w != m_virtualRect.Width()) || (h != m_virtualRect.Height()));

        if ((profile.GetVersion() == 1) || (numScreens == 0)) {
            MeaFSize manualRes;
            bool useManualRes = profile.ReadBool(_T("UseManualRes"), kDefUseManualRes);
            manualRes.cx = profile.ReadDbl(_T("ManualResX"), 0.0);
            manualRes.cy = profile.ReadDbl(_T("ManualResY"), 0.0);

            for (const auto& screenEntry : m_screens) {
                screenEntry.second->SetScreenRes(useManualRes, &manualRes);
            }
        } else {
            for (int i = 0; i < numScreens; i++) {
                CString tag;
                tag.Format(_T("Screen%d-"), i);

                CPoint center(profile.ReadInt(tag + _T("CenterX"), 0), 
                              profile.ReadInt(tag + _T("CenterY"), 0));

                Screen* screen = GetScreen(center);
                if (screen != nullptr) {
                    MeaFSize manualRes;

                    bool useManualRes = profile.ReadBool(tag + _T("UseManualRes"), kDefUseManualRes);
                    manualRes.cx = profile.ReadDbl(tag + _T("ManualResX"), 0.0);
                    manualRes.cy = profile.ReadDbl(tag + _T("ManualResY"), 0.0);
                    screen->SetScreenRes(useManualRes, &manualRes);

                    bool calInInches = profile.ReadBool(tag + _T("CalInInches"), kDefCalInInches);
                    screen->SetCalInInches(calInInches);
                }
            }
        }
    }
}

void MeaScreenMgr::MasterReset() const {
    for (const auto& screenEntry : m_screens) {
        Screen* screen = screenEntry.second;

        if (screen != nullptr) {
            MeaFSize manualRes;

            manualRes.cx = 0.0;
            manualRes.cy = 0.0;

            screen->SetScreenRes(kDefUseManualRes, &manualRes);
            screen->SetCalInInches(kDefCalInInches);
        }
    }
}

const CPoint& MeaScreenMgr::GetCenter() const {
    HMONITOR mon = MonitorFromWindow(*AfxGetMainWnd(), MONITOR_DEFAULTTONEAREST);
    assert(mon != nullptr);
    Screens::const_iterator iter = m_screens.find(mon);
    assert(iter != m_screens.end());
    return (*iter).second->GetCenter();
}

CPoint MeaScreenMgr::GetOffScreen() const {
    const CRect& vscreen = GetVirtualRect();
    CPoint pt(vscreen.left, vscreen.top);
    pt.Offset(-100, -100);
    return pt;
}

CRect MeaScreenMgr::EnsureVisible(const RECT& windowRect) const {
    CRect rect(windowRect);

    if (MonitorFromRect(&windowRect, MONITOR_DEFAULTTONULL) == nullptr) {
        HMONITOR mon = MonitorFromRect(&windowRect, MONITOR_DEFAULTTONEAREST);
        assert(mon != nullptr);
        Screens::const_iterator iter = m_screens.find(mon);
        assert(iter != m_screens.end());

        int dx = (*iter).second->GetRect().left - windowRect.left;
        int dy = (*iter).second->GetRect().top - windowRect.top;
        rect.OffsetRect(dx, dy);
    }

    return rect;
}

CPoint MeaScreenMgr::LimitPosition(const CPoint& pt) const {
    CPoint limitPt(pt);

    if (MonitorFromPoint(limitPt, MONITOR_DEFAULTTONULL) == nullptr) {
        HMONITOR mon = MonitorFromPoint(limitPt, MONITOR_DEFAULTTONEAREST);
        assert(mon != nullptr);
        Screens::const_iterator iter = m_screens.find(mon);
        assert(iter != m_screens.end());

        const CRect& rect = (*iter).second->GetRect();

        if (pt.x < rect.left) {
            limitPt.x = rect.left;
        } else if (pt.x >= rect.right) {
            limitPt.x = rect.right - 1;
        }

        if (pt.y < rect.top) {
            limitPt.y = rect.top;
        } else if (pt.y >= rect.bottom) {
            limitPt.y = rect.bottom - 1;
        }
    }

    return limitPt;
}

MeaScreenMgr::Screen* MeaScreenMgr::GetScreen(const POINT& point) const {
    Screen* screen = nullptr;

    HMONITOR mon = MonitorFromPoint(point, MONITOR_DEFAULTTONULL);
    if (mon != nullptr) {
        Screens::const_iterator iter = m_screens.find(mon);
        assert(iter != m_screens.end());
        screen = (*iter).second;
    }

    return screen;
}

MeaScreenMgr::ScreenIter MeaScreenMgr::GetScreenIter(const CWnd* wnd) const {
    HMONITOR mon = MonitorFromWindow(*wnd, MONITOR_DEFAULTTONEAREST);
    assert(mon != nullptr);

    Screens::const_iterator iter = m_screens.find(mon);
    assert(iter != m_screens.end());
    return iter;
}

MeaScreenMgr::ScreenIter MeaScreenMgr::GetScreenIter(const POINT& point) const {
    HMONITOR mon = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
    assert(mon != nullptr);

    Screens::const_iterator iter = m_screens.find(mon);
    assert(iter != m_screens.end());
    return iter;
}

MeaScreenMgr::ScreenIter MeaScreenMgr::GetScreenIter(const RECT& rect) const {
    HMONITOR mon = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
    assert(mon != nullptr);

    Screens::const_iterator iter = m_screens.find(mon);
    assert(iter != m_screens.end());
    return iter;
}

const CRect& MeaScreenMgr::GetScreenRect(const ScreenIter& iter) const {
    return (*iter).second->GetRect();
}

void MeaScreenMgr::GetScreenRes(const ScreenIter& iter, bool& useManualRes, MeaFSize& manualRes) const {
    (*iter).second->GetScreenRes(useManualRes, manualRes);
}

const MeaFSize& MeaScreenMgr::GetScreenRes(const ScreenIter& iter) const {
    return (*iter).second->GetScreenRes();
}

void MeaScreenMgr::SetScreenRes(const ScreenIter& iter, bool useManualRes, const MeaFSize* manualRes) const {
    (*iter).second->SetScreenRes(useManualRes, manualRes);
}

bool MeaScreenMgr::IsManualRes(const ScreenIter& iter) const { 
    return (*iter).second->IsManualRes();
}

bool MeaScreenMgr::AnyOSRes() const {
    for (const auto& screenEntry : m_screens) {
        if (!screenEntry.second->IsManualRes()) {
            return true;
        }
    }
    return false;
}

bool MeaScreenMgr::IsPrimary(const ScreenIter& iter) const {
    return (*iter).second->IsPrimary();
}

CString MeaScreenMgr::GetScreenName(const ScreenIter& iter) const {
    return (*iter).second->GetName();
}

bool MeaScreenMgr::IsCalInInches(const ScreenIter& iter) const {
    return (*iter).second->IsCalInInches();
}

void MeaScreenMgr::SetCalInInches(const ScreenIter& iter, bool calInInches) const {
    (*iter).second->SetCalInInches(calInInches);
}

BOOL CALLBACK MeaScreenMgr::CreateScreens(HMONITOR hMonitor, HDC /* hdcMonitor */, LPRECT monitorRect,
                                          LPARAM userData) {
    MeaScreenMgr* mgr = reinterpret_cast<MeaScreenMgr*>(userData);

    Screen* screen = new Screen(monitorRect);
    mgr->m_screens[hMonitor] = screen;

    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfo(hMonitor, &info) != 0) {
        CString name;

        if (info.dwFlags & MONITORINFOF_PRIMARY) {
            screen->SetPrimary(true);
            name.Format(IDS_MEA_SCREEN_PRIMARY, mgr->GetNumScreens());
        } else {
            screen->SetPrimary(false);
            name.Format(IDS_MEA_SCREEN, mgr->GetNumScreens());
        }

        screen->SetName(name);
    }

    return TRUE;
}
