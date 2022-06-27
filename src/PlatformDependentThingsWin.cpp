#include <PlatformDependentThings.h>

#if defined(WIN32) || defined(_WIN32)

PlatformDependentThings::PlatformDependentThings(SHORT y_pos)
	: start_y(y_pos), use(false)
{}

PlatformDependentThings::~PlatformDependentThings()
{}

int PlatformDependentThings::input()
{
	return (_kbhit() ? _getwch() : TEXT('\0'));
}

void PlatformDependentThings::cwrite(const std::wostringstream & buf)
{
	WriteConsole(hStdOut, buf.str().c_str(), buf.str().length(), &chs, NULL);
}

void PlatformDependentThings::cerror(const tstring &buf)
{
	WriteConsole(hStdErr, buf.c_str(), buf.length(), &chs, NULL);
}

void PlatformDependentThings::cerror(const tostringstream &buf)
{
	WriteConsole(hStdErr, buf.str().c_str(), buf.str().length(), &chs, NULL);
}

void PlatformDependentThings::shift_to_y(SHORT pos_y, SHORT y)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdOut, &csbi);

	COORD new_pos;
	new_pos.X = 0;

	if (pos_y > y)
	{
		y = pos_y - y;

		new_pos.Y = csbi.dwCursorPosition.Y - y;
	}
	else if (pos_y < y)
	{
		y = y - pos_y;

		new_pos.Y = csbi.dwCursorPosition.Y + y;
	}

	SetConsoleCursorPosition(hStdOut, new_pos);
}

#endif // !WINDOWS
