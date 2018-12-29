#pragma once
#include <string>
#include <windows.h>

void setupConsole(std::string title);
void setConsoleSendCloseTo(HWND hWnd);
void hideConsole();
void showConsole();
