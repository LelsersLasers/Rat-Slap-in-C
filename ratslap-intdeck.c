#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>  
#include <string.h>

int transferCount = 0; // see if the transfer card function is the issue

// prototypes
void condense(int *d, int *val);
void printCards(int *d);
void shuffle(int *de);
void splitDeck(int *deck, int *pile1, int *pile2);
int getCardsInPile(int *arr);
void getCardName(int value, char *stringName);
int flip(int *pile, char *stringName);
int checkDouble(int *arr);
int checkSandwich(int *arr);
int switchTurn();
int slap();
int checkFace(int cardValue);
void copyArray(int *arr1, int *arr2);
void transferCards(int *arr1, int *arr2);
void addToArray(int *arr, int value);
void blankArray(int *arr);
int game(int *pile1, int *pile2);


void condense(int *d, int *val) {
    // go through the array and move the first 0 to the end
    // so its all numbers until its all 0s
    int size = 52;
    int index;
    int notFound = 1;
    for (int i = 0; i < size; i++) { // loop to find the 0
        if (d[i] == 0) {
            if (notFound) {
                index = i;
                notFound = 0;
            }
        }
    }
    int valueAtIndex = d[index];
    for (int i = index; i < size - 1; i++) { // move everything down one
        d[i] = d[i+1];
    }
    d[size - 1] = valueAtIndex; // add the 0 back to the end
    *val = *val - 1;
}

void printCards(int *d) {
    // used to check if the deck shuffled ok
    // just prints each card in an 52 int array
    for (int i = 0; i < 52; i++) {
        printf("%d, ", d[i]);
    }
    printf("\n");
}

void shuffle(int *de) {

    int e[52]; // shuffled array
    int maxVal = 52;
    for (int i = 0; i < 52; i++) {
        int index = rand() % maxVal; // pick a random card from the remaining cards
        if (de[index] != 0) { // make it so each card can only be picked once
            e[i] = de[index];
            de[index] = 0;
            condense(de, &maxVal);
        }
    }
    copyArray(de, e); // set the passed array to the new shuffled array
}

void splitDeck(int *deck, int *pile1, int *pile2) {
    // takes the deck and splits it into 2 piles
    // done at start to make the 2 starting piles for the players
    for (int i = 0; i < 26; i++) {
        pile1[i] = deck[i];
        pile2[i] = deck[i + 26];
    }
    for (int i = 26; i < 52; i++) { // blank out the rest to 0
        pile1[i] = 0;
        pile2[i] = 0;
    }
}

int getCardsInPile(int *arr) {
    // count the cards in a pile
    // goes because of the condense function, they are all numbers greater than 0
    // until they are all 0, so just count until it finds a 0
    
    int counter = 0;
    int notFound = 1;
    while (notFound) {
        if (arr[counter] == 0) {
            return counter;
        }
        else {
            counter = counter + 1;
        }
    }
}

void getCardName(int value, char *stringName) {
    // get the display name/card name from an int
    // ex: 13 - king, 1 - Ace, 3 - 3;
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
    // flip a card from the pile, takes the 0th index
    // and returns it, and removes it from pile
    // by using the condense
    int value = pile[0];
    pile[0] = 0;
    int temp = 0;
    condense(pile, &temp);
    printf("\n");
    getCardName(value, stringName);
    return value;
}

int checkDouble(int *arr) {
    // checks if the last 2 cards are the same (if they are it can be slapped)
    int size = getCardsInPile(arr);
    if (arr[size - 2] == arr[size -1]) {
        return 1;
    }
    return 0;
}

int checkSandwich(int *arr) {
    // checks if the last card and the 3rd to last card are the same (if they are it can be slapped)
    int size = getCardsInPile(arr);
    if (arr[size - 3] == arr[size -1]) {
        return 1;
    }
    return 0;
}

int switchTurn(int currentPlayer) {
    // switchs the active player
    if (currentPlayer == 1) {
        return 2;
    }
    return 1;
}

int slap() { // will do a better one if I feel like
    // randomly pick someone to win the "slap off", when it is a double or sandwich
    printf("Slap Off!\n");
    
    int chance1 = rand() % 100;
    int chance2 = rand() % 100;
    if (chance1 > chance2) {
        return 1;
    }
    return 2;
}

int checkFace(int cardValue) {
    // return the amount of tries the other person has to draw a face
    // if it is a face, if not return 0
    if (cardValue == 1) {
        return 4;
    }
    else if (cardValue > 10) { 
        return cardValue - 10;
    }
    return 0;
}

void copyArray(int *arr1, int *arr2) {
    // set the first array equal to the second arrays
    int size = 52;
    for (int i = 0; i < size; i++) {
        arr1[i] = arr2[i];
    } 
}

void transferCards(int *arr1, int *arr2) {
    // add all the cards/values from the second array to the first array
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
    // add a single card to a pile
    int indexToAddTo = getCardsInPile(arr);
    arr[indexToAddTo] = value;
}

void blankArray(int *arr) {
    // set every card in the array to 0
    int size = 52;
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

int game(int *pile1, int *pile2) {
    // the main thing that happens, controls turns and piles
    // and manages things like printfs and returns the winner
    
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
    // opening printfs
    printf("\nWelcome to 0 player Egpytyian Rat Slap!\n");
    printf("Hit [enter] to start\n");
    getchar();
    printf("STARTING!\n\n");
    // init the random
    srand(time(NULL)); 

    // set up full deck
    int deck[52];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck[i * 13 + j] = j + 1;
        }
    }
    // blankArray(deck);
    shuffle(deck);
    // set up indivual piles
    int pile1[52]; // index 0 has a card, index 51 doesnt
    int pile2[52];
    splitDeck(deck, pile1, pile2); // this will blank it also

    // find winner and printf the ending messages
    int winner = game(pile1, pile2);

    printf("\n\nPi1 cards: %i\n", getCardsInPile(pile1)); // line marker A
    printf("Pi2 cards: %i\n", getCardsInPile(pile2)); // this line + line A should equal 52, and this or A should be 0
    printf("Transfer count: %i\n", transferCount);

    printf("\nWinner: Player %i\n", winner);
    printf("Game over! Hit [enter] to exit\n");

    getchar(); // block from ending
    return 0;
}