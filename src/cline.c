#include <stdlib.h>
#include <stdio.h>
// #include <threads.h>
#include "c11threads.h"

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#endif

struct cline {
  int64_t length;
  int64_t consumed;
  int64_t drawn;
  double drawn_remainder;
  thrd_t thr;
  double speed;
  struct timespec last_used;
  int64_t acc;
};

/**
 * Cross-platform sleep function for C
 * @param int milliseconds
 */
void sleep_ms(int milliseconds) {
#ifdef WIN32
  Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  nanosleep(&ts, NULL);
#else
  usleep(milliseconds * 1000);
#endif
}

void cline_update(struct cline *line, long dlt_nsec);

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result) {
  if ((stop->tv_nsec - start->tv_nsec) < 0) {
    result->tv_sec = stop->tv_sec - start->tv_sec - 1;
    result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
  } else {
    result->tv_sec = stop->tv_sec - start->tv_sec;
    result->tv_nsec = stop->tv_nsec - start->tv_nsec;
  }

  return;
}

// TODO: ATOMIC!
void cline_update_speed(struct cline *line) {

  struct timespec curr, diff;
  timespec_get(&curr, 0);

  timespec_diff(&line->last_used, &curr, &diff);

  // printf("%ld %ld\n", diff.tv_sec, diff.tv_nsec);

  double dlt;
  double coef = 1.0 / (1000 * 1000 * 1000);
  if (line->acc > 0) {
    if (diff.tv_sec > 0 || diff.tv_nsec > 1000 * 10000) {
      dlt = diff.tv_sec;
      dlt += diff.tv_nsec * coef;
      line->speed = line->acc / dlt;
      // printf("Speed: %.30f\n", line->speed);
      line->last_used = curr;
      line->acc = 0;
    }
  }
}

int cline_thread_start(void *args) {
  struct cline *line = (struct cline *)args;

  long sleeping_delta = 1000 * 1000 * 10;

  cline_update(line, 0);
  while (line->drawn < line->length) {
    thrd_sleep(&(struct timespec){.tv_nsec = sleeping_delta}, NULL);
    cline_update_speed(line);
    cline_update(line, sleeping_delta);
  }

  return 0;
}

struct cline *cline_create(int64_t length) {

  struct cline *line = malloc(sizeof(struct cline));

  line->length = length;
  line->consumed = 0;
  line->drawn = 0;
  line->drawn_remainder = 0;
  line->speed = 0;
  timespec_get(&line->last_used, 0);
  line->acc = 0;

  thrd_create(&line->thr, cline_thread_start, line);

  return line;
}

void cline_consume(struct cline *line, int64_t consume) {
  // TODO: NEED TO BE ATOMIC!!
  line->consumed += consume;
  line->acc += consume;
}

void cline_finish(struct cline *line) {
  pthread_join(line->thr, NULL);
  free(line);
}

void cline_update(struct cline *line, long dlt_nsec) {

  int64_t i;

  double coef = 1.0 / (1000 * 1000 * 1000);
  double dlt = coef * dlt_nsec;

  long ndots = (long)(line->speed * dlt);
  double ndots_remainder = line->speed * dlt - ((double)ndots);
  double rest = ndots_remainder + line->drawn_remainder;
  ndots += (long)rest;
  rest -= (double)((long)rest);

  if (line->drawn + ndots > line->consumed)
    ndots = line->consumed - line->drawn;

  for (i = 0; i < ndots; ++i) {
    putchar('.');
  }
  fflush(stdout);

  line->drawn += ndots;
  line->drawn_remainder = rest;
  // printf("%.60f\n", line->drawn_remainder);
}

int main() {

  struct cline *line = cline_create(150);

  sleep_ms(400);
  cline_consume(line, 5);
  sleep_ms(500);
  cline_consume(line, 5);
  sleep_ms(500);
  cline_consume(line, 5);
  sleep_ms(400);
  cline_consume(line, 5);
  sleep_ms(300);
  cline_consume(line, 5);
  sleep_ms(200);
  cline_consume(line, 5);
  sleep_ms(100);
  cline_consume(line, 5);
  sleep_ms(50);
  cline_consume(line, 5);
  sleep_ms(50);
  cline_consume(line, 5);

  sleep_ms(400);
  cline_consume(line, 10);
  sleep_ms(400);
  cline_consume(line, 10);
  sleep_ms(400);
  cline_consume(line, 10);
  sleep_ms(40);
  cline_consume(line, 10);
  sleep_ms(40);
  cline_consume(line, 10);

  sleep_ms(800);
  cline_consume(line, 55);

  cline_finish(line);

  return 0;
}
