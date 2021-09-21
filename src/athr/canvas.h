#ifndef ATHR_CANVAS_H
#define ATHR_CANVAS_H

#include "athr/widget/total.h"
#include <stdbool.h>

#define ATHR_CANVAS_MAX_SIZE 512
#define ATHR_CANVAS_MIN_SIZE ATHR_WIDGET_TOTAL_MIN_SIZE

struct athr_canvas
{
    unsigned size;
    unsigned min_size;
    unsigned max_size;
    char buff[ATHR_CANVAS_MAX_SIZE];
};

#define ATHR_CANVAS_INIT                                                       \
    (struct athr_canvas)                                                       \
    {                                                                          \
        0, 0, 0, { '\0' }                                                      \
    }

void athr_canvas_create(struct athr_canvas *canvas);
void athr_canvas_draw(struct athr_canvas const *canvas);
void athr_canvas_clean(struct athr_canvas *canvas);
void athr_canvas_close(struct athr_canvas *canvas);
bool athr_canvas_resize(struct athr_canvas *canvas);
void athr_canvas_setup(struct athr_canvas *canvas, unsigned min_size,
                       unsigned max_size);

#endif
