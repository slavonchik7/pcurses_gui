#include "curses.h"
#include <assert.h>
#include <stdint.h>
WINDOW *mwin;

#define RSRV_PAIR_NUM 1
#define INIT_PAIR_RSRV(fg, bg) init_pair(RSRV_PAIR_NUM, fg, bg)
/*
    PAIR 256 is reserved for fast printing
*/

typedef union {
    uint32_t    __pad32;
    uint8_t     __pad8;
    uint8_t     n;
    uint8_t     f;
    uint8_t     b;
} clr_pair_u_t;

#define CLRPAIR_MAKE(n, f, b)   ((uint32_t)( (((n) & 0xff) << 16) | (((f) & 0xff) << 8) | ((b) & 0xff) ))
#define CLRPAIR_N(cp)           ((uint8_t)((((uint32_t)(cp)) & 0x00ff0000) >> 16))
#define CLRPAIR_F(cp)           ((uint8_t)((((uint32_t)(cp)) & 0x0000ff00) >> 8))
#define CLRPAIR_B(cp)           ((uint8_t)(((uint32_t)(cp))  & 0x000000ff))

const uint32_t CLRPAIR_MAIN_TEXT   = CLRPAIR_MAKE(0, COLOR_WHITE, COLOR_BLACK);
const uint32_t CLRPAIR_ERROR_MSG   = CLRPAIR_MAKE(1, COLOR_RED, COLOR_BLACK);
const uint32_t CLRPAIR_SUCCESS_MSG = CLRPAIR_MAKE(2, COLOR_GREEN, COLOR_BLACK);
const uint32_t CLRPAIR_MENU_ITEM   = CLRPAIR_MAKE(3, COLOR_BLACK, COLOR_WHITE);
const uint32_t CLRPAIR_MENU_SLCT   = CLRPAIR_MAKE(4, COLOR_WHITE, COLOR_BLACK);
// #define CLRPAIRN_MAIN_TEXT CLRPAIR_MAKE(0, COLOR_WHITE, COLOR_BLACK)
// #define CLRPAIRN_ERROR_MSG CLRPAIR_MAKE(1, COLOR_RED, COLOR_BLACK)
// #define CLRPAIRN_MAIN_TEXT CLRPAIR_MAKE(3, COLOR_WHITE, COLOR_BLACK)


int clrpair_init_all(void)
{
    start_color();
    init_pair(CLRPAIR_N(CLRPAIR_MAIN_TEXT),    CLRPAIR_F(CLRPAIR_MAIN_TEXT),      CLRPAIR_B(CLRPAIR_MAIN_TEXT));
    init_pair(CLRPAIR_N(CLRPAIR_ERROR_MSG),    CLRPAIR_F(CLRPAIR_ERROR_MSG),      CLRPAIR_B(CLRPAIR_ERROR_MSG));
    init_pair(CLRPAIR_N(CLRPAIR_SUCCESS_MSG),  CLRPAIR_F(CLRPAIR_SUCCESS_MSG),    CLRPAIR_B(CLRPAIR_SUCCESS_MSG));
    init_pair(CLRPAIR_N(CLRPAIR_MENU_ITEM),    CLRPAIR_F(CLRPAIR_MENU_ITEM),      CLRPAIR_B(CLRPAIR_MENU_ITEM));
    init_pair(CLRPAIR_N(CLRPAIR_MENU_SLCT),    CLRPAIR_F(CLRPAIR_MENU_SLCT),      CLRPAIR_B(CLRPAIR_MENU_SLCT));
}



int printw_clr(uint32_t clrpair, const char *fmt, ...)
{
    
    /* DO NOT WORK */
    va_list args;
    int retval;
    uint8_t pairn = CLRPAIR_N(clrpair);

    attron(COLOR_PAIR(pairn));
    va_start(args, fmt);
    retval = vwprintw(stdscr, fmt, args);
    va_end(args);
    attroff(COLOR_PAIR(pairn));

    return retval;
}

int main()
{
    mwin = initscr();
    assert(mwin);
    #if 0
    start_color();
    assert(init_pair(1, COLOR_RED, COLOR_WHITE) == OK);
    assert(init_pair(2, COLOR_BLUE, COLOR_WHITE) == OK);
#else
clrpair_init_all();
#endif
    printw_clr(CLRPAIR_MAIN_TEXT, "Hello world!!!\n");
    printw_clr(CLRPAIR_SUCCESS_MSG, "Hello world!!!\n");
    printw_clr(CLRPAIR_ERROR_MSG, "Hello world!!!\n");
    
    printw_clr(CLRPAIR_MENU_ITEM, "item 1\n");
    printw_clr(CLRPAIR_MENU_ITEM, "item 2\n");
    printw_clr(CLRPAIR_MENU_SLCT, "item select\n");
    printw_clr(CLRPAIR_MENU_ITEM, "item 3\n");
    
    refresh();
    getch();
    endwin();
    return 0;
}