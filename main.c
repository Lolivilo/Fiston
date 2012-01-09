#include <stdio.h>
#include <stdlib.h>
#include "backgammon.h"
#include "strategy.h"


int main()
{
	// Creation d'un SGameState d'origine
	SZone plateau[28];
	
	plateau[0].player = EPlayer1;
	plateau[0].nb_checkers = 2;
	
	plateau[1].player = EPlayer1;
	plateau[1].nb_checkers = 0;
	
	plateau[2].player = EPlayer1;
	plateau[2].nb_checkers = 0;
	
	plateau[3].player = EPlayer2;
	plateau[3].nb_checkers = 1;
	
	plateau[4].player = EPlayer2;
	plateau[4].nb_checkers = 0;
	
	plateau[5].player = EPlayer2;
	plateau[5].nb_checkers = 4;
	
	plateau[6].player = EPlayer1;
	plateau[6].nb_checkers = 0;
	
	plateau[7].player = EPlayer2;
	plateau[7].nb_checkers = 3;
	
	plateau[8].player = EPlayer1;
	plateau[8].nb_checkers = 0;
	
	plateau[9].player = EPlayer1;
	plateau[9].nb_checkers = 0;
	
	plateau[10].player = EPlayer1;
	plateau[10].nb_checkers = 0;
	
	plateau[11].player = EPlayer1;
	plateau[11].nb_checkers = 5;

	plateau[12].player = EPlayer2;
	plateau[12].nb_checkers = 5;

	plateau[13].player = EPlayer1;
	plateau[13].nb_checkers = 0;

	plateau[14].player = EPlayer1;
	plateau[14].nb_checkers = 0;

	plateau[15].player = EPlayer1;
	plateau[15].nb_checkers = 0;

	plateau[16].player = EPlayer1;
	plateau[16].nb_checkers = 3;

	plateau[17].player = EPlayer1;
	plateau[17].nb_checkers = 0;

	plateau[18].player = EPlayer1;
	plateau[18].nb_checkers = 5;

	plateau[19].player = EPlayer2;
	plateau[19].nb_checkers = 0;

	plateau[20].player = EPlayer1;
	plateau[20].nb_checkers = 0;

	plateau[21].player = EPlayer1;
	plateau[21].nb_checkers = 0;

	plateau[22].player = EPlayer1;
	plateau[22].nb_checkers = 0;

	plateau[23].player = EPlayer2;
	plateau[23].nb_checkers = 2;

	SGameState etat;
	int i;
	for(i = 0; i <= 23; i++)
	{
		etat.zones[i] = plateau[i];
	}
	etat.die1 = 3;
	etat.die2 = 5;
	etat.score = 0;
	etat.scoreP2 = 0;
	etat.stake = 0;
	
	MakeDecision(&etat, NULL, 0);
	return 0;
}