#include "sha.h"
#define uint unsigned int	

uint K_0 = 0x5A827999;
uint K_1 = 0x6ED9EBA1;
uint K_2 = 0x8F1BBCDC;
uint K_3 = 0xCA62C1D6;

// TODO handle newlines in the middle of the text
int main()
{
	int i;
	FILE *fp;
	char *fname = "sha_input.txt";
	char *buf;
	unsigned char *pad_buf;
	unsigned int pad_len;

	fp = fopen(fname, "r");
	if (fp == NULL) {
		printf("OPENING THE FILE MESSED UP");
		exit(1);
	}
	
	buf = (char *) malloc(536870913);
	if (fgets(buf, 536870913, fp) == NULL)
	{
		printf("READING THE FILE FAILED\n");
		exit(1);
	}
	pad_len = setupBuffer(&buf);
	char *hash = initHash();
	for (i = 0; i < (pad_len / 64); i++)
	{
		char *currentBuf = buf + (i * 64); 
		runHashRound(hash, currentBuf);	
//		printBytes(hash, 20, 4);
	}	
	printBytes(hash,20,20);
	return 0;	
}

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

/******************** SETUP CODE ****************************/
unsigned int setupBuffer(char **buf)
{
	//first find the size of the last section. if there is less than 8 open bytes, resize 
	int msg_len, lastSectionSize, full_size;
	char *new_buf;
	msg_len = strlen(*buf)- 1;

	new_buf = (char *) malloc(msg_len);
	memcpy(new_buf, *buf, msg_len);
	lastSectionSize = (msg_len + 1) % 64; //pretty much assuming all input will be in byte form, which is a fair assumption due to text being in byte form
	if (64-lastSectionSize < 8) {
		full_size = msg_len + (64 - (lastSectionSize - 1)) + 64;
		*buf = (char *) malloc(full_size);
		memcpy(*buf, new_buf, msg_len);
	} else {
		full_size = (msg_len - (lastSectionSize - 1)) + 64;
		*buf = (char *) malloc(full_size);
		memcpy(*buf, new_buf, msg_len);
	}
	padBuffer(*buf, msg_len, full_size);
	free(new_buf);

//	printf("message length: %d\n", msg_len);
//	printf("padded length: %d\n", full_size);
//	printBytes(*buf, full_size, 4);
	return full_size;	
}

void padBuffer(char *buf, int msg_len, int full_length)
{
	int i;
	unsigned int temp, bit_len;
	bit_len = (unsigned int)  msg_len*8;
	for (i = 0; i < 4; i++)
	{
		int shift = 24 - (i * 8);
		temp = (unsigned int)  bit_len << shift;
		temp = temp >> 24; 
		unsigned char len_byte = (unsigned char) temp;
		buf[(full_length - 1) - i] = len_byte;
	}
	buf[msg_len] = 0x80;
	for (i = (msg_len + 1); i < (full_length - 4); i++)
	{
		buf[i] = 0;
	}
}

char *initHash()
{
	char *hash = (char *) malloc(20);
	hash[0]=0x67; hash[1]=0x45; hash[2]=0x23; hash[3]=0x01; hash[4]=0xEF; hash[5]=0xCD; hash[6]=0xAB; hash[7]=0x89; hash[8]=0x98;
	hash[9]=0xBA; hash[10]=0xDC; hash[11]=0xFE; hash[12]=0x10; hash[13]=0x32; hash[14]=0x54; hash[15]=0x76; hash[16]=0xC3; 
	hash[17]=0xD2; hash[18]=0xE1; hash[19]=0xF0;

	return hash;
}

/********************************* HASH GENERATION *************************************/

char *runHashRound(char *hash, char *msg)
{
	int i;
	char **w_set;
	w_set = malloc(80 * sizeof(char*));
	packWSet(msg, w_set);
//	for (i = 0; i < 80; i++)
//	{
//		printf("%d : ", i);
//		printBytes(w_set[i], 4, 4);
//	}
	char *A, *B, *C, *D, *E;
	A = (char *) malloc(4); B = (char *) malloc(4); C = (char *) malloc(4); D = (char *)
	 malloc(4); E = (char *) malloc(4);
	
	unsigned int intA, intB, intC, intD, intE;	
	intA = charArrayToInt(hash); intB = charArrayToInt(hash + 4); intC = charArrayToInt(hash + 8);
	intD = charArrayToInt(hash + 12); intE = charArrayToInt(hash + 16);
//	printf("%u\n%u\n%u\n%u\n%u\n", intA, intB, intC, intD, intE);
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
//	printBytes(hash, 4, 4);
//	printBytes(hash + 4, 4, 4);
//	printBytes(hash + 8, 4, 4);
//	printBytes(hash + 12, 4, 4);
//	printBytes(hash + 16, 4, 4);
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
//	printf("compression results\n");
//	printf("\tA: %u\n", intA);
//	printf("\tB: %u\n", intB);
//	printf("\tC: %u\n", intC);
//	printf("\tD: %u\n", intD);
//	printf("\tE: %u\n", intE); 
//	printBytes(A, 4, 4);
//	printBytes(B, 4, 4);
//	printBytes(C, 4, 4);
//	printBytes(D, 4, 4);
//	printBytes(E, 4, 4);
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

uint addMod(uint var1, uint var2)
{
	long long fin = var1 + var2;
	fin = fin % 4294967296; //mod 2^32
	return (uint) fin;	

}
