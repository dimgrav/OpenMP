// Array search using OpenMP, MPI peer to peer communication
// Author: Dimitris Gravanis, 2017

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	/* MPI vars */
	int id, P; // rank, size
	double start, stop; // timer

	/* vars */
	int *G, *L, *R, N, part, i; // arrays
	int x, lcount=0, tcount=0; // search item, counters

	/* MPI start */
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	/* master input */
	if (id == 0)
	{
		printf("\nInsert array size: ");
		scanf("%d", &N);

		while (N == 0 || N%(P-1) != 0) {
			printf("\nInvalid size!");
			printf("\nInsert array size: ");
			scanf("%d", &N);
		}

		printf("\nInsert search value in [0,100): ");
		scanf("%d", &x);

		while (x < 0 || x > 100) {
			printf("\nOut of bounds!");
			printf("\nInsert search value in [0,100): ");
			scanf("%d", &x);
		}

		G = malloc(N * sizeof(int));
		for (i = 0; i < N; i++)
			G[i] = rand()%100;
		part = N/(P-1);

		/* timer start */
		start = MPI_Wtime();

		/* result array */
		R = malloc((P-1)*sizeof(int));

		/* send to workers */
		for (i = 1; i < P; i++)
		{
			MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&part, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&G[(i-1)*part-1], part, MPI_INT, i, 0, MPI_COMM_WORLD);
			/* receive from workers */
			MPI_Recv(&R[i-1], 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			tcount += R[i-1];
		}

		/* timer stop */
		stop = MPI_Wtime();

		/* master output */
		printf("\n");
		for (i = 0; i < N; i++)
			printf("%5d", G[i]);

		printf("\n\n%d found %d times in total.", x, tcount);
		printf("\n\nTotal run time = %.8fsec\n\n", stop - start);
	}

	/* worker input by master */
	else
	{
		MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&part, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		L = malloc(part * sizeof(int));
		MPI_Recv(&L[0], part, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		# pragma omp parallel for private(i) shared(part) reduction (+:lcount) 
		for (i = 0; i < part; i++)
		{
			if (L[i] == x)
			{
				lcount++;
			}
		}
		MPI_Send(&lcount, 1, MPI_INT, 0, id, MPI_COMM_WORLD);
		
	}

	/* MPI Stop */
	MPI_Finalize();
	return 0;
}
