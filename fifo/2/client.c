// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"

int main() {
	HANDLE f1, f2;
	char s[128];
	int alfabet[26], i;
	DWORD x; 

	printf("I am the client \n");

	// connect to pipes, created by server
	f1=CreateFile(TEXT("\\\\.\\PIPE\\m1"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	f2=CreateFile(TEXT("\\\\.\\PIPE\\m2"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	
	// send message through pipe
	printf("Give the filename: ");
	scanf("%s", &s);
	if (WriteFile(f1, s, strlen(s)+1, &x, NULL)==0) {
		printf("writing error..%d\n", x);
	}

	// receive message through pipe
	if (ReadFile(f2, alfabet, sizeof(alfabet), &x, NULL)==0) {
		printf("reading error..%d\n", x);
	}

	if (alfabet[0] == -1) {
		printf("Eroare\n");
		CloseHandle(f1);
		CloseHandle(f2);
		return 0;
	}

	printf("The count of chars:\n");
	for (i = 0; i < 26; i++) {
		printf("the char %c is counted %d times\n",i+97,alfabet[i]);
	}

	// close connections
	CloseHandle(f1);
	CloseHandle(f2);
}