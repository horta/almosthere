#ifndef ATHR_THREAD_H_
#define ATHR_THREAD_H_

#if WIN32
#include "thread/tinycthread.h"
#else
#include "thread/c11threads.h"
#endif

#endif /* end of include guard: ATHR_THREAD_H_ */
