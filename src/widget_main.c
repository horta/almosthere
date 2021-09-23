#include "athr/widget_main.h"
#include "athr/canvas.h"
#include "athr/widget.h"
#include "common.h"
#include "terminal.h"
#include "widget.h"
#include "widget_main.h"
#include <assert.h>
#include <stdlib.h>

static void update(struct athr_widget *, double, double, double);

static unsigned min_size(struct athr_widget const *widget)
{
    struct athr_widget_main *main = (struct athr_widget_main *)widget->derived;
    unsigned s = 1;
    for (unsigned i = 0; i < main->nwidgets; ++i)
        s += main->children[i]->vtable->min_length(main->children[i]);
    return s;
}

static unsigned max_size(struct athr_widget const *widget)
{
    struct athr_widget_main *main = (struct athr_widget_main *)widget->derived;
    unsigned s = 1;
    for (unsigned i = 0; i < main->nwidgets; ++i)
        s += main->children[i]->vtable->max_length(main->children[i]);
    return s;
}

static void partition(unsigned nwidgets, struct athr_widget **widget,
                      unsigned size);

static struct athr_widget_vtable const vtable = {update, min_size, max_size};

static void assert_that_fits(struct athr_widget_main const *main)
{
    unsigned size = 0;
    for (unsigned i = 0; i < main->nwidgets; ++i)
        size += main->children[i]->vtable->min_length(main->children[i]);
    assert(size < ATHR_CANVAS_MAX_SIZE);
}

struct athr_widget_bar *widget_main_add_bar(struct athr_widget_main *main)
{
    main->children[main->nwidgets++] = (struct athr_widget *)&main->bar;
    return &main->bar;
}

struct athr_widget_eta *widget_main_add_eta(struct athr_widget_main *main)
{
    main->children[main->nwidgets++] = (struct athr_widget *)&main->eta;
    return &main->eta;
}

struct athr_widget_perc *widget_main_add_perc(struct athr_widget_main *main)
{
    main->children[main->nwidgets++] = (struct athr_widget *)&main->perc;
    return &main->perc;
}

struct athr_widget_text *widget_main_add_text(struct athr_widget_main *main)
{
    main->children[main->nwidgets++] = (struct athr_widget *)&main->text;
    return &main->text;
}

void widget_main_create(struct athr_widget_main *main)
{
    widget_setup((struct athr_widget *)main, &vtable);
    main->nwidgets = 0;
    for (unsigned i = 0; i < ATHR_WIDGET_MAIN_MAX_CHILDREN; ++i)
        main->children[i] = NULL;
    athr_canvas_create(&main->canvas);
}

void widget_main_setup(struct athr_widget_main *main)
{
    athr_canvas_setup(&main->canvas, min_size(&main->super),
                      max_size(&main->super));
    assert_that_fits(main);
    partition(main->nwidgets, main->children, main->canvas.size - 1);
}

static void update(struct athr_widget *widget, double consumed, double speed,
                   double dlt)
{
    struct athr_widget_main *main = widget->derived;
    unsigned offset = 0;

    bool resized = athr_canvas_resize(&main->canvas);
    athr_canvas_clean(&main->canvas);
    if (resized)
        partition(main->nwidgets, main->children, main->canvas.size - 1);

    for (unsigned i = 0; i < main->nwidgets; ++i)
    {
        main->children[i]->canvas.buff = main->canvas.buff + offset;
        main->children[i]->vtable->update(main->children[i], consumed, speed,
                                          dlt);
        offset += main->children[i]->canvas.size;
    }
    athr_canvas_draw(&main->canvas);
}

static unsigned assign_minimum_size(unsigned nwidgets,
                                    struct athr_widget **widget, unsigned size)
{
    for (unsigned i = 0; i < nwidgets; ++i)
    {
        unsigned s = widget[i]->vtable->min_length(widget[i]);
        widget[i]->canvas.size = s;
        assert(size >= s);
        size -= s;
    }
    return size;
}

static unsigned remaining_widgets(unsigned n, struct athr_widget **widget)
{
    unsigned remaining = 0;
    for (unsigned i = 0; i < n; ++i)
    {
        unsigned s = widget[i]->vtable->max_length(widget[i]);
        if (widget[i]->canvas.size < s) remaining++;
    }
    return remaining;
}

static unsigned increase_size(unsigned nwidgets, struct athr_widget **widget,
                              unsigned size, unsigned remain)
{
    for (unsigned i = 0; i < nwidgets; ++i)
    {
        unsigned max_size = widget[i]->vtable->max_length(widget[i]);
        unsigned amount = minu(size, max_size - widget[i]->canvas.size);
        amount = minu(amount, remain);
        widget[i]->canvas.size += amount;
        remain -= amount;
    }
    return remain;
}

static void partition(unsigned nwidgets, struct athr_widget **widget,
                      unsigned size)
{
    unsigned remain = assign_minimum_size(nwidgets, widget, size);
    unsigned npart = remaining_widgets(nwidgets, widget);
    if (npart > 0)
    {
        remain = increase_size(nwidgets, widget, remain / npart, remain);
        if (remain > 0)
            remain = increase_size(nwidgets, widget, remain, remain);
    }
}
