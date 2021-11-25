#include "athr/widget_perc.h"
#include "common.h"
#include "widget.h"
#include "widget_perc.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void update(struct athr_widget *w, double consumed, double speed)
{
    struct athr_widget_perc *eta = w->derived;

    unsigned perc = 0;
    eta->consumed = consumed;

    if (eta->consumed == 1.0)
        perc = 100;
    else
        perc = (unsigned)(eta->consumed * 100);

    char str[ATHR_WIDGET_PERC_LEN + 1] = {0};
    int n = snprintf(str, ATHR_WIDGET_PERC_LEN + 1, " %3d%%", perc);
    assert(n == ATHR_WIDGET_PERC_LEN);
    UNUSED(n);
    memcpy(w->canvas.buff, str, ATHR_WIDGET_PERC_LEN);
}

static unsigned min_len(struct athr_widget const *w)
{
    return ATHR_WIDGET_PERC_LEN;
}

static unsigned max_len(struct athr_widget const *w)
{
    return ATHR_WIDGET_PERC_LEN;
}

static struct athr_widget_vtable const vtable = {update, min_len, max_len};

void widget_perc_create(struct athr_widget_perc *perc)
{
    widget_setup((struct athr_widget *)perc, &vtable);
}
