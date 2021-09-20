#ifndef WIDGET_WIDGET_H
#define WIDGET_WIDGET_H

#include "athr/widget/widget.h"

static inline void widget_setup(struct athr_widget *widget,
                                struct athr_widget_vtable const *vtable)
{
    widget->derived = widget;
    widget->vtable = vtable;
}

#endif
