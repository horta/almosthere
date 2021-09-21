#ifndef ATHR_WIDGET_BAR_H
#define ATHR_WIDGET_BAR_H

#include "athr/widget/widget.h"

#define ATHR_WIDGET_BAR_SMALLEST "|=|"
#define ATHR_WIDGET_BAR_MIN_SIZE (sizeof(ATHR_WIDGET_BAR_SMALLEST) - 1)

struct athr_widget_bar
{
    struct athr_widget super;
    double consumed;
};

#define ATHR_WIDGET_BAR_INIT                                                   \
    (struct athr_widget_bar) { ATHR_WIDGET_INIT, 0 }

#endif
