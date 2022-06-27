#pragma once
#include <globals.h>

#if defined(unix) || defined(__unix) || defined(__unix__)
#include <codecvt>
#include <locale>
#endif // !UNIX

class PlatformDependentThings
{
public:
	PlatformDependentThings(SHORT y_pos);
	~PlatformDependentThings();

	// static void gotoxy(COORD &newpos)

	static int	input(); // Функция считывания клавиш
	static void cwrite(const std::wstring &buf); // Вывод данных на экран
	static void	cwrite(const std::wostringstream &buf); // Вывод данных на экран
	static void	cerror(const tstring &buf); // Вывод данных на экран ошибок
	static void	cerror(const tostringstream &buf); // Вывод данных на экран ошибок

	static void shift_to_y(SHORT pos_y, SHORT y); // Сместиться по y-ку

	SHORT get_start_pos() // Получить положение, отправленное в конструктор
	{ return start_y; }

	bool get_use() // Получить use
	{ return use; }

	bool set_non_use() // Переключить флаг use
	{
		use = !use;
		return use;
	}

private:
#if defined(unix) || defined(__unix) || defined(__unix__)
	struct termios stored_settings;

	// Функция проверки нажатой клавиши
	static int _kbhit()
	{
		static bool inited = false;
		int left;

		if (!inited)
		{
			termios t;
			tcgetattr(0, &t);
			t.c_lflag &= ~ICANON;
			tcsetattr(0, TCSANOW, &t);
			setbuf(stdin, NULL);
			inited = true;
		}

		ioctl(0, FIONREAD, &left);

		return left;
	}

	// Функция считывания клавишы
	static int _getch()
	{
		struct termios oldt, newt;
		int ch;

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

		return ch;
	}
#endif // !UNIX
	SHORT start_y;
	bool use;
};
