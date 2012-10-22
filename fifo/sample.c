#include <windows.h>
#include <stdio.h>
#include <string.h>

int main() {
	HANDLE f1, f2;
	char s;
	int i;
	DWORD x;
	f1 = CreateFile("C:\\Users\\BOG\\Desktop\\fifo\\sample.txt",GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);


	ReadFile(f1, &s, sizeof(s), &x, NULL);
	i = s - 96;
	printf("%d\n",i );
	
	ReadFile(f1, &s, sizeof(s), &x, NULL);
	printf("%c\n",s );
	
	ReadFile(f1, &s, sizeof(s), &x, NULL);
	printf("%c\n",s );
	
	CloseHandle(f1);

}