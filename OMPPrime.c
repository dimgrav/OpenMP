// Prime number finder in specified range
// Serial Vs. Parallel
// Author: Dimitrios Gravanis, 2017

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{
	int N;					// prime number sequence limit
	int i, j;				// iterators
	int sflag, pflag;			// flags
	int primep = 0, primes = 0;		// sum of primes
	double starts, startp, stops, stopp;	// timers

	printf("\nInsert prime number sequence limit: ");
	scanf("%d", &N);

	// serial
	starts = omp_get_wtime();
	for (i = 2; i <= N; i++)
		{
			sflag = 0;
			for (j = 2; j <= i / 2; j++)
			{
				if(i % j == 0)
				{
					sflag = 1;
					break;
				}				
			}
			if(sflag == 0)
			{
				primes++;
			}

		}
	stops = omp_get_wtime();

	// parallel
	startp = omp_get_wtime();
	# pragma omp parallel for private(pflag, j) schedule(dynamic) reduction(+:primep)
		for (i = 2; i <= N; i++)
		{
			pflag = 0;
			for (j = 2; j <= i / 2; j++)
			{
				if(i % j == 0)
				{
					pflag = 1;
					break;
				}
			}
			if(pflag == 0)
			{
				primep++;
			}
		}
	stopp = omp_get_wtime();

	// print results
	printf("\n\nNumber of primes in range 2-%d: %d\n", N, primes);
	printf("\nRun time parallel: %.6fs\n", stopp - startp);
	printf("Run time serial: %.6fs\n\n", stops - starts);
	return 0;
}
