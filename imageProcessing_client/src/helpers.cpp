#include "../includes/globals.h"

bool IsValidName(const std::wstring &name)
{
    return !name.empty() && std::all_of(name.begin(), name.end(), [](wchar_t ch)
                                        { return (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z') || ch == L' '; });
}

void UpdateValidation(HWND hwnd, HDC hdcStatic)
{
    GetWindowText(hTextBox1, firstName, 100);
    GetWindowText(hTextBox2, midName, 100);
    GetWindowText(hTextBox3, lastName, 100);

    // Validate first name
    if (IsValidName(firstName))
    {
        isValidFirst = true;
        SetWindowText(hText1, L"First Name");
        SetTextColor(hdcStatic, RGB(0, 0, 0)); // Set text color to black
        SendMessage(hText1, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText1), (LPARAM)CreateSolidBrush(RGB(0, 0, 0)));
    }
    else
    {
        isValidFirst = false;
        SetWindowText(hText1, L"First Name");
        SetTextColor(hdcStatic, RGB(255, 0, 0)); // Set text color to red
        SendMessage(hText1, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText1), (LPARAM)CreateSolidBrush(RGB(255, 0, 0)));
    }

    // Validate middle name
    if (IsValidName(midName))
    {
        isValidMiddle = true;
        SetWindowText(hText2, L"Middle Name");
        SendMessage(hText2, WM_SETTEXT, 0, (LPARAM)L"Middle Name");
        SetTextColor(hdcStatic, RGB(0, 0, 0)); // Set text color to black
        SendMessage(hText2, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText2), (LPARAM)CreateSolidBrush(RGB(0, 0, 0)));
    }
    else
    {
        isValidMiddle = false;
        SetWindowText(hText2, L"Middle Name");
        SetTextColor(hdcStatic, RGB(255, 0, 0)); // Set text color to red
        SendMessage(hText2, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText2), (LPARAM)CreateSolidBrush(RGB(255, 0, 0)));
    }

    // Validate last name
    if (IsValidName(lastName))
    {
        isValidLast = true;
        SetWindowText(hText3, L"Last Name");
        SendMessage(hText3, WM_SETTEXT, 0, (LPARAM)L"Last Name");
        SetTextColor(hdcStatic, RGB(0, 0, 0)); // Set text color to black
        SendMessage(hText3, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText3), (LPARAM)CreateSolidBrush(RGB(0, 0, 0)));
    }
    else
    {
        isValidLast = false;
        SetWindowText(hText3, L"Last Name");
        SetTextColor(hdcStatic, RGB(255, 0, 0)); // Set text color to red
        SendMessage(hText3, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText3), (LPARAM)CreateSolidBrush(RGB(255, 0, 0)));
    }

    // Enable/disable register button based on validation
    bool isValid = IsValidName(firstName) && IsValidName(midName) && IsValidName(lastName);
 
    EnableWindow(hRegisterButton, isValid);
}

void GetBestFitRect(int imageWidth, int imageHeight, int windowWidth, int windowHeight, RECT &destRect)
{
    float imgAspect = (float)imageWidth / imageHeight;
    float winAspect = (float)windowWidth / windowHeight;

    if (imgAspect > winAspect)
    {
        // Fit to width
        destRect.left = 0;
        destRect.right = windowWidth;
        int newHeight = (int)(windowWidth / imgAspect);
        destRect.top = (windowHeight - newHeight) / 2;
        destRect.bottom = destRect.top + newHeight;
    }
    else
    {
        // Fit to height
        destRect.top = 0;
        destRect.bottom = windowHeight;
        int newWidth = (int)(windowHeight * imgAspect);
        destRect.left = (windowWidth - newWidth) / 2;
        destRect.right = destRect.left + newWidth;
    }
}

HBITMAP GetFileName(HWND hwnd)
{

    OPENFILENAME ofn;  // Common dialog box structure
    TCHAR szFile[260]; // Buffer for file name
    HBITMAP hBitMap = NULL;

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("All Files\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = _T("Select a File");
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the dialog box
    if (GetOpenFileName(&ofn) == TRUE)
    {
        hBitMap = (HBITMAP)LoadImage(hInst, szFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (!hBitMap)
        {
            MessageBox(hwnd, TEXT("Failed to load bitmap."), TEXT("Error"), MB_OK | MB_ICONERROR);
            return 0;
        }

        // Keep a copy of the original unfiltered bitmap
        if (hBitmapOriginal)
            DeleteObject(hBitmapOriginal); // free old if any

        hBitmapOriginal = (HBITMAP)CopyImage(hBitMap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

        return hBitMap;
    }
    return hBitMap;
}

void SafeInterfaceRelease(void)
{
    if (pIInversion)
    {
        pIInversion->Release();
        pIInversion = NULL;
    }
    if (pISepia)
    {
        pISepia->Release();
        pISepia = NULL;
    }
    if (pIDesaturation)
    {
        pIDesaturation->Release();
        pIDesaturation = NULL;
    }
    if (pIUserHandler)
    {
        pIUserHandler->Release();
        pIUserHandler = NULL;
    }
}

void DisableItems(HWND hwnd)
{
    EnableWindow(GetDlgItem(hwnd, EDIT_RADIO_BUTTON), FALSE);
    EnableWindow(GetDlgItem(hwnd, DES_PB1), FALSE);
    EnableWindow(GetDlgItem(hwnd, DES_PB2), FALSE);
    EnableWindow(GetDlgItem(hwnd, SEP_PB1), FALSE);
    EnableWindow(GetDlgItem(hwnd, SEP_PB2), FALSE);
    EnableWindow(GetDlgItem(hwnd, INV_PB1), FALSE);
    EnableWindow(GetDlgItem(hwnd, INV_PB2), FALSE);
    EnableWindow(GetDlgItem(hwnd, PICK_RADIO_BUTTON), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_EDIT_R), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_EDIT_G), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_EDIT_B), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_CHECKB_1), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDC_CHECKB_2), FALSE);
    EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
}

void EnableItems(HWND hwnd)
{
    EnableWindow(GetDlgItem(hwnd, EDIT_RADIO_BUTTON), TRUE);
    EnableWindow(GetDlgItem(hwnd, PICK_RADIO_BUTTON), TRUE);
    EnableWindow(GetDlgItem(hwnd, IDOK), TRUE);
}

void EnableDisableColorPicker(HWND hwnd, bool flag)
{
    EnableWindow(GetDlgItem(hwnd, IDC_EDIT_R), flag);
    EnableWindow(GetDlgItem(hwnd, IDC_EDIT_G), flag);
    EnableWindow(GetDlgItem(hwnd, IDC_EDIT_B), flag);
    EnableWindow(GetDlgItem(hwnd, IDC_CHECKB_1), flag);
    EnableWindow(GetDlgItem(hwnd, IDC_CHECKB_2), flag);
}
void EnableDisableImageEditor(HWND hwnd, bool flag)
{
    EnableWindow(GetDlgItem(hwnd, DES_PB1), flag);
    EnableWindow(GetDlgItem(hwnd, DES_PB2), flag);
    EnableWindow(GetDlgItem(hwnd, SEP_PB1), flag);
    EnableWindow(GetDlgItem(hwnd, SEP_PB2), flag);
    EnableWindow(GetDlgItem(hwnd, INV_PB1), flag);
    EnableWindow(GetDlgItem(hwnd, INV_PB2), flag);
}

COLORREF GetPixelFromBitmap(HBITMAP hBitmap, int x, int y)
{
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    SelectObject(hdcMem, hBitmap);

    COLORREF color = GetPixel(hdcMem, x, y); // GDI function

    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    return color;
}

void ProcessBitmap(HBITMAP hBitmap, FilterMode mode, IUnknown *pIUserHandlerTest)
{
    if (!hBitmap || !pIUserHandlerTest)
        return;

    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    int width = bmp.bmWidth;
    int height = bmp.bmHeight;

    BITMAPINFOHEADER bi = {};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height; // top-down DIB
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;

    int rowSize = ((width * 3 + 3) & ~3);
    std::vector<BYTE> pixels(rowSize * height);

    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    SelectObject(hdcMem, hBitmap);

    GetDIBits(hdcMem, hBitmap, 0, height, pixels.data(), (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // Interface pointers
    IDesaturation *pDesat = nullptr;
    ISepia *pSepia = nullptr;
    IInversion *pInv = nullptr;

    // Query the appropriate interface
    switch (mode)
    {
    case FILTER_DESATURATION:
        pIUserHandlerTest->QueryInterface(IID_IDesaturation, (void **)&pDesat);
        break;
    case FILTER_SEPIA:
        pIUserHandlerTest->QueryInterface(IID_ISepia, (void **)&pSepia);
        break;
    case FILTER_INVERSION:
        pIUserHandlerTest->QueryInterface(IID_IInversion, (void **)&pInv);
        break;
    }

    // Apply filter
    for (int y = 0; y < height; y++)
    {
        BYTE *row = &pixels[y * rowSize];
        for (int x = 0; x < width; x++)
        {
            BYTE *pixel = &row[x * 3];
            COLORREF origColor = RGB(pixel[2], pixel[1], pixel[0]);
            COLORREF newColor = origColor;

            switch (mode)
            {
            case FILTER_DESATURATION:
                if (pDesat)
                    pDesat->destauration(origColor, newColor);
                break;
            case FILTER_SEPIA:
                if (pSepia)
                    pSepia->sepia(origColor, newColor);
                break;
            case FILTER_INVERSION:
                if (pInv)
                    pInv->inversion(origColor, newColor);
                break;
            }

            pixel[2] = GetRValue(newColor);
            pixel[1] = GetGValue(newColor);
            pixel[0] = GetBValue(newColor);
        }
    }

    // Update bitmap with processed data
    SetDIBits(hdcMem, hBitmap, 0, height, pixels.data(), (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // Cleanup
    if (pDesat)
        pDesat->Release();
    if (pSepia)
        pSepia->Release();
    if (pInv)
        pInv->Release();
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}

std::string GenerateLogFileName(const wchar_t *firstName, const wchar_t *MiddleName, const wchar_t *lastName, std::string &name, std::string &date)
{
    // Get current date
    std::time_t t = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &t); // Use localtime_r for Linux

    // Build the formatted filename
    std::wstringstream wss;
    wss << firstName << L"_" << lastName << L"_userlogs_"
        << std::setfill(L'0') << std::setw(2) << tm.tm_mday << L"_"
        << std::setfill(L'0') << std::setw(2) << tm.tm_mon + 1 << L"_"
        << tm.tm_year + 1900 << L".txt";

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string fileName = converter.to_bytes(wss.str());

    wss.str(L"");      
    wss << "Name : " <<firstName << " " << MiddleName << " " << lastName << "\n";
    name = converter.to_bytes(wss.str());

    wss.str(L"");  
     wss << "Date : " << std::setfill(L'0') << std::setw(2) << tm.tm_mday << L"_"
        << std::setfill(L'0') << std::setw(2) << tm.tm_mon + 1 << L"_"
        << tm.tm_year + 1900 << "\n";
    date = converter.to_bytes(wss.str()); 

    return fileName;
}
