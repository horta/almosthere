#include "term_ioctl.h"
#include "terminal.h"
#include <stdio.h>
#include <sys/ioctl.h>

/* Source: wget2 */
unsigned term_ioctl_width(void)
{
    struct winsize wsz;
    int fd = fileno(stderr);

    if (ioctl(fd, TIOCGWINSZ, &wsz) >= 0) return wsz.ws_col;

    return terminal_fallback_width();
}
