#include "./includes/globals.h"

// Global variable definitions
IUserHandler *pIUserHandler = NULL;
IDesaturation *pIDesaturation = NULL;
ISepia *pISepia = NULL;
IInversion *pIInversion = NULL;

HWND hTextBox1, hTextBox2, hTextBox3;
HWND hText1, hText2, hText3, hRegisterButton;
HWND colorTextBox1, colorTextBox2, colorTextBox3;
HWND hEditDialog = NULL;
HWND hColorBox = NULL;
HWND sharedHwnd;
HWND sharedHwndWndPrc;

HBITMAP hBitmap = NULL;
HBITMAP hBitmapOriginal = NULL;

HINSTANCE hInst;
unsigned int resizeWndWidth = 0, resizeWndHeight = 0;
bool isDisable = true, isValidFirst = false, isValidMiddle = false, isValidLast = false;
bool isUserRegisterd = false, isEditImageEnabled = false, isPickColorEnabled = false;
COLORREF currentPreviewColor = RGB(255, 255, 255);
wchar_t firstName[100], midName[100], lastName[100];
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    // variable declaration
    WNDCLASSEX wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szClassName[] = TEXT("MyWindow_KM");
    HRESULT hr = S_OK;

    // code
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("CoInit Failed"), TEXT("COM ERROR"), MB_ICONERROR);
        exit(0);
    }

    // Initialize window class
    ZeroMemory((void *)&wndclass, sizeof(WNDCLASSEX));
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = szClassName;
    wndclass.lpszMenuName = TEXT("MyMenu");
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.hInstance = hInstance;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(KSM_ICON));
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(KSM_ICON));

    // Register the window class
    RegisterClassEx(&wndclass);

    // Create window in memory
    hwnd = CreateWindow(szClassName,
                        TEXT("KM : Image Editor & Color Picker"),
                        WS_OVERLAPPEDWINDOW,
                        0,
                        0,
                        WINDOW_WIDTH,
                        WINDOW_HEIGHT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    // Show the window on the desktop
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return (int)msg.wParam;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    // Function declaration
    void SafeInterfaceRelease(void);
    static HDC hdc = NULL;
    static HDC hMemDc = NULL;
    static PAINTSTRUCT ps;
    static RECT rect;
    BITMAP bmp;
    TCHAR str[] = TEXT("Click on File Menu and Select 'Open' To open image file");
    static TCHAR fileName[500];
    HRESULT hr = S_OK;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        break;
    case WM_CREATE:
        ZeroMemory((void *)&ps, sizeof(PAINTSTRUCT));
        ZeroMemory((void *)&rect, sizeof(RECT));
        hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
        hr = CoCreateInstance(CLSID_ImageProcessor, NULL, CLSCTX_INPROC_SERVER, IID_IUserHandler, (void **)&pIUserHandler);
        if (FAILED(hr))
        {
            MessageBox(NULL, TEXT("IUserHandler Interface didn't obtained"), TEXT("COM ERROR"), MB_ICONERROR);
            DestroyWindow(hwnd);
        }
        MessageBox(NULL, TEXT("IUserHandler Interface obtained"), TEXT("COM SUCCESS"), MB_ICONINFORMATION);
        EnableWindow(GetDlgItem(hwnd, IDM_OPEN), FALSE);
        sharedHwndWndPrc = hwnd;
        break;
    case WM_PAINT:
    {
        GetClientRect(hwnd, &rect);
        hdc = BeginPaint(hwnd, &ps);
        SetBkColor(hdc, RGB(0, 0, 0));
        SetTextColor(hdc, RGB(0, 255, 0));

        // Show message text in the window center
        DrawText(hdc, str, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        if (hBitmap)
        {
            // Create a memory device context
            hMemDc = CreateCompatibleDC(hdc);
            SelectObject(hMemDc, hBitmap);

            // Get original bitmap dimensions
            BITMAP bmp;
            GetObject(hBitmap, sizeof(BITMAP), &bmp);

            GetBestFitRect(bmp.bmWidth, bmp.bmHeight, resizeWndWidth, resizeWndHeight, rect);

            // Set stretch mode for better quality
            SetStretchBltMode(hdc, COLORONCOLOR);

            // Draw the scaled image
            StretchBlt(
                hdc,
                rect.left, rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                hMemDc,
                0, 0,
                bmp.bmWidth, bmp.bmHeight,
                SRCCOPY);

            // Clean up
            DeleteDC(hMemDc);
        }

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_SIZE:
        resizeWndWidth = LOWORD(lParam);
        resizeWndHeight = HIWORD(lParam);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_OPEN:
            hBitmap = GetFileName(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case IDM_EXIT:
            PostQuitMessage(0);
            break;
        case IDM_EDIT:
            // DialogBox(hInst, MAKEINTRESOURCE(DB_EDIT), hwnd, EditDlgProc);
            hEditDialog = CreateDialog(hInst, MAKEINTRESOURCE(DB_EDIT), hwnd, EditDlgProc);
            ShowWindow(hEditDialog, SW_SHOW);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(DB_ABOUT), hwnd, AboutDlgProc);
            break;
        default:
            break;
        }
        break;
    case WM_LBUTTONDOWN:
    {
        if (!hBitmap)
            break;

        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);

        // Get original bitmap info
        BITMAP bmp;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        // Calculate best fit rectangle again
        RECT imgRect;
        GetBestFitRect(bmp.bmWidth, bmp.bmHeight, resizeWndWidth, resizeWndHeight, imgRect);

        // Check if click is within image bounds
        if (mouseX >= imgRect.left && mouseX < imgRect.right &&
            mouseY >= imgRect.top && mouseY < imgRect.bottom)
        {
            // Translate mouse coordinates to bitmap coordinates
            int imgX = (mouseX - imgRect.left) * bmp.bmWidth / (imgRect.right - imgRect.left);
            int imgY = (mouseY - imgRect.top) * bmp.bmHeight / (imgRect.bottom - imgRect.top);

            // Retrieve pixel RGB from bitmap
            COLORREF color = GetPixelFromBitmap(hBitmap, imgX, imgY);

            int r = GetRValue(color);
            int g = GetGValue(color);
            int b = GetBValue(color);

            if (colorTextBox1 && colorTextBox2 && colorTextBox3)
            {
                TCHAR buf[16];
                wsprintf(buf, TEXT("%d"), r);
                SetWindowText(colorTextBox1, buf);
                wsprintf(buf, TEXT("%d"), g);
                SetWindowText(colorTextBox2, buf);
                wsprintf(buf, TEXT("%d"), b);
                SetWindowText(colorTextBox3, buf);
            }
        }
        break;
    }
    case WM_WINDOWPOSCHANGING:
    {
        WINDOWPOS *pPos = (WINDOWPOS *)lParam;
        // Prevent moving by keeping current position
        if ((pPos->flags & SWP_NOMOVE) == 0)
        {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            pPos->x = rect.left;
            pPos->y = rect.top;
            pPos->flags |= SWP_NOMOVE; // Ensure movement is ignored
        }
        break;
    }

    case WM_DESTROY:
        pIUserHandler->closeLogFile();
        SafeInterfaceRelease();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }
    return 0;
}
