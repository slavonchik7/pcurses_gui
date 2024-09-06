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

static HANDLE _wmsgtdh = NULL;

/*
    @return 
        ERR
        OK
*/

static volatile int (*_console_resize_handler) (int /* width */, int /* height */) = NULL;

static int _pdc_resize_screen_internal(int nlines, int ncols)
{
    bool prog_resize = nlines || ncols;

    if (!prog_resize)
    {
        nlines = PDC_get_rows();
        ncols = PDC_get_columns();
    }

    if (nlines < 2 || ncols < 2)
        return ERR;

#if 1
    SetConsoleActiveScreenBuffer(pdc_con_out);

    PDC_flushinp();

    SP->resized = FALSE;
    SP->cursrow = SP->curscol = 0;
#endif
    return OK;
}

static void CALLBACK _console_resize_event(HWINEVENTHOOK hWinEventHook,
                                                  DWORD event,
                                                  HWND hwnd,
                                                  LONG idObject,
                                                  LONG idChild,
                                                  DWORD dwEventThread,
                                                  DWORD dwmsEventTime)
{
    CONSOLE_SCREEN_BUFFER_INFO sb_info;
    int width, height;

    if (!GetConsoleScreenBufferInfo(pdc_con_out, &sb_info))
        return;

    width = sb_info.srWindow.Right - sb_info.srWindow.Left + 1;
    height = sb_info.srWindow.Bottom - sb_info.srWindow.Top + 1;

    if ((((width != getmaxx(stdscr))) || (height != getmaxy(stdscr))) && _console_resize_handler)
        _console_resize_handler(width, height);
}

static unsigned long _check_resize_thread(void *arg)
{
    (void)arg;
    MSG msg;

    if (!SetWinEventHook(EVENT_CONSOLE_LAYOUT,
                            EVENT_CONSOLE_LAYOUT,
                            NULL,
                            _console_resize_event,
                            0,
                            0,
                            WINEVENT_OUTOFCONTEXT))
        printw("Error SetWinEventHook: %d\n", GetLastError());

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

int resize_term_buffer(int nlines, int ncols)
{
    PDC_LOG(("resize_term() - called: nlines %d\n", nlines));

    if (!stdscr || _pdc_resize_screen_internal(nlines, ncols) == ERR)
        return ERR;

    SP->lines = nlines;
    LINES = SP->lines - SP->linesrippedoff - SP->slklines;
    SP->cols = COLS = ncols;

    if (wresize(curscr, SP->lines, SP->cols) == ERR ||
        wresize(stdscr, LINES, COLS) == ERR ||
        wresize(SP->lastscr, SP->lines, SP->cols) == ERR)
        return ERR;

    werase(SP->lastscr);
    curscr->_clear = TRUE;

    if (SP->slk_winptr)
    {
        if (wresize(SP->slk_winptr, SP->slklines, COLS) == ERR)
            return ERR;

        wmove(SP->slk_winptr, 0, 0);
        wclrtobot(SP->slk_winptr);
        PDC_slk_initialize();
        slk_noutrefresh();
    }

    touchwin(stdscr);
    wnoutrefresh(stdscr);

    return OK;  
}

int waitingresize(int (*console_resize_handler) (int /* width */, int /* height */))
{
    if (!_wmsgtdh)
        if ((_wmsgtdh = CreateThread(NULL, 0, _check_resize_thread, NULL, 0, NULL)) == NULL)
            return ERR;

    _console_resize_handler = console_resize_handler;

    return OK;    
}

int getscrsx(void)
{
    return PDC_get_columns();
}

int getscrsy(void)
{
    return PDC_get_rows();
}

int win_resize_disable(void)
{
    HWND hcmd = GetConsoleWindow();
    return SetWindowLongPtr(hcmd, GWL_STYLE, GetWindowLong(hcmd, GWL_STYLE)&~WS_SIZEBOX) == 0 ? ERR : OK;
}

int win_resize_enable(void)
{
    HWND hcmd = GetConsoleWindow();
    return SetWindowLongPtr(hcmd, GWL_STYLE, GetWindowLong(hcmd, GWL_STYLE)|WS_SIZEBOX) == 0 ? ERR : OK;
}
