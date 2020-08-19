#ifndef _SGI_MALLOC_H
#define _SGI_MALLOC_H

struct header {
        struct header *nextblk;
        struct header *prevblk;
        struct header *nextfree;
        struct header *prevfree;
};

#define GROUND  (struct header *)0
#define LGROUND (struct lblk *)0

#endif
