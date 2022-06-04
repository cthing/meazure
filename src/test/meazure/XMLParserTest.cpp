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
#define BOOST_TEST_MODULE XMLParserTest
#include "GlobalFixture.h"
#include <boost/test/unit_test.hpp>
#include <meazure/utilities/XMLParser.h>
#include <float.h>
#include <stdlib.h>

namespace tt = boost::test_tools;

BOOST_TEST_DONT_PRINT_LOG_VALUE(MeaXMLNode)


LPCTSTR xml1 = _T(R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2>
        <elem3>Test XML Data</elem3>
        <elem4 attr1="abc" attr2="1" attr3="2.5" attr4="true"/>
    </elem2>
</elem1>
)|");

LPCTSTR xml2 = _T(R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem1 [
  <!ELEMENT elem1 (#PCDATA)>
]>
<elem1/>
)|");

LPCTSTR xml3 = _T(R"|(<?xml version="1.0" encoding="UTF-8"?>
<elem1>
    <elem2>
        <elem3>Test XML Data</elem3>
        <elem4 attr1="abc" attr2="1" attr3="2.5" attr4="true"/>
</elem1>
)|");

LPCTSTR xml4 = _T(R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem1 [
  <!ELEMENT elem1 (#PCDATA)>
]>
<elem1>
    <elem2/>
</elem1>
)|");

LPCTSTR xml5 = _T(R"|(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE elem1 SYSTEM "https://www.cthing.com/test.dtd">
<elem1/>
)|");


BOOST_AUTO_TEST_CASE(TestParserHandlerNoValidation) {

    struct TestHandler : public MeaXMLParserHandler {
        bool elem1Start = false;
        bool elem1End = false;
        bool elem2Start = false;
        bool elem2End = false;
        bool elem3Start = false;
        bool elem3End = false;
        bool elem4Start = false;
        bool elem4End = false;

        void StartElement(const CString& container, const CString& elementName,
                                 const MeaXMLAttributes& attrs) override {
            if (elementName == _T("elem1")) {
                elem1Start = true;
                BOOST_TEST(container.IsEmpty());
            } else if (elementName == _T("elem2")) {
                elem2Start = true;
                BOOST_TEST(container == _T("elem1"));
            } else if(elementName == _T("elem3")) {
                elem3Start = true;
                BOOST_TEST(container == _T("elem2"));
            } else if(elementName == _T("elem4")) {
                elem4Start = true;
                BOOST_TEST(container == _T("elem2"));
                
                bool isDefault1;
                CString value1;
                BOOST_TEST(attrs.GetValueStr(_T("attr1"), value1, isDefault1));
                BOOST_TEST(value1 == _T("abc"));
                BOOST_TEST(!isDefault1);

                bool isDefault2;
                int value2;
                BOOST_TEST(attrs.GetValueInt(_T("attr2"), value2, isDefault2));
                BOOST_TEST(value2 == 1);
                BOOST_TEST(!isDefault2);

                bool isDefault3;
                double value3;
                BOOST_TEST(attrs.GetValueDbl(_T("attr3"), value3, isDefault3));
                BOOST_TEST(value3 == 2.5, tt::tolerance(FLT_EPSILON));
                BOOST_TEST(!isDefault3);
            } else {
                BOOST_FAIL("Unexpected start element " + elementName);
            }
        }

        void EndElement(const CString& container, const CString& elementName) override {
            if (elementName == _T("elem1")) {
                elem1End = true;
                BOOST_TEST(container.IsEmpty());
            } else if (elementName == _T("elem2")) {
                elem2End = true;
                BOOST_TEST(container == _T("elem1"));
            } else if (elementName == _T("elem3")) {
                elem3End = true;
                BOOST_TEST(container == _T("elem2"));
            } else if (elementName == _T("elem4")) {
                elem4End = true;
                BOOST_TEST(container == _T("elem2"));
            } else {
                BOOST_FAIL("Unexpected end element " + elementName);
            }
        }

        void CharacterData(const CString& container, const CString& data) override {
            BOOST_TEST(container.GetLength() > 0);
            BOOST_TEST(data.GetLength() > 0);
        }

        void ParseEntity(MeaXMLParser&, const CString& pathname) override {
            BOOST_TEST(pathname.IsEmpty());
        }

        void ParsingError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }

        void ValidationError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }
    } testHandler;

    MeaXMLParser parser(&testHandler);
    parser.ParseString(xml1);

    BOOST_TEST(testHandler.elem1Start);
    BOOST_TEST(testHandler.elem1End);
    BOOST_TEST(testHandler.elem2Start);
    BOOST_TEST(testHandler.elem2End);
    BOOST_TEST(testHandler.elem3Start);
    BOOST_TEST(testHandler.elem3End);
    BOOST_TEST(testHandler.elem4Start);
    BOOST_TEST(testHandler.elem4End);
}

BOOST_AUTO_TEST_CASE(TestDOMNoValidation) {
    MeaXMLParser parser;
    parser.ParseString(xml1);

    const MeaXMLNode* elem1 = parser.GetDOM();
    BOOST_TEST(elem1);

    // elem1
    BOOST_TEST(elem1->GetType() == MeaXMLNode::Type::Element);
    BOOST_TEST(elem1->GetData() == _T("elem1"));
    BOOST_TEST(!elem1->HasAttributes());

    MeaXMLNode::NodeIter_c children = elem1->GetChildIter();
    BOOST_TEST(!elem1->AtEnd(children));

    // elem2
    while ((*children)->GetType() != MeaXMLNode::Type::Element) {
        children++;
        BOOST_TEST(!elem1->AtEnd(children));
    }

    const MeaXMLNode* elem2 = *children++;
    BOOST_TEST(elem2->GetData() == _T("elem2"));
    BOOST_TEST(!elem2->HasAttributes());

    children = elem2->GetChildIter();
    BOOST_TEST(!elem2->AtEnd(children));

    // elem3
    while ((*children)->GetType() != MeaXMLNode::Type::Element) {
        children++;
        BOOST_TEST(!elem1->AtEnd(children));
    }

    const MeaXMLNode* elem3 = *children++;
    BOOST_TEST(elem3->GetData() == _T("elem3"));
    BOOST_TEST(!elem3->HasAttributes());
    BOOST_TEST(elem3->GetChildData() == _T("Test XML Data"));

    // elem4
    while ((*children)->GetType() != MeaXMLNode::Type::Element) {
        children++;
        BOOST_TEST(!elem1->AtEnd(children));
    }

    const MeaXMLNode* elem4 = *children++;
    BOOST_TEST(elem4->GetData() == _T("elem4"));
    BOOST_TEST(elem4->HasAttributes());

    const MeaXMLAttributes& attrs = elem4->GetAttributes();

    bool isDefault1;
    CString value1;
    BOOST_TEST(attrs.GetValueStr(_T("attr1"), value1, isDefault1));
    BOOST_TEST(value1 == _T("abc"));
    BOOST_TEST(!isDefault1);

    bool isDefault2;
    int value2;
    BOOST_TEST(attrs.GetValueInt(_T("attr2"), value2, isDefault2));
    BOOST_TEST(value2 == 1);
    BOOST_TEST(!isDefault2);

    bool isDefault3;
    double value3;
    BOOST_TEST(attrs.GetValueDbl(_T("attr3"), value3, isDefault3));
    BOOST_TEST(value3 == 2.5, tt::tolerance(FLT_EPSILON));
    BOOST_TEST(!isDefault3);
}

BOOST_AUTO_TEST_CASE(TestValidationInternalDTD) {

    struct TestHandler : public MeaXMLParserHandler {
        bool elem1Start = false;
        bool elem1End = false;

        void StartElement(const CString& container, const CString& elementName,
                                 const MeaXMLAttributes&) override {
            if (elementName == _T("elem1")) {
                elem1Start = true;
                BOOST_TEST(container.IsEmpty());
            } else {
                BOOST_FAIL("Unexpected start element " + elementName);
            }
        }

        void EndElement(const CString& container, const CString& elementName) override {
            if (elementName == _T("elem1")) {
                elem1End = true;
                BOOST_TEST(container.IsEmpty());
            } else {
                BOOST_FAIL("Unexpected end element " + elementName);
            }
        }

        void ParseEntity(MeaXMLParser&, const CString& pathname) override {
            BOOST_TEST(pathname.IsEmpty());
        }

        void ParsingError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }

        void ValidationError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }
    } testHandler;

    MeaXMLParser parser(&testHandler);
    parser.ParseString(xml2);

    BOOST_TEST(testHandler.elem1Start);
    BOOST_TEST(testHandler.elem1End);
}

BOOST_AUTO_TEST_CASE(TestValidationExternalDTD) {

    struct TestHandler : public MeaXMLParserHandler {
        bool hadEntity = false;

        void ParseEntity(MeaXMLParser& parser, const CString& pathname) override {
            hadEntity = true;

            TCHAR fname[_MAX_FNAME], ext[_MAX_EXT];
            _tsplitpath_s(pathname, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, ext, _MAX_EXT);

            BOOST_TEST(CString(fname) == _T("test"));
            BOOST_TEST(CString(ext) == _T(".dtd"));

            MeaXMLParser entityParser(parser);
            entityParser.ParseString(_T("<!ELEMENT elem1 (#PCDATA)>"));
        }

        void ParsingError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }

        void ValidationError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }
    } testHandler;

    MeaXMLParser parser(&testHandler);
    parser.ParseString(xml5);

    BOOST_TEST(testHandler.hadEntity);

}

BOOST_AUTO_TEST_CASE(TestParsingError) {

    struct TestHandler : public MeaXMLParserHandler {
        bool hadError = false;

        void ParsingError(const CString& error, const CString& pathname, int line, int column) override {
            hadError = true;

            BOOST_TEST(error == _T("mismatched tag"));
            BOOST_TEST(pathname.IsEmpty());
            BOOST_TEST(line == 6);
            BOOST_TEST(column == 3);
        }

        void ValidationError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }
    } testHandler;

    MeaXMLParser parser(&testHandler);
    BOOST_CHECK_THROW(parser.ParseString(xml3), MeaXMLParserException);

    BOOST_TEST(testHandler.hadError);
}

BOOST_AUTO_TEST_CASE(TestValidationError) {

    struct TestHandler : public MeaXMLParserHandler {
        bool hadError = false;

        void ParsingError(const CString& error, const CString&, int, int) override {
            BOOST_FAIL(error);
        }

        void ValidationError(const CString& error, const CString& pathname, int line, int column) override {
            hadError = true;

            BOOST_TEST(error == _T("Element 'elem2' not declared in DTD"));
            BOOST_TEST(pathname.IsEmpty());
            BOOST_TEST(line == 6);
            BOOST_TEST(column == 5);

            throw MeaXMLParserException();
        }
    } testHandler;

    MeaXMLParser parser(&testHandler);
    BOOST_CHECK_THROW(parser.ParseString(xml4), MeaXMLParserException);

    BOOST_TEST(testHandler.hadError);
}

BOOST_AUTO_TEST_CASE(TestEncode) {
    BOOST_TEST(MeaXMLParser::Encode("") == _T(""));
    BOOST_TEST(MeaXMLParser::Encode("Hello world") == _T("Hello world"));
    BOOST_TEST(MeaXMLParser::Encode("a & b < c > d 'e' \"f\"") == _T("a &amp; b &lt; c &gt; d &apos;e&apos; &quot;f&quot;"));
}

boost::test_tools::assertion_result IsANSI(boost::unit_test::test_unit_id) {
    return GetACP() == 1252;
}

BOOST_AUTO_TEST_CASE(TestFromUTF8, * boost::unit_test::precondition(IsANSI)) {
    BOOST_TEST(MeaXMLParser::FromUTF8(_T("")) == _T(""));
    BOOST_TEST(MeaXMLParser::FromUTF8(_T("Hello world")) == _T("Hello world"));
    BOOST_TEST(MeaXMLParser::FromUTF8(_T(u8"\u2122\u2026")) == _T("\x99\x85"));
}

BOOST_AUTO_TEST_CASE(TestToUTF8, * boost::unit_test::precondition(IsANSI)) {
    BOOST_TEST(MeaXMLParser::ToUTF8(CString()) == _T(""));
    BOOST_TEST(MeaXMLParser::ToUTF8(CString(_T("Hello world"))) == _T("Hello world"));
    BOOST_TEST(MeaXMLParser::ToUTF8(CString(_T("\x99\x85"))) == _T(u8"\u2122\u2026"));
}
