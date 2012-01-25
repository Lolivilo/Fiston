#include <stdio.h>
#include <stdlib.h>
#include "strategy.h"


int main()
{
	// Creation d'un SGameState d'origine
	SZone plateau[28];
	
	plateau[0].player = EPlayer1;
	plateau[0].nb_checkers = 0;
	
	plateau[1].player = EPlayer1;
	plateau[1].nb_checkers = 3;
	
	plateau[2].player = EPlayer2;
	plateau[2].nb_checkers = 0;
	
	plateau[3].player = EPlayer1;
	plateau[3].nb_checkers = 1;
	
	plateau[4].player = EPlayer1;
	plateau[4].nb_checkers = 6;
	
	plateau[5].player = EPlayer1;
	plateau[5].nb_checkers = 5;
	
	plateau[6].player = EPlayer1;
	plateau[6].nb_checkers = 0;
	
	plateau[7].player = EPlayer1;
	plateau[7].nb_checkers = 0;
	
	plateau[8].player = EPlayer2;
	plateau[8].nb_checkers = 0;
	
	plateau[9].player = EPlayer1;
	plateau[9].nb_checkers = 0;
	
	plateau[10].player = EPlayer1;
	plateau[10].nb_checkers = 0;
	
	plateau[11].player = EPlayer2;
	plateau[11].nb_checkers = 5;

	plateau[12].player = EPlayer1;
	plateau[12].nb_checkers = 0;

	plateau[13].player = EPlayer2;
	plateau[13].nb_checkers = 0;

	plateau[14].player = EPlayer2;
	plateau[14].nb_checkers = 0;

	plateau[15].player = EPlayer2;
	plateau[15].nb_checkers = 0;

	plateau[16].player = EPlayer2;
	plateau[16].nb_checkers = 3;

	plateau[17].player = EPlayer2;
	plateau[17].nb_checkers = 0;

	plateau[18].player = EPlayer2;
	plateau[18].nb_checkers = 5;

	plateau[19].player = EPlayer2;
	plateau[19].nb_checkers = 0;

	plateau[20].player = EPlayer2;
	plateau[20].nb_checkers = 0;

	plateau[21].player = EPlayer2;
	plateau[21].nb_checkers = 0;

	plateau[22].player = EPlayer1;
	plateau[22].nb_checkers = 0;

	plateau[23].player = EPlayer1;
	plateau[23].nb_checkers = 0;

    plateau[24].player = EPlayer1;  // Sortie
    plateau[24].nb_checkers = 0;
    
    plateau[25].player = EPlayer1;  // Prison
    plateau[25].nb_checkers = 0;
    
    
	SGameState etat;
	int i;
	for(i = 0; i <= 25; i++)
	{
		etat.zones[i] = plateau[i];
	}
	etat.die1 = 6;
	etat.die2 = 3;
	etat.score = 0;
	etat.scoreP2 = 0;
	etat.stake = 0;
	
    SMove moves[4];
    
    int j = 0;
    for(j = 0 ; j < 4; j++)
    {
        moves[j].src_point = EPos_1;
        moves[j].dest_point = EPos_1;
    }
    StartGame();
	MakeDecision(&etat, moves, 0);
    int a = 0;
    printf("\n\nTableau renvoye : \n");
    for(a = 0 ; a <= 3 ; a++)
    {
        printf("moves[%d].src_point = %d\tmoves[%d].dest_point = %d\n", a, moves[a].src_point, a, moves[a].dest_point);
    }
    EndGame();
    
    
	return 0;
}