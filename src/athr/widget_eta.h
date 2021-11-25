#ifndef ATHR_WIDGET_ETA_H
#define ATHR_WIDGET_ETA_H

#include "athr/widget.h"

#define ATHR_WIDGET_ETA_LEN 9

struct athr_widget_eta
{
    struct athr_widget super;
    char str[ATHR_WIDGET_ETA_LEN + 1];
    double consumed;
};

#define ATHR_WIDGET_ETA_INIT                                                   \
    (struct athr_widget_eta) { ATHR_WIDGET_INIT, {0}, 0 }

#endif
