#include <string.h>
#include <intrz80.h>
#include <stdio.h>
char * mylib_font8x8=(void*)0x3c00;
static union {char * w;char b[2];}scrxy;
void scr_init(char a){
	*((char *)0x5800)=a;
	output8(0xfe,a>>3);
	memcpy((void *)0x5801, (void *)0x5800, 32*24-1);
	scrxy.w=(void *)0x4000;
	*((char *)0x4000)=0;
	memcpy((void *)0x4001, (void *)0x4000, (unsigned int)256*192/8-1);
}
int putchar(int ch){
	switch(ch){
		case '\n':	scrxy.b[0]+=32;
		case '\r':	scrxy.b[0]&=0xe0;
			break;
		default:{
				char* s=mylib_font8x8+(ch<<3);
				unsigned char i=8;
				while(i--){
					*scrxy.w=*(s++);
					scrxy.b[1]++;
				}
			}
			scrxy.w-=0x07ff;
			break;
	}
	if(!scrxy.b[0]){
		if((scrxy.b[1]+=8)==0x58) scrxy.b[1]=0x40;
	}
	return 1;
}