/*
 * nfa2dfa.c
 *
 *  Created on: 8 de ago de 2016
 *      Author: luan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATES 50
#define alphSize 8
#define maxTrans 2

struct alphabet {
	int size;
	char symbols[alphSize];
} ascii;

struct Dstate {
	char name;
	char StateString[STATES + 1];
	char trans[10];
	int is_final;
} Dstates[50];

struct transition {
    int notran;
	char symbol;
	int tostates[maxTrans];
};

struct state {
	int label;
	struct transition
           transitions[alphSize+1];
};

int stackA[100], stackB[100];
int C[100];
char temp[STATES+1];
int Cptr = -1, Aptr = -1, Bptr = -1, nods = -1;

int i, j, k, fin;

int final;
int start;
char str[1000];
int numberOfStates;
int numberOfSymbols;
struct state states[50];
/*char symbols[alphSize] = { ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')',
        '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D',
        'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
        'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^',
        '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
        'y', 'z', '{', '|', '}', '~' };*/

//char symbols[alphSize] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
//		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
//        'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
//		'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

char symbols[alphSize] = { 'l', 'u', 'a', 'n', ' ', 'c', 'p', 'k' };
// char symbols[alphSize] = { 'l', ' ', 'a' };


/*---------- Stack Manipulation ----------*/
void pushA(int z) { stackA[++Aptr] = z; }
void pushB(int z) { stackB[++Bptr] = z; }

int popA() { return stackA[Aptr--]; }
int popB() { return stackB[Bptr--]; }

int peekB() { return stackA[Bptr]; }
int peekA() { return stackA[Aptr]; }
/*----------------------------------------*/



void sort() {
    int i, j,  temp;
    for (i = 0; i < Bptr; i++) {
        printf("passou por aqui\n");
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

    for (i = 0; i <= Bptr; i++)
        temp[i] = stackB[i] + '0';

    temp[i] = '\0';
}

void copy(int i) {
    char temp[STATES + 1] = " ";
    int k = 0;
    Bptr = -1;
    strcpy(temp, Dstates[i].StateString);

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

void lambda_closure(int state) {
    int chk;
    int ctr = 0;
    int inState = state;
    int currentState = state;

    while (Aptr != -1) {
        currentState = popA();
        ctr = 0;
        inState = currentState;
        // printf("currentState: %d\n", currentState);
        // printf("notran: %d\n", states[currentState].transitions[76].notran);
        // chk = seek(stackB, Bptr, inState);
        // printf("chk: %d\n", chk);
        // pushB(inState);
        // printf("stackB: %d\n", peekB());
        // inState = states[currentState].transitions[alphSize].tostates[ctr++];
        // printf("inState: %d\n", inState);
        // chk = seek(stackA, Aptr, inState);
        // printf("chk: %d\nctr: %d\n\n", chk, ctr);

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
    fgets(str, STATES+1, stdin);
    // scanf("%[^]s", str);


    // printf("%d", strlen(str));

    numberOfStates  = strlen(str);
    numberOfSymbols = alphSize;

    start = 0;
    final = numberOfStates - 1;

    for (int i = 0; i < numberOfStates; i++)
        states[i].label = i;
    for (int i = 0; i < numberOfSymbols; i++)
        ascii.symbols[i] = symbols[i];
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

void printNFA() {
    for (int i = 0; i < numberOfStates; i++) {
        for (int j = 0; j < numberOfSymbols; j++) {
            // if (str[i] == symbols[j]) {
            //     printf("(%d, %c) = %d | \t", i, ascii.symbols[j],
            //         states[i].transitions[j].tostates[0]);
            //     if (i == 0 && str[i] == symbols[j]) {
            //         printf("(%d, %c) = %d | \t", i, ascii.symbols[j],
            //             states[i].transitions[j].tostates[1]);
            //     }
                // printf("notran = %d\n", states[i].transitions[j].notran);
                printf("%d, ", states[i].transitions[j].notran);
            // }
        }
        printf("%d, ", states[i].transitions[alphSize+1].notran);
        printf("\n");
    }
}

void conversion() {
    i = 0;
    nods = 0;
    fin = 0;
    pushA(start); // Push NFA's initial state into stackA

    lambda_closure(peekA()); // Perform lambda closure on NFA's initial state
    tostring();
    Dstates[nods].name = 'A';
    nods++;
    strcpy(Dstates[0].StateString, temp);

    while (i < nods) {
        printf("passei por aqui %d\n", i);
        for (j = 0; j < numberOfSymbols; j++) {
            fin = 0;
            copy(i);

            while (Bptr != -1) { move(popB(), j); }
            while (Aptr != -1) { lambda_closure(peekA()); }

            tostring();

            for (k = 0; k < nods; k++) {
               if ((strcmp(temp, Dstates[k].StateString) == 0)) {
                    Dstates[i].trans[j] = Dstates[k].name;
                    break;
                }
            }

            if (k == nods) {
                nods++;
                for (k = 0; k < 1; k++) {
                    fin = seek(stackB, Bptr, final);
                    if (fin == 1) {
                        Dstates[nods - 1].is_final = 1;
                        break;
                    }
                }
                strcpy(Dstates[nods - 1].StateString, temp);
                Dstates[nods - 1].name = 'A' + nods - 1;
                Dstates[i].trans[j] = Dstates[nods - 1].name;
            }
        }

        i++;
    }
}

void printDFA() {
    printf("\n       DFA Transition Table ");
    printf("\n---------------------------------");
    printf("\nStates\tString\t\tInputs\n\t");

    for (int i = 0; i < numberOfSymbols; i++) printf("%c    ", symbols[i]);

    printf("\n---------------------------------");

    for (int i = 0; i < nods; i++) {
        if (Dstates[i].is_final == 0) printf("\n%c",Dstates[i].name);
        else printf("\n*%c",Dstates[i].name);

        printf("\t%s",Dstates[i].StateString);

        for (int j = 0; j < numberOfSymbols; j++) {
            printf("\t%c", Dstates[i].trans[j]);
        }
    }

    printf("\n");
}

int main() {
    readString();
    // printNFA();
    conversion();
    printDFA();
    return 0;
}


