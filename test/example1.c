#include "athr/athr.h"
#include <stdlib.h>

int main()
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 14, "", ATHR_BAR)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 14; ++i)
    {
        elapsed_sleep(0.05); /* some time-consuming task */
        athr_consume(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}
