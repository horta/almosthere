#include "athr/athr.h"

static int example1(void)
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

int main()
{
    if (example1()) return EXIT_FAILURE;

    athr_terminal_force_fallback_use(true);
    if (example1()) return EXIT_FAILURE;

    athr_terminal_set_fallback(3);
    if (example1()) return EXIT_FAILURE;

    athr_terminal_set_fallback(2);
    if (example1()) return EXIT_FAILURE;

    athr_terminal_force_fallback_use(false);
    if (example1()) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
