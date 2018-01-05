#include "almosthere.h"
#include "almosthere_time.h"
#include "thread/thread.h"
#include "widget/bar.h"
#include "widget/line.h"
#include <stdio.h>
#include <stdlib.h>

struct almosthere {
    long volume;                  /* total size to be consumed */
    long consumed;                /* how much have been consumed */
    double speed;                 /* current consumption speed */
    struct timespec *last_update; /* last timestamp of update call */
    struct timespec delta_start;  /* last timestamp used as delta start */
    long consumed_start; /* how much was already consumed at delta_start */

    thrd_t thr;      /* spawed thread to perfom update and draw */
    int stop_thread; /* send stop thread signal */

    struct widget *line; /* root widget */
};

void update_speed(struct almosthere *at);
int thread_start(void *args);
int create_line(struct widget **line);
void update_speed(struct almosthere *at);
void update(struct almosthere *at);

struct almosthere *almosthere_create(long volume) {

    struct almosthere *at = NULL;
    int status;

    at = malloc(sizeof(struct almosthere));

    at->volume = volume;
    at->consumed = 0;
    at->speed = 0;
    at->last_update = NULL;
    almosthere_timespec_get(&at->delta_start);
    at->consumed_start = 0;

    if (create_line(&at->line))
        goto err;

    at->stop_thread = 0;
    status = thrd_create(&at->thr, thread_start, at);
    if (status != thrd_success) {
        fprintf(stderr, "Could not spawn a thread.\n");
        goto err;
    }

    return at;
err:
    if (at != NULL)
        free(at);
    return NULL;
}

void almosthere_consume(struct almosthere *at, long consume) {
    if (at->consumed + consume > at->volume) {
        fprintf(stderr, "Trying to consume more than the total volume.\n");
        at->consumed = at->volume;
    } else
        at->consumed += consume;
}

void almosthere_finish(struct almosthere *at) {

    at->stop_thread = 1;
    thrd_join(at->thr, NULL);
    if (at->last_update != NULL)
        free(at->last_update);

    at->line->finish(at->line);
    free(at);
    fprintf(stderr, "\n");
}

int thread_start(void *args) {

    struct almosthere *at = (struct almosthere *)args;

    update(at);
    while (at->stop_thread == 0) {
        almosthere_thread_sleep(ALMOSTHERE_TIMESTEP);
        update(at);
    }

    return 0;
}

int create_line(struct widget **line) {

    struct widget **widget = malloc(1 * sizeof(struct widget *));

    widget[0] = widget_bar_create();
    *line = widget_line_create(1, widget);
    if (*line == NULL)
        return 1;
    free(widget);
    return 0;
}

void update_speed(struct almosthere *at) {
    struct timespec curr, diff;
    long consumed;
    double dlt;

    almosthere_timespec_get(&curr);
    consumed = at->consumed;
    almosthere_timespec_diff(&at->delta_start, &curr, &diff);

    dlt = almosthere_timespec_sec(&diff);

    if (dlt >= ALMOSTHERE_MIN_DLT) {
        at->speed = at->consumed_start / dlt;
        at->delta_start = curr;
        at->consumed_start = consumed;
    }
}

void update(struct almosthere *at) {

    struct timespec curr, diff;
    double dlt, consumed;

    update_speed(at);

    almosthere_timespec_get(&curr);

    if (at->last_update == NULL) {
        at->last_update = malloc(sizeof(struct timespec));
        *at->last_update = curr;
    } // This is the first update call.

    almosthere_timespec_diff(at->last_update, &curr, &diff);
    dlt = almosthere_timespec_sec(&diff);

    consumed = ((double)at->consumed) / at->volume;

    at->line->update(at->line, consumed, at->speed, dlt);

    *at->last_update = curr;
}
