// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"
#include "SharedMemoryLib.c"

int main() {
	DWORD x;
	int status = 0,turn = 0, row, column;
	int m[3][3];
	
	//creates the area to be shared
	printf("Creating shared memory area\n");
	CreateSharedMemoryArea();
	WriteOnSharedMemory(0,20);

	start_game(m);
	printf("Waiting for a client to connect...\n");

	while (turn == 0){
		ReadFromSharedMemory(&turn,20);
	}
	printf("I am the server, marked with '1'\n");

	while (1) {
		// reads from client
		printf("Waiting for client's move...\n");
		while(turn != 1) { ReadFromSharedMemory(&turn,20);}
		//read game
		ReadGame(m);
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
		WriteGame(m);
		turn = 2;
		WriteOnSharedMemory(turn,20);
		print_game(m);
		// checks again for win
		status = win(m);
		if (status) break;
	}

	if (status == 1) printf("\nCongratulions! You won! \n");
		else if (status == 2) printf("\nThe client won! \n");
			else printf("It's a tie!\n");
	
	printf("Destroying memory area\n");
	DestroySharedMemoryArea (); 
	printf("Quiting\n");
	return 0;
}