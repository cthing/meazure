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
#include "XMLParser.h"
#include <meazure/resource.h>
#include <mbstring.h>
#include <cstring>


 //*************************************************************************
 // MeaXMLAttributes
 //*************************************************************************


MeaXMLAttributes::MeaXMLAttributes() {}

MeaXMLAttributes::MeaXMLAttributes(const XML_Char** atts, int numSpecified) {
    assert(atts != nullptr);

    // Run through the atts and insert them in the map. Also mark
    // which attributes are set by default versus  having been
    // explicitly specified.
    //
    numSpecified *= 2;
    for (int i = 0; atts[i] != nullptr; i += 2) {
        AttributeValue v;

        v.value = MeaXMLParser::FromUTF8(atts[i + 1]);
        v.isDefault = (i >= numSpecified);
        m_attributeMap[MeaXMLParser::FromUTF8(atts[i])] = v;
    }
}

bool MeaXMLAttributes::GetValueStr(LPCTSTR name, CString& value, bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = (*iter).second.value;
        isDefault = (*iter).second.isDefault;
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueInt(LPCTSTR name, int& value, bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = _ttoi((*iter).second.value);
        isDefault = (*iter).second.isDefault;
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueDbl(LPCTSTR name, double& value, bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = _tcstod((*iter).second.value, nullptr);
        isDefault = (*iter).second.isDefault;
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueBool(LPCTSTR name, bool& value, bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        const CString& vstr = (*iter).second.value;
        value = (vstr == _T("true") || vstr == _T("1"));
        isDefault = (*iter).second.isDefault;
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


MeaXMLNode::MeaXMLNode() : m_type(Type::Unknown), m_parent(nullptr) {}

MeaXMLNode::MeaXMLNode(const CString& elementName, const MeaXMLAttributes& attrs) : m_type(Type::Element),
    m_data(elementName), m_attributes(attrs),
    m_parent(nullptr) {}

MeaXMLNode::MeaXMLNode(const CString& data) :
    m_type(Type::Data), m_data(data), m_parent(nullptr) {}

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

#ifdef MEA_XMLNODE_DEBUG

void MeaXMLNode::Dump() const {
    static int indent = 0;

    CString indentStr(_T(' '), indent);

    TRACE(_T("%sNode Type: "), (LPCTSTR)indentStr);
    switch (m_type) {
    case Unknown:
        TRACE(_T("Unknown\n"));
        break;
    case Element:
        TRACE(_T("Element - %s\n"), m_data);
        break;
    case Data:
        TRACE(_T("Data - '%s'\n"), m_data);
        break;
    }

    for (auto child : m_children) {
        indent += 4;
        child->Dump();
        indent -= 4;
    }
}

#endif


//*************************************************************************
// MeaXMLParserHandler
//*************************************************************************


void MeaXMLParserHandler::StartElement(const CString&, const CString&, const MeaXMLAttributes&) {}

void MeaXMLParserHandler::EndElement(const CString&, const CString&) {}

void MeaXMLParserHandler::CharacterData(const CString&, const CString&) {}

void MeaXMLParserHandler::ParseEntity(MeaXMLParser&, const CString&) {}

void MeaXMLParserHandler::ParsingError(const CString& error, const CString& pathname, int line, int column) {
    CString title(reinterpret_cast<LPCSTR>(IDS_MEA_PARSER_TITLE));
    CString msg;

    msg.Format(IDS_MEA_PARSER_MSG, static_cast<LPCTSTR>(pathname), line, column, static_cast<LPCTSTR>(error));
    MessageBox(*AfxGetMainWnd(), msg, title, MB_OK | MB_ICONERROR);
}

void MeaXMLParserHandler::ValidationError(const CString& error, const CString& pathname, int line, int column) {
    CString title(reinterpret_cast<LPCSTR>(IDS_MEA_VALIDATION_TITLE));
    CString msg;

    msg.Format(IDS_MEA_VALIDATION_MSG, static_cast<LPCTSTR>(pathname), line, column, static_cast<LPCTSTR>(error));
    MessageBox(*AfxGetMainWnd(), msg, title, MB_OK | MB_ICONERROR);

    throw MeaXMLParserException();
}

CString MeaXMLParserHandler::GetFilePathname() {
    return _T("");
}


//*************************************************************************
// MeaXMLParser
//*************************************************************************


const CString MeaXMLParser::m_homeURL1(_T("https://www.cthing.com/"));
const CString MeaXMLParser::m_homeURL2(_T("http://www.cthing.com/"));
MeaXMLParserHandler MeaXMLParser::m_noopHandler;


MeaXMLParser::MeaXMLParser() : MeaXMLParser(&m_noopHandler, true) {}

MeaXMLParser::MeaXMLParser(MeaXMLParserHandler* handler, bool buildDOM) :
    IValidationHandler(),
    m_isSubParser(false),
    m_handler(handler),
    m_haveDTD(false),
    m_context(nullptr),
    m_buildDOM(buildDOM),
    m_dom(nullptr),
    m_pathnameStack(new PathnameStack),
    m_elementStack(new ElementStack),
    m_nodeStack(new NodeStack),
    m_parser(XML_ParserCreate(nullptr)),
    m_validator(new ev::Validator(this)) {

    assert(m_parser != nullptr);

    XML_SetUserData(m_parser, this);
    XML_SetExternalEntityRefHandlerArg(m_parser, this);
    XML_SetParamEntityParsing(m_parser, XML_PARAM_ENTITY_PARSING_UNLESS_STANDALONE);

    XML_SetElementHandler(m_parser, StartElementHandler, EndElementHandler);
    XML_SetCharacterDataHandler(m_parser, CharacterDataHandler);
    XML_SetExternalEntityRefHandler(m_parser, ExternalEntityRefHandler);
    XML_SetStartDoctypeDeclHandler(m_parser, DoctypeDeclHandler);
    XML_SetElementHandler(m_parser, StartElementHandler, EndElementHandler);
    XML_SetElementDeclHandler(m_parser, ElementDeclHandler);
    XML_SetAttlistDeclHandler(m_parser, AttributeDeclHandler);
}

MeaXMLParser::MeaXMLParser(const MeaXMLParser& parentParser) :
    IValidationHandler(),
    m_isSubParser(true),
    m_validator(parentParser.m_validator),
    m_handler(parentParser.m_handler),
    m_elementStack(parentParser.m_elementStack),
    m_pathnameStack(parentParser.m_pathnameStack),
    m_haveDTD(parentParser.m_haveDTD),
    m_context(parentParser.m_context),
    m_buildDOM(parentParser.m_buildDOM),
    m_dom(parentParser.m_dom),
    m_nodeStack(parentParser.m_nodeStack) {

    m_parser = XML_ExternalEntityParserCreate(parentParser.m_parser, m_context, nullptr);
    assert(m_parser != nullptr);

    XML_SetUserData(m_parser, this);
    XML_SetExternalEntityRefHandlerArg(m_parser, this);
}

MeaXMLParser::~MeaXMLParser() {
    try {
        XML_ParserFree(m_parser);
        if (!m_isSubParser) {
            delete m_validator;
            delete m_pathnameStack;
            delete m_elementStack;
            delete m_nodeStack;
            delete m_dom;
        }
    } catch (...) {
        assert(false);
    }
}

void MeaXMLParser::SetBasePath(const CString& path) {
    XML_SetBase(m_parser, ToUTF8(path));
}

void MeaXMLParser::ParseBuffer(int len, bool isFinal) {
    if (XML_ParseBuffer(m_parser, len, isFinal) == 0) {
        HandleParserError();
        throw MeaXMLParserException();
    }
}

void MeaXMLParser::ParseString(LPCTSTR content) {
    assert(content != nullptr);

    int numBytes = static_cast<int>(strlen(content));
    void* buf = GetBuffer(numBytes);
    std::memcpy(buf, content, numBytes);
    ParseBuffer(numBytes, true);
}

void MeaXMLParser::StartElementHandler(void* userData, const XML_Char* elementName, const XML_Char** attrs) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);

    if (ps->m_haveDTD) {
        ps->m_validator->StartElement(ps->m_parser, elementName, attrs);
    }

    MeaXMLAttributes attributes(attrs, XML_GetSpecifiedAttributeCount(ps->m_parser));
    CString name(FromUTF8(elementName));
    CString container;
    if (!ps->m_elementStack->empty()) {
        container = ps->m_elementStack->top();
    }
    ps->m_handler->StartElement(container, name, attributes);
    ps->m_elementStack->push(name);

    if (ps->m_buildDOM) {
        MeaXMLNode* node = new MeaXMLNode(name, attributes);
        if (ps->m_nodeStack->empty()) {
            assert(ps->m_dom == nullptr);
            ps->m_dom = node;
        } else {
            ps->m_nodeStack->top()->AddChild(node);
        }
        ps->m_nodeStack->push(node);
    }
}

void MeaXMLParser::EndElementHandler(void* userData, const XML_Char* elementName) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);

    if (ps->m_haveDTD) {
        ps->m_validator->EndElement(ps->m_parser);
    }

    CString name(FromUTF8(elementName));
    assert(!ps->m_elementStack->empty());
    ps->m_elementStack->pop();
    CString container;
    if (!ps->m_elementStack->empty()) {
        container = ps->m_elementStack->top();
    }
    ps->m_handler->EndElement(container, name);

    if (ps->m_buildDOM && !ps->m_nodeStack->empty()) {
        ps->m_nodeStack->pop();
    }
}

void MeaXMLParser::CharacterDataHandler(void* userData, const XML_Char* s, int len) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);
    if (ps->m_haveDTD) {
        ps->m_validator->CharacterData(ps->m_parser, s, len);
    }

    CString container;
    if (!ps->m_elementStack->empty()) {
        container = ps->m_elementStack->top();
    }
    if (!container.IsEmpty() && ps->m_validator->IsMixed(container)) {
        CString data(s, len);
        ps->m_handler->CharacterData(container, data);

        if (ps->m_buildDOM && !ps->m_nodeStack->empty()) {
            ps->m_nodeStack->top()->AddChild(new MeaXMLNode(data));
        }
    }
}

int MeaXMLParser::ExternalEntityRefHandler(XML_Parser parser,
                                           const XML_Char* context,
                                           const XML_Char* /*base*/,
                                           const XML_Char* systemId,
                                           const XML_Char* /*publicId*/) {
    if (systemId != nullptr) {
        MeaXMLParser* ps = reinterpret_cast<MeaXMLParser*>(parser);
        CString sysId(FromUTF8(systemId));

        int homeURLPos = sysId.Find(MeaXMLParser::m_homeURL1);
        int homeURLLen = MeaXMLParser::m_homeURL1.GetLength();
        if (homeURLPos < 0) {
            homeURLPos = sysId.Find(MeaXMLParser::m_homeURL2);
            homeURLLen = MeaXMLParser::m_homeURL2.GetLength();
        }

        if (homeURLPos == 0) {
            TCHAR pathname[_MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR];
            GetModuleFileName(nullptr, pathname, _MAX_PATH);
            _tsplitpath_s(pathname, drive, _MAX_DRIVE, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

            sysId = CString(drive) + CString(dir) + sysId.Mid(homeURLLen);
            sysId.Replace(_T('/'), _T('\\'));

            ps->m_pathnameStack->push(sysId);

            ps->m_context = context;
            ps->m_handler->ParseEntity(*ps, sysId);
            ps->m_context = nullptr;

            ps->m_pathnameStack->pop();
        }
    }

    return 1;
}

void MeaXMLParser::DoctypeDeclHandler(void* userData,
                                      const XML_Char* doctypeName,
                                      const XML_Char* /*sysid*/,
                                      const XML_Char* /*pubid*/,
                                      int /*has_internal_subset*/) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);

    ps->m_haveDTD = true;
    ps->m_validator->SetDocumentElement(doctypeName);
}

void MeaXMLParser::ElementDeclHandler(void* userData, const XML_Char* name, XML_Content* model) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);
    ps->m_validator->AddElementDecl(name, model);
    XML_FreeContentModel(ps->m_parser, model);
}

void MeaXMLParser::AttributeDeclHandler(void* userData,
                                        const XML_Char* elname,
                                        const XML_Char* attname,
                                        const XML_Char* att_type,
                                        const XML_Char* dflt,
                                        int isrequired) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);
    ps->m_validator->AddAttributeDecl(elname, attname,
                                     att_type, dflt, isrequired);
}

void MeaXMLParser::HandleParserError() {
    CString msg(XML_ErrorString(XML_GetErrorCode(m_parser)));
    CString pathname(m_pathnameStack->empty() ? m_handler->GetFilePathname() : m_pathnameStack->top());
    int line = XML_GetCurrentLineNumber(m_parser);
    int column = XML_GetCurrentColumnNumber(m_parser) + 1;

    m_handler->ParsingError(msg, pathname, line, column);
}

void MeaXMLParser::HandleValidationError(const ev::ValidationError& error) {
    CString msg(error.GetMessage());
    CString pathname(m_handler->GetFilePathname());
    int line = error.GetLineNumber();
    int column = error.GetCharacterPosition() + 1;

    m_handler->ValidationError(msg, pathname, line, column);
}

CString MeaXMLParser::FromUTF8(LPCTSTR str) {
#ifdef XML_UNICODE
    return str;
#else
    int numChars = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    wchar_t* buf = new wchar_t[numChars];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buf, numChars);
    CString buffer(buf);
    delete[] buf;
    return buffer;
#endif /* XML_UNICODE */
}

CString MeaXMLParser::ToUTF8(const CString& str) {
#ifdef _UNICODE
    return str;
#else
    int numWChars = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
    wchar_t* wbuf = new wchar_t[numWChars];
    MultiByteToWideChar(CP_ACP, 0, str, -1, wbuf, numWChars);

    CString buffer;

    int numMBChars = WideCharToMultiByte(CP_UTF8, 0, wbuf, numWChars, nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_UTF8, 0, wbuf, numWChars, buffer.GetBufferSetLength(numMBChars), numMBChars, nullptr, nullptr);
    buffer.ReleaseBuffer();

    delete[] wbuf;

    return buffer;
#endif
}

CString MeaXMLParser::Encode(const CString& src) {
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
