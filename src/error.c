#include "error.h"
#include "athr/error.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

enum athr_rc error(char *dst, char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(dst, ATHR_ERROR_SIZE, fmt, ap);
    va_end(ap);
    assert(n >= 0);
    return ATHR_FAILURE;
}
