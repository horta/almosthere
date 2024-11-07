#ifndef ATHR_TERMINAL_H
#define ATHR_TERMINAL_H

// PRIVATE ------------------------------------------------------------------
#include <stdbool.h>

unsigned __athr_terminal_width(void);
unsigned __athr_terminal_fallback_width(void);
bool __athr_terminal_force_fallback_use(void);

void athr_terminal_set_fallback_width(unsigned width);
void athr_terminal_force_fallback_use(bool v);
// PRIVATE ------------------------------------------------------------------

#endif
