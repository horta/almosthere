#ifndef ATHR_TERMINAL_H
#define ATHR_TERMINAL_H

#include <stdbool.h>

unsigned athr_terminal_width(void);
unsigned athr_terminal_fallback_width(void);
void     athr_terminal_set_fallback_width(unsigned width);
void     athr_terminal_force_fallback_use(bool v);

#endif
