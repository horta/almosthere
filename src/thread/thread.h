#ifndef ATHR_THREAD_H
#define ATHR_THREAD_H

#if USE_PTHREADS
#include "thread/c11threads.h"
#else
#include "thread/tinycthread.h"
#endif

#endif
