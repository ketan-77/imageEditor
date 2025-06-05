#pragma once
#include "globals.h"
#include <windows.h>
#include <string>
enum FilterMode
{
    FILTER_NONE,
    FILTER_DESATURATION,
    FILTER_SEPIA,
    FILTER_INVERSION
};


bool IsValidName(const std::wstring &name);
void UpdateValidation(HWND hwnd, HDC hdcStatic);
void GetBestFitRect(int imageWidth, int imageHeight, int windowWidth, int windowHeight, RECT &destRect);
HBITMAP GetFileName(HWND hwnd);
void SafeInterfaceRelease(void);

void DisableItems(HWND hwnd);
void EnableItems(HWND hwnd);
void EnableDisableColorPicker(HWND hwnd, bool flag);
void EnableDisableImageEditor(HWND hwnd, bool flag);
COLORREF GetPixelFromBitmap(HBITMAP hBitmap, int x, int y);

void ProcessBitmap(HBITMAP hBitmap, FilterMode mode, IUnknown *pIUserHandlerTest);

std::string GenerateLogFileName(const wchar_t *firstName, const wchar_t *MiddleName, const wchar_t *lastName, std::string &name, std::string &date);
