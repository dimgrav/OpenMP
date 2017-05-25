// Array search using OpenMP, MPI collective communication
// Author: Dimitris Gravanis, 2017

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int *A, *L;				// global, local array pointer
	int N, part;			// global, local array size	
	int x;					// search value
	int lcount, tcount = 0; // times x is found locally, globally
	int i; 					// iterator

	int id, P; 			// MPI rank, size
	double start, stop; // timer start, stop

	// MPI start
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	if (id == 0)
	{
		printf("\nInsert N, size of A: ");
		scanf("%d", &N);
		if(N == 0 || N%P != 0)
		{
			printf("\nInvalid size!");
			printf("\n*Terminated*\n\n");
			return -1;
		}

		// Create array A
		A = malloc(N * sizeof(int));
		for (i = 0; i < N; i++)
		{
			A[i] = rand()%100;
		}

		// Enter search value
		printf("\nInsert value to search for (range 0-100): ");
		scanf("%d", &x);
		while(x < 0 || x > 100) {
			printf("\nOut of bounds!");
			printf("\nInsert value to search for (range 0-100): ");
			scanf("%d", &x);
		}

		part = N/P;
	}
	MPI_Bcast(&part, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Create local array L
	L = malloc(part * sizeof(int));
	MPI_Scatter(&A[0], part, MPI_INT, &L[0], part, MPI_INT, 0, MPI_COMM_WORLD);

	if (id == 0)
	{
		start = MPI_Wtime();
	}

	lcount = 0;
	# pragma omp parallel for private(i) shared(part) reduction(+:lcount)
	for (i = 0; i < part; i++)
	{
		if (L[i] == x)
		{
			lcount++;
		}
	}

	MPI_Reduce(&lcount, &tcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (id == 0)
	{
		stop = MPI_Wtime();
		printf("\n\nSearch finished!");
		printf("\n\nFound %d in array %d times in total.", x, tcount);
		printf("\n\nTotal run time: %.6fs\n", stop - start);
		
		// free global memory
		free(A);
	}
	
	// free local memory
	free(L);
	
	MPI_Finalize();
	return 0;
}
