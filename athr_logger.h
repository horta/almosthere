#ifndef ATHR_LOGGER_H
#define ATHR_LOGGER_H

typedef void athr_logger_print_t(char const *msg, void *arg);

void athr_logger_setup(athr_logger_print_t *print, void *arg);

// PRIVATE ------------------------------------------------------------------
#define __STRINGIFY(n) #n
#define __LOCAL(n) __FILE__ ":" __STRINGIFY(n)

#define __ERROR_FMT(msg) __LOCAL(__LINE__) ": " msg
void __athr_logger_error(char const *msg);
#define error(msg) __athr_logger_error(__ERROR_FMT(msg))
// PRIVATE ------------------------------------------------------------------

#endif
