#include "canvas.h"
#include "report.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

void athr_canvas_create(struct canvas* canvas, int min_length)
{
    int ncols;

    canvas->min_length = min_length;

    ncols = athr_get_term_width() + 1;
#ifdef WIN32
    ncols--;
#endif

    if (ncols < canvas->min_length)
        canvas->length = canvas->min_length;
    else
        canvas->length = ncols;

    if (canvas->length < 0)
        athr_fatal("canvas length is negative");

    canvas->buff = malloc(((size_t)canvas->length) * sizeof(char));
}

void athr_canvas_draw(struct canvas* canvas)
{
    fprintf(stderr, "%.*s", canvas->length, canvas->buff);
    fflush(stderr);
}

void athr_canvas_resize(struct canvas* canvas)
{

    int ncols = athr_get_term_width() + 1;
#ifdef WIN32
    ncols--;
#endif

    if (ncols < canvas->min_length)
        ncols = canvas->min_length;

    if (ncols < 0)
        athr_fatal("ncols is negative");

    if (canvas->length != ncols) {
        canvas->buff = realloc(canvas->buff, (size_t)ncols);
        canvas->length = ncols;
    }
}

void athr_canvas_clean(struct canvas* canvas)
{
    int i;
    for (i = 0; i < canvas->length - 1; ++i)
        canvas->buff[i] = ' ';
    canvas->buff[canvas->length - 1] = '\r';
}

void athr_canvas_finish(struct canvas* canvas) { free(canvas->buff); }
