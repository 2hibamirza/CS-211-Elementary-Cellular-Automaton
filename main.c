//Elementary Cellular Automaton

#include <stdio.h>
#include <stdbool.h> 

const int WORLD_SIZE = 65;

typedef struct cell_struct{
    bool state[3]; //active status for [left, me, right] cells 
    bool active; //current active status for this cell
} cell;

//convert an 8-bit integer rule (0-255) to array of bits 
//(stored in reverse order)
//   ex: rule = 6  = 00000110 -> [01100000] 
//   ex: rule = 30 = 00011110 -> [01111000] 
//   ex: rule = 65 = 01000001 -> [10000010]
//return true if input rule is valid (0-255)
//return false if input rule is invalid
bool setBitArray(bool bitArray[8], int rule) {

    //TODO: Task 1 - write the setBitArray() function
    if (rule < 0 || rule > 255) {
        return false; // Invalid rule value
    }

    for (int i = 0; i < 8; i++) {
        bitArray[i] = (rule & (1 << i)) != 0; // Extract each bit
    }
    
    return true;
}

//convert a 3-bit state array to its 
//associated index of the rule# bit array
//   ex: {0 0 0} -> 0
//   ex: {0 0 1} -> 1
//   ex: {1 0 1} -> 5
int stateToIndex(bool state[3]) {

    //TODO: Task 4 - write the stateToIndex() function
    int index = 0;

    for (int i = 0; i < 3; i++) {
        int powerOf2 = 1;
        for (int j = 0; j < (2 - i); j++) {
            powerOf2 *= 2;
        }
        index += state[i] * powerOf2;
    }

    return index;
}

//update the state array for each cell in the world array based on the
//current status of active for the nearby [left,me,right] cells
//note: world is assumed periodic/cyclic,
//with front-to-back and back-to-front wrapping 
void setStates(cell world[WORLD_SIZE]) {

    for (int i = 0; i < WORLD_SIZE; i++) {
        int leftIdx = (i + WORLD_SIZE - 1) % WORLD_SIZE;
        int rightIdx = (i + 1) % WORLD_SIZE;

        world[i].state[0] = world[leftIdx].active;
        world[i].state[1] = world[i].active;
        world[i].state[2] = world[rightIdx].active;
    }
    
    return;
}

//evolve each cell's active status to the next generation 
//  using its state array
//ruleBitArray contains the 8-bits for the rule#, 
//  stored in reverse order
void evolveWorld(cell world[WORLD_SIZE], bool ruleBitArray[8]) {

    //TODO: Task 7 - write the evolveWorld() function

    cell newWorld[WORLD_SIZE]; // Temporary array to store the new states

    for (int i = 0; i < WORLD_SIZE; i++) {
        int leftIdx = (i + WORLD_SIZE - 1) % WORLD_SIZE;
        int rightIdx = (i + 1) % WORLD_SIZE;
        bool newState[3] = {world[leftIdx].active, world[i].active, world[rightIdx].active};
        int index = stateToIndex(newState);
        newWorld[i].active = ruleBitArray[index];
    }

    // Update the world array with the new states
    for (int i = 0; i < WORLD_SIZE; i++) {
        world[i].active = newWorld[i].active;
    }
    
    return;
}


int main() {
    cell world[WORLD_SIZE];

    printf("Welcome to the Elementary Cellular Automaton!\n");

    //TODO: Task 2 - read in a valid rule# and
    //      generate the rule's 8-bit rule bit array 
    //      print the bit array in correct binary number order
    int ruleNumber;
    bool ruleBitArray[8];
    bool validRule = false;

    while (!validRule) {
        printf("Enter the rule # (0-255): ");
        scanf("%d", &ruleNumber);

        validRule = setBitArray(ruleBitArray, ruleNumber);
    }
    printf("\n");
    printf("The bit array for rule #%d is ", ruleNumber);
    for (int i = 7; i >= 0; i--) {
        printf("%d", ruleBitArray[i]);
    }
    printf("\n");

    //TODO: Task 3 - use the rule bit array to report the evolution 
    //      step for all possible cell states.
    //      follow the format of the sample output exactly
    printf("The evolution of all possible states are as follows:\n");
    printf("|***|   |** |   |* *|   |*  |   | **|   | * |   |  *|   |   |\n");
    printf(" ");
    for (int i = 7; i > 0; i--) {
        if (ruleBitArray[i] == 0) {
            printf("| |     ");
        } else {
            printf("|*|     ");
        }
    }

    if (ruleBitArray[0] == 0) {
        printf("| |\n");
    } 
    else if (ruleBitArray[0] == 1) {
        printf("|*|\n");
    }
    printf("\n");

    //TODO: Task 6 - read in the total number of generation evolution 
    //      steps from the user and initialize the world with ONLY the 
    //      middle cell active, all other cells should be inactive; 
    //      make sure to set the state array for each cell.
    setStates(world);

    int numGenerations;
    printf("Enter the number of generations: ");
    scanf("%d", &numGenerations);
    // Initialize the world with all cells initially inactive
    for (int j = 0; j < WORLD_SIZE; j++) {
        world[j].active = 0;
    }

    // Activate the middle cell
    world[WORLD_SIZE / 2].active = 1;

    // Set the states for the world
    setStates(world);
    printf("Initializing world & evolving...\n");
    printf("                                *                                \n");

    //TODO: Task 8 - evolve the world the user-specified number  
    //      of generations, printing each active cell as '*' and
    //      each non-active cell as ' ' (whitespace) after each
    //      evolution step to the next generation
    for (int k = 1; k < numGenerations; k++) {
        evolveWorld(world, ruleBitArray);

        for (int i = 0; i < WORLD_SIZE; i++) {
            if (world[i].active == 1) 
            {
                printf("*");
            } 
            else 
            {
                printf(" ");
            }
        }
        printf("\n");

        setStates(world);
    }
    return 0;
}
