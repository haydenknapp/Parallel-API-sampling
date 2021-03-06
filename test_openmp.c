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
/* tim functions */
#include "timer.h"

/* do n amount of work. */
void do_work(uint64_t i, uint64_t j) {
	for (uint64_t it = 0; it < i * j * 1024 * 8; ++it) {
		it += 10;
		it -= 10;
		it += 10;
		it -= 10;
		it += 10;
		it -= 10;
		it += 10;
		it -= 10;
		it += 10;
		it -= 10;
		it += 10;
		it -= 10;
		it += 10;
		it -= 10;
		it += 10;
		it -= 10;
	}
}

float elapsed;

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
/*	#define N_4 4
	#define GOAL_4 256
	#define MET 1
	uint8_t met = 0;
	uint8_t cnt = 0;

	#pragma omp parallel for
	for (int threadid = 0; threadid < N_4; ++threadid) {
		#pragma omp single
		if (cnt == GOAL_4)
		{
			met = MET;
		}
		#pragma omp critical
		while (met != MET)
		{
			++cnt;
		}
	}
	assert(cnt == GOAL_4);
*/

	/* 5. Spawn a discrete amount of threads. */
	printf("\nUsing num_threads, using omp single to make one print.\n");
	#pragma omp parallel num_threads(2)
	{
		printf("This should print twice.\n");
		#pragma omp single
		printf("This should print once.\n");
	}

	/* 6. Add into one integer in a critical. */
	printf("\nAdding into one variable with atomic sections...\n");
	shared = 0;
	#define N_6 8
	#pragma omp parallel for
	for (uint8_t threadid = 0; threadid < N_6; ++threadid) {
		#pragma omp atomic
			++shared;
	}
	printf("Each thread combined incremented \"shared\" to %d.\n", shared);
	assert(shared == N_6);


	/* 7. Create seperate thread IDs for each using private */
	printf("Seperate threadids via private test.\n");
	uint8_t threadid;
	#define N_7 4
	#pragma omp parallel num_threads(N_7) private(threadid)
	{
		threadid = omp_get_thread_num();
		printf("Hello from thread %d!\n", threadid);
	}

	/* 8. Make a dynamic problem, but use static threads. */
	const uint16_t n_8 = 16;
	uint16_t i, j;
	timer_start();
	#pragma omp for
	for (i = 0; i < n_8; ++i) {
		for (j = 0; j < i; ++j) {
			do_work(i, j);
		}
	}
	timer_stop(elapsed);
	printf("\nElapsed time: %f\n", elapsed);

	/* 8. Make a dynamic problem, and use dynamic threads. */
	timer_start();
	#pragma omp for schedule(dynamic, 5) private(i, j)
	for (i = 0; i < n_8; ++i) {
		for (j = 0; j < i; ++j) {
			do_work(i, j);
		}
	}
	timer_stop(elapsed);
	printf("\nElapsed time: %f\n", elapsed);
}
