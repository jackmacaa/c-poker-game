#include <stdio.h>
#include "card.h"

static const char *RANK_NAMES[] = {
    "?", "?", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

static const char *SUIT_NAMES[] = {
    "C", "D", "H", "S"};

const char *rank_to_string(int rank)
{
    if (rank < 2 || rank > 14)
    {
        return "?";
    }
    return RANK_NAMES[rank];
}

const char *suit_to_string(int suit)
{
    if (suit < 0 || suit > 3)
    {
        return "?";
    }
    return SUIT_NAMES[suit];
}

void print_card(Card card)
{
    printf("%s%s", rank_to_string(card.rank), suit_to_string(card.suit));
}
