#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "strategy.h"

int FindMaxPriority(Strat_move* tab, int length)
{
    int i = 0;
    int nb = 0;
    int ret = -1;
    int max = 0;
    
    while(i <= length)  // 1er balayage : On chercher le plus grande priorite
    {
        if( (tab[i].priority > max) && !(tab[i].canMark) )
        {
            max = tab[i].priority;
            //ret = i;
            nb++;
        }
        i++;
    }
    
    // 2eme balayage : On prend au hasard un des maximums
    i = 0;
    int ran = rand() / nb;
    while(i <= length && rand > 0)
    {
        if(tab[i].priority == max && !(tab[i].canMark) )
        {
            ran--;
            ret = i;
        }
        i++;
    }
    printf("FindMaxPriority retourne %d pour une prio de %d\n", ret, max);
    return ret;
}



int FindMaxPriority2(Strat_move* tab, int length)
{
    int i = 0;
    int nb = 0;
    int ret = -1;
    int max = 0;
    
    while(i <= length)  // 1er balayage : On chercher le plus grande priorite
    {
        if( (tab[i].priority > max) )
        {
            max = tab[i].priority;
            //ret = i;
            nb++;
        }
        i++;
    }
    
    // 2eme balayage : On prend au hasard un des maximums
    i = 0;
    int ran = rand() / nb;
    while(i <= length && rand > 0)
    {
        if(tab[i].priority == max )
        {
            ran--;
            ret = i;
        }
        i++;
    }
    printf("FindMaxPriority retourne %d pour une prio de %d\n", ret, max);
    return ret;
}