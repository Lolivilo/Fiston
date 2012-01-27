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
    int proba;
} Strat_move;


// Prototypes de fonctions
void ListPotentialMoves();
void IsEligibleForRelease();
void FillPotentialMoves(const EPosition start, const int die, const int moveNumber);
void EvaluateToExit(Strat_move* const move);
void ChooseMove(const int tabLength);
void UpdateAfterDecision(const int previousMoveIndex, const int exitPrison);
int ChooseMarkMove(const int length);
int ChooseEatMove(const int length);
int ChooseProtectMove(const int length);
int ChooseDefaultMove(const int length);
int ProbaRisk(const EPosition zone);
void FinalReturn(const int index);


#endif