#ifndef ATHR_REPORT_H
#define ATHR_REPORT_H

#include <stdarg.h>

void athr_fatal(char const* err, ...) __attribute__((format(printf, 1, 2)));

#endif
