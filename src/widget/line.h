#ifndef ALMOSTHERE_LINE_H_
#define ALMOSTHERE_LINE_H_

#include "widget/widget.h"

struct widget *widget_line_create(int, struct widget **, int *, int *);
void widget_line_finish(struct widget *);
void widget_line_update(struct widget *, double, double, double);

#endif /* end of include guard: ALMOSTHERE_LINE_H_ */
