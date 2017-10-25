/* Hayden Knapp
 * In this file I will bes testing some of the pragmas that OpenMP has to offer,
 * to make sure I can get the most out of the API when I use it in the future.
 *
 * compile with: gcc test_openmp.c -Wall -fopenmp -o test_openmp
 */

/* printf */
#include <stdio.h>
/* malloc, free */
#include <stdlib.h>
/* openmp */
#include <omp.h>
/* standard ints */
#include <stdint.h>

int main() {
	printf("Running tests...\n\n");

	/* 1. See if we can print 4 times using parallel for. */
	printf("Using parallel for...\n");
	#pragma omp parallel for
	for (uint8_t threadid = 0; threadid < 8; ++threadid) {
		printf("Hello world from thread #%d\n", threadid);
	}

	/* 2. See if we can print 4 times using parallel sections. */
	printf("\nUsing parallel sections...\n");
	#pragma omp parallel sections
	{
		#pragma omp section
		printf("Hello world from thread #1\n");
		#pragma omp section
		printf("Hello world from thread #2\n");
		#pragma omp section
		printf("Hello world from thread #3\n");
		#pragma omp section
		printf("Hello world from thread #4\n");
	}
}
