#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "strategy.h"
#include "analysePlateau.h"
#include "traitementTableau.h"


// Soit on sort du plateau
// Soit on arrive sur une zone a nous
// Soit la zone est vide
// Soit elle est adverse avec un seul pion
#define IF_PLAYABLE(de) if( (((int)zone-de) <= 0) \
                        || (currentGameState.zones[zone - de].player == EPlayer1) \
                        || (currentGameState.zones[zone - de].nb_checkers == 0)	\
                        || (currentGameState.zones[zone - de].player == EPlayer2 && currentGameState.zones[zone - de].nb_checkers == 1)) \

#define HavePrisoner currentGameState.zones[EPos_BarP1].nb_checkers > 0

// Variables locales
SGameState currentGameState;    // Copie locale de l etat courant du jeu
Strat_move* potentialMoves = NULL;     // Tableau de mouvements potentiels
int dies[5];	// | nombre_de_des | de1 | de2 | de3 | de4 |
SMove* finalMoves;



void StartGame()
{
	// Allocation du tableau temporaire de ressources
    finalMoves = (SMove*) malloc(4 * sizeof(SMove));
    if(finalMoves == NULL)
    {
        printf("Erreur d'allocation !\n");
        exit(0);
    }
    int i;
    for(i = 0 ; i <= 4 ; i++)
    {
        finalMoves[i].src_point = EPos_nopos;
        finalMoves[i].dest_point = EPos_nopos;
    }
}

void EndGame()
{
	// Desallocation du tableau temporaire de ressources
    free(finalMoves);
    if(potentialMoves != NULL)
    {
        free(potentialMoves);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{
	srand(time(NULL));	// Pour le random
	currentGameState = *gameState;  // Copie locale de l etat courant du jeu

    ResetTab(moves);    // RaZ du tableau recu
    
	// Remplissage du tableau de des
	dies[1] = currentGameState.die1;
	dies[2] = currentGameState.die2;
	if(dies[1] == dies[2])
	{
		dies[3] = dies[1];
		dies[4] = dies[1];
		dies[0] = 4;
	}
	else
	{
		dies[3] = -1;
		dies[4] = -1;
		dies[0] = 2;
	}
	//printf("DES INITIAUX : | %d | %d | %d | %d |\n", dies[1], dies[2], dies[3], dies[4]);	
	ListPotentialMoves();
    
    CopyTab(moves, finalMoves); // Copie de nos mouvements choisis dans le tableau de renvoi
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void ListPotentialMoves()
  * Liste l ensemble des mouvements potentiellement realisables 
**/
void ListPotentialMoves()
{
	// Remise a zero du tableau de mouvements
    potentialMoves = realloc(potentialMoves, 0);
    potentialMoves = NULL;
    if(potentialMoves != NULL)
    {
        printf("Erreur d'allocation !\n");
        exit(0);
    }
    // Si un a un pion prisonnier, il faut le liberer avant toute chose
    if(HavePrisoner)
    {
        IsEligibleForRelease();
    }
    else
    {
        int moveNumber = 0;
        // Parcours de chaque zone
        EPosition zone;
        for(zone = EPos_24 ; zone >= EPos_1 ; zone--)
        {
            // Si la zone courante nous appartient, on peut peut-etre effectuer un mouvement
            if(currentGameState.zones[zone].player == EPlayer1 && currentGameState.zones[zone].nb_checkers > 0)
            {
                // De 1
                if(dies[1] != -1)	// Si le de n est pas utilise
                {
                    IF_PLAYABLE(dies[1])  // Conditionnelle : teste si le point d arrivee est possible
                    {
                        FillPotentialMoves(zone, dies[1], moveNumber);
                        moveNumber++;
                    }
                }
                // De 2
                if(dies[2] != -1 && dies[2] != dies[1])
                {
                    IF_PLAYABLE(dies[2])
                    {
                        FillPotentialMoves(zone, dies[2], moveNumber);
                        moveNumber++;
                    }
                }
                // De 3
                if(dies[3] != -1 && dies[3] != dies[1] && dies[3] != dies[2])			// Il faut que le de existe et que la valeur le soit pas deja evaluee
                {
                    IF_PLAYABLE(dies[3])
                    {
                        FillPotentialMoves(zone, dies[3], moveNumber);
                        moveNumber++;
                    }
                }
                // De 4
                if(dies[4] != -1 && dies[4] != dies[1] && dies[4] != dies[2] && dies[4] != dies[3])	// Il faut que le de existe et que la valeur le soit pas deja evaluee
                {
                    IF_PLAYABLE(dies[4])
                    {
                        FillPotentialMoves(zone, dies[4], moveNumber);
                        moveNumber++;
                    }
                }
            }
        }
        if(!(moveNumber == 0))  // On continue seulement si on a au moins un mouvement realisable
        {
            ChooseMove(moveNumber - 1);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void IsEligibleForRelease()
  * Determine si un pion prisonnier peut etre libere
**/
void IsEligibleForRelease()
{
    int canPlay = 0;
    int moveNumber = 0;
    int nbdies;
    for(nbdies = 1 ; nbdies <= 4 ; nbdies++ )  // Parcours des des
    {
        if( (dies[nbdies] != -1) && (nbdies < 2 || ((nbdies >= 2) && (dies[nbdies] != dies[nbdies - 1]))) )
        {
            SZone potExit = currentGameState.zones[24 - dies[nbdies]];  // sortie potenielle testee
            if((potExit.nb_checkers == 0)                               // Si aucun pion
            || (potExit.player == EPlayer1)                             // OU si zone a nous
            || (potExit.player == EPlayer2 && potExit.nb_checkers <= 1))// OU zone adverse a 1 pion
            {
                canPlay = 1;
                FillPotentialMoves(EPos_BarP1, dies[nbdies], moveNumber);
                moveNumber++;
            }
        }
    }
    if(!(canPlay))  // Si on ne peut pas jouer, notre tour est termine
    {
        FinalReturn(-1);
    }
    else
    {
        ChooseMove(moveNumber - 1);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void FillPotentialMoves(const EPosition start, const int die, const int moveNumber)
  * Remplie le tableau de mouvements potentiels
  * @param const EPosition start : le point de depart du mouvement a ajouter
  * @param const int die : la valeur du de utilise
  * @param const int moveNumber : le numero du mouvement a ajouter (pour redimensionner le tableau)
  *
**/
void FillPotentialMoves(const EPosition start, const int die, const int moveNumber)
{
    // Augmentation de la taille du tableau de 1
    potentialMoves = (Strat_move*) realloc(potentialMoves, (moveNumber+1) * sizeof(Strat_move));
    potentialMoves[moveNumber].from = start;
    potentialMoves[moveNumber].canEat = 0;
    potentialMoves[moveNumber].canMark = 0;
    potentialMoves[moveNumber].canProtect = 0;
    potentialMoves[moveNumber].priority = 0;
    potentialMoves[moveNumber].proba = 0;
    if(start == EPos_BarP1) // Si le mouvement vient de la prison, l arrivee n est pas calculee de la meme facon
    {
        potentialMoves[moveNumber].to = 24 - die;
        EvaluateToExit(&potentialMoves[moveNumber]);
    }
    else
    {
        if((int)(start - die) <= 0)                      // Sortie pour marquer
        {
            potentialMoves[moveNumber].to = EPos_OutP1;
        }
        else                                            // Mouvement normal
        {
            potentialMoves[moveNumber].to = start - die;
        }
        MoveType(&currentGameState, &potentialMoves[moveNumber]);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** EvaluateToExit()
  * Evalue chaque mouvement de sortie de prison et modifie la priorite de chacun
  * @param Strat_move* const move : le mouvement a evaluer
**/
void EvaluateToExit(Strat_move* const move)
{
    SZone exit = currentGameState.zones[move->to];
    if( (exit.player == EPlayer2) && (exit.nb_checkers == 1) )
    {
        move->priority = 4; // Si on peut manger un pion
    }
    else if( (exit.player == EPlayer1) && (exit.nb_checkers == 1) )
    {
        move->priority = 3; // Si on peut proteger un de nos pions seuls
    }
    else if( (exit.player == EPlayer1) && (exit.nb_checkers > 1) )
    {
        move->priority = 2; // Si on peut aller sur une zone nous appartenant
    }
    else
    {
        move->priority = 1; // Si aucune autre solution
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void ChooseMove(const int tabLength)
  * Selectionne un mouvement dans le tableau de mouvements potentiels
  * @param const int tabLength : la taille du tableau de mouvements potentiels
**/
void ChooseMove(const int tabLength)
{
    if( HavePrisoner )                                              // Priorite 1 : si on a des prisonniers
    {
        FinalReturn( FindMaxPriority(potentialMoves, tabLength) );
    }
    else if( CanWeMark(&currentGameState) )                         // Priorite 2 : si on peut marquer
    {
        FinalReturn( ChooseMarkMove(tabLength) );
    }
    else if( CanWeProtect(potentialMoves, tabLength) )              // Priorite 3 : Si on peut proteger
    {
        FinalReturn( ChooseProtectMove(tabLength) );
    }
    else if( CanWeEat(potentialMoves, tabLength) )                  // Priorite 2 : Si on peut manger
    {
        FinalReturn( ChooseEatMove(tabLength) );
    }
    else                                                            // Choix par defaut (aucun meilleur a priori)
    {
        FinalReturn( ChooseDefaultMove(tabLength) );
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void UpdateAfterDecision(const int previousMoveIndex, const int exitPrison)
    * Met a jour la liste des mouvements potentiels de la strategie apres la choix d un mouvement
    * ainsi que l etat courant du plateau
    * @param const int previousMoveIndex : la position du mouvement effectue dans le tableau de mouvements potentiels
    * @param exitPrison : indique si le mouvement choisi est une sortie de prison
**/
void UpdateAfterDecision(const int previousMoveIndex, const int exitPrison)
{
	Strat_move lastMove = potentialMoves[previousMoveIndex];
    
	// 1 : MaJ du GameState
	currentGameState.zones[lastMove.from].nb_checkers--;
    if( !((lastMove.canEat == 1) || ((exitPrison) && (lastMove.priority == 4))) )    // On ne change pas le nombre de pion si on mange un ennemi (changement de couleur)
    {
        currentGameState.zones[lastMove.to].nb_checkers++;
    }
	currentGameState.zones[lastMove.to].player = EPlayer1;
	
    // 2 : MaJ des des
    int found = 0;
    int value = 1;
    while(!(found))
    {
        if(exitPrison)
        {
            if(dies[value] == (24 - lastMove.to))
            {
                found = 1;
                dies[value] = -1;
                dies[0]--;
            }
        }
        else if( (lastMove.to == EPos_OutP1) && (dies[value] > lastMove.from) )
        {
            found = 1;
            dies[value] = -1;
            dies[0]--;
        }
        else if(dies[value] == (lastMove.from - lastMove.to))
        {
            found = 1;
            dies[value] = -1;
            dies[0]--;
        }
        value++;
    }
	// 3 : On recommence l evaluation du plateau seulement s il reste des des
    if(dies[0] > 0)
	{
		ListPotentialMoves();
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int ChooseMarkMove(const int length)
 * Affecte des priorites aux mouvements marqueurs
 * On privilegie le rapprochement vers la sortie plutot que la sortie directe
 * @param const int length : la taille du tableau de mouvements
 * @return int : l index du mouvement choisi (-1 si aucun)
 *
 **/
int ChooseMarkMove(const int length)
{
    // Si on est la, tous les mouvements ont un canMark == 1 !
    int i = 0;
    
    while(i <= length)
    {
        if(potentialMoves[i].to != EPos_OutP1)
        {
            potentialMoves[i].priority = 2; // Si on se rapproche, on le prefere
        }
        else
        {
            potentialMoves[i].priority = 1;
        }
        i++;
    }
    
    return( FindMaxPriority2(potentialMoves, length) );
}

          
////////////////////////////////////////////////////////////////////////////////////////////////////


/** int ChooseEatMove(const int length)
  * Affecte des priorites aux mouvements mangeurs
  * @param const int length : la taille du tableau de mouvements
  * @return int : l index du mouvement choisi (-1 si aucun)
  *
 **/
int ChooseEatMove(const int length)
{
    int i = 0;
    int choice = -1;
    EPosition dangerous = EPos_nopos;
    while( i <= length )  // Il est plus important de manger le pion le plus proche de la sortie
    {
        if(potentialMoves[i].canEat)
        {
            if(potentialMoves[i].to <= dangerous)
            {
                choice = i;
                dangerous = potentialMoves[i].to;
            }
        }
        i++;
    }
   /* if(dangerous <= EPos_12)    // Si on peut manger seulement dans notre moitie de plateau, ce n'est pas forcement utile
    {
        // Cela peut etre utile si la sortie adverse de la prison est bloque
    }*/

    return choice;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int ChooseProtectMove(const int length)
 * Affecte des priorites aux mouvements protecteurs
 * @param const int length : la taille du tableau de mouvements
 * @return int : l index du mouvement choisi (-1 si aucun)
 *
 **/
int ChooseProtectMove(const int length)
{
    int i = 0;
    while( i <= length )
    {
        if( (potentialMoves[i].canProtect) && currentGameState.zones[potentialMoves[i].from].nb_checkers > 2 )
        {
            potentialMoves[i].priority = 3; // Si on protege sans mettre en danger
        }
        else if( (potentialMoves[i].canProtect) && (currentGameState.zones[potentialMoves[i].from].nb_checkers == 1) )
        {
            potentialMoves[i].priority = 2;
        }
        else if( (potentialMoves[i].canProtect) && (currentGameState.zones[potentialMoves[i].from].nb_checkers == 2) )
        {   
            potentialMoves[i].priority = 1;
            potentialMoves[i].proba = ProbaRisk(potentialMoves[i].from);
        }
        i++;
    }
    
    return ( FindMaxPriority(potentialMoves, length) );
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int ChooseDefaultMove(const int length)
  * Choisis un mouvement par defaut (si aucun autre n a ete determine avant)
  * - On regarde si on peut remplir les cases de 1 a 6 (la sortie de prison adverse) par au moins 2 de nos pions
  * - Sinon, on prend au hasard
  * @param length : la taille du tableau de mouvements
  * return int : l index du mouvement selectionne
*/
int ChooseDefaultMove(const int length)
{
    int i = 0;
    // Remplissage 1 a 6
    while(i <= length)  // Affectation de priorites
    {
        if( (potentialMoves[i].to <= EPos_6) && (potentialMoves[i].to != EPos_OutP1) )
        {
            if( (currentGameState.zones[potentialMoves[i].to].nb_checkers == 1) && (currentGameState.zones[potentialMoves[i].from].nb_checkers > 2) )
            {
                potentialMoves[i].priority = 4; // Priorite max si on protege un pion sans mettre en danger un autre (couverture gardee)
            }
            else if( (currentGameState.zones[potentialMoves[i].to].nb_checkers == 1) && (currentGameState.zones[potentialMoves[i].from].nb_checkers == 1) )
            {
                potentialMoves[i].priority = 3; // Idem avec une perte de zone
            }
            else if( (currentGameState.zones[potentialMoves[i].to].nb_checkers == 1) )
            {
                potentialMoves[i].priority = 2; // Mise en danger d un pion sur la zone de depart
                potentialMoves[i].proba = ProbaRisk(potentialMoves[i].from);
            }
            else if( (currentGameState.zones[potentialMoves[i].to].nb_checkers == 0)
                  && (currentGameState.zones[EPos_BarP2].nb_checkers == 0)
                  && (dies[5] == dies[4]) )
            {
                potentialMoves[i].priority = 1; // Mise en place d un seul pion mais avec aucun pion adverse a sortir de prison
                potentialMoves[i].proba = ProbaRisk(potentialMoves[i].to);
            }
        }
        i++;
    }

    i = FindMaxPriority(potentialMoves, length);
   
    if(i != -1)
    {
        return i;
    }
    
    
    // Choix au hasard
    if(length == 0 && potentialMoves[0].to == EPos_OutP1)   // Erreur si length == 0 (division)
    {
        return(-1);
    }
    else
    {
        int cpt = 0;
        int ran = rand() % length;
        do
        {
            ran = rand() % length;
            cpt++;
        }
        while( (potentialMoves[ran].canMark) && (cpt < length) );   // Si on boucle trop longtemps, c'est qu'uncun mouvement n'est possible

        if(cpt > length)
        {
            return(-1);
        }
	return(ran);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int probaRisk(const EPosition zone)
  * Calcule le risque d etre mange au tour suivant
  * @param const EPosition zone : la zone a evaluer
  * @preturn int : 1- la probalibilite d etre mange
**/
int ProbaRisk(const EPosition zone)
{
    if( ((currentGameState.zones[zone-6].player == EPlayer2) && (currentGameState.zones[zone-6].nb_checkers > 0))
        && ((zone > EPos_6) && (zone < EPos_19)) )
    {
        return 100-47;
    }
    if( ((currentGameState.zones[zone-5].player == EPlayer2) && (currentGameState.zones[zone-5].nb_checkers > 0))
        && ((zone > EPos_5) && (zone < EPos_20)) )
    {
        return 100-41;
    }
    if( ((currentGameState.zones[zone-4].player == EPlayer2) && (currentGameState.zones[zone-4].nb_checkers > 0))
        && ((zone > EPos_4) && (zone < EPos_21)) )
    {
        return 100-41;
    }
    if( ((currentGameState.zones[zone-3].player == EPlayer2) && (currentGameState.zones[zone-3].nb_checkers > 0))
        && ((zone > EPos_3) && (zone < EPos_22)) )
    {
        return 100-38;
    }
    if( ((currentGameState.zones[zone-2].player == EPlayer2) && (currentGameState.zones[zone-2].nb_checkers > 0))
        && ( (zone > EPos_2) && (zone < EPos_23) ))
    {
        return 100-33;
    }
    if( ((currentGameState.zones[zone-1].player == EPlayer2) && (currentGameState.zones[zone-1].nb_checkers > 0))
        && ( (zone > EPos_1) && (zone < EPos_24) ))
    {
        return 100-30;
    }
    if( ((currentGameState.zones[zone-7].player == EPlayer2) && (currentGameState.zones[zone-7].nb_checkers > 0))
       && ( (zone > EPos_7) && (zone < EPos_18) ))
    {
        return 100-16;
    }
    if( ((currentGameState.zones[zone-8].player == EPlayer2) && (currentGameState.zones[zone-8].nb_checkers > 0))
       && ( (zone > EPos_8) && (zone < EPos_17) ))
    {
        return 100-16;
    }
    if( ((currentGameState.zones[zone-9].player == EPlayer2) && (currentGameState.zones[zone-9].nb_checkers > 0))
       && ( (zone > EPos_9) && (zone < EPos_16) ))
    {
        return 100-13;
    }
    if( ((currentGameState.zones[zone-10].player == EPlayer2) && (currentGameState.zones[zone-10].nb_checkers > 0))
       && ( (zone > EPos_10) && (zone < EPos_15) ))
    {
        return 100-8;
    }
    if( ((currentGameState.zones[zone-12].player == EPlayer2) && (currentGameState.zones[zone-12].nb_checkers > 0))
       && ( (zone > EPos_12) && (zone < EPos_13) ))
    {
        return 100-8;
    }
    if( ((currentGameState.zones[zone-11].player == EPlayer2) && (currentGameState.zones[zone-11].nb_checkers > 0))
       && ( (zone > EPos_11) && (zone < EPos_14) ))
    {
        return 100-5;
    }
    return 2;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void FinalReturn(const int index)
  * Stocke les mouvements a renvoyer dans un tableau intermediaire
  * @paramconst int index : l index, dans le tableau de mouvements potentiels, de celui choisi
  *
**/
void FinalReturn(const int index)
{
    if(index != -1)
    {
        // Parcours du tableau
        int i = 0;
        while( (finalMoves[i].src_point != -1) && (i <= 3) )
        {
            i++;
        }
        finalMoves[i].src_point = potentialMoves[index].from;
        finalMoves[i].dest_point = potentialMoves[index].to;
        if(potentialMoves[index].from == EPos_BarP1)
        {   
            UpdateAfterDecision(index, 1);
        }
        else
        {
            UpdateAfterDecision(index, 0);
        }
    }
}

