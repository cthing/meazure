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

/// @file
/// @brief Header file for color style class.

#pragma once

#include "Profile.h"
#include <map>


/// Represents a color in the Hue-Saturation-Lightness color space.
/// All values are between 0.0 and 1.0.
///
typedef struct {
    double hue;
    double saturation;
    double lightness;
} HSL;


/// A color style sheet class that provides the colors and opacities used
/// by the crosshairs, lines and other graphic elements.
///
class MeaColors {
public:
    /// Identifies the item whose color and opacity are maintained by this class.
    ///
    enum Item {
        LineFore,           ///< Line foreground color.
        CrossHairBack,      ///< Crosshair background color.
        CrossHairBorder,    ///< Crosshair edge color.
        CrossHairHilite,    ///< Crosshair hilite color.
        CrossHairOpacity,   ///< Opacity of the crosshair.
        RulerBack,          ///< Ruler background color.
        RulerBorder,        ///< Ruler edge and tick mark color.
        RulerOpacity        ///< Ruler opacity.
    };


    /// Initializes the class by establishing the default colors.
    ///
    static void Initialize();
    
    /// Resets all colors to their default values.
    ///
    static void Reset() {
        m_colors = m_defColors;
    }


    /// Persists the color settings to the specified profile.
    ///
    /// @param profile      [in] Profile into which the color settings
    ///                     are persisted.
    ///
    static void SaveProfile(MeaProfile& profile);

    /// Restores the color settings from the specified profile.
    ///
    /// @param profile      [in] Profile from which the color settings
    ///                     are restored.
    ///
    static void LoadProfile(MeaProfile& profile);

    /// Called when the master application reset is invoked. The colors
    /// are reset to their default values.
    ///
    static void MasterReset();


    /// Sets the specified item to the specified color.
    /// @param item     [in] Color item to set.
    /// @param clr      [in] Color to set.
    static void     Set(Item item, COLORREF clr) { m_colors[item] = clr; }

    /// Sets the specified item to the specified opacity
    /// @param item     [in] Item whose opacity is to be set.
    /// @param opacity  [in] Opacity to set, where 0 is completely transparent
    ///                 and 255 is completely opaque.
    static void     SetA(Item item, BYTE opacity) { m_colors[item] = RGB(opacity, 0, 0); }


    /// Returns the color of the specified item.
    /// @param item     [in] Item whose color is desired.
    /// @return Color of the specified item.
    static COLORREF Get(Item item) { return m_colors[item]; }

    /// Returns the value of the red color component of the specified item.
    /// @param item     [in] Item whose color component is desired.
    /// @return Red color component of the specified item.
    static BYTE     GetR(Item item) { return GetRValue(m_colors[item]); }

    /// Returns the value of the green color component of the specified item.
    /// @param item     [in] Item whose color component is desired.
    /// @return Green color component of the specified item.
    static BYTE     GetG(Item item) { return GetGValue(m_colors[item]); }

    /// Returns the value of the blue color component of the specified item.
    /// @param item     [in] Item whose color component is desired.
    /// @return Blue color component of the specified item.
    static BYTE     GetB(Item item) { return GetBValue(m_colors[item]); }

    /// Returns the opacity of the specified item.
    /// @param item     [in] Item whose opacity is desired.
    /// @return Opacity of the specified item, where 0 is completely
    ///         transparent and 255 is completely opaque.
    static BYTE     GetA(Item item) { return GetRValue(m_colors[item]); }

    /// Returns the default color for the specified item.
    /// @param item     [in] Item whose default color is desired.
    /// @return Default color for the specified item.
    static COLORREF GetDef(Item item) {
        return m_defColors[item];
    }


    /// Performs linear interpolation between the specified RGB color.
    /// The interpolation is performed in HSL space, which provides a
    /// more visually appealing result.
    ///
    /// @param startRGB     [in] Starting interpolation point.
    /// @param endRGB       [in] Ending interpolation point.
    /// @param percent      [in] Interpolation point between startRGB and endRGB as a percentage.
    ///
    /// @return Interpolated color.
    ///
    static COLORREF InterpolateColor(COLORREF startRGB, COLORREF endRGB, int percent);


    /// Converts from the RGB color space to the HSL color space.
    ///
    /// @param rgb      [in] RGB colors as values between 0 and 255.
    /// @param hsl      [out] HSL colors as values between 0.0 and 1.0
    ///
    static void RGBtoHSL(COLORREF rgb, HSL& hsl);

    /// Converts from the HSL color space to the RGB color space.
    ///
    /// @param hsl      [in] HSL colors as values between 0.0 and 1.0.
    /// @param rgb      [out] RGB colors as values between 0 and 255.
    ///
    static void HSLtoRGB(const HSL& hsl, COLORREF& rgb);

private:
    /// All members of this class are static. No instances
    /// of this class are ever created.
    ///
    MeaColors() { }

    /// All members of this class are static. No instances
    /// of this class are ever created.
    ///
    ~MeaColors() { }

   typedef std::map<Item, COLORREF> Colors;        ///< Maps items to their colors.

    /// Determines the minimum of two values.
    /// @return Minimum value.
    static double Min(double a, double b) { return ((a < b) ? a : b); }

    /// Determines the maximum of two values.
    /// @return Maximum value.
    static double Max(double a, double b) { return ((a > b) ? a : b); }


    /// Converts a hue to a RGB component value based on the specified
    /// weighting factors.
    /// @param m1       [in] Weighting factor between lightness and saturation.
    /// @param m2       [in] Weighting factor between lightness and saturation.
    /// @param h        [in] Hue value.
    /// @return Either an R, G, or B value between 0.0 and 1.0. The
    ///         component returned depends on the weighting factors
    ///         specified.
    static double HuetoRGB(double m1, double m2, double h);

    static Colors   m_defColors;    ///< Map of default colors. 
    static Colors   m_colors;       ///< Map of active colors.
};
