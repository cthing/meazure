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

#include <meazure/profile/Profile.h>
#include <map>


/// A color style sheet class that provides the colors and opacities used
/// by the crosshairs, lines and other graphic elements.
///
namespace MeaColors {

    /// Represents a color in the Hue-Saturation-Lightness color space.
    /// All values are between 0.0 and 1.0.
    ///
    struct HSL {
        double hue;
        double saturation;
        double lightness;

        HSL() : hue(0.0), saturation(0.0), lightness(0.0) {}
        HSL(double h, double s, double l) : hue(h), saturation(s), lightness(l) {}
    };


    /// Identifies the item whose color and opacity are maintained by this class.
    ///
    enum Item {
        LineFore,           ///< Line foreground color.
        CrossHairBack,      ///< Crosshair background color.
        CrossHairBorder,    ///< Crosshair edge color.
        CrossHairHilite,    ///< Crosshair highlight color.
        CrossHairOpacity,   ///< Opacity of the crosshair.
        RulerBack,          ///< Ruler background color.
        RulerBorder,        ///< Ruler edge and tick mark color.
        RulerOpacity        ///< Ruler opacity.
    };


    /// Resets all colors to their default values.
    ///
    void Reset();

    /// Persists the color settings to the specified profile.
    ///
    /// @param profile      [in] Profile into which the color settings
    ///                     are persisted.
    ///
    void SaveProfile(MeaProfile& profile);

    /// Restores the color settings from the specified profile.
    ///
    /// @param profile      [in] Profile from which the color settings
    ///                     are restored.
    ///
    void LoadProfile(MeaProfile& profile);

    /// Called when the master application reset is invoked. The colors
    /// are reset to their default values.
    ///
    void MasterReset();

    /// Sets the specified item to the specified color.
    /// 
    /// @param item     [in] Color item to set.
    /// @param clr      [in] Color to set.
    /// 
    void Set(Item item, COLORREF clr);

    /// Sets the specified item to the specified opacity.
    /// 
    /// @param item     [in] Item whose opacity is to be set.
    /// @param opacity  [in] Opacity to set, where 0 is completely transparent and 255 is completely opaque.
    /// 
    void SetA(Item item, BYTE opacity);

    /// Returns the color of the specified item.
    /// 
    /// @param item     [in] Item whose color is desired.
    /// @return Color of the specified item.
    /// 
    COLORREF Get(Item item);

    /// Returns the value of the red color component of the specified item.
    /// 
    /// @param item     [in] Item whose color component is desired.
    /// @return Red color component of the specified item.
    /// 
    BYTE GetR(Item item);

    /// Returns the value of the green color component of the specified item.
    /// 
    /// @param item     [in] Item whose color component is desired.
    /// @return Green color component of the specified item.
    /// 
    BYTE GetG(Item item);

    /// Returns the value of the blue color component of the specified item.
    /// 
    /// @param item     [in] Item whose color component is desired.
    /// @return Blue color component of the specified item.
    /// 
    BYTE GetB(Item item);

    /// Returns the opacity of the specified item.
    /// 
    /// @param item     [in] Item whose opacity is desired.
    /// @return Opacity of the specified item, where 0 is completely transparent and 255 is completely opaque.
    /// 
    BYTE GetA(Item item);

    /// Returns the default color for the specified item.
    /// 
    /// @param item     [in] Item whose default color is desired.
    /// @return Default color for the specified item.
    /// 
    COLORREF GetDefault(Item item);

    /// Performs linear interpolation between the specified RGB color. The interpolation is performed in HSL space,
    /// which provides a more visually appealing result.
    ///
    /// @param startRGB     [in] Starting interpolation point.
    /// @param endRGB       [in] Ending interpolation point.
    /// @param percent      [in] Interpolation point between startRGB and endRGB as a percentage.
    /// @return Interpolated color.
    ///
    COLORREF InterpolateColor(COLORREF startRGB, COLORREF endRGB, int percent);

    /// Converts from the RGB color space to the HSL color space.
    ///
    /// @param rgb      [in] RGB colors as values between 0 and 255.
    /// @return HSL colors as values between 0.0 and 1.0
    ///
    HSL RGBtoHSL(COLORREF rgb);

    /// Converts from the HSL color space to the RGB color space.
    ///
    /// @param hsl      [in] HSL colors as values between 0.0 and 1.0.
    /// @return RGB colors as values between 0 and 255.
    ///
    COLORREF HSLtoRGB(const HSL& hsl);
};
