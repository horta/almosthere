#ifndef ALMOSTHERE_WIDGET_H
#define ALMOSTHERE_WIDGET_H

typedef void (*almosthere_widget_create)(void **);
typedef void (*almosthere_widget_finish)(void *);
typedef void (*almosthere_widget_update)(double, double, double, void *);

void almosthere_widget_line_create(void **);
void almosthere_widget_line_finish(void *);
void almosthere_widget_line_update(double, double, double, void *);

#endif /* end of include guard: ALMOSTHERE_WIDGET_H */
