#include "curses.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "curses_addon.h"
#include "curses_addon.c"

chtype **cmdFullBuffer; /* хранит все данные буфера консоли 
                         * в виде массивов строк консоли 
                         * каждая следующая строка в массиве это строка в консоли */
int cmdFullWidthIdx;
int cmdFullHeightIdx;
int cmdHeightWidth;

chtype **cmdCurBuffer = NULL; /* хранит указатели на те строки массива cmdFullBuffer, 
                               * которые в данный момент отображаются в консоли */
int cmdCurWidthIdx;
int cmdCurHeightIdx;

int cmdWidth;   /* ширина окна консоли в символах */
int cmdHeight;  /* высота окна консоли в символах */


int cmd_init(int width, int height)
{
    int h;
    cmdCurBuffer = malloc(sizeof(chtype*) * height);
    /* буфер хранит 50 существующих экранов терминала */
    cmdHeightWidth = height * 50;
    cmdFullBuffer = malloc(sizeof(chtype*) * cmdHeightWidth);
    cmdFullHeightIdx = 0;
    cmdFullWidthIdx = 0;
    cmdCurWidthIdx = 0;
    cmdCurHeightIdx = 0;
    cmdWidth = width;
    cmdHeight = height;
    for (h = 0; h < cmdHeightWidth; h++) {
        cmdFullBuffer[h] = malloc(sizeof(chtype) * width);
        memset(cmdFullBuffer[h], 0, width);
    }
}

int cmd_fill_from_buffer(int fromIdx, int n)
{
    while (n--) {
        for (int i = 0; cmdFullBuffer[fromIdx++][i]; i++) {
            addch(cmdFullBuffer[fromIdx][i]);
        }
    }
}

void cmd_buffer_next_line_scrl(void)
{
    if (cmdCurHeightIdx == cmdHeight - 1) {
        scrl(1);
    } else {
        cmdFullHeightIdx++;
        cmdCurHeightIdx++;
    }
    cmdFullWidthIdx = 0;
    cmdCurWidthIdx = 0;
}

void cmd_buffer_next_line_noscrl(void)
{
    if (cmdCurHeightIdx != cmdHeight - 1) {
        cmdFullHeightIdx++;
        cmdCurHeightIdx++;
    }
    cmdFullWidthIdx = 0;
    cmdCurWidthIdx = 0;
}

int cmd_addch(chtype ch)
{
    chtype drawch = 0;
    if (cmdFullWidthIdx == cmdWidth) {
        if (ch == '\n') {
            cmdFullWidthIdx = 0;
            cmdCurWidthIdx = 0;
            if (cmdCurHeightIdx == cmdHeight - 1) {
                addch('\n');
            } else {
                cmdFullHeightIdx++;
                cmdCurHeightIdx++;
            }
            return 0;
        } else {
            cmdFullWidthIdx = 0;
            cmdCurWidthIdx = 0;
            if (cmdCurHeightIdx == cmdHeight - 1) {
                addch('\n');
            } else {
                cmdFullHeightIdx++;
                cmdCurHeightIdx++;
            }
            cmdFullBuffer[cmdFullHeightIdx][cmdFullWidthIdx] = ch;
            drawch = ch;
        }
    } else {
        cmdFullBuffer[cmdFullHeightIdx][cmdFullWidthIdx] = ch;
        drawch = ch;
        if (ch == '\n') {
            cmd_buffer_next_line_noscrl();
        } else {
            cmdCurWidthIdx++;
            cmdFullWidthIdx++;
        }
    }


    addch(drawch);
}

int _cmd_printw(const char *str, va_list args)
{
    chtype buf[2048];
}

int cmd_printw(const char *str, ...)
{
    va_list args;
    va_start(args, str);
    _cmd_printw(str, args);
    va_end(args);
}

int cmd_destroy()
{
    int h;
    for (h = 0; h < cmdHeightWidth; h++)
        free(cmdFullBuffer[h]);
    free(cmdFullBuffer);
    free(cmdCurBuffer);
}

int main()
{
    chtype ch = 0;
    
    WINDOW *wmain = initscr();
    if (!wmain) {
        printf("fuck\n");
        exit(1);
    }
    resize_term(6, 20);
    win_resize_disable();
    cbreak();
    noecho();
    /* реализовать запись в режимах raw_output() и nonl() */
    //nonl();
    //raw_output(TRUE);

    keypad(stdscr,TRUE);
    scrollok(stdscr, TRUE);
    cmd_init(getmaxx(wmain), getmaxy(wmain));

    while(ch != 'q') {
        ch = getch();
        cmd_addch(ch);
#if 0
        if(ch == KEY_UP)
        {
            scrl(-1);
        }
        else if (ch == KEY_DOWN)
        {
            scrl(1);
        }
#endif
        refresh();
    }

    endwin();

    cmd_destroy();
    return 0;
}