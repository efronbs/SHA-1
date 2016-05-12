#include "sha.h"

#define uint unsigned int

void printBytes(char *buf, int bytes, int bytesPerLine)
{
        int i;
        for(i = 0; i < bytes; i++)
        {
                if (i%bytesPerLine == 0) {
                        printf("\n");
                }
                printf("%02x", (unsigned char) buf[i]);
        }
        printf("\n");
}

char *xorCharArray(char arr1[4], char arr2[4])
{
        char temp[4];
        for(int i = 0; i < 4; i++)
        {
                temp[i] = arr1[i] ^ arr2[i];
        }
        char *retVal = (char *) malloc(4);
        memcpy(retVal, temp, 4);
        return retVal;
}

void circularShift(char *w_current)
{
        signed char sigByte = w_current[0];
        int inByte = 0;
        if (sigByte < 0)
                inByte = 1;

        unsigned int i, numForm = 0;
        for (i = 0; i < 4; i++)
        {
                numForm = numForm | ((unsigned char) w_current[3 - i] << (8 * i));
        }
        numForm = numForm << 1;
        numForm = numForm | inByte;
        for (i = 0; i < 4; i++) {
                unsigned int temp = (unsigned int) (numForm << (8 * i));
                temp = temp >> 24;
                w_current[i] = (unsigned char) temp;
        }
}

uint circularShiftFive(uint val)
{
        unsigned int lowFive = 0, finalVal = 0;
        lowFive = lowFive | val;
        lowFive = lowFive >> 27;
        finalVal = val << 5;
        finalVal = finalVal | lowFive;
        return finalVal;
}

uint circularShiftThirty(uint val)
{
        return (val >> 2) | (val << 30);
}

uint charArrayToInt(char *charArray)
{
        int i, numForm = 0;
        for (i = 0; i < 4; i++)
        {
                numForm = numForm | ((unsigned char) charArray[3 - i] << (8 * i));
        }
        return numForm;
}

void intToCharArray(char *finalArray, uint numForm)
{
        int i;
        for (i = 0; i < 4; i++) {
                unsigned int temp = (unsigned int) (numForm << (8 * i));
                temp = temp >> 24;
                finalArray[i] = (unsigned char) temp;
        }
}

uint addMod(uint var1, uint var2)
{
        long long fin = var1 + var2;
        fin = fin % 4294967296; //mod 2^32
        return (uint) fin;

}

