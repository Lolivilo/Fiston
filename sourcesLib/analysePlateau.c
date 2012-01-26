#include <stdio.h>
#include <stdlib.h>
#include "strategy.h"
#include "traitementTableau.h"



/** analysePlateau.c
  * Regroupe toutes les fonctions relatives a l analyse du plateau de jeu
  * Aucune prise de decision n est faite ici (voir strategy.c)
**/



/** void MoveType(const SGameState* const board, Strat_move* const move)
 * Donne le type du mouvement fraichement trouve en remplissant les booleens de la structure Strat_move
 * @param const SGameState* const board : le plateau de jeu actuel
 * @param Strat_move* const move : le mouvement a evaluer
 **/
void MoveType(const SGameState* const board, Strat_move* const move)
{
	SZone zoneArrivee = board->zones[move->to];
    
    if(zoneArrivee.player == EPlayer1 && zoneArrivee.nb_checkers == 1)  // Peut-il proteger un pion seul ?
    {
        move->canProtect = 1;
    }
	if(zoneArrivee.player == EPlayer2 && zoneArrivee.nb_checkers == 1)  // Peut-il manger un pion adverse ?
	{
		move->canEat = 1;
	}
	if(move->to == EPos_OutP1)                                          // Peut-il marquer ?
	{
		move->canMark = 1;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int CanWeMark(const SGameState* const board)
 * Analyse le plateau et regarde si on peut marquer
 * @param const SGameState* const board : le plateau de jeu actuel
 * @return int : 1 si on peut marquer ; 0 sinon
 **/
int CanWeMark(const SGameState* const board)
{
    // On parcours toutes les zones de 1 & 6 + la zone de marquage et on regarde si on a nos 15 pions
    EPosition current;
    int count = board->zones[EPos_OutP1].nb_checkers;
    for(current = EPos_1 ; current <= EPos_6 ; current++)
    {
        if(board->zones[current].player == EPlayer1)
        {
            count += board->zones[current].nb_checkers;
        }
    }
    if(count == 15)
    {
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int CanWeEat(const Strat_move* const tab, const int length)
 * Analyse le plateau et regarde si on peut manger un pion adverse
 * @param const Strat_move* const tab : le tableau de mouvements potentiels
 * @param const int length : la taille du tableau
 * @return int : 1 si on peut manger ; 0 sinon
 **/
int CanWeEat(const Strat_move* const tab, const int length)
{
    int i = 0;
    // On regarde dans notre tableau si on peut manger
    while(i <= length)
    {
        if(tab[i].canEat)
        {
            return 1;
        }
        i++;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int CanWeProtect(const Strat_move* const tab, int length)
 * Analyse le plateau et regarde si on peut proteger un de nos pions seuls
 * @param const Strat_move* const tab : le tableau de mouvements potentiels
 * @param const int length : la taille du tableau
 * @return int : 1 si on peut proteger ; 0 sinon
 **/
int CanWeProtect(const Strat_move* const tab, const int length)
{
    int i = 0;
    // On regarde dans notre tableau si on peut proteger
    while(i <= length)
    {
        if(tab[i].canProtect)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

