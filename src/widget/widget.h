#ifndef ALMOSTHERE_WIDGET_H_
#define ALMOSTHERE_WIDGET_H_

struct widget;

typedef void (*widget_finish)(struct widget *);
typedef void (*widget_update)(struct widget *, double, double,
                                         double);

struct canvas {
    char *buff;
    int length;
};

struct widget {
    widget_finish finish;
    widget_update update;
    void *data;
    struct canvas canvas;
};

#endif /* end of include guard: ALMOSTHERE_WIDGET_H_ */
