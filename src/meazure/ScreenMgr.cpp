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
#include "MeaAssert.h"
 /// Compile the multiple monitor stub functions.
#define COMPILE_MULTIMON_STUBS 1
#include "ScreenMgr.h"
#include "Units.h"


MEA_SINGLETON_DEF(MeaScreenMgr);        ///< Managers are singletons.


MeaScreenMgr::MeaScreenMgr() :
    MeaSingleton_T<MeaScreenMgr>(), m_sizeChanged(false) {
    EnumDisplayMonitors(nullptr, nullptr, CreateScreens, reinterpret_cast<LPARAM>(this));
    MeaAssert(m_screens.size() > 0);

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
        MeaAssert(false);
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
            FSIZE manualRes;

            screenEntry.second->GetScreenRes(useManualRes, manualRes);

            tag.Format(_T("Screen%d-"), screenIdx);
            profile.WriteInt(tag + _T("CenterX"), screenEntry.second->GetCenter().x);
            profile.WriteInt(tag + _T("CenterY"), screenEntry.second->GetCenter().y);
            profile.WriteBool(tag + _T("UseManualRes"), useManualRes);
            profile.WriteStr(tag + _T("ManualResX"), MeaUtils::DblToStr(manualRes.cx));
            profile.WriteStr(tag + _T("ManualResY"), MeaUtils::DblToStr(manualRes.cy));
            profile.WriteBool(tag + _T("CalInInches"), screenEntry.second->GetCalInInches());

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
            FSIZE manualRes;
            bool useManualRes = profile.ReadBool(_T("UseManualRes"), kDefUseManualRes);
            manualRes.cx = profile.ReadDbl(_T("ManualResX"), 0.0);
            manualRes.cy = profile.ReadDbl(_T("ManualResY"), 0.0);

            for (const auto& screenEntry : m_screens) {
                screenEntry.second->SetScreenRes(useManualRes, &manualRes);
            }
        } else {
            for (int i = 0; i < numScreens; i++) {
                CString tag;
                POINT center;

                tag.Format(_T("Screen%d-"), i);
                center.x = profile.ReadInt(tag + _T("CenterX"), 0);
                center.y = profile.ReadInt(tag + _T("CenterY"), 0);

                Screen* screen = GetScreen(center);
                if (screen != nullptr) {
                    FSIZE manualRes;

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
            FSIZE manualRes;

            manualRes.cx = 0.0;
            manualRes.cy = 0.0;

            screen->SetScreenRes(kDefUseManualRes, &manualRes);
            screen->SetCalInInches(kDefCalInInches);
        }
    }
}

const CPoint& MeaScreenMgr::GetCenter() const {
    HMONITOR mon = MonitorFromWindow(*AfxGetMainWnd(), MONITOR_DEFAULTTONEAREST);
    MeaAssert(mon != nullptr);
    Screens::const_iterator iter = m_screens.find(mon);
    MeaAssert(iter != m_screens.end());
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
        MeaAssert(mon != nullptr);
        Screens::const_iterator iter = m_screens.find(mon);
        MeaAssert(iter != m_screens.end());

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
        MeaAssert(mon != nullptr);
        Screens::const_iterator iter = m_screens.find(mon);
        MeaAssert(iter != m_screens.end());

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
        MeaAssert(iter != m_screens.end());
        screen = (*iter).second;
    }

    return screen;
}

MeaScreenMgr::ScreenIter MeaScreenMgr::GetScreenIter(const CWnd* wnd) const {
    HMONITOR mon = MonitorFromWindow(*wnd, MONITOR_DEFAULTTONEAREST);
    MeaAssert(mon != nullptr);

    Screens::const_iterator iter = m_screens.find(mon);
    MeaAssert(iter != m_screens.end());
    return iter;
}

MeaScreenMgr::ScreenIter MeaScreenMgr::GetScreenIter(const POINT& point) const {
    HMONITOR mon = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
    MeaAssert(mon != nullptr);

    Screens::const_iterator iter = m_screens.find(mon);
    MeaAssert(iter != m_screens.end());
    return iter;
}

MeaScreenMgr::ScreenIter MeaScreenMgr::GetScreenIter(const RECT& rect) const {
    HMONITOR mon = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
    MeaAssert(mon != nullptr);

    Screens::const_iterator iter = m_screens.find(mon);
    MeaAssert(iter != m_screens.end());
    return iter;
}

FSIZE MeaScreenMgr::GetOSScreenRes() const {
    FSIZE res;

    HDC dc = ::GetDC(nullptr);
    res.cx = static_cast<double>(::GetDeviceCaps(dc, LOGPIXELSX));
    res.cy = static_cast<double>(::GetDeviceCaps(dc, LOGPIXELSY));
    ::ReleaseDC(nullptr, dc);

    return res;
}

BOOL CALLBACK MeaScreenMgr::CreateScreens(HMONITOR hMonitor, HDC /* hdcMonitor */, LPRECT monitorRect,
                                          LPARAM userData) {
    MeaScreenMgr* mgr = reinterpret_cast<MeaScreenMgr*>(userData);

    Screen* screen = new Screen(*mgr, monitorRect);
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


//*************************************************************************
// Screen
//*************************************************************************


MeaScreenMgr::Screen::Screen(MeaScreenMgr& mgr, LPCRECT rect) :
    m_mgr(mgr),
    m_rect(rect),
    m_useManualRes(kDefUseManualRes),
    m_calInInches(kDefCalInInches),
    m_primary(false) {
    m_center = m_rect.CenterPoint();

    m_currentRes.cx = 0.0;
    m_currentRes.cy = 0.0;

    m_manualRes.cx = 0.0;
    m_manualRes.cy = 0.0;
}

MeaScreenMgr::Screen::~Screen() {}

void MeaScreenMgr::Screen::SetScreenRes(bool useManualRes, const FSIZE* manualRes) {
    m_useManualRes = useManualRes;

    if (manualRes != nullptr) {
        m_manualRes = *manualRes;
    }

    if (m_manualRes.cx < DBL_EPSILON || m_manualRes.cy < DBL_EPSILON) {
        m_manualRes = m_mgr.GetOSScreenRes();
    }

    m_currentRes = m_useManualRes ? m_manualRes : m_mgr.GetOSScreenRes();
}
