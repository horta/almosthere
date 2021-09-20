#ifndef ATHR_PERC_H
#define ATHR_PERC_H

#include "athr/widget/widget.h"

#define ATHR_WIDGET_PERC_EXAMPLE " 100%"
#define ATHR_WIDGET_PERC_SIZE (sizeof(ATHR_WIDGET_PERC_EXAMPLE) - 1)

struct athr_widget_perc
{
    struct athr_widget super;
    double consumed;
};

#define ATHR_WIDGET_PERC_INIT                                                  \
    (struct athr_widget_perc) { ATHR_WIDGET_INIT, 0 }

void widget_perc_setup(struct athr_widget_perc *);

#endif
