#include "athr/widget_text.h"
#include "widget.h"
#include "widget_text.h"
#include <assert.h>
#include <string.h>

static void update(struct athr_widget *w, double consumed, double speed)
{
    struct athr_widget_text *text = w->derived;
    assert(w->canvas.len >= text->len);
    memcpy(w->canvas.buff, text->str, text->len);
}

static unsigned min_len(struct athr_widget const *w)
{
    return ((struct athr_widget_text *)w->derived)->len;
}

static unsigned max_len(struct athr_widget const *w)
{
    return ((struct athr_widget_text *)w->derived)->len;
}

static struct athr_widget_vtable const vtable = {update, min_len, max_len};

void widget_text_create(struct athr_widget_text *text, char const *str)
{
    char const *ptr = memccpy(text->str, str, '\0', ATHR_WIDGET_TEXT_MAX_LEN);
    if (ptr)
        text->len = (unsigned)(ptr - text->str - 1);
    else
        text->len = ATHR_WIDGET_TEXT_MAX_LEN;
    widget_setup((struct athr_widget *)text, &vtable);
}
