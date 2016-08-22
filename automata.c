/*
 * automata.c
 *
 * This implementation has as its purpose to receive a string from the default
 * input and check wether it's a substring of a given text, or not.
 *
 * To do so, first we convert the string into a NFA (Non-deterministi finite au-
 * tomata) for matter of simplicity.
 *
 * Then, we convert the NFA into a DFA (Deterministic Finite Automata), because
 * of its deterministics properties and simple search algorithm.
 *
 * Last, we search the string as a substring of a given text, using the determi-
 * nism of the new computational model.
 *
 * Created on: 8 de ago de 2016
 * Last modified on: 22 de ago de 2016
 * Author: Luan Sousa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATES 50
#define FAILURE 0
#define SUCCESS 1
#define alphSize 2 // Size of the alphabet
#define maxTrans 2  // Maximum transitions of the NFA

struct alphabet {
	int size;
	char symbols[alphSize];
} ascii;

struct DFAstate {
    int isFinal;
	char label;
	char StateString[STATES + 1];
	char transitions[alphSize];
    int transitionsI[alphSize];
} DFAstates[STATES];

struct transition {
    int notran;
	char symbol;
	int tostates[maxTrans];
};

struct NFAstate {
	int label;
	struct transition
           transitions[alphSize+1];
};

int stackA[100], Aptr = -1;
int stackB[100], Bptr = -1;
int C[100],      Cptr = -1;

char str[1000];
char temp[STATES+1];

int i, j, k, fin, nods = -1;

int final;
int start;
int numberOfStates;
int numberOfSymbols;
struct NFAstate states[50];
char symbols[alphSize] = { ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')',
       '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7',
       '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D',
       'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
       'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^',
       '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
       'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
       'y', 'z', '{', '|', '}', '~' };

/*char symbols[alphSize] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
        'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
		'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
*/
/*char symbols[alphSize] = { '0', '1'};*/


/*---------- Stack Manipulation ----------*/
int popA() { return stackA[Aptr--]; }
int popB() { return stackB[Bptr--]; }

int peekB() { return stackA[Bptr]; }
int peekA() { return stackA[Aptr]; }

void pushA(int z) { stackA[++Aptr] = z; }
void pushB(int z) { stackB[++Bptr] = z; }
/*----------------------------------------*/

void sort() {
    int i, j, temp;
    for (i = 0; i < Bptr; i++) {
        for (j = 0; j < (Bptr - i); j++) {
            if (stackB[j] > stackB[j + 1]) {
                temp = stackB[j];
                stackB[j] = stackB[j + 1];
                stackB[j + 1] = temp;
            }
        }
    }
}

void tostring() {
    int i = 0;
    sort();

    for (i = 0; i <= Bptr; i++) {
        temp[i] = stackB[i] + '0';
    }

    temp[i] = '\0';
}

void copy(int i) {
    char temp[STATES + 1] = " ";
    int k = 0;
    Bptr = -1;
    strcpy(temp, DFAstates[i].StateString);

    while (temp[k] != '\0') {
        pushB(temp[k] - '0');
        k++;
    }
}

int seek(int arr[], int ptr, int s) {
    int i;

    for (i = 0; i <= ptr; i++) {
        if (s == arr[i]) return 1;
    }

    return 0;
}

void move(int st, int j) {
    int ctr = 0;

    while (ctr < states[st].transitions[j].notran) {
        pushA(states[st].transitions[j].tostates[ctr++]);
    }
}

void lambda_closure(int NFAstate) {
    int chk;
    int ctr = 0;
    int inState = NFAstate;
    int currentState = NFAstate;

    while (Aptr != -1) {
        currentState = popA();
        ctr = 0;
        inState = currentState;

        while (ctr <= states[currentState].transitions[alphSize+1].notran) {
            chk = seek(stackB, Bptr, inState);
            if (chk == 0)
                pushB(inState);
            inState = states[currentState].transitions[alphSize+1].tostates[ctr++];
            chk = seek(stackA, Aptr, inState);
            if (chk == 0 && ctr <= states[currentState].transitions[alphSize+1].notran)
                pushA(inState);
        }
    }
}

void readString() {
    printf("\nDigite sua busca: ");
    fgets(str, STATES, stdin);

    numberOfStates  = strlen(str);
    numberOfSymbols = alphSize;

    start = 0;
    final = numberOfStates - 1;

    for (int i = 0; i < numberOfStates; i++) {
        states[i].label = i;
    }

    for (int i = 0; i < numberOfSymbols; i++) {
        ascii.symbols[i] = symbols[i];
    }

    // Adiciona uma transição para o próprio estado com cada símbolo do
    // alfabeto. Válido somente para os estados inicial e final.
    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j < numberOfSymbols; j++) {
            states[i].transitions[j].symbol = ascii.symbols[j];
            states[i].transitions[j].notran = 0;
        }
        states[i].transitions[alphSize+1].notran = 0;
    }

    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j < numberOfSymbols; j++) {
            int k = 0;
            states[i].transitions[j].tostates[k] = k;
            if (i == start) {
                states[start].transitions[j].tostates[k++] = i;
                states[start].transitions[j].notran = k;
            }
            if (i == final) {
                states[final].transitions[j].tostates[k++] = i;
                states[final].transitions[j].notran = k;
            }
        }
    }

    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j <= numberOfSymbols; j++) {
            int k = states[i].transitions[j].notran;
            if (str[i] == symbols[j]) {
                states[i].transitions[j].tostates[k++] = i+1;
                states[i].transitions[j].notran = k;
            }
        }
    }
}

void conversion() {
    i = 0;
    nods = 0;
    fin = 0;
    pushA(start); // Push NFA's initial NFAstate into stackA

    lambda_closure(peekA()); // Perform lambda closure on NFA's initial NFAstate
    tostring();
    DFAstates[nods].label = 'A';
    nods++;
    strcpy(DFAstates[0].StateString, temp);

    while (i < nods) {
		for (j = 0; j < numberOfSymbols; j++) {
            fin = 0;
            copy(i);

            while (Bptr != -1) { move(popB(), j); }
            while (Aptr != -1) { lambda_closure(peekA()); }

            tostring();

			for (k = 0; k < nods; k++) {
               if ((strcmp(temp, DFAstates[k].StateString) == 0)) {
				   DFAstates[i].transitions[j] = DFAstates[k].label;
				   DFAstates[i].transitionsI[j] = k;
				   break;
			   }
            }

            if (k == nods) {
                nods++;
                for (k = 0; k < 1; k++) {
                    fin = seek(stackB, Bptr, final);
                    if (fin == 1) {
                        DFAstates[nods - 1].isFinal = 1;
                        break;
                    }
                }
                strcpy(DFAstates[nods - 1].StateString, temp);
                DFAstates[nods - 1].label = 'A' + nods - 1;
                DFAstates[i].transitions[j] = DFAstates[nods - 1].label;
                DFAstates[i].transitionsI[j] = nods - 1;
            }
        }

        i++;
    }
}

int search() {
	char txt[1000];
	printf("Digite o texto: ");
	fgets(txt, 1000, stdin);
	
	int i, j, k, p = -1;
	int N = strlen(txt);
	int M = strlen(str);
	
	for (i = 0, k = 0; i < N-1 && k < M-1; i++) {
		for (j = 0; j < M-1; j++) {
			if (txt[i] == str[j]) {
				p = j;
				break;
			}
		}
		
		if (p != -1) {
			k = DFAstates[i].transitionsI[k];
		}
	}
	if (DFAstates[k].isFinal == 1) {
		return 1;
	}

    return 0;
}

void printNFA() {
    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j < numberOfSymbols; j++) {
            // if (str[i] == symbols[j]) {
                printf("(%d, %c) = %d | \t", i, ascii.symbols[j],
                    states[i].transitions[j].tostates[0]);
                if (i == 0 && str[i] == symbols[j]) {
                    printf("(%d, %c) = %d | \t", i, ascii.symbols[j],
                        states[i].transitions[j].tostates[1]);
                }
                // printf("notran = %d\n", states[i].transitions[j].notran);
                // printf("%d, ", states[i].transitions[j].notran);
           // }
        }
        // printf("%d, ", states[i].transitions[alphSize+1].notran);
        printf("\n");
    }
}

void printDFA() {
    printf("\n       DFA Transition Table      ");
    printf("\n---------------------------------");
    printf("\nStates\tString\t\tInputs\n\t");

    for (int i = 0; i < numberOfSymbols; i++) printf("%c    ", symbols[i]);

    printf("\n---------------------------------");

    for (int i = 0; i < nods; i++) {
        if (DFAstates[i].isFinal == 0) printf("\n%c",DFAstates[i].label);
        else printf("\n*%c",DFAstates[i].label);

        printf("\t%s",DFAstates[i].StateString);

        for (int j = 0; j < numberOfSymbols; j++) {
            printf("\t%c, (%d)", DFAstates[i].transitions[j], DFAstates[i].transitionsI[j]);
        }
    }

    printf("\n");
}

int main() {
    readString();
    printNFA();
    conversion();
    printDFA();
    printf("\nA string buscada %sé uma substring do texto!\n\n",
                            (search() == SUCCESS)? "" : "não ");

   return 0;
}

//Fim