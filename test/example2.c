#include "athr/athr.h"

int main()
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 9, "My tasks", ATHR_BAR)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 9; ++i)
    {
        elapsed_sleep(0.05); /* some time-consuming task */
        athr_consume(&at, 1);
    }

    elapsed_sleep(1.05); /* some time-consuming task */
    athr_stop(&at);
    return EXIT_SUCCESS;
}
