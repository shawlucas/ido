#ifndef _SYNONYMS_H
#define _SYNONYMS_H

#ifdef _LIBC_NONSHARED
#define _INITBSS
#define _INITBSSS
#else
#define _INITBSS        =0
#define _INITBSSS       ={0}
#endif

#endif
