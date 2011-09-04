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

#include "StdAfx.h"
#include "NumberField.h"
#include "Layout.h"
#include <ctype.h>
#include <afxole.h>


int MeaNumberField::m_edgeHeight = -1;


BEGIN_MESSAGE_MAP(MeaNumberField, CEdit)
    ON_WM_CHAR()
    ON_MESSAGE(WM_PASTE, OnPaste)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


MeaNumberField::MeaNumberField() : CEdit(),
    m_valueType(AllValues)
{
    if (m_edgeHeight < 0) {
        m_edgeHeight = GetSystemMetrics(SM_CYEDGE);
    }
}


MeaNumberField::~MeaNumberField()
{
}


bool MeaNumberField::Create(DWORD style, const POINT& topLeft,
                      int numChars, CWnd* parentWnd, UINT id)
{
    // Have the parent create the window
    //
    if (!CEdit::CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), "", style,
            CRect(topLeft, CSize(5, 5)), parentWnd, id)) {
        return false;
    }

    // Resize the window based on the label. First set
    // a more appropriate font.
    //
    CFont *defaultFont = CFont::FromHandle(static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
    SetFont(defaultFont, FALSE);

    CDC *dc = GetDC();
    if (dc == NULL) {
        return false;
    }

    // Select the new font into the DC
    //
    if (dc->SelectStockObject(DEFAULT_GUI_FONT) == NULL) {
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
        metrics.tmHeight + 2 * m_edgeHeight);
}


BOOL MeaNumberField::PreTranslateMessage(MSG* msg)
{
    if (msg->message == WM_KEYDOWN) {
        CWnd *parent = GetParent();
        if (parent != NULL) {
            if (msg->wParam == VK_UP) {
                parent->SendMessage(MeaFieldArrowMsg, 1, GetDlgCtrlID());
                return TRUE;
            } else if (msg->wParam == VK_DOWN) {
                parent->SendMessage(MeaFieldArrowMsg, static_cast<WPARAM>(-1), GetDlgCtrlID());
                return TRUE;
            } else if (msg->wParam == VK_RETURN) {
                parent->SendMessage(MeaFieldEnterMsg, 0, GetDlgCtrlID());
                return TRUE;
            }
        }
    }

    return FALSE;
}


void MeaNumberField::OnKillFocus(CWnd *win)
{
    CEdit::OnKillFocus(win);

    CWnd *parent = GetParent();
    if (parent != NULL) {
        parent->SendMessage(MeaFieldFocusMsg, 0, GetDlgCtrlID());
    }
}


void MeaNumberField::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (IsValid(reinterpret_cast<TCHAR*>(&nChar), 1)) {
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    } else {
        MessageBeep(MB_OK);
    }
}


LRESULT MeaNumberField::OnPaste(WPARAM, LPARAM) 
{
    COleDataObject obj;

    if (obj.AttachClipboard()) {
        if (obj.IsDataAvailable(CF_TEXT)) {
            HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
            CMemFile sf(static_cast<BYTE*>(::GlobalLock(hmem)), ::GlobalSize(hmem));

            CString buffer;

            LPTSTR str = buffer.GetBufferSetLength(::GlobalSize(hmem));
            sf.Read(str, ::GlobalSize(hmem));
            ::GlobalUnlock(hmem);
            buffer.ReleaseBuffer();
            buffer.FreeExtra();

            if (IsValid(buffer, buffer.GetLength())) {
                ReplaceSel(buffer, TRUE);
            } else {
                MessageBeep(MB_OK);
            }
        }
    }

    return TRUE;
}


bool MeaNumberField::IsValid(LPCTSTR str, int len) const
{
    TCHAR ch;
    int i, j;

    for (j = 0, ch = str[j]; j < len; j++, ch = str[j]) {
        // Always allow numbers and backspace.
        //
        if (_istdigit(ch) || ch == VK_BACK) {
            return true;
        }

        // If the character is not '-'. '+' or '.', reject it.
        //
        if (!IsSign(ch) && !IsPoint(ch)) {
            return false;
        }

        // Get existing text and selection information.
        //
        CString fieldStr;
        int startChar, endChar;

        GetWindowText(fieldStr);
        GetSel(startChar, endChar);

        // Detect whether unselected text already contains a '+',
        // '-' or '.'. Selected text will be replaced so we don't
        // care what it contains.
        //
        bool signTyped      = false;
        bool periodTyped    = false;
        TCHAR fieldCh;

        // most of the work done in here..
        for (i = 0; i < fieldStr.GetLength(); i++) {
            if ((i < startChar) || (i >= endChar)) {
                fieldCh = fieldStr[i];

                if (IsSign(fieldCh)) {
                    signTyped = true;
                } else if (IsPoint(fieldCh)) {
                    periodTyped = true;
                }
            }
        }

        // Allow sign only once (first char).
        //
        if (IsSign(ch) && (signTyped || (startChar != 0))) {
            return false;
        }

        // Allow period only once.
        //
        if (IsPoint(ch) && periodTyped) {
            return false;
        }
    }

    return true;
}
