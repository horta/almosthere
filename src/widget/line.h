#ifndef ALMOSTHERE_LINE_H_
#define ALMOSTHERE_LINE_H_

#include "widget/widget.h"

struct widget *widget_line_create(int, struct widget **);
void widget_line_finish(struct widget *);
void widget_line_update(struct widget *, double, double, double);
int widget_line_get_min_length(struct widget *);
int widget_line_get_max_length(struct widget *);

#endif /* end of include guard: ALMOSTHERE_LINE_H_ */
