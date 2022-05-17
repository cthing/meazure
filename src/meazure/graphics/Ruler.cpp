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
#include "Ruler.h"
#include "Colors.h"
#include <meazure/ui/Layout.h>
#include <meazure/ui/LayeredWindows.h>


BEGIN_MESSAGE_MAP(MeaRuler, MeaGraphic)
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void MeaRulerCallback::OnRulerMove(const RulerInfo* /* info */) {}


MeaRuler::MeaRuler(const MeaScreenProvider& screenProvider, const MeaUnitsProvider& unitsProvider) :
    MeaGraphic(),
    m_screenProvider(screenProvider),
    m_unitsProvider(unitsProvider),
    m_borderColor(0),
    m_backColor(0),
    m_callback(nullptr),
    m_orientation(Horizontal),
    m_position(0),
    m_labelPosition(Top),
    m_fontHeight(0),
    m_thk(0),
    m_mouseCaptured(false),
    m_opacity(255),
    m_origRulerBitmap(nullptr),
    m_origBackBitmap(nullptr) {
    // Initially hide all position indicators.
    //
    for (int i = 0; i < NumIndicators; i++) {
        ClearIndicator(static_cast<IndicatorId>(i));
    }
}

MeaRuler::~MeaRuler() {}

bool MeaRuler::Create(COLORREF borderColor, COLORREF backColor, BYTE opacity,
                      Orientation orientation, const CRect& targetRect,
                      MeaRulerCallback* callback, const CWnd* parent,
                      UINT id) {
    HCURSOR cursor;

    m_callback = callback;
    m_orientation = orientation;
    m_targetRect = targetRect;

    SetColors(borderColor, backColor);

    FSIZE res = m_screenProvider.GetScreenRes(m_screenProvider.GetScreenIter(targetRect));

    // To ensure the proper sizing of the ruler tick marks and margins,
    // the dimensions are stated in inches and converted to pixels at
    // runtime. This allows the resolution of the screen on which the
    // ruler will be shown to be used to convert to pixels. In case the
    // resolution is such that the pixels sizes are too small, a minimum
    // pixel size is used.
    //
    m_majorTickHeight = m_unitsProvider.ConvertToPixels(MeaInchesId, res, kMajorTickHeight, kMinMajorTickHeight);
    m_minorTickHeight = m_unitsProvider.ConvertToPixels(MeaInchesId, res, kMinorTickHeight, kMinMinorTickHeight);
    m_margin = m_unitsProvider.ConvertToPixels(MeaInchesId, res, kMargin, kMinMargin);

    // Use the appropriate arrow cursor for the ruler orientation.
    //
    if (parent != nullptr) {
        cursor = nullptr;
    } else if (orientation == Horizontal) {
        cursor = LoadCursor(nullptr, IDC_SIZENS);
    } else {
        cursor = LoadCursor(nullptr, IDC_SIZEWE);
    }

    CString wndClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
        cursor);

    if (!MeaGraphic::Create(wndClass, CSize(5, 5), parent, id)) {
        return false;
    }

    // If layered windows are available, set the ruler opacity.
    //
    if (HaveLayeredWindows() && (parent == nullptr)) {
        ModifyStyleEx(0, WS_EX_LAYERED);
        SetOpacity(opacity);
    }

    // Calculate the thickness of the ruler. The ruler thickness is
    // measured in its narrow dimension. The thickness is calculated
    // based on the formula:
    //
    // thickness = major tick height + 2 * margin + label font height
    //
    TEXTMETRIC metrics;
    LOGFONT lf;
    CDC* dc = GetDC();

    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = 80;
    _tcscpy_s(lf.lfFaceName, _countof(lf.lfFaceName), _T("Arial"));
    m_hFont.CreatePointFontIndirect(&lf, dc);
    lf.lfEscapement = 900;
    m_vFont.CreatePointFontIndirect(&lf, dc);

    CFont* origFont = static_cast<CFont*>(dc->SelectObject(&m_hFont));
    dc->GetTextMetrics(&metrics);
    m_fontHeight = metrics.tmHeight;

    dc->SelectObject(origFont);

    m_thk = m_fontHeight;

    if (orientation == Horizontal) {
        m_thk += 2 * m_margin.cy + m_majorTickHeight.cy;

        SetWindowPos(nullptr, 0, 0, targetRect.Width(), m_thk,
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    } else {
        m_thk += 2 * m_margin.cx + m_majorTickHeight.cx;

        SetWindowPos(nullptr, 0, 0, m_thk, targetRect.Height(),
                     SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // If layered windows are available and the ruler is a child window,
    // this means that we need to simulate opacity. This scenario occurs
    // when the ruler is used as a sample on the preference page for
    // setting ruler preferences.
    //
    if (HaveLayeredWindows() && (m_parent != nullptr)) {
        CRect rect;

        GetClientRect(rect);

        m_backDC.CreateCompatibleDC(dc);
        m_backBitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
        m_origBackBitmap = m_backDC.SelectObject(&m_backBitmap);

        m_rulerDC.CreateCompatibleDC(dc);
        m_rulerBitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
        m_origRulerBitmap = m_rulerDC.SelectObject(&m_rulerBitmap);

        MeaLayout::DrawOpacityBackground(*this, m_backDC, m_screenProvider, m_unitsProvider);
    }

    ReleaseDC(dc);

    // Set an initial position for the ruler based on its orientation.
    //
    SetPosition((orientation == Horizontal) ? m_targetRect.top : m_targetRect.left);

    return true;
}

void MeaRuler::OnDestroy() {
    MeaGraphic::OnDestroy();

    m_hFont.DeleteObject();
    m_vFont.DeleteObject();

    if (m_origRulerBitmap != nullptr) {
        m_rulerDC.SelectObject(m_origRulerBitmap);
        m_rulerBitmap.DeleteObject();
        m_rulerDC.DeleteDC();
        m_origRulerBitmap = nullptr;

        m_backDC.SelectObject(m_origBackBitmap);
        m_backBitmap.DeleteObject();
        m_backDC.DeleteDC();
        m_origBackBitmap = nullptr;
    }
}

void MeaRuler::SetColors(COLORREF borderColor, COLORREF backColor) {
    m_borderColor = borderColor;
    m_backColor = backColor;

    Update();
}

void MeaRuler::SetOpacity(BYTE opacity) {
    m_opacity = opacity;

    if (m_hWnd != nullptr) {
        if (m_parent == nullptr) {
            SetLayeredWindowAttributes(*this, 0, opacity, LWA_ALPHA);
        } else {
            Update();
        }
    }
}

void MeaRuler::SetPosition(int position) {
    CPoint targetCenter(m_targetRect.CenterPoint());

    m_position = position;

    LabelPosition origLabelPosition = m_labelPosition;

    // Positioning the ruler includes determining where to place
    // the number labels relative to the tick marks.
    //
    switch (m_orientation) {
    case Horizontal:
        if (m_position < m_targetRect.top) {
            m_position = m_targetRect.top;
        } else if (m_position > (m_targetRect.bottom - m_thk)) {
            m_position = m_targetRect.bottom - m_thk;
        }

        m_labelPosition = (m_position < targetCenter.y) ? Top : Bottom;

        if (GetSafeHwnd() != nullptr) {
            SetWindowPos(nullptr, m_targetRect.left, m_position, 0, 0,
                SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
        }
        break;
    case Vertical:
        if (m_position < m_targetRect.left) {
            m_position = m_targetRect.left;
        } else if (m_position > (m_targetRect.right - m_thk)) {
            m_position = m_targetRect.right - m_thk;
        }

        m_labelPosition = (m_position < targetCenter.x) ? Left : Right;

        if (GetSafeHwnd() != nullptr) {
            SetWindowPos(nullptr, m_position, m_targetRect.top, 0, 0,
                SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
        }
        break;
    }

    if (origLabelPosition != m_labelPosition) {
        Invalidate(FALSE);
    }
}

void MeaRuler::SetIndicator(IndicatorId indId, int pixel) {
    if (GetSafeHwnd() != nullptr) {
        CClientDC dc(this);
        DrawIndicator(indId, dc);
        m_indicatorLoc[indId] = pixel;
        DrawIndicator(indId, dc);
    } else {
        m_indicatorLoc[indId] = pixel;
    }
}

void MeaRuler::ClearIndicator(IndicatorId indId) {
    // The indicators are hidden by moving them to a position
    // that is guaranteed to be off any screen.
    //
    CPoint offScreen = m_screenProvider.GetOffScreen();
    m_indicatorLoc[indId] = min(offScreen.x, offScreen.y);
}

void MeaRuler::DrawIndicator(IndicatorId indId, CDC& dc) {
    // A position indicator is a line that is XOR'd onto
    // the ruler window.
    //
    CPen pen(PS_DOT, 1, m_borderColor);
    CPen* origPen = static_cast<CPen*>(dc.SelectObject(&pen));
    int origROP = dc.SetROP2(R2_XORPEN);

    CRect clientRect;
    GetClientRect(clientRect);

    if (m_orientation == Horizontal) {
        int x = m_indicatorLoc[indId];
        MeaLayout::ScreenToClientX(*this, x);

        dc.MoveTo(x, clientRect.top);
        dc.LineTo(x, clientRect.bottom - 1);
    } else {
        int y = m_indicatorLoc[indId];
        MeaLayout::ScreenToClientY(*this, y);

        dc.MoveTo(clientRect.left, y);
        dc.LineTo(clientRect.right - 1, y);
    }

    dc.SetROP2(origROP);
    dc.SelectObject(origPen);
}

void MeaRuler::DrawRuler(CDC& dc) {
    // One of the major challenges in drawing the ruler is drawing the tick
    // marks in the correct location and with the appropriate appearance. This
    // is because the position of a tick mark might not necessarily land on an
    // integral pixel location. In that case, two tick marks are drawn on the two
    // bounding pixels in a color and brightness that gives the visual appearance
    // of a single tick mark.
    //
    CRect clientRect, winRect;
    GetClientRect(clientRect);
    GetWindowRect(winRect);

    const CRect& virtRect = m_screenProvider.GetVirtualRect();

    // Erase the background
    //
    CBrush backBrush(m_backColor);

    CBrush* pOldBrush = dc.SelectObject(&backBrush);
    dc.PatBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), PATCOPY);
    dc.SelectObject(pOldBrush);

    // Redraw the indicators
    //
    for (int indId = 0; indId < NumIndicators; indId++) {
        DrawIndicator(static_cast<IndicatorId>(indId), dc);
    }

    // Draw tick marks and labels
    // 
    MeaLinearUnits* units = m_unitsProvider.GetLinearUnits();
    int majorTickCount = m_unitsProvider.GetMajorTickCount();
    double p;
    bool isMajorTick, isExact;
    int tick, x, xa, xb, y, ya, yb;
    int tickHeight;

    FSIZE minorIncr = m_unitsProvider.GetMinorIncr(winRect);

    // Determine the blending colors for non-pixel aligned hash mark placement.
    //
    COLORREF hash1 = MeaColors::InterpolateColor(m_borderColor, m_backColor, 40);
    COLORREF hash2 = MeaColors::InterpolateColor(m_borderColor, m_backColor, 70);

    CPen pen(PS_SOLID, 1, m_borderColor);
    CPen pen1(PS_SOLID, 1, hash1);
    CPen pen2(PS_SOLID, 1, hash2);

    dc.SetTextColor(m_borderColor);
    dc.SetBkColor(m_backColor);
    dc.SetTextAlign(TA_CENTER);

    if (m_labelPosition == Left || m_labelPosition == Right) {
        CFont* oldFont = dc.SelectObject(&m_vFont);

        do {
            for (p = 0.0, tick = 0, y = static_cast<int>(units->UnconvertCoord(MeaConvertY, this, p));
                    y >= virtRect.top && y < virtRect.bottom;
                    p += minorIncr.cy, tick++, y = static_cast<int>(units->UnconvertCoord(MeaConvertY, this, p))) {

                if (y >= winRect.top && y < winRect.bottom) {
                    isMajorTick = ((tick % majorTickCount) == 0);
                    tickHeight = isMajorTick ? m_majorTickHeight.cx : m_minorTickHeight.cx;
                    isExact = units->UnconvertCoord(MeaConvertY, this, p, ya, yb);

                    MeaLayout::ScreenToClientY(*this, y);
                    MeaLayout::ScreenToClientY(*this, ya);
                    MeaLayout::ScreenToClientY(*this, yb);

                    CPen* oldPen = dc.SelectObject(isExact ? &pen : &pen1);

                    if (m_labelPosition == Left) {
                        dc.MoveTo(clientRect.right, ya);
                        dc.LineTo(clientRect.right - tickHeight, ya);
                        if (!isExact) {
                            dc.SelectObject(&pen2);
                            dc.MoveTo(clientRect.right, yb);
                            dc.LineTo(clientRect.right - tickHeight, yb);
                        }
                        if (isMajorTick) {
                            dc.TextOut(clientRect.left + m_margin.cx, y, m_unitsProvider.Format(MeaY, p));
                        }
                    } else {
                        dc.MoveTo(clientRect.left, ya);
                        dc.LineTo(clientRect.left + tickHeight, ya);
                        if (!isExact) {
                            dc.SelectObject(&pen2);
                            dc.MoveTo(clientRect.left, yb);
                            dc.LineTo(clientRect.left + tickHeight, yb);
                        }
                        if (isMajorTick) {
                            dc.TextOut(clientRect.left + tickHeight + m_margin.cx, y, m_unitsProvider.Format(MeaY, p));
                        }
                    }

                    dc.SelectObject(oldPen);
                }
            }
            minorIncr.cy = -minorIncr.cy;
        } while (minorIncr.cy < 0);

        dc.SelectObject(oldFont);
    } else {
        CFont* oldFont = dc.SelectObject(&m_hFont);

        do {
            for (p = 0.0, tick = 0, x = static_cast<int>(units->UnconvertCoord(MeaConvertX, this, p));
                    x >= virtRect.left && x < virtRect.right;
                    p += minorIncr.cx, tick++, x = static_cast<int>(units->UnconvertCoord(MeaConvertX, this, p))) {

                if (x >= winRect.left && x < winRect.right) {
                    isMajorTick = ((tick % majorTickCount) == 0);
                    tickHeight = isMajorTick ? m_majorTickHeight.cy : m_minorTickHeight.cy;
                    isExact = units->UnconvertCoord(MeaConvertX, this, p, xa, xb);

                    MeaLayout::ScreenToClientX(*this, x);
                    MeaLayout::ScreenToClientX(*this, xa);
                    MeaLayout::ScreenToClientX(*this, xb);

                    CPen* oldPen = dc.SelectObject(isExact ? &pen : &pen1);

                    if (m_labelPosition == Top) {
                        dc.MoveTo(xa, clientRect.bottom);
                        dc.LineTo(xa, clientRect.bottom - tickHeight);
                        if (!isExact) {
                            dc.SelectObject(&pen2);
                            dc.MoveTo(xb, clientRect.bottom);
                            dc.LineTo(xb, clientRect.bottom - tickHeight);
                        }
                        if (isMajorTick) {
                            dc.TextOut(x, clientRect.top + m_margin.cy, m_unitsProvider.Format(MeaX, p));
                        }
                    } else {
                        dc.MoveTo(xa, clientRect.top);
                        dc.LineTo(xa, clientRect.top + tickHeight);
                        if (!isExact) {
                            dc.SelectObject(&pen2);
                            dc.MoveTo(xb, clientRect.top);
                            dc.LineTo(xb, clientRect.top + tickHeight);
                        }
                        if (isMajorTick) {
                            dc.TextOut(x, clientRect.top + tickHeight + m_margin.cy, m_unitsProvider.Format(MeaX, p));
                        }
                    }

                    dc.SelectObject(oldPen);
                }
            }
            minorIncr.cx = -minorIncr.cx;
        } while (minorIncr.cx < 0);

        dc.SelectObject(oldFont);
    }

    // Draw border
    //
    CBrush borderBrush(m_borderColor);
    dc.FrameRect(clientRect, &borderBrush);
}

void MeaRuler::OnPaint() {
    CPaintDC dc(this);

    // If the ruler is a popup, just draw it. Otherwise it is a child
    // window and needs to be alpha blended with its background. This is
    // the case when layered windows are available and the ruler is being
    // used as a sample on the preferences dialog.
    //
    if (m_origRulerBitmap == nullptr) {
        DrawRuler(dc);
    } else {
        CRect rect;

        GetClientRect(rect);
        DrawRuler(m_rulerDC);
        MeaLayout::AlphaBlend(m_rulerDC, m_backDC, rect.Width(), rect.Height(), m_opacity);
        dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &m_rulerDC, 0, 0, SRCCOPY);
    }
}

void MeaRuler::FillInfo(MeaRulerCallback::RulerInfo& rulerInfo, UINT flags, const CPoint& point) {
    CPoint pt(point - m_pointerOffset);
    ClientToScreen(&pt);
    pt = m_screenProvider.LimitPosition(pt);
    rulerInfo.position = (m_orientation == Vertical) ? pt.x : pt.y;

    rulerInfo.ruler = this;
    rulerInfo.id = m_id;
    rulerInfo.flags = flags;
}

void MeaRuler::OnLButtonDown(UINT /* flags */, CPoint point) {
    m_pointerOffset = point;

    SetCapture();
    m_mouseCaptured = true;
}

void MeaRuler::OnLButtonUp(UINT /* flags */, CPoint /* point */) {
    m_mouseCaptured = false;
    ::ReleaseCapture();
}

void MeaRuler::OnMouseMove(UINT flags, CPoint point) {
    // If the left mouse button is being pressed during the
    // pointer move, send the ruler callback.
    //
    if (m_mouseCaptured) {
        MeaRulerCallback::RulerInfo rulerInfo;
        FillInfo(rulerInfo, flags, point);

        if (m_callback != nullptr) {
            m_callback->OnRulerMove(&rulerInfo);
        }
    }
}
