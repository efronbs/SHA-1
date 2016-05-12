#include "sha.h"

#define uint unsigned int

char *runHashRound(char *hash, char *msg)
{
        int i;
        char **w_set;
        w_set = malloc(80 * sizeof(char*));
        packWSet(msg, w_set);
        char *A, *B, *C, *D, *E;
        A = (char *) malloc(4); B = (char *) malloc(4); C = (char *) malloc(4); D = (char *)
         malloc(4); E = (char *) malloc(4);

        unsigned int intA, intB, intC, intD, intE;
        intA = charArrayToInt(hash); intB = charArrayToInt(hash + 4); intC = charArrayToInt(hash + 8);
        intD = charArrayToInt(hash + 12); intE = charArrayToInt(hash + 16);
        runCompression(A, B, C, D, E, intA, intB, intC, intD, intE, w_set, hash);
        for (i = 0; i < 20; i++)
        {
                if (i < 4)
                        hash[i] = A[i%4];
                else if (i < 8)
                        hash[i] = B[i%4];
                else if (i < 12)
                        hash[i] = C[i%4];
                else if (i < 16)
                        hash[i] = D[i%4];
                else
                        hash[i] = E[i%4];
        }
        free(A); free(B); free(C); free(D); free(E);
}

void packWSet(char *msg, char **w_set)
{
        int i;

        for(i = 0; i < 16; i++)
        {
                char *temp = (char *) malloc(4);
                memcpy(temp, (msg + (i * 4)), 4);
                w_set[i] = temp;
        }
        
        for (i = 16; i < 80; i++)
        {
                char *temp;
                char *w_current = (char *) malloc(4);
                memset(w_current, 0, 4);

                temp = xorCharArray(w_set[i-3], w_set[i-8]);
                memcpy(w_current, temp, 4);
                free(temp);

                temp = xorCharArray(w_current, w_set[i-14]);
                memcpy(w_current, temp, 4);
                free(temp);

                temp = xorCharArray(w_current, w_set[i-16]);
                memcpy(w_current, temp, 4);
                free(temp);

                circularShift(w_current);
                w_set[i] = w_current;
        }
}

void runCompression(char *A, char *B, char *C, char *D, char *E, uint intA, uint intB, uint intC, uint intD, uint intE, char **w_set, char *hash)
{       
        int T, intW, t = 0; 
        for (t = 0; t < 80; t++)
        {
                intW = charArrayToInt(w_set[t]); 
                T = genT(intA, intB, intC, intD, intE, intW, t);
                intE = intD;
                intD = intC;
                intC = circularShiftThirty(intB);
                intB = intA;
                intA = T;
        }

        intA = addMod(intA, charArrayToInt(hash));
        intB = addMod(intB, charArrayToInt(hash + 4));
        intC = addMod(intC, charArrayToInt(hash + 8));
        intD = addMod(intD, charArrayToInt(hash + 12));
        intE = addMod(intE, charArrayToInt(hash + 16));

        intToCharArray(A, intA);
        intToCharArray(B, intB);
        intToCharArray(C, intC);
        intToCharArray(D, intD);
        intToCharArray(E, intE);
}

uint genT(uint intA, uint intB, uint intC, uint intD, uint intE, uint intW, int t)
{
        uint T;
        T = circularShiftFive(intA);
        uint FofT_res = FofT(intB, intC, intD, t);
        T = addMod(T, FofT_res);
        T = addMod(T,intE);
        T = T + intW;

        if (t <= 19)
                T = T + K_0;
        else if (t <= 39)
                T = T + K_1;
        else if (t <= 59)
                T = T + K_2;
        else if (t <= 79)
                T = T + K_3;
        return T;

}

uint FofT(uint B, uint C, uint D, int t)
{
        uint finalVal;
        if (t <= 19)
        {
                uint temp1, temp2;
                temp1 = B & C;
                temp2 = (~B) & D;
                finalVal = temp1 | temp2;
        } else if (t <= 39) {
                finalVal = ((B ^ C) ^ D);
        } else if (t <= 59) {
                finalVal = ((B & C) | (B & D) | (C & D));
        } else {
                finalVal = ((B^C) ^ D);
        }
        return finalVal;
}

