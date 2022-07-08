#ifndef THR_H
#define THR_H

#include "athr/thr.h"

int __athr_thr_create(struct athr_thr *thr, athr_thr_start *func, void *arg);
void __athr_thr_detach(struct athr_thr *thr);
int __athr_thr_join(struct athr_thr *thr);

#endif
