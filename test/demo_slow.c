#include "athr.h"

int main() {

    struct athr *at = athr_create(150, "Demo:", ATHR_ETA);

    athr_sleep(400);
    athr_consume(at, 5);
    athr_sleep(500);
    athr_consume(at, 5);
    athr_sleep(500);
    athr_consume(at, 2);
    athr_sleep(10000);
    athr_consume(at, 1);
    athr_sleep(10000);
    athr_consume(at, 1);
    athr_sleep(10000);
    athr_consume(at, 1);
    athr_sleep(400);
    athr_consume(at, 5);
    athr_sleep(300);
    athr_consume(at, 5);
    athr_sleep(200);
    athr_consume(at, 5);
    athr_sleep(100);
    athr_consume(at, 5);
    athr_sleep(50);
    athr_consume(at, 5);
    athr_sleep(50);
    athr_consume(at, 5);

    athr_sleep(400);
    athr_consume(at, 10);
    athr_sleep(400);
    athr_consume(at, 10);
    athr_sleep(400);
    athr_consume(at, 10);
    athr_sleep(40);
    athr_consume(at, 10);
    athr_sleep(40);
    athr_consume(at, 10);
    athr_sleep(40);
    athr_consume(at, 10);
    athr_sleep(40);
    athr_consume(at, 10);
    athr_sleep(40);
    athr_consume(at, 10);
    athr_sleep(40);
    athr_consume(at, 10);

    athr_sleep(400);
    athr_consume(at, 10);

    athr_sleep(400);
    athr_consume(at, 4);

    athr_sleep(100);
    athr_consume(at, 1);

    athr_finish(at);

    return 0;
}
