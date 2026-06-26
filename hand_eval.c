#include <string.h>
#include "hand_eval.h"

static const char *CATEGORY_NAMES[] = {
    "High Card",
    "One Pair",
    "Two Pair",
    "Three of a Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four of a Kind",
    "Straight Flush"};

static void sort_desc(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int best = i;
        for (int j = i + 1; j < size; j++)
        {
            if (arr[j] > arr[best])
            {
                best = j;
            }
        }
        if (best != i)
        {
            int tmp = arr[i];
            arr[i] = arr[best];
            arr[best] = tmp;
        }
    }
}

static int detect_straight(const int rank_counts[15], int *high_straight_rank)
{
    for (int high = 14; high >= 5; high--)
    {
        int straight = 1;
        for (int r = high; r > high - 5; r--)
        {
            if (rank_counts[r] == 0)
            {
                straight = 0;
                break;
            }
        }
        if (straight)
        {
            *high_straight_rank = high;
            return 1;
        }
    }

    if (rank_counts[14] && rank_counts[2] && rank_counts[3] && rank_counts[4] && rank_counts[5])
    {
        *high_straight_rank = 5;
        return 1;
    }

    return 0;
}

HandValue evaluate_five_card_hand(const Card cards[BEST_HAND_CARDS])
{
    HandValue value;
    value.category = HIGH_CARD;
    memset(value.tiebreak, 0, sizeof(value.tiebreak));

    int rank_counts[15] = {0};
    int suit_counts[4] = {0};
    int ranks_desc[BEST_HAND_CARDS];

    for (int i = 0; i < BEST_HAND_CARDS; i++)
    {
        rank_counts[cards[i].rank]++;
        suit_counts[cards[i].suit]++;
        ranks_desc[i] = cards[i].rank;
    }
    sort_desc(ranks_desc, BEST_HAND_CARDS);

    int flush = 0;
    for (int s = 0; s < 4; s++)
    {
        if (suit_counts[s] == BEST_HAND_CARDS)
        {
            flush = 1;
            break;
        }
    }

    int high_straight_rank = 0;
    int straight = detect_straight(rank_counts, &high_straight_rank);

    int four = 0;
    int three = 0;
    int pairs[2] = {0, 0};
    int pair_count = 0;
    int singles[BEST_HAND_CARDS] = {0, 0, 0, 0, 0};
    int single_count = 0;

    for (int r = 14; r >= 2; r--)
    {
        if (rank_counts[r] == 4)
        {
            four = r;
        }
        else if (rank_counts[r] == 3)
        {
            three = r;
        }
        else if (rank_counts[r] == 2)
        {
            pairs[pair_count++] = r;
        }
        else if (rank_counts[r] == 1)
        {
            singles[single_count++] = r;
        }
    }

    if (straight && flush)
    {
        value.category = STRAIGHT_FLUSH;
        value.tiebreak[0] = high_straight_rank;
        return value;
    }

    if (four)
    {
        value.category = FOUR_OF_A_KIND;
        value.tiebreak[0] = four;
        value.tiebreak[1] = singles[0];
        return value;
    }

    if (three && pair_count == 1)
    {
        value.category = FULL_HOUSE;
        value.tiebreak[0] = three;
        value.tiebreak[1] = pairs[0];
        return value;
    }

    if (flush)
    {
        value.category = FLUSH;
        for (int i = 0; i < BEST_HAND_CARDS; i++)
        {
            value.tiebreak[i] = ranks_desc[i];
        }
        return value;
    }

    if (straight)
    {
        value.category = STRAIGHT;
        value.tiebreak[0] = high_straight_rank;
        return value;
    }

    if (three)
    {
        value.category = THREE_OF_A_KIND;
        value.tiebreak[0] = three;
        value.tiebreak[1] = singles[0];
        value.tiebreak[2] = singles[1];
        return value;
    }

    if (pair_count == 2)
    {
        value.category = TWO_PAIR;
        value.tiebreak[0] = pairs[0];
        value.tiebreak[1] = pairs[1];
        value.tiebreak[2] = singles[0];
        return value;
    }

    if (pair_count == 1)
    {
        value.category = ONE_PAIR;
        value.tiebreak[0] = pairs[0];
        value.tiebreak[1] = singles[0];
        value.tiebreak[2] = singles[1];
        value.tiebreak[3] = singles[2];
        return value;
    }

    value.category = HIGH_CARD;
    for (int i = 0; i < BEST_HAND_CARDS; i++)
    {
        value.tiebreak[i] = ranks_desc[i];
    }
    return value;
}

int compare_hand_values(HandValue a, HandValue b)
{
    if (a.category != b.category)
    {
        return (int)a.category - (int)b.category;
    }

    for (int i = 0; i < BEST_HAND_CARDS; i++)
    {
        if (a.tiebreak[i] != b.tiebreak[i])
        {
            return a.tiebreak[i] - b.tiebreak[i];
        }
    }
    return 0;
}

HandValue evaluate_best_of_seven(const Card cards[MAX_SEVEN_CARDS])
{
    HandValue best;
    int best_set = 0;

    for (int a = 0; a < MAX_SEVEN_CARDS - 4; a++)
    {
        for (int b = a + 1; b < MAX_SEVEN_CARDS - 3; b++)
        {
            for (int c = b + 1; c < MAX_SEVEN_CARDS - 2; c++)
            {
                for (int d = c + 1; d < MAX_SEVEN_CARDS - 1; d++)
                {
                    for (int e = d + 1; e < MAX_SEVEN_CARDS; e++)
                    {
                        Card subset[BEST_HAND_CARDS] =
                            {
                                cards[a], cards[b], cards[c], cards[d], cards[e]};

                        HandValue current = evaluate_five_card_hand(subset);
                        if (!best_set || compare_hand_values(current, best) > 0)
                        {
                            best = current;
                            best_set = 1;
                        }
                    }
                }
            }
        }
    }

    return best;
}

const char *hand_category_name(HandCategory category)
{
    if (category < HIGH_CARD || category > STRAIGHT_FLUSH)
    {
        return "Unknown";
    }
    return CATEGORY_NAMES[category];
}
