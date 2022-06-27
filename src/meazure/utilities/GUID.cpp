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
#include "GUID.h"


MeaGUID::MeaGUID(PCTSTR guidStr) {
    if (guidStr == nullptr) {
        HRESULT hr = CoCreateGuid(&m_guid);
        if (FAILED(hr)) {
            AfxThrowOleException(hr);
        }
    } else {
        Assign(guidStr);
    }
}

MeaGUID& MeaGUID::Assign(PCTSTR guidStr) {
    if (guidStr == nullptr) {
        AfxThrowOleException(E_POINTER);
    }
    if (_tcslen(guidStr) != 36) {
        AfxThrowOleException(E_INVALIDARG);
    }

    int count = _stscanf_s(guidStr, kGuidFormat, &m_guid.Data1, &m_guid.Data2, &m_guid.Data3,
                           &m_guid.Data4[0], &m_guid.Data4[1], &m_guid.Data4[2], &m_guid.Data4[3],
                           &m_guid.Data4[4], &m_guid.Data4[5], &m_guid.Data4[6], &m_guid.Data4[7]);
    if (count != 11) {
        AfxThrowOleException(E_INVALIDARG);
    }

    return *this;
}

CString MeaGUID::ToString() const {
    CString buffer;
    buffer.Format(kGuidFormat, m_guid.Data1, m_guid.Data2, m_guid.Data3,
                  m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2], m_guid.Data4[3],
                  m_guid.Data4[4], m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]);
    return buffer;
}


std::ostream& operator<<(std::ostream& os, const MeaGUID& guid) {
    os << guid.ToString();
    return os;
}
