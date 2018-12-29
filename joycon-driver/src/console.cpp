#include <cstdio>
#include "console.h"

namespace {
	// Unfortunately, there's no reliable way to the get the main window.
	// And there's no way to set user data to the console window,
	// (Setting GWLP_USERDATA results the console window crash)
	HWND g_hMain;

	// checking the minimized state causes high load,
	// and the don't run the check when it's not shown.
	bool g_shown;

	// Need to watch the window.
	bool g_watch = false;

	void watchConsoleAndHide(HWND hConsole) {
		WINDOWPLACEMENT wp;
		wp.length = sizeof(wp);
		if (!::GetWindowPlacement(hConsole, &wp)) {
			return;
		}
		if (wp.showCmd != SW_SHOWMINIMIZED) {
			return;
		}
		::ShowWindow(hConsole, SW_HIDE);
	}

	DWORD WINAPI watchConsoleAndHideThread(LPVOID) {
		while (g_watch) {
			if (g_shown) {
				HWND hConsole = ::GetConsoleWindow();
				if (hConsole == NULL) {
					return 0;
				}
				watchConsoleAndHide(hConsole);
			}
			::Sleep(500);
		}
		return 0;
	}
} // anonymous namespace

void setupConsole(std::string title) {
	// setup console
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	printf("Debugging Window:\n");

	g_shown = true;

	// Disable close button,
	// as closing the console window results termination of the program.
	// (that cannot be avoided in any way, even with SetConsoleCtrlHandler)
	HWND hConsole = ::GetConsoleWindow();
	if (hConsole == NULL) {
		return;
	}
	::DeleteMenu(::GetSystemMenu(hConsole, false), SC_CLOSE, MF_BYCOMMAND);

	// Watch the window.
	// If it is minimized, hide it.
	g_watch = true;
	::CreateThread(
		NULL,
		0,
		watchConsoleAndHideThread,
		NULL,
		0,
		NULL
	);
}

void closeConsole() {
	g_watch = false;
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
}

BOOL WINAPI consoleCtrlHandler(DWORD dwCtrlType) {
	switch (dwCtrlType) {
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		// controls that cannot be ignored
		// Send a close event to the main window for clean shutdown.
	{
		::SendMessage(g_hMain, WM_CLOSE, 0, 0);
		return TRUE;
	}
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	{
		hideConsole();
		return TRUE;
	}
	}
	return FALSE;
}

void setConsoleSendCloseTo(HWND hWnd) {
	g_hMain = hWnd;
	::SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
}

void hideConsole() {
	HWND hConsole = ::GetConsoleWindow();
	if (hConsole == NULL) {
		return;
	}
	::ShowWindow(hConsole, SW_HIDE);
	g_shown = false;
}

void showConsole() {
	HWND hConsole = ::GetConsoleWindow();
	if (hConsole == NULL) {
		return;
	}
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);
	if (!::GetWindowPlacement(hConsole, &wp)) {
		return;
	}
	if (wp.showCmd == SW_SHOWMINIMIZED) {
		::ShowWindow(hConsole, SW_RESTORE);
	}
	else {
		::ShowWindow(hConsole, SW_SHOW);
	}
	::SetForegroundWindow(hConsole);
	g_shown = true;
}
