#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main (int argc, char** argv)
{
	int *A, *B, *C; 	// array pointers
	int N; 			// size of arrays
	int i; 			// iterator
	int id, P; 		// task id, num of tasks
	double start, stop; 	// timers
	
	#pragma omp parallel private(id)
	{
		id = omp_get_thread_num();
		if (id == 0)
		{
			printf("\nInsert N, size of A and B arrays: ");
			scanf("%d", &N);
		}
	}
	
	A = malloc(N * sizeof(int));
	B = malloc(N * sizeof(int));
	C = malloc(N * sizeof(int));

	for (i = 0; i <N; i++)
	{
		A[i] = rand()%10;
		B[i] = rand()%10;
	}
	
	start = omp_get_wtime();
	#pragma omp parallel for private(id)
		
		for (i = 0; i < N; i++)
		{
			id = omp_get_thread_num();
			printf("\ntask: %d, i: %d", id, i);
			C[i] = A[i] + B[i];
		}
	stop = omp_get_wtime();

	printf("\n\nA:\n");	
	for (i = 0; i < N; i++)
	{
		printf("%4d", A[i]);
	}
	printf("\n");
	printf("\nB:\n");	
	for (i = 0; i < N; i++)
	{
		printf("%4d", B[i]);
	}
	printf("\n");
	printf("\nC:\n");	
	for (i = 0; i < N; i++)
	{
		printf("%4d", C[i]);
	}
	printf("\n\nRun time: %.6fs", stop - start);
	printf("\n\n");
	return 0;
}
