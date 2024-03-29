#include "term_ioctl.h"
#include "athr/os.h"
#include "logger.h"
#include "terminal.h"
#include <stdio.h>
#include <sys/ioctl.h>

#ifdef ATHR_WINDOWS
#include <windows.h>
#endif

/* Source: wget2 */
unsigned __athr_term_ioctl_width(void)
{
    struct winsize wsz;
    int fd = fileno(stderr);

    if (ioctl(fd, TIOCGWINSZ, &wsz) >= 0) return wsz.ws_col;

    error("ioctl failed");
    return __athr_terminal_fallback_width();
}
