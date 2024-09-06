#ifndef __PDCW_HPP__
#define __PDCW_HPP__

#include "curses.h"
#include "curspriv.h"

/* PDCurses window library */


class PDCWMainWindow
{
public:
    explicit PDCWMainWindow();
    ~PDCWMainWindow();

private:
    void resize(int height, int width);
};


#endif /* __PDCW_H__ */