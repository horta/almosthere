#include "almosthere.h"
#include "almosthere_time.h"
#include "almosthere_widget.h"
#include "c11threads.h"
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
    struct almosthere_widget *widget;
    thrd_t thr;
    int stop_thread;
};

void almosthere_update_speed(struct almosthere *at);

void almosthere_update(struct almosthere *at) {

    almosthere_update_speed(at);

    struct timespec curr, diff;
    timespec_get(&curr, 0);

    if (at->last_update == NULL) {
        // This is the first update call.
        at->last_update = malloc(sizeof(struct timespec));
        *at->last_update = curr;
    }

    almosthere_timespec_diff(at->last_update, &curr, &diff);
    double dlt = almosthere_timespec_sec(&diff);

    double consumed = ((double)at->consumed) / at->volume;
    almosthere_widget_update(at->widget, consumed, at->speed, dlt);

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

struct almosthere *almosthere_create(long volume) {

    struct almosthere *at = malloc(sizeof(struct almosthere));

    at->volume = volume;
    at->consumed = 0;
    at->speed = 0;
    at->last_update = NULL;
    timespec_get(&at->delta_start, 0);
    at->consumed_start = 0;

    almosthere_widget_create(&at->widget);

    at->stop_thread = 0;
    thrd_create(&at->thr, almosthere_thread_start, at);

    return at;
}

void almosthere_update_speed(struct almosthere *at) {
    // TODO: NEED TO BE ATOMIC!!
    struct timespec curr, diff;

    timespec_get(&curr, 0);
    almosthere_timespec_diff(&at->delta_start, &curr, &diff);

    double dlt = almosthere_timespec_sec(&diff);

    if (dlt >= MINIMUM_DELTA) {
        at->speed = at->consumed_start / dlt;
        at->delta_start = curr;
        at->consumed_start = at->consumed;
    }
}

void almosthere_consume(struct almosthere *at, long consume) {
    // TODO: NEED TO BE ATOMIC!!
    at->consumed += consume;
    if (at->consumed > at->volume)
        at->consumed = at->volume;
}

void almosthere_finish(struct almosthere *at) {

    at->stop_thread = 1;
    pthread_join(at->thr, NULL);
    if (at->last_update != NULL)
        free(at->last_update);
    almosthere_widget_finish(at->widget);
    free(at);
}

// void almosthere_draw(struct almosthere *at) { (at->widget)() }