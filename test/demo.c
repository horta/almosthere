#include "almosthere.h"

int main() {

    struct almosthere *at = almosthere_create(150);

    almosthere_sleep(400);
    almosthere_consume(at, 5);
    almosthere_sleep(500);
    almosthere_consume(at, 5);
    almosthere_sleep(500);
    almosthere_consume(at, 5);
    almosthere_sleep(400);
    almosthere_consume(at, 5);
    almosthere_sleep(300);
    almosthere_consume(at, 5);
    almosthere_sleep(200);
    almosthere_consume(at, 5);
    almosthere_sleep(100);
    almosthere_consume(at, 5);
    almosthere_sleep(50);
    almosthere_consume(at, 5);
    almosthere_sleep(50);
    almosthere_consume(at, 5);

    almosthere_sleep(400);
    almosthere_consume(at, 10);
    almosthere_sleep(400);
    almosthere_consume(at, 10);
    almosthere_sleep(400);
    almosthere_consume(at, 10);
    almosthere_sleep(40);
    almosthere_consume(at, 10);
    almosthere_sleep(40);
    almosthere_consume(at, 10);

    almosthere_sleep(800);
    almosthere_consume(at, 55);

    almosthere_finish(at);

    return 0;
}