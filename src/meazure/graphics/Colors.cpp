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
#include "Colors.h"
#include <meazure/ui/LayeredWindows.h>
#include <meazure/utilities/NumericUtils.h>
#include <algorithm>
#include <cmath>

// Remove Windows defines so std versions can be used.
#undef min
#undef max

typedef std::map<MeaColors::Item, COLORREF> Colors;        ///< Maps items to their colors.

static Colors const defaultColors = {
        { MeaColors::LineFore,         RGB(0xFF, 0, 0) },
        { MeaColors::CrossHairBack,    RGB(0xFF, 0, 0) },
        { MeaColors::CrossHairBorder,  RGB(0x50, 0x50, 0x50) },
        { MeaColors::CrossHairHilite,  RGB(0xFF, 0xFF, 0) },
        { MeaColors::CrossHairOpacity, RGB(HaveLayeredWindows() ? 0xE5 : 0xFF, 0, 0) },
        { MeaColors::RulerBack,        RGB(0xFF, 0xFF, 0xFF) },
        { MeaColors::RulerBorder,      RGB(0, 0, 0) },
        { MeaColors::RulerOpacity,     RGB(HaveLayeredWindows() ? 0xE5 : 0xFF, 0, 0) }
};

static Colors colors = defaultColors;


void MeaColors::Reset() {
    colors = defaultColors;
}

void MeaColors::SaveProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        profile.WriteInt(_T("LineFore"), colors[LineFore]);
        profile.WriteInt(_T("CrossHairBack"), colors[CrossHairBack]);
        profile.WriteInt(_T("CrossHairBorder"), colors[CrossHairBorder]);
        profile.WriteInt(_T("CrossHairHilite"), colors[CrossHairHilite]);
        profile.WriteInt(_T("CrossHairOpacity"), colors[CrossHairOpacity]);
        profile.WriteInt(_T("RulerBack"), colors[RulerBack]);
        profile.WriteInt(_T("RulerBorder"), colors[RulerBorder]);
        profile.WriteInt(_T("RulerOpacity"), colors[RulerOpacity]);
    }
}

void MeaColors::LoadProfile(MeaProfile& profile) {
    if (!profile.UserInitiated()) {
        colors[LineFore] = profile.ReadInt(_T("LineFore"), defaultColors.at(LineFore));
        colors[CrossHairBack] = profile.ReadInt(_T("CrossHairBack"), defaultColors.at(CrossHairBack));
        colors[CrossHairBorder] = profile.ReadInt(_T("CrossHairBorder"), defaultColors.at(CrossHairBorder));
        colors[CrossHairHilite] = profile.ReadInt(_T("CrossHairHilite"), defaultColors.at(CrossHairHilite));
        colors[CrossHairOpacity] = profile.ReadInt(_T("CrossHairOpacity"), defaultColors.at(CrossHairOpacity));
        colors[RulerBack] = profile.ReadInt(_T("RulerBack"), defaultColors.at(RulerBack));
        colors[RulerBorder] = profile.ReadInt(_T("RulerBorder"), defaultColors.at(RulerBorder));
        colors[RulerOpacity] = profile.ReadInt(_T("RulerOpacity"), defaultColors.at(RulerOpacity));
    }
}

void MeaColors::MasterReset() {
    colors = defaultColors;
}

void MeaColors::Set(Item item, COLORREF clr) {
    colors[item] = clr;
}

void MeaColors::SetA(Item item, std::uint8_t opacity) {
    colors[item] = RGB(opacity, 0, 0);
}

COLORREF MeaColors::Get(Item item) {
    return colors[item];
}

std::uint8_t MeaColors::GetR(Item item) {
    return GetRValue(colors[item]);
}

std::uint8_t MeaColors::GetG(Item item) {
    return GetGValue(colors[item]);
}

std::uint8_t MeaColors::GetB(Item item) {
    return GetBValue(colors[item]);
}

std::uint8_t MeaColors::GetA(Item item) {
    return GetRValue(colors[item]);
}

COLORREF MeaColors::GetDefault(Item item) {
    return defaultColors.at(item);
}

COLORREF MeaColors::InterpolateColor(COLORREF startRGB, COLORREF endRGB, int percent) {
    if (percent == 0) {
        return startRGB;
    }
    if (percent == 100) {
        return endRGB;
    }

    auto interpolate = [](int start, int end, int percent) {
        return start + static_cast<int>(std::round((end - start) * percent / 100.0));
    };

    HSL startHSL = RGBtoHSL(startRGB);
    HSL endHSL = RGBtoHSL(endRGB);

    HSL hsl(interpolate(startHSL.hue, endHSL.hue, percent), 
            interpolate(startHSL.saturation, endHSL.saturation, percent),
            interpolate(startHSL.lightness, endHSL.lightness, percent));

    return HSLtoRGB(hsl);
}

MeaColors::CMY MeaColors::RGBtoCMY(COLORREF rgb) {
    return CMY(255 - GetRValue(rgb), 255 - GetGValue(rgb), 255 - GetBValue(rgb));
}

MeaColors::CMYK MeaColors::RGBtoCMYK(COLORREF rgb) {
    CMY cmy = RGBtoCMY(rgb);
    int black = std::min({ cmy.cyan, cmy.magenta, cmy.yellow });
    
    if (black == 255) {
        return CMYK(0, 0, 0, black);
    }

    double denom = 255.0 - black;
    int cyan = static_cast<int>(std::round(255.0 * (cmy.cyan - black) / denom));
    int magenta = static_cast<int>(std::round(255.0 * (cmy.magenta - black) / denom));
    int yellow = static_cast<int>(std::round(255.0 * (cmy.yellow - black) / denom));

    return CMYK(cyan, magenta, yellow, black);
}

MeaColors::HSL MeaColors::RGBtoHSL(COLORREF rgb) {
    double h, s, l;
    double r = GetRValue(rgb) / 255.0;
    double g = GetGValue(rgb) / 255.0;
    double b = GetBValue(rgb) / 255.0;
    double cmax = std::max({ r, g, b });
    double cmin = std::min({ r, g, b });
    double delta = cmax - cmin;

    l = (cmax + cmin) / 2.0;
    if (MeaNumericUtils::IsZeroF(delta)) {      // Gray
        h = 0.0;
        s = 0.0;
    } else {                                    // Chroma
        if (l < 0.5) {
            s = delta / (cmax + cmin);
        } else {
            s = delta / (2.0 - cmax - cmin);
        }

        if (MeaNumericUtils::IsEqualF(r, cmax)) {
            h = (g - b) / delta;
        } else if (MeaNumericUtils::IsEqualF(g, cmax)) {
            h = 2.0 + (b - r) / delta;
        } else {
            h = 4.0 + (r - g) / delta;
        }
        h /= 6.0;

        if (h < 0.0) {
            h += 1.0;
        } else if (h > 1.0) {
            h -= 1.0;
        }
    }

    return HSL(static_cast<int>(std::round(h * 360.0)),
               static_cast<int>(std::round(s * 100.0)),
               static_cast<int>(std::round(l * 100.0)));
}

/// Converts a hue to an RGB component value based on the specified weighting factors. See conversion of HSL to RGB at
/// http://www.easyrgb.com/en/math.php.
/// 
/// @param m1       [in] Weighting factor between lightness and saturation.
/// @param m2       [in] Weighting factor between lightness and saturation.
/// @param h        [in] Hue value.
/// @return Either R, G, or B between 0.0 and 1.0. The component returned depends on the weighting factors specified.
/// 
static double HuetoRGB(double m1, double m2, double h) {
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

COLORREF MeaColors::HSLtoRGB(const HSL& hsl) {
    double r, g, b;
    double l = hsl.lightness / 100.0;

    if (hsl.saturation == 0) {
        r = g = b = l;
    } else {
        double h = hsl.hue / 360.0;
        double s = hsl.saturation / 100.0;

        double m2 = (l <= 0.5) ? l * (1.0 + s) : l + s - l * s;
        double m1 = 2.0 * l - m2;

        r = HuetoRGB(m1, m2, h + 1.0 / 3.0);
        g = HuetoRGB(m1, m2, h);
        b = HuetoRGB(m1, m2, h - 1.0 / 3.0);
    }

    return RGB(static_cast<int>(std::round(r * 255.0)),
               static_cast<int>(std::round(g * 255.0)),
               static_cast<int>(std::round(b * 255.0)));
}

MeaColors::YCbCr MeaColors::RGBtoYCbCr(COLORREF rgb) {
    double r = GetRValue(rgb);
    double g = GetGValue(rgb);
    double b = GetBValue(rgb);
    int y = static_cast<int>(std::round(0.257 * r + 0.504 * g + 0.098 * b + 16.0));
    int cb = static_cast<int>(std::round(-0.148 * r - 0.291 * g + 0.439 * b + 128.0));
    int cr = static_cast<int>(std::round(0.439 * r - 0.368 * g - 0.071 * b + 128.0));
    return YCbCr(y, cb, cr);
}

MeaColors::YIQ MeaColors::RGBtoYIQ(COLORREF rgb) {
    double r = GetRValue(rgb);
    double g = GetGValue(rgb);
    double b = GetBValue(rgb);
    int y = static_cast<int>(std::round(0.299 * r + 0.587 * g + 0.114 * b));
    int i = static_cast<int>(std::round(0.596 * r - 0.275 * g - 0.321 * b));
    int q = static_cast<int>(std::round(0.212 * r - 0.523 * g + 0.311 * b));
    return YIQ(y, i, q);
}
