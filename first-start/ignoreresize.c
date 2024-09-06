#include <windows.h>
#include <stdio.h>
// Глобальная переменная для хранения хэндла окна
HWND hWnd;
HWND hcmd;

// Процедура обработки сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
        case WM_NCHITTEST: // Обработка события, когда мышь перемещается над заголовком окна
            return HTCAPTION; // Возвращаем HTCAPTION, чтобы запретить изменение размера
        case WM_DESTROY: // Обработка события закрытия окна
            PostQuitMessage(0); // Завершаем приложение
            return 0;
        case WM_MOUSEMOVE:
            
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // Обработка остальных сообщений
    }

    printf("mouse\n");
}

int w = 100;
int h = 200;

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
        return;
    }


}

int main()
{
    hcmd = GetConsoleWindow();
    SetWindowLongPtr(hcmd, GWL_STYLE, GetWindowLong(hcmd, GWL_STYLE)&~WS_SIZEBOX);
    //GetShellWindow();
    //SetWindowLongPtr(GetConsoleWindow(), GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)&~WS_SIZEBOX);
#if 0
    HWINEVENTHOOK hook;
    if (!(hook = SetWinEventHook(EVENT_CONSOLE_LAYOUT,
                            EVENT_CONSOLE_LAYOUT,
                            NULL,
                            _console_resize_event,
                            0,
                            0,
                            WINEVENT_OUTOFCONTEXT))) {
               exit(1);
    }
    UnhookWinEvent(hook);
#endif

    Sleep(10000);
    return 0;
}