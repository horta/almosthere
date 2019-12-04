#include "athr.h"

int main() {
    struct athr *at = athr_create(10, .opts = ATHR_PERC);
    int i;

    for (i = 0; i < 10; ++i) {
        athr_sleep(50); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}
