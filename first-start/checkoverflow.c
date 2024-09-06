#if 0
#include "curses.h"
#include <stdlib.h>
#include "curses_addon.h"
#include "curses_addon.c"

int width;
int height;
WINDOW *wmain;

int main()
{
    win_resize_disable();
    wmain = initscr();

    width = getmaxx(wmain);
    height = getmaxy(wmain);
    resize_term_buffer(height * 2, width);

    for (int i = 0; i < height * 2; i++) {
        printw("note number %d\n", i);
        Sleep(500);
        refresh();
    }

    getch();
	endwin();
    return 0;
}
#else
#include <stdio.h>
#include <Windows.h>
#include "curses.h"
#include <stdlib.h>
#include "curses_addon.h"
#include "curses_addon.c"
int main(void)
{
    int ch = 0;

    initscr();
    cbreak();
    keypad(stdscr,TRUE);
    idlok(stdscr,TRUE);
    scrollok(stdscr,TRUE);

    printw("Welcome!");
    refresh();



    while(ch != 'q') {
        ch = getch();
        if(ch == KEY_UP)
        {
            scrl(-1);
        }
        else if (ch == KEY_DOWN)
        {
            scrl(1);
        }
        refresh();
    }

    endwin();
    return 0;

}
#endif