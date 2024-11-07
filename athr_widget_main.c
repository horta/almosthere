#include "athr_widget_main.h"
#include "athr_canvas.h"
#include "athr_min.h"
#include "athr_widget.h"
#include "athr_widget_main.h"
#include <assert.h>
#include <stdlib.h>

static void update(struct athr_widget *, double, double);
static void finish(struct athr_widget *, double);

static unsigned min_len(struct athr_widget const *x)
{
    struct athr_widget_main *m = (struct athr_widget_main *)x->derived;
    /* +1 for carriage return */
    unsigned s = 1;
    for (unsigned i = 0; i < m->nwidgets; ++i)
        s += m->children[i]->vtable->min_len(m->children[i]);
    return s;
}

static unsigned max_len(struct athr_widget const *x)
{
    struct athr_widget_main *m = (struct athr_widget_main *)x->derived;
    /* +1 for carriage return */
    unsigned s = 1;
    for (unsigned i = 0; i < m->nwidgets; ++i)
        s += m->children[i]->vtable->max_len(m->children[i]);
    return s;
}

static void partition(unsigned nwidgets, struct athr_widget **widget,
                      unsigned size);

static struct athr_widget_vtable const vtable = {update, finish, min_len,
                                                 max_len};

struct athr_widget_bar *athr_widget_main_add_bar(struct athr_widget_main *x)
{
    x->children[x->nwidgets++] = (struct athr_widget *)&x->bar;
    return &x->bar;
}

struct athr_widget_eta *athr_widget_main_add_eta(struct athr_widget_main *x)
{
    x->children[x->nwidgets++] = (struct athr_widget *)&x->eta;
    return &x->eta;
}

struct athr_widget_perc *athr_widget_main_add_perc(struct athr_widget_main *x)
{
    x->children[x->nwidgets++] = (struct athr_widget *)&x->perc;
    return &x->perc;
}

struct athr_widget_text *athr_widget_main_add_text(struct athr_widget_main *x)
{
    x->children[x->nwidgets++] = (struct athr_widget *)&x->text;
    return &x->text;
}

void athr_widget_main_create(struct athr_widget_main *x)
{
    widget_setup((struct athr_widget *)x, &vtable);
    x->nwidgets = 0;
    for (unsigned i = 0; i < ATHR_WIDGET_MAIN_MAX_CHILDREN; ++i)
        x->children[i] = NULL;
    athr_canvas_create(&x->canvas);
}

void athr_widget_main_setup(struct athr_widget_main *x)
{
    athr_canvas_setup(&x->canvas, min_len(&x->super), max_len(&x->super));
    partition(x->nwidgets, x->children, x->canvas.len - 1);
}

typedef void (*callback_t)(struct athr_widget *, void const *);

static void call_children(struct athr_widget *x, callback_t cb, void *arg)
{
    struct athr_widget_main *m = x->derived;
    unsigned offset = 0;

    bool resized = athr_canvas_resize(&m->canvas);
    athr_canvas_clean(&m->canvas);
    if (resized) partition(m->nwidgets, m->children, m->canvas.len - 1);

    for (unsigned i = 0; i < m->nwidgets; ++i)
    {
        m->children[i]->canvas.buff = m->canvas.buff + offset;
        cb(m->children[i], arg);
        offset += m->children[i]->canvas.len;
    }
    athr_canvas_draw(&m->canvas);
}

static void update_cb(struct athr_widget *x, void const *arg)
{
    double const *values = arg;
    x->vtable->update(x, values[0], values[1]);
}

static void update(struct athr_widget *x, double consumed, double speed)
{
    double const arg[2] = {consumed, speed};
    call_children(x, update_cb, (void *)arg);
}

static void finish_cb(struct athr_widget *x, void const *arg)
{
    double const *total_elapsed = arg;
    x->vtable->finish(x, *total_elapsed);
}

static void finish(struct athr_widget *x, double total_elapsed)
{
    call_children(x, finish_cb, (void *)&total_elapsed);
}

static unsigned assign_minimum_len(unsigned nwidgets,
                                   struct athr_widget **widget, unsigned len)
{
    for (unsigned i = 0; i < nwidgets; ++i)
    {
        unsigned s = widget[i]->vtable->min_len(widget[i]);
        widget[i]->canvas.len = s;
        len -= s;
    }
    return len;
}

static unsigned remaining_widgets(unsigned n, struct athr_widget **widget)
{
    unsigned remaining = 0;
    for (unsigned i = 0; i < n; ++i)
    {
        unsigned s = widget[i]->vtable->max_len(widget[i]);
        if (widget[i]->canvas.len < s) remaining++;
    }
    return remaining;
}

static unsigned increase_len(unsigned nwidgets, struct athr_widget **widget,
                             unsigned size, unsigned remain)
{
    for (unsigned i = 0; i < nwidgets; ++i)
    {
        unsigned max_size = widget[i]->vtable->max_len(widget[i]);
        unsigned amount = athr_min(size, max_size - widget[i]->canvas.len);
        amount = athr_min(amount, remain);
        widget[i]->canvas.len += amount;
        remain -= amount;
    }
    return remain;
}

static void partition(unsigned nwidgets, struct athr_widget **widget,
                      unsigned len)
{
    unsigned remain = assign_minimum_len(nwidgets, widget, len);
    unsigned npart = remaining_widgets(nwidgets, widget);
    if (npart > 0)
    {
        remain = increase_len(nwidgets, widget, remain / npart, remain);
        if (remain > 0) remain = increase_len(nwidgets, widget, remain, remain);
    }
}
