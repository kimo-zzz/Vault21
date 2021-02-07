#pragma once
#include <Windows.h>

class CConsole {
public:
	void startConsoleWin(int width, int height, char* fname);
	int print(char* fmt, ...);
};
extern CConsole Console;