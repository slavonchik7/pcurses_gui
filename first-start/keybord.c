#include "curses.h"
#include <assert.h>
#include <stdint.h>
WINDOW *mwin;

int main()
{

    int ch;
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    while (1) {
        ch = getch();
        if (ch == KEY_F(1)) {
            printw("F1 pressed! Exit!\n");
            break;
        }

        if (ch == KEY_ENTER) {
            printw("ENTER pressed!\n");
        }

        printw("The pressed key is ");
		attron(A_BOLD);
		printw("%c\n", ch);
		attroff(A_BOLD);
        wmove(stdscr, 40, 40);
        refresh();
    }

    endwin();

    return 0;
}