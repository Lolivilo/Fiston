#ifndef _analysePlateau_h
#define _analysePlateau_h

#include "strategy.h"


// Prototypes (voir analysePlateau.c pour plus d informations)
void MoveType(const SGameState* const board, const Strat_move* move);
int CanWeMark(const SGameState* const board);
int CanWeEat( Strat_move* const tab, const int length);
int CanWeProtect(const Strat_move* const tab, const int length);


#endif