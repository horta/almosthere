#include "athr/athr.h"

int main()
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 11, "My tasks", ATHR_PERC)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 11; ++i)
    {
        elapsed_sleep(0.005); /* some time-consuming task */
        athr_consume(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}
