#include "widget/text.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct text_data
{
    char* str;
    int   len;
};

static void widget_text_update(struct widget*, double, double, double);
static void widget_text_finish(struct widget*);
static int  widget_text_get_min_length(struct widget*);
static int  widget_text_get_max_length(struct widget*);

struct widget* widget_text_create(const char* str)
{
    struct text_data* d = malloc(sizeof(struct text_data));
    struct widget*    w = malloc(sizeof(struct widget));

    size_t len = strlen(str);
    if (len > INT_MAX) {
        fprintf(stderr, "str is longer than INT_MAX");
        exit(1);
    }
    d->len = (int)len;
    d->str = malloc(((size_t)d->len) * sizeof(char));

    memcpy(d->str, str, (size_t)d->len);

    w->data = d;
    w->finish = widget_text_finish;
    w->update = widget_text_update;
    w->get_min_length = widget_text_get_min_length;
    w->get_max_length = widget_text_get_max_length;

    return w;
}

static void widget_text_finish(struct widget* w)
{
    struct text_data* d = w->data;
    free(d->str);
    free(w->data);
    free(w);
}

static void widget_text_update(struct widget* w, double consumed, double speed, double dlt)
{
    struct text_data* d = w->data;

    for (int i = 0; i < w->canvas.length; ++i) {
        w->canvas.buff[i] = d->str[i];
    }
}

static int widget_text_get_min_length(struct widget* w)
{
    struct text_data* d = w->data;
    return d->len;
}

static int widget_text_get_max_length(struct widget* w)
{
    struct text_data* d = w->data;
    return d->len;
}
