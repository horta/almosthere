#include "athr/widget_perc.h"
#include "common.h"
#include "widget.h"
#include "widget_perc.h"
#include <assert.h>
#include <string.h>

/* Non null-terminated sequence of characters with percentage.
 * From 0% to 100%. Buff needs to have room for 4 characters. */
static void perc_buff(char *buff, unsigned perc)
{
    assert(perc <= 100);

    if (perc / 100)
        *buff = '1';
    else
        *buff = ' ';
    ++buff;

    if (perc / 10)
        *buff = (char)('0' + (perc % 100) / 10);
    else
        *buff = ' ';
    ++buff;

    *buff = (char)('0' + (perc % 100 % 10));
    ++buff;

    *buff = '%';
}

static void update(struct athr_widget *w, double consumed, double speed)
{
    struct athr_widget_perc *eta = w->derived;

    unsigned perc = 0;
    eta->consumed = consumed;

    if (eta->consumed == 1.0)
        perc = 100;
    else
        perc = (unsigned)(eta->consumed * 100);

    memcpy(w->canvas.buff, ATHR_WIDGET_PERC_EXAMPLE,
           ARRAY_SIZE(ATHR_WIDGET_PERC_EXAMPLE) - 1);
    /* Skip the first space */
    perc_buff(w->canvas.buff + 1, perc);
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
