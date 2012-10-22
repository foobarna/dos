#include <Windows.h>
#include <stdio.h>
#include <conio.h>

HANDLE hFile;
LPINT hView, aux;     

void CreateSharedMemoryArea()
{
	hFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 2 * 4, "SharedMemoryFN");
	if (hFile == NULL)
	{
		 printf("Unable to create the shared memory file.");
		 exit(1);
	}

	hView = (LPINT) MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (hView == NULL)
	{
		 printf("Unable to map the memory segment file to process memory.");
		 exit(1);
	}
	printf("Shared memory has been successfully created....\n");

	aux = hView;
}    

void OpenSharedMemory()
{
	hFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "SharedMemoryFN");            
	if (hFile == NULL)
	{
		 printf("Unable to open the shared memory area.\n");
		 exit(1);
	}

	hView = (LPINT) MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (hView == NULL)
	{
		 printf("Unable to map the memory segment file to process memory.");
		 exit(1);
	}
	printf("Shared memory has been successfully opened....\n");

	aux = hView;
}

void DestroySharedMemoryArea ()
{
	if (!UnmapViewOfFile(hView)) 
	{ 
       printf("Could not unmap memory segment from process memory."); 
	} 
	
	CloseHandle(hFile); 
    
	printf("Shared memory has been successfully destroyed....\n");
}

void ReadGame(int m[3][3]) {
	int i,j, cont = 1;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			m[i][j] = aux[cont++];
}

void WriteGame(int m[3][3]) {
	int i,j, cont = 1;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			aux[cont++] = m[i][j];
}

void writeNo(int a) {
	aux[1] = a;
 }

 void readNo(int *a) {
 	*a=aux[1];
 }
