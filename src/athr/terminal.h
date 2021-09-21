#ifndef ATHR_TERMINAL_H
#define ATHR_TERMINAL_H

#include "athr/export.h"
#include <stdbool.h>

ATHR_API void athr_terminal_set_fallback_width(unsigned width);
ATHR_API void athr_terminal_force_fallback_use(bool v);

#endif
