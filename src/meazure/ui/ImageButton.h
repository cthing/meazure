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
 /// @brief Header file for a bitmap-based button.

#pragma once


/// A button that uses bitmap resources. Based on code from
/// Joseph M. Newcomer at:
///
/// http://www.codeproject.com/buttonctrl/betterbmpbutton.asp
///
class MeaImageButton : public CButton {

public:
    /// Constructs a image button using the specified image(s).
    ///
    /// @param up       [in] Resource ID of image for button in up position.
    /// @param down     [in] Resource ID of image for button in down position.
    /// @param disabled [in] Resource ID of image to use when button is disabled.
    ///
    explicit MeaImageButton(UINT up, UINT down = 0, UINT disabled = 0);

    /// Constructs an image button with no images specified; the LoadBitmaps
    /// method must be called.
    ///
    MeaImageButton() : MeaImageButton(0) {}

    /// Destroys a image button object.
    ///
    virtual ~MeaImageButton();

    /// Convenience version of the button create method. Using
    /// this method allows the button to size itself to the bitmap.
    ///
    /// @param dwStyle      [in] Button style as defined by CButton.
    /// @param pParentWnd   [in] Parent window for the button.
    /// @param nID          [in] ID for the button.
    /// @param toolTipID    [in] String resource ID for the button's tooltip.
    ///
    /// @return <b>true</b> if creation successful.
    ///
    bool Create(DWORD dwStyle, CWnd* pParentWnd, UINT nID = 0xffff, UINT toolTipID = 0xffff);

    /// Relays messages to the tooltip control.
    ///
    /// @param pMsg     [in] Message information structure.
    ///
    /// @return <b>TRUE</b> if message was processed.
    ///
    virtual BOOL PreTranslateMessage(MSG* pMsg) override;

    /// Specifies whether the button is a push button or a toggle button.
    ///
    /// @param toggle   [in] <b>true</b> if the button should have toggle behavior.
    ///
    void SetToggleType(bool toggle) { m_toggle = toggle; }

    /// Loads the button's bitmaps.
    ///
    /// @param up       [in] Resource ID of image to use for up appearance when enabled.
    /// @param down     [in] Resource ID of image to use for down appearance when enabled.
    ///                 If 0, the up image is used.
    /// @param disabled [in] Resource ID of image to use for disabled appearance.
    ///                 If 0, the up image is half-toned gray
    ///
    void LoadBitmaps(UINT up, UINT down, UINT disabled = 0) {
        m_up = up; m_down = down; m_disabled = disabled;
    }

    /// Returns the bitmaps associated with the button image.
    ///
    /// @param up       [out] Resource ID of image to use for up appearance when enabled.
    /// @param down     [out] Resource ID of image to use for down appearance when enabled.
    /// @param disabled [out] Resource ID of image to use for disabled appearance.
    ///
    void GetBitmaps(UINT& up, UINT& down, UINT& disabled) const {
        up = m_up; down = m_down; disabled = m_disabled;
    }

    /// Sets the button state: up or down.
    ///
    /// @param state    [in] Button state (0 = up, 1 = down).
    ///
    void SetState(bool state) {
        m_depressed = state;
        CButton::SetState(state);
        InvalidateRect(nullptr);
    }

protected:
    DECLARE_MESSAGE_MAP()

    /// Performs cleanup of objects created in the Create method.
    ///
    afx_msg void OnDestroy();

    /// Called to render the button.
    ///
    /// @param lpDrawItemStruct     [in] Button item information structure.
    ///
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    /// Called when the left mouse button is pressed down.
    ///
    /// @param nFlags       [in] Flags present when button pressed.
    /// @param point        [in] Location of the mouse when the button was pressed.
    ///
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


    HTHEME m_theme;             ///< Windows rendering theme for buttons
    UINT m_up;                  ///< ID of bitmap for button up appearance.
    UINT m_down;                ///< ID of bitmap for button down appearance.
    UINT m_disabled;            ///< ID of bitmap for button disabled appearance.
    bool m_depressed;           ///< Indicates if the button is depressed.
    bool m_toggle;              ///< Indicates if this is a toggle button.
    CToolTipCtrl m_toolTip;     ///< Tool tip control for the button.
};
