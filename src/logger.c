#include "logger.h"
#include "athr/logger.h"

static void null_print(char const *msg, void *arg) {}

static athr_logger_print_t *__logger_print = null_print;
static void *__logger_arg = 0;

void athr_logger_setup(athr_logger_print_t *print, void *arg)
{
    __logger_print = print;
    __logger_arg = arg;
}

static void logger_print(char const *msg) { __logger_print(msg, __logger_arg); }

void __logger_error(char const *msg) { logger_print(msg); }
