#include <stdio.h>
#include <stdlib.h>


/** int CanWeMark()
 * Analyse le plateau et regarde si on peut marquer
 * @return int : 1 si on peut marquer ; 0 sinon
 **/
int CanWeMark(SGameState* board)
{
    // On parcours toutes les zones de 1 & 6 + la zone de marquage et on regarde si on a nos 16 pions
    EPosition current;
    int count = board.zones[EPos_OutP1].nb_checkers;
    for(current = EPos_1 ; current <= EPos_6 ; current++)
    {
        if(board.zones[current].player == EPlayer1)
        {
            count += board.zones[current].nb_checkers;
        }
    }
    if(count == 15)
    {
        return 1;
    }
    return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
