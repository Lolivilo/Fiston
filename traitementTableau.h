#ifndef _traitementTableau_h
#define _traitementTableau_h

#include "strategy.h"




void ResetTab(SMove tab[4]);
void CopyTab(SMove* const dest, const SMove* const src);
int FindMaxPriority(Strat_move* tab, int length);
int FindMaxPriority2(Strat_move* tab, int length);

#endif
