#include "athr_terminal_win32.h"
#include "athr_logger.h"
#include "athr_terminal.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static long tput_cols(void)
{
    char buff[16] = {0};
    FILE *fd = NULL;
    long ncols = -1;

    if (!(fd = _popen("tput cols", "r")))
    {
        error("failed to run tput");
        goto cleanup;
    }

    if (!fgets(buff, 16, fd))
    {
        error("fgets failed");
        goto cleanup;
    }
    if (!buff[0])
    {
        error("fgets read failed");
        goto cleanup;
    }

    char *end = NULL;
    long tentative = strtol(buff, &end, 10);
    if (*end == '\n') *end = 0;
    if (*end)
    {
        error("invalid number");
        goto cleanup;
    }

    if (tentative < 0 || tentative > UINT_MAX)
    {
        error("ncols overflow");
        goto cleanup;
    }
    ncols = (unsigned)tentative;

cleanup:
    if (fd) _pclose(fd);
    return ncols;
}

unsigned athr_terminal_win32_width(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hdl == INVALID_HANDLE_VALUE)
    {
        error("invalid handle");
        goto fallback;
    }
    if (GetConsoleScreenBufferInfo(hdl, &csbi) == 0)
    {
        error("failed to get screen info");
        goto fallback;
    }

    return csbi.srWindow.Right - csbi.srWindow.Left + 1;

    long ncols = -1;

fallback:
    ncols = tput_cols();
    return ncols == -1 ? athr_terminal_fallback_width() : (unsigned)ncols;
}
