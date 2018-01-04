#include "almosthere_widget.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>

void almosthere_widget_create(struct almosthere_widget **widget) {

    *widget = malloc(sizeof(struct almosthere_widget));
    (*widget)->consumed = -1.0;
    (*widget)->length = almosthere_get_term_width();
}

void almosthere_widget_finish(struct almosthere_widget *widget) {
    free(widget);
}

void almosthere_widget_draw(struct almosthere_widget *widget) {

    int i;
    for (i = 0; i < (int)(widget->consumed * widget->length); ++i) {
        fputc('.', stderr);
    }
    fputc('\r', stderr);
    fflush(stderr);
}

void almosthere_widget_update(struct almosthere_widget *widget, double consumed,
                              double speed, double dlt) {

    if (widget->consumed == -1) {
        // First time this update is called.
        widget->consumed = consumed;
    }
    widget->consumed = speed * dlt + widget->consumed;
    if (widget->consumed > consumed)
        widget->consumed = consumed;

    almosthere_widget_draw(widget);
}
