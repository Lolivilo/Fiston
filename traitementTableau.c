#include <stdio.h>
#include <stdlib.h>
#include "strategy.h"

int FindMaxPriority(Strat_move* tab, int length)
{
    int i = 0;
    int ret = -1;
    int max = 0;
    
    while(i <= length)
    {
        if( (tab[i].priority > max) && !(tab[i].canMark) )
        {
            max = tab[i].priority;
            ret = i;
        }
        i++;
    }

    return ret;
}