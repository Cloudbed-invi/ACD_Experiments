# FIRST and FOLLOW Sets Calculator

This C program computes the **FIRST** and **FOLLOW** sets for a given grammar. It prompts the user to input grammar productions in a specified format and then calculates and displays the FIRST set for each distinct nonterminal along with a union of the FOLLOW sets.

## Features

- **User Input:** Accepts up to 10 grammar productions.
- **FIRST Set Computation:** Recursively calculates the FIRST set for each nonterminal.
- **FOLLOW Set Computation:** Recursively calculates the FOLLOW set, including handling of epsilon (`#`) productions.
- **Recursion Guard:** Uses a recursion guard to prevent infinite recursion during FOLLOW set computation.
- **User-friendly Output:** Displays computed FIRST and FOLLOW sets in a clear and organized manner.

## Grammar Input Format

- Productions must be entered in the format:  
  `NonTerminal=ProductionBody`  
  For example, to represent the production _A → aAaB_, enter:  
  ```
  A=aAaB
  ```
- If you need to use `=` as a terminal in the production body, use double equals (e.g., `S==A=a`).
- Epsilon (empty production) is represented by the character `#`.

## Compilation

To compile the program, use a C compiler such as `gcc`. For example, if your source file is named `main.c`, compile it with:

```bash
gcc -o first_follow main.c
```

This will create an executable named `first_follow`.

## Running the Program

After compiling, run the executable:

```bash
./first_follow
```

Follow the on-screen prompts:
- Enter the number of productions.
- Enter each production in the required format.

The program will then display the computed FIRST sets for each nonterminal and a union of the FOLLOW sets.

## Code Structure

- **Global Variables:**  
  - `grammarRules`: Array storing the grammar productions.
  - `firstSets`: Array to store computed FIRST sets for each nonterminal.
  - `tempFirst` and `tempFollow`: Temporary storage arrays for FIRST and FOLLOW computations.
  - `followRecursionGuard`: Array to prevent recursive loops during FOLLOW set computation.

- **Key Functions:**
  - `resetGlobals()`: Resets all global arrays and counters before processing the input.
  - `computeFirst(char symbol, int prodIndex, int bodyIndex)`: Recursively computes the FIRST set for a given symbol.
  - `computeFollow(char symbol)`: Recursively computes the FOLLOW set for a given symbol and adds the end marker `$` for the start symbol.
  - `computeFollowFirst(char symbol, int prodIndex, int bodyIndex)`: Helper function used in FOLLOW set computation for handling the symbol following a nonterminal.
  - `runUserInput()`: Handles user input, triggers the computation of FIRST and FOLLOW sets, and prints the results.

## Limitations

- The program is limited to a maximum of 10 grammar productions and assumes a maximum production length.
- Nonterminals must be represented by uppercase letters.
- The program may not handle all edge cases found in more complex grammars.

## License

This code was developed by me in accordance with my university standards. If you encounter any issues or would like to contribute via modifications or pull requests, please contact me directly.