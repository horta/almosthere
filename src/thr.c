#include "thr.h"

#if defined(_WIN32)
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
    return 0;
}

enum athr_rc thr_create(struct athr_thr *thr, athr_thr_start *func, void *arg)
{
    thr->func = func;
    thr->arg = arg;

#if defined(_WIN32)
    thr->handle = CreateThread(NULL, 0, __thr_wrapper, (LPVOID)thr, 0, NULL);
    if (!thr->thrd) return ATHR_FAILURE;
#else
    if (pthread_create(&thr->handle, 0, __thr_wrapper, (void *)thr))
        return ATHR_FAILURE;
#endif

    return ATHR_SUCCESS;
}

void thr_detach(struct athr_thr *thr)
{
#if defined(_WIN32)
    CloseHandle(thr->handle);
#else
    pthread_detach(thr->handle);
#endif
}

_Noreturn void thr_exit(void)
{
#if defined(_WIN32)
    ExitThread(0);
#else
    pthread_exit(0);
#endif
}
