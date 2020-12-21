#include<cstdio>

int main(){
	unsigned long val = 0x135792468a;
	char cbuf[100];
	sprintf(cbuf, "%.8x", val);
	for (int i = 0; *(cbuf+i); ++i){
		printf("%x\t", *(cbuf+i));
	}
	return 0;
}
