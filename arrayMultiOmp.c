// Array multiplication using OpenMP and MPI
// Author: Dimitris Gravanis, 2017

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int *A, *B, *C; // array pointers
	int N; // array size
	int i; // iterator

	/* MPI variables */
	int id, P; // rank, size
	double ta, tt; // timer start, stop

	/* MPI Start */
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	#pragma omp parallel private(id)
	{
		if (id == 0)
		{
			printf("\n\nInsert array size N: ");
			scanf("%d", &N);
		}
	}

	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	A = malloc(N * sizeof(int));
	B = malloc(N * sizeof(int));
	C = malloc(N * sizeof(int));

	for (i = 0; i < N; i++)
	{
		A[i] = rand()%10;
		B[i] = rand()%10;
	}

	#pragma omp parallel for private(i)

		for (i = 0; i < N; i++)
		{
			C[i] = A[i] * B[i];
		}
	
	MPI_Finalize();
	return 0;
}