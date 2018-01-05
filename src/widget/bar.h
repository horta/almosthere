#ifndef ALMOSTHERE_BAR_H_
#define ALMOSTHERE_BAR_H_

#include "widget/widget.h"

struct widget *widget_bar_create(void);
void widget_bar_finish(struct widget *);
void widget_bar_update(struct widget *, double, double, double);

#endif /* end of include guard: ALMOSTHERE_BAR_H_ */
