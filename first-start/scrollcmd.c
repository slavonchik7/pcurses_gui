#include "curses.h"

#define BUFFER_SIZE 1000 // Максимальный размер буфера
#define WINDOW_HEIGHT 20  // Высота окна консоли
#define WINDOW_WIDTH 50   // Ширина окна консоли

char buffer[BUFFER_SIZE]; // Буфер для хранения данных
int start_line = 0;       // Начальная линия для отображения

void init_buffer() {
    // Инициализация буфера данными
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 'A' + (i % 26); // Заполнение буфера буквами
    }
}

void display_buffer(WINDOW *win) {
    // Отображение данных из буфера в окне
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        if (start_line + i < BUFFER_SIZE) {
            mvwaddch(win, i, 0, buffer[start_line + i]); // Вывод символа
        } else {
            mvwaddch(win, i, 0, ' '); // Пустое место, если данных недостаточно
        }
    }
    wrefresh(win); // Обновление окна
}

int main() {
    initscr(); // Инициализация библиотеки curses
    cbreak();  // Включение режима немедленного ввода
    noecho();  // Отключение отображения вводимых символов

    WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0); // Создание нового окна
    init_buffer(); // Инициализация буфера

    int ch;
    while ((ch = getch()) != 'q') { // Выход при нажатии 'q'
        switch (ch) {
            case KEY_UP: // Прокрутка вверх
                if (start_line > 0) {
                    start_line--;
                }
                break;
            case KEY_DOWN: // Прокрутка вниз
                if (start_line < BUFFER_SIZE - WINDOW_HEIGHT) {
                    start_line++;
                }
                break;
        }
        display_buffer(win); // Отображение буфера
    }

    delwin(win); // Удаление окна
    endwin();    // Завершение работы с curses
    return 0;
}