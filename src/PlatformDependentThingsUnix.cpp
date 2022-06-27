#include <PlatformDependentThings.h>

#if defined(unix) || defined(__unix) || defined(__unix__)

PlatformDependentThings::PlatformDependentThings(const SHORT y_pos)
	: start_y(y_pos), use(false)
{
	// Перехват клавиатуры (для Linux), чтобы набранные пользователем
	// клавиши не отображались в консоли
	struct termios new_settings;

	tcgetattr(0, &stored_settings);

	new_settings = stored_settings;

	new_settings.c_lflag &= (~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;

	tcsetattr(0, TCSANOW, &new_settings);
}

PlatformDependentThings::~PlatformDependentThings()
{
	// Возобнавление управления клавиатуры
	tcsetattr(0, TCSANOW, &stored_settings);
}

int PlatformDependentThings::input()
{
	return (_kbhit() ? _getch() : TEXT('\0'));
}

void PlatformDependentThings::cwrite(const std::wstring &buf)
{
	write(STDOUT_FILENO, buf.c_str(), buf.length());
}

void PlatformDependentThings::cwrite(const std::wostringstream &buf)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	std::string str_buf = conv.to_bytes(buf.str());

	write(STDOUT_FILENO, str_buf.c_str(), str_buf.length());
}

void PlatformDependentThings::cerror(const tstring &buf)
{
	write(STDERR_FILENO, buf.c_str(), buf.length());
}

void PlatformDependentThings::cerror(const tostringstream &buf)
{
	write(STDERR_FILENO, buf.str().c_str(), buf.str().length());
}

void PlatformDependentThings::shift_to_y(SHORT pos_y, SHORT y)
{
	std::stringstream go;

	if (pos_y > y)
	{
		go << "\033["
			<< pos_y - y
			<< "A";
	}
	else if (pos_y < y)
	{
		go << "\033["
			<< y - pos_y
			<< "B";
	}

	write(STDOUT_FILENO, go.str().c_str(), go.str().length());
}

#endif //!UNIX
