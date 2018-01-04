#include "almosthere_widget.h"
#include <stdlib.h>

void almosthere_widget_create(struct almosthere_widget **widget) {

  *widget = malloc(sizeof(struct almosthere_widget));
  // (*widget)->drawn = 0;
}

void almosthere_widget_finish(struct almosthere_widget *widget) {
  free(widget);
}

void almosthere_widget_update(long dlt_nsec) {

  // long i;
  //
  // double coef = 1.0 / (1000 * 1000 * 1000);
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
