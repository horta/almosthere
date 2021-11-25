#ifndef ATHR_CANVAS_H
#define ATHR_CANVAS_H

#include "athr/export.h"
#include <stdbool.h>

#define ATHR_CANVAS_MAX_LEN 512
#define ATHR_CANVAS_MIN_LEN                                                    \
    (ATHR_WIDGET_BAR_MIN_LEN + ATHR_WIDGET_ETA_LEN + ATHR_WIDGET_PERC_LEN +    \
     ATHR_WIDGET_TEXT_MIN_LEN)

struct athr_canvas
{
    unsigned len;
    unsigned min_len;
    unsigned max_len;
    char buff[ATHR_CANVAS_MAX_LEN];
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
ATHR_API void athr_canvas_setup(struct athr_canvas *canvas, unsigned min_len,
                                unsigned max_len);

#endif
