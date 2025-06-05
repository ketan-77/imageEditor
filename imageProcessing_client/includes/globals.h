#pragma once
#include <windows.h>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <cwchar>
#include <sstream>
#include <stdio.h>
#include <tchar.h>
#include <codecvt>
#include <ctime>
#include <iomanip>
#include "window.h"
#include "combinedHeader.h"
#include "helpers.h"

// Shared COM interfaces
extern IUserHandler *pIUserHandler;
extern IDesaturation *pIDesaturation;
extern ISepia *pISepia;
extern IInversion *pIInversion;

// Shared windows/controls
extern HWND hTextBox1, hTextBox2, hTextBox3;
extern HWND hText1, hText2, hText3, hRegisterButton;
extern HWND colorTextBox1, colorTextBox2, colorTextBox3;
extern HWND hEditDialog;
extern HWND hColorBox;
extern HWND sharedHwnd;
extern HWND sharedHwndWndPrc;

// Bitmaps
extern HBITMAP hBitmap;
extern HBITMAP hBitmapOriginal;

// App state
extern HINSTANCE hInst;
extern unsigned int resizeWndWidth, resizeWndHeight;
extern bool isDisable, isValidFirst, isValidMiddle, isValidLast;
extern bool isUserRegisterd, isEditImageEnabled, isPickColorEnabled;
extern COLORREF currentPreviewColor;

// Names
extern wchar_t firstName[100], midName[100], lastName[100];

// Function declarations
INT_PTR CALLBACK EditDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK RegisterDlgProc(HWND, UINT, WPARAM, LPARAM);
