#include "athr.h"
#include <stdlib.h>

int main(void)
{
    athr_disable_threading(true);
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 10, "All", ATHR_PERC | ATHR_ETA | ATHR_BAR))
        return EXIT_FAILURE;

    for (unsigned i = 0; i < 10; i += 2)
    {
        athr_sleep(200);
        athr_eat(&at, 2);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}
