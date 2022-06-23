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

// wingdi.h defines a CMYK macro.
#ifdef CMYK
#undef CMYK
#endif


/// A color style sheet class that provides the colors and opacities used
/// by the crosshairs, lines and other graphic elements.
///
namespace MeaColors {

    /// Represents a color in the Cyan-Magenta-Yellow color space.
    ///
    struct CMY {
        int cyan;
        int magenta;
        int yellow;

        CMY() : cyan(0), magenta(0), yellow(0) {}
        CMY(int c, int m, int y) : cyan(c), magenta(m), yellow(y) {}
    };

    /// Represents a color in the Cyan-Magenta-Yellow-Black color space. By convention, the letter "K" represents
    /// the black component.
    /// 
    struct CMYK {
        int cyan;
        int magenta;
        int yellow;
        int black;

        CMYK() : cyan(0), magenta(0), yellow(0), black(0) {}
        CMYK(int c, int m, int y, int k) : cyan(c), magenta(m), yellow(y), black(k) {}
    };

    /// Represents a color in the Hue (H), Saturation (S), and Lightness (L) color space.
    ///
    struct HSL {
        int hue;
        int saturation;
        int lightness;

        HSL() : hue(0), saturation(0), lightness(0) {}
        HSL(int h, int s, int l) : hue(h), saturation(s), lightness(l) {}
    };

    /// Represents a color in the Luminance (Y), Blue-Difference (Cb) and Red-Difference (Cr) chrominance color
    /// space. See https://en.wikipedia.org/wiki/YCbCr for more information.
    /// 
    struct YCbCr {
        int y;
        int cb;
        int cr;

        YCbCr() : y(0), cb(0), cr(0) {}
        YCbCr(int luma, int blueChroma, int redChroma) : y(luma), cb(blueChroma), cr(redChroma) {}
    };

    /// Represents a color in the Luminance (Y), In-phase (I) and Quadrature (Q) chrominance color space.
    /// 
    struct YIQ {
        int y;
        int i;
        int q;

        YIQ() : y(0), i(0), q(0) {}
        YIQ(int luma, int inphase, int quadrature) : y(luma), i(inphase), q(quadrature) {}
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
    void SetA(Item item, std::uint8_t opacity);

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
    std::uint8_t GetR(Item item);

    /// Returns the value of the green color component of the specified item.
    /// 
    /// @param item     [in] Item whose color component is desired.
    /// @return Green color component of the specified item.
    /// 
    std::uint8_t GetG(Item item);

    /// Returns the value of the blue color component of the specified item.
    /// 
    /// @param item     [in] Item whose color component is desired.
    /// @return Blue color component of the specified item.
    /// 
    std::uint8_t GetB(Item item);

    /// Returns the opacity of the specified item.
    /// 
    /// @param item     [in] Item whose opacity is desired.
    /// @return Opacity of the specified item, where 0 is completely transparent and 255 is completely opaque.
    /// 
    std::uint8_t GetA(Item item);

    /// Returns the default color for the specified item.
    /// 
    /// @param item     [in] Item whose default color is desired.
    /// @return Default color for the specified item.
    /// 
    COLORREF GetDefault(Item item);

    /// Performs linear interpolation between the specified RGB color. The interpolation is performed in HSL space,
    /// which provides a more visually appealing result. The result of the interpolation is converted back to RGB.
    ///
    /// @param startRGB     [in] Starting interpolation point.
    /// @param endRGB       [in] Ending interpolation point.
    /// @param percent      [in] Interpolation point between startRGB and endRGB as a percentage.
    /// @return Interpolated color.
    ///
    COLORREF InterpolateColor(COLORREF startRGB, COLORREF endRGB, int percent);

    /// Converts from the RGB color space to the Cyan (C), Magenta (M) and Yellow (Y) color space. The conversion is
    /// done using the algorithm for RGB to CMY conversion presented at http://www.easyrgb.com/en/math.php. The input
    /// and output range is [0, 255].
    /// 
    /// @param rgb      [in] RGB color
    /// @return CMY color
    /// 
    CMY RGBtoCMY(COLORREF rgb);

    /// Converts from the RGB color space to the Cyan (C), Magenta (M), Yellow (Y) and Black (K) color space. The
    /// conversion is done using the algorithm presetend at http://www.easyrgb.com/en/math.php. The input and output
    /// range is [0, 255].
    /// 
    /// @param rgb      [in] RGB color
    /// @return CMYK color
    /// 
    CMYK RGBtoCMYK(COLORREF rgb);

    /// Converts from the RGB color space to the Hue (H), Saturation (S) and Lightness (L) color space. The conversion
    /// is done using the algorithm for RGB to HSL conversion presented at http://www.easyrgb.com/en/math.php. The
    /// input range is [0, 255]. The output range is scaled such that H values are in degrees in the range [0, 360),
    /// and S and L values are percentages in the range [0, 100].
    ///
    /// @param rgb      [in] RGB color
    /// @return HSL color
    ///
    HSL RGBtoHSL(COLORREF rgb);

    /// Converts from the HSL color space to the RGB color space. The conversion is done using the algorithm for HSL
    /// to RGB conversion presented at http://www.easyrgb.com/en/math.php. The input range is H in degrees in the
    /// range [0, 360), and S and L as percentages in the range [0, 100]. The output range is [0, 255].
    ///
    /// @param hsl      [in] HSL color.
    /// @return RGB color
    ///
    COLORREF HSLtoRGB(const HSL& hsl);

    /// Converts from the RGB color space to the Luminance (Y), Blue-Difference (Cb), Red-Difference (Cr) color space.
    /// The conversion is done using the ITU-R BT.601 coefficients for digital YCbCr from digital RGB with input
    /// range [0, 255]. The output range is [16, 235] for Y, and [16, 240] for both Cb and Cr)values. See
    /// https://en.wikipedia.org/wiki/YCbCr for more information.
    ///
    /// @param rgb      [in] RGB color
    /// @return YCbCr color
    ///
    YCbCr RGBtoYCbCr(COLORREF rgb);

    /// Converts from the RGB color space to the Luminance (Y), In-phase (I), Quadrature (Q) color space. The
    /// conversion is done using the NTSC 1953 colorimetry coefficients. The input range is [0, 255]. The output range
    /// is [0, 255] for Y, [-152, 152] I and [-133, 133] for quadrature. See https://en.wikipedia.org/wiki/YIQ for
    /// more information.
    /// 
    /// @param rgb      [in] RGB color
    /// @return YIQ color
    /// 
    YIQ RGBtoYIQ(COLORREF rgb);
};
