#ifndef ATHR_WIDGET_TEXT_H
#define ATHR_WIDGET_TEXT_H

#include "athr/widget.h"

#define ATHR_WIDGET_TEXT_MAX_SIZE 64
#define ATHR_WIDGET_TEXT_MIN_SIZE 0

struct athr_widget_text
{
    struct athr_widget super;
    char str[ATHR_WIDGET_TEXT_MAX_SIZE];
    unsigned size;
};

#define ATHR_WIDGET_TEXT_INIT                                                  \
    (struct athr_widget_text) { ATHR_WIDGET_INIT, {0}, 0 }

#endif
