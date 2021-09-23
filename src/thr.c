#include "thr.h"
#include "athr/os.h"

#ifdef ATHR_WINDOWS
#define WRAPPER_RETURN DWORD WINAPI
#define WRAPPER_ARG_T LPVOID
#else
#define WRAPPER_RETURN void *
#define WRAPPER_ARG_T void *
#endif

static WRAPPER_RETURN __thr_wrapper(WRAPPER_ARG_T arg)
{
    struct athr_thr *thr = (struct athr_thr *)arg;
    thr->func(thr->arg);

#ifdef ATHR_WINDOWS
    ExitThread(0);
#else
    pthread_exit(0);
#endif

    return 0;
}

int thr_create(struct athr_thr *thr, athr_thr_start *func, void *arg)
{
    thr->func = func;
    thr->arg = arg;

#ifdef ATHR_WINDOWS
    thr->handle = CreateThread(NULL, 0, __thr_wrapper, (LPVOID)thr, 0, NULL);
    return !thr->handle;
#else
    return pthread_create(&thr->handle, 0, __thr_wrapper, (void *)thr);
#endif
}

void thr_detach(struct athr_thr *thr)
{
#if ATHR_WINDOWS
    CloseHandle(thr->handle);
#else
    pthread_detach(thr->handle);
#endif
}

int thr_join(struct athr_thr *thr)
{
#ifdef ATHR_WINDOWS
    if (WaitForSingleObject(thr, INFINITE) == WAIT_FAILED) return 1;
    CloseHandle(thr->handle);
    return 0;
#else
    void *pres = NULL;
    return pthread_join(thr->handle, &pres);
#endif
}
