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
#include "NumberField.h"
#include <meazure/Messages.h>
#include "Layout.h"
#include <ctype.h>
#include <afxole.h>


int MeaNumberField::m_edgeHeight { -1 };


BEGIN_MESSAGE_MAP(MeaNumberField, MeaTextField)
    ON_WM_CHAR()
    ON_MESSAGE(WM_PASTE, OnPaste)
END_MESSAGE_MAP()


MeaNumberField::MeaNumberField() :
    MeaTextField(), m_valueType(AllValues) {
    if (m_edgeHeight < 0) {
        m_edgeHeight = GetSystemMetrics(SM_CYEDGE);
    }
}

MeaNumberField::~MeaNumberField() {}

BOOL MeaNumberField::PreTranslateMessage(MSG* msg) {
    if (msg->message == WM_KEYDOWN) {
        CWnd* parent = GetParent();
        if (parent != nullptr) {
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

void MeaNumberField::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (IsValid(reinterpret_cast<TCHAR*>(&nChar), 1)) {
        MeaTextField::OnChar(nChar, nRepCnt, nFlags);
    } else {
        MessageBeep(MB_OK);
    }
}

LRESULT MeaNumberField::OnPaste(WPARAM, LPARAM) {
    COleDataObject obj;

    bool editable = (GetStyle() & ES_READONLY) != ES_READONLY;

    if (editable && obj.AttachClipboard()) {
        if (obj.IsDataAvailable(CF_TEXT)) {
            HGLOBAL hmem = obj.GetGlobalData(CF_TEXT);
            CMemFile sf(static_cast<BYTE*>(::GlobalLock(hmem)), static_cast<UINT>(::GlobalSize(hmem)));

            CString buffer;

            PTSTR str = buffer.GetBufferSetLength(static_cast<int>(::GlobalSize(hmem)));
            sf.Read(str, static_cast<UINT>(::GlobalSize(hmem)));
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

bool MeaNumberField::IsValid(PCTSTR str, int len) const {
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
        bool signTyped = false;
        bool periodTyped = false;
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
