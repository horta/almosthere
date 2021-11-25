#include "athr/widget_bar.h"
#include "athr/canvas.h"
#include "widget.h"
#include "widget_bar.h"
#include <string.h>

static void draw(struct athr_widget_bar *bar, struct athr_canvas_view *canvas)
{
    unsigned consumed = (unsigned)(bar->consumed * (canvas->size - 1));
    canvas->buff[0] = '|';
    for (unsigned i = 1; i < consumed; ++i)
        canvas->buff[i] = '=';
    canvas->buff[canvas->size - 1] = '|';
}

static void update(struct athr_widget *w, double consumed, double speed)
{
    struct athr_widget_bar *bar = w->derived;
    bar->consumed = consumed;
    draw(bar, &w->canvas);
}

static unsigned min_size(struct athr_widget const *widget)
{
    return ATHR_WIDGET_BAR_MIN_SIZE;
}

static unsigned max_size(struct athr_widget const *widget)
{
    return ATHR_CANVAS_MAX_SIZE;
}

static struct athr_widget_vtable const vtable = {update, min_size, max_size};

void widget_bar_create(struct athr_widget_bar *bar)
{
    widget_setup((struct athr_widget *)bar, &vtable);
    bar->consumed = 0;
}
