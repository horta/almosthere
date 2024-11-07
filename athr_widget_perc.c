#include "athr_widget_perc.h"
#include "athr_array.h"
#include "athr_widget.h"
#include "athr_widget_perc.h"
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

static void update(struct athr_widget *x, double consumed, double speed)
{
    (void)speed;
    struct athr_widget_perc *eta = x->derived;

    unsigned perc = 0;
    eta->consumed = consumed;

    if (eta->consumed == 1.0)
        perc = 100;
    else
        perc = (unsigned)(eta->consumed * 100);

    memcpy(x->canvas.buff, ATHR_WIDGET_PERC_EXAMPLE,
           athr_array_size(ATHR_WIDGET_PERC_EXAMPLE) - 1);
    /* Skip the first space */
    perc_buff(x->canvas.buff + 1, perc);
}

static void finish(struct athr_widget *x, double total_elapsed)
{
    (void)total_elapsed;
    update(x, 1.0f, 0.0f);
}

static unsigned min_len(struct athr_widget const *x)
{
    (void)x;
    return ATHR_WIDGET_PERC_LEN;
}

static unsigned max_len(struct athr_widget const *x)
{
    (void)x;
    return ATHR_WIDGET_PERC_LEN;
}

static struct athr_widget_vtable const vtable = {update, finish, min_len,
                                                 max_len};

void athr_widget_perc_create(struct athr_widget_perc *x)
{
    widget_setup((struct athr_widget *)x, &vtable);
}
