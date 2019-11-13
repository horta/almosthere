#include "widget/line.h"
#include "canvas.h"
#include "report.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

struct line_data
{
    int             nwidgets;
    struct widget** widget;
};

static void widget_line_finish(struct widget*);
static void widget_line_update(struct widget*, double, double, double);
static int  widget_line_get_min_length(struct widget*);
static int  widget_line_get_max_length(struct widget*);
static int  check_if_fit(int nwidgets, struct widget** widget);
static int  widget_line_dist_len(int nwidgets, struct widget** widget, int length);

struct widget* widget_line_create(int nwidgets, struct widget** widget)
{
    if (nwidgets < 0)
        athr_fatal("nwidgets cannot be negative");

    if (!check_if_fit(nwidgets, widget)) {
        fprintf(stderr, "The widgets don't fit in the line widget.\n");
        return NULL;
    }

    struct widget*    w = malloc(sizeof(struct widget));
    struct line_data* line = malloc(sizeof(struct line_data));

    line->nwidgets = nwidgets;
    line->widget = malloc(((size_t)nwidgets) * sizeof(struct widget*));

    for (int i = 0; i < nwidgets; ++i) {
        line->widget[i] = widget[i];
    }

    w->data = line;
    w->finish = widget_line_finish;
    w->update = widget_line_update;
    w->get_min_length = widget_line_get_min_length;
    w->get_max_length = widget_line_get_max_length;

    athr_canvas_create(&w->canvas, widget_line_get_min_length(w));

    return w;
}

static void widget_line_update(struct widget* widget, double consumed, double speed,
                               double dlt)
{
    struct line_data* l = widget->data;
    int               base = 0;

    athr_canvas_resize(&widget->canvas);
    athr_canvas_clean(&widget->canvas);

    widget_line_dist_len(l->nwidgets, l->widget, widget->canvas.length - 1);

    for (int i = 0; i < l->nwidgets; ++i) {
        l->widget[i]->canvas.buff = widget->canvas.buff + base;

        l->widget[i]->update(l->widget[i], consumed, speed, dlt);
        base += l->widget[i]->canvas.length;
    }
    athr_canvas_draw(&widget->canvas);
}

static void widget_line_finish(struct widget* widget)
{
    struct line_data* d = widget->data;

    for (int i = 0; i < d->nwidgets; ++i) {
        d->widget[i]->finish(d->widget[i]);
    }

    free(d->widget);
    athr_canvas_finish(&widget->canvas);
    free(d);
    free(widget);
}

static int widget_line_dist_len(int nwidgets, struct widget** widget, int length)
{
    int j = 0;

    for (int i = 0; i < nwidgets; ++i) {
        int len0 = widget[i]->get_min_length(widget[i]);
        int len1 = widget[i]->get_max_length(widget[i]);
        widget[i]->canvas.length = len0;
        length -= len0;
        if (widget[i]->canvas.length < len1)
            j++;
    }

    while (j > 0 && length > 0) {
        int part = length / j;
        if (part == 0)
            break;
        for (int i = 0; i < nwidgets; ++i) {
            int len0 = widget[i]->get_max_length(widget[i]);

            if (widget[i]->canvas.length == len0)
                continue;

            int len1 = part;
            if (len1 >= len0 - widget[i]->canvas.length) {
                len1 = len0 - widget[i]->canvas.length;
                j--;
            }

            widget[i]->canvas.length += len1;
            length -= len1;
        }
    }

    for (int i = 0; i < nwidgets; ++i) {
        if (j == 0 || length == 0)
            break;

        int len0 = widget[i]->get_max_length(widget[i]);

        if (widget[i]->canvas.length == len0)
            continue;

        int len1 = length;
        if (len1 >= len0 - widget[i]->canvas.length) {
            len1 = len0 - widget[i]->canvas.length;
            j--;
        }

        widget[i]->canvas.length += len1;
        length -= len1;
    }

    return length;
}

static int widget_line_get_min_length(struct widget* widget)
{
    struct line_data* l = (struct line_data*)widget->data;
    int               s = 1;
    for (int i = 0; i < l->nwidgets; ++i)
        s += l->widget[i]->get_min_length(l->widget[i]);
    return s;
}

static int widget_line_get_max_length(struct widget* widget) { return ATHR_MAX_STR_LEN; }

static int check_if_fit(int nwidgets, struct widget** widget)
{
    int len = 0;
    for (int i = 0; i < nwidgets; ++i) {
        len += widget[i]->get_min_length(widget[i]);
    }
    return len < widget_line_get_max_length(NULL);
}
