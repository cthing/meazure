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
#include "Utils.h"
#include <float.h>

static constexpr long PRESSED_MASK { 0x8000 };


CString MeaUtils::DblToStr(double value) {
    CString numStr;
    numStr.Format(_T("%.*f"), DBL_DIG - 1, value);

    int idx = numStr.GetLength();
    while (idx-- > 1) {
        if ((numStr[idx] != _T('0')) || (numStr[idx - 1] == _T('.'))) {
            break;
        }
    }

    return numStr.Left(idx + 1);
}

bool MeaUtils::IsNumber(LPCTSTR str, double* valuep) {
    CString vstr(str);
    LPTSTR endStr;

    vstr.TrimLeft();
    vstr.TrimRight();
    if (vstr.IsEmpty() || (static_cast<int>(_tcslen(str)) != vstr.GetLength())) {
        return false;
    }

    errno = 0;
    double v = _tcstod(str, &endStr);
    if ((*endStr != _T('\0')) || (errno == ERANGE)) {
        return false;
    }

    if (valuep != nullptr) {
        *valuep = v;
    }

    return true;
}

bool MeaUtils::IsBoolean(LPCTSTR str, bool* valuep) {
    CString vstr(str);

    vstr.TrimLeft();
    vstr.TrimRight();
    if (vstr.IsEmpty() || (static_cast<int>(_tcslen(str)) != vstr.GetLength())) {
        return false;
    }

    vstr.MakeLower();
    if (vstr == _T("1") || vstr == _T("true")) {
        if (valuep != nullptr) {
            *valuep = true;
        }
        return true;
    }
    if (vstr == _T("0") || vstr == _T("false")) {
        if (valuep != nullptr) {
            *valuep = false;
        }
        return true;
    }

    return false;
}

CString MeaUtils::LFtoCRLF(CString str) {
    CString conv(str);
    int pos = 0;

    do {
        pos = conv.Find(_T('\n'), pos);

        if (pos != -1) {
            if ((pos == 0) || (conv.GetAt(pos - 1) != _T('\r'))) {
                conv.Insert(pos, _T('\r'));
                pos += 2;
            } else {
                pos++;
            }
        }
    } while (pos != -1);

    return conv;
}

CString MeaUtils::CRLFtoLF(CString str) {
    CString conv(str);
    conv.Replace(_T("\r\n"), _T("\n"));
    return conv;
}

bool MeaUtils::IsKeyPressed(int keyCode) {
    return (GetKeyState(keyCode) & PRESSED_MASK) == PRESSED_MASK;
}
