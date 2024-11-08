#include "athr_canvas.h"
#include "athr_max.h"
#include "athr_min.h"
#include "athr_terminal.h"
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static atomic_bool use_stderr = false;

void athr_canvas_use_stderr(bool use) { atomic_store(&use_stderr, use); }

void athr_canvas_create(struct athr_canvas *x)
{
    x->len     = 0;
    x->min_len = 0;
    x->max_len = 0;
}

void athr_canvas_draw(struct athr_canvas const *x)
{
    if (use_stderr)
    {
        fprintf(stderr, "%.*s", x->len, x->buff);
        fflush(stderr);
    }
    else
    {
        fprintf(stdout, "%.*s", x->len, x->buff);
        fflush(stdout);
    }
}

bool athr_canvas_resize(struct athr_canvas *x)
{
    unsigned ncols    = athr_terminal_width() + 1;
    unsigned prev_len = x->len;
    x->len       = ncols;
    x->len       = athr_min(x->len, x->max_len);
    x->len       = athr_max(x->len, x->min_len);
    return prev_len != x->len;
}

void athr_canvas_setup(struct athr_canvas *x, unsigned min_len,
                       unsigned max_len)
{
    x->min_len = min_len;
    x->max_len = athr_min(max_len, ATHR_CANVAS_MAX_LEN);
}

void athr_canvas_clean(struct athr_canvas *x)
{
    memset(x->buff, ' ', x->len - 1);
    x->buff[x->len - 1] = '\r';
}

void athr_canvas_close(struct athr_canvas *x)
{
    (void)x;
    if (use_stderr)
    {
        fprintf(stderr, "\n");
        fflush(stderr);
    }
    else
    {
        fprintf(stdout, "\n");
        fflush(stdout);
    }
}
