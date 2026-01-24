#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Globals
#define MAX_SIZE 20      // Maximum grid size; MAX * MAX
#define DEFAULT_SIZE 15  // Default grid size; DEFAULT * DEFAULT
// Passes for attempting random placement of words before resorting to
// sequential placement
#define DEFAULT_RAND_ATTEMPTS \
    50000  // bigger number will result in lower performance, obviously

// globally storing table with maximum size
char table[MAX_SIZE][MAX_SIZE];
// temp table for attempting word insertion
char tempTable[MAX_SIZE][MAX_SIZE];
// store the argv idx for the words included
uint32_t wordidxList[MAX_SIZE * MAX_SIZE];
// store the inputed size of the square table.
uint32_t size, rand_attempts;
uint8_t xray_enable;

typedef int (*insert_func_t)(int, int, char[]);

insert_func_t funcs[4];  // array of insertion functions

void usage(const char* s) {
    fprintf(stderr,
            "Usage: %s [-h|--help]"
            " [-x|--xray]"
            " [-s|--size <width/height>]"
            " [-r|--rand_attempts <number>]\n",
            s);
    exit(-1);
}

// Fills in gaps with random letters and prints table to console.
void printTable(char table[MAX_SIZE][MAX_SIZE]) {
    int i, j;

    // populating empty values; i.e., filling in gaps between words
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (table[i][j] == '\0') {
                if (xray_enable) {
                    table[i][j] = ' ';  // for testing
                } else {
                    table[i][j] = 'A' + rand() % 26;
                }
            }
        }
    }

    // print table
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            // print characters in table
            printf(" %c ", table[i][j]);
        }
        // new row
        printf("\n");
    }
}

// Randomizes order of array of 4 insertion functions.
void shuffle_insert_funcs(insert_func_t funcs[], size_t n) {
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);

        insert_func_t tmp = funcs[i];
        funcs[i] = funcs[j];
        funcs[j] = tmp;
    }
}

// Attempts directly forward word placement; 0 if success, 1 if fail
// row, col to attempt from taken as input
int forwardInsert(int r, int c, char word[]) {
    int i;  // loop control

    // Filling tempTable with copy of original; this way I can easily revert if
    // unsuccessful. Definitely not efficient, but simple for implementation
    // purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++) {
        if (c + i >= size) {
            // spot is out of bounds
            return 1;
        } else if (tempTable[r][c + i] == '\0' ||
                   tempTable[r][c + i] == toupper(word[i])) {
            // spot is empty or character matches
            tempTable[r][c + i] = toupper(word[i]);  // insert character
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
int downwardInsert(int r, int c, char word[]) {
    // basically identical to forward insert
    int i;  // loop control

    // Filling tempTable with copy of original; this way I can easily revert if
    // unsuccessful. Definitely not efficient, but simple for implementation
    // purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++) {
        if (r + i >= size) {
            // spot is out of bounds
            return 1;
        } else if (tempTable[r + i][c] == '\0' ||
                   tempTable[r + i][c] == toupper(word[i])) {
            // spot is empty or character matches
            tempTable[r + i][c] = toupper(word[i]);  // insert character
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
int diagDownInsert(int r, int c, char word[]) {
    int i;  // loop control

    // Filling tempTable with copy of original; this way I can easily revert if
    // unsuccessful. Definitely not efficient, but simple for implementation
    // purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++) {
        if (r + i >= size || c + i >= size) {
            // spot is out of bounds
            return 1;
        } else if (tempTable[r + i][c + i] == '\0' ||
                   tempTable[r + i][c + i] == toupper(word[i])) {
            // spot is empty or character matches
            tempTable[r + i][c + i] = toupper(word[i]);  // insert character
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
int diagUpInsert(int r, int c, char word[]) {
    int i;  // loop control

    // Filling tempTable with copy of original; this way I can easily revert if
    // unsuccessful. Definitely not efficient, but simple for implementation
    // purposes.
    memcpy(tempTable, table, sizeof(table));
    // start insertion; return 1 if not compatble with an existing character
    for (i = 0; word[i]; i++) {
        if (r - i < 0 || c + i >= size) {
            // spot is out of bounds
            return 1;
        } else if (tempTable[r - i][c + i] == '\0' ||
                   tempTable[r - i][c + i] == toupper(word[i])) {
            // spot is empty or character matches
            tempTable[r - i][c + i] = toupper(word[i]);  // insert character
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
int insertWord(char word[]) {
    // implementation notes: This is only kinda random. I figure doing both
    // random and checking every possible spot is difficult to do in one go, So
    // I'll try random locations, then move to itteration through the grid.
    // Maybe not the best solution, but I figure it's good enough. also worth
    // noting: Max size is 20. There are 4 directions to go. 20*20*4 = 1600
    // possible orientations, per word. That's a lot.
    int r, c;  // specific spot to try and put a character
    int i, j;  // loop control

    // Shuffle placement function order
    shuffle_insert_funcs(funcs, 4);

    // attempting random position
    for (i = 0; i < rand_attempts; i++) {
        // randomize location to attempt from
        r = rand() % size;
        c = rand() % size;
        // Rotate through 4 insertion functions in array
        for (j = 0; j < 4; j++) {
            if (funcs[j](r, c, word) == 0) {
                return 0;  // success
            }
        }
    }

    // random attempts exhausted; itterating through all possible locations and
    // directions. this is purely sequential, so no randomness is done here.
    for (r = 0; r < size; r++) {
        for (c = 0; c < size; c++) {
            for (j = 0; j < 4; j++) {
                // rotate through 4 insertion functions
                if (funcs[j](r, c, word) == 0) {
                    return 0;  // success
                }
            }
        }
    }
    return 1;  // failed to insert
}

// prints word in format "Word "test"", with test being the example word here.
// mostly for testing purposes.
void printWord(char word[]) {
    int i;
    printf("\"");
    for (i = 0; word[i]; i++) {
        printf("%c", toupper(word[i]));
    }
    printf("\"");
}

// Run the thingy
int main(int argc, char* argv[]) {
    int32_t i, opt, wordPass, tempsize;
    int32_t wordc = 0;  // # words successfully inserted
    int32_t option_index = 0;

    // Populate default values; will overwrite if specified
    xray_enable = 0;
    size = DEFAULT_SIZE;
    rand_attempts = DEFAULT_RAND_ATTEMPTS;
    // Populate array of insertion functions
    funcs[0] = forwardInsert; 
    funcs[1] = downwardInsert; 
    funcs[2] = diagDownInsert; 
    funcs[3] = diagUpInsert;

    srand(time(NULL));

    // Command Line Option Definitions
    static struct option long_opts[] = {
        {"help", no_argument, 0, 'h'},
        {"xray", no_argument, 0, 'x'},
        {"size", required_argument, 0, 's'},
        {"rand_attempts", required_argument, 0, 'r'},
        {0, 0, 0, 0}  // terminator
    };

    // Parse command line arguments
    while ((opt = getopt_long(argc, argv, "hxs:r:", long_opts,
                              &option_index)) != -1) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                break;
            case 'x':
                xray_enable = 1;
                break;
            case 's':
                tempsize = atoi(optarg);
                // Check that size is not too large
                if (tempsize > MAX_SIZE) {
                    // Requested was larger than MAX, so fallback on MAX
                    size = MAX_SIZE;
                    printf("Generating with maximum size of %d\n", MAX_SIZE);
                } else {
                    size = tempsize;
                    printf("Generating with requested size of %d\n", size);
                }
                break;
            case 'r':
                rand_attempts = atoi(optarg);
                break;
            default:
                usage(argv[0]);
                break;
        }
    }

    // Parse remaining words (assuming there are any)
    for (i = optind; i < argc; i++) {
        wordPass = insertWord(argv[i]);
        if (wordPass == 0) {
            wordidxList[wordc] = i;  // putting idx of word into wordlist
            wordc++;                 // increasing successful word count
            // printWord(argv[i]);
            // printf(" successfully inserted [PASS]\n");
        } else {
            printWord(argv[i]);
            printf(" Did not fit; ignoring word.\n");
        }
    }

    printf("============================================================\n");
    printf("Word Search Generator by Andrew Butler \n");
    printf("============================================================\n");
    // print table to console
    printTable(table);
    printf("============================================================\n");

    // this is going to look confusing. Again, I wrote this with the intention
    // of not terminating on a word fitment failure.
    for (i = 0; wordidxList[i]; i++) {
        printf("%d. ", i + 1);
        printWord(argv[wordidxList[i]]);
        printf("\n");
    }
}