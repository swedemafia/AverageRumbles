#include "PCH.h"

BOOL RunThread = FALSE;

DWORD WINAPI PluginThread(LPVOID Parameter)
{
	unsigned long AverageA, AverageB;
	unsigned long RumbleA, RumbleB;
	unsigned long CountA, CountB;
	unsigned long TotalA, TotalB;
	unsigned long LastA, LastB;
	bool ShowRumble = false;

	// Initialize variables
	AverageA = AverageB = 0;
	CountA = CountB = 0;
	TotalA = TotalB = 0;
	LastA = LastB = 0;

	while (RunThread) {

		if (API.EventPress(Controller::XB1_RT)) {
			// Reset average rumbles and counters
			CountA = CountB = 0;
			LastA = LastB = 0;
			TotalA = TotalB = 0;

			// Prompt user
			DisplayDialog->Timestamp();
			DisplayDialog->InsertFormattedText(GREEN, "Fire button pressed, beginning capture...\r\n");

		}
		else if (API.EventRelease(Controller::XB1_RT)) {

			// Prompt user
			DisplayDialog->Timestamp();
			DisplayDialog->InsertFormattedText(GREEN, "Fire button released, capture complete.\r\n");

		}
		/*else if (API.EventPress(Controller::XB1_X)) {
			// Prompt user
			DisplayDialog->Timestamp();
			DisplayDialog->InsertFormattedText(GREEN, "Reload button pressed!\r\n");
			ShowRumble = true;
		}
		else if (API.EventRelease(Controller::XB1_Y)) {
			// Prompt user
			DisplayDialog->Timestamp();
			DisplayDialog->InsertFormattedText(GREEN, "Weapon button released!\r\n");
			ShowRumble = true;
		}*/
		else if (API.GetInputValue(Controller::XB1_RT)) {
			
			// Add to the total rumble values
			RumbleA = API.GetRumble(Controller::RumbleA);
			RumbleB = API.GetRumble(Controller::RumbleB);
			
			TotalA += RumbleA;
			TotalB += RumbleB;

			// Increment counter
			CountA++;
			CountB++;

			// Get averages
			AverageA = TotalA / CountA;
			AverageB = TotalB / CountB;

			// Output difference
			if ((AverageA != LastA) || (AverageB != LastB)) /* && ShowRumble == true*/ {

				// Prompt user
				DisplayDialog->Timestamp();
				DisplayDialog->InsertFormattedText(YELLOW, "%d:%d (actual: %d:%d)\r\n", AverageA, AverageB, RumbleA, RumbleB);

				//ShowRumble = false;

				// Set last values
				LastA = AverageA;
				LastB = AverageB;
			}

		}

	}

	ExitThread(0);

	return 0;
}

DWORD WINAPI DialogThread(LPVOID Parameter)
{
	MSG Message;

	// Perform window message loop
	while (GetMessage(&Message, NULL, 0, 0)) {

		// Check if window is being terminated
		if (Message.message == WM_QUIT) {
			delete DisplayDialog;
			break;
		}
		else {
			// Dispatch window message
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return 0;
}

BOOL Plugin::Initialize(void)
{
	RunThread = TRUE;

	// Create the main loop that can process controller input/output
	PluginAPI::Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PluginThread, (LPVOID)this, 0, NULL);

	// Confirm the thread was created
	if (PluginAPI::Thread == INVALID_HANDLE_VALUE)
		return FALSE;

	/* Add any initialization code below here (return TRUE if successful, FALSE if not) */
	DisplayDialog = new DisplayDialogManager(LibraryInstance, DIALOG_DISPLAY);

	// Make sure our class object was created
	if (DisplayDialog != nullptr) {

		// Show dialog
		DisplayDialog->MakeDialog();

		// Create window message thread
		MessageThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DialogThread, NULL, 0, 0);

		// Check if window message thread was created
		if (MessageThread == INVALID_HANDLE_VALUE) {
			delete DisplayDialog;
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

void Plugin::Terminate(void)
{
	// Close the PluginThread handle
	if (PluginAPI::Thread != INVALID_HANDLE_VALUE) {
		RunThread = FALSE;
		CloseHandle(PluginAPI::Thread);
	}

	/* Add any termination code here */
	DestroyWindow(DisplayDialog->Handle);
	delete DisplayDialog;
}

// Command Hook
BOOL Plugin::CommandHook(LPCSTR CommandString)
{
	// Process a console command (if it is not an internal command to the host)

	/* Add code here (return TRUE if processed, FALSE if not) */

	return FALSE;
}

// Connection hook
void Plugin::ConnectionHook(Emulator::ConnectionState EventCode)
{
	// Called upon changes in the connection state

	/* Add code here */

	return;
}

// Device Hook
void Plugin::DeviceHook(Emulator::DeviceState EventCode)
{
	// Called when the target device is found or not found to be connected to the system

	/* Add code here */

	return;
}

// Mesasge Hook
void Plugin::MessageHook(BYTE PacketID, WORD PayloadSize, BYTE* Payload)
{
	// Called when a complete packet is received from the device

	/* Add code here */

	return;
}

// Included user function for printing colored & formatted text to console window
// This function does not need to be modified
void Plugin::WriteFormattedOutput(UINT Color, LPCSTR Format, ...)
{
	char Output[512] = "";

	va_list argptr;
	va_start(argptr, Format);
	vsnprintf_s(Output, sizeof(Output), sizeof(Output) - 1, Format, argptr);
	va_end(argptr);

	// Print the formatted string to the console
	API.WriteOutputTimestamp();
	API.WriteOutputString(Color, Output);
}