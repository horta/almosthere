#ifndef COMMON_H
#define COMMON_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define MAX(a, b)                                                              \
    ({                                                                         \
        __typeof__(a) _a = (a);                                                \
        __typeof__(b) _b = (b);                                                \
        _a > _b ? _a : _b;                                                     \
    })

#define MIN(a, b)                                                              \
    ({                                                                         \
        __typeof__(a) _a = (a);                                                \
        __typeof__(b) _b = (b);                                                \
        _a <= _b ? _a : _b;                                                    \
    })

#define UNUSED(x) (void)(x)

#endif
