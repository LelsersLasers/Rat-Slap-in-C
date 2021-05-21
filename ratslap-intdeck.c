#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>  
#include <string.h>

int transferCount = 0;

void condense(int *d, int *val) {
    
    int size = 52;
    int index;
    int notFound = 1;
    for (int i = 0; i < size; i++) {
        if (d[i] == NULL || d[i] == 0) {
            if (notFound) {
                index = i;
                notFound = 0;
            }
        }
    }
    int valueAtIndex = d[index];
    for (int i = index; i < size - 1; i++) {
        d[i] = d[i+1];
    }
    d[size - 1] = valueAtIndex;
    *val = *val - 1;
}

void printCards(int *d) {
    for (int i = 0; i < 52; i++) {
        printf("%d, ", d[i]);
    }
    printf("\n");
}

void shuffle(int *de) {

    int e[52];
    int maxVal = 52;
    for (int i = 0; i < 52; i++) {
        int index = rand() % maxVal;
        if (de[index] != NULL) {
            e[i] = de[index];
            de[index] = NULL;
            condense(de, &maxVal);
        }
    }
    copyArray(de, e);
}

void splitDeck(int *deck, int *pile1, int *pile2) {

    for (int i = 0; i < 26; i++) {
        pile1[i] = deck[i];
        pile2[i] = deck[i + 26];
    }
    for (int i = 26; i < 52; i++) {
        pile1[i] = 0;
        pile2[i] = 0;
    }
}

int getCardsInPile(int *arr) {
    
    int counter = 0;
    int notFound = 1;
    while (notFound) {
        if (arr[counter] == NULL || arr[counter] == 0) {
            return counter;
        }
        else {
            counter = counter + 1;
        }
    }
}

void getCardName(int value, char *stringName) {

    switch (value) {
    case 1:
        strcpy(stringName, "Ace");
        break;
    case 11:
        strcpy(stringName, "Jack");
        break;
    case 12:
        strcpy(stringName, "Queen");
        break;
    case 13:
        strcpy(stringName, "King");
        break;
    default:;
        int length = snprintf(NULL, 0, "%i", value);
        snprintf(stringName, length + 1, "%i", value);
        break;
    }
}

int flip(int *pile, char *stringName) {
    
    int value = pile[0];
    pile[0] = 0;
    int temp = 0;
    condense(pile, &temp);
    printf("\n");
    getCardName(value, stringName);
    return value;
}

int checkDouble(int *arr) {
    int size = getCardsInPile(arr);
    if (arr[size - 2] == arr[size -1]) {
        return 1;
    }
    return 0;
}

int checkSandwich(int *arr) {
    int size = getCardsInPile(arr);
    if (arr[size - 3] == arr[size -1]) {
        return 1;
    }
    return 0;
}

int switchTurn(int currentPlayer) {
    if (currentPlayer == 1) {
        return 2;
    }
    return 1;
}

int slap() { // will do a better one if I feel like
    
    printf("Slap Off!\n");
    
    int chance1 = rand() % 100;
    int chance2 = rand() % 100;
    if (chance1 > chance2) {
        return 1;
    }
    return 2;
}

int checkFace(int cardValue) {
    if (cardValue == 1) {
        return 4;
    }
    else if (cardValue > 10) { 
        return cardValue - 10;
    }
    return 0;
}

void copyArray(int *arr1, int *arr2) {
    int size = 52;
    for (int i = 0; i < size; i++) {
        arr1[i] = arr2[i];
    } 
}

void transferCards(int *arr1, int *arr2) {
    int size2 = getCardsInPile(arr2);
    int start1 = getCardsInPile(arr1);
    for (int i = 0; i < size2; i++) {
        arr1[start1 + i] = arr2[i];
    }
    printf("Transfering %i cards from pile.\n", size2);
    blankArray(arr2);
    transferCount = transferCount + 1;
}

void addToArray(int *arr, int value) {

    int indexToAddTo = getCardsInPile(arr);
    arr[indexToAddTo] = value;
}

void blankArray(int *arr) {
    int size = 52;
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

int game(int *pile1, int *pile2) {
    
    int currentPlayer = 1;
    char *name = (char *)malloc(5 * sizeof(char));

    while (1) {

        int pileLive[52]; // index 0 has a card, index 51 doesnt
        int importantLastCard = 0;
        blankArray(pileLive);
        int inRound = 1;

        while (inRound) {
            int dontSwitch = 0;
            // starting turn
            int currentPile[52];
            int otherPile[52];
            if (currentPlayer == 1){
                copyArray(currentPile, pile1);
                copyArray(otherPile, pile2);
            }
            else {
                copyArray(currentPile, pile2);
                copyArray(otherPile, pile1);
            }

            // mid turn
            if (getCardsInPile(currentPile) > 0) {

                int card = flip(currentPile, name);
                if (importantLastCard == 0) {
                    printf("Player %i flips a %s.\n", currentPlayer, name);
                    addToArray(pileLive, card);
                }
                else {
                    card = importantLastCard;
                    importantLastCard = 0;
                }

                int isDouble = checkDouble(pileLive);
                int isSandwich = checkSandwich(pileLive);
                if (isDouble || isSandwich) { // slap prio
                    int slapWinner = slap();
                    printf("Player %i won the slap!\n", slapWinner);
                    if (slapWinner == currentPlayer) {
                        transferCards(currentPile, pileLive);
                        // currentPlayer = switchTurn(currentPlayer); // double flip to keep on same person
                        dontSwitch = 1;
                    }
                    else {
                        transferCards(otherPile, pileLive);
                    }
                    inRound = 0;
                }
                else {
                    int isFace = checkFace(card);
                    int pileCard;
                    if (isFace > 0) { // if its a face card
                        printf("Player %i must draw a face in %i flips.\n", switchTurn(currentPlayer), isFace);
                        while (isFace > 0) { // must draw a face card
                            pileCard = flip(otherPile, name);
                            printf("Player %i flips a %s.\n", switchTurn(currentPlayer), name);
                            addToArray(pileLive, pileCard);
                            // slap prio
                            int isDouble = checkDouble(pileLive);
                            int isSandwich = checkSandwich(pileLive);
                            if (isDouble || isSandwich) {
                                int slapWinner = slap();
                                printf("Player %i won the slap!\n", slapWinner);
                                if (slapWinner == currentPlayer) {
                                    transferCards(currentPile, pileLive);
                                    // currentPlayer = switchTurn(currentPlayer); // double flip to keep on same person
                                    dontSwitch = 1;
                                }
                                else {
                                    transferCards(otherPile, pileLive);
                                }
                                inRound = 0;
                                isFace = -1;
                            }
                            // slap end
                            if (checkFace(pileCard) > 0) {
                                isFace = -10;
                            }
                            isFace = isFace - 1;
                        }
                        if (isFace < -5) { // they got a face card
                            importantLastCard = pileCard; // prep next time as a face
                        }
                        else if (isFace < 0) { // slap
                            // already taken care of
                        }
                        else {
                            printf("Player %i was not able to get a face card!\n", switchTurn(currentPlayer));
                            transferCards(currentPile, pileLive); // person who placed face wins
                            //currentPlayer = switchTurn(currentPlayer);
                            dontSwitch = 1;
                            inRound = 0;
                        }
                    }
                }  

                printf("Player %i cards left: %i\n", currentPlayer, getCardsInPile(currentPile));
                
                //_sleep(100);
            }
            else {
                free(name);
                return switchTurn(currentPlayer); // other person wins if out of cards
            }
            // ending turn
            if (currentPlayer == 1){
                copyArray(pile1, currentPile);
                copyArray(pile2, otherPile);
            }
            else {
                copyArray(pile2, currentPile);
                copyArray(pile1, otherPile);
                // printf("Live pile size: %i\n\n", getCardsInPile(pileLive));
            }
            printf("Total cards: %i\n", getCardsInPile(pile1) + getCardsInPile(pile2) + getCardsInPile(pileLive));
            // printf("\n");
            if (dontSwitch == 0) {
                currentPlayer = switchTurn(currentPlayer);
            }
            else {
                dontSwitch = 0;
            }
        }

    }
}


int main() {

    printf("\nWelcome to 0 player Egpytyian Rat Slap!\n");
    printf("Hit [enter] to start\n");
    getchar();
    printf("STARTING!\n\n");

    srand(time(NULL)); 

    int deck[52];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck[i * 13 + j] = j + 1;
        }
    }
    shuffle(deck);

    int pile1[52]; // index 0 has a card, index 51 doesnt
    int pile2[52];
    splitDeck(deck, pile1, pile2); // this will blank it also

    int winner = game(pile1, pile2);

    printf("\n\nPi1 cards: %i\n", getCardsInPile(pile1));
    printf("\nPi2 cards: %i\n", getCardsInPile(pile2));
    printf("Transfer count: %i\n", transferCount);

    printf("\nWinner: Player %i\n", winner);
    printf("Game over! Hit [enter] to exit\n");

    getchar();
    return 0;
}