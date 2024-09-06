#include "curses.h"
#include <assert.h>
#include <stdint.h>
#include "curspriv.h"
#include <Windows.h>
#include <WinUser.h>
#include <signal.h>
#include "curses_addon.c"


WINDOW *mwin;
extern HANDLE pdc_con_out;

void reinit_scr()
{
    endwin();
    mwin = initscr();
}



#define EVENT_CONSOLE_LAYOUT            0x4005
/* 

    В файле pdcwin.h содержится 'extern HANDLE pdc_con_out;'
        который является дескриптором основного окна, на сколько я понимаю по исходникам

*/

#define FABORTE() \
    do { \
        *((int*)(NULL)) = 0; \
        __builtin_unreachable(); \
    } while (0)


static void CALLBACK uv__tty_console_resize_event(HWINEVENTHOOK hWinEventHook,
                                                  DWORD event,
                                                  HWND hwnd,
                                                  LONG idObject,
                                                  LONG idChild,
                                                  DWORD dwEventThread,
                                                  DWORD dwmsEventTime) {
  CONSOLE_SCREEN_BUFFER_INFO sb_info;
  int width, height;

 if (!GetConsoleScreenBufferInfo(pdc_con_out, &sb_info))
     return;
    width = sb_info.srWindow.Right - sb_info.srWindow.Left + 1;
    height = sb_info.srWindow.Bottom - sb_info.srWindow.Top + 1;

#if 0
  if (width != uv__tty_console_width || height != uv__tty_console_height) {
    uv__tty_console_width = width;
    uv__tty_console_height = height;
    uv__signal_dispatch(SIGWINCH);
  }
#endif

#if  1
    if (((width != getmaxx(mwin)) || (height != getmaxy(mwin)))) {
        //wresize(mwin, width, height);
        //setsyx(width, height);
        //setmaxx(width);
        //setmaxy(height);
        //wresize(mwin, width, height);
        printw("x;%d, y:%d", getscrsx(), getscrsy());
        //resize_term(width, height);
        //resize_term_internal(height, width);
        clear();
        for (int i = 0; i < 200; i++)
            printw("x;%d, y:%d", width, height);
        refresh();

        // MEVENT events;
        // mmask_t mask = getmouse();
        // getmouse()
        // if ((mask & BUTTON1_MOVED)) printw("BUTTON1_MOVED |");
        // if ((mask & BUTTON1_RELEASED)) printw("BUTTON1_RELEASED |");
        // if ((mask & BUTTON1_PRESSED)) printw("BUTTON1_PRESSED |");
        // if ((mask & BUTTON1_CLICKED)) printw("BUTTON1_CLICKED |");
        // if ((mask & BUTTON1_DOUBLE_CLICKED)) printw("BUTTON1_DOUBLE_CLICKED |");
        // if ((mask & BUTTON1_TRIPLE_CLICKED)) printw("BUTTON1_TRIPLE_CLICKED |");
        // if ((mask & BUTTON1_MOVED)) printw("BUTTON1_MOVED");
        // printw("\n");
        //     refresh();
    }
#else
    if (width != getmaxx(mwin) || height != getmaxy(mwin)) {
        reinit_scr();
        mvprintw(width / 2, height / 2,"x;%d, y:%d", width, height);
        refresh();
    }
#endif
}

void check_resize_thread(void)
{
    CONSOLE_SCREEN_BUFFER_INFO sb_info;
    MSG msg;
    if (!GetConsoleScreenBufferInfo(pdc_con_out, &sb_info))
        FABORTE();

    mmask_t old;
    mousemask(ALL_MOUSE_EVENTS, &old);

    if (!SetWinEventHook(EVENT_CONSOLE_LAYOUT,
                            EVENT_CONSOLE_LAYOUT,
                            NULL,
                            uv__tty_console_resize_event,
                            0,
                            0,
                            WINEVENT_OUTOFCONTEXT))
        FABORTE();


    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    printw("end of wait msg");
}

int myresize(int width, int height)
{
    resize_term_buffer(height, width);
    clear();
#if 0
    for (int i = 0; i < 200; i++)
        printw("x;%d, y:%d", width, height);
#endif
    raw();
    keypad(stdscr, TRUE);
    noecho();
    refresh();
}

int main(int argc, char *argv[])
{
	mwin = initscr();
    waitingresize(myresize);
    
#if 0
    while (getch() != 'q') {
        printw("x;%d, y:%d\n", getscrsx(), getscrsy());
        refresh();
    }
#else
    int c = 0;
    while (c != 'q') {
        c = getch();
        attron(A_BOLD);
        printw("%c | %04x\n", c, c);
        attroff(A_BOLD);
        refresh();
    }
#endif
    endwin();
    return 0;
}