// Array search using OpenMP, MPI peer to peer communication
// Author: Dimitris Gravanis, 2017

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "mpi.h"

int main (int argc, char **argv)
{
	int id, P; // MPI rank, size
	
	// MPI start
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	
	if (id == 0)
	{
	  	int *A, N, i;		// array pointer, size, iterator
	  	int *R;			// local array pointer
	  	int part;		// local array size
	  	int x, f = 0;		// search value, counter
	  	double start, stop;	// timer start, stop

	  	printf("\nInsert array size: ");
	  	scanf("%d", &N);
		
		// allocate memory for A
	    A = malloc(N * sizeof(int));
	    if (A == NULL || N == 0) {
			printf("\nInvalid size!");
			printf("\n*Terminated*\n\n");
			return -1;
	    }

        // insert values in A
        for (i = 0; i < N; i++){
	    	A[i] = rand() % 100;
	    }

	    printf("\nInsert value to search for (range 0-100): ");
	    scanf("%d", &x);

	    while (x > 100 || x < 0){
			printf("\nOut of bounds!\n\n");  
			printf("\nInsert value to search for (0-100): ");
	    	scanf("%d", &x);
	    }

	    // timer start
	    start = MPI_Wtime();

	    // allocate memory for R
	    part = N / (P-1);
	    R = malloc((N / part) * sizeof(int));
	    
	    for (i = 1; i < P; i++){
	    	MPI_Send(&part, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	    for (i = 1; i < P; i++){
	    	MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	    for (i = 1; i < P; i++){
 	    	MPI_Send(&A[(N/(P-1))*(i-1)], part, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	    printf("\n");
	    for (i = 1; i < P; i++){
	    	MPI_Recv(&R[i-1], 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	        printf("\nSearching... ");
	        printf("Found %d times in node %d.\n", R[i-1], i);
	        f += R[i-1];
 	    }

	    // timer stop
	    stop = MPI_Wtime();
	    printf("\n\nFinished!");
	    printf("\n\nFound %d times in total.",f);
	    printf("\n\nTotal run time: %.6fs\n\n", stop - start);
	
		// free memory
		free(A);
		free(R);
	
	} else {
		int *Pw;
		int i;
		int partworker, xw;
		int fw = 0;

		MPI_Recv(&partworker, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&xw, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		Pw = malloc(partworker * sizeof(int));
		MPI_Recv(&Pw[0], partworker, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
		# pragma omp parallel for private(i) shared(partworker) reduction (+:fw)	 
		for (i = 0; i < partworker; i++)
			if (Pw[i] == xw) 
	    		fw++;
          		MPI_Send(&fw, 1, MPI_INT, 0, id, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
