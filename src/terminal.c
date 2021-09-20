#include "terminal.h"
#include "athr/terminal.h"

#if defined(TERMINAL_CURSES)
#include "term_curses.h"
#endif

#if defined(TERMINAL_WIN32)
#include "term_win32.h"
#endif

static unsigned fallback_width = 80;
static bool force_fallback_use = false;

void athr_terminal_set_fallback(unsigned width) { fallback_width = width; }

void athr_terminal_use_fallback(void) { force_fallback_use = true; }

unsigned terminal_width(void)
{
    if (terminal_force_fallback_use()) return terminal_fallback_width();
#if defined(TERMINAL_CURSES)
    return term_curses_width();
#elif defined(TERMINAL_WIN32)
    return term_win32_width();
#else
    return terminal_fallback_width();
#endif
}

unsigned terminal_fallback_width(void) { return fallback_width; }

bool terminal_force_fallback_use(void) { return force_fallback_use; }
