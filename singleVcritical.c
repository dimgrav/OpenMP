#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{
	int x = 0, y = 0, z = 0;
	
	#pragma omp parallel
	{
		
		z++;

		#pragma omp critical
		{
			x++;
		}
		
		#pragma omp single
		{
			y++;
		}

	}
	
	printf("\nx = %d, y = %d, z = %d\n\n", x, y, z);
	
	return 0;
}
