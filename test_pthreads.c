/* Hayden Knapp
 *
 * Test pthreads
 *
 * Attempt to get more use out of pthreads than simply creating,
 * running and destroying them.
 *
 * gcc test_pthreads.c -pthread -o test_pthreads
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <assert.h>

#if defined CRERUNDES

/* Print "threadid: threadid" */
void *thread_write(void *args) {
	uint8_t *threadid = (uint8_t*) args;
	printf("threadid: %d\n", *threadid);
	return NULL;
}

#define NUM_THREADS 4

int main() {
	pthread_t threadids[NUM_THREADS];
	uint8_t threadnum[NUM_THREADS];
	for (int id = 0; id < NUM_THREADS; ++id) {
		threadnum[id] = id;
		pthread_create(&threadids[id], NULL, thread_write, (void*)&threadnum[id]);
	}

	for (int id = 0; id < NUM_THREADS; ++id)
		pthread_join(threadids[id], NULL);

	return 0;
}

#elif MUTEX

uint64_t shared_data;
pthread_mutex_t lock;

#define NUM_THREADS 32
#define TIMES_TO_INC (1024*1024)
/* Increment the shared data by 1 using mutex locks. */
void *increment(void *arg) {
	for (int i = 0; i < TIMES_TO_INC; ++i) {
		pthread_mutex_lock(&lock);
		++shared_data;
		pthread_mutex_unlock(&lock);
	}
}

int main() {
	pthread_mutex_init(&lock, NULL);
	pthread_t ids[NUM_THREADS];
	shared_data = 0;
	for (int i = 0; i < NUM_THREADS; ++i)
		pthread_create(&ids[i], NULL, increment, NULL);

	for (int i = 0; i < NUM_THREADS; ++i)
		pthread_join(ids[i], NULL);
	pthread_mutex_destroy(&lock);
	printf("shared_data == %lu, should be %lu\n", shared_data, NUM_THREADS*TIMES_TO_INC);
	assert(shared_data == NUM_THREADS*TIMES_TO_INC);
}
/* With mutex locking, the process completes correctly without
 * any issues, but is of course much slower. Without the locks,
 * the assertion is almost guaranteed to fail.
 */

#elif defined COND_VAR

int main() {

}

#else

int main() {
	printf("No defines specified!\n");
}

#endif
