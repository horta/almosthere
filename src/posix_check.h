#ifndef POSIX_CHECK_H_
#define POSIX_CHECK_H_

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
/* You can find out the version with _POSIX_VERSION. */
#define POSIX_SYSTEM
#endif

#endif /* end of include guard: POSIX_CHECK_H_ */
