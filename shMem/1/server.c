// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "tictactoe.h"
#include "SharedMemoryLib.c"

int main() {
	HANDLE sem;
	DWORD x;
	int status = 0, row, column;
	int m[3][3];
	
	//creates the area to be shared
	printf("Creating shared memory area\n");
	CreateSharedMemoryArea();
	WriteOnSharedMemory(0,20);

	//creating the semaphore
    sem = CreateSemaphore(
		NULL,					// security attributes
		0,						// initial count
		1,						// maximum count
		"semaphore");			// name of the semaphore

	start_game(m);

	// waiting for a client to connect
	printf("Waiting for a client to connect...\n");
	WaitForSingleObject( 
		sem,					// handle to semaphore
		INFINITE);				// infinite time-out interval

	printf("I am the server, marked with '1'\n");

	while (1) {
		// wait for client's move
		printf("Waiting for client's move...\n");
		WaitForSingleObject(sem, INFINITE);
		// read game
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
		// let the client to move
		ReleaseSemaphore(sem, 1, NULL);
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
	CloseHandle(sem);
	printf("Quiting\n");
	return 0;
}