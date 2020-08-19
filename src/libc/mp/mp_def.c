#include <synonyms.h>
#include <stdio.h>

#pragma weak _sproced = __multi_thread
#pragma weak _us_sthread_stdio = __us_sthread_stdio
#pragma weak _us_rsthread_stdio = __us_rsthread_stdio
#pragma weak _us_sthread_misc = __us_sthread_misc
#pragma weak _us_rsthread_misc = __us_rsthread_misc
#pragma weak _us_sthread_malloc = __us_sthread_malloc
#pragma weak _us_rsthread_malloc = __us_rsthread_malloc
#pragma weak _us_sthread_pmq = __us_sthread_pmq
#pragma weak _us_rsthread_pmq = __us_rsthread_pmq

/*
 * global variable to tell whether to enable multi-threading or
 * not - automatically turned on by sproc(2)
 * This needs to be initialized for ragnarok & weak to work.
 */
void *__multi_thread = 0;

int (*_lock)() _INITBSS;
int (*_ulock)() _INITBSS;
void * (*_nlock)() _INITBSS;
int (*_ilock)() _INITBSS;
void (*_freelock)() _INITBSS;
int (*_wlock)() _INITBSS;
int (*_clock)() _INITBSS;
int (*_tlock)() _INITBSS;
int (*_ctlock)() _INITBSS;
int (*_dlock)() _INITBSS;
int (*_cas)() _INITBSS;
int (*_cas32)() _INITBSS;
int (*_usgrablock)() _INITBSS;
int (*_usrellock)() _INITBSS;

/*
 * To provide finer granularity locking, we use these globals
 * to govern the locking macros used in libc
 * The 'r' versions are the ones actually used, the non 'r' versions
 * are so that users can set them via usconfig before having
 * called sproc.
 */
int __us_sthread_stdio = 1;
int __us_rsthread_stdio = 0;
int __us_sthread_misc = 1;
int __us_rsthread_misc = 0;
int __us_sthread_malloc = 1;
int __us_rsthread_malloc = 0;
int __us_sthread_pmq = 1;
int __us_rsthread_pmq = 0;
