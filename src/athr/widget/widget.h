#ifndef ATHR_WIDGET_WIDGET_H
#define ATHR_WIDGET_WIDGET_H

#include "athr/canvas.h"
#include "athr/canvas_view.h"

/* maximum string number for preventing catastrophic mistakes */
#define ATHR_MAX_STR_LEN 1048576

struct athr_widget;

typedef void(athr_widget_update)(struct athr_widget *, double, double, double);
typedef unsigned(athr_widget_get_min_length)(struct athr_widget *);
typedef unsigned(athr_widget_get_max_length)(struct athr_widget *);

struct athr_widget_vtable
{
    athr_widget_update *update;
    athr_widget_get_min_length *min_length;
    athr_widget_get_max_length *max_length;
};

struct athr_widget
{
    void *derived;
    struct athr_widget_vtable const *vtable;
    struct athr_canvas_view canvas;
};

#define ATHR_WIDGET_INIT                                                       \
    (struct athr_widget) { NULL, NULL, ATHR_CANVAS_VIEW_INIT }

#endif
