#include "athr_thr.h"

#if ATHR_OS == ATHR_OS_WIN32
#define WRAPPER_RETURN DWORD WINAPI
#define WRAPPER_ARG_T LPVOID
#elif ATHR_OS == ATHR_OS_UNIX
#define WRAPPER_RETURN void *
#define WRAPPER_ARG_T void *
#endif

static WRAPPER_RETURN __thr_wrapper(WRAPPER_ARG_T arg)
{
    struct athr_thread *thr = (struct athr_thread *)arg;
    thr->func(thr->arg);

#if ATHR_OS == ATHR_OS_WIN32
    ExitThread(0);
#elif ATHR_OS == ATHR_OS_UNIX
    pthread_exit(0);
#endif

    return 0;
}

int athr_thread_create(struct athr_thread *thr, athr_thread_start *func, void *arg)
{
    thr->has_been_created = 0;
    thr->func = func;
    thr->arg = arg;
    int rc = 0;

#if ATHR_OS == ATHR_OS_WIN32
    thr->handle = CreateThread(NULL, 0, __thr_wrapper, (LPVOID)thr, 0, NULL);
    rc = !thr->handle;
#elif ATHR_OS == ATHR_OS_UNIX
    rc = pthread_create(&thr->handle, 0, __thr_wrapper, (void *)thr);
#endif
    thr->has_been_created = !rc;
    return rc;
}

void athr_thread_detach(struct athr_thread *thr)
{
    if (!thr->has_been_created) return;
#if ATHR_OS == ATHR_OS_WIN32
    CloseHandle(thr->handle);
#elif ATHR_OS == ATHR_OS_UNIX
    pthread_detach(thr->handle);
#endif
}

int athr_thread_join(struct athr_thread *thr)
{
#if ATHR_OS == ATHR_OS_WIN32
    if (WaitForSingleObject(thr, INFINITE) == WAIT_FAILED) return 1;
    CloseHandle(thr->handle);
    return 0;
#elif ATHR_OS == ATHR_OS_UNIX
    void *pres = NULL;
    return pthread_join(thr->handle, &pres);
#endif
}
