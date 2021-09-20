#ifndef ATHR_TEXT_H
#define ATHR_TEXT_H

#include "athr/widget/widget.h"

#define ATHR_WIDGET_TEXT_MAX_SIZE 64

struct athr_widget_text
{
    struct athr_widget super;
    char str[ATHR_WIDGET_TEXT_MAX_SIZE];
    unsigned size;
};

#define ATHR_WIDGET_TEXT_INIT                                                  \
    (struct athr_widget_text) { ATHR_WIDGET_INIT, {0}, 0 }

#endif
