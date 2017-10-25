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
/*assert*/
#include <assert.h>

int main() {
	printf("Running tests...\n\n");

	/* 1. See if we can print 4 times using parallel for. */
	printf("Using parallel for...\n");
	#pragma omp parallel for
	for (uint8_t threadid = 0; threadid < 4; ++threadid) {
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

	/* 3. Add into one integer in a critical. */
	printf("\nAdding into one variable with critical sections...\n");
	uint8_t shared = 0;
	#define N_3 8
	#pragma omp parallel for
	for (uint8_t threadid = 0; threadid < N_3; ++threadid) {
		#pragma omp critical
		++shared;
	}
	printf("Each thread combined incremented \"shared\" to %d.\n", shared);
	assert(shared == N_3);

	/* 4. Have the master thread stop the other threads. */
	#define N_4 4
	#define GOAL_4 256
	#define MET 1
	uint8_t met = 0;
	uint8_t cnt = 0;

	#pragma omp parallel for
	for (int threadid = 0; threadid < N_4; ++threadid) {
		#pragma omp critical
		while (met != MET) {
			++cnt;
		}
		#pragma omp master
		if (cnt == GOAL_4)
			met = met;
	}
	assert(cnt == GOAL_4);
}


