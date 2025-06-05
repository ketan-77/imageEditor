#include "../includes/globals.h"

FILE *pickedColorFile = NULL;
FILE *pickedNormalizeColorFile = NULL;

INT_PTR CALLBACK EditDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool pickLogEnabled = false;
    static bool pickNormalizeLogEnabled = false;
    static HBRUSH hBrush = NULL;
    static HWND testHWND = NULL;
    HRESULT hr = S_OK;
    std::stringstream colorstr;

    switch (message)
    {
    case WM_INITDIALOG:
        testHWND = (HWND)lParam;
        sharedHwnd = hwndDlg;
        colorTextBox1 = GetDlgItem(hwndDlg, IDC_EDIT_R);
        colorTextBox2 = GetDlgItem(hwndDlg, IDC_EDIT_G);
        colorTextBox3 = GetDlgItem(hwndDlg, IDC_EDIT_B);
        hColorBox = GetDlgItem(hwndDlg, IDC_COLOR_BOX);
        if (!isUserRegisterd)
            DisableItems(sharedHwnd);

        if (!isUserRegisterd)
        {
            SetWindowText(colorTextBox1, L"0");
            SetWindowText(colorTextBox2, L"0");
            SetWindowText(colorTextBox3, L"0");
        }

        SetWindowLong(hColorBox, GWL_STYLE,
                      GetWindowLong(hColorBox, GWL_STYLE) | SS_OWNERDRAW);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDEXIT:
            DestroyWindow(hwndDlg);
            hEditDialog = NULL;
            return TRUE;

        case DES_PB1:
            MessageBox(hwndDlg, TEXT("Applying Desaturation Filter..."), TEXT("Desaturation"), MB_OK);
            ProcessBitmap(hBitmap, FILTER_DESATURATION, pIUserHandler);
            pIUserHandler->addEntryInLogFile(" - > Applying desaturation filter\n");
            InvalidateRect(testHWND, NULL, TRUE);
            break;

        case SEP_PB1:
            MessageBox(hwndDlg, TEXT("Applying Sepia Filter..."), TEXT("Sepia"), MB_OK);
            ProcessBitmap(hBitmap, FILTER_SEPIA, pIUserHandler);
            pIUserHandler->addEntryInLogFile(" - > Applying sepia filter\n");
            InvalidateRect(testHWND, NULL, TRUE);
            break;

        case INV_PB1:
            MessageBox(hwndDlg, TEXT("Applying Inversion Filter..."), TEXT("Inversion"), MB_OK);
            ProcessBitmap(hBitmap, FILTER_INVERSION, pIUserHandler);
            pIUserHandler->addEntryInLogFile(" - > Applying inversion filter\n");
            InvalidateRect(testHWND, NULL, TRUE);
            break;
        case INV_PB2:
        case SEP_PB2:
        case DES_PB2:
            pIUserHandler->addEntryInLogFile(" - > resetting filter\n");
            if (hBitmapOriginal)
            {
                DeleteObject(hBitmap);
                hBitmap = (HBITMAP)CopyImage(hBitmapOriginal, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
                InvalidateRect(testHWND, NULL, TRUE);
                MessageBox(hwndDlg, TEXT("Image reset to original"), TEXT("Reset"), MB_OK);
            }
            else
            {
                MessageBox(hwndDlg, TEXT("No original image to reset to."), TEXT("Reset Error"), MB_OK | MB_ICONERROR);
            }
            break;

        case IDABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(DB_ABOUT), hwndDlg, AboutDlgProc);
            break;

        case REGUSR_PB:
            DialogBox(hInst, MAKEINTRESOURCE(DB_REGISTER), hwndDlg, RegisterDlgProc);
            break;
        case IDC_EDIT_R:
        case IDC_EDIT_G:
        case IDC_EDIT_B:
            if (HIWORD(wParam) == EN_CHANGE)
            {
                TCHAR rStr[8], gStr[8], bStr[8];
                int r = -1, g = -1, b = -1;

                GetWindowText(colorTextBox1, rStr, 8);
                GetWindowText(colorTextBox2, gStr, 8);
                GetWindowText(colorTextBox3, bStr, 8);

                if (_tcslen(rStr) > 0 && _tcslen(gStr) > 0 && _tcslen(bStr) > 0)
                {
                    r = _ttoi(rStr);
                    g = _ttoi(gStr);
                    b = _ttoi(bStr);

                    if ((r >= 0 && r <= 255) && (g >= 0 && g <= 255) && (b >= 0 && b <= 255))
                    {
                        currentPreviewColor = RGB(r, g, b);

                        if (hColorBox)
                        {
                            InvalidateRect(hColorBox, NULL, TRUE);
                            UpdateWindow(hColorBox);
                        }

                        // Logging
                        if (pickLogEnabled)
                        {
                            std::stringstream ss;
                            ss << "\nCurrent Selection : [ R : " << r << ", G : " << g << ", B : " << b << " ]\n";
                            fprintf(pickedColorFile, ss.str().c_str());
                        }

                        if (pickNormalizeLogEnabled)
                        {
                            std::stringstream ss;
                            float r_norm = r / 255.0f;
                            float g_norm = g / 255.0f;
                            float b_norm = b / 255.0f;
                            ss << "\nCurrent Selection[Normalized]: [ R : " << r_norm << ", G : " << g_norm << ", B : " << b_norm << " ]\n";
                            fprintf(pickedNormalizeColorFile, ss.str().c_str());
                        }
                    }
                }
            }
            break;
        case EDIT_RADIO_BUTTON:
            isEditImageEnabled = !isEditImageEnabled;
            SendMessage(GetDlgItem(hwndDlg, EDIT_RADIO_BUTTON),
                        BM_SETCHECK,
                        isEditImageEnabled ? BST_CHECKED : BST_UNCHECKED,
                        0);
            EnableDisableImageEditor(hwndDlg, isEditImageEnabled);
            break;
        case PICK_RADIO_BUTTON:
            isPickColorEnabled = !isPickColorEnabled;
            SendMessage(GetDlgItem(hwndDlg, PICK_RADIO_BUTTON),
                        BM_SETCHECK,
                        isPickColorEnabled ? BST_CHECKED : BST_UNCHECKED,
                        0);
            EnableDisableColorPicker(hwndDlg, isPickColorEnabled);
            break;
        case IDC_CHECKB_1:
            // For picked log files
            pickLogEnabled = !pickLogEnabled;
            SendMessage(GetDlgItem(hwndDlg, IDC_CHECKB_1),
                        BM_SETCHECK,
                        pickLogEnabled ? BST_CHECKED : BST_UNCHECKED,
                        0);

            if (pickLogEnabled)
            {
                pickedColorFile = fopen("PickedLogFile.txt", "w");
            }
            else
            {
                if (pickedColorFile)
                {
                    fclose(pickedColorFile);
                    pickedColorFile = NULL;
                }
                pickLogEnabled = false;
            }
            break;
        case IDC_CHECKB_2:
            pickNormalizeLogEnabled = !pickNormalizeLogEnabled;
            SendMessage(GetDlgItem(hwndDlg, IDC_CHECKB_2),
                        BM_SETCHECK,
                        pickNormalizeLogEnabled ? BST_CHECKED : BST_UNCHECKED,
                        0);

            if (pickNormalizeLogEnabled)
            {
                pickedNormalizeColorFile = fopen("PickedNormalizedLogFile.txt", "w");
            }
            else
            {
                if (pickedNormalizeColorFile)
                {
                    fclose(pickedNormalizeColorFile);
                    pickedNormalizeColorFile = NULL;
                }
                pickNormalizeLogEnabled = false;
            }
            break;
        default:
            return FALSE;
        }
        break;

    case WM_CTLCOLORDLG:
        hBrush = CreateSolidBrush(RGB(135, 206, 235)); // Sky blue background
        return (INT_PTR)hBrush;
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
        return (INT_PTR)GetStockObject(NULL_BRUSH);
    }
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
        if (lpDrawItem->hwndItem == hColorBox)
        {
            HBRUSH hBrush = CreateSolidBrush(currentPreviewColor);
            FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, hBrush);
            DeleteObject(hBrush);
            return TRUE;
        }
        break;
    }
    case WM_DESTROY:
        if (pickedColorFile)
        {
            fclose(pickedColorFile);
            pickedColorFile = NULL;
        }
        if (pickedNormalizeColorFile)
        {
            fclose(pickedNormalizeColorFile);
            pickedNormalizeColorFile = NULL;
        }
        DeleteObject(hBrush);
        return TRUE;

    default:
        return FALSE;
    }

    return FALSE;
}