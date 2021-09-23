#include "athr/athr.h"

int main()
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 14, "", ATHR_BAR)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 14; ++i)
    {
        athr_sleep(100);
        athr_eat(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}
