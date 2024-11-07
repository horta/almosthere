#include "athr_terminal.h"

#if ATHR_TERMINAL == ATHR_TERMINAL_CURSES
#include "athr_terminal_curses.h"
#endif

#if ATHR_TERMINAL == ATHR_TERMINAL_WIN32
#include "athr_terminal_win32.h"
#endif

#if ATHR_TERMINAL == ATHR_TERMINAL_IOCTL
#include "athr_terminal_ioctl.h"
#endif

static unsigned fallback_width = 80;
static bool force_fallback_use = false;

void athr_terminal_set_fallback_width(unsigned width) { fallback_width = width; }

void athr_terminal_force_fallback_use(bool v) { force_fallback_use = v; }

unsigned athr_terminal_width(void)
{
    if (force_fallback_use) return athr_terminal_fallback_width();
#if ATHR_TERMINAL == ATHR_TERMINAL_CURSES
    return athr_terminal_curses_width();
#elif ATHR_TERMINAL == ATHR_TERMINAL_WIN32
    return athr_terminal_win32_width();
#elif ATHR_TERMINAL == ATHR_TERMINAL_IOCTL
    return athr_terminal_ioctl_width();
#else
    return __athr_terminal_fallback_width();
#endif
}

unsigned athr_terminal_fallback_width(void) { return fallback_width; }
