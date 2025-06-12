﻿#include "pch.h"
#include "KeyboardIndicatorItem.h"
#include "DataManager.h"
#include "KeyboardIndicator.h"
#include <gdiplus.h>

const wchar_t* INDICATOR_CAPS_LOCK = L"Caps";
const wchar_t* INDICATOR_NUM_LOCK = L"Num";
const wchar_t* INDICATOR_SCROLL_LOCK = L"ScrLk";


const wchar_t* CKeyboardIndicatorItem::GetItemName() const
{
    return g_data.StringRes(IDS_PLUGIN_ITEM_NAME);
}

const wchar_t* CKeyboardIndicatorItem::GetItemId() const
{
    return L"3db99u17";
}

const wchar_t* CKeyboardIndicatorItem::GetItemLableText() const
{
    return L"";
}

const wchar_t* CKeyboardIndicatorItem::GetItemValueText() const
{
    return L"";
}

const wchar_t* CKeyboardIndicatorItem::GetItemValueSampleText() const
{
    return L"";
}

bool CKeyboardIndicatorItem::IsCustomDraw() const
{
    return true;
}

int CKeyboardIndicatorItem::GetItemWidthEx(void * hDC) const
{
    //绘图句柄
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //设置字体
    CKeyboardIndicatorItem* pThis = (CKeyboardIndicatorItem*)this;
    pThis->InitFont(pDC);
    CFont* pOldFont = pDC->SelectObject(&pThis->m_font);
    int item_space = g_data.DPI(4) + g_data.DPI(2) - g_data.DPI(1);
    int width = 0;
    if (g_data.m_setting_data.show_caps_lock)
        width += (pDC->GetTextExtent(INDICATOR_CAPS_LOCK).cx + item_space);
    if (g_data.m_setting_data.show_num_lock)
        width += (pDC->GetTextExtent(INDICATOR_NUM_LOCK).cx + item_space);
    if (g_data.m_setting_data.show_scroll_lock)
        width += (pDC->GetTextExtent(INDICATOR_SCROLL_LOCK).cx + item_space);
    //恢复字体
    pDC->SelectObject(pOldFont);
    return width;
}

static void DrawRectOutLine(CDC* pDC, CRect rect, COLORREF color)	//绘制矩形边框
{
    CPen aPen, * pOldPen;
    aPen.CreatePen(PS_SOLID, g_data.DPI(1), color);
    pOldPen = pDC->SelectObject(&aPen);
    CBrush* pOldBrush{ dynamic_cast<CBrush*>(pDC->SelectStockObject(NULL_BRUSH)) };

    pDC->Rectangle(rect);
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);       // Restore the old brush
    aPen.DeleteObject();
}

static void DrawRoundRectOutLine(CDC* pDC, CRect rect, COLORREF color)  //绘制圆角矩形
{
    Gdiplus::Graphics graphics(pDC->GetSafeHdc()); // 创建GDI+ Graphics对象
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias); // 启用抗锯齿

    Gdiplus::Pen pen(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color))); // 创建画笔
    pen.SetWidth(g_data.DPIF(1)); // 设置画笔宽度

    int x = rect.left; // 矩形左上角x坐标
    int y = rect.top; // 矩形左上角y坐标
    int width = rect.Width() - g_data.DPI(1); // 矩形宽度
    int height = rect.Height() - g_data.DPI(1); // 矩形高度
    int cornerRadius = g_data.DPI(3); // 圆角半径

    // 创建圆角矩形路径
    Gdiplus::GraphicsPath path;
    path.AddArc(x, y, cornerRadius * 2, cornerRadius * 2, 180, 90); // 左上角
    path.AddArc(x + width - cornerRadius * 2, y, cornerRadius * 2, cornerRadius * 2, 270, 90); // 右上角
    path.AddArc(x + width - cornerRadius * 2, y + height - cornerRadius * 2, cornerRadius * 2, cornerRadius * 2, 0, 90); // 右下角
    path.AddArc(x, y + height - cornerRadius * 2, cornerRadius * 2, cornerRadius * 2, 90, 90); // 左下角
    path.CloseFigure();

    // 绘制无填充的圆角矩形
    graphics.DrawPath(&pen, &path);
}

static void DrawIndicator(CDC* pDC, CRect& rect, const wchar_t* text, bool dark_mode, bool enable, COLORREF color_ori)
{
    COLORREF color_default;
    COLORREF color_disable;
    if (dark_mode)
    {
        color_default = RGB(255, 255, 255);
        color_disable = RGB(145, 145, 145);
    }
    else
    {
        color_default = RGB(0, 0, 0);
        color_disable = RGB(140, 140, 140);
    }

    COLORREF color_text = enable ? color_ori : color_disable;
    COLORREF color_frame = enable ? color_default : color_disable;
    //根据文本宽度设置矩形的宽度
    rect.right = rect.left + pDC->GetTextExtent(text).cx + g_data.DPI(4);
    //绘制边框
    if (g_data.m_setting_data.draw_round_rect)
        DrawRoundRectOutLine(pDC, rect, color_frame);
    else
        DrawRectOutLine(pDC, rect, color_frame);
    //绘制文本
    pDC->SetTextColor(color_text);
    pDC->DrawText(text, rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOPREFIX);
    //绘制完成后将矩形的左边框移动到右边框的位置
    rect.MoveToX(rect.right + g_data.DPI(2));
}

void CKeyboardIndicatorItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
    //绘图句柄
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    //矩形区域
    CRect rect(CPoint(x, y), CSize(w, h));
    //TrafficMonitor设置的文本颜色
    COLORREF color_ori = pDC->GetTextColor();
    //设置字体
    InitFont(pDC);
    CFont* old_font = pDC->SelectObject(&m_font);
    //设置指示器高度
    int item_height = g_data.DPI(14);
    CRect rect_indicator{ rect };
    rect_indicator.top += (rect.Height() - item_height) / 2;
    rect_indicator.bottom = rect_indicator.top + item_height;
    //绘制Caps Lock
    if (g_data.m_setting_data.show_caps_lock)
        DrawIndicator(pDC, rect_indicator, INDICATOR_CAPS_LOCK, dark_mode, CKeyboardIndicator::IsCapsLockOn(), color_ori);
    //绘制num lock
    if (g_data.m_setting_data.show_num_lock)
        DrawIndicator(pDC, rect_indicator, INDICATOR_NUM_LOCK, dark_mode, CKeyboardIndicator::IsNumLockOn(), color_ori);
    //绘制scroll lock
    if (g_data.m_setting_data.show_scroll_lock)
        DrawIndicator(pDC, rect_indicator, INDICATOR_SCROLL_LOCK, dark_mode, CKeyboardIndicator::IsScrollLockOn(), color_ori);
    //恢复字体
    pDC->SelectObject(old_font);
}

void CKeyboardIndicatorItem::InitFont(CDC* pDC)
{
    if (m_font.GetSafeHandle() == NULL)
        m_font.CreatePointFont(70, _T("Segoe UI"), pDC);
}
