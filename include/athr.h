#ifndef ATHR_H_
#define ATHR_H_

/** Major athr version. */
#define ATHR_VERSION_MAJOR 1
/** Minor athr version. */
#define ATHR_VERSION_MINOR 0
/** Minor athr version. */
#define ATHR_VERSION_PATCH 5
/** Athr version. */
#define ATHR_VERSION "1.0.5"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
#ifdef ATHR_API_EXPORTS
#define ATHR_API __declspec(dllexport)
#else
#define ATHR_API __declspec(dllimport)
#endif
#else
#define ATHR_API
#endif

    struct athr;

    enum ATHR_OPTS
    {
        ATHR_BAR = 1,
        ATHR_ETA = 2,
        ATHR_PERC = 4
    };

    typedef struct
    {
        long volume;
        const char *desc;
        enum ATHR_OPTS opts;
    } athr_create_args;

    ATHR_API struct athr *athr_create_var(athr_create_args in);
#define athr_create(...) athr_create_var((athr_create_args){__VA_ARGS__});

    ATHR_API void athr_consume(struct athr *at, long consume);
    ATHR_API void athr_finish(struct athr *at);
    ATHR_API void athr_sleep(long milliseconds);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: ATHR_H_ */
