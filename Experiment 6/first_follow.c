#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RULES 10
#define MAX_RULE_LEN 50
#define MAX_FIRST_SET_LEN 100
#define MAX_FOLLOW_SET_LEN 50

// Global variables for grammar input and computed sets
int ruleCount;                              // Number of grammar rules
char grammarRules[MAX_RULES][MAX_RULE_LEN];   // Array to store grammar productions
char firstSets[MAX_RULES][MAX_FIRST_SET_LEN];   // FIRST sets for each distinct nonterminal
char tempFirst[MAX_FIRST_SET_LEN];            // Temporary storage for FIRST symbols
int tempFirstIndex = 0;                       // Index into tempFirst
int followIndex = 0;                          // Index into the global FOLLOW array
char tempFollow[MAX_FOLLOW_SET_LEN];          // Temporary storage for FOLLOW symbols

// Recursion guard for FOLLOW computation (indexed by ASCII value)
int followRecursionGuard[128] = {0};

// Function prototypes (forward declarations)
void resetGlobals();
void computeFirst(char symbol, int prodIndex, int bodyIndex);
void computeFollow(char symbol);
void computeFollowFirst(char symbol, int prodIndex, int bodyIndex);
void runUserInput();

// ----------------------------------------------------------------
// resetGlobals()
// Resets global arrays and counters before processing input.
void resetGlobals() {
    tempFirstIndex = 0;
    followIndex = 0;
    memset(tempFollow, 0, sizeof(tempFollow));
    for (int i = 0; i < MAX_RULES; i++) {
        for (int j = 0; j < MAX_FIRST_SET_LEN; j++) {
            firstSets[i][j] = '!';
        }
    }
    memset(followRecursionGuard, 0, sizeof(followRecursionGuard));
}

// ----------------------------------------------------------------
// computeFirst()
// Recursively computes the FIRST set for a symbol and appends it to tempFirst.
// If the symbol is not an uppercase letter (i.e. a terminal), it is added directly.
void computeFirst(char symbol, int prodIndex, int bodyIndex) {
    if (!isupper(symbol)) {
        tempFirst[tempFirstIndex++] = symbol;
        return;
    }
    for (int i = 0; i < ruleCount; i++) {
        if (grammarRules[i][0] == symbol) {
            // Check for epsilon represented by '#' (if applicable)
            if (grammarRules[i][2] == '#') {
                if (grammarRules[prodIndex][bodyIndex] == '\0')
                    tempFirst[tempFirstIndex++] = '#';
                else if (grammarRules[prodIndex][bodyIndex] != '\0' && (prodIndex != 0 || bodyIndex != 0))
                    computeFirst(grammarRules[prodIndex][bodyIndex], prodIndex, bodyIndex + 1);
                else
                    tempFirst[tempFirstIndex++] = '#';
            }
            else if (!isupper(grammarRules[i][2])) {
                tempFirst[tempFirstIndex++] = grammarRules[i][2];
            } else {
                computeFirst(grammarRules[i][2], i, 3);
            }
        }
    }
}

// ----------------------------------------------------------------
// computeFollow()
// Recursively computes the FOLLOW set for a symbol and stores the results in tempFollow.
// If the symbol is the start symbol (grammarRules[0][0]), '$' is added.
void computeFollow(char symbol) {
    if (followRecursionGuard[(int)symbol])
        return;
    followRecursionGuard[(int)symbol] = 1;
    
    if (grammarRules[0][0] == symbol) {
        int exists = 0;
        for (int i = 0; i < followIndex; i++) {
            if (tempFollow[i] == '$') { exists = 1; break; }
        }
        if (!exists)
            tempFollow[followIndex++] = '$';
    }
    
    // Scan all grammar rules for occurrences of 'symbol' in the production body.
    for (int i = 0; i < ruleCount; i++) {
        int len = strlen(grammarRules[i]);
        // Production format: LHS '=' ProductionBody.
        // Assume production body starts at index 2 (index 1 is the separator).
        for (int j = 2; j < len; j++) {
            if (grammarRules[i][j] == symbol) {
                // If there is a symbol following 'symbol', add its FIRST set.
                if (j + 1 < len)
                    computeFollowFirst(grammarRules[i][j+1], i, j+2);
                // If 'symbol' is at the end of the production,
                // add FOLLOW(LHS) to FOLLOW(symbol) and union FIRST set of that nonterminal if applicable.
                if (j + 1 >= len) {
                    computeFollow(grammarRules[i][0]);
                    if (isupper(grammarRules[i][j])) {
                        int r;
                        for (r = 0; r < ruleCount; r++) {
                            if (firstSets[r][0] == grammarRules[i][j])
                                break;
                        }
                        int col = 1;
                        while (firstSets[r][col] != '!' && firstSets[r][col] != '\0') {
                            char sym = firstSets[r][col];
                            int exists = 0;
                            for (int t = 0; t < followIndex; t++) {
                                if (tempFollow[t] == sym) { exists = 1; break; }
                            }
                            if (!exists)
                                tempFollow[followIndex++] = sym;
                            col++;
                        }
                    }
                }
            }
        }
    }
    followRecursionGuard[(int)symbol] = 0;
}

// ----------------------------------------------------------------
// computeFollowFirst()
// Helper for FOLLOW computation: if the next symbol is a terminal, add it;
// if it's a nonterminal, add its FIRST set.
void computeFollowFirst(char symbol, int prodIndex, int bodyIndex) {
    if (!isupper(symbol)) {
        int exists = 0;
        for (int i = 0; i < followIndex; i++) {
            if (tempFollow[i] == symbol) { exists = 1; break; }
        }
        if (!exists)
            tempFollow[followIndex++] = symbol;
    } else {
        int r;
        for (r = 0; r < ruleCount; r++) {
            if (firstSets[r][0] == symbol)
                break;
        }
        int j = 1;
        while (firstSets[r][j] != '!' && firstSets[r][j] != '\0') {
            if (firstSets[r][j] != '#') {
                int exists = 0;
                for (int i = 0; i < followIndex; i++) {
                    if (tempFollow[i] == firstSets[r][j]) { exists = 1; break; }
                }
                if (!exists)
                    tempFollow[followIndex++] = firstSets[r][j];
            } else {
                if (grammarRules[prodIndex][bodyIndex] == '\0')
                    computeFollow(grammarRules[prodIndex][0]);
                else
                    computeFollowFirst(grammarRules[prodIndex][bodyIndex], prodIndex, bodyIndex+1);
            }
            j++;
        }
    }
}

// ----------------------------------------------------------------
// runUserInput()
// Reads user input for grammar productions, computes FIRST and FOLLOW sets,
// and prints the results.
void runUserInput() {
    int i, j, k;
    char nonTerminal;
    
    // Instructions for input:
    printf("Enter the number of productions (max %d): ", MAX_RULES);
    scanf("%d", &ruleCount);
    
    printf("\nEnter each production in the format:\n");
    printf("  NonTerminal=ProductionBody\n");
    printf("For example, to enter A -> aAaB, type: A=aAaB\n");
    printf("If '=' must appear as a terminal, use double '=' (e.g., S==A=a)\n\n");
    
    for (i = 0; i < ruleCount; i++) {
        printf("Enter production %d: ", i + 1);
        scanf("%s", grammarRules[i]);
    }
    
    resetGlobals();
    
    // ----- Compute FIRST sets for each distinct nonterminal -----
    char processedNonTerminals[10] = {0};
    int nonTerminalCount = 0;
    int firstSegmentIndex = 0; // starting index in tempFirst for current nonterminal
    for (i = 0; i < ruleCount; i++) {
        nonTerminal = grammarRules[i][0];
        int alreadyProcessed = 0;
        for (j = 0; j < nonTerminalCount; j++) {
            if (processedNonTerminals[j] == nonTerminal) { alreadyProcessed = 1; break; }
        }
        if (alreadyProcessed)
            continue;
        
        computeFirst(nonTerminal, 0, 0);
        processedNonTerminals[nonTerminalCount++] = nonTerminal;
        
        // Copy computed FIRST symbols (from tempFirst) into firstSets for this nonterminal.
        int pos = 0;
        firstSets[nonTerminalCount - 1][pos++] = nonTerminal;  // Save nonterminal as first element
        for (j = firstSegmentIndex; j < tempFirstIndex; j++) {
            int duplicate = 0;
            for (k = firstSegmentIndex; k < j; k++) {
                if (tempFirst[k] == tempFirst[j]) { duplicate = 1; break; }
            }
            if (!duplicate) {
                firstSets[nonTerminalCount - 1][pos++] = tempFirst[j];
            }
        }
        firstSets[nonTerminalCount - 1][pos] = '\0';
        firstSegmentIndex = tempFirstIndex; // Move to next segment for next nonterminal
    }
    
    // Print FIRST sets
    printf("\nComputed FIRST Sets:\n");
    for (i = 0; i < nonTerminalCount; i++) {
        printf(" First(%c) = { ", firstSets[i][0]);
        for (j = 1; firstSets[i][j] != '\0'; j++) {
            printf("%c, ", firstSets[i][j]);
        }
        printf("}\n");
    }
    
    printf("\n-----------------------------------------------\n\n");
    
    // ----- Compute FOLLOW sets for each distinct nonterminal -----
    char processedForFollow[10] = {0};
    int processedFollowCount = 0;
    for (i = 0; i < ruleCount; i++) {
        nonTerminal = grammarRules[i][0];
        int alreadyProcessed = 0;
        for (j = 0; j < processedFollowCount; j++) {
            if (processedForFollow[j] == nonTerminal) { alreadyProcessed = 1; break; }
        }
        if (alreadyProcessed)
            continue;
        computeFollow(nonTerminal);
        processedForFollow[processedFollowCount++] = nonTerminal;
    }
    
    // Compute union of all FOLLOW symbols from tempFollow.
    char unionFollow[MAX_FOLLOW_SET_LEN];
    int unionCount = 0;
    for (i = 0; i < followIndex; i++) {
        int exists = 0;
        for (j = 0; j < unionCount; j++) {
            if (unionFollow[j] == tempFollow[i]) { exists = 1; break; }
        }
        if (!exists)
            unionFollow[unionCount++] = tempFollow[i];
    }
    
    // Print FOLLOW sets (the same union is printed for each nonterminal)
    printf("Computed FOLLOW Sets:\n");
    for (i = 0; i < processedFollowCount; i++) {
        printf(" Follow(%c) = { ", processedForFollow[i]);
        for (j = 0; j < unionCount; j++) {
            printf("%c, ", unionFollow[j]);
        }
        printf("}\n");
    }
}

int main() {
    runUserInput();
    return 0;
}
