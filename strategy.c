#include <stdio.h>
#include <stdlib.h>
#include "backgammon.h"
#include "strategy.h"



// Variables locales
SGameState currentGameState;
Strat_move potentialMoves[MAX_POTENTIAL_MOVES];
int dies[5];	// | 0 si 2 des ; 1 sinon | de1 | de2 | de3 | de4 |
SMove mov[4];


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
	currentGameState = *gameState;
	// Remplissage du tableau de des
	dies[1] = currentGameState.die1;
	dies[2] = currentGameState.die2;
	if(dies[1] == dies[2])
	{
		dies[3] = dies[1];
		dies[4] = dies[1];
		dies[0] = 1;
	}
	else
	{
		dies[3] = -1;
		dies[4] = -1;
		dies[0] = 0;
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
	int i;
	for(i = 0 ; i < MAX_POTENTIAL_MOVES ; i++)
	{
		potentialMoves[i].from = -1;
		potentialMoves[i].to = -1;
		potentialMoves[i].canEat = 0;
		potentialMoves[i].canMark = 0;
        potentialMoves[i].canProtect = 0;
	}
	i = 0;

	// Parcours de chaque zone
	int zone;
	for(zone = 0 ; zone <= 23 ; zone++)
	{
		// Si la zone courante nous appartient, on peut peut-etre effectuer un mouvement
		if(currentGameState.zones[zone].player == EPlayer1 && currentGameState.zones[zone].nb_checkers > 0)
		{
			// De 1
			if(dies[1] != -1)	// Si le de n est pas utilise
			{
				DEST(dies[1])  // Conditionnelle : teste si le point d arrivee est possible
				{
					potentialMoves[i].from = zone;
					potentialMoves[i].to = zone + dies[1];
					PriorityLevel(&potentialMoves[i]);	// Evaluation du mouvement tout juste ajoute
					i++;
				}
			}
			// De 2
			if(dies[2] != -1 && dies[2] != dies[1])
			{
				DEST(dies[2])  // Conditionnelle : teste si le point d arrivee est possible
				{
					potentialMoves[i].from = zone;
					potentialMoves[i].to = zone + dies[2];
					PriorityLevel(&potentialMoves[i]);	// Evaluation du mouvement tout juste ajoute
					i++;
				}
			}
			// De 3
			if(dies[3] != -1 && dies[3] != dies[1] && dies[3] != dies[2])			// Il faut que le de existe et que la valeur le soit pas deja evaluee
			{
				DEST(dies[3])  // Conditionnelle : teste si le point d arrivee est possible
                {
					potentialMoves[i].from = zone;
					potentialMoves[i].to = zone + dies[3];
					PriorityLevel(&potentialMoves[i]);	// Evaluation du mouvement tout juste ajoute
					i++;
				}
			}
			// De 4
			if(dies[4] != -1 && dies[4] != dies[1] && dies[4] != dies[2] && dies[4] != dies[3])	// Il faut que le de existe et que la valeur le soit pas deja evaluee
			{
				DEST(dies[4])  // Conditionnelle : teste si le point d arrivee est possible
				{
					potentialMoves[i].from = zone;
					potentialMoves[i].to = zone + dies[4];
					PriorityLevel(&potentialMoves[i]);	// Evaluation du mouvement tout juste ajoute
					i++;
				}
			}
		}
	}
	int j = 0;
	while(potentialMoves[j].from != -1)
	{
		printf("MOUVEMENT %d : Depart de %d ; Arrivee de %d ; Mangeur?%d ; Marqueur?%d ; Protecteur?%d\n", j, potentialMoves[j].from, potentialMoves[j].to, potentialMoves[j].canEat, potentialMoves[j].canMark, potentialMoves[j].canProtect);
		j++;
	}
	ChooseMove(potentialMoves);
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
	if(move->to > 23)
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
void ChooseMove()
{
    int i = 0;
	int choosen = 0;
    // Priorite 1 : Si on peut proteger un pion seul
    while(potentialMoves[i].from != -1 && !(choosen))
	{
		// SELECTIONNER QUEL PION A MANGER SI PLUSIEURS
		if(potentialMoves[i].canProtect)
		{
			choosen = 1;
		}
		i++;
	}

	// Priorite 2 : Si mouvement permet de manger un pion adverse
	if(!(choosen))
    {
        i = 0;
    }
	while(potentialMoves[i].from != -1 && !(choosen))
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
    mov[0].src_point = potentialMoves[i-1].from;
    mov[0].dest_point = potentialMoves[i-1].to;
    printf("Choix du mouvement %d -> %d\n", potentialMoves[i-1].from, potentialMoves[i-1].to);	// A ENLEVER PLUS TARD : AFFICHAGE DU MOUVEMENT CHOISI
    printf("Appel : UpdateAfterDecision(%d)\n", i-1);	// A ENLEVER PLUS TARD
	// Appel de la fonction qui met a jour la liste des mouvements
    if(!(choosen))
    {
        UpdateAfterDecision(0);
    }
    else
    {
        UpdateAfterDecision(i-1);
    }
}


/** UpdateAfterDecision
    * Met a jour la liste des mouvements potentiels de la strategie apres la choix d un premier mouvement
    * ainsi que l etat courant du plateau
    * @param previousMoveIndex : la position, dans la liste, mouvement precedemment effectue
**/
void UpdateAfterDecision(int previousMoveIndex)
{
	Strat_move lastMove = potentialMoves[previousMoveIndex];
	// 1 : MaJ du GameState
	printf("J'enlève un pion à la case %d\nJ'ajoute un pion à la case %d\n", lastMove.from, lastMove.to);	// A ENLEVER
	currentGameState.zones[lastMove.from].nb_checkers--;
    if(lastMove.canEat != 1)    // On ne change pas le nombre de pion si on mange un ennemi (changement de couleur)
    {
        currentGameState.zones[lastMove.to].nb_checkers++;
    }
    printf("Il reste %d pions sur la zone 0\n", currentGameState.zones[lastMove.from].nb_checkers); // A ENLEVER
	currentGameState.zones[lastMove.to].player = EPlayer1;
	// 2 : MaJ des des
    int found = 0;
    int value = 1;
    while(!(found))
    {
        if(dies[value] == (lastMove.to - lastMove.from))
        {
            found = 1;
            dies[value] = -1;
        }
        value++;
    }
    //A SUPPRIMER : AFFICHAGE DE LA LISTE DES DES APRES UN CHOIX
	printf("DES : | %d | %d | %d | %d |\n", dies[1], dies[2], dies[3], dies[4]);
	// 3 : On recomment l evaluation du plateau seulement s il reste des des
	if(!(dies[1] == -1 && dies[2] == -1 && dies[3] == -1 && dies[4] == -1 ))
	{
		ListPotentialMoves();
	}
}