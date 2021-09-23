#ifndef ATHR_CANVAS_H
#define ATHR_CANVAS_H

#include "athr/export.h"
#include <stdbool.h>

#define ATHR_CANVAS_MAX_SIZE 512
#define ATHR_CANVAS_MIN_SIZE                                                   \
    (ATHR_WIDGET_BAR_MIN_SIZE + ATHR_WIDGET_ETA_SIZE + ATHR_WIDGET_PERC_SIZE + \
     ATHR_WIDGET_TEXT_MIN_SIZE)

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

ATHR_API void athr_canvas_use_stderr(bool use);
ATHR_API void athr_canvas_create(struct athr_canvas *canvas);
ATHR_API void athr_canvas_draw(struct athr_canvas const *canvas);
ATHR_API void athr_canvas_clean(struct athr_canvas *canvas);
ATHR_API void athr_canvas_close(struct athr_canvas *canvas);
ATHR_API bool athr_canvas_resize(struct athr_canvas *canvas);
ATHR_API void athr_canvas_setup(struct athr_canvas *canvas, unsigned min_size,
                                unsigned max_size);

#endif
