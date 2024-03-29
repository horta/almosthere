#ifndef ATHR_THR_H
#define ATHR_THR_H

#include "athr/os.h"

#ifdef ATHR_WINDOWS
#include <windows.h>
typedef HANDLE athr_thr_handle;
#else
#include <pthread.h>
typedef pthread_t athr_thr_handle;
#endif

typedef void(athr_thr_start)(void *);

struct athr_thr
{
    int has_been_created;
    athr_thr_handle handle;
    athr_thr_start *func;
    void *arg;
};

#define ATHR_THR_INIT                                                          \
    {                                                                          \
        0                                                                      \
    }

#endif
