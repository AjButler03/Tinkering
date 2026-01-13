#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Globals
#define MAX 50 // Maximum grid size; MAX * MAX
#define DEFAULT 15 // Default grid size; DEFAULT * DEFAULT
// Passes for attempting random placement of words before resorting to sequential placement
#define RAND_ATTEMPTS 100000 // bigger number will result in lower performance, obviously

// globally storing table with maximum size
char table[MAX][MAX];
// temp table for attempting word insertion
char tempTable[MAX][MAX];
// store the argv idx for the words included
int wordidxList[MAX*MAX];
// store the inputed size of the square table.
int size;

// Fills in gaps with random letters and prints table to console.
void printTable(char table[MAX][MAX]){
    int i, j;

    // populating empty values; i.e., filling in gaps between words
    // for (i = 0; i < size; i++) {
    //     for (j = 0; j < size; j++){
    //         if (table[i][j] == '\0'){
    //             table[i][j] = 'A' + rand() % 26;
    //             // table[i][j] = ' '; // for testing
    //         }
    //     }
    // }

    // print table
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++){
            // print characters in table
            printf(" %c ", table[i][j]);
        }
        // new row
        printf("\n");
    }
}

// Array of the 4 placement methods given, and randomizes their order.
// int shuffle_platmpt_order(int *place_fn_ptr_arr(int, int, char[])[]){
// //    I have to come back to this.
    
//     return NULL;
// }

// Attempts directly forward word placement; 0 if success, 1 if fail
// row, col to attempt from taken as input
int forwardInsert(int r, int c, char word[]){
    int i; // loop control

    // Filling tempTable with copy of original; this way I can easily revert if unsuccessful.
    // Definitely not efficient, but simple for implementation purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++){
        if (c + i >= size){
            // spot is out of bounds
            return 1;
        } else if (tempTable[r][c + i] == '\0' || tempTable[r][c + i] == toupper(word[i])){
            // spot is empty or character matches
            tempTable[r][c + i] = toupper(word[i]); // insert character
        } else {
            // spot is not empty or character does not match
            return 1;
        }
    }
    // Success- Copy the temp to the original and return 0.
    memcpy(table, tempTable, sizeof(tempTable));
    return 0;
}

// Attempts directly downward word placement; 0 if success, 1 if fail
// row, col to attempt from taken as input
int downwardInsert(int r, int c, char word[]){
    // basically identical to forward insert
    int i; // loop control

    // Filling tempTable with copy of original; this way I can easily revert if unsuccessful.
    // Definitely not efficient, but simple for implementation purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++){
        if (r + i >= size){
            // spot is out of bounds
            return 1;
        } else if (tempTable[r + i][c] == '\0' || tempTable[r + i][c] == toupper(word[i])){
            // spot is empty or character matches
            tempTable[r + i][c] = toupper(word[i]); // insert character
        } else {
            // spot is not empty or character does not match
            return 1;
        }
    }
    // Success- Copy the temp to the original and return 0.
    memcpy(table, tempTable, sizeof(tempTable));
    return 0;
}

// Attempts diagonal downward word placement; 0 if success, 1 if fail
// row, col to attempt from taken as input
int diagDownInsert(int r, int c, char word[]){
    int i; // loop control

    // Filling tempTable with copy of original; this way I can easily revert if unsuccessful.
    // Definitely not efficient, but simple for implementation purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++){
        if (r + i >= size || c + i >= size){
            // spot is out of bounds
            return 1;
        } else if (tempTable[r + i][c + i] == '\0' || tempTable[r + i][c + i] == toupper(word[i])){
            // spot is empty or character matches
            tempTable[r + i][c + i] = toupper(word[i]); // insert character
        } else {
            // spot is not empty or character does not match
            return 1;
        }
    }
    // Success- Copy the temp to the original and return 0.
    memcpy(table, tempTable, sizeof(tempTable));
    return 0;
}

// Attempts diagonal upward word placement; 0 if success, 1 if fail
// row, col to attempt from taken as input
int diagUpInsert(int r, int c, char word[]){
    int i; // loop control

    // Filling tempTable with copy of original; this way I can easily revert if unsuccessful.
    // Definitely not efficient, but simple for implementation purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++){
        if (r - i < 0 || c + i >= size){
            // spot is out of bounds
            return 1;
        } else if (tempTable[r - i][c + i] == '\0' || tempTable[r - i][c + i] == toupper(word[i])){
            // spot is empty or character matches
            tempTable[r - i][c + i] = toupper(word[i]); // insert character
        } else {
            // spot is not empty or character does not match
            return 1;
        }
    }
    // Success- Copy the temp to the original and return 0.
    memcpy(table, tempTable, sizeof(tempTable));
    return 0;
}

// attempts to insert a word into the table.
// returns 0 if successful, 1 if unsuccessful.
int insertWord(char word[]){
    // implementation notes: This is only kinda random. I figure doing both random and checking every possible spot is difficult to do in one go,
    // So I'll try random locations, then move to itteration through the grid. Maybe not the best solution, but I figure it's good enough.
    // also worth noting: Max size is 20. There are 4 directions to go. 20*20*4 = 1600 possible orientations, per word. That's a lot.
    int r, c; // specific spot to try and put a character
    int i; // loop control
    
    // store result from call to insertion methods; used for randomizing which to use
    int f, d, diagu, diagd = 2; // 2 as a default state
    int temp; // used to randomize which of the insertion methods to call in inner loops

    // attempting random position
    for (i = 0; i < RAND_ATTEMPTS; i++){
        // randomize location to attempt from
        r = rand() % size;
        c = rand() % size;
        // now attempt the 4 positions; will randomize until all 4 have been attempted
        while (f != 1 || d != 1 || diagu != 1 || diagd != 1){
            // randomizing the direction; I assume there is a better way to do this
            temp = rand() % 4;
            if (temp == 0){
                f = forwardInsert(r, c, word); // attempt forward insertion
                if (f == 0){
                    return 0; // success, return
                }
            } else if (temp == 1){
                d = downwardInsert(r, c, word); // attempt downward insertion
                if (d == 0){
                    return 0; // success, return
                }
            } else if (temp == 2){
                diagu = diagUpInsert(r, c, word); // attempt diagonal upward insertion
                if (diagu == 0){
                    return 0; // success, return
                }
            } else {
                diagd = diagDownInsert(r, c, word); // attempt diagonal downward insertion
                if (diagd == 0){
                    return 0; // success, return
                }
            }
        }
    }

    // random attempts exhausted; itterating through all possible locations and directions.
    // this is purely sequential, so no randomness is done here.
    for (r = 0; r < size; r++){
        for (c = 0; c < size; c++){
            // attempt the 4 possible directions from this point
                if (forwardInsert(r, c, word) == 0){
                    return 0; // success, return
                } else if (downwardInsert(r, c, word) == 0){
                    return 0; // success, return
                } else if (diagUpInsert(r, c, word) == 0){
                    return 0; // success, return
                } else if (diagDownInsert(r, c, word) == 0){
                    return 0; // success, return
                }
        }
    }
    return 1; // failed to insert
}

// prints word in format "Word "test"", with test being the example word here.
// mostly for testing purposes.
void printWord(char word[]){
    int i;
    printf("\"");
    for (i = 0; word[i]; i++){
        printf("%c", toupper(word[i]));
    }
    printf("\"");
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    printf("============================================================\n");
    printf("Word Search Generator by Andrew Butler \n");

    int pass, wordPass; // used to help store if words failed
    int i;// loop control
    int wordc = 0; // # words successfully inserted

    // check args
    if (argc == 1){
        // no args passed; basically just create gibberish table
        size = DEFAULT;
        printf("Generating with default size of %d\n", size);
    } else {
        for (i = 1; i < argc && pass != 1; i++){
            // check if argument is number and first arg; handle as size input if so
            if (isdigit(argv[i][0]) && (i == 1)){
                // convert from string to number
                size = atoi(argv[1]);
                // check that specified size is not more than maximum
                if (size > MAX){
                    // Requested was larger than MAX, so fallback on MAX
                    size = MAX;
                    printf("Generating with maximum size of %d\n", MAX);
                } else {
                    printf("Generating with requested size of %d\n", size);
                }
            } else if (!isdigit(argv[i][0])) {
                if (i==1) {
                    // first arg is not a number, use fallback default size
                    size = DEFAULT;
                    printf("Generating with default size of %d\n", size);
                }
                // continue and parse arg as string
                wordPass = insertWord(argv[i]);
                if (wordPass == 0){
                    wordidxList[wordc] = i; // putting idx of word into wordlist
                    wordc++; // increasing successful word count
                    // printWord(argv[i]);
                    // printf(" successfully inserted [PASS]\n");
                } else {
                    pass = 1; // <==<==<==<=============================================================================== Comment this line to allow 'skipping' words that don't fit.
                    printWord(argv[i]);
                    printf(" did not fit. Generation canceled.\n");
                    // printf(" failed to be inserted [FAIL]\n");
                }
            }
        }
    }

    printf("============================================================\n");
    // fill in blank spots and print to console
    if (pass != 1){
        printTable(table);
        printf("------------------------------------------------------------\n");

        // this is going to look confusing. Again, I wrote this with the intention of not terminating on a word fitment failure.
        for (i = 0; wordidxList[i]; i++){
            printf("%d. ", i + 1);
            printWord(argv[wordidxList[i]]);
            printf("\n");
        }
    }
}