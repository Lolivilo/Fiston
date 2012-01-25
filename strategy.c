#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "strategy.h"

// Soit on sort du plateau
// Soit on arrive sur une zone a nous
// Soit la zone est vide
// Soit elle est adverse avec un seul pion
#define IF_PLAYABLE(de) if( (((int)zone-de) <= 0) \
                        || (currentGameState.zones[zone - de].player == EPlayer1) \
                        || (currentGameState.zones[zone - de].nb_checkers == 0)	\
                        || (currentGameState.zones[zone - de].player == EPlayer2 && currentGameState.zones[zone - de].nb_checkers == 1)) \


// Variables locales
SGameState currentGameState;    // Copie locale de l etat courant du jeu
Strat_move* potentialMoves;     // Tableau de mouvements potentiels
int dies[5];	// | nombre_de_des | de1 | de2 | de3 | de4 |
SMove* finalMoves;


void InitLibrary(char name[50])
{
	strcpy(name, "Strategie_Olivier_&_Oussama");
}

void StartMatch(const unsigned int target_score)
{
	
}

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

void EndMatch()
{
	
}

int DoubleStack(const SGameState * const gameState)
{
	return 0;
}

int TakeDouble(const SGameState * const gameState)
{
	return 0;
}

void MakeDecision(const SGameState * const gameState, SMove moves[4], unsigned int lastTimeError)
{
	srand(time(NULL));	// Pour le random
	currentGameState = *gameState;  // Copie locale de l etat courant du jeu

    // RaZ du tableau de mouvements
    int i = 0;
    for(i = 0 ; i < 4; i++)
    {
        moves[i].src_point = EPos_nopos;
        moves[i].dest_point = EPos_nopos;
    }
    
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
	printf("DES INITIAUX : | %d | %d | %d | %d |\n", dies[1], dies[2], dies[3], dies[4]);	
	ListPotentialMoves();
    
    // Remplissage du tableau renvoye
    for(i = 0 ; i <= 3 ; i++)
    {
        moves[i] = finalMoves[i];
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void ListPotentialMoves()
  * Liste l ensemble des mouvements potentiellement réalisables 
**/
void ListPotentialMoves()
{
	// Remise a zero du tableau de mouvements
    potentialMoves = realloc(potentialMoves, 0);
    if(potentialMoves == NULL)
    {
        printf("Erreur d'allocation !\n");
        exit(0);
    }
    // Si un a un pion prisonnier, il faut le liberer avant toute chose
    if(currentGameState.zones[EPos_BarP1].nb_checkers > 0)
    {
        printf("TA %d PRISONNERS PATATE !!!\n", currentGameState.zones[EPos_BarP1].nb_checkers);
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
        int j = 0;
        for(j = 0 ; j <= (moveNumber - 1) ; j++)
        {
            printf("MOUVEMENT %d : Depart de %d ; Arrivee de %d ; Mangeur?%d ; Marqueur?%d ; Protecteur?%d\n", j, potentialMoves[j].from, potentialMoves[j].to, potentialMoves[j].canEat, potentialMoves[j].canMark, potentialMoves[j].canProtect);
        }
        if(!(moveNumber == 0))
        {
            ChooseMove(moveNumber - 1);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void IsEligibleForRelease()
  * Determine si un pion prisonnier peut etre libere
  * Si oui, affecte un priorite a chaque mouvement de sortie possible
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
                printf("Sortie n°%d sur %d ; priorite de %d\n", moveNumber, potentialMoves[moveNumber-1].to, potentialMoves[moveNumber-1].priority);
            }
        }
    }
    if(!(canPlay))  // Si on ne peut pas jouer, notre tour est termine
    {
        FinalReturn(-1);
        printf("TOUR TERMINE !!!!!\n");
    }
    else
    {
        ChooseMove(moveNumber - 1);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void FillPotentialMoves(EPosition start, int die, int moveNumber)
  * Remplie le tableau de mouvements potentiels
  @param EPosition start : le point de depart du mouvement a ajouter
         int die : la valeur du de utilise
         int moveNumber : le numero du mouvement a ajouter (pour redimensionner le tableau)
  *
**/
void FillPotentialMoves(EPosition start, int die, int moveNumber)
{
    // Augmentation de la taille du tableau de 1
    potentialMoves = (Strat_move*) realloc(potentialMoves, (moveNumber+1) * sizeof(Strat_move));
    potentialMoves[moveNumber].from = start;
    potentialMoves[moveNumber].canEat = 0;
    potentialMoves[moveNumber].canMark = 0;
    potentialMoves[moveNumber].canProtect = 0;
    potentialMoves[moveNumber].priority = 0;
    potentialMoves[moveNumber].isPrisonner = 0;
    if(start == EPos_BarP1) // Si le mouvement vient de la prison, l arrivee n est pas calculee de la meme facon
    {
        potentialMoves[moveNumber].to = 24 - die;
        potentialMoves[moveNumber].isPrisonner = 1;
        EvaluateToExit(&potentialMoves[moveNumber]);
    }
    else
    {
        if((int)(start - die) < 0)                      // Sortie pour marquer
        {
            potentialMoves[moveNumber].to = EPos_OutP1;
        }
        else                                            // Mouvement normal
        {
            potentialMoves[moveNumber].to = start - die;
        }
        PriorityLevel(&potentialMoves[moveNumber]);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** int CanWeMark()
  * Analyse le plateau et regarde si on peut marquer
  * @return int : 1 si on peut marquer ; 0 sinon
**/
int CanWeMark()
{
    // On parcours toutes les zones de 1 & 6 + la zone de marquage et on regarde si on a nos 16 pions
    EPosition current;
    int count = currentGameState.zones[EPos_OutP1].nb_checkers;
    for(current = EPos_1 ; current <= EPos_6 ; current++)
    {
        if(currentGameState.zones[current].player == EPlayer1)
        {
            count += currentGameState.zones[current].nb_checkers;
        }
    }
    if(count == 15)
    {
        return 1;
    }
    return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////


/** PriorityLevel
  * Calcule le niveau de priorite d un mouvement potentiel
  * @param Strat_move move : le mouvement a evaluer
**/
void PriorityLevel(Strat_move* move)
{
	SZone zoneArrivee = currentGameState.zones[move->to];
    // Peut-il proteger un pion seul ?
    if(zoneArrivee.player == EPlayer1 && zoneArrivee.nb_checkers == 1)
    {
        move->canProtect = 1;
        move->priority = 2;
        if(currentGameState.zones[move->from].nb_checkers == 2) // Si la protection rend un pion mangeable : moins prioritaire
        {
            move->priority = 1;
        }
       
    }
	if(zoneArrivee.player == EPlayer2 && zoneArrivee.nb_checkers == 1)
	{
		move->canEat = 1;
	}
	if(move->to == EPos_OutP1)
	{
		move->canMark = 1;
	}
}
    
    
////////////////////////////////////////////////////////////////////////////////////////////////////


/** EvaluateToExit()
  * Evalue chaque mouvement de sortie de prison et modifie la priorite de chacun
  * Priorite 1 : Si on peut manger un pion                      => 4
  * Priorite 2 : Si on peut protéger un de nos pions seuls      => 3
  * Priorite 3 : Si on peut aller sur une zone nous appartenant => 2
  * Priorite 4 : Si aucune autre solution                       => 1
**/
void EvaluateToExit(Strat_move* move)
{
    SZone exit = currentGameState.zones[move->to];
    if( (exit.player == EPlayer2) && (exit.nb_checkers == 1) )
    {
        move->priority = 4;
    }
    else if( (exit.player == EPlayer1) && (exit.nb_checkers == 1) )
    {
        move->priority = 3;
    }
    else if( (exit.player == EPlayer1) && (exit.nb_checkers > 1) )
    {
        move->priority = 2;
    }
    else
    {
        move->priority = 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// UNE SEULE FOIS L AFFECTATION DANS LE TABLEAU FINAL ???!!!



/** ChooseMove
  * Selectionne un mouvement dans le tableau de mouvements potentiels
  * @param Strat_move currentList[MAX_POTENTIAL_MOVES] : liste des mouvements potentiels
**/
void ChooseMove(int tabLength)
{
    int i = 0;
	int choosen = 0;
    
    // Priorite 1 : Si on a des prisonniers
    int max = 0;
    if(potentialMoves[0].isPrisonner)   // S il y a des prisonniers, seuls eux sont listes donc pas besoin de parcourir la liste
    {
        int lastPriority = -1;
        while(i <= tabLength)
        {
            // On prend le mouvement avec la priorite maximale
            if(potentialMoves[i].priority > lastPriority)
            {
                lastPriority = potentialMoves[i].priority;
                max = i;
            }
            i++;
        }
        printf("Choix du mouvement %d -> %d\n", potentialMoves[max].from, potentialMoves[max].to);
        FinalReturn(max);
        choosen = 1;
    }

    else if(CanWeMark())
    {
        int j = 1;
        int indexChoisi = 0;
        while(j <= tabLength)   // On cherche les pions les plus eloignes pour les rapprocher
        {
            if(potentialMoves[j].from > potentialMoves[j-1].from)
            {
                indexChoisi = j;
            }
            j++;
        }
        printf("Choix du mouvement %d -> %d\n", potentialMoves[indexChoisi].from, potentialMoves[indexChoisi].to);
        FinalReturn(j-1);
        choosen = 1;
    }
    else
    {
        // Priorite 2 : Si on peut proteger un pion seul
        while((i <= tabLength) && !(choosen))
        {
            if(potentialMoves[i].canProtect)
            {
                choosen = 1;
                if(i <= tabLength)     // On continue de parcourir seulement s'il y a d'autres mouvements
                {                           // Pour regarder s il y a une plus grande priorite
                    int currentPriority = potentialMoves[i].priority;
                    int iPrim = i + 1;
                    while( (iPrim <= tabLength) )
                    {
                        if( (potentialMoves[iPrim].canProtect) && (potentialMoves[iPrim].priority > currentPriority))
                        {
                            currentPriority = potentialMoves[iPrim].priority;
                            i = iPrim;
                        }
                        iPrim++;
                    }
                }
                FinalReturn(i);
            }
            i++;
        }

        // Priorite 3 : Si mouvement permet de manger un pion adverse
        if(!(choosen))
        {
            i = ChooseEatMove(tabLength);
            if(i != -1)
            {
                choosen = 1;
                FinalReturn(i);
            }
        }
        
        
        /// SI AUCUN MOUVEMENT BIEN ON PEUT REGARDER SI ON PEUT RAVANCER NOS PIONS DANS LES ZONES 1-6
        if(!(choosen))	// A ENLEVER PLUS TARD : SI AUCUN CHOIX, ON PREND LE PREMIER MOUVEMENT
        {
            i = ChooseDefaultMove(tabLength);
			printf("Choix du mouvement %d -> %d\n", potentialMoves[i].from, potentialMoves[i].to);
        	FinalReturn(i);
        }
        
    }
	// Appel de la fonction qui met a jour la liste des mouvements
    /*if(!(choosen))
    {
        UpdateAfterDecision(0, 0);
    }
    else if (choosen && potentialMoves[0].isPrisonner)
    {
        UpdateAfterDecision(max, 1);
    }
    else
    {
        UpdateAfterDecision(i, 0);
    }*/
}


/** UpdateAfterDecision
    * Met a jour la liste des mouvements potentiels de la strategie apres la choix d un premier mouvement
    * ainsi que l etat courant du plateau
    * @param previousMoveIndex : la position, dans la liste, du mouvement precedemment effectue
    * @param exitPrison : indique si le mouvement choisit est une sortie de prison
**/
void UpdateAfterDecision(int previousMoveIndex, int exitPrison)
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
        else if(dies[value] == (lastMove.from - lastMove.to))
        {
            found = 1;
            dies[value] = -1;
            dies[0]--;
        }
        value++;
    }
    //A SUPPRIMER : AFFICHAGE DE LA LISTE DES DES APRES UN CHOIX
	printf("DES : %d ||| %d | %d | %d | %d |\n", dies[0], dies[1], dies[2], dies[3], dies[4]);
	// 3 : On recomment l evaluation du plateau seulement s il reste des des
    if(dies[0] > 0)
	{
		ListPotentialMoves();
	}
}

          
////////////////////////////////////////////////////////////////////////////////////////////////////


/** Strat_move* ChooseEatMove()
  * Choisis le meilleur mouvement "mangeur" a jouer
  * @param int length : la taille du tableau de mouvements
  * @return int : l index du mouvement choisi (-1 si aucun)
  *
 **/
int ChooseEatMove(int length)
{
    int i = 0;
    int choice = -1;
    EPosition dangerous = EPos_nopos;
    while( i <= length )  // Il est plus important de manger le pion le plus proche de la sortie
    {
        if(potentialMoves[i].canEat)
        {
            if(potentialMoves[i].to >= dangerous)
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


/** int ChooseDefaultMove(int length)
  * Choisis un mouvement par defaut (si aucun autre n a ete determine avant)
  * - On regarde si on peut remplir les cases de 1 a 6 (la sortie de prison adverse) par au moins 2 de nos pions
  * - Sinon, on prend au hasard
  * @param length : la taille du tableau de mouvements
  * return int : l index du mouvement selectionne
*/
int ChooseDefaultMove(int length)
{
    int i = 0;
    
    // Remplissage 1 a 6
    while(i <= length)
    {
        if( (potentialMoves[i].to <= EPos_6) && (potentialMoves[i].to != EPos_OutP1) )
        {
            if(currentGameState.zones[potentialMoves[i].to].nb_checkers == 1)
            {
                return i;
            }
        }
        i++;
    }
    
    // Choix au hasard
	int ran = rand() % length;
	do
	{
		ran = rand() % length;
	}
	while(potentialMoves[ran].canMark);
	
	return(ran);
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void FinalReturn(Strat_move* move)
  * Stocke les mouvements a renvoyer dans un tableau intermediaire
  * @param Strat_move* move : le mouvement a stocker
  *
**/
void FinalReturn(int index)
{
    printf("Final return(%d)\n", index);
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
    }
    
    if(potentialMoves[index].from == EPos_BarP1)
    {   
        UpdateAfterDecision(index, 1);
    }
    else
    {
        UpdateAfterDecision(index, 0);
    }
}

