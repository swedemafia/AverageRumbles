#ifndef DISPLAYDIALOG_H
#define DISPLAYDIALOG_H
#pragma once

class DisplayDialogManager : public DialogManager::DialogHandler
{
public:
	explicit DisplayDialogManager(HINSTANCE Instance, int ResourceID) {
		this->Instance = Instance;
		this->ResourceID = ResourceID;
		this->Handle = NULL;
		this->RichEditOutput = NULL;
	}

	// RichEdit functions:
	void InsertFormattedText(COLORREF Color, const char* Format, ...);
	void Timestamp(void);

	// Dialog objects:
	// 
	// RichEdit:
	HWND RichEditOutput;


private:

	// Message handlers:
	INT_PTR OnCommand(WPARAM wParam, LPARAM lParam);
	INT_PTR OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam);
	INT_PTR OnInitDialog(WPARAM wParam, LPARAM lParam);
	INT_PTR OnSize(WPARAM wParam, LPARAM lParam);

protected:
	// Callback functions:
	INT_PTR CALLBACK HandleMessage(UINT Message, WPARAM wParam, LPARAM lParam);

};

#endif