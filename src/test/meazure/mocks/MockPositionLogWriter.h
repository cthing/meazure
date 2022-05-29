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

#pragma once


class MockPositionLogWriter : public MeaPositionLogWriter {
public:
    void Write(int indentLevel, LPCTSTR format, ...) override {
        CString indent(_T(' '), indentLevel * 4);
        CString str;
        va_list args;

        va_start(args, format);
        str.FormatV(format, args);
        va_end(args);

        contents += MeaXMLParser::ToUTF8(str);
    }

    CString contents;
};
