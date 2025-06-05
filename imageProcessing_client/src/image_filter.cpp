#include "../includes/globals.h"

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
            // COLORREF origColor = GetPixel(hdcMem, x, y);

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
