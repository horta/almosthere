#include "widget/text.h"
#include <stdlib.h>
#include <string.h>

struct text_data {
    char *str;
    int len;
};

void widget_text_update(struct widget *, double, double, double);
void widget_text_finish(struct widget *);
int widget_text_get_min_length(struct widget *);
int widget_text_get_max_length(struct widget *);

struct widget *widget_text_create(const char *str) {

    struct text_data *d = malloc(sizeof(struct text_data));
    struct widget *w = malloc(sizeof(struct widget));

    d->len = strlen(str);
    d->str = malloc(d->len * sizeof(char));

    memcpy(d->str, str, d->len);

    w->data = d;
    w->finish = widget_text_finish;
    w->update = widget_text_update;
    w->get_min_length = widget_text_get_min_length;
    w->get_max_length = widget_text_get_max_length;

    return w;
}

void widget_text_finish(struct widget *w) {
    struct text_data *d = w->data;
    free(d->str);
    free(w->data);
    free(w);
}

void widget_text_update(struct widget *w, double consumed, double speed,
                        double dlt) {
    struct text_data *d = w->data;
    int i;

    for (i = 0; i < w->canvas.length; ++i) {
        w->canvas.buff[i] = d->str[i];
    }
}

int widget_text_get_min_length(struct widget *w) {
    struct text_data *d = w->data;
    return d->len;
}
int widget_text_get_max_length(struct widget *w) {
    struct text_data *d = w->data;
    return d->len;
}
