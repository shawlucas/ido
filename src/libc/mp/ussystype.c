#include "synonyms.h"
#include <unistd.h>
#include <stdlib.h>


int _getsystype(void) 
{
    int nprocs;

    nprocs = (int)sysconf(_SC_NPROCESSORS_CONF);

#ifndef NDEBUG
    {
        char* syst;
        int ctype;
        if ((syst = getenv("USSYSTYPE")) != NULL) {
            ctype = atoi(syst);
        }
    }
}