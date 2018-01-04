#ifndef ALMOSTHERE_WIDGET_H
#define ALMOSTHERE_WIDGET_H

struct almosthere_widget {};

void almosthere_widget_create(struct almosthere_widget **widget);
void almosthere_widget_finish(struct almosthere_widget *widget);
void almosthere_widget_update(long dlt_nsec);

#endif /* end of include guard: ALMOSTHERE_WIDGET_H */
