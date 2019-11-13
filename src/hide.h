#ifndef ATHR_HIDE_H
#define ATHR_HIDE_H

#if defined(_WIN32) || defined(__CYGWIN__)
#define HIDE
#else
#define HIDE __attribute__((visibility("hidden")))
#endif

#endif
