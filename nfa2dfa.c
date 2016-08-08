/*
 * nfa2dfa.c
 *
 *  Created on: 8 de ago de 2016
 *      Author: luan
 */

//**************************************
// Name: NFA to DFA Conversion
// Description:It is a program for NFA ( Non-deterministic Finite Automata) to DFA (Deterministic Finite Auctomata ) Conversion using the Subset Construction Algorithm.
// By: Ritin (from psc cd)
//
// Inputs:NFA states, inputs, transitions
//
// Returns:DFA transition table
//**************************************
//NFA to DFA conversion
#include <stdio.h>
#include <string.h>
#define STATES 50

void pushA(int z);
void pushB(int z);

struct Alphabet {
	char symbols[127];
	int size;
} ascii;

struct Dstate {
	char name;
	char StateString[STATES + 1];
	char trans[10];
	int is_final;
} Dstates[50];

struct tran {
	char symbol;
	int tostates[2];
	int notran;
};

struct state {
	int no;
	struct tran transitions[127];
};

int stackA[100], stackB[100];
int C[100];
int Cptr = -1, Aptr = -1, Bptr = -1;

struct state states[STATES];
char temp[STATES + 1], inp[10];
int nos, noi, nof, j, k, nods = -1; //states,initial,final

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
	sort();
	int i = 0;

	for (int i = 0; i <= Bptr; i++)
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

void readString() {
	int final, start, fin = 0, i;
	//	char ans, st[20];
	char str[1000];
	char symbols[95] = { ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*',
			'+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7',
			'8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D',
			'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
			'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^',
			'_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
			'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
			'y', 'z', '{', '|', '}', '~' };

	printf("\nDigite sua busca: ");
	scanf("%s", str);

	nof = 1;
	noi = sizeof(symbols);
	nos = strlen(str) + 1;
	for (int i = 0; i < nos; i++, states[i].no = i);
	for (int i = 0; i < noi; i++) {
		ascii.symbols[i] = symbols[i];
	}

	start = 0;
	final = nos - 1;

	// Adiciona uma transição para o próprio estado com cada símbolo do
	// alfabeto. Válido somente para os estados inicial e final.
	for (int i = 0; i < nos; i++) {
		for (int j = 0; j < noi; j++) {
			states[start].transitions[j].symbol = ascii.symbols[j];
			states[final].transitions[j].symbol = ascii.symbols[j];

			states[i].transitions[j].tostates[0] = -1;
			states[start].transitions[j].tostates[0] = start;
			states[final].transitions[j].tostates[0] = final;

			if (str[i] == symbols[j]) {
				states[i].transitions[j].tostates[(i == 0) ? 1 : 0] = i+1;
			}
		}
	}

	for (i = 0; i < nos; i++) {
		for (j = 0; j < noi; j++) {
			printf("(%d, %c) = %d\n", i, symbols[j],
					states[i].transitions[j].tostates[0]);
			if (i == 0 && str[i] == symbols[j]) {
				printf("(%d, %c) = %d\n", i, symbols[j],
						states[i].transitions[j].tostates[1]);
			}
		}
	}

//Conversion
//	i = 0;
//	nods = 0;
//	fin = 0;
//	pushA(start);
//	lambda_closure(peekA());
//	tostring();
//	Dstates[nods].name = 'A';
//	nods++;
//	strcpy(Dstates[0].StateString, temp);
//	while (i < nods) {
//		for (j = 0; j < noi; j++) {
//			fin = 0;
//			copy(i);
//			while (Bptr != -1) {
//				move(popB(), j);
//			}
//			while (Aptr != -1)
//				lambda_closure(peekA());
//			tostring();
//			for (k = 0; k < nods; k++) {
//				if ((strcmp(temp, Dstates[k].StateString) == 0)) {
//					Dstates[i].trans[j] = Dstates[k].name;
//					break;
//				}
//			}
//			if (k == nods) {
//				nods++;
//				for (k = 0; k < nof; k++) {
//					fin = seek(stackB, Bptr, final[k]);
//					if (fin == 1) {
//						Dstates[nods - 1].is_final = 1;
//						break;
//					}
//				}
//				strcpy(Dstates[nods - 1].StateString, temp);
//				Dstates[nods - 1].name = 'A' + nods - 1;
//				Dstates[i].trans[j] = Dstates[nods - 1].name;
//			}
//		}
//		i++;
//	}
}

void display_DTran() {
	int i, j;
	printf("\n DFA Transition Table ");
	printf("\n -------------------- ");
	printf("\nStates\tString\tInputs\n");
	for (i = 0; i < noi; i++) {
		printf("%c ", inp[i]);
	}
	printf("\n \t----------");
	for (i = 0; i < nods; i++) {

		if (Dstates[i].is_final == 0)
			printf("\n%c", Dstates[i].name);
		else
			printf("\n*%c", Dstates[i].name);

		printf("\t%s", Dstates[i].StateString);
		for (j = 0; j < noi; j++) {
			printf("\t%c", Dstates[i].trans[j]);
		}
	}
	printf("\n");
}

void move(int st, int j) {
	int ctr = 0;
	while (ctr < states[st].transitions[j].notran) {
		pushA(states[st].transitions[j].tostates[ctr++]);
	}
}

void lambda_closure(int st) {
	int ctr = 0, in_state = st, curst = st, chk;
	while (Aptr != -1) {
		curst = popA();
		ctr = 0;
		in_state = curst;
		while (ctr <= states[curst].transitions[noi].notran) {
			chk = seek(stackB, Bptr, in_state);
			if (chk == 0)
				pushB(in_state);
			in_state = states[curst].transitions[noi].tostates[ctr++];
			chk = seek(stackA, Aptr, in_state);
			if (chk == 0 && ctr <= states[curst].transitions[noi].notran)
				pushA(in_state);
		}
	}
}

void pushA(int z) {
	stackA[++Aptr] = z;
}
void pushB(int z) {
	stackB[++Bptr] = z;
}

int popA() {
	return stackA[Aptr--];
}
int popB() {
	return stackB[Bptr--];
}
int peekB() {
	return stackA[Bptr];
}
int peekA() {
	return stackA[Aptr];
}

int seek(int arr[], int ptr, int s) {

	for (int i = 0; i <= ptr; i++)
		if (s == arr[i])
			return 1;

	return 0;

}

int main() {
	readString();
	//conversion();
	//display_DTran();

	return 0;
}
