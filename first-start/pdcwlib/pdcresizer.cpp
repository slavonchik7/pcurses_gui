
#include <Windows.h>
#include <WinUser.h>
#include <QThread>
#include <QDebug>
#include "notifyallprivate.hpp"
#include "pdcresizer.hpp"
#ifndef EVENT_CONSOLE_LAYOUT
#define EVENT_CONSOLE_LAYOUT            0x4005
#endif /* EVENT_CONSOLE_LAYOUT */

#define WM_MY_TERM_THREAD (WM_USER + 2)


static NotifyAll __notifyAll;

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

    if (!GetConsoleScreenBufferInfo((HANDLE)hwnd, &sb_info)) {
        __notifyAll.notifyError((HANDLE)hwnd);
        return;
    }

    width = sb_info.srWindow.Right - sb_info.srWindow.Left + 1;
    height = sb_info.srWindow.Bottom - sb_info.srWindow.Top + 1;

    __notifyAll.notifyResize((HANDLE)hwnd, width, height);
}

PDCResizeChecker::PDCResizeChecker()
    :   QObject(0)
{
    m_thread = 0;
    m_handle = 0;
    m_thIdReady = false;
}

PDCResizeChecker::~PDCResizeChecker()
{
    if (m_thread) {
        m_thread->quit();
        /* wake up GetMessage */
        PostThreadMessage(GetThreadId(m_thHandle), WM_MY_TERM_THREAD, 0, 0);
    }
}

void PDCResizeChecker::start(void *handle)
{
    m_handle = handle;
    m_thread = new QThread;
    moveToThread(m_thread);

    connect(&__notifyAll, &NotifyAll::notifyAllResize,
            this, &PDCResizeChecker::notifyResizeSlot);
    connect(&__notifyAll, &NotifyAll::notifyAllError,
            this, &PDCResizeChecker::notifyErrorSlot);

    connect(m_thread, &QThread::finished,
            m_thread, &QThread::deleteLater);
    connect(m_thread, &QThread::started,
        this, &PDCResizeChecker::process);

    m_thread->start();

    m_mut.lock();
    while (!m_thIdReady)
        m_mutCond.wait(&m_mut, QDeadlineTimer::Forever);
    m_mut.unlock();
}

void PDCResizeChecker::notifyErrorSlot(void *handle)
{
    if (handle == m_handle)
        emit errorOccurred();
}

void PDCResizeChecker::notifyResizeSlot(void *handle, int width, int height)
{
    if (handle == m_handle)
        emit resized(width, height);
}

void PDCResizeChecker::process()
{
    MSG msg;
    HWINEVENTHOOK hook;
    m_thHandle = QThread::currentThreadId();

    m_mut.lock();
    m_thIdReady = true;
    m_mut.unlock();
    m_mutCond.notify_all();

    if (!(hook = SetWinEventHook(EVENT_CONSOLE_LAYOUT,
                            EVENT_CONSOLE_LAYOUT,
                            NULL,
                            _console_resize_event,
                            0,
                            0,
                            WINEVENT_OUTOFCONTEXT))) {
        emit errorOccurred();
        return;
    }

    qDebug() << "Start wait msg from thread with id" << GetCurrentThreadId();
    BOOL bRet;
    while ((bRet = GetMessage(&msg, (HWND)m_handle, 0, 0)) != 0) {
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else
        {
            if (msg.message == WM_MY_TERM_THREAD) {
                qDebug() << "Terminated thread with id" << GetCurrentThreadId();
                return;
            }
            qDebug() << "Get msg with thread id" << GetCurrentThreadId();
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWinEvent(hook);
    qDebug() << "Error in thread with id" << GetCurrentThreadId();
    emit errorOccurred();
}
