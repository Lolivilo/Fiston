#ifndef __STRATEGY__
#define __STRATEGY__


#include "backgammon.h"



// Structure locale de la strategie representant un mouvement possible en fonction de l'etat courant du jeu
typedef struct
{
    EPosition from;		// Point de depart
	EPosition to;		// Point d arrivee
	int canEat;		// Possibilite de manger (booleen)
	int canMark;	// Possibilite de marquer (booleen)
    int canProtect; // Possibilite de proteger (booleen)
    int priority;
} Strat_move;

// Prototypes de fonctions
void ListPotentialMoves();
void FillPotentialMoves(EPosition start, int length, int moveNumber);
void IsEligibleForRelease();
void AnalysePlateau();
void UpdateAfterDecision(int previousMoveIndex, int exitPrison);
void EvaluateToExit(Strat_move* move);
void ChooseMove(int tabLength);
void FinalReturn(int index);
int ChooseMarkMove(int length);
int ChooseEatMove(int length);
int ChooseProtectMove(int length);
int ChooseDefaultMove(int length);
#endif
