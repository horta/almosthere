#ifndef ATHR_CANVAS_VIEW_H
#define ATHR_CANVAS_VIEW_H

struct athr_canvas_view
{
    unsigned  len;
    unsigned  min_len;
    char     *buff;
};

#define ATHR_CANVAS_VIEW_INIT                                                  \
    (struct athr_canvas_view) { 0, 0, NULL }

#endif
