/* Hayden Knapp
 *
 * Test AVX and possible make a benchmark comparing AVX code
 * to non AVX code.
 *
 * gcc -mavx avx.c -o avx
 *
 */

/* for tests */
#include <assert.h>
/* standard ints */
#include <stdint.h>
/* avx */
#include <immintrin.h>
/* test if indidual floats are equal */
#define TOLERANCE 1024.0f
static inline uint8_t f_eq(float one, float two) {
	return (uint32_t)(one * TOLERANCE) == (uint32_t)(two * TOLERANCE);
}

/* compare two float arrays. use assertions. */
static void test_eq(float *one, float *two, uint64_t n) {
	for (uint64_t i = 0; i < n; ++i)
		assert(f_eq(one[i], two[i]));
}




int main() {	
	float one[] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 };
	float two[] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8 };
	float *thr;
	float res[] = { 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6 };
	__m256 _one;
	__m256 _two;
	__m256 _thr;
	_one = _mm256_loadu_ps(one);
	_two = _mm256_loadu_ps(two);
	_thr = _mm256_add_ps(_one, _two);
	test_eq(res, (float*)&_thr, 8);
}
