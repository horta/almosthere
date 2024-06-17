#include "athr/athr.h"
#include <stdlib.h>

static int brake(char const *desc, enum athr_option opt)
{
    struct athr at = ATHR_INIT;
    if (athr_start(&at, 8, desc, opt)) return EXIT_FAILURE;

    for (unsigned i = 0; i < 8; ++i)
    {
        athr_consume(&at, 1);
    }

    athr_stop(&at);
    return EXIT_SUCCESS;
}

static char const *huge_desc = "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem."
                               "Morbi ac cursus sem.";

int main(void)
{
    athr_disable_threading(true);
    athr_term_force_fallback_use(true);

    enum athr_option options[8] = {
        ATHR_NONE,
        ATHR_BAR,
        ATHR_ETA,
        ATHR_PERC,
        ATHR_BAR | ATHR_ETA,
        ATHR_BAR | ATHR_PERC,
        ATHR_ETA | ATHR_PERC,
        ATHR_BAR | ATHR_ETA | ATHR_PERC,
    };

    for (unsigned i = 0; i < 7; ++i)
    {
        if (brake("Desc", options[i])) return EXIT_FAILURE;
    }

    for (unsigned i = 0; i < 7; ++i)
    {
        for (unsigned w = 0; w < 1024; ++w)
        {
            athr_term_set_fallback_width(w);
            if (brake("Desc", options[i])) return EXIT_FAILURE;
        }
    }

    athr_term_force_fallback_use(false);
    if (brake(huge_desc, options[7])) return EXIT_FAILURE;

    athr_term_force_fallback_use(true);
    athr_term_set_fallback_width(25);
    if (brake(huge_desc, options[7])) return EXIT_FAILURE;

    athr_term_set_fallback_width(256);
    if (brake(huge_desc, options[7])) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
