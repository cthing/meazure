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

 /// @file
 /// @brief Data field identifiers.

#pragma once


/// Each data text field on the display is specified by one of these identifiers. Internally each identifier is used
/// as a mask and as such, must be consecutive powers of 2.
///
enum MeaDataFieldId {
    MeaX1Field = 0x0001,            ///< X1 coordinate data field.
    MeaY1Field = 0x0002,            ///< Y1 coordinate data field.
    MeaX2Field = 0x0004,            ///< X2 coordinate data field.
    MeaY2Field = 0x0008,            ///< Y2 coordinate data field.
    MeaXVField = 0x0010,            ///< Vertex or center X coordinate data field.
    MeaYVField = 0x0020,            ///< Vertex or center Y coordinate data field.
    MeaWidthField = 0x0040,         ///< Width data field.
    MeaHeightField = 0x0080,        ///< Height data field.
    MeaDistanceField = 0x0100,      ///< Distance data field.
    MeaAngleField = 0x0200,         ///< Angle data field.
    MeaAreaField = 0x0400,          ///< Area data field.
    MeaScreenWidthField = 0x0800,   ///< Screen width data field.
    MeaScreenHeightField = 0x1000,  ///< Screen height data field.
    MeaResXField = 0x2000,          ///< Data field for the screen resolution in the X dimension.
    MeaResYField = 0x4000,          ///< Data field for the screen resolution in the Y dimension.
    MeaAspectField = 0x8000         ///< Aspect ratio field.
};
