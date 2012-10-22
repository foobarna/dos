// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"
#include "SharedMemoryLib.c"

int main() {
	DWORD x;
	int status = 0, turn = 2, row, column;
	int m[3][3];

	OpenSharedMemory();

	printf("I am the client, marked with '2' \n");
	WriteOnSharedMemory(turn,20);
	start_game(m);

	while (1) {
		// make the move
		printf("Give the row:");
		scanf("%d",&row);
		printf("Give the column: ");
		scanf("%d",&column);
		mark(m,row,column,2);
		// writes to server
		WriteGame(m);
		turn = 1;
		WriteOnSharedMemory(turn, 20);
		print_game(m);
		// checks for win
		status = win(m);
		if (status) break;
		//reads from server
		printf("Waiting for server's move...\n");
		while (turn != 2) { ReadFromSharedMemory(&turn, 20);}
		ReadGame(m);
		print_game(m);
		// checks for win again
		status = win(m);
		if (status) break;
	}

	if (status == 2) printf("\nCongratulions! You won! \n");
		else if (status == 1) printf("\nThe server won! \n");
			else printf("It's a tie!\n");
	return 0;
}