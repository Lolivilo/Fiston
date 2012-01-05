#include <stdio.h>
#include <stdlib.h>
#include "backgammon.h"

/** InitLibrary
	Initialise la strategie
	@param char name[50] : le nom de la librairie
*/
void InitLibrary(char name[50])
{
	printf("Strategie \"%s\" lancee\n", name);
}

/** StartMatch
    Débute un match (composée de plusieurs parties [games])
    @param int target_score : le score à atteindre pour gagner sur l'ensemble des parties
*/
void StartMatch(const unsigned int target_score)
{
    // Initialisation d'un SGameState
    SGameState* gameState = (SGameState*) malloc(sizeof(SGameState));
	gameState.die1 = 0;
	gameState.die2 = 0;
	gameState.score = 0;
	gameState.scoreP2 = 0;
	gameState.stake = 0;
}


/** EndMatch
	Termine un match
	Le gagnant et le score doivent être inscrits dans le fichier de log
*/
void EndMatch()
{
	// Désallocation du SGameState
	
}