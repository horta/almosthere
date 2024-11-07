#include "athr_widget_bar.h"
#include "athr_widget.h"
#include "athr_widget_bar.h"
#include <string.h>

static void draw(struct athr_widget_bar *x, struct athr_canvas_view *canvas)
{
    unsigned consumed = (unsigned)(x->consumed * (canvas->len - 1));
    canvas->buff[0] = '|';
    for (unsigned i = 1; i < consumed; ++i)
        canvas->buff[i] = '=';
    canvas->buff[canvas->len - 1] = '|';
}

static void update(struct athr_widget *x, double consumed, double speed)
{
    (void)speed;
    struct athr_widget_bar *bar = x->derived;
    bar->consumed = consumed;
    draw(bar, &x->canvas);
}

static void finish(struct athr_widget *x, double total_elapsed)
{
    (void)total_elapsed;
    update(x, 1.0f, 0.0f);
}

static unsigned min_len(struct athr_widget const *x)
{
    (void)x;
    return ATHR_WIDGET_BAR_MIN_LEN;
}

static unsigned max_len(struct athr_widget const *x)
{
    (void)x;
    return ATHR_WIDGET_BAR_MAX_LEN;
}

static struct athr_widget_vtable const vtable = {update, finish, min_len,
                                                 max_len};

void athr_widget_bar_create(struct athr_widget_bar *x)
{
    widget_setup((struct athr_widget *)x, &vtable);
    x->consumed = 0;
}
