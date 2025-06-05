#include "../includes/globals.h"

INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR str[] = TEXT("\n\n\tIMAGE EDITOR & COLOR PICKER \n\n\n A Project By : AstroMediComp \n Ketan Sunil Mundhe \n Email ID: ketanmundhe01@gmail.com\n");
    HICON hIcon;
    HBRUSH hBrush;
    switch (message)
    {
    case WM_INITDIALOG:
        hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(KSM_ICON), IMAGE_ICON, 32, 32, LR_SHARED);
        if (hIcon)
        {
            SendDlgItemMessage(hwndDlg, IDC_STATIC_ICON, STM_SETICON, (WPARAM)hIcon, 0);
        }
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwndDlg, IDEXIT);
            return TRUE;
        }
        break;
    case WM_CTLCOLORDLG:
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        return (LRESULT)hBrush;
    case WM_PAINT:
        if (!(hdc = BeginPaint(hwndDlg, &ps)))
        {
            MessageBox(hwndDlg, TEXT("Begin paint failed in About Dlg Proc"), TEXT("ERROR"), MB_ICONERROR);
            return FALSE;
        }
        SetTextColor(hdc, RGB(255, 165, 0));
        SetBkColor(hdc, RGB(0, 0, 0));
        GetClientRect(hwndDlg, &rect);
        DrawText(hdc, str, -1, &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        EndPaint(hwndDlg, &ps);
        return TRUE;
    default:
        return FALSE;
    }
    return FALSE;
}