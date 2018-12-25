#include "chan.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

void *run_sync_test(void *chan)
{
	int *ptr;
	while (1) {
		schan_get(chan, (void *) &ptr);
		printf("Got: %d\n", *ptr);
	}
}

int sync_chan_test()
{
	struct schan *chan = malloc(sizeof(struct schan));
	schan_init(chan);

	pthread_t tid;
	pthread_create(&tid, NULL, run_sync_test, chan);

	int *msg = malloc(sizeof(int));
	for (int i=0; i<10; ++i) {
		*msg = i;
		schan_put(chan, msg);
	}

	pthread_cancel(tid);
	pthread_join(tid, NULL);

	free(msg);
	schan_destroy(chan);
	free(chan);
}

void *run_async_test(void *chan)
{
	int *ptr;
	bool done = false;

	while (!done) {
		achan_get(chan, (void *) &ptr);
		printf("Got: %d\n", *ptr);
		if (*ptr == 9)
			done = true;
		free(ptr);
	}
}

int async_chan_test()
{
	const int cap = 10;
	struct achan *chan = malloc(sizeof(struct achan));
	achan_init(chan, cap);

	pthread_t tid;
	pthread_create(&tid, NULL, run_async_test, chan);

	for (int i=0; i<cap; ++i) {
		int *msg = malloc(sizeof(int));
		*msg = i;
		printf("Put: %d\n", i);
		achan_put(chan, msg);
	}

	pthread_join(tid, NULL);
	achan_destroy(chan);
	free(chan);
}

int main()
{
	sync_chan_test();
	async_chan_test();
}
