#include "athr/widget/perc.h"
#include "common.h"
#include "widget/widget.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void update(struct athr_widget *w, double consumed, double speed,
                   double dlt)
{
    struct athr_widget_perc *eta = w->derived;

    unsigned perc = 0;
    eta->consumed = consumed;

    if (eta->consumed == 1.0)
        perc = 100;
    else
        perc = (unsigned)(eta->consumed * 100);

    char str[ATHR_WIDGET_PERC_SIZE + 1] = {0};
    int n = snprintf(str, ATHR_WIDGET_PERC_SIZE + 1, " %3d%%", perc);
    assert(n == ATHR_WIDGET_PERC_SIZE);
    UNUSED(n);

    for (unsigned i = 0; i < ATHR_WIDGET_PERC_SIZE; ++i)
        w->canvas.buff[i] = eta->str[i];

    memcpy(w->canvas.buff, eta->str, ATHR_WIDGET_PERC_SIZE);
}

static unsigned min_size(struct athr_widget const *w)
{
    return ATHR_WIDGET_PERC_SIZE;
}

static unsigned max_size(struct athr_widget const *w)
{
    return ATHR_WIDGET_PERC_SIZE;
}

static struct athr_widget_vtable const vtable = {update, min_size, max_size};

void widget_perc_setup(struct athr_widget_perc *perc)
{
    widget_setup((struct athr_widget *)perc, &vtable);
}
