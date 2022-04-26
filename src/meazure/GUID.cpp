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

#include "StdAfx.h"
#include "GUID.h"


MeaGUID::MeaGUID()
{
    HRESULT hr = CoCreateGuid(&m_guid);
    if (FAILED(hr)) {
        AfxThrowOleException(hr);
    }
}


MeaGUID& MeaGUID::Assign(LPCTSTR guidStr)
{
    if (guidStr == nullptr) {
        AfxThrowOleException(E_POINTER);
    }
    if (_tcslen(guidStr) != 36) {
        AfxThrowOleException(E_INVALIDARG);
    }

    int field[11];
    int count = _stscanf_s(guidStr,
                           _T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"),
                           &field[0], &field[1], &field[2], &field[3],
                           &field[4], &field[5], &field[6], &field[7],
                           &field[8], &field[9], &field[10]);
    if (count != 11) {
        AfxThrowOleException(E_INVALIDARG);
    }

    m_guid.Data1 = static_cast<unsigned long>(field[0]);
    m_guid.Data2 = static_cast<unsigned short>(field[1]);
    m_guid.Data3 = static_cast<unsigned short>(field[2]);
    for (int i = 0; i < 8; i++) {
        m_guid.Data4[i] = static_cast<unsigned char>(field[i+3]);
    }

    return *this;
}


CString MeaGUID::ToString() const
{
    m_buffer.Format(
                _T("%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"),
                m_guid.Data1, m_guid.Data2, m_guid.Data3,
                m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2], m_guid.Data4[3],
                m_guid.Data4[4], m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]);
    return m_buffer;
}
