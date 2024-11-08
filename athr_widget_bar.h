#ifndef ATHR_WIDGET_BAR_H
#define ATHR_WIDGET_BAR_H

#include "athr_canvas.h"
#include "athr_widget.h"

#define ATHR_WIDGET_BAR_SMALLEST "|=|"
#define ATHR_WIDGET_BAR_MIN_LEN (sizeof(ATHR_WIDGET_BAR_SMALLEST) - 1)
#define ATHR_WIDGET_BAR_MAX_LEN ATHR_CANVAS_MAX_LEN

struct athr_widget_bar
{
    struct athr_widget super;
    double consumed;
};

#define ATHR_WIDGET_BAR_INIT (struct athr_widget_bar){ATHR_WIDGET_INIT, 0}

void athr_widget_bar_create(struct athr_widget_bar *);

#endif
