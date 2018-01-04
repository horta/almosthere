#ifndef ALMOSTHERE_WIDGET_H
#define ALMOSTHERE_WIDGET_H

struct almosthere_widget {
    double consumed;
    int length;
};

void almosthere_widget_create(struct almosthere_widget **widget);
void almosthere_widget_finish(struct almosthere_widget *widget);
void almosthere_widget_update(struct almosthere_widget *widget, double consumed,
                              double speed, double dlt);

#endif /* end of include guard: ALMOSTHERE_WIDGET_H */
