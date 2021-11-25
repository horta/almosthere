#include "athr/canvas.h"
#include "athr/ovs_atomic.h"
#include "common.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static atomic_bool use_stderr = false;

void athr_canvas_use_stderr(bool use) { atomic_store(&use_stderr, use); }

void athr_canvas_create(struct athr_canvas *canvas)
{
    canvas->len = 0;
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
    unsigned ncols = terminal_width() + 1;
#ifdef WIN32
    ncols--;
#endif

    unsigned prev_len = canvas->len;
    canvas->len = ncols;
    canvas->len = minu(canvas->len, canvas->max_len);
    canvas->len = maxu(canvas->len, canvas->min_len);
    return prev_len != canvas->len;
}

void athr_canvas_setup(struct athr_canvas *canvas, unsigned min_len,
                       unsigned max_len)
{
    canvas->min_len = min_len;
    canvas->max_len = minu(max_len, ATHR_CANVAS_MAX_LEN);
}

void athr_canvas_clean(struct athr_canvas *canvas)
{
    memset(canvas->buff, ' ', canvas->len - 1);
    canvas->buff[canvas->len - 1] = '\r';
}

void athr_canvas_close(struct athr_canvas *canvas)
{
    if (use_stderr)
    {
        fprintf(stderr, "\n");
        fflush(stderr);
    }
    else
    {
        fprintf(stdout, "\n");
        fflush(stderr);
    }
}
