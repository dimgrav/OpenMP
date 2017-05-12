#include <stdio.h>
#include <omp.h>

int main (int argc, char** argv)
{
	int P; // synolo diergasiwn
	int myid; // diergasia komvoy

	#pragma omp parallel private(myid)
	{
		myid = omp_get_thread_num();
		printf("Hello World apo th diergasia %d\n", myid);
		if (myid == 0) 
		{	// ektelei mono o master
			P = omp_get_num_threads();
			printf("SYNOLO DIERGASIWN = %d\n", P);
		}
	}
	return 0;
}
