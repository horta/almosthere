#ifndef ATHR_CANVAS_VIEW_H
#define ATHR_CANVAS_VIEW_H

struct athr_canvas_view
{
    unsigned size;
    unsigned min_size;
    char *buff;
};

#define ATHR_CANVAS_VIEW_INIT                                                  \
    (struct athr_canvas_view) { 0, 0, NULL }

static inline void athr_canvas_view_setup(struct athr_canvas_view *view,
                                          unsigned size, unsigned min_size,
                                          char *buff)
{
    view->size = size;
    view->min_size = min_size;
    view->buff = buff;
}

#endif
