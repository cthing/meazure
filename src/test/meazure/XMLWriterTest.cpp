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

#include "pch.h"
#define BOOST_TEST_MODULE XMLWriterTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/xml/XMLWriter.h>
#include <sstream>


struct TestXMLWriter : public MeaXMLWriter {

    TestXMLWriter(std::ostream& out) : MeaXMLWriter(out) {}

    void WriteQuoted(PCTSTR str) override { MeaXMLWriter::WriteQuoted(str); }

    void WriteEscaped(PCTSTR str) override { MeaXMLWriter::WriteEscaped(str); };

    void WriteEscaped(TCHAR ch) override { MeaXMLWriter::WriteEscaped(ch); };

    void WriteNewline() override { MeaXMLWriter::WriteNewline(); }

    void WriteRaw(PCTSTR str) override { MeaXMLWriter::WriteRaw(str); }

    void WriteRaw(TCHAR ch) override { MeaXMLWriter::WriteRaw(ch); }
};


struct TestFixture {
    TestFixture() : writer(stream) {
        stream.exceptions(std::ios::failbit | std::ios::badbit);
    }

    void clear() { stream.str(""); }

    std::ostringstream stream;
    TestXMLWriter writer;
};


bool IsANSI() {
    return GetACP() == 1252;
}


BOOST_FIXTURE_TEST_CASE(TestWriteRawChar, TestFixture) {
    writer.WriteRaw(_T('a'));
    BOOST_TEST(stream.str() == u8"a");

    clear();
    writer.WriteRaw(_T(' '));
    BOOST_TEST(stream.str() == u8" ");

    clear();
    writer.WriteRaw(_T('<'));
    BOOST_TEST(stream.str() == u8"<");

    clear();
    writer.WriteRaw(_T('>'));
    BOOST_TEST(stream.str() == u8">");

    clear();
    writer.WriteRaw(_T('\x00'));
    BOOST_TEST(stream.str() == u8"\u0000");

    if (IsANSI()) {
        clear();
        writer.WriteRaw(_T('\x99'));
        BOOST_TEST(stream.str() == u8"\u2122");
    }
}

BOOST_FIXTURE_TEST_CASE(TestWriteRawString, TestFixture) {
    writer.WriteRaw(_T("A bc<>&\"'"));
    BOOST_TEST(stream.str() == u8"A bc<>&\"'");

    if (IsANSI()) {
        clear();
        writer.WriteRaw(_T("\x99\x85"));
        BOOST_TEST(stream.str() == u8"\u2122\u2026");
    }
}

BOOST_FIXTURE_TEST_CASE(TestWriteNewline, TestFixture) {
    writer.WriteNewline();
    BOOST_TEST(stream.str() == u8"\n");
}

BOOST_FIXTURE_TEST_CASE(TestWriteEscapedChar, TestFixture) {
    writer.WriteEscaped(_T('a'));
    BOOST_TEST(stream.str() == u8"a");

    clear();
    writer.WriteEscaped(_T('\n'));
    BOOST_TEST(stream.str() == u8"\n");

    clear();
    writer.WriteEscaped(_T('\t'));
    BOOST_TEST(stream.str() == u8"\t");

    clear();
    writer.WriteEscaped(_T('\r'));
    BOOST_TEST(stream.str() == u8"\r");

    clear();
    writer.WriteEscaped(_T('&'));
    BOOST_TEST(stream.str() == u8"&amp;");

    clear();
    writer.WriteEscaped(_T('<'));
    BOOST_TEST(stream.str() == u8"&lt;");

    clear();
    writer.WriteEscaped(_T('>'));
    BOOST_TEST(stream.str() == u8"&gt;");

    clear();
    writer.WriteEscaped(_T('"'));
    BOOST_TEST(stream.str() == u8"&quot;");

    clear();
    writer.WriteEscaped(_T('\''));
    BOOST_TEST(stream.str() == u8"&apos;");

    if (IsANSI()) {
        clear();
        writer.WriteEscaped(_T('\x99'));
        BOOST_TEST(stream.str() == u8"&#8482;");

        clear();
        writer.WriteEscaped(_T('\x1A'));
        BOOST_TEST(stream.str() == u8"ctrl-26");
    }
}

BOOST_FIXTURE_TEST_CASE(TestWriteEscapedString, TestFixture) {
    writer.WriteEscaped(_T("a\n\t\r&<>\"'"));
    BOOST_TEST(stream.str() == u8"a\n\t\r&amp;&lt;&gt;&quot;&apos;");

    if (IsANSI()) {
        clear();
        writer.WriteEscaped(_T("\x99\x85"));
        BOOST_TEST(stream.str() == u8"&#8482;&#8230;");
    }
}

BOOST_FIXTURE_TEST_CASE(TestWriteQuoted, TestFixture) {
    writer.WriteQuoted(_T("abcd &efg"));
    BOOST_TEST(stream.str() == u8"\"abcd &amp;efg\"");
}

BOOST_FIXTURE_TEST_CASE(TestStartEndDocument, TestFixture) {
    writer.StartDocument().EndDocument();
    BOOST_TEST(stream.str() == u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n");
}

BOOST_FIXTURE_TEST_CASE(TestElements, TestFixture) {
    writer.StartDocument()
          .StartElement(_T("elem1"))
          .StartElement(_T("elem2"))
          .EndElement()
          .EndElement()
          .EndDocument();
    BOOST_TEST(stream.str() == u8R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2/>
</elem1>
)|");
}

BOOST_FIXTURE_TEST_CASE(TestAttributes, TestFixture) {
    writer.StartDocument()
          .StartElement(_T("elem"))
          .AddAttribute(_T("attr1"), _T("abc"))
          .AddAttribute(_T("attr2"), 2)
          .AddAttribute(_T("attr3"), 3.5)
          .EndElement()
          .EndDocument();
    BOOST_TEST(stream.str() == u8R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem attr1="abc" attr2="2" attr3="3.5"/>
)|");
}

BOOST_FIXTURE_TEST_CASE(TestDoctype, TestFixture) {
    writer.StartDocument()
        .Doctype(_T("elem"), _T("http://www.cthing.com/dtd/PositionLog1.dtd"))
        .StartElement(_T("elem"))
        .EndElement()
        .EndDocument();
    BOOST_TEST(stream.str() == u8R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem SYSTEM "http://www.cthing.com/dtd/PositionLog1.dtd">
<elem/>
)|");
}

BOOST_FIXTURE_TEST_CASE(TestCharacters, TestFixture) {
    writer.StartDocument()
        .StartElement(_T("elem"))
        .Characters(_T("Hello\r\n\nWorld"))
        .EndElement()
        .EndDocument();
    BOOST_TEST(stream.str() == u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<elem>Hello\n\nWorld</elem>\n");
}
