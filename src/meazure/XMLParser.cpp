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
#include "Resource.h"
#include "XMLParser.h"
#include <mbstring.h>


 //*************************************************************************
 // MeaXMLAttributes
 //*************************************************************************


MeaXMLAttributes::MeaXMLAttributes() {}

MeaXMLAttributes::MeaXMLAttributes(const XML_Char** atts, int numSpecified) {
    MeaAssert(atts != nullptr);

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

MeaXMLAttributes::~MeaXMLAttributes() {}

bool MeaXMLAttributes::GetValueStr(LPCTSTR name, CString& value,
                                   bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = (*iter).second.value;
        isDefault = (*iter).second.isDefault;
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueInt(LPCTSTR name, int& value,
                                   bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = _ttoi((*iter).second.value);
        isDefault = (*iter).second.isDefault;
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueDbl(LPCTSTR name, double& value,
                                   bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        value = _tcstod((*iter).second.value, nullptr);
        isDefault = (*iter).second.isDefault;
        return true;
    }
    return false;
}

bool MeaXMLAttributes::GetValueBool(LPCTSTR name, bool& value,
                                    bool& isDefault) const {
    std::map<CString, AttributeValue>::const_iterator iter;

    iter = m_attributeMap.find(name);
    if (iter != m_attributeMap.end()) {
        CString vstr = (*iter).second.value;
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

MeaXMLNode::MeaXMLNode(const CString& elementName,
                       const MeaXMLAttributes& attrs) : m_type(Type::Element),
    m_data(elementName), m_attributes(attrs),
    m_parent(nullptr) {}

MeaXMLNode::MeaXMLNode(const CString& data) : m_type(Type::Data), m_data(data),
m_parent(nullptr) {}

MeaXMLNode::~MeaXMLNode() {
    try {
        for (auto child : m_children) {
            delete child;
        }
        m_children.clear();
    } catch (...) {
        MeaAssert(false);
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


void MeaXMLParserHandler::StartElementHandler(const CString& /* container */,
                                              const CString& /* elementName */,
                                              const MeaXMLAttributes& /* attrs */) {}

void MeaXMLParserHandler::EndElementHandler(const CString& /* container */,
                                            const CString& /* elementName */) {}

void MeaXMLParserHandler::CharacterDataHandler(const CString& /* container */,
                                               const CString& /* data */) {}

void MeaXMLParserHandler::ParseEntity(MeaXMLParser& /*parser*/,
                                      const CString& /*pathname*/) {}

CString MeaXMLParserHandler::GetFilePathname() {
    return _T("");
}


//*************************************************************************
// MeaXMLParser
//*************************************************************************


CString MeaXMLParser::m_homeURL1(_T("https://www.cthing.com/"));
CString MeaXMLParser::m_homeURL2(_T("http://www.cthing.com/"));


MeaXMLParser::MeaXMLParser(MeaXMLParserHandler* handler, bool buildDOM) :
    IValidationHandler(),
    m_isSubParser(false),
    m_handler(handler),
    m_haveDTD(false),
    m_context(nullptr),
    m_buildDOM(buildDOM),
    m_dom(nullptr) {
    // Create the XML parser and set its handlers.
    //
    m_pathnameStack = new PathnameStack;
    m_elementStack = new ElementStack;
    m_nodeStack = new NodeStack;

    m_parser = XML_ParserCreate(nullptr);
    MeaAssert(m_parser != nullptr);

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

    // Create the validator.
    //
    m_validator = new ev::Validator(this);
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
    m_parser = XML_ExternalEntityParserCreate(parentParser.m_parser,
                                                m_context, nullptr);
    MeaAssert(m_parser != nullptr);

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
        MeaAssert(false);
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

void MeaXMLParser::StartElementHandler(void* userData, const XML_Char* elementName,
                                        const XML_Char** attrs) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);

    if (ps->m_haveDTD) {
        ps->m_validator->StartElement(ps->m_parser, elementName, attrs);
    }

    MeaXMLAttributes attributes(attrs, XML_GetSpecifiedAttributeCount(ps->m_parser));
    CString name(FromUTF8(elementName));
    CString container;
    if (!ps->m_elementStack->empty())
        container = ps->m_elementStack->top();
    ps->m_handler->StartElementHandler(container, name, attributes);
    ps->m_elementStack->push(name);

    if (ps->m_buildDOM) {
        MeaXMLNode* node = new MeaXMLNode(name, attributes);
        if (ps->m_nodeStack->empty()) {
            MeaAssert(ps->m_dom == nullptr);
            ps->m_dom = node;
        } else {
            ps->m_nodeStack->top()->AddChild(node);
        }
        ps->m_nodeStack->push(node);
    }
}

void MeaXMLParser::EndElementHandler(void* userData, const XML_Char* elementName) {
    MeaXMLParser* ps = static_cast<MeaXMLParser*>(userData);

    if (ps->m_haveDTD)
        ps->m_validator->EndElement(ps->m_parser);

    CString name(FromUTF8(elementName));
    MeaAssert(!ps->m_elementStack->empty());
    ps->m_elementStack->pop();
    CString container;
    if (!ps->m_elementStack->empty()) {
        container = ps->m_elementStack->top();
    }
    ps->m_handler->EndElementHandler(container, name);

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
        ps->m_handler->CharacterDataHandler(container, data);

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

void MeaXMLParser::ElementDeclHandler(void* userData,
                                        const XML_Char* name,
                                        XML_Content* model) {
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
    CString title(reinterpret_cast<LPCSTR>(IDS_MEA_PARSER_TITLE));
    CString msg, errorMsg;
    CString pathname(m_pathnameStack->empty() ?
                        m_handler->GetFilePathname() : m_pathnameStack->top());

    msg.Format(IDS_MEA_PARSER_MSG,
                static_cast<LPCTSTR>(pathname),
                XML_GetCurrentLineNumber(m_parser),
                XML_GetCurrentColumnNumber(m_parser) + 1);

    switch (XML_GetErrorCode(m_parser)) {
    case XML_ERROR_NONE:
        return;
    case XML_ERROR_NO_MEMORY:
        errorMsg.LoadString(IDS_MEA_NO_MEMORY);
        break;
    case XML_ERROR_SYNTAX:
        errorMsg.LoadString(IDS_MEA_SYNTAX);
        break;
    case XML_ERROR_NO_ELEMENTS:
        errorMsg.LoadString(IDS_MEA_NO_ELEMENTS);
        break;
    case XML_ERROR_INVALID_TOKEN:
        errorMsg.LoadString(IDS_MEA_INVALID_TOKEN);
        break;
    case XML_ERROR_UNCLOSED_TOKEN:
        errorMsg.LoadString(IDS_MEA_UNCLOSED_TOKEN);
        break;
    case XML_ERROR_PARTIAL_CHAR:
        errorMsg.LoadString(IDS_MEA_PARTIAL_CHAR);
        break;
    case XML_ERROR_TAG_MISMATCH:
        errorMsg.LoadString(IDS_MEA_TAG_MISMATCH);
        break;
    case XML_ERROR_DUPLICATE_ATTRIBUTE:
        errorMsg.LoadString(IDS_MEA_DUPLICATE_ATTRIBUTE);
        break;
    case XML_ERROR_JUNK_AFTER_DOC_ELEMENT:
        errorMsg.LoadString(IDS_MEA_DOC_ELEMENT);
        break;
    case XML_ERROR_PARAM_ENTITY_REF:
        errorMsg.LoadString(IDS_MEA_ENTITY_REF);
        break;
    case XML_ERROR_UNDEFINED_ENTITY:
        errorMsg.LoadString(IDS_MEA_UNDEFINED_ENTITY);
        break;
    case XML_ERROR_RECURSIVE_ENTITY_REF:
        errorMsg.LoadString(IDS_MEA_RECURSIVE_ENTITY_REF);
        break;
    case XML_ERROR_ASYNC_ENTITY:
        errorMsg.LoadString(IDS_MEA_ASYNC_ENTITY);
        break;
    case XML_ERROR_BAD_CHAR_REF:
        errorMsg.LoadString(IDS_MEA_BAD_CHAR_REF);
        break;
    case XML_ERROR_BINARY_ENTITY_REF:
        errorMsg.LoadString(IDS_MEA_BINARY_ENTITY_REF);
        break;
    case XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF:
        errorMsg.LoadString(IDS_MEA_ATTRIBUTE_EXTERNAL_ENTITY_REF);
        break;
    case XML_ERROR_MISPLACED_XML_PI:
        errorMsg.LoadString(IDS_MEA_MISPLACED_XML_PI);
        break;
    case XML_ERROR_UNKNOWN_ENCODING:
        errorMsg.LoadString(IDS_MEA_UNKNOWN_ENCODING);
        break;
    case XML_ERROR_INCORRECT_ENCODING:
        errorMsg.LoadString(IDS_MEA_INCORRECT_ENCODING);
        break;
    case XML_ERROR_UNCLOSED_CDATA_SECTION:
        errorMsg.LoadString(IDS_MEA_UNCLOSED_CDATA_SECTION);
        break;
    case XML_ERROR_EXTERNAL_ENTITY_HANDLING:
        errorMsg.LoadString(IDS_MEA_EXTERNAL_ENTITY_HANDLING);
        break;
    case XML_ERROR_NOT_STANDALONE:
        errorMsg.LoadString(IDS_MEA_NOT_STANDALONE);
        break;
    case XML_ERROR_UNEXPECTED_STATE:
        errorMsg.LoadString(IDS_MEA_UNEXPECTED_STATE);
        break;
    case XML_ERROR_ENTITY_DECLARED_IN_PE:
        errorMsg.LoadString(IDS_MEA_ENTITY_DECLARED_IN_PE);
        break;
    case XML_ERROR_FEATURE_REQUIRES_XML_DTD:
    case XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING:
        return;
    }

    MessageBox(*AfxGetMainWnd(), msg + errorMsg, title, MB_OK | MB_ICONERROR);
}

void MeaXMLParser::HandleValidationError(const ev::ValidationError& error) {
    CString title(reinterpret_cast<LPCSTR>(IDS_MEA_VALIDATION_TITLE));
    CString msg, errorMsg;

    msg.Format(IDS_MEA_VALIDATION_MSG, static_cast<LPCTSTR>(m_handler->GetFilePathname()),
                error.GetLineNumber(), error.GetCharacterPosition() + 1);

    switch (error.GetCode()) {
    case ev::ValidationError::MultipleDocumentElements:
        errorMsg.Format(IDS_MEA_MULTIPLE_DOCUMENT_ELEMENTS, error.GetCurrentElement());
        break;
    case ev::ValidationError::MultipleTopLevelElements:
        errorMsg.Format(IDS_MEA_MULTIPLE_TOP_LEVEL_ELEMENTS, error.GetCurrentElement());
        break;
    case ev::ValidationError::InvalidDocumentElement:
        errorMsg.Format(IDS_MEA_INVALID_DOCUMENT_ELEMENT, error.GetCurrentElement());
        break;
    case ev::ValidationError::UndeclaredElement:
        errorMsg.Format(IDS_MEA_UNDECLARED_ELEMENT, error.GetCurrentElement());
        break;
    case ev::ValidationError::UndeclaredAttribute:
        errorMsg.Format(IDS_MEA_UNDECLARED_ATTRIBUTE, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::InvalidElement:
        errorMsg.Format(IDS_MEA_INVALID_ELEMENT, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::NoCharacterData:
        errorMsg.Format(IDS_MEA_NO_CHARACTER_DATA, error.GetCurrentElement());
        break;
    case ev::ValidationError::UncontainedCharacterData:
        errorMsg.LoadString(IDS_MEA_UNCONTAINED_CHARACTER_DATA);
        break;
    case ev::ValidationError::MismatchedFixedValue:
        errorMsg.Format(IDS_MEA_MISMATCHED_FIXED_VALUE, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::MissingRequiredAttribute:
        errorMsg.Format(IDS_MEA_MISSING_REQUIRED_ATTRIBUTE, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::BadAttributeValue:
        errorMsg.Format(IDS_MEA_BAD_ATTRIBUTE_VALUE, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::DuplicateId:
        errorMsg.Format(IDS_MEA_DUPLICATE_ID, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::UndeclaredNotation:
        errorMsg.Format(IDS_MEA_UNDECLARED_NOTATION, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::UndeclaredEntity:
        errorMsg.Format(IDS_MEA_UNDECLARED_ENTITY, error.GetCurrentElement(), error.GetContainingElement());
        break;
    case ev::ValidationError::IdNotFound:
        errorMsg.Format(IDS_MEA_ID_NOT_FOUND, error.GetCurrentElement());
        break;
    case ev::ValidationError::InvalidElementPattern:
        errorMsg.Format(IDS_MEA_INVALID_ELEMENT_PATTERN, error.GetCurrentElement());
        break;
    }

    MessageBox(*AfxGetMainWnd(), msg + errorMsg, title, MB_OK | MB_ICONERROR);

    throw MeaXMLParserException();
}

CString MeaXMLParser::FromUTF8(const XML_Char* str) {
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
