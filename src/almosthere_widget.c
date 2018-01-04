#include "almosthere_widget.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

struct line_data {
    double consumed;
};

// struct almosthere_line_widget {
//     int nwidgets;
//     void *widget;
//     void *widget_start;
//     void *widget_length;
// };

// struct almosthere_bar_widget {};

void almosthere_widget_line_create(void **data) {

    struct line_data *d = malloc(sizeof(struct line_data));
    d->consumed = -1.0;
    *data = d;
}

void almosthere_widget_line_finish(void *data) { free(data); }

void line_draw(struct line_data *data) {

    int i;
    unsigned length = almosthere_get_term_width();
    for (i = 0; i < (int)(data->consumed * length); ++i) {
        fputc('.', stderr);
    }
    fputc('\r', stderr);
    fflush(stderr);
}

void almosthere_widget_line_update(double consumed, double speed, double dlt,
                                   void *data) {
    struct line_data *d = data;
    if (d->consumed == -1) {
        // First time this update is called.
        d->consumed = consumed;
    }
    d->consumed = speed * dlt + d->consumed;
    if (d->consumed > consumed)
        d->consumed = consumed;

    line_draw(d);
}
