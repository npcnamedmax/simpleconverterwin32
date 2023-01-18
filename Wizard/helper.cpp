#include "helper.h"

void DrawStringBox(HDC hdc, LPCTSTR lpString, int nCount, RECT* lpRect)
{
    // Set the font, text color, and background mode
    HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    // Draw the text
    DrawText(hdc, lpString, nCount, lpRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Clean up
    DeleteObject(hFont);
}
