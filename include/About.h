#pragma once
#include <TheGame.h>
#include <boost/program_options.hpp>

// Файл, хранящий всю статиcтику змейки
std::string	TheGame::file_data = "data.ssave";

class About
{
public:
	static void help()
	{
		/* --size-board -s
		 * --snake-delay -d
		 * --walk-through-walls -w
		 * --number-balls -b
		 * --big-ball-after -B
		 * --use-file-data -f

		 * --help -h
		 * --records -r */

		tostringstream ref;
		ref << TEXT("Использование: Snake [параметры] <аргументы>\n"
					"Змейка - консольная псевдографическая игра.\n"
					"   W, A, S, D - перемещение\n"
					"   P - пауза,\n"
					"   Q или E в режиме паузы - выход\n\n"

					"Параметры:\n"
					"  -h, --help                        Вывод справки по параметрам\n"
					"                                    командной строки.\n"
					"  -r, --records                     Вывод списка рекордов.\n"
					"  -s, --size-board <x> <y>          Установить длину и ширину доски.\n"
					"  -d, --snake-delay <миллисекунды>  Установить задержку перемещения змейки.\n"
					"  -w, --walk-through-walls <on/off> Ограничить перемещение змейки\n"
					"                                    сквозь стены.\n"
					"  -b, --number-balls <количество>   Задаёт одновременное нахождение\n"
					"                                    шариков на доске.\n"
					"  -B, --big-ball-after <количество> Задаёт генерация большого шарика после\n"
					"                                    n-ого обычного.\n\n"
					"  -f, --use-file-data <файл>        Установить файл инструкции для сохранения\n"
					"                                    игрового прогресса.\n\n"

					"Примечания:\n"
					"    <миллисекунды> Ожидает целое число в диапазоне от 1 до ") << INT_MAX << TEXT("\n"
					"                   чем параметр меньше, тем быстрее накапливаются очки.\n"
					"                   Пример: ... --snake-delay=300\n"
					"    <x> <y>        Предполагает 2 значения, от 5, указанные через пробел.\n"
					"                   Пример: ... --size-board=70 20\n"
					"    <on/off>       Логический параметр поддерживает сигнатуры:\n"
					"                   \'on/off\', \'yes/no\', \'1/0\', \'true/false\'\n"
					"                   Пример: ... --walk-through-walls=true\n"
					"    <количество>   Ожидает целое число в диапазонах от 1 до ") << INT_MAX << TEXT("\n"
																																														 "                   для --number-balls и\n"
																																														 "                   от 0 (откл.) до ")
#if defined(WIN32) || defined(_WIN32)
			<< TEXT("%size_board%")
#elif defined(unix) || defined(__unix) || defined(__unix__)
			<< TEXT("$size_board")
#endif // OS
			<< TEXT(" для --big-ball-after\n"
					"                   Пример: ... --number-balls=5 --big-ball-after=0\n"
					"    <файл>         Ожидает путь к файлу, к которому пользователь\n"
					"                   имеет доступ\n"
			);

		PlatformDependentThings::cerror(ref);
	}

	static void records(std::istream &file)
	{
		tostringstream out;

		std::common_type<std::chrono::milliseconds, std::chrono::milliseconds>::type time;
		int score;
		//char what[10];

		out << TEXT("Таблица рекордов: \n");

		for (int i = 1;
			 file.read(reinterpret_cast<char *>(&time), sizeof(time)) &&
			 file.read(reinterpret_cast<char *>(&score), sizeof(score));
			 //file.read(reinterpret_cast<char *>(&what), sizeof(what));
			 ++i)
		{
			out << i << TEXT(". ") << to_format_time(&time)
				<< TEXT(" -> ") << score << /* " :: " << what <<*/ TEXT('\n');
		}

		PlatformDependentThings::cerror(out);
	}
};
