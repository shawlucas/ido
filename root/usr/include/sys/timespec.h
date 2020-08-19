#ifndef _STRUCT_TIMESPEC
#define _STRUCT_TIMESPEC 1

#include <bits/types.h>

struct timespec
{
  __time_t tv_sec;              /* Seconds.  */
  __syscall_slong_t tv_nsec;    /* Nanoseconds.  */
};

#endif
