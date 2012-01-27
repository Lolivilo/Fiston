#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backgammon.h"



void InitLibrary(char name[50])
{
	strcpy(name, "Strategie_Olivier_&_Oussama");
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void StartMatch(const unsigned int target_score)
{
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void EndMatch()
{
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int DoubleStack(const SGameState * const gameState)
{
    int CasesRestantesJ1 = 0; 
    int CasesRestantesJ2 = 0;
    EPosition i;
    
    // On calcule la distance restante à jouer pour chaque pion de chaque zone
    for( i = EPos_1 ; i <= EPos_24 ; i++ )
    {
        if( gameState->zones[i].player == EPlayer1 )
        {   
            CasesRestantesJ1 = CasesRestantesJ1 + ( (24-(int)i) * gameState->zones[i].nb_checkers );                                                                             
        }
        else if( gameState->zones[i].player == EPlayer2 )
        {
            CasesRestantesJ2 = CasesRestantesJ2 + ( ((int)i+1) * gameState->zones[i].nb_checkers );
        }
        // S il a des prisonners adverses, on prend le cas ou ils sortiront avec un de de valeur 6
        CasesRestantesJ2 += gameState->zones[EPos_BarP2].nb_checkers * 19;
    }
    
    // On deduit, des distances calculees, les decisions a prendre
    if( (CasesRestantesJ1 <= (0.70 * CasesRestantesJ2)) && (gameState->zones[EPos_BarP1].nb_checkers == 0) )
    {
        return 1;   // Si on a 70% de mouvements en moins que EPlayer2 a effectuer, on propose de doubler
    }
    if( (CasesRestantesJ1 <= (0.70 * CasesRestantesJ2)) && (gameState->zones[EPos_BarP1].nb_checkers == 1) )
    {
        for( i = EPos_24 ; i >= EPos_19 ; i-- )     // Si on a un prisonner, on regarde si on peut sortir
        { 
            if( (gameState->zones[i].nb_checkers == 0)
             || (gameState->zones[i].player == EPlayer1)
             || ((gameState->zones[i].player == EPlayer2) && (gameState->zones[i].nb_checkers == 1)) )
            {
                return 1;   // Si oui, on propose
            }
        }
    }
    return 0;
}

       
////////////////////////////////////////////////////////////////////////////////////////////////////


int TakeDouble(const SGameState * const gameState)
{
	if( DoubleStack(gameState) )
    {
        return 1;
    }
    return 0;
}