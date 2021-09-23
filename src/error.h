#ifndef ERROR_H
#define ERROR_H

struct athr;

void __error(struct athr *at, char const *msg);

#define __ARRSIZE(x) (sizeof(x) / sizeof(x[0]))
#define __STRADDR(x) ((char const *)(char const(*)[__ARRSIZE(x)]){&(x)})

#define error(at, msg) __error(at, __STRADDR(msg))

#endif
