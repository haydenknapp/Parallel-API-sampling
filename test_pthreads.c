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
