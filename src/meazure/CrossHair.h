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
/// @brief Crosshair graphic header file.

#pragma once

#include "Graphic.h"


class MeaCrossHair;


/// Crosshair activity callback mixin class. A class wishing to receive
/// notification of crosshair events (e.g. movement), can inherit from
/// this class and override the On* methods of interest.
///
class MeaCrossHairCallback
{
public:
    /// Callback information structure. A callback information structure
    /// is initialized and passed as the parameter in each callback
    /// method.
    ///
    struct CHInfo {
        MeaCrossHair    *ch;            ///< Pointer to crosshair object that initiated the callback
        CPoint          centerPoint;    ///< Location of the crosshair center point, in pixels
        UINT            id;             ///< User defined ID for the crosshair
        UINT            flags;          ///< Virtual key flags including modifier keys (e.g. MK_CONTROL, MK_SHIFT)
    };


    /// Constructs a callback object.
    ///
    MeaCrossHairCallback() { }

    /// Destroys a callback object.
    ///
    virtual ~MeaCrossHairCallback() { }


    /// Called when the left mouse button is pressed while
    /// the pointer is positioned over the crosshair.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHSelect(const CHInfo *info);

    /// Called when the left mouse button is release while
    /// the pointer is positioned over the crosshair.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHDeselect(const CHInfo *info);

    /// Called when the user moves the mouse pointer over
    /// the crosshair while the left mouse button is pressed
    /// (i.e. a drag operation). It is the responsibility of
    /// the callback method to actually moved the crosshair to
    /// its new position as specified in the callback information
    /// structure.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHMove(const CHInfo *info);

    /// Called when the pointer enters the crosshair window.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHEnter(const CHInfo *info);

    /// Called when the pointer leaves the crosshair window.
    ///
    /// @param info     [in] Crosshair information structure
    ///
    virtual void OnCHLeave(const CHInfo *info);
};


/// A crosshair element. A crosshair consists of four triangular
/// window segments arranged vertically and horizontally such
/// that each triangle is aimed at a center point. The crosshair
/// is used by a number of the measurement tools to identify their
/// measurement points and to allow the user to perform measurements
/// by dragging the crosshairs using the pointer.
///
class MeaCrossHair : public MeaGraphic
{
public:
    static const int kFlashCount;       ///< Number of times to flash the crosshair when Flash() is called
    static const int kStrobeCount;      ///< Number of times to flash the crosshair when Strobe() is called
    

    /// Constructs a crosshair. Prior to displaying the crosshair with
    /// the Show() method, the Create() method must be called to
    /// create the crosshair's window.
    ///
    MeaCrossHair();

    /// Destroys the circle.
    ///
    virtual ~MeaCrossHair();


    /// Creates the window for the crosshair. This method must be called
    /// before the crosshair can be displayed and before any operation that
    /// attempts to manipulate the crosshair's window.
    ///
    /// @param borderColor  [in] The color for the crosshair border lines. The
    ///                     border lines are drawn around each triangular section
    ///                     of the crosshair.
    /// @param backColor    [in] Background color for the crosshair. Each
    ///                     triangular section of the crosshair is drawn in the
    ///                     background color. The crosshair is displayed in the
    ///                     background color when the pointer is not over the
    ///                     crosshair.
    /// @param hiliteColor  [in] Background color for the crosshair when the
    ///                     pointer is over the crosshair.
    /// @param opacity      [in] Opacity value for the crosshair expressed as
    ///                     a value between 0 (transparent) and 255 (opaque)
    ///                     inclusive.
    /// @param callback     [in] A class derived from MeaCrossHairCallback that
    ///                     wants to receive notification of crosshair events.
    ///                     Can be set to NULL if no callback is desired.
    /// @param parent       [in] Parent window or <b>NULL</b> if the crosshair does
    ///                     not have an immediate parent. If a parent window is
    ///                     specified, the crosshair is created as a child window
    ///                     (<b>WS_CHILD</b>). Otherwise the crosshair is created
    ///                     as a popup window (<b>WS_POPUP</b>) and is assigned
    ///                     the main window as a parent.
    /// @param toolTipId    [in] String resource ID for the tooltip associated
    ///                     with the crosshair.
    /// @param id           [in] Caller-defined ID for the crosshair.
    ///
    /// @return <b>true</b> if the window is created successfully.
    ///
    bool    Create(COLORREF borderColor, COLORREF backColor, COLORREF hiliteColor,
                    BYTE opacity, MeaCrossHairCallback *callback,
                    const CWnd *parent = NULL, UINT toolTipId = 0xffff,
                    UINT id = 0xffff);


    /// Visually flashes the crosshair by cycling its background
    /// between normal and hilited the number of times specified.
    ///
    /// @param flashCount   [in] Number of times to cycle the
    ///                     crosshair background.
    ///
    void    Flash(int flashCount = kFlashCount);


    /// Sets the position of the crosshair.
    ///
    /// @param center   [in] Position for the center of the crosshair, in pixels.
    ///
    void    SetPosition(const POINT& center);

    /// Sets the colors for the crosshair.
    ///
    /// @param borderColor  [in] Color for the crosshair border lines
    /// @param backColor    [in] Normal background color
    /// @param hiliteColor  [in] Highlight background color
    ///
    void    SetColors(COLORREF borderColor, COLORREF backColor, COLORREF hiliteColor);

    /// Sets the opacity for the crosshair.
    ///
    /// @param opacity      [in] Opacity value between 0 (transparent)
    ///                     and 255 (opaque) inclusive
    ///
    void    SetOpacity(BYTE opacity);

    /// Relays messages to the crosshair's tooltip.
    ///
    /// @param pMsg     [in] Window message to relay
    ///
    /// @return TRUE if message was processed.
    ///
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    DECLARE_MESSAGE_MAP()

    /// Called when the crosshair window is about to be destroyed.
    /// If the crosshair was created with a parent window specified,
    /// a number of bitmaps were created to simulate an opacity effect.
    /// This method deletes those bitmaps.
    ///
    afx_msg void OnDestroy();

    /// Called when the crosshair needs repainting.
    ///
    afx_msg void OnPaint();

    /// Called when the crosshair is dragged. The method
    /// initializes the crosshair information structure and calls
    /// the callback MeaCrossHairCallback::OnCHMove method, if a
    /// callback has been specified.
    ///
    /// @param flags    [in] Virtual key state
    /// @param point    [in] Location of the pointer, in pixels
    ///
    afx_msg void OnMouseMove(UINT flags, CPoint point);

    /// Called when the pointer leaves the crosshair window. The
    /// method initializes the crosshair information structure
    /// and calls the callback MeaCrossHairCallback::OnCHLeave,
    /// if a callback has been specified.
    ///
    /// @param wParam   [in] Not used
    /// @param lParam   [in] Not used
    ///
    /// @return Always zero.
    ///
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

    /// Called when the left mouse button is pressed while the
    /// pointer is over the crosshair window.
    ///
    /// @param flags    [in] Virtual key state
    /// @param point    [in] Location of the pointer, in pixels
    ///
    afx_msg void OnLButtonDown(UINT flags, CPoint point);

    /// Called when the left mouse button is released while the
    /// pointer is captured by the crosshair window.
    ///
    /// @param flags    [in] Virtual key state
    /// @param point    [in] Location of the pointer, in pixels
    ///
    afx_msg void OnLButtonUp(UINT flags, CPoint point);

    /// Called when the crosshair is cycling between normal
    /// and highlighted display as a result of a call to the Flash()
    /// method. This method redraws the crosshair in the
    /// appropriate background color.
    ///
    /// @param timerId  [in] ID for the timer that has expired.
    ///
    afx_msg void OnTimer(UINT_PTR timerId);

private:
    /// Specifies how to draw the crosshair (i.e. normally or
    /// inverted).
    ///
    enum DrawState {
        Normal,     ///< Draw the crosshair in its normal appearance.
        Inverted    ///< Draw the crosshair in its inverted appearance.
    };


    /// Given the center of the crosshair, returns the corresponding
    /// upper left corner.
    ///
    /// @param center   [in] Location of the center of the crosshair, in pixels
    ///
    /// @return Location of the upper left corner of the crosshair window,
    ///         in pixels.
    ///
    static CPoint   GetLeftTop(const CPoint& center) { return center - m_halfSize; }


    static SIZE     m_size;             ///< Width and height of the crosshair, in pixels
    static SIZE     m_halfSize;         ///< Half the width and height of the crosshair, in pixels
    static SIZE     m_spread;           ///< Half the length of the base of a triangular section of the crosshair, in pixels
    static UINT     m_flashInterval;    ///< Number of milliseconds to hold each display state while flashing the crosshair
    static int      *m_numCoords;       ///< Number of coordinates in each rectangle making up part of the crosshair (4)


    /// Forms the window into the shape of the crosshair. A series of
    /// rectangular regions are aggregated together to form the four
    /// petals of the crosshair.
    ///
    void    SetRegion();

    /// Draws the crosshair background and border. Before this method
    /// is called, the shape of the crosshair has already been set by
    /// the SetRegion() method. The DrawCrossHair() method simply draws
    /// the appropriate background color and frames the window's region.
    /// The background color depends on whether the crosshair is in a
    /// highlight state or not.
    ///
    /// @param dc   [in] Device context into which the crosshair is drawn
    ///
    void    DrawCrossHair(CDC& dc);

    /// Deletes the brushes used to draw the crosshair's background and
    /// border.
    ///
    void    DestroyColors();

    /// Fills in the crosshair information structure with the current
    /// location of the pointer and the state of the keyboard modifiers.
    ///
    /// @param chs      [in, out] Crosshair information structure to fill
    /// @param flags    [in] State of the keyboard modifier keys
    /// @param point    [in] Current location of the pointer, in pixels
    ///
    void    FillInfo(MeaCrossHairCallback::CHInfo& chs, UINT flags, const CPoint& point);


    MeaCrossHairCallback    *m_callback;        ///< Object to call for crosshair events
    bool                    m_mouseCaptured;    ///< Indicates if the pointer is captured
    bool                    m_mouseOver;        ///< Indicates if the pointer is over the crosshair
    CBrush                  *m_backBrush;       ///< Brush to paint the normal crosshair background
    CBrush                  *m_borderBrush;     ///< Brush to paint the crosshair border
    CBrush                  *m_hiliteBrush;     ///< Brush to paint the highlighted crosshair background
    CSize                   m_pointerOffset;    ///< Offset from the pointer location in the window to the center of the crosshair
    DrawState               m_drawState;        ///< Indicates how the crosshair should be drawn
    int                     m_flashCount;       ///< Number of times to flash the crosshair
    CToolTipCtrl            m_toolTip;          ///< Tooltip associated with the crosshair
    BYTE                    m_opacity;          ///< Opacity of the crosshair (0 - transparent, 255 - opaque)

    CDC         m_chDC;                 ///< Crosshair device context
    CDC         m_backDC;               ///< Background device context for alpha blending when the crosshair is a child window
    CBitmap     m_chBitmap;             ///< Bitmap into which to draw the crosshair when it is a child window
    CBitmap     *m_origCHBitmap;        ///< Original bitmap for the crosshair
    CBitmap     m_backBitmap;           ///< Bitmap for the background when alpha blending when the crosshair is a child window
    CBitmap     *m_origBackBitmap;      ///< Original background bitmap 
};
