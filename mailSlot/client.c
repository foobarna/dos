// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"
#include "malesluts.h"

int main() {
	HANDLE s2c, c2s;
	DWORD x;
	int status = 0,row,column;
	int m[3][3];

	// s2c = CreateMaleSlut(TEXT("\\\\.\\mailslot\\c2s"));
	c2s = ConnectMaleSlut(TEXT("\\\\.\\mailslot\\c2s"));
	s2c = CreateMaleSlut(TEXT("\\\\.\\mailslot\\s2c"));
	printf("I am the client, marked with '2' \n");

	
	// connecting to pipes created by server
	// msg1 = CreateFile(TEXT("\\\\.\\PIPE\\c2s"), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, NMPWAIT_WAIT_FOREVER, NULL);
	// msg2 = CreateFile(TEXT("\\\\.\\PIPE\\s2c"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NMPWAIT_WAIT_FOREVER, NULL);
	
	start_game(m);

	while (1) {
		// make the move
		printf("Give the row:");
		scanf("%d",&row);
		printf("Give the column: ");
		scanf("%d",&column);
		mark(m,row,column,2);
		// writes to server
		if (WriteFile(c2s, m, sizeof(m), &x, NULL)==0) {
			printf("writing error..%d\n", x);
		}
		print_game(m);
		// checks for win
		status = win(m);
		if (status) break;

		printf("Waiting for server's move...\n");
		// writes to client
		if (ReadFile(s2c, m, sizeof(m), &x, NULL)==0) {
			printf("reading error.. %d\n", x);
		}
		print_game(m);
		// checks for win again
		status = win(m);
		if (status) break;
	}

	if (status == 2) printf("\nCongratulions! You won! \n");
		else if (status == 1) printf("\nThe server won! \n");
			else printf("It's a tie!\n");

	// closing pipes
	CloseHandle(s2c);
	CloseHandle(c2s);
}