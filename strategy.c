#include <stdio.h>
#include <stdlib.h>
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
SMove finalMoves[4];


void InitLibrary(char name[50])
{
	
}

void StartMatch(const unsigned int target_score)
{
	
}

void StartGame()
{
	
}

void EndGame()
{
	
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
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/** void ListPotentialMoves()
  * Liste l ensemble des mouvements potentiellement réalisables 
**/
void ListPotentialMoves()
{
	// Remise a zero du tableau de mouvements
    potentialMoves = realloc(potentialMoves, 0);
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
        AnalysePlateau();
        ChooseMove(moveNumber - 1);
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
                printf("Sortie %d sur %d ; Mangeur = %d ; Protecteur = %d\n", moveNumber, potentialMoves[moveNumber-1].to, potentialMoves[moveNumber-1].canEat, potentialMoves[moveNumber-1].canMark);
            }
        }
    }
    if(!(canPlay))  // Si on ne peut pas jouer, notre tour est termine
    {
        printf("TOUR TERMINE !!!!!\n");
    }
    else
    {
        ChooseMove(moveNumber - 1);
    }
}

void FillPotentialMoves(EPosition start, int die, int moveNumber)
{
    // Augmentation de la taille du tableau de 1
    potentialMoves = (Strat_move*) realloc(potentialMoves, (moveNumber+1) * sizeof(Strat_move));
    potentialMoves[moveNumber].from = start;
    if(start == EPos_BarP1) // Si le mouvement vient de la prison, l arrivee n est pas calculee de la meme facon
    {
        potentialMoves[moveNumber].to = 24 - die;
        potentialMoves[moveNumber].isPrisonner = 1;
    }
    else
    {
        if((int)(start - die) < 0)
        {
            potentialMoves[moveNumber].to = EPos_OutP1;
            potentialMoves[moveNumber].isPrisonner = 0;
        }
        else
        {
            potentialMoves[moveNumber].to = start - die;
            potentialMoves[moveNumber].isPrisonner = 0;
        }
    }
    potentialMoves[moveNumber].canEat = 0;
    potentialMoves[moveNumber].canMark = 0;
    potentialMoves[moveNumber].canProtect = 0;
    potentialMoves[moveNumber].priority = 0;
    PriorityLevel(&potentialMoves[moveNumber]);
}


void AnalysePlateau()
{
    // Analyse du plateau pour voir si on a un pion seul
    //int zone;
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
// UNE SEULE FOIS L AFFECTATION DANS LE TABLEAU FINAL ???!!!



/** ChooseMove
  * Selectionne un mouvement dans le tableau de mouvements potentiels
  * @param Strat_move currentList[MAX_POTENTIAL_MOVES] : liste des mouvements potentiels
**/
void ChooseMove(int tabLength)
{
    printf("Appel de ChooseMove(%d)\n", tabLength);
    int i = 0;
	int choosen = 0;
    int exitPrison = 0;
    // Si deux mouvements permettent une avancee protegee (deux pions sur une zone), on joue ces mouvements
    /*EPosition securedAdvanceDest = FindSecuredAdvance();
    if(securedAdvanceDest == EPos_24)
    {
        printf("LOOOOOL\n");
    }*/
    
    // Priorite 1 : Si on a des prisonniers
    if(potentialMoves[0].isPrisonner)   // S il y a des prisonniers, seuls eux sont listes donc pas besoin de parcourir la liste
    {
        int max = 0;
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
        finalMoves[0].src_point = potentialMoves[i-1].from;
        finalMoves[0].dest_point = potentialMoves[i-1].to;
        choosen = 1;
        exitPrison = 1;
    }
    
    // Priorite 2 : Si on peut proteger un pion seul
    while((i <= tabLength) && !(choosen))
	{
		// SELECTIONNER QUEL PION A MANGER SI PLUSIEURS
		if(potentialMoves[i].canProtect)
		{
			choosen = 1;
		}
		i++;
	}

	// Priorite 3 : Si mouvement permet de manger un pion adverse
	if(!(choosen))
    {   
        i = 0;
    }
	while((i <= tabLength) && !(choosen))
	{
		// SELECTIONNER QUEL PION A MANGER SI PLUSIEURS
		if(potentialMoves[i].canEat)
		{
			choosen = 1;
		}
		i++;
	}
	if(!(choosen))	// A ENLEVER PLUS TARD : SI AUCUN CHOIX, ON PREND LE PREMIER MOUVEMENT
	{
        i = 1;
	}
    // On copie le mouvement dans le tableau d envoi final
    finalMoves[0].src_point = potentialMoves[i-1].from;
    finalMoves[0].dest_point = potentialMoves[i-1].to;
    printf("Choix du mouvement %d -> %d\n", potentialMoves[i-1].from, potentialMoves[i-1].to);	// A ENLEVER PLUS TARD : AFFICHAGE DU MOUVEMENT CHOISI
	// Appel de la fonction qui met a jour la liste des mouvements
    if(!(choosen))
    {
        UpdateAfterDecision(0, exitPrison);
    }
    else
    {
        UpdateAfterDecision(i-1, exitPrison);
    }
}


/** UpdateAfterDecision
    * Met a jour la liste des mouvements potentiels de la strategie apres la choix d un premier mouvement
    * ainsi que l etat courant du plateau
    * @param previousMoveIndex : la position, dans la liste, mouvement precedemment effectue
    * @param exitPrison : indique si le mouvement choisit est une sortie de prison
**/
void UpdateAfterDecision(int previousMoveIndex, int exitPrison)
{
	Strat_move lastMove = potentialMoves[previousMoveIndex];
	// 1 : MaJ du GameState
	currentGameState.zones[lastMove.from].nb_checkers--;
    if(lastMove.canEat != 1)    // On ne change pas le nombre de pion si on mange un ennemi (changement de couleur)
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
	//if(!(dies[1] == -1 && dies[2] == -1 && dies[3] == -1 && dies[4] == -1 ))
    if(dies[0] > 0)
	{
		ListPotentialMoves();
	}
}

          
////////////////////////////////////////////////////////////////////////////////////////////////////


/** int FindSecuredAdvance()
  *
  * Determine si deux mouvements permettant une avancee securisee (deux pions sur une meme zone) sont possibles
  * @return EPosition : la zone destination de l avancee securisee
**/
EPosition FindSecuredAdvance()
{
    
    return EPos_1;
}