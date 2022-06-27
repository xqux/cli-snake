# Terminal Snake

**cli-snake** — компактная программа, которая позволяет играть в змейку через командную строку. Интерфейс выполнен с использованием псевдографики. В отличии от большинства похожих проектов, в этом приложении фокус внимания направлен на красивую картину. Змейка меняет положение головы в зависимости от направления, а все элементы графики тщательно избирались, чтобы максимально имитировать GUI.

Из-за того, что программа не нуждается в графическом интерфейсе, она не требовательна к железу. Вкупе с широким функционалом и поддержкой кроссплатформенности это делает ее незаменимым time-killer-ом на любом устройстве.

# Requirements
- [x] OS: ``Windows``, ``Linux``;
- [x] Compilers: ``any C++11 compliant``;
- [x] Libraries: ``Boost.Program_options``;
- [x] CMake: ``3.5+``.

# Install
1. ``git clone https://github.com/xqux/cli-snake``.

# Overview

| By Default                                 | Game Over                                 | Difficult Configuration                                 |
| ------------------------------------------ | ----------------------------------------- | ------------------------------------------------------- |
| ![By Default](/screenshots/overview-1.png) | ![Game Over](/screenshots/overview-2.png) | ![Difficult Configuration](/screenshots/overview-3.png) |

# Manual
> Вы можете вызвать справку для программы.
> Введите в вашем эмуляторе ``$APP_PATH --help`` или ``$APP_PATH -h``.

```
Использование: Snake [параметры] <аргументы>
Змейка - консольная псевдографическая игра.
   W, A, S, D - перемещение
   P - пауза,
   Q или E в режиме паузы - выход

Параметры:
  -h, --help                        Вывод справки по параметрам
                                    командной строки.
  -r, --records                     Вывод списка рекордов.
  -s, --size-board <x> <y>          Установить длину и ширину доски.
  -d, --snake-delay <миллисекунды>  Установить задержку перемещения змейки.
  -w, --walk-through-walls <on/off> Ограничить перемещение змейки
                                    сквозь стены.
  -b, --number-balls <количество>   Задаёт одновременное нахождение
                                    шариков на доске.
  -B, --big-ball-after <количество> Задаёт генерация большого шарика после
                                    n-ого обычного.

  -f, --use-file-data <файл>        Установить файл инструкции для сохранения
                                    игрового прогресса.

Примечания:
    <миллисекунды> Ожидает целое число в диапазоне от 1 до 2147483647
                   чем параметр меньше, тем быстрее накапливаются очки.
                   Пример: ... --snake-delay=300
    <x> <y>        Предполагает 2 значения, от 5, указанные через пробел.
                   Пример: ... --size-board=70 20
    <on/off>       Логический параметр поддерживает сигнатуры:
                   'on/off', 'yes/no', '1/0', 'true/false'
                   Пример: ... --walk-through-walls=true
    <количество>   Ожидает целое число в диапазонах от 1 до 2147483647
                   для --number-balls и
                   от 0 (откл.) до $size_board для --big-ball-after
                   Пример: ... --number-balls=5 --big-ball-after=0
    <файл>         Ожидает путь к файлу, к которому пользователь
                   имеет доступ
```
