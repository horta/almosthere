#include "athr/widget/text.h"
#include "report.h"
#include "widget/widget.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void update(struct athr_widget *w, double consumed, double speed,
                   double dlt)
{
    struct athr_widget_text *text = w->derived;

    for (unsigned i = 0; i < w->canvas.size; ++i)
        w->canvas.buff[i] = text->str[i];
}

static unsigned min_length(struct athr_widget *w)
{
    struct athr_widget_text *text = w->derived;
    return text->len;
}

static unsigned max_length(struct athr_widget *w)
{
    struct athr_widget_text *text = w->derived;
    return text->len;
}

static struct athr_widget_vtable const vtable = {update, min_length,
                                                 max_length};

void widget_text_create(struct athr_widget_text *text, char const *str)
{
    text->len = (unsigned)strlen(str);
    assert(text->len < ATHR_WIDGET_TEXT_MAX_SIZE);
    strlcpy(text->str, str, ATHR_WIDGET_TEXT_MAX_SIZE);
    widget_setup((struct athr_widget *)text, &vtable);
}
