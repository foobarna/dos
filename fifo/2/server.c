// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"

int main() {
	HANDLE f1, f2,file;
	char s[128], path[256], c;
	int alfabet[26] = {0};
	DWORD x;
	int cont = 0;

	printf("I am the server! \n");

	// creating pipes
	f1=CreateNamedPipe(TEXT("\\\\.\\PIPE\\m1"), PIPE_ACCESS_INBOUND,PIPE_TYPE_BYTE|PIPE_WAIT, 3, 0, 0, 0, NULL);
	f2=CreateNamedPipe(TEXT("\\\\.\\PIPE\\m2"), PIPE_ACCESS_OUTBOUND,PIPE_TYPE_BYTE|PIPE_WAIT, 3, 0, 0, 0, NULL);
	ConnectNamedPipe(f1, NULL);
	ConnectNamedPipe(f2, NULL);
	
	// reading from pipe
	x = 0;
	if (ReadFile(f1, s, sizeof(s), &x, NULL)==0) {
		printf("reading error.. %d\n", x);
	}
	printf("read from pipe: %s\n", s);

	strcpy(path,"C:\\Users\\BOG\\Desktop\\fifo\\");
	strcat(path,s);

	file = CreateFile(path,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) { 
		printf("Error\n");
		alfabet[0] = -1;
		if (WriteFile(f2, alfabet, sizeof(alfabet), &x, NULL)==0) {
			printf("writing error..%d\n", x);
		}
		DisconnectNamedPipe(f1);
		DisconnectNamedPipe(f2);
		CloseHandle(f1);
		CloseHandle(f2);
		CloseHandle(file);
		return 0;
	}

	while (ReadFile(file, &c, sizeof(c), &x, NULL)) {
		if (x == 0) break;
		cont = c - 97;
		// printf("%c",c );			//prints out the content of file
		if ((cont >= 0) && (cont < 26) ) alfabet[cont]++;
	}
	
	if (WriteFile(f2, alfabet, sizeof(alfabet), &x, NULL)==0) {
		printf("writing error..%d\n", x);
	}

	// close pipes
	DisconnectNamedPipe(f1);
	DisconnectNamedPipe(f2);
	CloseHandle(f1);
	CloseHandle(f2);
	CloseHandle(file);
}