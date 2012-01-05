#ifndef __STRATEGY__
#define __STRATEGY__


#define MAX_POTENTIAL_MOVES 100



// Structure locale de la strategie representant un mouvement possible en fonction de l'etat courant du jeu
typedef struct
{
	int from;		// Point de depart
	int to;			// Point d arrivee
	int canEat;		// Possibilite de manger (booleen)
	int canMark;	// Possibilite de marquer (booleen)
} Strat_move;

// Prototypes de fonctions
void ListPotentialMoves();
void UpdateAfterDecision(int previousMoveIndex);
void PriorityLevel(Strat_move* move);
void ChooseMove(Strat_move currentList[MAX_POTENTIAL_MOVES]);

#endif