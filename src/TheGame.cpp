#include <TheGame.h>

// Значение по умолчанию
int TheGame::record = 0;

TheGame::TheGame(
	const COORD &size_board,
	const bool walk_through_walls,
	const int snake_delay,
	const int number_balls,
	const int big_ball_after)
	: snake(size_board, snake_delay),
	score(0), time(std::chrono::milliseconds(0)),
	bonus(size_board, big_ball_after),
	height(false),
	pdt(size_board.Y + 3)
{
	// Инициализируем матрицу, заполнив все элементы пробелами
	board.resize(size_board.X,
				 std::vector<wchar_t>(size_board.Y, L' '));

	// Добавление границ доски по краям
	std::function<void()> create_board = [&]()
	{
		wchar_t side_borders;
		wchar_t top_borders;

		// Установить границ доски в зависимости от прохождения сквозь стены
		if (walk_through_walls)
		{
			side_borders = L'╎';
			top_borders = L'╌';
		}
		else // !walk_through_walls
		{
			side_borders = L'│';
			top_borders = L'─';
		}

		//////////

		board[0][0] = L'┌';

		for (SHORT x = 1; x < size_board.X - 1; ++x)
		{
			board[x][0] = top_borders;
		}

		board[size_board.X - 1][0] = L'┐';

		//////////

		for (SHORT y = 1; y < size_board.Y - 1; ++y)
		{
			board[0][y] = side_borders;

			// Вектор по умолчанию инициализируется пробелами
			/*for (SHORT y = 1; y < size_board.Y - 1; ++y)
			{
				board[x][y] = L' ';
			}*/

			board[size_board.X - 1][y] = side_borders;
		}

		//////////

		board[0][size_board.Y - 1] = L'└';

		for (SHORT x = 1; x < size_board.X - 1; ++x)
		{
			board[x][size_board.Y - 1] = top_borders;
		}

		board[size_board.X - 1][size_board.Y - 1] = L'┘';
	};

	create_board();

	// Устанавливаем шарики на доске
	for (int i = 0; i < number_balls; ++i)
	{
		put_ball();
	}
}

// Начать игру
void TheGame::start()
{
	std::function<void()> pause = [&]() -> void
	{
		show_board(L"ON \b");

		for (;;)
		{
			switch (pdt.input())
			{
			case TEXT('\0'):	break;
			case TEXT('Q'): // Quit
			case TEXT('q'):
			case TEXT('E'): // Exit
			case TEXT('e'):		throw std::runtime_error("StopTheGame");
			default:			return;
			}
		}
	};

	place_snake();
	pause();

	for (;;)
	{
		place_snake();
		show_board(L"OFF");
		time += snake.delay;
		sleep(snake.delay);

		switch (pdt.input())
		{
		case TEXT('W'): // Вперёд
		case TEXT('w'):
			if (snake.direction != Directions::DOWN)
			{
				snake.turn = snake.direction;
				snake.direction = Directions::UP;
			}

			break;
		case TEXT('S'): // Назад
		case TEXT('s'):
			if (snake.direction != Directions::UP)
			{
				snake.turn = snake.direction;
				snake.direction = Directions::DOWN;
			}

			break;
		case TEXT('D'): // Вправо
		case TEXT('d'):
			if (snake.direction != Directions::LEFT)
			{
				snake.turn = snake.direction;
				snake.direction = Directions::RIGHT;
			}

			break;
		case TEXT('A'): // Влево
		case TEXT('a'):
			if (snake.direction != Directions::RIGHT)
			{
				snake.turn = snake.direction;
				snake.direction = Directions::LEFT;
			}

			break;
		case TEXT('P'): // Пауза
		case TEXT('p'):
			pause();
			break;
		}
	}
}

// Кончить игру
void TheGame::to_choke()
{
	// Отобразить анимацию конца игры
	for (int i = 0; i < 5; ++i)
	{
		board[snake.head().X][snake.head().Y] = L'╱';
		show_board(L"END");
		sleep(snake.delay);

		board[snake.head().X][snake.head().Y] = L'╲';
		show_board(L"END");
		sleep(snake.delay);
	}

	board[snake.head().X][snake.head().Y] = L'╳';
	show_board(L"END");
}

// Сохранить счёт игры в файл
bool TheGame::save_score()
{
	std::ofstream file;
	file.open(file_data, std::ios::app | std::ios::binary);

	if (file.is_open())
	{
		file.write(reinterpret_cast<char *>(&time), sizeof(time));
		file.write(reinterpret_cast<char *>(&score), sizeof(score));
		//file.write(reinterpret_cast<char *>(&what), sizeof(what));

		file.close();
		return true;
	}

	return false;
}

// Обновить рекорд из файла игры
bool TheGame::update_record()
{
	std::ifstream file;
	file.open(file_data, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		int temp_score;

		while (
			file.seekg(sizeof(time), std::ios::cur) &&
			file.read(reinterpret_cast<char *>(&temp_score), sizeof(temp_score))
			//file.read(reinterpret_cast<tchar *>(&what), sizeof(what));
			)
		{
			if (temp_score > record)
			{
				record = temp_score;
			}
		}

		file.close();
		return true;
	}

	return false;
}

// Отобразить доску на экране
void TheGame::show_board(const std::wstring &text_pause)
{
	std::wostringstream out;

	if (!pdt.get_use())
	{
		pdt.set_non_use();
	}
	else
	{
		pdt.shift_to_y(pdt.get_start_pos(), 0);
	}

	for (size_t i = 0; i < board.front().size(); ++i)
	{
		for (size_t j = 0; j < board.size(); ++j)
		{
			out << board[j][i];
		}

		out << '\n';
	}

	out << L"Счёт: " << score
		<< L"        Рекорд: " << record
		<< L"        Длинна: " << snake.length()
		<< L"        Пауза: " << text_pause
		<< L"\nВремя: " << to_format_time(time)
		<< L"\nБонус: ";

	// Заполнение статус бара "Бонус"

	int p100 = bonus.start_tick;
	double p5 = p100 / static_cast<double>(100) * 5;
	double p10 = p100 / static_cast<double>(100) * 10;

	// Заполнить статус бар "Бонус" в % соотношении
	for (double i = p10; static_cast<int>(i) <= p100; i += p10)
	{
		if (bonus.ticks_to_extinction >= i)
		{
			out << L'█';
		}
		else if (bonus.ticks_to_extinction >= i - p5)
		{
			out << L'▓';
		}
		else if (bonus.ticks_to_extinction > i - p10)
		{
			out << L'▒';
		}
		else // bonus.ticks_to_end < i - p100 / static_cast<double>(100) * 10
		{
			out << L'░';
		}
	}

	if (bonus.ticks_to_extinction >= 0)
	{
		--bonus.ticks_to_extinction;

		if (bonus.ticks_to_extinction == 0)
		{
			board[bonus.coord.X][bonus.coord.Y] = L' ';
			bonus.count_balls = 0;
		}
	}
	out << L"\n";

	PlatformDependentThings::cwrite(out);
}

// Переместить змейку на координаты и определить её состояние
void TheGame::change_snake(COORD &new_coord_head, const wchar_t head_type)
{
	// Переместить голову в другой нонец доски, при столкновении с проходимой стенкой
	switch (board[new_coord_head.X][new_coord_head.Y])
	{
	case L'╌':
		if (snake.direction == Directions::UP)
		{
			new_coord_head.Y = board.front().size() - 2; // С нуля и - 1
		}
		else // snake.direction == Directions::DOWN
		{
			new_coord_head.Y = 1;
		}

		break;
	case L'╎':
		if (snake.direction == Directions::RIGHT)
		{
			new_coord_head.X = 1;
		}
		else // snake.direction == Directions::LEFT
		{
			new_coord_head.X = board.size() - 2; // С нуля и - 1
		}

		break;
	}

	// Обработать текущие координаты головы змейки
	switch (board[new_coord_head.X][new_coord_head.Y])
	{
	case L'║':
	case L'═':
	case L'╚':
	case L'╝':
	case L'╔':
	case L'╗':
	case L'─':
	case L'│':
		snake.bones.push_front(new_coord_head); // Установить новые координаты головы,
		// для возможности воспроизведения анимации смерти змейки
		throw std::runtime_error("GameOver");
	case L'·':
		// Установить очки если змейка поймала шарик
		score += 1000 / snake.delay.count();

		// is overflow
		if (score < 0)
		{
			throw std::runtime_error("Overflow");
		}

		++bonus.count_balls;
		height = true;
		put_ball();
		break;
	case L'•':
		// Установить очки если змейка поймала шарик
		score *= 2;

		// is overflow
		if (score < 0)
		{
			throw std::runtime_error("Overflow");
		}

		bonus.count_balls = 0;
		bonus.ticks_to_extinction = -1;
		break;
	}

	// Установить голову, если змейка не проиграла.
	snake.bones.push_front(new_coord_head);
	board[snake.head().X][snake.head().Y] = head_type;
}

// Повернуть голову змейке и определить её траекторию движения
void TheGame::place_snake()
{
	// Уменьшить хвост если змейка не собрала обычный шарик прошлым ходом
	if (!height)
	{
		board[snake.tail().X][snake.tail().Y] = L' ';
		snake.bones.pop_back();
	}
	else // height
	{
		height = false;
	}

	COORD new_coord_head = { snake.head().X, snake.head().Y };

	switch (snake.direction)
	{
	case Directions::UP:
		if (snake.turn == Directions::RIGHT)
		{
			board[snake.head().X][snake.head().Y] = L'╝';
			snake.turn = Directions::UP;
		}
		else if (snake.turn == Directions::LEFT)
		{
			board[snake.head().X][snake.head().Y] = L'╚';
			snake.turn = Directions::UP;
		}
		else // snake.turn == Directions::UP
		{
			board[snake.head().X][snake.head().Y] = L'║';
		}

		new_coord_head.Y -= 1;

		change_snake(new_coord_head, L'╫');

		break;
	case Directions::DOWN:
		if (snake.turn == Directions::RIGHT)
		{
			board[snake.head().X][snake.head().Y] = L'╗';
			snake.turn = Directions::DOWN;
		}
		else if (snake.turn == Directions::LEFT)
		{
			board[snake.head().X][snake.head().Y] = L'╔';
			snake.turn = Directions::DOWN;
		}
		else // snake.turn == Directions::DOWN
		{
			board[snake.head().X][snake.head().Y] = L'║';
		}

		new_coord_head.Y += 1;

		change_snake(new_coord_head, L'╫');

		break;
	case Directions::RIGHT:
		if (snake.turn == Directions::DOWN)
		{
			board[snake.head().X][snake.head().Y] = L'╚';
			snake.turn = Directions::RIGHT;
		}
		else if (snake.turn == Directions::UP)
		{
			board[snake.head().X][snake.head().Y] = L'╔';
			snake.turn = Directions::RIGHT;
		}
		else // snake.turn == Directions::RIGHT
		{
			board[snake.head().X][snake.head().Y] = L'═';
		}

		new_coord_head.X += 1;

		change_snake(new_coord_head, L'╪');

		break;
	case Directions::LEFT:
		if (snake.turn == Directions::DOWN)
		{
			board[snake.head().X][snake.head().Y] = L'╝';
			snake.turn = Directions::LEFT;
		}
		else if (snake.turn == Directions::UP)
		{
			board[snake.head().X][snake.head().Y] = L'╗';
			snake.turn = Directions::LEFT;
		}
		else // snake.turn == Directions::LEFT
		{
			board[snake.head().X][snake.head().Y] = L'═';
		}

		new_coord_head.X -= 1;

		change_snake(new_coord_head, L'╪');

		break;
	}
}

// Переместить змейку на координаты и определить её состояние
void TheGame::put_ball()
{
	// Лямбда-выражение находит пустую ячейку и записывает результат в coord
	std::function<bool(COORD &)> find_empty_cell = [&](COORD &coord) -> bool
	{
		// Небольшая оптимизация: мы уменьшаем диапазон генерируемых значений, за счёт границ доски.
		// Так board.size() - 1 && board.front().size() - 1 - всегда границы и
		// координаты 0 - тоже границы.

		std::vector<std::vector<wchar_t>>::iterator it;

		it = std::find_if(board.begin() + 1, board.end() - 1, [](const std::vector<wchar_t> &el)
						  {
							  return (std::find(el.begin() + 1,
												el.end() - 1,
												L' ') != el.end() - 1);
						  });

		if (it != board.end() - 1)
		{
			do
			{
				coord.X = rand() % (board.size() - 2) + 1;
				coord.Y = rand() % (board.front().size() - 2) + 1;
			} while (board[coord.X][coord.Y] != L' ');

			return true;
		}

		return false;
	};

	// Установить шарик
	COORD temp_pos;
	if (find_empty_cell(temp_pos))
	{
		board[temp_pos.X][temp_pos.Y] = L'·';

		// Устноавить большой шарик
		if (bonus.count_balls == bonus.big_ball_after)
		{
			if (find_empty_cell(bonus.coord))
			{
				board[bonus.coord.X][bonus.coord.Y] = L'•';

				bonus.ticks_to_extinction = bonus.start_tick;
			}
		}
	}
}
