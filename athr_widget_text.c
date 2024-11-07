#include "athr_widget_text.h"
#include "athr_widget.h"
#include "athr_widget_text.h"
#include <assert.h>
#include <string.h>

static void update(struct athr_widget *x, double consumed, double speed)
{
    (void)consumed;
    (void)speed;
    struct athr_widget_text *text = x->derived;
    assert(x->canvas.len >= text->len);
    memcpy(x->canvas.buff, text->buff, text->len);
}

static void finish(struct athr_widget *x, double total_elapsed)
{
    (void)total_elapsed;
    update(x, 1.0f, 0.0f);
}

static unsigned min_len(struct athr_widget const *x)
{
    return ((struct athr_widget_text *)x->derived)->len;
}

static unsigned max_len(struct athr_widget const *x)
{
    return ((struct athr_widget_text *)x->derived)->len;
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

void athr_widget_text_create(struct athr_widget_text *x, char const *buff)
{
    char const *ptr = __memccpy(x->buff, buff, 0, ATHR_WIDGET_TEXT_MAX_LEN);
    if (ptr)
        x->len = (unsigned)(ptr - x->buff - 1);
    else
        x->len = ATHR_WIDGET_TEXT_MAX_LEN;
    widget_setup((struct athr_widget *)x, &vtable);
}
