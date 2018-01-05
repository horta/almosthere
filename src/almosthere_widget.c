#include "almosthere_widget.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

// struct line_widget {
//     int nwidgets;
//     almosthere_widget *widget;
//     void *widget_data;
//     int *start;
//     int *length;
// };
//
// void *almosthere_widget_line_create(int nwidgets, almosthere_widget *widget,
//                                     int *start, int *length) {
//
//     struct line_widget *w;
//     int i;
//
//     w = malloc(sizeof(struct line_widget));
//     w->nwidgets = nwidgets;
//     w->widget = malloc(nwidgets * sizeof(struct almosthere_widget));
//     w->start = malloc(nwidgets * sizeof(int));
//     w->length = malloc(nwidgets * sizeof(int));
//
//     for (i = 0; i < nwidgets; ++i) {
//         w->widget[i] = widget[i];
//         w->start[i] = start[i];
//         w->length[i] = length[i];
//     }
//
//     return w;
// }
//
// void almosthere_widget_line_finish(void *) {}
//
// void almosthere_widget_line_update(double, double, double, void *) {}

struct bar_data {
    double consumed;
};

struct almosthere_widget *almosthere_widget_bar_create(void) {

    struct bar_data *d = malloc(sizeof(struct bar_data));
    struct almosthere_widget *w = malloc(sizeof(struct almosthere_widget));
    d->consumed = -1.0;
    w->data = d;
    w->finish = almosthere_widget_bar_finish;
    w->update = almosthere_widget_bar_update;
    return w;
}

void almosthere_widget_bar_finish(struct almosthere_widget *w) {
    free(w->data);
    free(w);
}

void bar_draw(struct bar_data *data) {

    int i;
    unsigned length = almosthere_get_term_width();
    for (i = 0; i < (int)(data->consumed * length); ++i) {
        fputc('.', stderr);
    }
    fputc('\r', stderr);
    fflush(stderr);
}

void almosthere_widget_bar_update(struct almosthere_widget *w, double consumed,
                                  double speed, double dlt) {
    struct bar_data *d = w->data;
    if (d->consumed == -1) {
        // First time this update is called.
        d->consumed = consumed;
    }
    d->consumed = speed * dlt + d->consumed;
    if (d->consumed > consumed)
        d->consumed = consumed;

    bar_draw(d);
}
