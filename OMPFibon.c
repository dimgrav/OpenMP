// Fibonacci sequence in specified range
// Serial Vs. Parallel time
// Author: Dimitrios Gravanis, 2017

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int Fibon(int n);

int main(int argc, char** argv)
{
	double start, starts, stop, stops;	// timers
	int *F, *f; 				// Fib arrays
	int N;					// fibon limit
	int i;					// iterator

	printf("\nInsert Fibonacci sequence limit: ");
	scanf("%d", &N);

	F = malloc(N * sizeof(int));
	F[0] = 1;
	F[1] = 1;

	f = malloc(N * sizeof(int));
	f[0] = 1;
	f[1] = 1;

	// parallel
	start = omp_get_wtime();
	# pragma omp parallel for schedule(dynamic, 2)
	for (i = 2; i < N; i++)
	{
		F[i] = Fibon(i);
	}
	stop = omp_get_wtime();

	// serial
	starts = omp_get_wtime();
	printf("\n\nThe Fibonacci numbers up to %d are:\n", N);
	for (i = 2; i < N; i++)
	{
		f[i] = Fibon(i);
		printf("%5d", f[i]);
	}
	stops = omp_get_wtime();

	// time results
	printf("\n\n\nRun time parallel: %.6fs\n", stop - start);
	printf("Run time serial: %.6fs\n\n", stops - starts);
	return 0;
}

// recursive Fibonacci function
int Fibon(int n)
{
	if (n < 2)
	{
		return 1;
	} else
	{
		return Fibon(n - 1) + Fibon(n - 2);
	}
}
