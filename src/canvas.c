#include "canvas.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

struct canvas *canvas_create(int min_length) {
    int ncols;
    struct canvas *c;
    c = malloc(sizeof(struct canvas));

    c->min_length = min_length;

    ncols = (int)almosthere_get_term_width();
    if (ncols < c->min_length)
        c->length = c->min_length;
    else
        c->length = ncols;

    c->buff = malloc(c->length * sizeof(char));
    return c;
}

void canvas_draw(struct canvas *canvas) {
    fprintf(stderr, "%.*s", canvas->length, canvas->buff);
    fflush(stderr);
}

void canvas_resize(struct canvas *canvas) {
    int ncols = (int)almosthere_get_term_width();
    if (ncols < canvas->min_length)
        ncols = canvas->min_length;

    if (canvas->length != ncols) {
        canvas->buff = realloc(canvas->buff, ncols);
        canvas->length = ncols;
    }
}

void canvas_clean(struct canvas *canvas) {
    int i;
    for (i = 0; i < canvas->length; ++i)
        canvas->buff[i] = ' ';
}

void canvas_finish(struct canvas *canvas) {
    free(canvas->buff);
    free(canvas);
}
