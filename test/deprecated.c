#include "athr/athr.h"

int test_example1(void);
int test_example2(void);
int test_example3(void);
int test_example4(void);

int main(void)
{
    return test_example1() | test_example2() | test_example3() |
           test_example4();
}

int test_example1(void)
{
    struct athr *at = athr_create(14, 0, 0);
    int i;

    for (i = 0; i < 14; ++i)
    {
        athr_sleep(10); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}

int test_example2(void)
{
    struct athr *at = athr_create(9, "My tasks", 0);
    int i;

    for (i = 0; i < 9; ++i)
    {
        athr_sleep(10); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}

int test_example3(void)
{
    struct athr *at = athr_create(11, "My tasks", ATHR_PERC_DEPRECATED);
    int i;

    for (i = 0; i < 11; ++i)
    {
        athr_sleep(10); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}

int test_example4(void)
{
    struct athr *at = athr_create(10, .opts = ATHR_PERC_DEPRECATED);
    int i;

    for (i = 0; i < 10; ++i)
    {
        athr_sleep(10); /* some time-consuming task */
        athr_consume(at, 1);
    }

    athr_finish(at);

    return 0;
}
