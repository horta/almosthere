#ifndef ATHR_WIDGET_WIDGET_H
#define ATHR_WIDGET_WIDGET_H

#include "athr_canvas_view.h"

struct athr_widget;

typedef void(athr_widget_update_t)(struct athr_widget *, double, double);
typedef void(athr_widget_finish_t)(struct athr_widget *, double);
typedef unsigned(athr_widget_min_len_t)(struct athr_widget const *);
typedef unsigned(athr_widget_max_len_t)(struct athr_widget const *);

struct athr_widget_vtable
{
    athr_widget_update_t  *update;
    athr_widget_finish_t  *finish;
    athr_widget_min_len_t *min_len;
    athr_widget_max_len_t *max_len;
};

struct athr_widget
{
    void                            *derived;
    struct athr_widget_vtable const *vtable;
    struct athr_canvas_view          canvas;
};

#define ATHR_WIDGET_INIT                                                       \
    (struct athr_widget) { NULL, NULL, ATHR_CANVAS_VIEW_INIT }

static inline void athr_widget_update(struct athr_widget *x, double consumed,
                                      double speed)
{
    x->vtable->update(x, consumed, speed);
}

static inline void athr_widget_finish(struct athr_widget *x,
                                      double total_elapsed)
{
    x->vtable->finish(x, total_elapsed);
}

static inline unsigned athr_widget_min_len(struct athr_widget *x)
{
    return x->vtable->min_len(x);
}

static inline unsigned athr_widget_max_len(struct athr_widget *x)
{
    return x->vtable->max_len(x);
}

static inline void widget_setup(struct athr_widget *x,
                                struct athr_widget_vtable const *vtable)
{
    x->derived = x;
    x->vtable = vtable;
}

#endif
