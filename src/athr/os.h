#ifndef ATHR_OS_H
#define ATHR_OS_H

/* Source: https://tinycthread.github.io */
#if !defined(ATHR_PLATFORM_DEFINED)
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define ATHR_WINDOWS
#else
#define ATHR_POSIX
#endif
#define ATHR_PLATFORM_DEFINED
#endif

#endif
