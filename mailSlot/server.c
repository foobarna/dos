// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"
#include "malesluts.h"

int main() {
	HANDLE c2s, s2c;
	DWORD x, cbMessage, cMessage, cbRead, dwRet;
	int status = 0,row,column;
	int m[3][3];
	start_game(m);
	printf("Waiting for a client to connect...\n");

	// creating mailslots
	// msg2 = CreateMailslot(TEXT("\\\\.\\mailslot\\c2s"), 0, MAILSLOT_WAIT_FOREVER, NULL);
	c2s = CreateMaleSlut(TEXT("\\\\.\\mailslot\\c2s"));
	// msg2 = CreateFile(TEXT("\\\\.\\PIPE\\c2s"), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NMPWAIT_WAIT_FOREVER, NULL);
	s2c = ConnectMaleSlut(TEXT("\\\\.\\mailslot\\s2c"));

	printf("I am the server, marked with '1'\n");

	while (1) {
		//reads from client
		printf("Waiting for client's move...\n");
		if (ReadFile(c2s, m, sizeof(m), &x, NULL)==0) {
			printf("reading error.. %d\n", x);
		}
		print_game(m);
		// chekcs for win
		status = win(m);
		if (status) break;
		// make the move
		printf("Give the row:");
		scanf("%d",&row);
		printf("Give the column: ");
		scanf("%d",&column);
		mark(m,row,column,1);
		// writes to client
		if (WriteFile(s2c, m, sizeof(m), &x, NULL)==0) {
			printf("writing error..%d\n", x);
		}
		print_game(m);
		// checks again for win
		status = win(m);
		if (status) break;
	}

	if (status == 1) printf("\nCongratulions! You won! \n");
		else if (status == 2) printf("\nThe client won! \n");
			else printf("It's a tie!\n");

	// closing connections
	// DisconnectNamedPipe(c2s);
	// DisconnectNamedPipe(s2c);
	// closing pipes
	CloseHandle(c2s);
	CloseHandle(s2c);

}
