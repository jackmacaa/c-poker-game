#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECK_SIZE 52
#define HAND_SIZE 5

/*
 * Card model:
 * - rank: 2..14 where 11=J, 12=Q, 13=K, 14=A
 * - suit: 0..3 where 0=Clubs, 1=Diamonds, 2=Hearts, 3=Spades
 */
typedef struct {
	int rank;
	int suit;
} Card;

typedef struct {
	Card cards[DECK_SIZE];
	int nextIndex;
} Deck;

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

/*
 * HandValue stores:
 * - category: poker hand category
 * - tiebreak: values used in order when categories are equal
 *
 * Example:
 * - For One Pair: [pairRank, kicker1, kicker2, kicker3, 0]
 * - For Two Pair: [highPair, lowPair, kicker, 0, 0]
 */
typedef struct {
	HandCategory category;
	int tiebreak[HAND_SIZE];
} HandValue;

const char *RANK_NAMES[] = {
	"?", "?", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
};

const char *SUIT_NAMES[] = {
	"C", "D", "H", "S"
};

const char *CATEGORY_NAMES[] = {
	"High Card",
	"One Pair",
	"Two Pair",
	"Three of a Kind",
	"Straight",
	"Flush",
	"Full House",
	"Four of a Kind",
	"Straight Flush"
};

void swap_cards(Card *a, Card *b) {
	Card temp = *a;
	*a = *b;
	*b = temp;
}

/* Initialize an ordered deck: 2C..AC, 2D..AD, 2H..AH, 2S..AS */
void init_deck(Deck *deck) {
	int index = 0;
	for (int suit = 0; suit < 4; suit++) {
		for (int rank = 2; rank <= 14; rank++) {
			deck->cards[index].rank = rank;
			deck->cards[index].suit = suit;
			index++;
		}
	}
	deck->nextIndex = 0;
}

/* Fisher-Yates shuffle: O(n), unbiased if rand() is uniform enough */
void shuffle_deck(Deck *deck) {
	for (int i = DECK_SIZE - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap_cards(&deck->cards[i], &deck->cards[j]);
	}
	deck->nextIndex = 0;
}

Card deal_card(Deck *deck) {
	if (deck->nextIndex >= DECK_SIZE) {
		/* Defensive fallback; this should never happen in this small demo. */
		Card empty = {0, 0};
		return empty;
	}
	return deck->cards[deck->nextIndex++];
}

void print_card(Card card) {
	printf("%s%s", RANK_NAMES[card.rank], SUIT_NAMES[card.suit]);
}

void print_hand(const Card hand[HAND_SIZE]) {
	for (int i = 0; i < HAND_SIZE; i++) {
		print_card(hand[i]);
		if (i < HAND_SIZE - 1) {
			printf(" ");
		}
	}
	printf("\n");
}

/* Sort ranks descending in-place (small array, simple selection sort is fine). */
void sort_desc(int arr[], int size) {
	for (int i = 0; i < size - 1; i++) {
		int best = i;
		for (int j = i + 1; j < size; j++) {
			if (arr[j] > arr[best]) {
				best = j;
			}
		}
		if (best != i) {
			int temp = arr[i];
			arr[i] = arr[best];
			arr[best] = temp;
		}
	}
}

/*
 * Detect straight and return highest rank of that straight in *highStraightRank.
 * Special case handled: A-2-3-4-5 has high card 5.
 */
int detect_straight(const int rankCounts[15], int *highStraightRank) {
	for (int high = 14; high >= 5; high--) {
		int isStraight = 1;
		for (int r = high; r > high - 5; r--) {
			if (rankCounts[r] == 0) {
				isStraight = 0;
				break;
			}
		}
		if (isStraight) {
			*highStraightRank = high;
			return 1;
		}
	}

	/* Wheel straight: A,2,3,4,5 */
	if (rankCounts[14] && rankCounts[2] && rankCounts[3] && rankCounts[4] && rankCounts[5]) {
		*highStraightRank = 5;
		return 1;
	}

	return 0;
}

HandValue evaluate_hand(const Card hand[HAND_SIZE]) {
	HandValue value;
	value.category = HIGH_CARD;
	memset(value.tiebreak, 0, sizeof(value.tiebreak));

	int rankCounts[15] = {0};
	int suitCounts[4] = {0};
	int ranksDesc[HAND_SIZE];

	for (int i = 0; i < HAND_SIZE; i++) {
		rankCounts[hand[i].rank]++;
		suitCounts[hand[i].suit]++;
		ranksDesc[i] = hand[i].rank;
	}
	sort_desc(ranksDesc, HAND_SIZE);

	int isFlush = 0;
	for (int s = 0; s < 4; s++) {
		if (suitCounts[s] == HAND_SIZE) {
			isFlush = 1;
			break;
		}
	}

	int highStraightRank = 0;
	int isStraight = detect_straight(rankCounts, &highStraightRank);

	int fourRank = 0;
	int threeRank = 0;
	int pairRanks[2] = {0, 0};
	int pairCount = 0;
	int singles[5] = {0, 0, 0, 0, 0};
	int singleCount = 0;

	for (int r = 14; r >= 2; r--) {
		if (rankCounts[r] == 4) {
			fourRank = r;
		} else if (rankCounts[r] == 3) {
			threeRank = r;
		} else if (rankCounts[r] == 2) {
			pairRanks[pairCount++] = r;
		} else if (rankCounts[r] == 1) {
			singles[singleCount++] = r;
		}
	}

	if (isStraight && isFlush) {
		value.category = STRAIGHT_FLUSH;
		value.tiebreak[0] = highStraightRank;
		return value;
	}

	if (fourRank) {
		value.category = FOUR_OF_A_KIND;
		value.tiebreak[0] = fourRank;
		value.tiebreak[1] = singles[0];
		return value;
	}

	if (threeRank && pairCount == 1) {
		value.category = FULL_HOUSE;
		value.tiebreak[0] = threeRank;
		value.tiebreak[1] = pairRanks[0];
		return value;
	}

	if (isFlush) {
		value.category = FLUSH;
		for (int i = 0; i < HAND_SIZE; i++) {
			value.tiebreak[i] = ranksDesc[i];
		}
		return value;
	}

	if (isStraight) {
		value.category = STRAIGHT;
		value.tiebreak[0] = highStraightRank;
		return value;
	}

	if (threeRank) {
		value.category = THREE_OF_A_KIND;
		value.tiebreak[0] = threeRank;
		value.tiebreak[1] = singles[0];
		value.tiebreak[2] = singles[1];
		return value;
	}

	if (pairCount == 2) {
		value.category = TWO_PAIR;
		value.tiebreak[0] = pairRanks[0];
		value.tiebreak[1] = pairRanks[1];
		value.tiebreak[2] = singles[0];
		return value;
	}

	if (pairCount == 1) {
		value.category = ONE_PAIR;
		value.tiebreak[0] = pairRanks[0];
		value.tiebreak[1] = singles[0];
		value.tiebreak[2] = singles[1];
		value.tiebreak[3] = singles[2];
		return value;
	}

	value.category = HIGH_CARD;
	for (int i = 0; i < HAND_SIZE; i++) {
		value.tiebreak[i] = ranksDesc[i];
	}
	return value;
}

/*
 * Compare two evaluated hands:
 * - returns >0 if a wins
 * - returns <0 if b wins
 * - returns 0 for exact tie
 */
int compare_hands(HandValue a, HandValue b) {
	if (a.category != b.category) {
		return (int)a.category - (int)b.category;
	}

	for (int i = 0; i < HAND_SIZE; i++) {
		if (a.tiebreak[i] != b.tiebreak[i]) {
			return a.tiebreak[i] - b.tiebreak[i];
		}
	}
	return 0;
}

void deal_hand(Deck *deck, Card hand[HAND_SIZE]) {
	for (int i = 0; i < HAND_SIZE; i++) {
		hand[i] = deal_card(deck);
	}
}

void play_round(void) {
	Deck deck;
	Card player[HAND_SIZE];
	Card cpu[HAND_SIZE];

	init_deck(&deck);
	shuffle_deck(&deck);

	deal_hand(&deck, player);
	deal_hand(&deck, cpu);

	HandValue playerValue = evaluate_hand(player);
	HandValue cpuValue = evaluate_hand(cpu);

	printf("\n=== New Round ===\n");
	printf("Your hand:     ");
	print_hand(player);
	printf("Computer hand: ");
	print_hand(cpu);

	printf("You:      %s\n", CATEGORY_NAMES[playerValue.category]);
	printf("Computer: %s\n", CATEGORY_NAMES[cpuValue.category]);

	int result = compare_hands(playerValue, cpuValue);
	if (result > 0) {
		printf("Result: You win!\n");
	} else if (result < 0) {
		printf("Result: Computer wins.\n");
	} else {
		printf("Result: Tie.\n");
	}
}

int main(void) {
	srand((unsigned int)time(NULL));

	printf("Poker Learning Game (5-card showdown)\n");
	printf("--------------------------------------\n");

	char answer[16];
	do {
		play_round();
		printf("\nPlay again? (y/n): ");

		if (fgets(answer, sizeof(answer), stdin) == NULL) {
			break;
		}
	} while (answer[0] == 'y' || answer[0] == 'Y');

	printf("Thanks for playing.\n");
	return 0;
}
