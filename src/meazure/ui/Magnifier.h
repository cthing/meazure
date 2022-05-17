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
/// @brief Header file for the screen magnifier. 

#pragma once

#include "Label.h"
#include "NumberField.h"
#include "Themes.h"
#include "ImageButton.h"
#include <meazure/Messages.h>
#include <meazure/utilities/Timer.h>
#include <meazure/profile/Profile.h>


/// Provides a screen magnifier window complete with freeze frame, optional
/// pixel grid and pixel color capture. A number of color spaces are available
/// for the display of the captured pixel color. The contents of the screen are
/// read and displayed in this window using a 1X through 32X zoom factor.
/// Note that layered windows will not appear in the magnifier. Similarly,
/// windows rendered using DirectX will appear black since they are drawing
/// directly to the hardware and displaying using a hardware overlay.
///
/// Typically, the magnifier is centered on the active graphic element, usually
/// a crosshair.
///
class MeaMagnifier : public CWnd {

public:
    /// The magnifier displays the color of the pixel at the center of the
    /// window. This point typically corresponds to the hot spot of an active
    /// graphic element such as a crosshair. The color can be displayed in a
    /// number of different color spaces. This enumeration indicates which
    /// color space is in use.
    ///
    enum ColorFmt {
        RGBFmt = 0,    ///< Red, green and blue as individual values between 0 and 255 inclusive.
        RGBHexFmt = 1,    ///< Red, green and blue as a composite hex number of the form \#RRGGBB.
        CMYFmt = 2,    ///< Cyan, magenta and yellow as individual values between 0 and 255 inclusive.
        CMYKFmt = 3,    ///< Cyan, magenta, yellow and black as individual values.
        HSLFmt = 4,    ///< Hue, saturation and lightness as individual values.
        YCbCrFmt = 5,    ///< Luminance and chrominance as individual values.
        YIQFmt = 6     ///< Luminance, and inphase and quadrature chrominance channels.
    };

    /// Indicates whether the magnifier is running or in freeze frame mode.
    ///
    enum class RunState {
        Run = 0,        ///< Magnifier is in real time mode.
        Freeze = 1         ///< Magnifier is in freeze frame mode.
    };


    static constexpr int kDefZoomIndex { 1 };                   ///< Default index for the zoom factor.
    static constexpr bool kDefShowGrid { true };                ///< Indicates whether the grid should be display by default.
    static constexpr ColorFmt kDefColorFmt { RGBFmt };          ///< Default color display format.
    static constexpr RunState   kDefRunState { RunState::Run }; ///< Default magnifier display mode.


    /// Constructs a magnifier. To use the magnifier the Create
    /// method must be called.
    ///
    MeaMagnifier();

    /// Destroys a magnifier instance.
    ///
    virtual ~MeaMagnifier();

    /// Creates the window for the magnifier.
    ///
    /// @param topLeft      [in] Location of the top left corner of the magnifier window.
    /// @param width        [in] Width of the magnifier, in pixels. The aspect ratio of
    ///                     the magnifier is fixed, so the width determines the height.
    /// @param parentWnd    [in] Parent window for the magnifier.
    ///
    /// @return <b>true</b> if the window was created successfully.
    ///
    bool Create(const POINT& topLeft, int width, CWnd* parentWnd);

    /// Persists the magnifier settings to the specified profile object.
    ///
    /// @param profile      [in] Persistence object.
    ///
    void SaveProfile(MeaProfile& profile);

    /// Restores the magnifier settings from the specified profile object.
    ///
    /// @param profile      [in] Persistence object.
    ///
    void LoadProfile(MeaProfile& profile);

    /// Called to tell the magnifier to restore its default state.
    ///
    void MasterReset();

    /// Displays the magnifier window and places it in the desired run mode.
    ///
    void Enable();

    /// Hides the magnifier window.
    ///
    void Disable();

    /// Forces a redraw of the magnifier window.
    ///
    void Update();

    /// Indicates whether the magnifier is visible.
    ///
    /// @return <b>true</b> if the magnifier is enabled.
    ///
    bool IsEnabled() const { return m_enabled; }

    /// Returns the height of the magnifier.
    ///
    /// @return Height of the magnifier, in pixels.
    ///
    int GetHeight() const { return m_magHeight; }

    /// Zooms in the magnifier. If the magnifier is already
    /// at the minimum zoom, calling this method has no effect.
    ///
    void ZoomIn() { SetZoomIndex(m_zoomIndex + 1); }

    /// Zooms out the magnifier. If the magnifier is already
    /// at the maximum zoom, calling this method has no effect.
    ///
    void ZoomOut() { SetZoomIndex(m_zoomIndex - 1); }

    /// Indicates whether it is possible to zoom in the magnifier.
    ///
    /// @return <b>true</b> if the magnifier can zoom in further.
    ///
    bool CanZoomIn() const { return m_zoomIndex < kMaxZoomIndex; }

    /// Indicates whether it is possible to zoom out the magnifier.
    ///
    /// @return <b>true</b> if the magnifier can zoom out further.
    ///
    bool CanZoomOut() const { return m_zoomIndex > kMinZoomIndex; }

    /// Sets a specific zoom factor index.
    ///
    /// @param zoomIndex        [in] Index that selects a zoom factor.
    ///
    void SetZoomIndex(int zoomIndex) {
        if (zoomIndex >= kMinZoomIndex && zoomIndex <= kMaxZoomIndex) {
            m_zoomIndex = zoomIndex;
        }
        m_zoomSlider.SetPos(m_zoomIndex);
        Update();
    }

    /// Returns the current zoom factor index.
    ///
    /// @return Index that selects the zoom factor.
    ///
    int GetZoomIndex() const { return m_zoomIndex; }

    /// Sets the color display format.
    ///
    /// @param colorFmt     [in] Color format specifier.
    ///
    void SetColorFmt(ColorFmt colorFmt) {
        m_colorFmt = colorFmt;
        Update();
    }

    /// Returns the current color display format.
    ///
    /// @return Color display format specifier.
    ///
    ColorFmt GetColorFmt() const { return m_colorFmt; }

    /// Set the display mode of the magnifier to either running
    /// or frozen.
    ///
    /// @param runState     [in] Display mode.
    ///
    void SetRunState(RunState runState);

    /// Returns the display mode of the magnifier.
    ///
    /// @return Magnifier display mode.
    ///
    RunState GetRunState() const { return m_runState; }

    /// Displays or hides the magnifier grid. The grid is one pixel
    /// in dimension. Note that regardless of the setting, the grid
    /// is not displayed for lower zoom factors because it would
    /// completely obscure the display.
    ///
    /// @param show     [in] Indicates whether to show or hide the magnifier grid.
    ///
    void SetShowGrid(bool show) { m_showGrid = show; Update(); }

    /// Indicates whether the magnifier grid is displayed. Note that
    /// regardless of whether the magnifier grid is selected for display,
    /// it will not be displayed if the zoom factor is low because it
    /// would obscure the display.
    ///
    /// @return <b>true</b> if the magnifier grid is selected for display.
    ///
    bool GetShowGrid() const { return m_showGrid; }

    /// Returns a pointer to the pixel color display text field if it
    /// has the keyboard focus, and nullptr if it does not.
    ///
    /// @return Pointer to the color display text field if it has the
    ///         focus, and nullptr if it does not.
    ///
    MeaNumberField* GetFieldFocus() {
        return (GetFocus() == &m_swatchField) ? &m_swatchField : nullptr;
    }

protected:
    DECLARE_MESSAGE_MAP()

    /// Repaints the magnifier window.
    ///
    afx_msg void OnPaint();

    /// Called when the timer expires and it is time to capture and
    /// draw another magnified view.
    ///
    /// @return Always zero.
    ///
    afx_msg LRESULT OnHPTimer(WPARAM, LPARAM);

    /// Called when the zoom factor slider is moved.
    ///
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    /// Called when the pause button is toggled to either freeze
    /// the magnifier image or start it running again.
    ///
    afx_msg void OnRunState();

    /// Called to display the magnifier help topic when help is requested.
    ///
    /// @param lparam   [in] Cursor coordinates.
    ///
    /// @return Help ID corresponding to section where mouse clicked.
    ///
    afx_msg LRESULT OnHelpHitTest(WPARAM, LPARAM lparam);

private:
    static constexpr int kUpdateRate { 70 };    ///< Magnifier refresh rate, in milliseconds.
    static constexpr int kZoomHeight { 22 };    ///< Height of the zoom factor slider, in pixels.
    static constexpr SIZE kMargin { 5, 5 };     ///< Margin around the magnifier window.
    static constexpr int kSwatchWidth { 40 };   ///< Width of the pixel color swatch, in pixels.
    static constexpr int kZoomFactorArr[] { 1, 2, 3, 4, 6, 8, 16, 32 };                 ///< Arrays of zoom factors selected by the zoom index.
    static constexpr int kMaxZoomIndex { sizeof(kZoomFactorArr) / sizeof(int) - 1 };    ///< Index of the maximum allowable zoom factor.
    static constexpr int kMinZoomIndex { 0 };   ///< Index of the minimum allowable zoom factor.
    static constexpr int kMinGridFactor { 6 };  ///< Minimum zoom factor below which the grid is not displayed.


    /// Reads an appropriately sized region around the cursor and
    /// zooms it into the magnifier window.
    ///
    /// @param hDC      [in] Magnifier window device context.
    ///
    void Draw(HDC hDC);

    /// Performs the work of changing the display run mode.
    ///
    /// @param runState [in] Indicates the desired display run state.
    ///
    void ChangeRunState(RunState runState);


    CPoint m_curPos;                ///< Current position being magnified.
    bool m_enabled;                 ///< Indicates whether the magnifier is visible.
    RunState m_runState;            ///< Display mode run state.
    MeaImageButton m_runStateBtn;   ///< Magnifier pause button.
    MeaTimer m_timer;               ///< Refresh timer.
    ColorFmt m_colorFmt;            ///< Pixel color display format.
    MeaLabel m_swatchLabel;         ///< Label for the pixel color swatch.
    MeaNumberField m_swatchField;   ///< Text field to display the pixel color.
    CWnd m_swatchWin;               ///< Window for the pixel color swatch.
    MeaLabel m_zoomLabel;           ///< Label for the zoom factor slider.
    CSliderCtrl m_zoomSlider;       ///< Zoom factor slider.
    MeaLabel m_factorLabel;         ///< Zoom factor label.
    int m_zoomIndex;                ///< Currently selected zoom factor index.
    bool m_showGrid;                ///< Indicates whether the grid should be displayed, if possible.
    int m_magHeight;                ///< Height of the magnifier, in pixels.
};
