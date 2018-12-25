#include "chan.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *run_receiver(void *chan)
{
	int *ptr;
	while (1) {
		schan_get(chan, (void *) &ptr);
		printf("Got: %d\n", *ptr);
	}
}

int main()
{
	struct schan *chan = malloc(sizeof(struct schan));
	pthread_t tid;
	pthread_create(&tid, NULL, run_receiver, chan);

	int *msg = malloc(sizeof(int));
	for (int i=0; i<10; ++i) {
		*msg = i;
		schan_put(chan, msg);
	}

	/** Todo: How to clean up semaphores after cancel */
	pthread_cancel(tid);
	pthread_join(tid, NULL);

	free(msg);
	free(chan);
}
