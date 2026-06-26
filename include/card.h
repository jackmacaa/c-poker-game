#ifndef CARD_H
#define CARD_H

#define DECK_SIZE 52
#define HOLE_CARDS 2
#define BOARD_CARDS 5
#define MAX_SEVEN_CARDS 7
#define BEST_HAND_CARDS 5

typedef struct {
    int rank; /* 2..14 where 14 is Ace */
    int suit; /* 0..3 => Clubs, Diamonds, Hearts, Spades */
} Card;

const char *rank_to_string(int rank);
const char *suit_to_string(int suit);
void print_card(Card card);

#endif