// #include "StdAfx.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "SharedMemoryLib.c"

int main() {
	HANDLE sem;
	DWORD x;
	int no = 0, i;

	// opening the shared mem
	OpenSharedMemory();

	// opening the semaphore
	sem = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,		// access attributes
		0,							// inherit handle
		"semaphore");				// name of the semaphore

	// releasing the sem and start doing stuff
	ReleaseSemaphore(sem, 1, NULL);

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
		WaitForSingleObject(sem, INFINITE);
		readNo(&no);
		no++;
		writeNo(no);
		printf("\n%d..",no);
		Sleep(1000);
		ReleaseSemaphore(sem, 1, NULL);
	}
	printf("\n");
	CloseHandle(sem);
	system("pause");
	return 0;
}