#include "pch.h"

INT_PTR CALLBACK DisplayDialogManager::HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam)
{
    // Dispatch message handler
    switch (Message) {
    case WM_COMMAND:
        return OnCommand(wParam, lParam);
    case WM_GETMINMAXINFO:
        return OnGetMinMaxInfo(wParam, lParam);
    case WM_INITDIALOG:
        return OnInitDialog(wParam, lParam);
    case WM_SIZE:
        return OnSize(wParam, lParam);
    }

    return FALSE;
}

INT_PTR DisplayDialogManager::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Determine which dialog object was clicked
    switch (LOWORD(wParam)) {
    case MENU_DISPLAY_CLEAR_OUTPUT:
            
        // Clear RichEdit window
        SetWindowText(this->RichEditOutput, NULL);

        break;
    }

    return FALSE;
}

INT_PTR DisplayDialogManager::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam)
{
    MINMAXINFO* pmmi = reinterpret_cast<MINMAXINFO*>(lParam);

    // Set the minimum size of the dialog
    pmmi->ptMinTrackSize.x = 400;  // Minimum width
    pmmi->ptMinTrackSize.y = 300;  // Minimum height

    return FALSE;
}

INT_PTR DisplayDialogManager::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
    RECT DialogRect;

    // Get window rect
    GetWindowRect(this->Handle, &DialogRect);

    // Determine X/Y coordinates
    int DialogX = (GetSystemMetrics(SM_CXSCREEN) - (DialogRect.right - DialogRect.left)) >> 1;
    int DialogY = (GetSystemMetrics(SM_CYSCREEN) - (DialogRect.bottom - DialogRect.top)) >> 1;

    // Set dialog position
    SetWindowPos(this->Handle, NULL, DialogX, DialogY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    // Get dialog object handles
    this->RichEditOutput = GetDlgItem(this->Handle, RICHEDIT_OUTPUT);

    // Prepare RichEdit output
    SendMessage(this->RichEditOutput, EM_SETBKGNDCOLOR, FALSE, BLACK);

    // Set paragraph formatting
    PARAFORMAT2 pf;
    pf.cbSize = sizeof(PARAFORMAT2);
    pf.dwMask = PFM_OFFSET;
    pf.dxOffset = 1440;
    SendMessage(this->RichEditOutput, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

    // Set character formatting
    CHARFORMAT2A cf;
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_SIZE | CFM_FACE;
    cf.yHeight = 210;
    strcpy_s(cf.szFaceName, "Microsoft Sans Serif");
    SendMessageA(this->RichEditOutput, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);

    // Show window
    ShowWindow(this->Handle, SW_SHOW);

	return FALSE;
}

INT_PTR DisplayDialogManager::OnSize(WPARAM wParam, LPARAM lParam)
{
    unsigned int DialogWidth = LOWORD(lParam) - 20;
    unsigned int DialogHeight = HIWORD(lParam) - 20;

    // Resize RichEdit control
    MoveWindow(this->RichEditOutput, 10, 10, DialogWidth, DialogHeight, TRUE);

    return FALSE;
}

void DisplayDialogManager::InsertFormattedText(COLORREF Color, const char* Format, ...)
{
    CHARFORMAT2 CharFormat;
	char Output[512] = "";
	va_list argptr;

    // Build formatted text
	va_start(argptr, Format);
	vsnprintf_s(Output, sizeof(Output), sizeof(Output) - 1, Format, argptr);
	va_end(argptr);

    // Prepare character formatting structure
    memset(&CharFormat, 0, sizeof(CHARFORMAT2));

    // Set character formatting attributes
    CharFormat.cbSize = sizeof(CHARFORMAT2);
    CharFormat.dwMask = CFM_COLOR;
    CharFormat.dwEffects = 0;
    CharFormat.crTextColor = Color;
    CharFormat.bCharSet = DEFAULT_CHARSET;
    CharFormat.bPitchAndFamily = DEFAULT_PITCH;

    // Set the this attributes for the existing text
    SendMessage(this->RichEditOutput, EM_SETSEL, -1, -1);
    SendMessage(this->RichEditOutput, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&CharFormat);

    // Append the text
    SendMessageA(this->RichEditOutput, EM_REPLACESEL, FALSE, (LPARAM)Output); // Non-unicode output
    SendMessage(this->RichEditOutput, EM_SETSEL, -1, -1);
    SendMessage(this->RichEditOutput, EM_SCROLL, SB_BOTTOM, 0);
}

void DisplayDialogManager::Timestamp(void)
{
    SYSTEMTIME SystemTime;
    GetLocalTime(&SystemTime);
    InsertFormattedText(WHITE, "[%02i:%02i:%02i.%03i] ", SystemTime.wHour % 12, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
}