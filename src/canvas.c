#include "athr/canvas.h"
#include "common.h"
#include "terminal.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static atomic_bool use_stderr = false;

void athr_canvas_use_stderr(bool use) { atomic_store(&use_stderr, use); }

void athr_canvas_create(struct athr_canvas *canvas)
{
    canvas->size = 0;
    canvas->min_size = 0;
    canvas->max_size = 0;
#if 0
    unsigned ncols = terminal_width() + 1;
#ifdef WIN32
    assert(ncols > 0);
    ncols--;
#endif

    if (ncols < canvas->min_size)
        canvas->size = canvas->min_size;
    else
        canvas->size = ncols;
    canvas->size = MIN(canvas->size, ATHR_CANVAS_MAX_SIZE);
#endif
}

void athr_canvas_draw(struct athr_canvas const *canvas)
{
    if (use_stderr)
    {
        fprintf(stderr, "%.*s", canvas->size, canvas->buff);
        fflush(stderr);
    }
    else
    {
        fprintf(stdout, "%.*s", canvas->size, canvas->buff);
        fflush(stdout);
    }
}

bool athr_canvas_resize(struct athr_canvas *canvas)
{
    unsigned ncols = terminal_width() + 1;
#ifdef WIN32
    assert(ncols > 0);
    ncols--;
#endif

    unsigned prev_size = canvas->size;
    canvas->size = ncols;
    canvas->size = minu(canvas->size, canvas->max_size);
    canvas->size = maxu(canvas->size, canvas->min_size);
    return prev_size != canvas->size;
}

void athr_canvas_setup(struct athr_canvas *canvas, unsigned min_size,
                       unsigned max_size)
{
    canvas->min_size = min_size;
    canvas->max_size = minu(max_size, ATHR_CANVAS_MAX_SIZE);
}

void athr_canvas_clean(struct athr_canvas *canvas)
{
    memset(canvas->buff, ' ', canvas->size - 1);
    canvas->buff[canvas->size - 1] = '\r';
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
