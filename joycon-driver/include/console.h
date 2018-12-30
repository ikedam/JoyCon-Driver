#pragma once
#include <string>
#include <windows.h>

void setupConsole(LPCTSTR title);
void setConsoleSendCloseTo(HWND hWnd);
void closeConsole();
void hideConsole();
void showConsole();
