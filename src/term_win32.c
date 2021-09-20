#include "term_win32.h"
#include "terminal.h"
#include <windows.h>

unsigned term_win32_width(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hdl == INVALID_HANDLE_VALUE)
    {
        return terminal_fallback_width();
    }
    if (GetConsoleScreenBufferInfo(hdl, &csbi) == 0)
    {
        return terminal_fallback_width();
    }
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}
