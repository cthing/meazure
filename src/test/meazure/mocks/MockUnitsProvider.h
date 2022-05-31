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

#pragma once

#include <meazure/units/Units.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ScreenProvider.h>

class MockUnitsProvider : public MeaUnitsProvider {

public:
    MockUnitsProvider(const MockScreenProvider& screenProvider) :
        m_screenProvider(screenProvider),
        m_linearUnits(new MeaPixelUnits(screenProvider)),
        m_angularUnits(new MeaDegreeUnits()),
        m_origin({0, 0})
    {}

    ~MockUnitsProvider() {
        delete m_linearUnits;
        delete m_angularUnits;
    }

    MeaLinearUnits* GetLinearUnits() const override { return m_linearUnits; }

    MeaLinearUnits* GetLinearUnits(const CString&) const override { return m_linearUnits; }

    MeaAngularUnits* GetAngularUnits() const override { return m_angularUnits; }

    MeaAngularUnits* GetAngularUnits(const CString&) const override { return m_angularUnits; }

    bool IsInvertY() const override { return false; }

    const POINT& GetOrigin() const override { return m_origin; }

    void SetOrigin(const MeaFPoint& origin) {
        m_origin.x = origin.x;
        m_origin.y = origin.y;
    }

    MeaFSize GetWidthHeight(const POINT& p1, const POINT& p2) const override {
        POINT np1 = p1;
        POINT np2 = p2;

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

        return MeaFSize(fabs(np1.x - np2.x), fabs(np1.y - np2.y));
    }

    CString FormatConvertAngle(double angle) const override {
        return Format(MeaA, ConvertAngle(angle));
    }

    CString Format(MeaLinearMeasurementId id, double value) const override {
        return m_linearUnits->Format(id, value);
    }

    CString Format(MeaAngularMeasurementId id, double value) const override {
        return m_angularUnits->Format(id, value);
    }

    MeaFPoint ConvertCoord(const POINT& pos) const override {
        return m_linearUnits->ConvertCoord(pos);
    }

    MeaFPoint ConvertPos(const POINT& pos) const override {
        return m_linearUnits->ConvertPos(pos);
    }

    POINT UnconvertPos(const MeaFPoint& pos) const override {
        return m_linearUnits->UnconvertPos(pos);
    }

    MeaFSize ConvertRes(const MeaFSize& res) const override {
        return m_linearUnits->ConvertRes(res);
    }

    double ConvertAngle(double angle) const override {
        return m_angularUnits->ConvertAngle(angle);
    }

    SIZE ConvertToPixels(MeaLinearUnitsId, const MeaFSize& res, double value, int minPixels) const override {
        return m_linearUnits->ConvertToPixels(res, value, minPixels);
    }

    int GetMajorTickCount() const override {
        return 10;
    }

    MeaFSize GetMinorIncr(const RECT&) const override {
        return MeaFSize(2.0, 2.0);
    }

private:
    const MockScreenProvider m_screenProvider;
    MeaLinearUnits* m_linearUnits;
    MeaAngularUnits* m_angularUnits;
    POINT m_origin;
};
