#ifndef ALMOSTHERE_WIDGET_H_
#define ALMOSTHERE_WIDGET_H_

#include "canvas.h"

struct widget;

typedef void (*widget_finish)(struct widget *);
typedef void (*widget_update)(struct widget *, double, double, double);
typedef int (*widget_get_min_length)(struct widget *);
typedef int (*widget_get_max_length)(struct widget *);

struct widget {
    widget_finish finish;
    widget_update update;
    widget_get_min_length get_min_length;
    widget_get_max_length get_max_length;
    void *data;
    struct canvas *canvas;
};

#endif /* end of include guard: ALMOSTHERE_WIDGET_H_ */
