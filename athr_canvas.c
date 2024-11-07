#include "athr_canvas.h"
#include "athr_min.h"
#include "athr_max.h"
#include "athr_terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if ATHR_OS == ATHR_OS_WIN32
#include "athr_ovs_atomic_msvc.h"
#elif ATHR_OS == ATHR_OS_UNIX
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#endif

static atomic_bool use_stderr = false;

void athr_canvas_use_stderr(bool use) { atomic_store(&use_stderr, use); }

void athr_canvas_create(struct athr_canvas *canvas)
{
    canvas->len     = 0;
    canvas->min_len = 0;
    canvas->max_len = 0;
}

void athr_canvas_draw(struct athr_canvas const *canvas)
{
    if (use_stderr)
    {
        fprintf(stderr, "%.*s", canvas->len, canvas->buff);
        fflush(stderr);
    }
    else
    {
        fprintf(stdout, "%.*s", canvas->len, canvas->buff);
        fflush(stdout);
    }
}

bool athr_canvas_resize(struct athr_canvas *canvas)
{
    unsigned ncols    = athr_terminal_width() + 1;
    unsigned prev_len = canvas->len;
    canvas->len       = ncols;
    canvas->len       = athr_min(canvas->len, canvas->max_len);
    canvas->len       = athr_max(canvas->len, canvas->min_len);
    return prev_len != canvas->len;
}

void athr_canvas_setup(struct athr_canvas *canvas, unsigned min_len,
                       unsigned max_len)
{
    canvas->min_len = min_len;
    canvas->max_len = athr_min(max_len, ATHR_CANVAS_MAX_LEN);
}

void athr_canvas_clean(struct athr_canvas *canvas)
{
    memset(canvas->buff, ' ', canvas->len - 1);
    canvas->buff[canvas->len - 1] = '\r';
}

void athr_canvas_close(struct athr_canvas *canvas)
{
    (void)canvas;
    if (use_stderr)
    {
        fprintf(stderr, "\n");
        fflush(stderr);
    }
    else
    {
        fprintf(stdout, "\n");
        fflush(stdout);
    }
}
