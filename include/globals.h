#pragma once
#include <thread>  // chrono, ctime
#include <sstream> // string
#include <fstream>

#if defined(WIN32) || defined(_WIN32)

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Windows.h>
#include <conio.h>

// Глобальные константы стандартных потоков
static CONST HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
static CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
static CONST HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);

// Глобальные переменные
static DWORD chs = 0;

// Глобальные директивы препроцессора
#define tmain wmain

// Глобальные псевдонимы
typedef std::wstring tstring;
typedef std::wostringstream tostringstream;

#elif defined(unix) || defined(__unix) || defined(__unix__)

#include <unistd.h>
#include <termio.h>

// Глобальные дерективы препроцессора
#define tmain main
//#define gotoxy(x, y) SetConsoleCursorPosition(hStdOut, COORD{ x, y })
#define TEXT(quote) quote

// Глобальные псевдонимы
typedef std::string tstring;
typedef std::ostringstream tostringstream;
typedef char TCHAR;
typedef int SHORT;

typedef struct _COORD
{
	SHORT X;
	SHORT Y;
} COORD, *PCOORD;

#endif // OS

#include "PlatformDependentThings.h"

// Деректива усыпления процесса
#define sleep(msec) std::this_thread::sleep_for(msec);

// Функция форматирования времени
static std::wstring to_format_time(std::common_type<std::chrono::milliseconds,
	std::chrono::milliseconds>::type &tp)
{
	std::wostringstream format;

	int hours = std::chrono::duration_cast<std::chrono::hours>(tp).count() % 24;
	int minutes = std::chrono::duration_cast<std::chrono::minutes>(tp).count() % 60;
	int seconds = std::chrono::duration_cast<std::chrono::seconds>(tp).count() % 60;

	if (hours < 10)
	{
		format << TEXT('0');
	}

	format << hours << TEXT(':');

	if (minutes < 10)
	{
		format << TEXT('0');
	}

	format << minutes << TEXT(':');

	if (seconds < 10)
	{
		format << TEXT('0');
	}

	format << seconds;

	return format.str();
}

static tstring to_format_time(std::common_type<std::chrono::milliseconds,
	std::chrono::milliseconds>::type *tp)
{
	tostringstream format;

	int hours = std::chrono::duration_cast<std::chrono::hours>(*tp).count() % 24;
	int minutes = std::chrono::duration_cast<std::chrono::minutes>(*tp).count() % 60;
	int seconds = std::chrono::duration_cast<std::chrono::seconds>(*tp).count() % 60;

	if (hours < 10)
	{
		format << TEXT('0');
	}

	format << hours << TEXT(':');

	if (minutes < 10)
	{
		format << TEXT('0');
	}

	format << minutes << TEXT(':');

	if (seconds < 10)
	{
		format << TEXT('0');
	}

	format << seconds;

	return format.str();
}
