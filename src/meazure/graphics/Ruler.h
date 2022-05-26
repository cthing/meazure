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
/// @brief Ruler graphic header file.

#pragma once

#include "Graphic.h"
#include <meazure/ui/ScreenProvider.h>
#include <meazure/units/UnitsProvider.h>


class MeaRuler;


/// Ruler drag callback mixin class. When the mouse drags the ruler, the
/// OnRulerMove method is called. The inheriting class overrides the
/// OnRulerMove method and actually moves the ruler to the new position. 
///
class MeaRulerCallback {

public:
    /// Callback information structure. A callback information structure
    /// is initialized and passed as the parameter in the callback method.
    ///
    struct RulerInfo {
        MeaRuler* ruler;    ///< Pointer to ruler object that initiated the callback.
        LONG position;      ///< Location of the ruler, in pixels. If the ruler orientation is vertical, the x
                            ///< position is reported. If the ruler is horizontal, the y position is reported.
        UINT id;            ///< User defined ID for the ruler.
        UINT flags;         ///< Virtual key flags including modifier keys (e.g. MK_CONTROL, MK_SHIFT).
    };


    /// Called when the user presses the left mouse button and
    /// then moves the mouse pointer while over the ruler
    /// (i.e. a drag operation). It is the responsibility of
    /// the callback method to actually moved the ruler to its
    /// new position as specified in the callback information
    /// structure.
    ///
    /// @param info     [in] Ruler information structure
    ///
    virtual void OnRulerMove(const RulerInfo* info);
};


/// A ruler element. The ruler resembles a classic measurement
/// straight edge. The ruler can be oriented vertically to
/// measure along the height of the screen or horizontally to
/// measure along the width of the screen. A ruler can be
/// positioned anywhere along the height or width of the screen.
/// Using the MeaRulerCallback mixin, a class can provide
/// dragging of the ruler.
///
/// Up to three position indicators can be displayed on the face
/// of the ruler. These indicators are used by the MeaRulerTool
/// to show the current position of crosshairs and other
/// measurement points.
///
/// The MeaRulerTool uses one vertical and one horizontal ruler
/// per monitor.
///
class MeaRuler : public MeaGraphic {

public:
    /// A ruler can display up to three position indicators. An
    /// indicator is a line that can be moved along the ruler to
    /// show a position of interest. Each indicator is selected
    /// by an identifier.
    ///
    enum IndicatorId {
        Ind1 = 0,    ///< Identifier for indicator 1
        Ind2 = 1,    ///< Identifier for indicator 2
        Ind3 = 2     ///< Identifier for indicator 3
    };

    /// A ruler can be oriented vertically to measure along the
    /// height of the screen, or horizontally to measure along the
    /// width of the screen.
    ///
    enum Orientation {
        Vertical,       ///< Orient the ruler vertically.
        Horizontal      ///< Orient the ruler horizontally.
    };

    enum {
        NumIndicators = 3   ///< Maximum number of indicators.
    };


    /// Constructs a ruler.
    /// 
    /// @param screenProvider   [in] Screen information provider
    /// @param unitsProvider    [in] Units information provider
    ///
    MeaRuler(const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unisProvider);

    /// Destroys a ruler.
    ///
    virtual ~MeaRuler();

    /// Creates the window for the ruler. This method must be called
    /// before the ruler can be displayed and before any operation that
    /// attempts to manipulate the ruler's window.
    ///
    /// @param borderColor  [in] The color for the ruler border and tick lines.
    /// @param backColor    [in] Background color for the ruler.
    /// @param opacity      [in] Opacity value for the ruler expressed as
    ///                     a value between 0 (transparent) and 255 (opaque)
    ///                     inclusive.
    /// @param orientation  [in] Orientation of the ruler: vertical or horizontal.
    /// @param targetRect   [in] Containment rectangle for the ruler. The ruler's
    ///                     longest dimension will be sized to fit within this
    ///                     rectangle.
    /// @param callback     [in] A class derived from MeaRulerCallback that
    ///                     wants to receive notification of ruler movement.
    ///                     Can be set to nullptr if no callback is desired.
    /// @param parent       [in] Parent window or <b>nullptr</b> if the ruler does
    ///                     not have an immediate parent. If a parent window is
    ///                     specified, the ruler is created as a child window
    ///                     (<b>WS_CHILD</b>). Otherwise the ruler is created
    ///                     as a popup window (<b>WS_POPUP</b>) and is assigned
    ///                     the main window as a parent.
    /// @param id           [in] Caller-defined ID for the ruler.
    ///
    /// @return <b>true</b> if the window is created successfully.
    ///
    bool Create(COLORREF borderColor, COLORREF backColor, BYTE opacity, Orientation orientation, 
                const CRect& targetRect, MeaRulerCallback* callback = nullptr, const CWnd* parent = nullptr,
                UINT id = 0xffff);

    /// Sets the position of the ruler.
    ///
    /// @param position     [in] Location for the ruler, in pixels. If the ruler
    ///                     is oriented vertically, the position parameter specifies
    ///                     the location of the left side of the ruler. If the ruler is
    ///                     horizontal, the position parameter specified the location
    ///                     of the top of the ruler.
    ///
    void SetPosition(int position);

    /// Obtains the position of the ruler.
    ///
    /// @return     Location of the ruler, in pixels. If the ruler is oriented
    ///             vertically, the location of the left side of the ruler is
    ///             returned. If the ruler is horizontal, the location of the top
    ///             of the ruler is returned.
    ///
    int GetPosition() const { return m_position; }

    /// Obtains the orientation of the ruler.
    ///
    /// @return     The ruler is either vertically or horizontally oriented.
    ///
    Orientation GetOrientation() const { return m_orientation; }

    /// Sets the position of the specified ruler indicator. Up to three position
    /// indicators can be displayed on the face of the ruler. These indicators
    /// are used by the MeaRulerTool to show the current position of crosshairs
    /// and other measurement points. Specifying a position for an indicator
    /// displays that indicator. To hide an indicator call ClearIndicator().
    ///
    /// @param indId    [in] Identifier of the indicator whose position is to be set.
    /// @param pixel    [in] Position for the indicator, in pixels.
    ///
    void SetIndicator(IndicatorId indId, int pixel);

    /// Hides the specified position indicator.
    ///
    /// @param indId    [in] Identifier of the indicator to be hidden.
    ///
    void ClearIndicator(IndicatorId indId);

    /// Sets the colors for the ruler.
    ///
    /// @param borderColor  [in] Color for the ruler border and tick lines
    /// @param backColor    [in] Background color for the ruler
    ///
    void SetColors(COLORREF borderColor, COLORREF backColor);

    /// Sets the opacity for the ruler.
    ///
    /// @param opacity      [in] Opacity value between 0 (transparent)
    ///                     and 255 (opaque) inclusive
    ///
    void SetOpacity(BYTE opacity);

    /// Redraws the ruler.
    ///
    void Update() {
        if (m_hWnd != nullptr) {
            Invalidate(FALSE);
            UpdateWindow();
        }
    }

protected:
    DECLARE_MESSAGE_MAP()

    /// Called when the ruler window is about to be destroyed.
    /// If the ruler was created with a parent window specified,
    /// a number of bitmaps were created to simulate an opacity effect.
    /// This method deletes those bitmaps.
    ///
    afx_msg void OnDestroy();

    /// Called when the ruler needs repainting.
    ///
    afx_msg void OnPaint();

    /// Called when the ruler is dragged. The method
    /// initializes the ruler information structure and calls
    /// the callback MeaRulerCallback::OnRulerMove method, if a
    /// callback has been specified.
    ///
    /// @param flags    [in] Virtual key state
    /// @param point    [in] Location of the pointer, in pixels
    ///
    afx_msg void OnMouseMove(UINT flags, CPoint point);

    /// Called when the left mouse button is pressed while the
    /// pointer is over the ruler window.
    ///
    /// @param flags    [in] Virtual key state
    /// @param point    [in] Location of the pointer, in pixels
    ///
    afx_msg void OnLButtonDown(UINT flags, CPoint point);

    /// Called when the left mouse button is released while the
    /// pointer is captured by the ruler window.
    ///
    /// @param flags    [in] Virtual key state
    /// @param point    [in] Location of the pointer, in pixels
    ///
    afx_msg void OnLButtonUp(UINT flags, CPoint point);

private:
    static constexpr int kMinMajorTickHeight { 8 };     ///< Minimum height of the major tick marks, in pixels.
    static constexpr double kMajorTickHeight { 0.08 };  ///< Height of the major tick marks, in inches.
    static constexpr int kMinMinorTickHeight { 5 };     ///< Minimum height of the minor tick marks, in pixels.
    static constexpr double kMinorTickHeight { 0.05 };  ///< Height of the minor tick marks, in inches.
    static constexpr int kMinMargin { 1 };              ///< Minimum label height margin, in pixels.
    static constexpr double kMargin { 0.01 };           ///< Label height margin, in inches.


    /// Specifies the location of the ruler number labels. For horizontally
    /// oriented rulers, the labels may be position on the top or bottom
    /// relative to the tick marks. For vertical rulers, the labels may be
    /// positioned on the left or right relative to the tick marks. The
    /// positioning of the labels is determined automatically based on the
    /// position of the ruler on the screen. For example, a horizontal ruler
    /// located in the upper half of the screen has its labels positioned on
    /// the top, whereas the same ruler located in the lower half of the
    /// screen has labels on the bottom.
    ///
    enum LabelPosition {
        Top,        ///< Position number labels above the ruler tick marks
        Bottom,     ///< Position number labels below the ruler tick marks
        Left,       ///< Position number labels to the left of the ruler tick marks
        Right       ///< Position number labels to the right of the ruler tick marks.
    };


    /// Draws the specified position indicator on the ruler at the current
    /// position for that indicator.
    ///
    /// @param indId    [in] Specifies the indicator to be drawn.
    /// @param dc       [in] Specifies the device context to use
    ///                 to draw the indicator.
    ///
    void DrawIndicator(IndicatorId indId, CDC& dc);

    /// Draws the ruler tick marks and number labels.
    ///
    /// @param dc       [in] Specifies the device context to use
    ///                 to draw the ruler.
    ///
    void DrawRuler(CDC& dc);

    /// Fills the ruler information structure so that it can be
    /// passed to the callback method.
    ///
    /// @param rulerInfo    [in] Ruler information structure to be filled.
    /// @param flags        [in] Virtual key flags including modifier keys (e.g. MK_CONTROL, MK_SHIFT).
    /// @param point        [in] Location of the mouse pointer, in pixels.
    ///
    void FillInfo(MeaRulerCallback::RulerInfo& rulerInfo, UINT flags, const CPoint& point);

    const MeaScreenProvider& m_screenProvider;  ///< Screen information provider
    const MeaUnitsProvider& m_unitsProvider;    ///< Units information provider
    COLORREF m_borderColor;                     ///< Color for the ruler border and tick marks.
    COLORREF m_backColor;                       ///< Color for the ruler background.
    SIZE m_majorTickHeight;                     ///< Height of the major tick marks, in pixels.
    SIZE m_minorTickHeight;                     ///< Height of the minor tick marks, in pixels.
    SIZE m_margin;                              ///< Height spacing between tick marks, number label and border, in pixels.
    MeaRulerCallback* m_callback;               ///< Object to be called on ruler drag
    Orientation m_orientation;                  ///< Orientation of the rule (i.e. horizontal or vertical)
    int m_position;                             ///< Position of the ruler, in pixels
    CRect m_targetRect;                         ///< Containment rectangle for the ruler. The ruler's longest
                                                ///< dimension will be sized to fit within this rectangle.
    LabelPosition m_labelPosition;              ///< Current location of the number labels.
    CFont m_hFont;                              ///< Font for horizontal ruler labels.
    CFont m_vFont;                              ///< Font for vertical ruler labels.
    int m_fontHeight;                           ///< Height of the current font, in pixels.
    int m_thk;                                  ///< Thickness of the ruler (i.e. its minor dimension), in pixels.
    int m_indicatorLoc[NumIndicators];          ///< Location of each position indicator, in pixels.
    bool m_mouseCaptured;                       ///< Indicates whether the mouse is currently captured by the ruler window.
    CSize m_pointerOffset;                      ///< Offset between pointer position and ruler edge.
    BYTE m_opacity;                             ///< Current ruler opacity setting (0 - 255).
    CDC m_rulerDC;                              ///< Ruler device context
    CDC m_backDC;                               ///< Background device context for alpha blending when the ruler is a child window
    CBitmap m_rulerBitmap;                      ///< Bitmap into which to draw the ruler when it is a child window
    CBitmap* m_origRulerBitmap;                 ///< Original bitmap for the ruler
    CBitmap m_backBitmap;                       ///< Bitmap for the background when alpha blending when the ruler is a child window
    CBitmap* m_origBackBitmap;              ///< Original background bitmap 
};
