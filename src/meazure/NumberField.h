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

/// @file
/// @brief Header file for the text field control.

#pragma once

#include "Messages.h"


/// Provides a text field control that allows only numeric entries.
/// The text field can be configured to allow any floating point value,
/// only integer values, or only positive integer values.
///
class MeaNumberField : public CEdit
{
public:
    /// Configurations for the text field.
    ///
    enum ValueType {
        AllValues   = 0,    ///< Allows entry of any floating point number.
        IntValues   = 1,    ///< Allows entry of positive or negative integers.
        PosValues   = 2     ///< Allows entry of positive integers.
    };


    /// Constructs a field object. Before the field can be used, the
    /// Create method must be called.
    ///
    MeaNumberField();

    /// Destroys a field object.
    ///
    virtual ~MeaNumberField();


    /// Creates the window for the field. To make the field visible,
    /// add the WS_VISIBLE style or call ShowWindow.
    ///
    /// @param style        [in] Field styles as defined for CEdit.
    /// @param topLeft      [in] Position of the top, left corner for the field, in pixels.
    /// @param numChars     [in] Number of visible characters. For proportional fonts
    ///                     the number of characters actually visible may vary from the
    ///                     specified value.
    /// @param parentWnd    [in] Parent window for the field.
    /// @param id           [in] ID for the field.
    ///
    /// @return <b>true</b> if the field was created successfully.
    ///
    bool Create(DWORD style, const POINT& topLeft, int numChars,
        CWnd* parentWnd, UINT id = 0xFFFF);

    /// Configures the field with the type of value to accept.
    ///
    /// @param valueType    [in] One of the type defined by MeaNumberField::ValueType.
    ///
    void SetValueType(DWORD valueType) { m_valueType = valueType; }


    /// Displays an integer value in the field.
    ///
    /// @param value    [in] Integer value to display in the field.
    ///
    void SetValue(int value) {
        CString str;
        str.Format(_T("%d"), value);
        SetWindowText(str);
    }

    /// Displays a double precision value in the field.
    ///
    /// @param value    [in] Double value to display in the field.
    ///
    void SetValue(double value) {
        CString str;
        str.Format(_T("%f"), value);
        SetWindowText(str);
    }

    /// Obtains the value contained in the text field as an integer.
    ///
    /// @param value    [out] Value contained in the field converted to an integer.
    ///
    /// @return <b>true</b> if the field is not empty.
    ///
    bool GetValue(int& value) const {
        CString str;
        GetWindowText(str);
        if (str.IsEmpty()) {
            return false;
        }
        value = _ttoi(str);
        return true;
    }

    /// Obtains the value contained in the text field as a double precision value.
    ///
    /// @param value    [out] Value contained in the field converted to a double.
    ///
    /// @return <b>true</b> if the field is not empty.
    ///
    bool GetValue(double& value) const {
        CString str;
        GetWindowText(str);
        if (str.IsEmpty()) {
            return false;
        }
        value = _tcstod(str, nullptr);
        return true;
    }


    /// Indicates if the value in the field is selected.
    ///
    /// @return <b>true</b> if the value in the field is selected.
    ///
    bool IsTextSelected() const {
        int start, end;
        GetSel(start, end);
        return (start != end);
    }


    /// Handles the RETURN key, UP and DOWN arrow keys.
    ///
    /// @param msg      [in] Message structure.
    ///
    /// @return TRUE if UP/DOWN and FALSE otherwise.
    ///
    virtual BOOL PreTranslateMessage(MSG* msg) override;

protected:
    DECLARE_MESSAGE_MAP()

    /// Filters out characters not used in a number.
    ///
    /// @param nChar    [in] Character code.
    /// @param nRepCnt  [in] Character repeat count.
    /// @param nFlags   [in] Low level key codes.
    ///
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    /// Ensures only a valid number is pasted.
    ///
    /// @return Always returns TRUE.
    ///
    afx_msg LRESULT OnPaste(WPARAM, LPARAM);

    /// Called when the field loses keyboard focus. The parent
    /// window is sent a MeaFieldFocusMsg message so that it can
    /// take actions such as reading the value in the field.
    ///
    /// @param win      [in] Next window to receive focus.
    /// 
    afx_msg void OnKillFocus(CWnd *win);
    

    /// Indicates if the specified character is an acceptable sign character.
    /// Acceptable sign characters are '+' and '-' for a float and integer
    /// configured field, and only '+' for a positive integer configured field.
    ///
    /// @param ch       [in] Character to test.
    ///
    /// @return <b>true</b> if the specified character represents an acceptable
    ///         sign character.
    ///
    bool IsSign(TCHAR ch) const     { return ((ch == _T('+')) || ((ch == _T('-') && !PosOnly()))); }
    
    /// Indicates if the specified character is an acceptable decimal point.
    /// An acceptable decimal point character is a '.' when the field is
    /// configured to accept floating point values.
    ///
    /// @param ch       [in] Character to test.
    ///
    /// @return <b>true</b> if the specified character represents an acceptable
    ///         decimal point character.
    ///
    bool IsPoint(TCHAR ch) const    { return ((ch == _T('.')) && !IntOnly()); }

private:
    static int m_edgeHeight;        ///< Height of the field's 3D sunken edge


    /// Tests whether the specified string can be placed in the text field.
    ///
    /// @param str      [in] String to test.
    /// @param len      [in] Length of string.
    ///
    /// @return <b>true</b> if the specified string can be placed in the field.
    ///
    bool IsValid(LPCTSTR str, int len) const;


    /// Indicates whether the field has been configured to only accept integer values.
    ///
    /// @return <b>true</b> if the field has been configured to only accept integers.
    ///
    bool IntOnly() const { return (m_valueType & IntValues) != 0; }

    /// Indicates whether the field has been configured to only accept positive integer values.
    ///
    /// @return <b>true</b> if the field has been configured to only accept positive integers.
    ///
    bool PosOnly() const { return (m_valueType & PosValues) != 0; }

    DWORD   m_valueType;        ///< Configuration for the field as defined by MeaNumberField::ValueType.
};
