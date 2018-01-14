#include "canvas.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

void canvas_create(struct canvas *canvas, int min_length) {
    int ncols;

    canvas->min_length = min_length;

    ncols = (int)athr_get_term_width() + 1;
#ifdef WIN32
    ncols--;
#endif

    if (ncols < canvas->min_length)
        canvas->length = canvas->min_length;
    else
        canvas->length = ncols;

    canvas->buff = malloc(canvas->length * sizeof(char));
}

void canvas_draw(struct canvas *canvas) {
    fprintf(stderr, "%.*s", canvas->length, canvas->buff);
    fflush(stderr);
}

void canvas_resize(struct canvas *canvas) {

    int ncols = (int)athr_get_term_width() + 1;
#ifdef WIN32
    ncols--;
#endif

    if (ncols < canvas->min_length)
        ncols = canvas->min_length;

    if (canvas->length != ncols) {
        canvas->buff = realloc(canvas->buff, ncols);
        canvas->length = ncols;
    }
}

void canvas_clean(struct canvas *canvas) {
    int i;
    for (i = 0; i < canvas->length - 1; ++i)
        canvas->buff[i] = ' ';
    canvas->buff[canvas->length - 1] = '\r';
}

void canvas_finish(struct canvas *canvas) { free(canvas->buff); }
