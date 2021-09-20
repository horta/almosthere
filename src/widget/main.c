#include "athr/widget/main.h"
#include "athr/canvas.h"
#include "athr/widget/widget.h"
#include "report.h"
#include "terminal/terminal.h"
#include "widget/widget.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void update(struct athr_widget *, double, double, double);

static unsigned min_length(struct athr_widget *widget)
{
    struct athr_widget_main *main = (struct athr_widget_main *)widget->derived;
    unsigned s = 1;
    for (unsigned i = 0; i < main->nwidgets; ++i)
        s += main->children[i]->vtable->min_length(main->children[i]);
    return s;
}

static unsigned max_length(struct athr_widget *widget)
{
    return ATHR_MAX_STR_LEN;
}

static unsigned widget_line_dist_len(unsigned nwidgets,
                                     struct athr_widget **widget,
                                     unsigned length);

static struct athr_widget_vtable const vtable = {update, min_length,
                                                 max_length};

void widget_main_assert_that_fits(struct athr_widget_main const *main)
{
    unsigned len = 0;
    for (unsigned i = 0; i < main->nwidgets; ++i)
        len += main->children[i]->vtable->min_length(main->children[i]);
    assert(len < max_length(NULL));
}

struct athr_widget_bar *widget_main_setup_bar(struct athr_widget_main *main)
{
    assert(main->nwidgets < 4);
    main->children[main->nwidgets++] = (struct athr_widget *)&main->bar;
    return &main->bar;
}

struct athr_widget_eta *widget_main_setup_eta(struct athr_widget_main *main)
{
    assert(main->nwidgets < 4);
    main->children[main->nwidgets++] = (struct athr_widget *)&main->eta;
    return &main->eta;
}

struct athr_widget_perc *widget_main_setup_perc(struct athr_widget_main *main)
{
    assert(main->nwidgets < 4);
    main->children[main->nwidgets++] = (struct athr_widget *)&main->perc;
    return &main->perc;
}

struct athr_widget_text *widget_main_setup_text(struct athr_widget_main *main)
{
    assert(main->nwidgets < 4);
    main->children[main->nwidgets++] = (struct athr_widget *)&main->text;
    return &main->text;
}

void widget_main_setup(struct athr_widget_main *main)
{
    main->nwidgets = 0;
    widget_setup((struct athr_widget *)main, &vtable);
    athr_canvas_create(&main->canvas, min_length(&main->super));
}

static void update(struct athr_widget *widget, double consumed, double speed,
                   double dlt)
{
    struct athr_widget_main *main = widget->derived;
    unsigned offset = 0;

    athr_canvas_resize(&main->canvas);
    athr_canvas_clean(&main->canvas);

    widget_line_dist_len(main->nwidgets, main->children, main->canvas.size - 1);

    for (unsigned i = 0; i < main->nwidgets; ++i)
    {
        main->children[i]->canvas.buff = main->canvas.buff + offset;
        main->children[i]->vtable->update(main->children[i], consumed, speed,
                                          dlt);
        offset += main->children[i]->canvas.size;
    }
    athr_canvas_draw(&main->canvas);
}

static unsigned widget_line_dist_len(unsigned nwidgets,
                                     struct athr_widget **widget,
                                     unsigned length)
{
    unsigned j = 0;

    for (unsigned i = 0; i < nwidgets; ++i)
    {
        unsigned len0 = widget[i]->vtable->min_length(widget[i]);
        unsigned len1 = widget[i]->vtable->max_length(widget[i]);
        widget[i]->canvas.size = len0;
        length -= len0;
        if (widget[i]->canvas.size < len1) j++;
    }

    while (j > 0 && length > 0)
    {
        unsigned part = length / j;
        if (part == 0) break;
        for (unsigned i = 0; i < nwidgets; ++i)
        {
            unsigned len0 = widget[i]->vtable->max_length(widget[i]);

            if (widget[i]->canvas.size == len0) continue;

            unsigned len1 = part;
            if (len1 >= len0 - widget[i]->canvas.size)
            {
                len1 = len0 - widget[i]->canvas.size;
                j--;
            }

            widget[i]->canvas.size += len1;
            length -= len1;
        }
    }

    for (unsigned i = 0; i < nwidgets; ++i)
    {
        if (j == 0 || length == 0) break;

        unsigned len0 = widget[i]->vtable->max_length(widget[i]);

        if (widget[i]->canvas.size == len0) continue;

        unsigned len1 = length;
        if (len1 >= len0 - widget[i]->canvas.size)
        {
            len1 = len0 - widget[i]->canvas.size;
            j--;
        }

        widget[i]->canvas.size += len1;
        length -= len1;
    }

    return length;
}
