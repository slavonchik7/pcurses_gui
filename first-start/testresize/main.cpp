#include <QDebug>
#include "../pdcwlib/pdcresizer.hpp"
#include <QCoreApplication>
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    //qDebug() << "handler";
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void win1resizedslot(int weight, int height)
{
    qDebug() << "WIN1 was resized to";
}
void win2resizedslot(int weight, int height)
{
    qDebug() << "WIN2 was resized to";
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    const wchar_t CLASS_NAME1[] = L"win1class";
    const wchar_t CLASS_NAME2[] = L"win2class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
     
#if 0
    wc.lpszClassName = CLASS_NAME1;
    RegisterClass(&wc);
    HWND hwnd1 = CreateWindowEx(
    0,                              // Optional window styles.
    CLASS_NAME1,                     // Window class
    L"WIN1",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    NULL,       // Parent window    
    NULL,       // Menu
     wc.hInstance,  // Instance handle
    NULL        // Additional application data
    );
    if (hwnd1 == NULL) {
        qDebug() << "can not create WIN1" << GetLastError();
        exit(1);
    }
    ShowWindow(hwnd1, SW_SHOW);


    wc.lpszClassName = CLASS_NAME2;
    RegisterClass(&wc);
    HWND hwnd2 = CreateWindowEx(
    0,                              // Optional window styles.
    CLASS_NAME2,                     // Window class
    L"WIN2",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    NULL,       // Parent window    
    NULL,       // Menu
    wc.hInstance,  // Instance handle
    NULL        // Additional application data
    );
    if (hwnd1 == NULL) {
        qDebug() << "can not create WIN2" << GetLastError();
        exit(1);
    }
    ShowWindow(hwnd2, SW_SHOW);

    PDCResizeChecker win1rs, win2rs;
    QObject::connect(&win1rs, &PDCResizeChecker::resized, win1resizedslot);
    QObject::connect(&win2rs, &PDCResizeChecker::resized, win2resizedslot);
    
    win1rs.start(hwnd1);
    win2rs.start(hwnd2);
#endif
    PDCResizeChecker wincmd;
    QObject::connect(&wincmd, &PDCResizeChecker::resized, win2resizedslot);
    HWND hcmd = GetConsoleWindow();
    wincmd.start(hcmd);

    return app.exec();
}