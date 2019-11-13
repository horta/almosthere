#ifndef ATHR_REPORT_H
#define ATHR_REPORT_H

#if defined(HAVE_ATTR_FORMAT)
#define ATTR_FORMAT __attribute__((format(printf, 1, 2)))
#else
#define ATTR_FORMAT
#endif

void athr_fatal(char const* err, ...) ATTR_FORMAT;

#endif
