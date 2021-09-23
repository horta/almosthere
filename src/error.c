#include "error.h"
#include "athr/athr.h"
#include <stdio.h>

void __error(struct athr *at, char const *msg)
{
    at->error = msg;
#ifndef NDEBUG
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);
#endif
}
