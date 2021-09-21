#ifndef ATHR_WIDGET_MAIN_H
#define ATHR_WIDGET_MAIN_H

#include "athr/canvas.h"
#include "athr/widget/bar.h"
#include "athr/widget/eta.h"
#include "athr/widget/perc.h"
#include "athr/widget/text.h"
#include "athr/widget/widget.h"

#define ATHR_WIDGET_MAIN_MAX_CHILDREN 4

struct athr_widget_main
{
    struct athr_widget super;
    struct athr_canvas canvas;
    unsigned nwidgets;
    struct athr_widget *children[ATHR_WIDGET_MAIN_MAX_CHILDREN];
    struct athr_widget_bar bar;
    struct athr_widget_eta eta;
    struct athr_widget_perc perc;
    struct athr_widget_text text;
};

#define ATHR_WIDGET_MAIN_INIT                                                  \
    (struct athr_widget_main)                                                  \
    {                                                                          \
        ATHR_WIDGET_INIT, ATHR_CANVAS_INIT, 0, {0}, ATHR_WIDGET_BAR_INIT,      \
            ATHR_WIDGET_ETA_INIT, ATHR_WIDGET_PERC_INIT, ATHR_WIDGET_TEXT_INIT \
    }

void widget_main_create(struct athr_widget_main *);
void widget_main_setup(struct athr_widget_main *);

struct athr_widget_bar *widget_main_add_bar(struct athr_widget_main *main);
struct athr_widget_eta *widget_main_add_eta(struct athr_widget_main *main);
struct athr_widget_perc *widget_main_add_perc(struct athr_widget_main *main);
struct athr_widget_text *widget_main_setup_text(struct athr_widget_main *main);

#endif
