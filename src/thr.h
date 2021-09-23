#ifndef THR_H
#define THR_H

#include "athr/thr.h"

int thr_create(struct athr_thr *thr, athr_thr_start *func, void *arg);
void thr_detach(struct athr_thr *thr);
int thr_join(struct athr_thr *thr);

#endif
