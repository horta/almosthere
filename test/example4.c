#include "athr/athr.h"

int main()
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 10, NULL, ATHR_PERC | ATHR_ETA)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 10; ++i)
    {
        elapsed_sleep(0.005); /* some time-consuming task */
        athr_eat(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}
