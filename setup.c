#include "sha.h"

#define uint unsigned int

/**
*	Takes the buffer holding the raw input string, truncates it to the correct size, and pads it. It returns the full size of the buffer on success
*/
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

	return full_size;	
}

/**
*	Pads the buffer.
*	Takes the truncated buffer, the message size, and the buffer size. Based on this input sets the last 4 bytes as the amount of bits the message is,
*	sets the first byte after the message to 0b10000000, and the fills in the rest of the padding bytes with 0
*/
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

/**
*	initializes the hash to its initial value
*/
char *initHash() {
        char *hash = (char *) malloc(20);
        hash[0]=0x67; hash[1]=0x45; hash[2]=0x23; hash[3]=0x01; hash[4]=0xEF; hash[5]=0xCD; hash[6]=0xAB; hash[7]=0x89; hash[8]=0x98;
        hash[9]=0xBA; hash[10]=0xDC; hash[11]=0xFE; hash[12]=0x10; hash[13]=0x32; hash[14]=0x54; hash[15]=0x76; hash[16]=0xC3;
        hash[17]=0xD2; hash[18]=0xE1; hash[19]=0xF0;

        return hash;
}

