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
            nb++;
        }
        i++;
    }
    
    i = 0;
    while(i <= length)  // 2eme balayage : On compte les mouvements avec une priorite max
    {
        if( (tab[i].priority == max) )
        {
            nb++;
        }
        i++;
    }
        
    i = 0;              // 3eme balayage : On prend au hasard un des maximums
    int ran = rand() % nb;
    while(i <= length && ran > 0)
    {
        if(tab[i].priority == max && !(tab[i].canMark) )
        {
            ran--;
            ret = i;
        }
        i++;
    }
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
        if( (tab[i].priority >= max) )
        {
            max = tab[i].priority;
        }
        i++;
    }
    
    i = 0;
    while(i <= length)  // 2eme balayage : On compte les mouvements avec une priorite max
    {
        if( (tab[i].priority == max) )
        {
            nb++;
        }
        i++;
    }
    printf("Il a trouve %d random\n", nb);
                
    i = 0;              // 3eme balayage : On prend au hasard un des maximums
    int ran = rand() % nb;
    printf("rand = %d\n", ran);
    while(i <= length && ran > -1)
    {
        if(tab[i].priority == max )
        {
            ran--;
            ret = i;
        }
        i++;
    }
    printf("Renvoi de ret = %d\n", ret);
    return ret;
}