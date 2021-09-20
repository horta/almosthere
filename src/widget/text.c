#include "athr/widget/text.h"
#include "widget/widget.h"
#include <assert.h>
#include <string.h>

static void update(struct athr_widget *w, double consumed, double speed,
                   double dlt)
{
    struct athr_widget_text *text = w->derived;
    memcpy(w->canvas.buff, text->str, w->canvas.size);
}

static unsigned min_size(struct athr_widget const *w)
{
    struct athr_widget_text *text = w->derived;
    return text->size;
}

static unsigned max_size(struct athr_widget const *w)
{
    struct athr_widget_text *text = w->derived;
    return text->size;
}

static struct athr_widget_vtable const vtable = {update, min_size, max_size};

void widget_text_create(struct athr_widget_text *text, char const *str)
{
    text->size = (unsigned)strlen(str);
    assert(text->size <= ATHR_WIDGET_TEXT_MAX_SIZE);
    memcpy(text->str, str, text->size);
    widget_setup((struct athr_widget *)text, &vtable);
}
