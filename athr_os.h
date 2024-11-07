#ifndef ATHR_OS_H
#define ATHR_OS_H

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define ATHR_WINDOWS
#else
#define ATHR_POSIX
#endif

#endif
