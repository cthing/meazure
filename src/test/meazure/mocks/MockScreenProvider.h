/*
 * Copyright 2022 C Thing Software
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

#include <meazure/ui/ScreenProvider.h>
#include <meazure/units/Units.h>

class MockScreenProvider : public MeaScreenProvider {

public:
    MockScreenProvider() : m_rect(0, 0, 1280, 1024) {
        m_center = m_rect.CenterPoint();
        m_res.cx = 96.0;
        m_res.cy = 96.0;
    }

    virtual int GetNumScreens() const override {
        return 1;
    }

    virtual ScreenIter GetScreenIter() const override {
        return m_screens.begin();
    }

    virtual ScreenIter GetScreenIter(const CWnd*) const override {
        return m_screens.begin();
    }

    virtual ScreenIter GetScreenIter(const POINT&) const override {
        return m_screens.begin();
    }

    virtual ScreenIter GetScreenIter(const RECT&) const override {
        return m_screens.begin();
    }

    virtual bool AtEnd(const ScreenIter& iter) const override {
        return iter == m_screens.end();
    }

    virtual const CPoint& GetCenter() const override {
        return m_center;
    }

    virtual CPoint GetOffScreen() const override {
        return CPoint(3000, 5000);
    }

    virtual const CRect& GetVirtualRect() const override {
        return m_rect;
    }

    virtual const CRect& GetScreenRect(const ScreenIter&) const override {
        return m_rect;
    }

    virtual void GetScreenRes(const ScreenIter&, bool& useManualRes, MeaFSize& manualRes) const override {
        useManualRes = false;
        manualRes = m_res;
    }

    virtual const MeaFSize& GetScreenRes(const ScreenIter&) const override {
        return m_res;
    }

    virtual bool IsManualRes(const ScreenIter&) const override {
        return false;
    }

    virtual bool AnyOSRes() const override {
        return true;
    }

    virtual bool IsPrimary(const ScreenIter&) const override {
        return true;
    }

    virtual CString GetScreenName(const ScreenIter&) const override {
        return CString("MockScreen");
    }

    virtual CPoint LimitPosition(const CPoint& pt) const override {
        CPoint limitPt(pt);

        if (pt.x < m_rect.left) {
            limitPt.x = m_rect.left;
        } else if (pt.x >= m_rect.right) {
            limitPt.x = m_rect.right - 1;
        }

        if (pt.y < m_rect.top) {
            limitPt.y = m_rect.top;
        } else if (pt.y >= m_rect.bottom) {
            limitPt.y = m_rect.bottom - 1;
        }

        return limitPt;
    }

    static void ChangeLabel(MeaLinearUnitsId unitsId, const CString& label) {
        changeLabelCalled = true;
        changeLabelUnits = unitsId;
        changeLabel = label;
    }

    inline static bool changeLabelCalled = false;
    inline static MeaLinearUnitsId changeLabelUnits = MeaPixelsId;
    inline static CString changeLabel;

private:
    Screens m_screens;
    CRect m_rect;
    CPoint m_center;
    MeaFSize m_res;
};
