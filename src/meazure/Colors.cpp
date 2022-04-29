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
#include "Colors.h"
#include "LayeredWindows.h"
#include "Utils.h"


MeaColors::Colors MeaColors::m_defColors;
MeaColors::Colors MeaColors::m_colors;


void MeaColors::Initialize() {
    if (m_defColors.empty()) {
        m_defColors[LineFore] = RGB(0xFF, 0, 0);
        m_defColors[CrossHairBack] = RGB(0xFF, 0, 0);
        m_defColors[CrossHairBorder] = RGB(0x50, 0x50, 0x50);
        m_defColors[CrossHairHilite] = RGB(0xFF, 0xFF, 0);
        m_defColors[CrossHairOpacity] = RGB(HaveLayeredWindows() ? 0xE5 : 0xFF, 0, 0);
        m_defColors[RulerBack] = RGB(0xFF, 0xFF, 0xFF);
        m_defColors[RulerBorder] = RGB(0, 0, 0);
        m_defColors[RulerOpacity] = RGB(HaveLayeredWindows() ? 0xE5 : 0xFF, 0, 0);

        m_colors = m_defColors;
    }
}

void MeaColors::SaveProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        profile.WriteInt(_T("LineFore"), m_colors[LineFore]);
        profile.WriteInt(_T("CrossHairBack"), m_colors[CrossHairBack]);
        profile.WriteInt(_T("CrossHairBorder"), m_colors[CrossHairBorder]);
        profile.WriteInt(_T("CrossHairHilite"), m_colors[CrossHairHilite]);
        profile.WriteInt(_T("CrossHairOpacity"), m_colors[CrossHairOpacity]);
        profile.WriteInt(_T("RulerBack"), m_colors[RulerBack]);
        profile.WriteInt(_T("RulerBorder"), m_colors[RulerBorder]);
        profile.WriteInt(_T("RulerOpacity"), m_colors[RulerOpacity]);
    }
}

void MeaColors::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        m_colors[LineFore] = profile.ReadInt(_T("LineFore"), m_defColors[LineFore]);
        m_colors[CrossHairBack] = profile.ReadInt(_T("CrossHairBack"), m_defColors[CrossHairBack]);
        m_colors[CrossHairBorder] = profile.ReadInt(_T("CrossHairBorder"), m_defColors[CrossHairBorder]);
        m_colors[CrossHairHilite] = profile.ReadInt(_T("CrossHairHilite"), m_defColors[CrossHairHilite]);
        m_colors[CrossHairOpacity] = profile.ReadInt(_T("CrossHairOpacity"), m_defColors[CrossHairOpacity]);
        m_colors[RulerBack] = profile.ReadInt(_T("RulerBack"), m_defColors[RulerBack]);
        m_colors[RulerBorder] = profile.ReadInt(_T("RulerBorder"), m_defColors[RulerBorder]);
        m_colors[RulerOpacity] = profile.ReadInt(_T("RulerOpacity"), m_defColors[RulerOpacity]);
    }
}

void MeaColors::MasterReset() {
    m_colors = m_defColors;
}

COLORREF MeaColors::InterpolateColor(COLORREF startRGB, COLORREF endRGB, int percent) {
    if (percent == 0) {
        return startRGB;
    }
    if (percent == 100) {
        return endRGB;
    }

    auto interpolate = [](double start, double end, int percent) {
        return start + (end - start) * percent / 100.0;
    };

    HSL hsl, startHSL, endHSL;
    COLORREF rgb;

    RGBtoHSL(startRGB, startHSL);
    RGBtoHSL(endRGB, endHSL);

    hsl.hue = interpolate(startHSL.hue, endHSL.hue, percent);
    hsl.saturation = interpolate(startHSL.saturation, endHSL.saturation, percent);
    hsl.lightness = interpolate(startHSL.lightness, endHSL.lightness, percent);

    HSLtoRGB(hsl, rgb);

    return rgb;
}

void MeaColors::RGBtoHSL(COLORREF rgb, HSL& hsl) {
    double h, s, l;
    double r = GetRValue(rgb) / 255.0;
    double g = GetGValue(rgb) / 255.0;
    double b = GetBValue(rgb) / 255.0;
    double cmax = Max(r, Max(g, b));
    double cmin = Min(r, Min(g, b));

    l = (cmax + cmin) / 2.0;
    if (MEA_DBL_EQL(cmax, cmin)) {
        s = 0.0;
        h = 0.0; // it's really undefined
    } else {
        if (l < 0.5) {
            s = (cmax - cmin) / (cmax + cmin);
        } else {
            s = (cmax - cmin) / (2.0 - cmax - cmin);
        }
        double delta = cmax - cmin;

        if (MEA_DBL_EQL(r, cmax)) {
            h = (g - b) / delta;
        } else if (MEA_DBL_EQL(g, cmax)) {
            h = 2.0 + (b - r) / delta;
        } else {
            h = 4.0 + (r - g) / delta;
        }
        h /= 6.0;

        if (h < 0.0) {
            h += 1.0;
        }
    }

    hsl.hue = h;
    hsl.lightness = l;
    hsl.saturation = s;
}

double MeaColors::HuetoRGB(double m1, double m2, double h) {
    if (h < 0.0) {
        h += 1.0;
    }
    if (h > 1.0) {
        h -= 1.0;
    }
    if ((6.0 * h) < 1.0) {
        return (m1 + (m2 - m1) * h * 6.0);
    }
    if ((2.0 * h) < 1.0) {
        return m2;
    }
    if ((3.0 * h) < 2.0) {
        return (m1 + (m2 - m1) * ((2.0 / 3.0) - h) * 6.0);
    }
    return m1;
}

void MeaColors::HSLtoRGB(const HSL& hsl, COLORREF& rgb) {
    double r, g, b;

    if (hsl.saturation == 0.0) {
        r = g = b = hsl.lightness;
    } else {
        double m2;

        if (hsl.lightness <= 0.5) {
            m2 = hsl.lightness * (1.0 + hsl.saturation);
        } else {
            m2 = hsl.lightness + hsl.saturation - hsl.lightness * hsl.saturation;
        }
        double m1 = 2.0 * hsl.lightness - m2;

        r = HuetoRGB(m1, m2, hsl.hue + 1.0 / 3.0);
        g = HuetoRGB(m1, m2, hsl.hue);
        b = HuetoRGB(m1, m2, hsl.hue - 1.0 / 3.0);
    }

    rgb = RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}
