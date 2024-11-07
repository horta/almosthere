#ifndef ATHR_OVS_ATOMIC_POSIX_H
#define ATHR_OVS_ATOMIC_POSIX_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

static inline bool atomic_load_bool(atomic_bool *x) { return atomic_load(x); }

static inline uint_fast64_t atomic_load_uint_fast64(atomic_uint_fast64_t *x)
{
    return atomic_load(x);
}

static inline uint_fast64_t
atomic_fetch_add_uint_fast64(atomic_uint_fast64_t *x, uint_fast64_t arg)
{
    return atomic_fetch_add(x, arg);
}

#endif
