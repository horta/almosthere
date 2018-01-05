#include "almosthere.h"
#include "almosthere_time.h"
#include "thread/thread.h"
#include "widget/bar.h"
#include "widget/line.h"
#include <stdio.h>
#include <stdlib.h>

/* Minimum different in seconds considered safe for computing speed. */
static const double MINIMUM_DELTA = 0.01;

/* how often to update, in seconds */
static const double TIMESTEP = 1.0 / 30.0;

struct almosthere {
    long volume;
    long consumed;
    double speed;
    struct timespec *last_update;
    struct timespec delta_start;
    long consumed_start;

    thrd_t thr;
    int stop_thread;

    struct widget *line;
};

void almosthere_update_speed(struct almosthere *at);

void almosthere_update(struct almosthere *at) {

    struct timespec curr, diff;
    double dlt, consumed;

    almosthere_update_speed(at);

    almosthere_timespec_get(&curr);

    if (at->last_update == NULL) {
        // This is the first update call.
        at->last_update = malloc(sizeof(struct timespec));
        *at->last_update = curr;
    }

    almosthere_timespec_diff(at->last_update, &curr, &diff);
    dlt = almosthere_timespec_sec(&diff);

    consumed = ((double)at->consumed) / at->volume;

    at->line->update(at->line, consumed, at->speed, dlt);

    *at->last_update = curr;
}

int almosthere_thread_start(void *args) {

    struct almosthere *at = (struct almosthere *)args;

    almosthere_update(at);
    while (at->stop_thread == 0) {
        almosthere_thread_sleep(TIMESTEP);
        almosthere_update(at);
    }

    return 0;
}

void almosthere_create_line(struct widget **line) {

    *line = widget_line_create(0, NULL, NULL, NULL);
}

struct almosthere *almosthere_create(long volume) {

    struct almosthere *at = malloc(sizeof(struct almosthere));
    int status;

    at->volume = volume;
    at->consumed = 0;
    at->speed = 0;
    at->last_update = NULL;
    almosthere_timespec_get(&at->delta_start);
    at->consumed_start = 0;

    almosthere_create_line(&at->line);

    // struct widget **widget = malloc(1 * sizeof(struct widget *));
    // widget[0] = widget_bar_create();
    // int start[] = {0};
    // int length[] = {25};
    // at->line = widget_line_create(1, widget, start, length);

    // free(widget);

    at->stop_thread = 0;
    status = thrd_create(&at->thr, almosthere_thread_start, at);
    if (status != thrd_success) {
        printf("Could not spawn a thread.", stderr);
        return NULL;
    }

    return at;
}

void almosthere_update_speed(struct almosthere *at) {
    struct timespec curr, diff;
    long consumed;
    double dlt;

    almosthere_timespec_get(&curr);
    consumed = at->consumed;
    almosthere_timespec_diff(&at->delta_start, &curr, &diff);

    dlt = almosthere_timespec_sec(&diff);

    if (dlt >= MINIMUM_DELTA) {
        at->speed = at->consumed_start / dlt;
        at->delta_start = curr;
        at->consumed_start = consumed;
    }
}

void almosthere_consume(struct almosthere *at, long consume) {
    if (at->consumed + consume > at->volume)
        at->consumed = at->volume;
    else
        at->consumed += consume;
}

void almosthere_finish(struct almosthere *at) {

    at->stop_thread = 1;
    thrd_join(at->thr, NULL);
    if (at->last_update != NULL)
        free(at->last_update);

    at->line->finish(at->line);
    free(at);
}
