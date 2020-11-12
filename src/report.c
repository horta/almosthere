#include "report.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void athr_fatal(char const* err, ...)
{
    va_list params;
    va_start(params, err);
    fprintf(stderr, err, params);
    fputc('\n', stderr);
    va_end(params);
    exit(1);
}
