#ifndef COMMON_H
#define COMMON_H

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static inline unsigned maxu(unsigned a, unsigned b) { return a > b ? a : b; }

static inline unsigned minu(unsigned a, unsigned b) { return a <= b ? a : b; }

#define UNUSED(x) (void)(x)

#endif
