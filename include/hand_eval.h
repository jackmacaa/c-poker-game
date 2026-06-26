#ifndef HAND_EVAL_H
#define HAND_EVAL_H

#include "card.h"

typedef enum {
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH
} HandCategory;

typedef struct {
    HandCategory category;
    int tiebreak[BEST_HAND_CARDS];
} HandValue;

HandValue evaluate_five_card_hand(const Card cards[BEST_HAND_CARDS]);
HandValue evaluate_best_of_seven(const Card cards[MAX_SEVEN_CARDS]);
int compare_hand_values(HandValue a, HandValue b);
const char *hand_category_name(HandCategory category);

#endif