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
#include "XMLWriter.h"


CString MeaXMLWriter::Encode(const CString& src) {
    CString encStr;

#ifdef _UNICODE
    for (int i = 0; i < src.GetLength(); i++) {
        TCHAR ch = src.GetAt(i);
        switch (ch) {
        case _T('&'):
            encStr += _T("&amp;");
            break;
        case _T('<'):
            encStr += _T("&lt;");
            break;
        case _T('>'):
            encStr += _T("&gt;");
            break;
        case _T('\''):
            encStr += _T("&apos;");
            break;
        case _T('\"'):
            encStr += _T("&quot;");
            break;
        default:
            encStr += ch;
            break;
        }
    }
#else
    const unsigned char* str = reinterpret_cast<const unsigned char*>(static_cast<LPCTSTR>(src));
    const unsigned char* sptr;
    const size_t len = _mbslen(str);
    size_t i;

    for (i = 0, sptr = str; i < len; i++, sptr = _mbsinc(sptr)) {
        switch (*sptr) {
        case _T('&'):
            encStr += _T("&amp;");
            break;
        case _T('<'):
            encStr += _T("&lt;");
            break;
        case _T('>'):
            encStr += _T("&gt;");
            break;
        case _T('\''):
            encStr += _T("&apos;");
            break;
        case _T('\"'):
            encStr += _T("&quot;");
            break;
        default:
            encStr += *sptr;
            break;
        }
    }
#endif

    return encStr;
}
