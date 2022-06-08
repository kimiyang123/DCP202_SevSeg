#include "utils.h"

#include <stdlib.h>

void uSrand(unsigned int seed)
{
    srand(seed);
}

int  random(int rmin,int rmax)
{
    // srand(230);
    return rand() % (rmax - rmin) + rmin;
}
