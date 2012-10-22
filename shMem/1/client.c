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

	// opening the shared mem
	OpenSharedMemory();

	// opening the semaphore
	sem = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,		// access attributes
		0,							// inherit handle
		"semaphore");				// name of the semaphore

	// releasing the sem and start doing stuff
	ReleaseSemaphore(sem, 1, NULL);
	printf("I am the client, marked with '2' \n");
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
		print_game(m);
		// let the server to move
		ReleaseSemaphore(sem, 1, NULL);
		// checks for win
		status = win(m);
		if (status) break;
		//waiting and reads from server
		printf("Waiting for server's move...\n");
		WaitForSingleObject(sem, INFINITE);
		ReadGame(m);
		print_game(m);
		// checks for win again
		status = win(m);
		if (status) break;
	}

	if (status == 2) printf("\nCongratulions! You won! \n");
		else if (status == 1) printf("\nThe server won! \n");
			else printf("It's a tie!\n");

	CloseHandle(sem);
	return 0;
}