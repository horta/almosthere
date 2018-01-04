#include "almosthere_time.h"
#include "c11threads.h"

/**
 * Cross-platform sleep function for C
 * @param int milliseconds
 */
void almosthere_sleep(long milliseconds) {
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

void almosthere_timespec_diff(struct timespec *start, struct timespec *stop,
                              struct timespec *diff) {
  if ((stop->tv_nsec - start->tv_nsec) < 0) {
    diff->tv_sec = stop->tv_sec - start->tv_sec - 1;
    diff->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
  } else {
    diff->tv_sec = stop->tv_sec - start->tv_sec;
    diff->tv_nsec = stop->tv_nsec - start->tv_nsec;
  }

  return;
}

double almosthere_timespec_sec(struct timespec *ts) {
  return ts->tv_sec + ts->tv_nsec / 1000.0 / 1000.0 / 1000.0;
}

void almosthere_sec_timespec(double seconds, struct timespec *ts) {

  int isec = (int)seconds;
  ts->tv_sec = isec;
  double remainder = seconds - ((double)isec);
  ts->tv_nsec = remainder * 1000 * 1000 * 1000;
}

void almosthere_thread_sleep(double seconds) {

  struct timespec ts;
  almosthere_sec_timespec(seconds, &ts);
  thrd_sleep(&ts, NULL);
}
