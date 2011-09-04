/*
 * Copyright 2001, 2004, 2011 C Thing Software
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
/// @brief Header file for a slider control on a ruler background.

#pragma once


/// Slider control on a ruler background. Dynamic readout of pixel position is
/// provided while the slider is dragged.
///
class MeaRulerSlider : public CProgressCtrl
{
    DECLARE_DYNAMIC(MeaRulerSlider)

public:
    /// Constructs a ruler slider control. Before the control can be used, the
    /// Create method must be called.
    ///
    MeaRulerSlider();

    /// Destroys a ruler slider control.
    ///
    virtual ~MeaRulerSlider();


    /// Returns the position value of the slider.
    ///
    /// @return The slider's current position.
    ///
    int     GetSliderPos();

    /// Set the position of the slider.
    ///
    /// @param pos  [in] Position for the slider.
    ///
    void    SetSliderPos(int pos);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(MeaRulerSlider)
    public:
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

    /// @fn Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
    /// Creates the window for the ruler slider control.
    ///
    /// @param dwStyle      [in] Style for the control's window as defined by CProgressCtrl.
    /// @param rect         [in] Bounding retangle for the control, in pixels.
    /// @param pParentWnd   [in] Parent window for the control.
    /// @param nID          [in] ID for the control.
    ///
    /// @return TRUE if the control was created successfully.

    /// @fn PreSubclassWindow()
    /// Called right before the base class control is subclassed. This
    /// method is used to perform intial setup of the slider.

protected:
    /// Initializes the slider.
    ///
    /// @param rect     [in] Client rectangle for the slider.
    ///
    void Init(const CRect& rect);
    
    /// Draws the calipers that form the thumb control for the slider.
    ///
    /// @param pDC      [in] Device context into which the calipers are drawn.
    ///
    void DrawCalipers(CDC* pDC);

    // Generated message map functions
    //{{AFX_MSG(MeaRulerSlider)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnEnable(BOOL bEnable);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    /// @fn OnLButtonDown(UINT nFlags, CPoint point)
    /// Called when the left mouse button is pressed. Captures the pointer
    /// for subsequent dragging of the slider.
    /// @param nFlags       [in] Modifier flags in effect.
    /// @param point        [in] Location of the pointer.

    /// @fn OnLButtonUp(UINT nFlags, CPoint point)
    /// Called when the left mouse button is released. Releases the capture
    /// of the pointer.
    /// @param nFlags       [in] Modifier flags in effect.
    /// @param point        [in] Location of the pointer.

    /// @fn OnMouseMove(UINT nFlags, CPoint point)
    /// Called when the mouse is moved over the slider. If the pointer is
    /// captured, the slider thumb control is dragged.
    /// @param nFlags       [in] Modifier flags in effect.
    /// @param point        [in] Location of the pointer.

    /// @fn OnPaint()
    /// Called to repaint the slider control.

    /// @fn OnEraseBkgnd(CDC* pDC)
    /// Called to clear the control's background.
    /// @param pDC      [in] Device context into which to draw the background.
    /// @return Always returns TRUE.

    /// @fn OnSize(UINT nType, int cx, int cy)
    /// Called when the size of the control's window is changed. The control
    /// resets its position accordingly.
    /// @param nType    [in] Indicates the type of resizing.
    /// @param cx       [in] New width, in pixels.
    /// @param cy       [in] New height, in pixels.

    /// @fn OnEnable(BOOL bEnable)
    /// Called when the control is enabled or disabled so that it can redraw
    /// itself to reflect the new state.
    /// @param bEnable  [in] Indicates if the control has been enabled or disabled.

private:
    static const int kHotSpotOffset;    ///< Offset around the caliper that is draggable.
    static const int kTickPosition;     ///< Relative position for the ruler tick marks.
    static const int kTickFactor;       ///< The tick height is the thickness of the window divided by this factor.


    /// Indicates whether the specified slider position is in the draggable area.
    ///
    /// @return <b>true</b> if the position is in the draggable area for the slider.
    ///
    bool        IsHot(int pos) const;

    bool        m_vertical;             ///< Indicates if the slider is oriented vertically.
    bool        m_captured;             ///< Indicates if the slider thumb control captured.
    bool        m_sliderActive;         ///< Indaictes the slider can be dragged.
    CFont       m_posFont;              ///< Font used for displaying the current slider position.
    CPen        m_activePen;            ///< Pen to draw the control when enabled.
    CPen        m_inactivePen;          ///< Pen to draw the control when disabled.
    CBrush      m_activeBrush;          ///< Brush to paint the control when enabled.
    CBrush      m_inactiveBrush;        ///< Brush to paint the control when disabled.
    CRect       m_sliderRect;           ///< Window rectangle for the control.
    int         m_sliderPos;            ///< Current slider position.
};

