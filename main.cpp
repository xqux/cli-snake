#include <About.h>

namespace po = boost::program_options;

int tmain(int argc, TCHAR *argv[])
{
	srand(time(NULL));

	// Переменные для конструктора игры
	COORD size_board;
	bool walk_through_walls;
	int snake_delay, number_balls, big_ball_after;

	// Определение параметров командной строки
	po::options_description desc;
	desc.add_options()
		("help,h", "")
		("records,r", "")
		("size-board,s", po::value<std::vector<short>>()->multitoken()
			->default_value(std::vector<short>{ 70, 20 }, "70 25"), "")
		("walk-through-walls,w", po::value<bool>(&walk_through_walls)
			->default_value(true), "")
		("snake-delay,d", po::value<int>(&snake_delay)
			->default_value(300), "")
		("number-balls,b", po::value<int>(&number_balls)
			->default_value(1), "")
		("big-ball-after,B", po::value<int>(&big_ball_after)
			->default_value(5), "")
		("use-file-data,f", po::value<std::string>(&TheGame::file_data)
			->default_value("data.ssave"), "");

	try
	{
#if defined(WIN32) || defined(_WIN32)

		po::wcommand_line_parser parser{ argc, argv };
		parser.options(desc).allow_unregistered().style(
			po::command_line_style::default_style |
			po::command_line_style::allow_slash_for_short);
		po::wparsed_options parsed_options = parser.run();

#else // defined(unix) || defined(__unix) || defined(__unix__)

		po::parsed_options parsed_options = po::parse_command_line(argc, argv, desc);

#endif // OS

		// Установка локали системы
		setlocale(LC_ALL, "");

		po::variables_map vm;
		po::store(parsed_options, vm);
		po::notify(vm);

		if (vm.count("help"))
		{
			About::help();
			return -1;
		}

		std::ifstream file;
		file.open(TheGame::file_data, std::ios::app);

		if (!file.is_open())
		{
			PlatformDependentThings::cerror(TEXT("Не удалось получить доступ к файлу инструкции.\n"));
			return -1;
		}

		if (vm.count("record"))
		{
			About::records(file);
			file.close();
			return -1;
		}

		file.close();

		std::vector<short> temp_v = vm["size-board"].as<std::vector<short>>();

		if (snake_delay > 0 && snake_delay <= 1000
			&& temp_v.size() == 2)
		{
			size_board.X = temp_v.front();
			size_board.Y = temp_v.back();

			if (// Минимально допустимые размеры доски
				size_board.X < 5 || size_board.Y < 5 ||
				// -2 -- верняя и нижняя границы и правые и левые границы
				number_balls > (size_board.X - 2) *(size_board.Y - 2) || number_balls < 0)
			{
				throw std::exception();
			}

			if (big_ball_after == 0)
			{
				big_ball_after = -1;
			}
			else if (big_ball_after < 0)
			{
				throw std::exception();
			}
		}
		else
		{
			throw std::exception();
		}
	}
	catch (...) // Отлавливаем любое исключение
	{
		About::help();
		return -1;
	}

	TheGame sn(size_board,
	           walk_through_walls,
	           snake_delay,
	           number_balls,
	           big_ball_after);

	try
	{
		sn.update_record();
		sn.start();
	}
	catch (const std::exception &ex)
	{
		sn.save_score();
		sn.to_choke();
	}

	return sn.get_score();
}
