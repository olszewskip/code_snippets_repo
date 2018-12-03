#include <stdio.h>
#include <omp.h>

int main()
{
	int k;
	int n;

	#pragma omp parallel private(k) shared(n) default(none)
	{
		k = omp_get_thread_num();
		n = omp_get_num_threads();

		printf("Jestem watkiem %d z %d\n", k, n);
	}
	
	return 0;
}