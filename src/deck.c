#include <stdlib.h>
#include "../include/deck.h"

static void swap_cards(Card *a, Card *b)
{
    Card temp = *a;
    *a = *b;
    *b = temp;
}

void deck_init(Deck *deck)
{
    int index = 0;
    for (int suit = 0; suit < 4; suit++)
    {
        for (int rank = 2; rank <= 14; rank++)
        {
            deck->cards[index].rank = rank;
            deck->cards[index].suit = suit;
            index++;
        }
    }
    deck->next_index = 0;
}

void deck_shuffle(Deck *deck)
{
    for (int i = DECK_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap_cards(&deck->cards[i], &deck->cards[j]);
    }
    deck->next_index = 0;
}

Card deck_deal(Deck *deck)
{
    Card invalid = {0, 0};
    if (deck->next_index >= DECK_SIZE)
    {
        return invalid;
    }
    return deck->cards[deck->next_index++];
}
