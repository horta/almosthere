#ifndef LOGGER_H
#define LOGGER_H

#define __STRINGIFY(n) #n
#define __LOCAL(n) __FILE__ ":" __STRINGIFY(n)

#define __ERROR_FMT(msg) __LOCAL(__LINE__) ": " msg
void __athr_logger_error(char const *msg);
#define error(msg) __athr_logger_error(__ERROR_FMT(msg))

#endif
