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
#include "XMLParser.h"
#include <meazure/resource.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <cstring>
#include <iostream>

#ifdef _UNICODE
#define PCXMLCH CString::PCXSTR
#else
#define PCXMLCH CString::PCYSTR
#endif
#define CAST_PCXMLCH(str) reinterpret_cast<PCXMLCH>(str)


//*************************************************************************
// MeaXMLAttributes
//*************************************************************************


MeaXMLAttributes::MeaXMLAttributes(const xercesc::AttributeList& atts) {
    for (XMLSize_t i = 0; i < atts.getLength(); i++) {
        PCXMLCH name = CAST_PCXMLCH(atts.getName(i));
        PCXMLCH value = CAST_PCXMLCH(atts.getValue(i));

        m_attributeMap[CString(name)] = CString(value);
    }
}

bool MeaXMLAttributes::GetValueStr(PCTSTR name, CString& value) const {
    AttributeMap::const_iterator iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = (*iter).second;
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueInt(PCTSTR name, int& value) const {
    AttributeMap::const_iterator iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = _ttoi((*iter).second);
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueDbl(PCTSTR name, double& value) const {
    AttributeMap::const_iterator iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = _tcstod((*iter).second, nullptr);
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueBool(PCTSTR name, bool& value) const {
    AttributeMap::const_iterator iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        const CString& vstr = (*iter).second;
        value = (vstr == _T("true") || vstr == _T("1"));
        return true;
    }
    return false;
}

MeaXMLAttributes& MeaXMLAttributes::Assign(const MeaXMLAttributes& attrs) {
    m_attributeMap = attrs.m_attributeMap;
    return *this;
}


//*************************************************************************
// MeaXMLNode
//*************************************************************************


MeaXMLNode::MeaXMLNode() :
    m_type(Type::Unknown),
    m_parent(nullptr) {}

MeaXMLNode::MeaXMLNode(const CString& elementName, const MeaXMLAttributes& attrs) :
    m_type(Type::Element),
    m_data(elementName),
    m_attributes(attrs),
    m_parent(nullptr) {}

MeaXMLNode::MeaXMLNode(const CString& data) :
    m_type(Type::Data),
    m_data(data),
    m_parent(nullptr) {}

MeaXMLNode::~MeaXMLNode() {
    try {
        for (auto child : m_children) {
            delete child;
        }
        m_children.clear();
    } catch (...) {
        assert(false);
    }
}

MeaXMLNode& MeaXMLNode::Assign(const MeaXMLNode& node) {
    m_type = node.m_type;
    m_data = node.m_data;
    m_attributes = node.m_attributes;
    m_children = node.m_children;
    m_parent = node.m_parent;

    return *this;
}

CString MeaXMLNode::GetChildData() const {
    CString data;

    for (const MeaXMLNode* node : m_children) {
        if (node->GetType() == MeaXMLNode::Type::Data) {
            data += node->GetData();
        }
    }

    return data;
}


std::ostream& operator<<(std::ostream& os, const MeaXMLNode::Type& type){
    switch (type) {
    case MeaXMLNode::Type::Element:
        os << "Element";
        break;
    case MeaXMLNode::Type::Data:
        os << "Data";
        break;
    default:
        os << "Unknown";
        break;
    }

    return os;
}


std::ostream& operator<<(std::ostream& os, const MeaXMLNode& node){
    static int indent = 0;

    CString indentStr(_T(' '), indent);

    MeaXMLNode::Type type = node.GetType();

    os << indentStr << _T("Node Type: ") << type << _T('\n');
    if (type == MeaXMLNode::Type::Element) {
        os << indentStr << _T("Name: ") << node.GetData() << _T('\n');
    } else if (type == MeaXMLNode::Type::Data) {
        os << indentStr << _T("Data: ") << node.GetData() << _T('\n');
    }

    for (auto child : node.m_children) {
        indent += 4;
        os << child;
        indent -= 4;
    }

    return os;
}


//*************************************************************************
// MeaXMLParserHandler
//*************************************************************************


void MeaXMLParserHandler::StartElement(const CString&, const CString&, const MeaXMLAttributes&) {}

void MeaXMLParserHandler::EndElement(const CString&, const CString&) {}

void MeaXMLParserHandler::CharacterData(const CString&, const CString&) {}

xercesc::InputSource* MeaXMLParserHandler::ResolveEntity(const CString&) {
    return nullptr;
}

void MeaXMLParserHandler::ParsingError(const CString& error, const CString& pathname, int line, int column) {
    CString title(reinterpret_cast<PCSTR>(IDS_MEA_PARSER_TITLE));
    CString msg;
    
    if (GetConsoleWindow() == nullptr) {
        msg.Format(IDS_MEA_PARSER_MSG, static_cast<PCTSTR>(pathname), line, column, static_cast<PCTSTR>(error));
        MessageBox(*AfxGetMainWnd(), msg, title, MB_OK | MB_ICONERROR);
    } else {
        msg.Format(_T("XML format error in %s (%d, %d): %s"), static_cast<PCTSTR>(pathname), line, column,
                   static_cast<PCTSTR>(error));
        std::cerr << msg << _T('\n');
    }
}

void MeaXMLParserHandler::ValidationError(const CString& error, const CString& pathname, int line, int column) {
    CString title(reinterpret_cast<PCSTR>(IDS_MEA_VALIDATION_TITLE));
    CString msg;

    if (GetConsoleWindow() == nullptr) {
        msg.Format(IDS_MEA_VALIDATION_MSG, static_cast<PCTSTR>(pathname), line, column, static_cast<PCTSTR>(error));
        MessageBox(*AfxGetMainWnd(), msg, title, MB_OK | MB_ICONERROR);
    } else {
        msg.Format(_T("XML validation error in %s (%d, %d): %s"), static_cast<PCTSTR>(pathname), line, column,
                   static_cast<PCTSTR>(error));
        std::cerr << msg << _T('\n');
    }

    throw MeaXMLParserException();
}

CString MeaXMLParserHandler::GetFilePathname() {
    return _T("");
}


//*************************************************************************
// MeaXMLParser
//*************************************************************************


MeaXMLParserHandler MeaXMLParser::m_noopHandler;
const CString MeaXMLParser::m_homeURL1(_T("https://www.cthing.com/"));
const CString MeaXMLParser::m_homeURL2(_T("http://www.cthing.com/"));


MeaXMLParser::MeaXMLParser() : MeaXMLParser(&m_noopHandler, true) {}

MeaXMLParser::MeaXMLParser(MeaXMLParserHandler* handler, bool buildDOM) :
    m_handler(handler),
    m_buildDOM(buildDOM),
    m_dom(nullptr) {

    xercesc::XMLPlatformUtils::Initialize();

    m_parser = new xercesc::SAXParser();
    m_parser->setValidationScheme(xercesc::SAXParser::ValSchemes::Val_Auto);
    m_parser->setDocumentHandler(this);
    m_parser->setEntityResolver(this);
    m_parser->setErrorHandler(this);
}

MeaXMLParser::~MeaXMLParser() {
    try {
        delete m_parser;
        if (m_dom != nullptr) {
            delete m_dom;
        }

        xercesc::XMLPlatformUtils::Terminate();
    } catch (...) {
        assert(false);
    }
}

void MeaXMLParser::ParseFile(PCTSTR pathname) {
    m_parser->parse(pathname);
}

void MeaXMLParser::ParseString(PCTSTR content) {
    assert(content != nullptr);

#ifdef _UNICODE
    int numWideChars = wcslen(content);
    int numBytes = WideCharToMultiByte(CP_UTF8, 0, content, numWideChars, nullptr, 0, nullptr, nullptr);
    char* utf8Content = new char[numBytes];
    WideCharToMultiByte(CP_UTF8, 0, content, numWideChars, utf8Content, numBytes, nullptr, nullptr);
#else
    int numWideChars = MultiByteToWideChar(CP_ACP, 0, content, -1, nullptr, 0);
    wchar_t* wideContent = new wchar_t[numWideChars];
    MultiByteToWideChar(CP_ACP, 0, content, -1, wideContent, numWideChars);

    int numBytes = WideCharToMultiByte(CP_UTF8, 0, wideContent, numWideChars, nullptr, 0, nullptr, nullptr);
    char* utf8Content = new char[numBytes];
    WideCharToMultiByte(CP_UTF8, 0, wideContent, numWideChars, utf8Content, numBytes, nullptr, nullptr);

    delete[] wideContent;
#endif /* XML_UNICODE */

    xercesc::MemBufInputSource source(reinterpret_cast<const XMLByte*>(utf8Content), numBytes, "XMLBuf");
    m_parser->parse(source);

#ifndef _UNICODE
    delete[] utf8Content;
#endif
}


void MeaXMLParser::startDocument() {
}

void MeaXMLParser::setDocumentLocator(const xercesc::Locator* const /*locator*/) {
}

void MeaXMLParser::processingInstruction(const XMLCh* const /*target*/, const XMLCh* const /*data*/) {
}

void MeaXMLParser::startElement(const XMLCh* const elementName, xercesc::AttributeList& attrs) {
    MeaXMLAttributes attributes(attrs);
    CString name(CAST_PCXMLCH(elementName));
    CString container;

    if (!m_elementStack.empty()) {
        container = m_elementStack.top();
    }
    m_handler->StartElement(container, name, attributes);
    m_elementStack.push(name);

    if (m_buildDOM) {
        MeaXMLNode* node = new MeaXMLNode(name, attributes);
        if (m_nodeStack.empty()) {
            assert(m_dom == nullptr);
            m_dom = node;
        } else {
            m_nodeStack.top()->AddChild(node);
        }
        m_nodeStack.push(node);
    }
}

void MeaXMLParser::endElement(const XMLCh* const elementName) {
    CString name(CAST_PCXMLCH(elementName));
    
    assert(!m_elementStack.empty());
    m_elementStack.pop();
    
    CString container;
    if (!m_elementStack.empty()) {
        container = m_elementStack.top();
    }
    m_handler->EndElement(container, name);

    if (m_buildDOM && !m_nodeStack.empty()) {
        m_nodeStack.pop();
    }
}

void MeaXMLParser::characters(const XMLCh* const chars, const XMLSize_t length) {
    CString container;
    if (!m_elementStack.empty()) {
        container = m_elementStack.top();
    }
    if (!container.IsEmpty()) {
        CString data(CAST_PCXMLCH(chars), static_cast<int>(length));
        m_handler->CharacterData(container, data);

        if (m_buildDOM && !m_nodeStack.empty()) {
            m_nodeStack.top()->AddChild(new MeaXMLNode(data));
        }
    }
}

void MeaXMLParser::ignorableWhitespace(const XMLCh* const /*chars*/, const XMLSize_t /*length*/) {
    // Ignore the whitespace.
}

void MeaXMLParser::endDocument() {
}

void MeaXMLParser::resetDocument() {
    if (m_dom != nullptr) {
        delete m_dom;
    }
    m_dom = nullptr;

    while (!m_elementStack.empty()) {
        m_elementStack.pop();
    }

    while (!m_nodeStack.empty()) {
        m_nodeStack.pop();
    }

    while (!m_pathnameStack.empty()) {
        m_pathnameStack.pop();
    }
}

xercesc::InputSource* MeaXMLParser::resolveEntity(const XMLCh* const /*publicId*/, const XMLCh* const systemId) {
    if (systemId != nullptr) {
        CString sysId(CAST_PCXMLCH(systemId));

        int homeURLPos = sysId.Find(m_homeURL1);
        int homeURLLen = m_homeURL1.GetLength();
        if (homeURLPos < 0) {
            homeURLPos = sysId.Find(m_homeURL2);
            homeURLLen = m_homeURL2.GetLength();
        }

        if (homeURLPos == 0) {
            TCHAR pathname[_MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR];
            GetModuleFileName(nullptr, pathname, _MAX_PATH);
            _tsplitpath_s(pathname, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

            sysId = CString(drive) + CString(dir) + sysId.Mid(homeURLLen);
            sysId.Replace(_T('/'), _T('\\'));

            m_pathnameStack.push(sysId);
            xercesc::InputSource* source = m_handler->ResolveEntity(sysId);
            m_pathnameStack.pop();

            return source;
        }
    }

    return nullptr;
}

void MeaXMLParser::warning(const xercesc::SAXParseException& /*exc*/) {
    // Ignore warnings
}

void MeaXMLParser::error(const xercesc::SAXParseException& exc) {
    CString msg(CAST_PCXMLCH(exc.getMessage()));
    CString pathname(m_pathnameStack.empty() ? m_handler->GetFilePathname() : m_pathnameStack.top());
    int line = static_cast<int>(exc.getLineNumber());
    int column = static_cast<int>(exc.getColumnNumber());

    m_handler->ValidationError(msg, pathname, line, column);

    throw MeaXMLParserException();
}

void MeaXMLParser::fatalError(const xercesc::SAXParseException& exc) {
    CString msg(CAST_PCXMLCH(exc.getMessage()));
    CString pathname(m_pathnameStack.empty() ? m_handler->GetFilePathname() : m_pathnameStack.top());
    int line = static_cast<int>(exc.getLineNumber());
    int column = static_cast<int>(exc.getColumnNumber());

    m_handler->ParsingError(msg, pathname, line, column);

    throw MeaXMLParserException();
}

void MeaXMLParser::resetErrors() {
}
