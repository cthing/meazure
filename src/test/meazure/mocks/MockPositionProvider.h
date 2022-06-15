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

#include <meazure/position/PositionProvider.h>


class MockPositionProvider : public MeaPositionProvider {
public:
    MockPositionProvider(): m_title(_T("Test Title")), m_description(_T("This is a test")) {}

    virtual const CString& GetTitle() const {
        return m_title;
    }

    virtual const CString& GetDescription() const {
        return m_description;
    }

    virtual PCTSTR GetCurrentDtdUrl() const {
        return _T("https://www.cthing.com/dtd/PositionLog1.dtd");
    }

    virtual PositionDesktops GetReferencedDesktops() const {
        return m_desktops;
    }

    void AddReferencedDesktop(const MeaPositionDesktop& desktop) {
        m_desktops.push_back(desktop);
    }

    virtual const MeaPositionCollection& GetPositions() const {
        return m_positions;
    }

    void AddPosition(MeaPosition* position) {
        m_positions.Add(position);
    }

private:
    PositionDesktops m_desktops;
    MeaPositionCollection m_positions;
    CString m_title;
    CString m_description;
};
