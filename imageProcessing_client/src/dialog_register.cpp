#include "../includes/globals.h"

INT_PTR CALLBACK RegisterDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBrush;
    HDC hdcDlg = (HDC)wParam;
    HWND hCtrl = (HWND)lParam;

    switch (message)
    {
    case WM_INITDIALOG:
      
        hTextBox1 = GetDlgItem(hwndDlg, IDC_FIRST_NAME);
        hTextBox2 = GetDlgItem(hwndDlg, IDC_MIDDLE_NAME);
        hTextBox3 = GetDlgItem(hwndDlg, IDC_LAST_NAME);
        hText1 = GetDlgItem(hwndDlg, IDC_STATIC_FN);
        hText2 = GetDlgItem(hwndDlg, IDC_STATIC_MN);
        hText3 = GetDlgItem(hwndDlg, IDC_STATIC_LN);
        hRegisterButton = GetDlgItem(hwndDlg, IDREGISTER);

        if (!isUserRegisterd)
            EnableWindow(hRegisterButton, TRUE);
        else
            EnableWindow(hRegisterButton, FALSE);

 
        SetWindowText(hText1, L"First Name");
        SetWindowText(hText2, L"Middle Name");
        SetWindowText(hText3, L"Last Name");

    
        SendMessage(hText1, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText1), (LPARAM)CreateSolidBrush(RGB(255, 255, 255)));
        SendMessage(hText2, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText2), (LPARAM)CreateSolidBrush(RGB(255, 255, 255)));
        SendMessage(hText3, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hText3), (LPARAM)CreateSolidBrush(RGB(255, 255, 255)));

        return TRUE;

    case WM_COMMAND:
    {
       
        HDC hdcStatic = (HDC)wParam;
        if (LOWORD(wParam) == IDC_FIRST_NAME || LOWORD(wParam) == IDC_MIDDLE_NAME || LOWORD(wParam) == IDC_LAST_NAME)
        {
            UpdateValidation(hwndDlg, hdcStatic);
        }
        else if (LOWORD(wParam) == IDREGISTER)
        {
            std::string name, date;
            isUserRegisterd = true;
            std::string fileName = GenerateLogFileName(firstName, midName, lastName, name, date);
            pIUserHandler->createLogFile(fileName.c_str());
            pIUserHandler->addEntryInLogFile(name.c_str());
            pIUserHandler->addEntryInLogFile(date.c_str());
            pIUserHandler->addEntryInLogFile("------------------------------------------------------\n");

            EnableItems(sharedHwnd);
            EnableWindow(GetDlgItem(sharedHwndWndPrc, IDM_OPEN), TRUE);

            MessageBox(hwndDlg, L"Registration Successful!", L"Success", MB_OK);
            EndDialog(hwndDlg, IDEXIT);
        }
    }
        return TRUE;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        HWND hCtrl = (HWND)lParam;

        SetBkMode(hdcStatic, TRANSPARENT);

        if (hCtrl == hText1)
        {
            SetTextColor(hdcStatic, isValidFirst ? RGB(0, 0, 0) : RGB(255, 0, 0));
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        else if (hCtrl == hText2)
        {
            SetTextColor(hdcStatic, isValidMiddle ? RGB(0, 0, 0) : RGB(255, 0, 0));
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        else if (hCtrl == hText3)
        {
            SetTextColor(hdcStatic, isValidLast ? RGB(0, 0, 0) : RGB(255, 0, 0));
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }

        break;
    }

    default:
        return FALSE;
    }
    return FALSE;
}
