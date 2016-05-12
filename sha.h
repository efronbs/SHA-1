#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint unsigned int

extern uint K_0, K_1, K_2, K_3;

void printBytes(char *buf, int bytes, int bytesPerLine);
unsigned int setupBuffer(char **buf);
void padBuffer(char *buf, int msg_len, int full_length);
char *initHash();
void runHashRound(char *hash, char *msg);
void packWSet(char *msg, char **w_set);
char *xorCharArray(char arr1[4], char arr2[4]);
void circularShift(char *w_current);
void performPermutations(char *A, char *B, char *C, char *D, char *E, char **w_set);
uint charArrayToInt(char *charArray);
void intToCharArray(char *finalArray, uint numForm);
void runCompression(char *A, char *B, char *C, char *D, char *E, uint intA, uint intB, uint intC, uint intD, uint intE, char **w_set, char *hash);
uint genT(uint intA, uint intB, uint intC, uint intD, uint intE, uint intW, int t);
uint FofT(uint B, uint C, uint D, int t);
uint circularShiftFive(uint val);
uint addMod(uint var1, uint var2);
uint circularShiftThirty(uint val);
