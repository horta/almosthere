#ifndef THR_H
#define THR_H

#include "athr/rc.h"
#include "athr/thr.h"

#if defined(_WIN32)
#include <processthreadsapi.h>
typedef HANDLE athr_thrd;
#else
#include <pthread.h>
typedef pthread_t athr_thrd;
#endif

typedef void(athr_thr_start)(void *);

enum athr_rc thr_create(struct athr_thr *thr, athr_thr_start *func, void *arg);
void thr_detach(struct athr_thr *thr);
_Noreturn void thr_exit(void);

#endif
