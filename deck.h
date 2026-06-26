#ifndef DECK_H
#define DECK_H

#include "card.h"

typedef struct {
    Card cards[DECK_SIZE];
    int next_index;
} Deck;

void deck_init(Deck *deck);
void deck_shuffle(Deck *deck);
Card deck_deal(Deck *deck);

#endif