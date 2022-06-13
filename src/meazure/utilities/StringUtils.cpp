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

#include <meazure/pch.h>
#include "StringUtils.h"
#include <iostream>


CString MeaStringUtils::IntToStr(int value) {
    CString numStr;
    numStr.Format("%d", value);
    return numStr;
}

CString MeaStringUtils::DblToStr(double value) {
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

bool MeaStringUtils::IsNumber(PCTSTR str, double* valuep) {
    CString vstr(str);
    PTSTR endStr;

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

bool MeaStringUtils::IsBoolean(PCTSTR str, bool* valuep) {
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

CString MeaStringUtils::LFtoCRLF(CString str) {
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

CString MeaStringUtils::CRLFtoLF(CString str) {
    CString conv(str);
    conv.Replace(_T("\r\n"), _T("\n"));
    return conv;
}

CStringA MeaStringUtils::ACPtoUTF8(const CString& str) {
    return ACPtoUTF8(static_cast<PCTSTR>(str), str.GetLength());
}

CStringA MeaStringUtils::ACPtoUTF8(PCTSTR str, std::size_t len) {
    CStringA utf8Str;

    if (str == nullptr || len == 0) {
        return utf8Str;
    }

    std::size_t strLen = (len == SIZE_MAX) ? _tcslen(str) : len;
    if (strLen == 0) {
        return utf8Str;
    }

#ifdef _UNICODE
    int numBytes = WideCharToMultiByte(CP_UTF8, 0, str, strLen, nullptr, 0, nullptr, nullptr);
    if (numBytes > 0) {
        WideCharToMultiByte(CP_UTF8, 0, str, strLen, utf8Str.GetBuffer(numBytes), numBytes, nullptr, nullptr);
        utf8Str.ReleaseBufferSetLength(numBytes);
    } else {
        DWORD errorCode = GetLastError();
        if (GetConsoleWindow() != nullptr) {
            std::cerr << "Could not convert wide character to UTF-8. Error code " << errorCode << '\n';
        }
    }
#else
    CStringW wideStr(str, static_cast<int>(strLen));

    int numWideChars = wideStr.GetLength();
    int numBytes = WideCharToMultiByte(CP_UTF8, 0, static_cast<PCWSTR>(wideStr), numWideChars, nullptr, 0,
                                       nullptr, nullptr);
    if (numBytes > 0) {
        WideCharToMultiByte(CP_UTF8, 0, static_cast<PCWSTR>(wideStr), numWideChars,
                            utf8Str.GetBuffer(numBytes), numBytes, nullptr, nullptr);
        utf8Str.ReleaseBufferSetLength(numBytes);
    } else {
        DWORD errorCode = GetLastError();
        if (GetConsoleWindow() != nullptr) {
            std::cerr << "Could not convert wide character to UTF-8. Error code " << errorCode << '\n';
        }
    }
#endif

    return utf8Str;
}

CStringA MeaStringUtils::ACPtoUTF8(TCHAR ch) {

#ifdef _UNICODE
    if (ch > '\u001F' && ch < '\u007F') {
        return CStringA(&ch, 1);
    }

    CStringA utf8Str;

    int numBytes = WideCharToMultiByte(CP_UTF8, 0, &ch, 1, nullptr, 0, nullptr, nullptr);
    if (numBytes > 0) {
        WideCharToMultiByte(CP_UTF8, 0, &ch, 1, utf8Str.GetBuffer(numBytes), numBytes, nullptr, nullptr);
        utf8Str.ReleaseBufferSetLength(numBytes);
    } else {
        DWORD errorCode = GetLastError();
        if (GetConsoleWindow() != nullptr) {
            std::cerr << "Could not convert wide character to UTF-8. Error code " << errorCode << '\n';
        }
    }

    return utf8Str;
#else
    if (ch > '\x1F' && ch < '\x7F') {
        return CStringA(&ch, 1);
    }

    CStringW wideStr(&ch, 1);
    CStringA utf8Str;

    int numBytes = WideCharToMultiByte(CP_UTF8, 0, static_cast<PCWSTR>(wideStr), 1, nullptr, 0, nullptr, nullptr);
    if (numBytes > 0) {
        WideCharToMultiByte(CP_UTF8, 0, static_cast<PCWSTR>(wideStr), 1, utf8Str.GetBuffer(numBytes), numBytes,
                            nullptr, nullptr);
        utf8Str.ReleaseBufferSetLength(numBytes);
    } else {
        DWORD errorCode = GetLastError();
        if (GetConsoleWindow() != nullptr) {
            std::cerr << "Could not convert wide character to UTF-8. Error code " << errorCode << '\n';
        }
    }

    return utf8Str;
#endif
}
