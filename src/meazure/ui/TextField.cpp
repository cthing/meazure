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
#include "TextField.h"
#include "Layout.h"
#include <meazure/Messages.h>


BEGIN_MESSAGE_MAP(MeaTextField, CEdit)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


bool MeaTextField::Create(DWORD style, const POINT& topLeft, int numChars, CWnd* parentWnd, UINT id) {
    // Have the parent create the window
    //
    if (!CEdit::CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), "", style, CRect(topLeft, CSize(5, 5)), parentWnd, id)) {
        return false;
    }

    // Resize the window based on the label. First set a more appropriate font.
    //
    CFont* defaultFont = CFont::FromHandle(static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
    SetFont(defaultFont, FALSE);

    CDC* dc = GetDC();
    if (dc == nullptr) {
        return false;
    }

    // Select the new font into the DC
    //
    if (dc->SelectStockObject(DEFAULT_GUI_FONT) == nullptr) {
        ReleaseDC(dc);
        return false;
    }

    // Get the character height for the font.
    //
    TEXTMETRIC metrics;

    if (!dc->GetTextMetrics(&metrics)) {
        ReleaseDC(dc);
        return false;
    }

    ReleaseDC(dc);

    // Set the window to the character height and the text length.
    //
    return MeaLayout::SetWindowSize(*this, numChars * metrics.tmAveCharWidth * 3 / 2, 
                                    metrics.tmHeight + 2 * GetSystemMetrics(SM_CYEDGE));
}

void MeaTextField::OnKillFocus(CWnd* win) {
    CEdit::OnKillFocus(win);

    CWnd* parent = GetParent();
    if (parent != nullptr) {
        parent->SendMessage(MeaFieldFocusMsg, 0, GetDlgCtrlID());
    }
}
