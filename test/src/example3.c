#include "athr/athr.h"

int main()
{
    struct athr* at = athr_create(11, "My tasks", ATHR_PERC);
    int          i;

    for (i = 0; i < 11; ++i) {
        athr_sleep(50); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}
