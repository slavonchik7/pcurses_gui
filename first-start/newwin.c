#include "curses_addon.h"
#include "curses.h"
#include "curspriv.h"
#include <Windows.h>
#include <WinUser.h>
#include <signal.h>

#ifndef EVENT_CONSOLE_LAYOUT
#define EVENT_CONSOLE_LAYOUT            0x4005
#endif /* EVENT_CONSOLE_LAYOUT */

extern HANDLE pdc_con_out;

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(local_win);
	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

/* creating windows */
int main(int argc, char *argv[])
{
	WINDOW *border_win;
	WINDOW *local_win;
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	printw("Hello World !!!");
	attroff(COLOR_PAIR(1));
    mvaddstr(6, 32, "Test");
	refresh();
	border_win = create_newwin(20, 40, 5, 5);
	local_win = create_newwin(20, 40, 5, 5);
	getch();
	wmove(local_win, 1, 1);
	waddch(local_win, 'c');
	//waddstr(local_win, "hello world");
	//wprintw(local_win, "hello world");
	//wmove(local_win, 0, 0);
	wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(local_win);
	//wrefresh(border_win);
	//refresh();
	getch();
	destroy_win(local_win);
	endwin();

	return 0;
}