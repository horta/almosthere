#ifndef ALMOSTHERE_WIDGET_H_
#define ALMOSTHERE_WIDGET_H_

#include "canvas.h"

struct widget;

typedef void (*widget_finish)(struct widget *);
typedef void (*widget_update)(struct widget *, double, double, double);
typedef int (*widget_get_min_length)(void);

struct widget {
    widget_finish finish;
    widget_update update;
    widget_get_min_length get_min_length;
    void *data;
    struct canvas *canvas;
};

#endif /* end of include guard: ALMOSTHERE_WIDGET_H_ */
