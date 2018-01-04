#include "almosthere.h"
#include "almosthere_time.h"
#include "almosthere_widget.h"
#include "c11threads.h"
#include <stdlib.h>

/* Minimum different in seconds considered safe for computing speed. */
static const double MINIMUM_DELTA = 0.01;

/* how often to update */
static const double TIMESTEP = 1.0 / 30.0;

struct almosthere {
  long volume;
  double consumed;
  double speed;
  struct timespec delta_start;
  double consumed_start;
  struct almosthere_widget *widget;
  thrd_t thr;
  int stop_thread;
};

void almosthere_update(struct almosthere *at, long dlt_nsec) {

  // long i;
  //
  // double dlt = coef * dlt_nsec;
  //
  // long ndots = (long)(at->speed * dlt);
  // double ndots_remainder = at->speed * dlt - ((double)ndots);
  // double rest = ndots_remainder + at->drawn_remainder;
  // ndots += (long)rest;
  // rest -= (double)((long)rest);
  //
  // if (at->drawn + ndots > at->consumed)
  //   ndots = at->consumed - at->drawn;
  //
  // for (i = 0; i < ndots; ++i) {
  //   putchar('.');
  // }
  // fflush(stdout);
  //
  // at->drawn += ndots;
  // at->drawn_remainder = rest;
  // // printf("%.60f\n", line->drawn_remainder);
}

int almosthere_thread_start(void *args) {

  // struct almosthere *at = (struct almosthere *)args;
  //
  // long sleeping_delta = 1000 * 1000 * 10;
  //
  // almosthere_update(at, 0);
  // while (at->stop_thread == 0) {
  //   almosthere_thread_sleep(&(struct timespec){.tv_nsec = sleeping_delta},
  //                           NULL);
  //   almosthere_update_speed(at);
  //   almosthere_update(at, sleeping_delta);
  // }

  return 0;
}

struct almosthere *almosthere_create(long volume) {

  struct almosthere *at = malloc(sizeof(struct almosthere));

  at->volume = volume;
  at->consumed = 0;
  at->speed = 0;
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

struct almosthere *almosthere_finish(struct almosthere *at) {

  at->stop_thread = 0;
  pthread_join(at->thr, NULL);
  almosthere_widget_create(&at->widget);
  free(at);
}

// void almosthere_draw(struct almosthere *at) { (at->widget)() }
