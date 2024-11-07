#ifndef ATHR_LOGGER_H
#define ATHR_LOGGER_H

#include "athr_stringify.h"

typedef void athr_logger_print_t(char const *msg, void *arg);

void athr_logger_setup(athr_logger_print_t *, void *arg);
void athr_logger_error(char const *msg);

#define athr_logger_format(msg) __FILE__ ":" athr_stringify(__LINE__) ": " msg
#define error(msg) athr_logger_error(athr_logger_format(msg))

#endif
