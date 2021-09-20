#include "athr/widget/perc.h"
#include "snprintf.h"
#include "widget/widget.h"
#include <stdio.h>
#include <stdlib.h>
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

    snprintf(eta->str, ATHR_WIDGET_PERC_LEN + 1, " %3d%%", perc);

    for (unsigned i = 0; i < ATHR_WIDGET_PERC_LEN; ++i)
    {
        w->canvas.buff[i] = eta->str[i];
    }
}

static unsigned min_length(struct athr_widget *w)
{
    return ATHR_WIDGET_PERC_LEN;
}

static unsigned max_length(struct athr_widget *w)
{
    return ATHR_WIDGET_PERC_LEN;
}

static struct athr_widget_vtable const vtable = {update, min_length,
                                                 max_length};
void widget_perc_setup(struct athr_widget_perc *perc)
{
    widget_setup((struct athr_widget *)perc, &vtable);
}
