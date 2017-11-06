/* Hayden Knapp
 *
 * Do some comparisons of parallel algorithms between pthreads
 * and openmp
 *
 * gcc openmp_vs_pthreads.c -g -pthread -fopenmp -o comparison
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include <pthread.h>
#include <omp.h>

#include "timer.h"

/* use this macro so the functions are not optimized out.
 * A is the list and B is the length.
 */
#define print_rand(A, B) {\
	uint64_t index = rand() % B;\
	printf("Random: %lu\n", A[index]);\
}

/* assert that each element is equal to its index. */
void check(uint64_t *data, uint64_t n) {
	for (uint64_t i = 0; i < n; ++i)
		if (data[i] != i) {
			printf("Not equal at %lu\n", i);
			assert(data[i] == i);
		}
}

#define LOOPS 1024
#define N (1024 * 1024 * 16)

void do_work(uint64_t *data, uint64_t n, uint16_t loops) {
	for (uint64_t out = 1; out < n; ++out) {
		data[out] = out;
		for (uint16_t in = 0; in < loops; ++in) {
			--data[out];	
			data[out] *= 2;	
			data[out] /= 2;	
			++data[out];	
		}
	}
}


int main() {
	/* used for later */
	float elapsed;
	srand(time(NULL));

	uint64_t *data = (uint64_t*) malloc(N * sizeof(uint64_t));
	timer_start();
	do_work(data, N, LOOPS);
	timer_stop(elapsed);
	printf("Single threaded took %.5f\n", elapsed);
	print_rand(data, N);
	check(data, N);
	free(data);
}
