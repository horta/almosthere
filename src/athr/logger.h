#ifndef ATHR_LOGGER_H
#define ATHR_LOGGER_H

#include "athr/export.h"

typedef void athr_logger_print_t(char const *msg, void *arg);

ATHR_API void athr_logger_setup(athr_logger_print_t *print, void *arg);

#endif
