#define ATHR_DEFAULT_NCOLS 80

#include "terminal/terminal.h"
#ifdef CURSES_FOUND
#include <curses.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <unistd.h>

unsigned athr_get_term_width(void)
{
    static int failed_before = 0;
    int cols = -1;
    int tty_fd = -1;

    char const *const term = getenv("TERM");
    if (!term) {
        if (!failed_before)
            fprintf(stderr, "TERM environment variable not set\n");
        failed_before = 1;
        goto done;
    }

    char const *const cterm_path = ctermid(NULL);
    if (!cterm_path || !cterm_path[0]) {
        if (!failed_before)
            fprintf(stderr, "ctermid() failed\n");
        return ATHR_DEFAULT_NCOLS;
    }

    tty_fd = open(cterm_path, O_RDWR);
    if (tty_fd == -1) {
        if (!failed_before)
            fprintf(stderr, "open(\"%s\") failed (%d): %s\n", cterm_path, errno,
                    strerror(errno));
        failed_before = 1;
        goto done;
    }

    int setupterm_err;

    if (setupterm((char *)term, tty_fd, &setupterm_err) == ERR) {
        switch (setupterm_err) {
        case -1:
            if (!failed_before)
                fprintf(stderr, "setupterm() failed: terminfo database not found\n");
            failed_before = 1;
            goto done;
        case 0:
            if (!failed_before)
                fprintf(stderr,
                        "setupterm() failed: TERM=%s not found in database or too "
                        "generic\n",
                        term);
            failed_before = 1;
            goto done;
        case 1:
            if (!failed_before)
                fprintf(stderr, "setupterm() failed: terminal is hardcopy\n");
            failed_before = 1;
            goto done;
        } // err
    }

    cols = tigetnum((char *)"cols");
    if (cols < 0) {
        if (!failed_before)
            fprintf(stderr, "tigetnum() failed (%d)\n", cols);
        failed_before = 1;
    }

    struct term *termp = set_curterm(NULL);
    (void)del_curterm(termp);

done:
    if (tty_fd != -1)
        close(tty_fd);

    return cols < 0 ? ATHR_DEFAULT_NCOLS : cols;
}

#else
#ifdef WIN32
#include <windows.h>
unsigned athr_get_term_width(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hdl == INVALID_HANDLE_VALUE) {
        return ATHR_DEFAULT_NCOLS;
    }
    if (GetConsoleScreenBufferInfo(hdl, &csbi) == 0) {
        return ATHR_DEFAULT_NCOLS;
    }
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}
#else
unsigned athr_get_term_width(void) { return ATHR_DEFAULT_NCOLS; }
#endif
#endif
