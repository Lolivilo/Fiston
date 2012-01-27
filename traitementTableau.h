#ifndef _traitementTableau_h
#define _traitementTableau_h

#include "strategy.h"



// Prototypes (voir traitementTableau.c pour plus d informations)
void ResetTab(SMove* const tab);
void CopyTab(SMove* const dest, const SMove* const src);
int FindMinProba(const Strat_move* const tab, const int length);
int FindMaxPriority(const Strat_move* const tab, const int length);
int FindMaxPriority2(const Strat_move* const tab, const int length);

#endif