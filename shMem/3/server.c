// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "SharedMemoryLib.c"

int main() {
	HANDLE sem;
	DWORD x;
	int no = 1, i;
	
	//creates the area to be shared
	printf("Creating shared memory area\n");
	CreateSharedMemoryArea();

	//creating the semaphore
    sem = CreateSemaphore(
		NULL,					// security attributes
		0,						// initial count
		1,						// maximum count
		"semaphore");			// name of the semaphore

	// waiting for a client to connect
	printf("Waiting for a client to connect...\n");
	WaitForSingleObject( 
		sem,					// handle to semaphore
		INFINITE);				// infinite time-out interval

	printf("Starting in...");
	for (i = 3; i > 0; i--) {
		printf(" %d.",i);
		Sleep(300);
		printf(".");
		Sleep(300);
		printf(".");
		Sleep(400);
	}

	while (no < 10) {
		writeNo(no);
		printf("\n%d..",no);
		Sleep(1000);
		ReleaseSemaphore(sem, 1, NULL);
		WaitForSingleObject(sem, INFINITE);
		readNo(&no);
		no++;
	}

	printf("\nDestroying memory area\n");
	DestroySharedMemoryArea ();
	CloseHandle(sem);
	printf("Quiting\n");
	system("pause");
	return 0;
}