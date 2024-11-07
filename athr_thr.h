#ifndef ATHR_THR_H
#define ATHR_THR_H

#if ATHR_OS == ATHR_OS_WIN32
#include <windows.h>
typedef HANDLE athr_thr_handle;
#elif ATHR_OS == ATHR_OS_UNIX
#include <pthread.h>
typedef pthread_t athr_thr_handle;
#endif

typedef void(athr_thread_start)(void *);

struct athr_thread
{
    int                has_been_created;
    athr_thr_handle    handle;
    athr_thread_start *func;
    void              *arg;
};

#define ATHR_THR_INIT {0}

int  athr_thread_create(struct athr_thread *, athr_thread_start *, void *arg);
void athr_thread_detach(struct athr_thread *);
int  athr_thread_join(struct athr_thread *);

#endif
