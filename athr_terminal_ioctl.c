#include "athr_terminal_ioctl.h"
#include "athr_logger.h"
#include "athr_terminal.h"
#include <stdio.h>
#include <sys/ioctl.h>

#if ATHR_OS == ATHR_OS_WIN32
#include <windows.h>
#endif

/* Source: wget2 */
unsigned athr_terminal_ioctl_width(void)
{
    struct winsize wsz;
    int fd = fileno(stderr);

    if (ioctl(fd, TIOCGWINSZ, &wsz) >= 0) return wsz.ws_col;

    error("ioctl failed");
    return athr_terminal_fallback_width();
}
