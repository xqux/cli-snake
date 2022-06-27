#pragma once
#include <globals.h>
#include <algorithm>
#include <functional>

#include <list>
#include <vector>

class TheGame
{
public:
	TheGame(
		const COORD &size_board = { 70, 20 },
		const bool walk_through_walls = true,
		const int snake_delay = 300,
		const int number_balls = 1,
		const int big_ball_after = 5);

	void start(); // Начать игру
	void to_choke(); // Кончить игру
	bool save_score(); // Сохранить счёт игры в файл

	// Возвращает текущий счёт
	int get_score() const
	{ return score; }

	// Обновить рекорд из файла игры
	static bool update_record();

	// Возвращает глобальный рекорд всех игр
	static int get_record()
	{ return record; }

	// Возвращает игровое время
	std::common_type < std::chrono::milliseconds,
		std::chrono::milliseconds> ::type get_time() const
	{ return time; }

	static std::string file_data;

private:
	// Отобразить доску на экране
	void show_board(const std::wstring &text_pause);
	// Установить шарик на экране
	void put_ball();
	// Повернуть голову змейке и определить её траекторию движения
	void place_snake();
	// Переместить змейку на координаты и определить её состояние
	void change_snake(COORD &new_coord_head, const wchar_t head_type);

	// Направления
	enum class Directions
	{
		UP = 0,
		DOWN = 1,
		RIGHT = 2,
		LEFT = 3
	};

	// Змейка
	class Snake
	{
	public:
		Snake(const COORD &size_board, const int snake_delay)
			// Устанавливаем задежку отображения доски (по-другому fps или задержку перемещения змейки)
			: delay(std::chrono::milliseconds(snake_delay))
		{
			// Установить координаты головы
			bones.push_front(
				{ size_board.X / 2,
				  size_board.Y / 2 });

			// Установить координаты хвоста
			bones.push_back(
				{ size_board.X / 2,
				  size_board.Y / 2 });

			// Случайно генерируем направление головы
			direction = turn = static_cast<Directions>(rand() % 4);

			// В зависимости от направления смещаем хвост
			switch (direction)
			{
			case Directions::UP:	tail().Y++; break;
			case Directions::DOWN:	tail().Y--; break;
			case Directions::RIGHT:	tail().X--; break;
			case Directions::LEFT:	tail().X++; break;
			}
		}

		// Возвращает ссылку на координаты головы
		COORD &head()
		{
			return bones.front();
		}

		// Возвращает ссылку на координаты основания хвоста
		COORD &tail()
		{
			return bones.back();
		}

		// Возвращает длинну хвоста змейки
		size_t length() const
		{
			return bones.size() - /* head */ 1;
		}

		const std::chrono::milliseconds delay; // Задержка перемещения змейки
		std::list<COORD> bones; // Объект, хранящий координаты змейки
		Directions direction; // Направление головы
		Directions turn; // Бывшее напровление головы (при повороте головы)
	};	Snake snake;

	// Матрица, хранящая все элементы доски
	std::vector<std::vector<wchar_t>> board;

	// Основные поля
	int score; // Текущий счёт
	static int record; // Глобальный рекорд всех игр
	std::common_type<std::chrono::milliseconds,
		std::chrono::milliseconds>::type time; // Продолжительность игры

	struct Bonus
	{
		Bonus(const COORD &size_board, const int big_ball_after)
			: coord({ 0, 0 }),
			count_balls(0),
			big_ball_after(big_ball_after),
			ticks_to_extinction(-1),
			start_tick((size_board.X > size_board.Y)
							? size_board.X
							: size_board.Y)
		{}

		COORD coord; // Координаты большого шарика
		int count_balls; // Сколько змейка собрала обычных шариков
		const int big_ball_after; // Через какое количество count_balls сгенерируется большой шарик
		short ticks_to_extinction; // Сколько осталось тиков до исчезновения большого шарика
		const short start_tick; // Получить начальный тик (от начального до -1)
	};	Bonus bonus;

	// Условия событий
	bool height; // Флаг, который активируется, чтобы хвост, после ловли обычного шарика, не уменьшался
	PlatformDependentThings pdt; // Платформозависимые вещи
};
