#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "strategy.h"


/** traitementTableau.h
  * Regroupe toutes les fonctions relatives aux traitements de tableaux
  * Aucune prise de decision n est faite ici (voir strategy.c)
 **/


/** void ResetTab(SMove* const tab)
  * Remet a zero le tableau de mouvements que l on recoit de l ui
**/
void ResetTab(SMove* const tab)
{
    int i;
    for(i = 0 ; i < 4 ; i++)
    {
        tab[i].src_point = EPos_nopos;
        tab[i].dest_point = EPos_nopos;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////


/** void CopyTab(SMove* const dest, const SMove* const src)
  * Copie le tableau de mouvements finaux temporaires dans celui qui sert de renvoi
  * @param SMove* const dest : le tableau final dans lequel on veut copier
  * @param const SMove* const src : le tableau de mouvements finaux a copier
**/
void CopyTab(SMove* const dest, const SMove* const src)
{
    int i;
    for(i = 0 ; i < 4 ; i++)
    {
        dest[i] = src[i];
    }
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int FindMinProba(const Strat_move* const tab, const int length)
{
    int i = 0;
    int nb = 0;
    int ret = -1;
    int max = 0;
    
    while(i <= length)  // 1er balayage : On cherche la plus petite proba
    {
        if( (tab[i].proba >= max) )
        {
            max = tab[i].proba;
        }
        i++;
    }
    
    i = 0;
    while(i <= length)  // 2eme balayage : On compte les mouvements avec une proba min
    {
        if( (tab[i].proba == max) )
        {
            nb++;
        }
        i++;
    }
    
    i = 0;              // 3eme balayage : On prend au hasard un des min
    int ran = rand() % nb;
    while(i <= length && ran > -1)
    {
        if(tab[i].proba == max )
        {
            ran--;
            ret = i;
        }
        i++;
    }
    return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int FindMaxPriority(const Strat_move* const tab, const int length)
  * Determine le mouvement avec la priorite maximale
  * Choisis un mouvement au hasard parmi ceux de la priorite maximale, s il y en a plusieurs
  * @param const Strat_move* const tab : le tableau de mouvements
  * @param const int length : la taille du tableau
  * @return int : l index du mouvement choisi dans le tableau
**/
int FindMaxPriority(const Strat_move* const tab, const int length)
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
    
    if(max == 1)
    {
        return( FindMinProba( tab, length) );
    }
    else
    {
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
        while(i <= length && ran > -1)
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
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int FindMaxPriority2(const Strat_move* const tab, const int length)
 * Determine le mouvement avec la priorite maximale
 * Choisis un mouvement au hasard parmi ceux de la priorite maximale, s il y en a plusieurs
 * Utilise seulement pour determiner le mouvement lorsque l on peut marquer
 * @param const Strat_move* const tab : le tableau de mouvements
 * @param const int length : la taille du tableau
 * @return int : l index du mouvement choisi dans le tableau
 **/
int FindMaxPriority2(const Strat_move* const tab, const int length)
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
                
    i = 0;              // 3eme balayage : On prend au hasard un des maximums
    int ran = rand() % nb;
    while(i <= length && ran > -1)
    {
        if(tab[i].priority == max )
        {
            ran--;
            ret = i;
        }
        i++;
    }
    return ret;
}