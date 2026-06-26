#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "holdem.h"

int main(void)
{
    srand((unsigned int)time(NULL));

    printf("Poker Learning Game (Texas Hold'em)\n");
    printf("-----------------------------------\n");

    char answer[16];
    do
    {
        play_holdem_round();
        printf("\nPlay another round? (Y/n): ");

        if (fgets(answer, sizeof(answer), stdin) == NULL)
        {
            break;
        }
    } while (answer[0] == 'y' || answer[0] == 'Y' || answer[0] == '\n');

    printf("Thanks for playing.\n");
    return 0;
}
