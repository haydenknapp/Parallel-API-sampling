/* Hayden Knapp
 *
 * Experiment with OpenACC
 *
 * gcc -g -O3 test_openacc.c -o test_openacc
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "timer.h"

/* Amount of elements. */
#define N 1024 * 64
/* Amount of instructions per element. */
#define INST_PER 1024 * 128

/* Fill each index with its index. (arr[i] = i). */
void fill(uint32_t *arr, uint32_t n) {
	for (uint32_t i = 0; i < n; ++i)
		arr[i] = i;
}

#if defined SERIAL

/* Write a serial section first. */
/* Multiple by itself, divide by itself, add by itself,
 *  subtract by itself. times times. */
void stress(uint32_t *arr, uint32_t n, uint32_t times) {
	for (int i = 0; i < n; ++i) {
		for (int t = 0; t < times; ++t) {
			if (i != 0) {
				arr[i] *= i;
				arr[i] /= i;
				arr[i] -= i;
				arr[i] += i;
			}
		}
	}
}

#elif defined ACC

/* The same function as above, but with OpenACC acelration. */
void stress(uint32_t *arr, uint32_t n, uint32_t times) {
	#pragma acc kernels
	for (int i = 0; i < n; ++i) {
		for (int t = 0; t < times; ++t) {
			if (i != 0) {
				arr[i] *= i;
				arr[i] /= i;
				arr[i] -= i;
				arr[i] += i;
			}
		}
	}
}

#endif

/* Check to see that value is still equal to index. */
void check(uint32_t *arr, uint32_t n) {
	for (uint32_t i = 0; i < n; ++i)
		assert(arr[i] == i);
}

float elapsed;

int main() {
	uint32_t *arr = (uint32_t*) malloc(N * sizeof(uint32_t));
	fill(arr, N);
	check(arr, N);

	timer_start();
	stress(arr, N, INST_PER);
	timer_stop(elapsed);
	printf("Took %f seconds using serial.\n", elapsed);

	check(arr, N);
	free(arr);
}
