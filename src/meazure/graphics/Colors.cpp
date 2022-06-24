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
#include <float.h>

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

static MeaColors::ColorTableEntry basicWebColors[] = {
    { "black",      RGB(  0,   0,   0), MeaColors::RGBtoLab(RGB(  0,   0,   0)) },
    { "silver",     RGB(192, 192, 192), MeaColors::RGBtoLab(RGB(192, 192, 192)) },
    { "gray",       RGB(128, 128, 128), MeaColors::RGBtoLab(RGB(128, 128, 128)) },
    { "white",      RGB(255, 255, 255), MeaColors::RGBtoLab(RGB(255, 255, 255)) },
    { "maroon",     RGB(128,   0,   0), MeaColors::RGBtoLab(RGB(128,   0,   0)) },
    { "red",        RGB(255,   0,   0), MeaColors::RGBtoLab(RGB(255,   0,   0)) },
    { "purple",     RGB(128,   0, 128), MeaColors::RGBtoLab(RGB(128,   0, 128)) },
    { "fuchsia",    RGB(255,   0, 255), MeaColors::RGBtoLab(RGB(255,   0, 255)) },
    { "green",      RGB(  0, 128,   0), MeaColors::RGBtoLab(RGB(  0, 128,   0)) },
    { "lime",       RGB(  0, 255,   0), MeaColors::RGBtoLab(RGB(  0, 255,   0)) },
    { "olive",      RGB(128, 128,   0), MeaColors::RGBtoLab(RGB(128, 128,   0)) },
    { "yellow",     RGB(255, 255,   0), MeaColors::RGBtoLab(RGB(255, 255,   0)) },
    { "navy",       RGB(  0,   0, 128), MeaColors::RGBtoLab(RGB(  0,   0, 128)) },
    { "blue",       RGB(  0,   0, 255), MeaColors::RGBtoLab(RGB(  0,   0, 255)) },
    { "teal",       RGB(  0, 128, 128), MeaColors::RGBtoLab(RGB(  0, 128, 128)) },
    { "aqua",       RGB(  0, 255, 255), MeaColors::RGBtoLab(RGB(  0, 255, 255)) },
    { nullptr,      0,                  MeaColors::Lab()                        },
};

static MeaColors::ColorTableEntry extendedWebColors[] = {
    { "aliceblue",              RGB(240, 248, 255), MeaColors::RGBtoLab(RGB(240, 248, 255)) },
    { "antiquewhite",           RGB(250, 235, 215), MeaColors::RGBtoLab(RGB(250, 235, 215)) },
    { "aqua",                   RGB(  0, 255, 255), MeaColors::RGBtoLab(RGB(  0, 255, 255)) },
    { "aquamarine",             RGB(127, 255, 212), MeaColors::RGBtoLab(RGB(127, 255, 212)) },
    { "azure",                  RGB(240, 255, 255), MeaColors::RGBtoLab(RGB(240, 255, 255)) },
    { "beige",                  RGB(245, 245, 220), MeaColors::RGBtoLab(RGB(245, 245, 220)) },
    { "bisque",                 RGB(255, 228, 196), MeaColors::RGBtoLab(RGB(255, 228, 196)) },
    { "black",                  RGB(  0,   0,   0), MeaColors::RGBtoLab(RGB(  0,   0,   0)) },
    { "blanchedalmond",         RGB(255, 235, 205), MeaColors::RGBtoLab(RGB(255, 235, 205)) },
    { "blue",                   RGB(  0,   0, 255), MeaColors::RGBtoLab(RGB(  0,   0, 255)) },
    { "blue",                   RGB(  0,   0, 255), MeaColors::RGBtoLab(RGB(  0,   0, 255)) },
    { "blueviolet",             RGB(138,  43, 226), MeaColors::RGBtoLab(RGB(138,  43, 226)) },
    { "brown",                  RGB(165,  42,  42), MeaColors::RGBtoLab(RGB(165,  42,  42)) },
    { "burlywood",              RGB(222, 184, 135), MeaColors::RGBtoLab(RGB(222, 184, 135)) },
    { "cadetblue",              RGB( 95, 158, 160), MeaColors::RGBtoLab(RGB( 95, 158, 160)) },
    { "chartreuse",             RGB(127, 255,   0), MeaColors::RGBtoLab(RGB(127, 255,   0)) },
    { "chocolate",              RGB(210, 105,  30), MeaColors::RGBtoLab(RGB(210, 105,  30)) },
    { "coral",                  RGB(255, 127,  80), MeaColors::RGBtoLab(RGB(255, 127,  80)) },
    { "cornflowerblue",         RGB(100, 149, 237), MeaColors::RGBtoLab(RGB(100, 149, 237)) },
    { "cornsilk",               RGB(255, 248, 220), MeaColors::RGBtoLab(RGB(255, 248, 220)) },
    { "crimson",                RGB(220,  20,  60), MeaColors::RGBtoLab(RGB(220,  20,  60)) },
    { "cyan",                   RGB(  0, 255, 255), MeaColors::RGBtoLab(RGB(  0, 255, 255)) },
    { "darkblue",               RGB(  0,   0, 139), MeaColors::RGBtoLab(RGB(  0,   0, 139)) },
    { "darkcyan",               RGB(  0, 139, 139), MeaColors::RGBtoLab(RGB(  0, 139, 139)) },
    { "darkgoldenrod",          RGB(184, 134,  11), MeaColors::RGBtoLab(RGB(184, 134,  11)) },
    { "darkgray",               RGB(169, 169, 169), MeaColors::RGBtoLab(RGB(169, 169, 169)) },
    { "darkgreen",              RGB(  0, 100,   0), MeaColors::RGBtoLab(RGB(  0, 100,   0)) },
    { "darkkhaki",              RGB(189, 183, 107), MeaColors::RGBtoLab(RGB(189, 183, 107)) },
    { "darkmagenta",            RGB(139,   0, 139), MeaColors::RGBtoLab(RGB(139,   0, 139)) },
    { "darkolivegreen",         RGB( 85, 107,  47), MeaColors::RGBtoLab(RGB( 85, 107,  47)) },
    { "darkorange",             RGB(255, 140,   0), MeaColors::RGBtoLab(RGB(255, 140,   0)) },
    { "darkorchid",             RGB(153,  50, 204), MeaColors::RGBtoLab(RGB(153,  50, 204)) },
    { "darkred",                RGB(139,   0,   0), MeaColors::RGBtoLab(RGB(139,   0,   0)) },
    { "darksalmon",             RGB(233, 150, 122), MeaColors::RGBtoLab(RGB(233, 150, 122)) },
    { "darkseagreen",           RGB(143, 188, 143), MeaColors::RGBtoLab(RGB(143, 188, 143)) },
    { "darkslateblue",          RGB( 72,  61, 139), MeaColors::RGBtoLab(RGB( 72,  61, 139)) },
    { "darkslategray",          RGB( 47,  79,  79), MeaColors::RGBtoLab(RGB( 47,  79,  79)) },
    { "darkturquoise",          RGB(  0, 206, 209), MeaColors::RGBtoLab(RGB(  0, 206, 209)) },
    { "darkviolet",             RGB(148,   0, 211), MeaColors::RGBtoLab(RGB(148,   0, 211)) },
    { "deeppink",               RGB(255,  20, 147), MeaColors::RGBtoLab(RGB(255,  20, 147)) },
    { "deepskyblue",            RGB(  0, 191, 255), MeaColors::RGBtoLab(RGB(  0, 191, 255)) },
    { "dimgray",                RGB(105, 105, 105), MeaColors::RGBtoLab(RGB(105, 105, 105)) },
    { "dodgerblue",             RGB( 30, 144, 255), MeaColors::RGBtoLab(RGB( 30, 144, 255)) },
    { "firebrick",              RGB(178,  34,  34), MeaColors::RGBtoLab(RGB(178,  34,  34)) },
    { "floralwhite",            RGB(255, 250, 240), MeaColors::RGBtoLab(RGB(255, 250, 240)) },
    { "forestgreen",            RGB( 34, 139,  34), MeaColors::RGBtoLab(RGB( 34, 139,  34)) },
    { "fuchsia",                RGB(255,   0, 255), MeaColors::RGBtoLab(RGB(255,   0, 255)) },
    { "gainsboro",              RGB(220, 220, 220), MeaColors::RGBtoLab(RGB(220, 220, 220)) },
    { "ghostwhite",             RGB(248, 248, 255), MeaColors::RGBtoLab(RGB(248, 248, 255)) },
    { "gold",                   RGB(255, 215,   0), MeaColors::RGBtoLab(RGB(255, 215,   0)) },
    { "goldenrod",              RGB(218, 165,  32), MeaColors::RGBtoLab(RGB(218, 165,  32)) },
    { "gray",                   RGB(128, 128, 128), MeaColors::RGBtoLab(RGB(128, 128, 128)) },
    { "green",                  RGB(  0, 128,   0), MeaColors::RGBtoLab(RGB(  0, 128,   0)) },
    { "greenyellow",            RGB(173, 255,  47), MeaColors::RGBtoLab(RGB(173, 255,  47)) },
    { "honeydew",               RGB(240, 255, 240), MeaColors::RGBtoLab(RGB(240, 255, 240)) },
    { "hotpink",                RGB(255, 105, 180), MeaColors::RGBtoLab(RGB(255, 105, 180)) },
    { "indianred",              RGB(205,  92,  92), MeaColors::RGBtoLab(RGB(205,  92,  92)) },
    { "indigo",                 RGB( 75,   0, 130), MeaColors::RGBtoLab(RGB( 75,   0, 130)) },
    { "ivory",                  RGB(255, 255, 240), MeaColors::RGBtoLab(RGB(255, 255, 240)) },
    { "khaki",                  RGB(240, 230, 140), MeaColors::RGBtoLab(RGB(240, 230, 140)) },
    { "lavender",               RGB(230, 230, 250), MeaColors::RGBtoLab(RGB(230, 230, 250)) },
    { "lavenderblush",          RGB(255, 240, 245), MeaColors::RGBtoLab(RGB(255, 240, 245)) },
    { "lawngreen",              RGB(124, 252,   0), MeaColors::RGBtoLab(RGB(124, 252,   0)) },
    { "lemonchiffon",           RGB(255, 250, 205), MeaColors::RGBtoLab(RGB(255, 250, 205)) },
    { "lightblue",              RGB(173, 216, 230), MeaColors::RGBtoLab(RGB(173, 216, 230)) },
    { "lightcoral",             RGB(240, 128, 128), MeaColors::RGBtoLab(RGB(240, 128, 128)) },
    { "lightcyan",              RGB(224, 255, 255), MeaColors::RGBtoLab(RGB(224, 255, 255)) },
    { "lightgoldenrodyellow",   RGB(250, 250, 210), MeaColors::RGBtoLab(RGB(250, 250, 210)) },
    { "lightgray",              RGB(211, 211, 211), MeaColors::RGBtoLab(RGB(211, 211, 211)) },
    { "lightgreen",             RGB(144, 238, 144), MeaColors::RGBtoLab(RGB(144, 238, 144)) },
    { "lightpink",              RGB(255, 182, 193), MeaColors::RGBtoLab(RGB(255, 182, 193)) },
    { "lightsalmon",            RGB(255, 160, 122), MeaColors::RGBtoLab(RGB(255, 160, 122)) },
    { "lightseagreen",          RGB( 32, 178, 170), MeaColors::RGBtoLab(RGB( 32, 178, 170)) },
    { "lightskyblue",           RGB(135, 206, 250), MeaColors::RGBtoLab(RGB(135, 206, 250)) },
    { "lightslategray",         RGB(119, 136, 153), MeaColors::RGBtoLab(RGB(119, 136, 153)) },
    { "lightsteelblue",         RGB(176, 196, 222), MeaColors::RGBtoLab(RGB(176, 196, 222)) },
    { "lightyellow",            RGB(255, 255, 224), MeaColors::RGBtoLab(RGB(255, 255, 224)) },
    { "lime",                   RGB(  0, 255,   0), MeaColors::RGBtoLab(RGB(  0, 255,   0)) },
    { "limegreen",              RGB( 50, 205,  50), MeaColors::RGBtoLab(RGB( 50, 205,  50)) },
    { "linen",                  RGB(250, 240, 230), MeaColors::RGBtoLab(RGB(250, 240, 230)) },
    { "magenta",                RGB(255,   0, 255), MeaColors::RGBtoLab(RGB(255,   0, 255)) },
    { "maroon",                 RGB(128,   0,   0), MeaColors::RGBtoLab(RGB(128,   0,   0)) },
    { "mediumaquamarine",       RGB(102, 205, 170), MeaColors::RGBtoLab(RGB(102, 205, 170)) },
    { "mediumblue",             RGB(  0,   0, 205), MeaColors::RGBtoLab(RGB(  0,   0, 205)) },
    { "mediumorchid",           RGB(186,  85, 211), MeaColors::RGBtoLab(RGB(186,  85, 211)) },
    { "mediumpurple",           RGB(147, 112, 219), MeaColors::RGBtoLab(RGB(147, 112, 219)) },
    { "mediumseagreen",         RGB( 60, 179, 113), MeaColors::RGBtoLab(RGB( 60, 179, 113)) },
    { "mediumslateblue",        RGB(123, 104, 238), MeaColors::RGBtoLab(RGB(123, 104, 238)) },
    { "mediumspringgreen",      RGB(  0, 250, 154), MeaColors::RGBtoLab(RGB(  0, 250, 154)) },
    { "mediumturquoise",        RGB( 72, 209, 204), MeaColors::RGBtoLab(RGB( 72, 209, 204)) },
    { "mediumvioletred",        RGB(199,  21, 133), MeaColors::RGBtoLab(RGB(199,  21, 133)) },
    { "midnightblue",           RGB( 25,  25, 112), MeaColors::RGBtoLab(RGB( 25,  25, 112)) },
    { "mintcream",              RGB(245, 255, 250), MeaColors::RGBtoLab(RGB(245, 255, 250)) },
    { "mistyrose",              RGB(255, 228, 225), MeaColors::RGBtoLab(RGB(255, 228, 225)) },
    { "moccasin",               RGB(255, 228, 181), MeaColors::RGBtoLab(RGB(255, 228, 181)) },
    { "navajowhite",            RGB(255, 222, 173), MeaColors::RGBtoLab(RGB(255, 222, 173)) },
    { "navy",                   RGB(  0,   0, 128), MeaColors::RGBtoLab(RGB(  0,   0, 128)) },
    { "oldlace",                RGB(253, 245, 230), MeaColors::RGBtoLab(RGB(253, 245, 230)) },
    { "olive",                  RGB(128, 128,   0), MeaColors::RGBtoLab(RGB(128, 128,   0)) },
    { "olivedrab",              RGB(107, 142,  35), MeaColors::RGBtoLab(RGB(107, 142,  35)) },
    { "orange",                 RGB(255, 165,   0), MeaColors::RGBtoLab(RGB(255, 165,   0)) },
    { "orangered",              RGB(255,  69,   0), MeaColors::RGBtoLab(RGB(255,  69,   0)) },
    { "orchid",                 RGB(218, 112, 214), MeaColors::RGBtoLab(RGB(218, 112, 214)) },
    { "palegoldenrod",          RGB(238, 232, 170), MeaColors::RGBtoLab(RGB(238, 232, 170)) },
    { "palegreen",              RGB(152, 251, 152), MeaColors::RGBtoLab(RGB(152, 251, 152)) },
    { "paleturquoise",          RGB(175, 238, 238), MeaColors::RGBtoLab(RGB(175, 238, 238)) },
    { "palevioletred",          RGB(219, 112, 147), MeaColors::RGBtoLab(RGB(219, 112, 147)) },
    { "papayawhip",             RGB(255, 239, 213), MeaColors::RGBtoLab(RGB(255, 239, 213)) },
    { "peachpuff",              RGB(255, 218, 185), MeaColors::RGBtoLab(RGB(255, 218, 185)) },
    { "peru",                   RGB(205, 133,  63), MeaColors::RGBtoLab(RGB(205, 133,  63)) },
    { "pink",                   RGB(255, 192, 203), MeaColors::RGBtoLab(RGB(255, 192, 203)) },
    { "plum",                   RGB(221, 160, 221), MeaColors::RGBtoLab(RGB(221, 160, 221)) },
    { "powderblue",             RGB(176, 224, 230), MeaColors::RGBtoLab(RGB(176, 224, 230)) },
    { "purple",                 RGB(128,   0, 128), MeaColors::RGBtoLab(RGB(128,   0, 128)) },
    { "red",                    RGB(255,   0,   0), MeaColors::RGBtoLab(RGB(255,   0,   0)) },
    { "rosybrown",              RGB(188, 143, 143), MeaColors::RGBtoLab(RGB(188, 143, 143)) },
    { "royalblue",              RGB( 65, 105, 225), MeaColors::RGBtoLab(RGB( 65, 105, 225)) },
    { "saddlebrown",            RGB(139,  69,  19), MeaColors::RGBtoLab(RGB(139,  69,  19)) },
    { "salmon",                 RGB(250, 128, 114), MeaColors::RGBtoLab(RGB(250, 128, 114)) },
    { "sandybrown",             RGB(244, 164,  96), MeaColors::RGBtoLab(RGB(244, 164,  96)) },
    { "seagreen",               RGB( 46, 139,  87), MeaColors::RGBtoLab(RGB( 46, 139,  87)) },
    { "seashell",               RGB(255, 245, 238), MeaColors::RGBtoLab(RGB(255, 245, 238)) },
    { "sienna",                 RGB(160, 82,   45), MeaColors::RGBtoLab(RGB(160, 82,   45)) },
    { "silver",                 RGB(192, 192, 192), MeaColors::RGBtoLab(RGB(192, 192, 192)) },
    { "skyblue",                RGB(135, 206, 235), MeaColors::RGBtoLab(RGB(135, 206, 235)) },
    { "slateblue",              RGB(106,  90, 205), MeaColors::RGBtoLab(RGB(106,  90, 205)) },
    { "slategray",              RGB(112, 128, 144), MeaColors::RGBtoLab(RGB(112, 128, 144)) },
    { "snow",                   RGB(255, 250, 250), MeaColors::RGBtoLab(RGB(255, 250, 250)) },
    { "springgreen",            RGB(  0, 255, 127), MeaColors::RGBtoLab(RGB(  0, 255, 127)) },
    { "steelblue",              RGB( 70, 130, 180), MeaColors::RGBtoLab(RGB( 70, 130, 180)) },
    { "tan",                    RGB(210, 180, 140), MeaColors::RGBtoLab(RGB(210, 180, 140)) },
    { "teal",                   RGB(  0, 128, 128), MeaColors::RGBtoLab(RGB(  0, 128, 128)) },
    { "thistle",                RGB(216, 191, 216), MeaColors::RGBtoLab(RGB(216, 191, 216)) },
    { "tomato",                 RGB(255,  99,  71), MeaColors::RGBtoLab(RGB(255,  99,  71)) },
    { "turquoise",              RGB( 64, 224, 208), MeaColors::RGBtoLab(RGB( 64, 224, 208)) },
    { "violet",                 RGB(238, 130, 238), MeaColors::RGBtoLab(RGB(238, 130, 238)) },
    { "wheat",                  RGB(245, 222, 179), MeaColors::RGBtoLab(RGB(245, 222, 179)) },
    { "white",                  RGB(255, 255, 255), MeaColors::RGBtoLab(RGB(255, 255, 255)) },
    { "whitesmoke",             RGB(245, 245, 245), MeaColors::RGBtoLab(RGB(245, 245, 245)) },
    { "yellow",                 RGB(255, 255,   0), MeaColors::RGBtoLab(RGB(255, 255,   0)) },
    { "yellowgreen",            RGB(154, 205,  50), MeaColors::RGBtoLab(RGB(154, 205,  50)) },
    { nullptr,                  0,                  MeaColors::Lab()                        },
};


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

void MeaColors::Set(Item item, COLORREF color) {
    colors[item] = color;
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

double MeaColors::ColorDifference(const MeaColors::Lab& color1, const MeaColors::Lab& color2) {
    // This implementation of the CIEDE2000 algorithm follows the implementation notes in
    // https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/ciede2000noteCRNA.pdf. Excerpts from this
    // paper and references to the equation numbers are included in the comments in this implementation.

    // Constants
    constexpr double pow25To7 = 6103515625.0;     // 25^7
    constexpr double deg360Rad = MeaNumericUtils::DegToRad(360.0);
    constexpr double deg180Rad = MeaNumericUtils::DegToRad(180.0);
    constexpr double deg30Rad = MeaNumericUtils::DegToRad(30.0);
    constexpr double deg63Rad = MeaNumericUtils::DegToRad(63.0);
    constexpr double deg6Rad = MeaNumericUtils::DegToRad(6.0);
    constexpr double deg25Rad = MeaNumericUtils::DegToRad(25.0);
    constexpr double deg275Rad = MeaNumericUtils::DegToRad(275.0);

    //
    // Step 1
    //

    // Equation 2
    double b1Squared = color1.b * color1.b;
    double b2Squared = color2.b * color2.b;
    double c1 = std::sqrt((color1.a * color1.a) + b1Squared);
    double c2 = std::sqrt((color2.a * color2.a) + b2Squared);

    // Equation 3
    double cAve = (c1 + c2) / 2.0;

    // Equation 4
    double cAvePow7 = std::pow(cAve, 7.0);
    double g = 0.5 * (1.0 - std::sqrt(cAvePow7 / (cAvePow7 + pow25To7)));

    // Equation 5
    double aPrime1 = (1.0 + g) * color1.a;
    double aPrime2 = (1.0 + g) * color2.a;

    // Equation 6
    double cPrime1 = std::sqrt((aPrime1 * aPrime1) + b1Squared);
    double cPrime2 = std::sqrt((aPrime2 * aPrime2) + b2Squared);

    // Equation 7
    auto hueAngle = [deg360Rad](const Lab& color, double aPrime) {
        if (MeaNumericUtils::IsZeroF(color.b) && MeaNumericUtils::IsZeroF(aPrime)) {
            return 0.0;
        }

        double hPrime = std::atan2(color.b, aPrime);

        // Per implementation clarification 1, add 360 degrees to negative hue angles
        if (hPrime < 0.0) {
            hPrime += deg360Rad;
        }

        return hPrime;
    };

    double hPrime1 = hueAngle(color1, aPrime1);
    double hPrime2 = hueAngle(color2, aPrime2);

    //
    // Step 2
    //

    // Equation 8
    double deltaLPrime = color2.l - color1.l;

    // Equation 9
    double deltaCPrime = cPrime2 - cPrime1;

    // Equation 10
    double cPrimeProduct = cPrime1 * cPrime2;
    double deltahPrime;

    if (MeaNumericUtils::IsZeroF(cPrimeProduct)) {
        deltahPrime = 0.0;
    } else {
        deltahPrime = hPrime2 - hPrime1;

        if (deltahPrime > deg180Rad) {
            deltahPrime -= deg360Rad;
        } else if (deltahPrime < -deg180Rad) {
            deltahPrime += deg360Rad;
        }
    }

    // Equation 11
    double deltaHPrime = 2.0 * std::sqrt(cPrimeProduct) * std::sin(deltahPrime / 2.0);

    //
    // Step 3
    //

    // Equation 12
    double lPrimeAve = (color1.l + color2.l) / 2.0;

    // Equation 13
    double cPrimeAve = (cPrime1 + cPrime2) / 2.0;

    // Equation 14
    double hPrimeSum = hPrime1 + hPrime2;
    double hPrimeAve;

    if (MeaNumericUtils::IsZeroF(cPrimeProduct)) {
        hPrimeAve = hPrimeSum;
    } else {
        if (std::fabs(hPrime1 - hPrime2) <= deg180Rad) {
            hPrimeAve = hPrimeSum / 2.0;
        } else {
            if (hPrimeSum < deg360Rad) {
                hPrimeAve = (hPrimeSum + deg360Rad) / 2.0;
            } else {
                hPrimeAve = (hPrimeSum - deg360Rad) / 2.0;
            }
        }
    }

    // Equation 15
    double t = 1.0
        - 0.17 * std::cos(hPrimeAve - deg30Rad)
        + 0.24 * std::cos(2.0 * hPrimeAve)
        + 0.32 * cos(3.0 * hPrimeAve + deg6Rad)
        - 0.20 * std::cos(4.0 * hPrimeAve - deg63Rad);

    // Equation 16
    double deltaTheta = deg30Rad
        * std::exp(-std::pow((hPrimeAve - deg275Rad) / deg25Rad, 2.0));

    // Equation 17
    double cPrimeAveTo7 = std::pow(cPrimeAve, 7.0);
    double rc = 2.0 * std::sqrt(cPrimeAveTo7 / (cPrimeAveTo7 + pow25To7));

    // Equation 18
    double lPrimeAveMinus50Squared = std::pow(lPrimeAve - 50.0, 2.0);
    double sl = 1.0 + 0.015 * lPrimeAveMinus50Squared / std::sqrt(20.0 + lPrimeAveMinus50Squared);

    // Equation 19
    double sc = 1.0 + 0.045 * cPrimeAve;

    // Equation 20
    double sh = 1.0 + 0.015 * cPrimeAve * t;

    // Equation 21
    double rt = -std::sin(2.0 * deltaTheta) * rc;

    // Equation 22
    // Use the reference conditions for the parametric weighting factors (i.e. kL=kC=kH=1.0).
    return std::sqrt(std::pow(deltaLPrime / sl, 2.0)
                     + std::pow(deltaCPrime / sc, 2.0)
                     + std::pow(deltaHPrime / sh, 2.0)
                     + rt * (deltaCPrime / sc) * (deltaHPrime / sh));
}

const MeaColors::ColorTableEntry* MeaColors::MatchBasicColor(COLORREF rgb) {
    // Typically colors are the same over adjacent pixels so remember the last match.
    static const ColorTableEntry* lastEntry = &basicWebColors[0];
    static COLORREF lastColor = lastEntry->rgb;

    if (lastColor != rgb) {
        lastEntry = MatchColor(basicWebColors, rgb);
        lastColor = rgb;
    }

    return lastEntry;
}

const MeaColors::ColorTableEntry* MeaColors::MatchExtendedColor(COLORREF rgb) {
    // Typically colors are the same over adjacent pixels so remember the last match.
    static const ColorTableEntry* lastEntry = &extendedWebColors[0];
    static COLORREF lastColor = lastEntry->rgb;

    if (lastColor != rgb) {
        lastEntry = MatchColor(extendedWebColors, rgb);
        lastColor = rgb;
    }

    return lastEntry;
}

const MeaColors::ColorTableEntry* MeaColors::MatchColor(const ColorTableEntry* table, COLORREF rgb) {
    Lab lab = RGBtoLab(rgb);

    double minDiff = DBL_MAX;
    const ColorTableEntry* bestEntry = nullptr;

    for (const ColorTableEntry* entry = table; entry->name != nullptr; entry++) {
        if (entry->rgb == rgb) {
            return entry;
        }

        double diff = ColorDifference(entry->lab, lab);
        if (diff < minDiff) {
            minDiff = diff;
            bestEntry = entry;
        }
    }

    return bestEntry;
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

MeaColors::XYZ MeaColors::RGBtoXYZ(COLORREF rgb) {
    double r = GetRValue(rgb) / 255.0;
    double g = GetGValue(rgb) / 255.0;
    double b = GetBValue(rgb) / 255.0;

    auto inverseCompanding = [](double value) {
        return (value > 0.04045) ? std::pow((value + 0.055) / 1.055, 2.4) : value / 12.92;
    };

    r = 100.0 * inverseCompanding(r);
    g = 100.0 * inverseCompanding(g);
    b = 100.0 * inverseCompanding(b);

    double x = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
    double y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
    double z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
    return XYZ(x, y, z);
}

MeaColors::Lab MeaColors::XYZtoLab(const MeaColors::XYZ& xyz) {
    // 1931 D65 2 degree illuminant reference.
    //
    constexpr double xref = 95.047;
    constexpr double yref = 100.000;
    constexpr double zref = 108.883;

    constexpr double k = 903.3 / 116.0;
    constexpr double m = 16.0 / 116.0;

    double x = xyz.x / xref;
    double y = xyz.y / yref;
    double z = xyz.z / zref;

    auto func = [k, m](double value) {
        return (value > 0.008856) ? std::cbrt(value) : k * value + m;
    };

    x = func(x);
    y = func(y);
    z = func(z);

    double l = (116.0 * y) - 16.0;
    double a = 500.0 * (x - y);
    double b = 200.0 * (y - z);
    return Lab(l, a, b);
}

MeaColors::Lab MeaColors::RGBtoLab(COLORREF rgb) {
    return XYZtoLab(RGBtoXYZ(rgb));
}
