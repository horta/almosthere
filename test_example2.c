#include "athr.h"
#include <stdlib.h>

int main(void)
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 9, "My tasks", ATHR_BAR)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 9; ++i)
    {
        athr_sleep(100);
        athr_eat(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}
