#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>

unsigned terminal_width(void);
unsigned terminal_fallback_width(void);
bool terminal_force_fallback_use(void);

#endif
