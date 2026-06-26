#include <stdio.h>
#include "../include/card.h"
#include "../include/deck.h"
#include "../include/hand_eval.h"

static void print_cards(const Card *cards, int count)
{
    for (int i = 0; i < count; i++)
    {
        print_card(cards[i]);
        if (i < count - 1)
        {
            printf(" ");
        }
    }
    printf("\n");
}

static void collect_seven_cards(const Card hole[HOLE_CARDS], const Card board[BOARD_CARDS], Card out[MAX_SEVEN_CARDS])
{
    out[0] = hole[0];
    out[1] = hole[1];
    for (int i = 0; i < BOARD_CARDS; i++)
    {
        out[2 + i] = board[i];
    }
}

void play_holdem_round(void)
{
    Deck deck;
    Card player_hole[HOLE_CARDS];
    Card cpu_hole[HOLE_CARDS];
    Card board[BOARD_CARDS];
    Card player_seven[MAX_SEVEN_CARDS];
    Card cpu_seven[MAX_SEVEN_CARDS];

    deck_init(&deck);
    deck_shuffle(&deck);

    for (int i = 0; i < HOLE_CARDS; i++)
    {
        player_hole[i] = deck_deal(&deck);
        cpu_hole[i] = deck_deal(&deck);
    }

    /* Burn + flop */
    (void)deck_deal(&deck);
    board[0] = deck_deal(&deck);
    board[1] = deck_deal(&deck);
    board[2] = deck_deal(&deck);

    /* Burn + turn */
    (void)deck_deal(&deck);
    board[3] = deck_deal(&deck);

    /* Burn + river */
    (void)deck_deal(&deck);
    board[4] = deck_deal(&deck);

    collect_seven_cards(player_hole, board, player_seven);
    collect_seven_cards(cpu_hole, board, cpu_seven);

    HandValue player_value = evaluate_best_of_seven(player_seven);
    HandValue cpu_value = evaluate_best_of_seven(cpu_seven);

    printf("\n=== Texas Hold'em Round ===\n");
    printf("Your hole cards: ");
    print_cards(player_hole, HOLE_CARDS);
    printf("Board:           ");
    print_cards(board, BOARD_CARDS);
    printf("CPU hole cards:  ");
    print_cards(cpu_hole, HOLE_CARDS);

    printf("You: %s\n", hand_category_name(player_value.category));
    printf("CPU: %s\n", hand_category_name(cpu_value.category));

    int result = compare_hand_values(player_value, cpu_value);
    if (result > 0)
    {
        printf("Result: You win!\n");
    }
    else if (result < 0)
    {
        printf("Result: CPU wins.\n");
    }
    else
    {
        printf("Result: Tie.\n");
    }
}
