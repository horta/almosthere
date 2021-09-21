#include "athr/athr.h"

static int example1(void)
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 14, "", ATHR_BAR)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 14; ++i)
    {
        /* elapsed_sleep(0.00000001); /1* some time-consuming task *1/ */
        athr_consume(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}

int main()
{
    athr_disable_threading(true);
    if (example1()) return EXIT_FAILURE;

    athr_terminal_force_fallback_use(true);
    if (example1()) return EXIT_FAILURE;

    for (unsigned i = 0; i < 1024; ++i)
    {
        athr_terminal_set_fallback_width(i);
        if (example1()) return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
