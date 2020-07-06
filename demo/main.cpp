#include <iostream>
#include <cstdlib>
#include <ctime>

char* heaptmp=heap_listp;
char* freetmp=free_listp;
for(; GET_SIZE(HDRP(heaptmp)) > 0; heaptmp = NEXT_BLKP(heaptmp))
{
    if(heaptmp % DSIZE!=0)
    {
        fprintf(stderr, "error, The address is not 8 aligned");
        return 0;
    }
    if((!GET_ALLOC(HDRP(bp))) &&( !GET_ALLOC(HDRP(NEXT_BLKP(bp)))))
    {
        fprintf(stderr, "error, There are contiguous free blocks");
        return 0;
    }
}
for(; freetmp; freetmp = (char*)GETDSIZE(SUCC((heaptmp))))
{
    if(GET_ALLOC(HDRP(freetmp)))
    {
        fprintf(stderr, "error, the allocated block is in free block");
        return 0;
    }
}
