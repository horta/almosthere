#include "athr_thread.h"

#if defined(ATHR_OS_WIN32)
#define WRAPPER_RETURN DWORD WINAPI
#define WRAPPER_ARG_T LPVOID
#elif defined(ATHR_OS_UNIX)
#define WRAPPER_RETURN void *
#define WRAPPER_ARG_T void *
#endif

static WRAPPER_RETURN __thr_wrapper(WRAPPER_ARG_T arg)
{
    struct athr_thread *x = (struct athr_thread *)arg;
    x->func(x->arg);

#if defined(ATHR_OS_WIN32)
    ExitThread(0);
#elif defined(ATHR_OS_UNIX)
    pthread_exit(0);
#endif

    return 0;
}

int athr_thread_create(struct athr_thread *x, athr_thread_start *func, void *arg)
{
    x->has_been_created = 0;
    x->func = func;
    x->arg = arg;
    int rc = 0;

#if defined(ATHR_OS_WIN32)
    x->handle = CreateThread(NULL, 0, __thr_wrapper, (LPVOID)x, 0, NULL);
    rc = !x->handle;
#elif defined(ATHR_OS_UNIX)
    rc = pthread_create(&x->handle, 0, __thr_wrapper, (void *)x);
#endif
    x->has_been_created = !rc;
    return rc;
}

void athr_thread_detach(struct athr_thread *x)
{
    if (!x->has_been_created) return;
#if defined(ATHR_OS_WIN32)
    CloseHandle(x->handle);
#elif defined(ATHR_OS_UNIX)
    pthread_detach(x->handle);
#endif
}

int athr_thread_join(struct athr_thread *x)
{
#if defined(ATHR_OS_WIN32)
    if (WaitForSingleObject(x, INFINITE) == WAIT_FAILED) return 1;
    CloseHandle(x->handle);
    return 0;
#elif defined(ATHR_OS_UNIX)
    void *pres = NULL;
    return pthread_join(x->handle, &pres);
#endif
}
