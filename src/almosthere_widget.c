#include "almosthere_widget.h"
#include <stdio.h>
#include <stdlib.h>

void almosthere_widget_create(struct almosthere_widget **widget) {

    *widget = malloc(sizeof(struct almosthere_widget));
    (*widget)->consumed = -1.0;
    (*widget)->length = 70;
}

void almosthere_widget_finish(struct almosthere_widget *widget) {
    free(widget);
}

void almosthere_widget_draw(struct almosthere_widget *widget) {

    int i;
    for (i = 0; i < (int)(widget->consumed * widget->length); ++i) {
        putchar('.');
    }
    printf("\n");
    fflush(stdout);
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
    // long i;
    //
    // double coef = 1.0 / (1000 * 1000 * 1000);
    // double dlt = coef * dlt_nsec;
    //
    // long ndots = (long)(at->speed * dlt);
    // double ndots_remainder = at->speed * dlt - ((double)ndots);
    // double rest = ndots_remainder + at->drawn_remainder;
    // ndots += (long)rest;
    // rest -= (double)((long)rest);
    //
    // if (at->drawn + ndots > at->consumed)
    //   ndots = at->consumed - at->drawn;
    //
    // for (i = 0; i < ndots; ++i) {
    //   putchar('.');
    // }
    // fflush(stdout);
    //
    // at->drawn += ndots;
    // at->drawn_remainder = rest;
    // // printf("%.60f\n", line->drawn_remainder);
}
