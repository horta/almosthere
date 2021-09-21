#ifndef ATHR_WIDGET_TOTAL_H
#define ATHR_WIDGET_TOTAL_H

#include "athr/widget/bar.h"
#include "athr/widget/eta.h"
#include "athr/widget/perc.h"
#include "athr/widget/text.h"

#define ATHR_WIDGET_TOTAL_MIN_SIZE                                             \
    (ATHR_WIDGET_BAR_MIN_SIZE + ATHR_WIDGET_ETA_SIZE + ATHR_WIDGET_PERC_SIZE + \
     ATHR_WIDGET_TEXT_MIN_SIZE)

#endif
