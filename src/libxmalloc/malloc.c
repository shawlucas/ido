#include <stdio.h>
#define LIBMALLOC
#include <malloc.h>
#include <sgimalloc.h>
#include <mallint.h>
#include <assert.h>
#include <string.h>

/* _malloc */
/* _free */

#define AP(x)   x
#define GROW(x) _sbrk(x)
#define ADDAARG0()
#define ADDAARG(x)      (x)

static struct header arena[2];  /* the second word is a minimal block to
                                   start the arena. The first is a busy
                                   block to be pointed to by the last
                                   block.       */

static struct header freeptr[2];
static struct header ignorefp[2];

static struct header* arenaend;
static struct holdblk** holdhead;

static int numlblks = NUMLBLKS;
static int change = 0;

static int blocksz = BLOCKSZ;
static int addtotail = 1;
static int m_debug = 0;           /* whether to check arena each action */
static int grain = ALIGNSZ;
static int maxfast = MAXFAST;
static unsigned int maxcheck = MAXCHECK;

/* minhead = MAX(MINHEAD, ALIGNSZ); */
static int minhead = 2*(sizeof(struct header *));

/* The following are computable based on the rest (and not user settable) */
static int fastct;
static int ignoresz;
/* number of small block sizes to map to one size */
static int grain2;
static int grain2shift;
static int clronfree = 0;
static char clronfreevalue;

static void *_lmalloc(size_t);
static void _lfree(void *);
static int ntos(int);

void* _recalloc(void* ptr, size_t num, size_t size) {
    register char* mp;
    register size_t oldsize, newsize;

    num *= size;
    oldsize = mallocblksize(ptr);
    mp = realloc(ptr, num);
    newsize = mallocblksize(mp);

    if (mp) {
        if (newsize > oldsize) 
            (void)memset(mp + oldsize, 0, newsize - oldsize);
        else
            (void)memset(mp + num, 0, newsize - num);
    }

    return mp;
}

static void* _lmalloc(register size_t nbytes) {
    register size_t nb;

    if (freeptr[0].nextfree == GROUND) {
        AP(grain2) = 0;
        if (((AP(grain) - 1) & AP(grain)) == 0) {
            AP(grain2) = AP(grain) - 1;
            AP(grain2shift) = ntos(AP(grain));
        }

        AP(arena)[1].nextblk = (struct header *)BUSY;
        AP(arena)[0].nextblk = (struct header *)BUSY;
        AP(arenaend) = &(AP(arena)[1]);

        AP(freeptr)[0].nextfree = &(AP(freeptr)[1]);
        AP(freeptr)[1].nextblk = &(AP(arena)[0]);
        AP(freeptr)[1].prevfree = &(AP(freeptr)[0]);
        AP(ignorefp)[0].nextfree = &(AP(ignorefp)[1]);
        AP(ignorefp)[1].prevfree = &(AP(ignorefp)[0]);
        SETIGNSZ;
    }

    if (nbytes == 0)
        return NULL;
    if (nbytes <= AP(maxfast)) {
        register struct holdblk* holdblk;
        register struct lblk* lblk;
        register int holdn;
        register int chg, g2, g2shift;

        chg = AP(change);
        g2 = AP(grain2);
        g2shift = AP(grain2shift);

        if (!chg) {
            register int i;
            /*
			 *	This allocates space for hold block
			 *	pointers by calling malloc recursively.
			 *	Maxfast is temporarily set to 0, to
			 *	avoid infinite recursion.  Allocate
			 *	space for an extra ptr so that an index
			 *	is just (size + MINSMALLHEAD)/grain
			 *	with the first pointer unused.
			 *	Once change is set, changes to algorithm
			 *	parameters are no longer allowed.
			 */
            AP(fastct) = ((AP(maxfast) + MINSMALLHEAD) + AP(grain) - 1) / AP(grain);
            AP(change) = AP(maxfast);
            AP(maxfast) = 0;
            AP(holdhead) = (struct holdblk **)_lmalloc(ADDAARG(sizeof(struct holdblk *) * (AP(fastct) + 1)));
            if (AP(holdhead) == NULL) {
                AP(change) = 0;
                return(_lmalloc(ADDAARG(nbytes)));
            }
            AP(maxfast) = AP(change);
            for (i = 1; i < AP(fastct) + 1; i++) {
                AP(holdhead)[i] = HGROUND;
            }
        }
        /*
         * Find out which holding block. Add room for header
         * Round up to grain boundary
         */

        if (g2) {
            holdn = (int)(nbytes + MINSMALLHEAD + g2);
            holdn >>= g2shift;
            holdblk = AP(holdhead)[holdn];
            nb = holdn << g2shift;
        } else {
            holdn = (int)((nbytes + MINSMALLHEAD + AP(grain) - 1) / AP(grain));
            holdblk = AP(holdhead)[holdn];
            nb = holdn * AP(grain);
        }

        if ((holdblk != HGROUND) && (holdblk->lfreeq != LGROUND)) {
            assert(holdblk->blksz >= nbytes);
            lblk = holdblk->lfreeq;

            if (lblk < holdblk->unused) {
                if ((holdblk->lfreeq = CLRSMAL(lblk->header.nextfree)) == LGROUND) {
                    AP(holdhead)[holdn] = holdblk->nexthblk;
                }
            } else if (((char *)holdblk->unused + nb + MINSMALLHEAD) < ((char *)holdblk + holdblk->holdsz)) {
                holdblk->lfreeq = holdblk->unused = (struct lblk *)((char *)holdblk->unused + nb);
            } else {
                holdblk->lfreeq = LGROUND;
                holdblk->unused += nb;
                AP(holdhead)[holdn] = holdblk->nexthblk;
            }

            lblk->header.holder = (struct holdblk *)SETALL(holdblk);
        } else {
            register struct holdblk* newhold;

            assert(ISALIGNED(HOLDSZ(nb)));
            newhold = (struct holdblk *)_lmalloc(ADDAARG(HOLDSZ(nb)));
            if (newhold == NULL)
                return NULL;
            
            if (holdblk != HGROUND) {
                newhold->nexthblk = holdblk;
                newhold->prevhblk = holdblk->prevhblk;
                holdblk->prevhblk = newhold;
                newhold->prevhblk->nexthblk = newhold;
            } else {
                newhold->nexthblk = newhold->prevhblk = newhold;
            }
            AP(holdhead)[holdn] = newhold;

            lblk = (struct lblk *)((char *)newhold + AHOLDSZ - MINSMALLHEAD);
            newhold->lfreeq = newhold->unused = (struct lblk *)((char *)lblk + nb);
            newhold->blksz = (int)(nb - MINSMALLHEAD);
            newhold->holdsz = (int)HOLDSZ(nb);
            lblk->header.holder = (struct holdblk *)SETALL(newhold);
        }
        CHECKQ;
        return (char *)lblk + MINSMALLHEAD;
    } else {
        register struct header* blk;
        register struct header* newblk;
        register int tries = 0;
        register int try = AP(maxcheck);

        nb = nbytes + AP(minhead);
        nb = ALIGNIT(nb);
        nb = (nb > MINBLKSZ) ? nb : MINBLKSZ;

        if (nbytes > MAXALLOC) 
            return NULL;
        
        blk = AP(freeptr);
        do {
            blk = blk->nextfree;
            if (tries++ >= try) {
                if (blk != &(AP(freeptr)[1])) {
                    MOVEHEAD(blk);
                    blk = AP(arenaend)->prevblk;
                    if (!TESTBUSY(blk->nextblk)) {
                        if (((char *)(blk->nextblk) - (char *)blk) >= nb)
                            break;
                    }
                    blk = &(AP(freeptr)[1]);
                }
                break;
            }
        } while (((char *)(blk->nextblk) - (char *)blk) < nb);

		if (blk == &(AP(freeptr)[1]))  {
			register struct header *newend; /* new end of arena */
			register size_t nget;      /* number of words to get */
			register size_t al;	   /* amount newblk off align */
			register struct header *lastblk;

			/* Three cases - 1. There is space between arenaend
					    and the break value that will become
					    a permanently allocated block.
					 2. Case 1 is not true, and the last
					    block is allocated.
					 3. Case 1 is not true, and the last
					    block is free
			*/
			newblk = GROW(0);
			al = (size_t)newblk & (ALIGNSZ - 1);
			lastblk = AP(arenaend)->prevblk;
			if (newblk != (struct header *)((char *)AP(arenaend) + HEADSZ)) {
				/* case 1 */
				/* get size to fetch */
				nget = nb+HEADSZ;
				/* round up to a block */
				/* break into components so ``smart''
				 * compiler won't nop "/BLOCKSZ*BLOCKSZ".
				 */
				nget = nget+AP(blocksz)-1;
				nget /= AP(blocksz);
				nget *= AP(blocksz);
				/* always get aligned amount */
				nget = ALIGNIT(nget);

				/*
				 * if start (newblk) not properly aligned,
				 * get extra space
				 */
				if (al)
					nget += (ALIGNSZ - al);
				assert((((size_t)newblk+nget)%ALIGNSZ) == 0);
				/* get memory */
				if (GROW(nget) == (void *)-1L)
					return NULL;

				/* add to arena */
				newend = (struct header *)((char *)newblk + nget
					 - HEADSZ);
				/* ignore some space to make block aligned */
				if (al)
					newblk = (struct header *)
						 ((char *)newblk +
						 ALIGNSZ - al);
				newend->nextblk = SETBUSY(&(AP(arena)[1]));
				newblk->nextblk = newend;
				AP(arenaend)->nextblk = SETBUSY(newblk);
				newend->prevblk = newblk;
				AP(arena)[1].prevblk = newend;
				newblk->prevblk = AP(arenaend);
				/* adjust other pointers */
				AP(arenaend) = newend;
				blk = newblk;
			} else if (TESTBUSY(lastblk->nextblk))  {
				/* case 2 */
				nget = nb+AP(blocksz)-1;
				nget /= AP(blocksz);
				nget *= AP(blocksz);
				/* always get aligned amount */
				nget = ALIGNIT(nget);
				if (GROW(nget) == (void *)-1L)
					return NULL;

				/* block must be aligned */
				assert(((int)newblk%ALIGNSZ) == 0);
				newend = (struct header *)
				    ((char *)AP(arenaend)+nget);
				newend->nextblk = SETBUSY(&(AP(arena)[1]));
				AP(arenaend)->nextblk = newend;
				newend->prevblk = AP(arenaend);
				AP(arena)[1].prevblk = newend;
				blk = AP(arenaend);
				AP(arenaend) = newend;
			}  else  {
				/* case 3 */
				/* last block in arena is at end of memory and
				   is free */
				nget = nb -
				      ((char *)AP(arenaend) - (char *)lastblk);
				nget = nget+AP(blocksz)-1;
				nget /= AP(blocksz);
				nget *= AP(blocksz);
				/* always get aligned amount */
				nget = ALIGNIT(nget);
				/* if not properly aligned, get extra space */
				if (GROW(nget) == (void *)-1L)
					return NULL;

				assert(((int)newblk%ALIGNSZ) == 0);
				/* combine with last block, put in arena */
				newend = (struct header *)
				    ((char *)AP(arenaend) + nget);
				AP(arenaend) = lastblk->nextblk = newend;
				newend->nextblk = SETBUSY(&(AP(arena)[1]));
				newend->prevblk = lastblk;
				AP(arena)[1].prevblk = newend;
				/* set which block to use */
				blk = lastblk;
				DELFREEQ(blk);
			}
		}  else  {
			register struct header *nblk;      /* next block */

			/* take block found off free queue */
			nblk = blk->nextfree;
			DELFREEQ(blk);
			/*
			 * Make head of free queue immediately follow blk,
			 * unless blk was at the end of the queue.
			 */
			if (nblk != &(AP(freeptr)[1]))   {
				MOVEHEAD(nblk);
			}
		}
		/*
		 * Blk now points to an adequate block.
		 */
		assert(TESTBUSY(blk->prevblk->nextblk));
		assert(TESTBUSY(blk->nextblk->nextblk));
		nbytes = (char *)blk->nextblk - (char *)blk - nb;
		if (nbytes >= MINBLKSZ) {
			/*
			 * Carve out the right size block.
			 * newblk will be the remainder.
			 */
			newblk = (struct header *)((char *)blk + nb);
			newblk->nextblk = blk->nextblk;
			/* mark the block busy */
			blk->nextblk = SETBUSY(newblk);
			/*
			 * If the trailing space is so small it won't
			 * ever fit a regular block, tuck it away on
			 * the ignore free list.  It might be coalesced
			 * when an adjacent block is freed.
			 */
			if (nbytes >= AP(ignoresz)) {
				ADDFREEQ(AP(freeptr), newblk);
			} else {
				ADDFREEQ(AP(ignorefp), newblk);
			}
			newblk->prevblk = blk;
			newblk->nextblk->prevblk = newblk;
		}  else  {
			/* just mark the block busy */
			blk->nextblk = SETBUSY(blk->nextblk);
		}
		assert(ISALIGNED(blk));
		CHECKQ;
		return (char *)blk + AP(minhead);
	}
}

static void
_lfree(register void *ptr)
{
	register struct holdblk *holdblk;       /* block holding blk */
	register struct holdblk *oldhead;       /* former head of the hold 
						   block queue containing
						   blk's holder */

	if (ptr == NULL)			/* protect lazy programmers */
		return;
	if (AP(clronfree)) {
		memset(ptr, AP(clronfreevalue), _lmallocblksize(ADDAARG(ptr)));
	}
	if (TESTSMAL(((struct header *)((char *)ptr - MINSMALLHEAD))->nextblk))  {
		register struct lblk *lblk;     /* pointer to freed block */
		register int offset;		/* choice of header lists */

		lblk = (struct lblk *)CLRBUSY((char *)ptr - MINSMALLHEAD);
		assert((struct header *)lblk < AP(arenaend));
		assert((struct header *)lblk > AP(arena));
		/* allow twits (e.g. awk) to free a block twice */
		if (!TESTBUSY(holdblk = lblk->header.holder)) return;
		holdblk = (struct holdblk *)CLRALL(holdblk);
		/* put lblk on its hold block's free list */
		lblk->header.nextfree = SETSMAL(holdblk->lfreeq);
		holdblk->lfreeq = lblk;
		/* move holdblk to head of queue, if its not already there */
		offset = (holdblk->blksz + MINSMALLHEAD)/AP(grain);
		oldhead = AP(holdhead)[offset];
		if (oldhead != holdblk)  {
			/* first take out of current spot */
			AP(holdhead)[offset] = holdblk;
			holdblk->nexthblk->prevhblk = holdblk->prevhblk;
			holdblk->prevhblk->nexthblk = holdblk->nexthblk;
			/* now add at front */
			holdblk->nexthblk = oldhead;
			holdblk->prevhblk = oldhead->prevhblk;
			oldhead->prevhblk = holdblk;
			holdblk->prevhblk->nexthblk = holdblk;
		}
	}  else  {
		register struct header *blk;	    /* real start of block*/
		register struct header *nxt;      /* nxt = blk->nextblk*/

		blk = (struct header *)((char *)ptr - AP(minhead));
		nxt = blk->nextblk;
		/* take care of twits (e.g. awk) who return blocks twice */
		if (!TESTBUSY(nxt))
			return;
		blk->nextblk = nxt = CLRBUSY(nxt);
		/*
		 * See if we can compact with the following chunk.
		 */
		COMPACTFWD(blk, nxt);
		/*
		 * See if we can compact with the preceeding chunk.
		 */
		COMPACTBACK(blk);
		ADDFREEQ(AP(freeptr), blk);
	}
	CHECKQ
}

