#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "stdafx.h"


using namespace std;

// maximum mumber of lines the output console should have

static const WORD MAX_CONSOLE_LINES = 500;

FILE* __fStdOut = NULL;
HANDLE __hStdOut = NULL;
// width and height is the size of console window, if you specify fname, 
// the output will also be writton to thisPtr file. The file pointer is automatically closed 
// when you close the app

void CConsole::startConsoleWin(int width, int height, char* fname)
{
	AllocConsole();
	SetConsoleTitle("Debug Window");
	__hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD co = { width,height };
	SetConsoleScreenBufferSize(__hStdOut, co);
	if (fname)
		__fStdOut = fopen(fname, "w");
}
// Use print like TRACE0, TRACE1, ... (The arguments are the same as print)
int CConsole::print(char* fmt, ...)
{
	char s[3000];
	va_list argptr;
	int cnt;
	va_start(argptr, fmt);
	cnt = vsprintf(s, fmt, argptr);
	va_end(argptr);
	DWORD cCharsWritten;
	if (__hStdOut)
		WriteConsole(__hStdOut, s, strlen(s), &cCharsWritten, NULL);
	if (__fStdOut)
		fprintf(__fStdOut, s);
	return(cnt);
}
