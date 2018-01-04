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

        printf("Consumed: %.30f\n", widget->consumed);
        fflush(stdout);
    int i;
    for (i = 0; i < (int)(widget->consumed * widget->length); ++i) {
        printf("ldqkdkqlw\n");
        fflush(stdout);
        fputc('.', stderr);
        fputc('.', stdout);
    }
    //fputc('\r', stderr);
    //fputc('\n', stderr);
    fflush(stderr);
    fflush(stdout);
}

void almosthere_widget_update(struct almosthere_widget *widget, double consumed,
                              double speed, double dlt) {

    if (widget->consumed == -1) {
        // First time this update is called.
        widget->consumed = consumed;
    }
    printf("Speed: %.30f\n", speed);
    widget->consumed = speed * dlt + widget->consumed;
    if (widget->consumed > consumed)
        widget->consumed = consumed;

    almosthere_widget_draw(widget);
}
