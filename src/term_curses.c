#include "term_curses.h"
#include "logger.h"
#include "terminal.h"
#include <curses.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <unistd.h>

unsigned term_curses_width(void)
{
    static volatile int failed_before = 0;
    int cols = -1;
    int tty_fd = -1;

    char const *const term = getenv("TERM");
    if (!term)
    {
        if (!failed_before) error("TERM environment variable not set");
        failed_before = 1;
        goto done;
    }

    char const *const cterm_path = ctermid(NULL);
    if (!cterm_path || !cterm_path[0])
    {
        if (!failed_before) error("ctermid() failed");
        return terminal_fallback_width();
    }

    tty_fd = open(cterm_path, O_RDWR);
    if (tty_fd == -1)
    {
        if (!failed_before) error("open() failed");
        failed_before = 1;
        goto done;
    }

    int setupterm_err;

    if (setupterm((char *)term, tty_fd, &setupterm_err) == ERR)
    {
        switch (setupterm_err)
        {
        case -1:
            if (!failed_before)
                error("setupterm() failed: terminfo database not found");
            failed_before = 1;
            goto done;
        case 0:
            if (!failed_before)
                error("setupterm() failed: TERM=%s not found in database or "
                      "too generic");
            failed_before = 1;
            goto done;
        case 1:
            if (!failed_before)
                error("setupterm() failed: terminal is hardcopy");
            failed_before = 1;
            goto done;
        } // err
    }

    cols = tigetnum((char *)"cols");
    if (cols < 0)
    {
        if (!failed_before) error("tigetnum() failed");
        failed_before = 1;
    }

    struct term *termp = set_curterm(NULL);
    (void)del_curterm(termp);

done:
    if (tty_fd != -1) close(tty_fd);

    return cols < 0 ? terminal_fallback_width() : (unsigned)cols;
}
