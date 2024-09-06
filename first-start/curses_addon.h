#ifndef __CURSES_ADDON__
#define __CURSES_ADDON__

int resize_term_buffer(int nlines, int ncols);
int waitingresize(int (*console_resize_handler) (int /* width */, int /* height */));
int getscrsx(void);
int getscrsy(void);

int win_resize_disable(void);
int win_resize_enable(void);

#endif /* __CURSES_ADDON__ */