#include "sha.h"
#define uint unsigned int	

uint K_0 = 0x5A827999;
uint K_1 = 0x6ED9EBA1;
uint K_2 = 0x8F1BBCDC;
uint K_3 = 0xCA62C1D6;

// TODO handle newlines in the middle of the text
/**
	Driver function. This reads the text in the file sha_input.txt and calls the functions that perform the different parts of the algorithm
	at the end it prints out the generated hash
*/

int main()
{
	int i;
	FILE *fp;
	char *fname = "sha_input.txt";
	char *buf;
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
	}	
	printBytes(hash,20,20);
	return 0;	
}

