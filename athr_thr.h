#ifndef ATHR_THR_H
#define ATHR_THR_H

#if ATHR_OS == ATHR_OS_WIN32
#include <windows.h>
typedef HANDLE athr_thr_handle;
#elif ATHR_OS == ATHR_OS_UNIX
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

#define ATHR_THR_INIT {0}

// PRIVATE ------------------------------------------------------------------
int  __athr_thr_create(struct athr_thr *thr, athr_thr_start *func, void *arg);
void __athr_thr_detach(struct athr_thr *thr);
int  __athr_thr_join(struct athr_thr *thr);
// PRIVATE ------------------------------------------------------------------

#endif
