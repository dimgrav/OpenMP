// OpenMP array average
// Author: Dimitris Gravanis, 2017

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char** argv)
{
	int *A; 			// array pointer
	int N, i; 			// array size, iterator
	int sum = 0, counter = 0;
	float avg; 			// avg array value
	double start, stop; 		// timers
	
	printf("\nInsert N size of A: ");
	scanf("%d", &N);
	printf("\n");

	A = malloc(N * sizeof(int));
	for (i = 0; i < N; i++)
	{
		A[i] = rand()%10;
		printf("%4d", A[i]);
	}
	
	start = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp for reduction(+:sum)
		for (i = 0; i < N; i++)
		{
			sum += A[i];
		}
		
		#pragma omp single
		{
			avg = (float) sum / N;
		}

		#pragma omp for reduction(+:counter) nowait
		for (i = 0; i < N; i++)
		{
			if (A[i] > avg)
			{
				counter++;
			}
		}

	}
	stop = omp_get_wtime();	
	
	printf("\n\narray avg = %.2f\n", avg);
	printf("\nhigher than avg: %2d\n", counter);
	printf("\nrun time: %.6fs\n\n", stop - start);

	return 0;
}














