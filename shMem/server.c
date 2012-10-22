#include <stdio.h>
#include <Windows.h>
#include "SharedMemoryLib.c"

#include "tictactoe.h"

int main(int argc, char* argv[])
{
	//creates the area to be shared
	printf("Creating shared memory area\n");
	CreateSharedMemoryArea();

	printf("Type 1 to write to a position in the memory area.\n");
	printf("Type 2 to read from a position in the momery area.\n");
	printf("Type 3 or any other key to destroy memory area and quit.\n");

	int todo=0;
	int pos=0,value=0;
	scanf("%i",&todo);
	while ((todo>0)&&(todo<3))
	{
		if (todo==1)
		{
			printf("Input position to write into the memory area\n");
			scanf("%i",&pos);
			while(pos<1)
			{
				printf("Position must be positive\n");
				scanf("%i",&pos);
			}
			printf("Input value to write into the memory area on position %i\n",pos);
			scanf("%i",&value);
			while(value<1)
			{
				printf("Value must be positive\n");
				scanf("%i",&value);
			}
  			WriteOnSharedMemory(value, pos); // performs the writing
			printf("Wrote %d to position %d.\n",value,pos);

			printf("Type 1 to write to a position in the memory area.\n");
			printf("Type 2 to read from a position in the momery area.\n");
			printf("Type 3 or any other key to destroy memory area and quit.\n");
			scanf("%i",&todo);
		}
		if (todo==2)
		{
			printf("Input position to read from the memory area\n");
			scanf("%i",&pos);
			while(pos<1)
			{
				printf("Position must be positive\n");
				scanf("%i",&pos);
			}
			
			ReadFromSharedMemory(&value, pos); // performs the writing
			printf("Read %d from position %d.\n",value,pos);

			printf("Type 1 to write to a position in the memory area.\n");
			printf("Type 2 to read from a position in the momery area.\n");
			printf("Type 3 or any other key to destroy memory area and quit.\n");
			scanf("%i",&todo);
		}
	}

	printf("Destroying memory area\n");
	DestroySharedMemoryArea (); 
	printf("Quiting\n");
	return 0;
}


