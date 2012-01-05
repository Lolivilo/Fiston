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
	ListPotentialMoves2();
}


/** Liste l ensemble des mouvements possibles par la strategie
  *
**/
void ListPotentialMoves()
{
	int fourDies = 0;	// A considerer comme un booleen
	
	// Remise a zero du tableau de mouvements
	int i;
	for(i = 0 ; i < MAX_POTENTIAL_MOVES ; i++)
	{
		potentialMoves[i].from = -1;
		potentialMoves[i].to = -1;
		potentialMoves[i].canEat = 0;
		potentialMoves[i].canMark = 0;
	}
	i = 0;
	
	// Si les deux des on la meme valeur, alors on a quatre mouvements disponibles
	if(dies[0] == 1)
	{
		fourDies = 1;
	}
	
	
	// VERIFIER QUIL NY A AUCUN PION AU MILIEU !!!!!!!!!!!!!!
	
	
	// Parcours de chaque zone
	int zone;
	for(zone = 0 ; zone <= 23 ; zone++)
	{
		// Si la zone courante nous appartient, on peut peut-etre effectuer un mouvement
		if(currentGameState.zones[zone].player == EPlayer1 && currentGameState.zones[zone].nb_checkers > 0)
		{
			// De 1
			if((currentGameState.zones[zone + dies[1]].player == EPlayer1)	// Si la zone d arrivee nous appartient
		   || (currentGameState.zones[zone + dies[1]].nb_checkers == 0)		// Ou si la zone d arrive n a aucun pion
		   || (currentGameState.zones[zone + dies[1]].player == EPlayer2		// Ou si la zone est adverse avec un seul pion
		       && currentGameState.zones[zone + dies[1]].nb_checkers == 1))
			{/*
				// Si le mouvement depasse la 24eme zone, il va dans la zone de marque : il faut verifier qu on a le droit de marquer
				if((zone + currentGameState->die1) >= 24)
				{
					// Si un seul de nos pions est dans une zone inferieure a 18, on ne peut pas marquer
					int j = 17;
					int allowedToMark = 1;
					while(!(allowedToMark))
					{
						if(currentGameState->zones[j].player == EPlayer1 && currentGameState->zones[j].nb_checkers > 0)
						{	
							allowedToMark = 0;
						}
						j--;
					}
					if(allowedToMark)
					{
						potentialMoves[i].from = zone;
						potentialMoves[i].to = 24;
						i++;	
					}
				}*/
				potentialMoves[i].from = zone;
				potentialMoves[i].to = zone + currentGameState.die1;
				PriorityLevel(&potentialMoves[i]);	// Evaluation du mouvement tout juste ajoute
				i++;
			}
			// De 2
			// S il y a 4 mouvements disponibles, alors tous les des sont egaux ; pas besoin de reverifier
			if(!(fourDies))
			{
				if(currentGameState.zones[zone + dies[2]].player == EPlayer1
				|| currentGameState.zones[zone + dies[2]].nb_checkers == 0
				|| (currentGameState.zones[zone + dies[2]].player == EPlayer2
				    && currentGameState.zones[zone + dies[2]].nb_checkers == 1))
				{/*
					// Si le mouvement depasse la 24eme zone, il va dans la zone de marque : il faut verifier qu on a le droit de marquer
					if((zone + currentGameState->die1) >= 24)
					{
						// Si un seul de nos pions est dans une zone inferieure a 18, on ne peut pas marquer
						int j = 17;
						int allowedToMark = 1;
						while(!(allowedToMark))
						{
							if(currentGameState->zones[j].player == EPlayer1 && currentGameState->zones[j].nb_checkers > 0)
							{	
								allowedToMark = 0;
							}
							j--;
						}
						if(allowedToMark)
						{
							potentialMoves[i].from = zone;
							potentialMoves[i].to = 24;
							i++;	
						}
					}*/
					potentialMoves[i].from = zone;
					potentialMoves[i].to = zone + currentGameState.die2;
					PriorityLevel(&potentialMoves[i]);
					i++;
				}
			}
		}
	}
	int j = 0;
	while(potentialMoves[j].from != -1)
	{
		printf("MOUVEMENT %d : Depart de %d ; Arrivee de %d ; Mangeur?%d ; Marqueur?%d\n", j, potentialMoves[j].from, potentialMoves[j].to, potentialMoves[j].canEat, potentialMoves[j].canMark);
		j++;
	}
	ChooseMove(potentialMoves);
}

void ListPotentialMoves2()
{
	// Remise a zero du tableau de mouvements
	int i;
	for(i = 0 ; i < MAX_POTENTIAL_MOVES ; i++)
	{
		potentialMoves[i].from = -1;
		potentialMoves[i].to = -1;
		potentialMoves[i].canEat = 0;
		potentialMoves[i].canMark = 0;
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
				if((currentGameState.zones[zone + dies[1]].player == EPlayer1)	// Si la zone d arrivee nous appartient
	   		|| (currentGameState.zones[zone + dies[1]].nb_checkers == 0)		// Ou si la zone d arrive n a aucun pion
	   		|| (currentGameState.zones[zone + dies[1]].player == EPlayer2		// Ou si la zone est adverse avec un seul pion
	       	 && currentGameState.zones[zone + dies[1]].nb_checkers == 1))
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
				if((currentGameState.zones[zone + dies[2]].player == EPlayer1)		// Si la zone d arrivee nous appartient
	   		|| (currentGameState.zones[zone + dies[2]].nb_checkers == 0)		// Ou si la zone d arrive n a aucun pion
	   		|| (currentGameState.zones[zone + dies[2]].player == EPlayer2		// Ou si la zone est adverse avec un seul pion
	      	 && currentGameState.zones[zone + dies[2]].nb_checkers == 1))
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
				if((currentGameState.zones[zone + dies[3]].player == EPlayer1)		// Si la zone d arrivee nous appartient
	   		|| (currentGameState.zones[zone + dies[3]].nb_checkers == 0)		// Ou si la zone d arrive n a aucun pion
	   		|| (currentGameState.zones[zone + dies[3]].player == EPlayer2		// Ou si la zone est adverse avec un seul pion
	      	 && currentGameState.zones[zone + dies[3]].nb_checkers == 1))
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
				if((currentGameState.zones[zone + dies[4]].player == EPlayer1)		// Si la zone d arrivee nous appartient
	   		|| (currentGameState.zones[zone + dies[4]].nb_checkers == 0)		// Ou si la zone d arrive n a aucun pion
	   		|| (currentGameState.zones[zone + dies[4]].player == EPlayer2		// Ou si la zone est adverse avec un seul pion
	      	 && currentGameState.zones[zone + dies[4]].nb_checkers == 1))
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
		printf("MOUVEMENT %d : Depart de %d ; Arrivee de %d ; Mangeur?%d ; Marqueur?%d\n", j, potentialMoves[j].from, potentialMoves[j].to, potentialMoves[j].canEat, potentialMoves[j].canMark);
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
	if(zoneArrivee.player == EPlayer2 && zoneArrivee.nb_checkers == 1)
	{
		move->canEat = 1;
	}
	if(move->to > 23)
	{
		move->canMark = 1;
	}
}


/** ChooseMove
  * Selectionne un mouvement dans le tableau de mouvements potentiels
  * &param Strat_move currentList[MAX_POTENTIAL_MOVES] : liste des mouvements potentiels
**/
void ChooseMove(Strat_move currentList[MAX_POTENTIAL_MOVES])
{
	// Priorite 1 : Si mouvement permet de manger un pion adverse
	int i = 0;
	int choosen = 0;
	while(currentList[i].from != -1 && !(choosen))
	{
		// SELECTIONNER QUEL PION A MANGER SI PLUSIEURS
		if(currentList[i].canEat)
		{
			choosen = 1;
			mov[0].src_point = currentList[i].from;
			mov[0].dest_point = currentList[i].to;
			printf("Choix du mouvement %d\n", i);	// A ENLEVER PLUS TARD : AFFICHAGE DU MOUVEMENT CHOISI
			printf("Appel : UpdateAfterDecision(%d)\n", i);	// A ENLEVER PLUS TARD
		}
		i++;
	}
	if(!(choosen))	// A ENLEVER PLUS TARD : SI AUCUN CHOIX, ON PREND LE PREMIER MOUVEMENT
	{
		mov[0].src_point = currentList[0].from;
		mov[0].dest_point = currentList[0].to;
		printf("Choix du mouvement %d\n", 0);	// A ENLEVER PLUS TARD : AFFICHAGE DU MOUVEMENT CHOISI
		printf("Appel : UpdateAfterDecision(%d)\n", 0);	// A ENLEVER PLUS TARD
	}
	// Il faut enlever le de utilise
	int found = 0;
	int k = 1;
	while(!(found))
	{
		if(dies[k] == (mov[0].dest_point - mov[0].src_point))
		{
			found = 1;
			dies[k] = -1;
		}
		k++;
	}
	//A SUPPRIMER : AFFICHAGE DE LA LISTE DES DES APRES UN CHOIX
	printf("DES : | %d | %d | %d | %d |\n", dies[1], dies[2], dies[3], dies[4]);	
	// Appel de la fonction qui met a jour la liste des mouvements

	UpdateAfterDecision(i-1);
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
	currentGameState.zones[lastMove.from].nb_checkers -= 1;
	currentGameState.zones[lastMove.to].nb_checkers += 1;
	currentGameState.zones[lastMove.to].player = EPlayer1;
	// 2 : MaJ des des
	// 3 : On recomment l evaluation du plateau seulement s il reste des des
	if(!(dies[1] == -1 && dies[2] == -1 && dies[3] == -1 && dies[4] == -1 ))
	{
		ListPotentialMoves2();
	}
}