#ifndef ALMOSTHERE_WIDGET_H
#define ALMOSTHERE_WIDGET_H

struct almosthere_widget;

typedef void (*almosthere_widget_finish)(struct almosthere_widget *);
typedef void (*almosthere_widget_update)(struct almosthere_widget *, double,
                                         double, double);

struct almosthere_widget {
    almosthere_widget_finish finish;
    almosthere_widget_update update;
    void *data;
};

// void *almosthere_widget_line_create(int, void *, int *, int *);
// void almosthere_widget_line_finish(void *);
// void almosthere_widget_line_update(double, double, double, void *);

struct almosthere_widget *almosthere_widget_bar_create(void);
void almosthere_widget_bar_finish(struct almosthere_widget *);
void almosthere_widget_bar_update(struct almosthere_widget *, double, double,
                                  double);

#endif /* end of include guard: ALMOSTHERE_WIDGET_H */
