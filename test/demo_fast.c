#include "athr.h"

int main() {

    struct athr *at = athr_create(15, "Demo:");

    athr_sleep(200);
    athr_consume(at, 3);
    athr_sleep(200);
    athr_consume(at, 1);
    athr_sleep(200);
    athr_consume(at, 1);
    athr_sleep(200);
    athr_consume(at, 1);
    athr_sleep(200);
    athr_consume(at, 4);
    athr_sleep(250);
    athr_consume(at, 1);
    athr_sleep(300);
    athr_consume(at, 1);
    athr_sleep(100);
    athr_consume(at, 1);
    athr_sleep(10);
    athr_consume(at, 1);
    athr_sleep(300);
    athr_consume(at, 1);

    athr_finish(at);

    return 0;
}
