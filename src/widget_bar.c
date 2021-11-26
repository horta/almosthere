#include "athr/widget_bar.h"
#include "athr/canvas.h"
#include "widget.h"
#include "widget_bar.h"
#include <string.h>

static void draw(struct athr_widget_bar *bar, struct athr_canvas_view *canvas)
{
    unsigned consumed = (unsigned)(bar->consumed * (canvas->len - 1));
    canvas->buff[0] = '|';
    for (unsigned i = 1; i < consumed; ++i)
        canvas->buff[i] = '=';
    canvas->buff[canvas->len - 1] = '|';
}

static void update(struct athr_widget *w, double consumed, double speed)
{
    struct athr_widget_bar *bar = w->derived;
    bar->consumed = consumed;
    draw(bar, &w->canvas);
}

static void finish(struct athr_widget *w, double total_elapsed)
{
    update(w, 1.0f, 0.0f);
}

static unsigned min_len(struct athr_widget const *widget)
{
    return ATHR_WIDGET_BAR_MIN_LEN;
}

static unsigned max_len(struct athr_widget const *widget)
{
    return ATHR_WIDGET_BAR_MAX_LEN;
}

static struct athr_widget_vtable const vtable = {update, finish, min_len,
                                                 max_len};

void widget_bar_create(struct athr_widget_bar *bar)
{
    widget_setup((struct athr_widget *)bar, &vtable);
    bar->consumed = 0;
}
