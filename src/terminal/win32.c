#include "terminal/terminal.h"
#include <windows.h>

int athr_get_term_width(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE                     hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hdl == INVALID_HANDLE_VALUE) {
        return ATHR_DEFAULT_NCOLS;
    }
    if (GetConsoleScreenBufferInfo(hdl, &csbi) == 0) {
        return ATHR_DEFAULT_NCOLS;
    }
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}
