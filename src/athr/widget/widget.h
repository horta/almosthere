#ifndef ATHR_WIDGET_WIDGET_H
#define ATHR_WIDGET_WIDGET_H

#include "athr/canvas.h"
#include "athr/canvas_view.h"

/* maximum string number for preventing catastrophic mistakes */
#define ATHR_MAX_STR_LEN 1048576

struct athr_widget;

typedef void(athr_widget_update_t)(struct athr_widget *, double, double,
                                   double);
typedef unsigned(athr_widget_min_size_t)(struct athr_widget *);
typedef unsigned(athr_widget_max_size_t)(struct athr_widget *);

struct athr_widget_vtable
{
    athr_widget_update_t *update;
    athr_widget_min_size_t *min_length;
    athr_widget_max_size_t *max_length;
};

struct athr_widget
{
    void *derived;
    struct athr_widget_vtable const *vtable;
    struct athr_canvas_view canvas;
};

#define ATHR_WIDGET_INIT                                                       \
    (struct athr_widget) { NULL, NULL, ATHR_CANVAS_VIEW_INIT }

static inline void athr_widget_update(struct athr_widget *w, double consumed,
                                      double speed, double dlt)
{
    w->vtable->update(w, consumed, speed, dlt);
}

static inline unsigned athr_widget_min_size(struct athr_widget *w)
{
    return w->vtable->min_length(w);
}

static inline unsigned athr_widget_max_size(struct athr_widget *w)
{
    return w->vtable->max_length(w);
}

#endif
