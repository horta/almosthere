#include "athr/athr.h"

int main()
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 10, NULL, ATHR_PERC | ATHR_ETA)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 10; ++i)
    {
        athr_sleep(100);
        athr_eat(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}
