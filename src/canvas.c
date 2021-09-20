#include "athr/canvas.h"
#include "common.h"
#include "report.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

void athr_canvas_create(struct athr_canvas *canvas, unsigned min_size)
{
    canvas->min_size = min_size;
    unsigned ncols = athr_get_term_width() + 1;
#ifdef WIN32
    assert(ncols > 0);
    ncols--;
#endif

    if (ncols < canvas->min_size)
        canvas->size = canvas->min_size;
    else
        canvas->size = ncols;
    canvas->size = MIN(canvas->size, ATHR_CANVAS_MAX_SIZE);
}

void athr_canvas_draw(struct athr_canvas const *canvas)
{
    fprintf(stderr, "%.*s", canvas->size, canvas->buff);
    fflush(stderr);
}

void athr_canvas_resize(struct athr_canvas *canvas)
{
    unsigned ncols = athr_get_term_width() + 1;
#ifdef WIN32
    assert(ncols > 0);
    ncols--;
#endif

    if (ncols < canvas->min_size) ncols = canvas->min_size;
    canvas->size = ncols;
    canvas->size = MIN(canvas->size, ATHR_CANVAS_MAX_SIZE);
}

void athr_canvas_clean(struct athr_canvas *canvas)
{
    for (unsigned i = 0; i < canvas->size - 1; ++i)
        canvas->buff[i] = ' ';
    canvas->buff[canvas->size - 1] = '\r';
}
