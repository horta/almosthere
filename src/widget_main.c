#include "athr/widget_main.h"
#include "athr/canvas.h"
#include "athr/widget.h"
#include "common.h"
#include "terminal.h"
#include "widget.h"
#include "widget_main.h"
#include <assert.h>
#include <stdlib.h>

static void update(struct athr_widget *, double, double);

static unsigned min_len(struct athr_widget const *widget)
{
    struct athr_widget_main *m = (struct athr_widget_main *)widget->derived;
    /* +1 for carriage return */
    unsigned s = 1;
    for (unsigned i = 0; i < m->nwidgets; ++i)
        s += m->children[i]->vtable->min_len(m->children[i]);
    return s;
}

static unsigned max_len(struct athr_widget const *widget)
{
    struct athr_widget_main *m = (struct athr_widget_main *)widget->derived;
    /* +1 for carriage return */
    unsigned s = 1;
    for (unsigned i = 0; i < m->nwidgets; ++i)
        s += m->children[i]->vtable->max_len(m->children[i]);
    return s;
}

static void partition(unsigned nwidgets, struct athr_widget **widget,
                      unsigned size);

static struct athr_widget_vtable const vtable = {update, min_len, max_len};

struct athr_widget_bar *widget_main_add_bar(struct athr_widget_main *m)
{
    m->children[m->nwidgets++] = (struct athr_widget *)&m->bar;
    return &m->bar;
}

struct athr_widget_eta *widget_main_add_eta(struct athr_widget_main *m)
{
    m->children[m->nwidgets++] = (struct athr_widget *)&m->eta;
    return &m->eta;
}

struct athr_widget_perc *widget_main_add_perc(struct athr_widget_main *m)
{
    m->children[m->nwidgets++] = (struct athr_widget *)&m->perc;
    return &m->perc;
}

struct athr_widget_text *widget_main_add_text(struct athr_widget_main *m)
{
    m->children[m->nwidgets++] = (struct athr_widget *)&m->text;
    return &m->text;
}

void widget_main_create(struct athr_widget_main *m)
{
    widget_setup((struct athr_widget *)m, &vtable);
    m->nwidgets = 0;
    for (unsigned i = 0; i < ATHR_WIDGET_MAIN_MAX_CHILDREN; ++i)
        m->children[i] = NULL;
    athr_canvas_create(&m->canvas);
}

void widget_main_setup(struct athr_widget_main *m)
{
    athr_canvas_setup(&m->canvas, min_len(&m->super), max_len(&m->super));
    partition(m->nwidgets, m->children, m->canvas.len - 1);
}

static void update(struct athr_widget *widget, double consumed, double speed)
{
    struct athr_widget_main *m = widget->derived;
    unsigned offset = 0;

    bool resized = athr_canvas_resize(&m->canvas);
    athr_canvas_clean(&m->canvas);
    if (resized) partition(m->nwidgets, m->children, m->canvas.len - 1);

    for (unsigned i = 0; i < m->nwidgets; ++i)
    {
        m->children[i]->canvas.buff = m->canvas.buff + offset;
        m->children[i]->vtable->update(m->children[i], consumed, speed);
        offset += m->children[i]->canvas.len;
    }
    athr_canvas_draw(&m->canvas);
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
        unsigned amount = minu(size, max_size - widget[i]->canvas.len);
        amount = minu(amount, remain);
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
