#include "athr/widget_text.h"
#include "widget.h"
#include "widget_text.h"
#include <assert.h>
#include <string.h>

static void update(struct athr_widget *w, double consumed, double speed)
{
    (void)consumed;
    (void)speed;
    struct athr_widget_text *text = w->derived;
    assert(w->canvas.len >= text->len);
    memcpy(w->canvas.buff, text->buff, text->len);
}

static void finish(struct athr_widget *w, double total_elapsed)
{
    (void)total_elapsed;
    update(w, 1.0f, 0.0f);
}

static unsigned min_len(struct athr_widget const *w)
{
    return ((struct athr_widget_text *)w->derived)->len;
}

static unsigned max_len(struct athr_widget const *w)
{
    return ((struct athr_widget_text *)w->derived)->len;
}

static void *__memccpy(void *restrict dest, const void *restrict src, int c,
                       size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    c = (unsigned char)c;
    for (; n && (*d = *s) != c; n--, s++, d++)
        ;
    return n ? d + 1 : 0;
    if (n) return d + 1;
    return 0;
}

static struct athr_widget_vtable const vtable = {update, finish, min_len,
                                                 max_len};

void __athr_widget_text_create(struct athr_widget_text *text, char const *buff)
{
    char const *ptr = __memccpy(text->buff, buff, 0, ATHR_WIDGET_TEXT_MAX_LEN);
    if (ptr)
        text->len = (unsigned)(ptr - text->buff - 1);
    else
        text->len = ATHR_WIDGET_TEXT_MAX_LEN;
    widget_setup((struct athr_widget *)text, &vtable);
}
