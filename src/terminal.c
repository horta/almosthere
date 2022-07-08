#include "terminal.h"
#include "athr/term.h"

#if defined(TERMINAL_CURSES)
#include "term_curses.h"
#endif

#if defined(TERMINAL_WIN32)
#include "term_win32.h"
#endif

#if defined(TERMINAL_IOCTL)
#include "term_ioctl.h"
#endif

static unsigned fallback_width = 80;
static bool force_fallback_use = false;

void athr_term_set_fallback_width(unsigned width) { fallback_width = width; }

void athr_term_force_fallback_use(bool v) { force_fallback_use = v; }

unsigned __athr_terminal_width(void)
{
    if (__athr_terminal_force_fallback_use())
        return __athr_terminal_fallback_width();
#if defined(TERMINAL_CURSES)
    return __athr_term_curses_width();
#elif defined(TERMINAL_WIN32)
    return __athr_term_win32_width();
#elif defined(TERMINAL_IOCTL)
    return __athr_term_ioctl_width();
#else
    return __athr_terminal_fallback_width();
#endif
}

unsigned __athr_terminal_fallback_width(void) { return fallback_width; }

bool __athr_terminal_force_fallback_use(void) { return force_fallback_use; }
