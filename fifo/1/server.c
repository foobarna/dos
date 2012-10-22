// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"

int main() {
	HANDLE fifo1, fifo2;
	DWORD x;
	int status = 0,row,column;
	int m[3][3];
	
	start_game(m);
	printf("Waiting for a client to connect...\n");

	// creating pipes
	fifo1 = CreateNamedPipe(TEXT("\\\\.\\PIPE\\s2c"), PIPE_ACCESS_INBOUND,PIPE_TYPE_BYTE|PIPE_WAIT, 3, 0, 0, NMPWAIT_WAIT_FOREVER, NULL);
	fifo2 = CreateNamedPipe(TEXT("\\\\.\\PIPE\\c2s"), PIPE_ACCESS_OUTBOUND,PIPE_TYPE_BYTE|PIPE_WAIT, 3, 0, 0, NMPWAIT_WAIT_FOREVER, NULL);
	ConnectNamedPipe(fifo1, NULL);
	ConnectNamedPipe(fifo2, NULL);

	printf("I am the server, marked with '1'\n");

	while (1) {
		// reads from client
		printf("Waiting for client's move...\n");
		if (ReadFile(fifo1, m, sizeof(m), &x, NULL)==0) {
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
		if (WriteFile(fifo2, m, sizeof(m), &x, NULL)==0) {
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
	DisconnectNamedPipe(fifo1);
	DisconnectNamedPipe(fifo2);
	// closing pipes
	CloseHandle(fifo1);
	CloseHandle(fifo2);

}