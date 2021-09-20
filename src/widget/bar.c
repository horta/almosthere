#include "athr/widget/bar.h"
#include "widget/widget.h"

static void bar_draw(struct athr_widget_bar *bar,
                     struct athr_canvas_view *canvas)
{
    unsigned consumed = (unsigned)(bar->consumed * (canvas->size - 1));
    for (unsigned i = 1; i < consumed; ++i)
        canvas->buff[i] = '=';
    canvas->buff[0] = '|';
    canvas->buff[canvas->size - 1] = '|';
}

static void update(struct athr_widget *w, double consumed, double speed,
                   double dlt)
{
    struct athr_widget_bar *bar = w->derived;
    bar->consumed = consumed;
    bar_draw(bar, &w->canvas);
}

static unsigned min_length(struct athr_widget *widget) { return 3; }

static unsigned max_length(struct athr_widget *widget)
{
    return ATHR_MAX_STR_LEN;
}

static struct athr_widget_vtable const vtable = {update, min_length,
                                                 max_length};

void widget_bar_create(struct athr_widget_bar *bar)
{
    bar->consumed = -1.0;
    widget_setup((struct athr_widget *)bar, &vtable);
}
