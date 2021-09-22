#ifndef THR_H
#define THR_H

#include "athr/rc.h"
#include "athr/thr.h"

enum athr_rc thr_create(struct athr_thr *thr, athr_thr_start *func, void *arg);
void thr_detach(struct athr_thr *thr);
_Noreturn void thr_exit(void);
enum athr_rc thr_join(struct athr_thr *thr);

#endif
