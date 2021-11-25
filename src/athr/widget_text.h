#ifndef ATHR_WIDGET_TEXT_H
#define ATHR_WIDGET_TEXT_H

#include "athr/widget.h"

#define ATHR_WIDGET_TEXT_MAX_LEN 64
#define ATHR_WIDGET_TEXT_MIN_LEN 0

struct athr_widget_text
{
    struct athr_widget super;
    char str[ATHR_WIDGET_TEXT_MAX_LEN];
    unsigned len;
};

#define ATHR_WIDGET_TEXT_INIT                                                  \
    (struct athr_widget_text) { ATHR_WIDGET_INIT, {0}, 0 }

#endif
