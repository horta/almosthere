#include "almosthere_widget.h"

void almosthere_widget_create(struct almosthere_widget **widget) {

  *widget = malloc(sizeof(struct almosthere_widget_bar));
  // (*widget)->drawn = 0;
}

void almosthere_widget_create(struct almosthere_widget *widget) {
  free(widget);
}

// void cline_update(struct cline *line, long dlt_nsec) {
//
//   int64_t i;
//
//   double coef = 1.0 / (1000 * 1000 * 1000);
//   double dlt = coef * dlt_nsec;
//
//   long ndots = (long)(line->speed * dlt);
//   double ndots_remainder = line->speed * dlt - ((double)ndots);
//   double rest = ndots_remainder + line->drawn_remainder;
//   ndots += (long)rest;
//   rest -= (double)((long)rest);
//
//   if (line->drawn + ndots > line->consumed)
//     ndots = line->consumed - line->drawn;
//
//   for (i = 0; i < ndots; ++i) {
//     putchar('.');
//   }
//   fflush(stdout);
//
//   line->drawn += ndots;
//   line->drawn_remainder = rest;
//   // printf("%.60f\n", line->drawn_remainder);
// }
