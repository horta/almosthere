#include "athr/widget/text.h"
#include "widget/text.h"
#include "widget/widget.h"
#include <assert.h>
#include <string.h>

static void update(struct athr_widget *w, double consumed, double speed,
                   double dlt)
{
    struct athr_widget_text *text = w->derived;
    assert(w->canvas.size >= text->size);
    memcpy(w->canvas.buff, text->str, text->size);
}

static unsigned min_size(struct athr_widget const *w)
{
    return ((struct athr_widget_text *)w->derived)->size;
}

static unsigned max_size(struct athr_widget const *w)
{
    return ((struct athr_widget_text *)w->derived)->size;
}

static struct athr_widget_vtable const vtable = {update, min_size, max_size};

void widget_text_create(struct athr_widget_text *text, char const *str)
{
    char const *ptr = memccpy(text->str, str, '\0', ATHR_WIDGET_TEXT_MAX_SIZE);
    if (ptr)
        text->size = (unsigned)(ptr - text->str - 1);
    else
        text->size = ATHR_WIDGET_TEXT_MAX_SIZE;
    widget_setup((struct athr_widget *)text, &vtable);
}
