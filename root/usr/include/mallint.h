#ifndef _MALLINT_H
#define _MALLINT_H

/* number of bytes to align to  (must be at least 4, because lower 2 bits
   are used for flags */

typedef __int32_t __psint_t;

#ifdef LIBMALLOC
#if (_MIPS_SZLONG == 32)
#define ALIGNSZ 8
#else
#define ALIGNSZ 16      /* 2*(sizeof(struct header *)) Min size */
#endif /* _MIPS_SZLONG */
#else
#define ALIGNSZ 16
#endif /* LIBMALLOC */

#ifndef MAX
#define MAX(a,b)        (a > b ? a : b)
#endif
#define ALIGNIT(x)      (((__psint_t)(x) + ALIGNSZ - 1) & ~(ALIGNSZ - 1))
#define ISALIGNED(x)    (((__psint_t)(x) & (ALIGNSZ - 1)) == 0)

#ifdef LIBMALLOC
#define CHECKQ          if (AP(m_debug)) __checkq();
#else
#define CHECKQ          if (AP(m_debug)) __acheckq(ap);
#endif

#define HEADSZ  ((int)sizeof(struct header))   /* size of unallocated block header */
#define MINBLKSZ        HEADSZ

#define BLOCKSZ (8*1024)        /* memory is gotten from sbrk in
                                   multiples of BLOCKSZ */
#define NUMLBLKS        100             /* default # of small blocks per
                                                holding block */

#define HGROUND (struct holdblk *)0     /* ground for the holding block queue */
#ifndef NULL
#define NULL    0
#endif

/*
        The following manipulate the small block flag
*/
#define SMAL    2L
#define SETSMAL(x)      ((struct lblk *)((__psint_t)(x) | SMAL))
#define CLRSMAL(x)      ((struct lblk *)((__psint_t)(x) & ~SMAL))
#define TESTSMAL(x)     ((__psint_t)(x) & SMAL)

/*
        The following manipulate both flags.  They must be
        type coerced
*/
#define SETALL(x)       ((__psint_t)(x) | (SMAL | BUSY))
#define CLRALL(x)       ((__psint_t)(x) & ~(SMAL | BUSY))


/* size of a holding block with small blocks of size blksz */
#define AHOLDSZ         (ALIGNIT(sizeof(struct holdblk)))
#define HOLDSZ(blksz)   (AHOLDSZ + (blksz)*AP(numlblks))
#define MAXFAST         28              /* default maximum size block for fast
                                             allocation */
#define MAXCHECK        100
#if (_MIPS_SZLONG == 32)
#define MAXALLOC        0x80000000      /* maximum bytes/malloc call */
#else
#define MAXALLOC        0x8000000000000000      /* maximum bytes/malloc call */
#endif     

#define BUSY    1L
#define SETBUSY(x)      ((struct header *)((__psint_t)(x) | BUSY))
#define CLRBUSY(x)      ((struct header *)((__psint_t)(x) & ~BUSY))
#define TESTBUSY(x)     ((__psint_t)(x) & BUSY)

#define SETIGNSZ    AP(ignoresz) = (int)ALIGNIT(AP(maxfast) + 1 + AP(minhead));

#define ADDFREEQ(fptr,x) if (AP(addtotail)) addtail(fptr,x); \
                                else addhead(fptr,x);
#define DELFREEQ(x)       (x)->prevfree->nextfree = (x)->nextfree;\
                                (x)->nextfree->prevfree = (x)->prevfree;\
                                (x)->prevfree = (x)->nextfree = 0; \
                                assert((x)->nextfree != (x));\
                                assert((x)->prevfree != (x));
#define MOVEHEAD(x)       AP(freeptr)[1].prevfree->nextfree = \
                                        AP(freeptr)[0].nextfree;\
                                AP(freeptr)[0].nextfree->prevfree = \
                                        AP(freeptr)[1].prevfree;\
                                (x)->prevfree->nextfree = &(AP(freeptr)[1]);\
                                AP(freeptr)[1].prevfree = (x)->prevfree;\
                                (x)->prevfree = &(AP(freeptr)[0]);\
                                AP(freeptr)[0].nextfree = (x);\
                                assert((x)->nextfree != (x));\
                                assert((x)->prevfree != (x));

#define COMPACTFWD(b,n) { \
                        register struct header *nextnext; \
                        if (!TESTBUSY(nextnext = (n)->nextblk))  { \
                                DELFREEQ(n); \
                                (b)->nextblk = nextnext; \
                                nextnext->prevblk = (b); \
                        } \
                        }

#define COMPACTBACK(b)  { \
                        register struct header *next; \
                        if (!TESTBUSY((next = (b)->prevblk)->nextblk)) { \
                                DELFREEQ(next); \
                                next->nextblk = (b)->nextblk; \
                                (b)->nextblk->prevblk = next; \
                                (b) = next; \
                        } \
                        }

struct lblk  {
        union {
                struct lblk *nextfree;  /* the next free little block in this
                                           holding block.  This field is used
                                           when the block is free */
                struct holdblk *holder; /* the holding block containing this
                                           little block.  This field is used
                                           when the block is allocated */
        }  header;
        char byte;                  /* There is no telling how big this
                                           field freally is.  */
};
#define MINSMALLHEAD ((int)sizeof(struct lblk *))

struct holdblk {
        struct holdblk *nexthblk;   /* next holding block */
        struct holdblk *prevhblk;   /* previous holding block */
        struct lblk *lfreeq;    /* head of free queue within block */
        struct lblk *unused;    /* pointer to 1st little block never used */
        int blksz;              /* size of little blocks contained */
        int holdsz;             /* useful holdblk constant */
        char head[MINSMALLHEAD];/* maybe header for first small block
                                 * in hold group (depends on alignment) */
};

#endif
