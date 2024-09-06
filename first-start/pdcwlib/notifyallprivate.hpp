

#ifndef __NOTIFYALLPRIVATE_HPP__
#define __NOTIFYALLPRIVATE_HPP__

#include <QObject>

class NotifyAll
    : public QObject
{
    Q_OBJECT

public:
    explicit inline NotifyAll() : QObject(0) {};

    inline void notifyResize(void* hwnd, int w, int h) { emit notifyAllResize((void*)hwnd, w, h); };
    inline void notifyError(void* hwnd) { emit notifyAllError((void*)hwnd); };

signals:
    void notifyAllResize(void*, int, int);
    void notifyAllError(void*);
};

#endif /* __NOTIFYALLPRIVATE_HPP__ */
