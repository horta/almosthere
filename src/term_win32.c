#include "term_win32.h"
#include "terminal.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static long tput_cols(void)
{
    char buff[16] = {0};
    FILE *fd = NULL;
    long ncols = -1;

    if (!(fd = _popen("tput cols", "r"))) goto cleanup;

    if (!fgets(buff, 16, fd)) goto cleanup;
    if (!buff[0]) goto cleanup;

    char *end = NULL;
    long tentative = strtol(buff, &end, 10);
    if (*end == '\n') *end = 0;
    if (*end) goto cleanup;

    if (tentative < 0 || tentative > UINT_MAX) goto cleanup;
    ncols = (unsigned)tentative;

cleanup:
    if (fd) _pclose(fd);
    return ncols;
}

unsigned term_win32_width(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hdl == INVALID_HANDLE_VALUE)
    {
        goto fallback;
    }
    if (GetConsoleScreenBufferInfo(hdl, &csbi) == 0)
    {
        goto fallback;
    }

    return csbi.srWindow.Right - csbi.srWindow.Left + 1;

    long ncols = -1;

fallback:
    ncols = tput_cols();
    return ncols == -1 ? terminal_fallback_width() : (unsigned)ncols;
}
