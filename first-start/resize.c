#include "curses.h"


/* window resize */
int main(int argc, char *argv[])
{
	int row, col;

	initscr();
    
    resize_term(20, 30);

    getmaxyx(stdscr, row, col);
    getmaxyx(stdscr, row, col);

	getmaxyx(stdscr,row,col);
	for (int i = 0; i < 100; i++)
    printw("Screen size: rows %d, columns %d", row, col);
    getch();
    clear();

	resize_term(100, 100);
    for (int i = 0; i < 100; i++)
    printw("Screen size: rows %d, columns %d", row, col);
	refresh();
	getch();
	endwin();

	return 0;
}