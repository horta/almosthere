#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>

unsigned __athr_terminal_width(void);
unsigned __athr_terminal_fallback_width(void);
bool __athr_terminal_force_fallback_use(void);

#endif
