#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/**
 * Synchronous channel.
 */
struct schan {
	void *box;
	sem_t sem;
	sem_t got;
	pthread_mutex_t mut;
};

int schan_init(struct schan *p)
{
	if (!p)
		return -1;

	sem_init(&p->sem, 0, 0);
	sem_destroy(&p->got);
	pthread_mutex_init(&p->mut, NULL);
	p->box = NULL;

	return 0;
}

int schan_destroy(struct schan *p)
{
	if (!p)
		return -1;

	sem_destroy(&p->sem);
	sem_destroy(&p->got);
	pthread_mutex_destroy(&p->mut);
	p->box = NULL;

	return 0;
}

int schan_put(struct schan *p, void *msg)
{
	if (!p)
		return -1;

	pthread_mutex_lock(&p->mut);
	p->box = msg;
	pthread_mutex_unlock(&p->mut);
	sem_post(&p->sem);
	sem_wait(&p->got);

	return 0;
}

int schan_get(struct schan *p, void **res)
{
	if (!p || !res)
		return -1;

	sem_wait(&p->sem);
	pthread_mutex_lock(&p->mut);
	*res = p->box;
	pthread_mutex_unlock(&p->mut);
	sem_post(&p->got);

	return 0;
}

void *run_receiver(void *chan)
{
	void *ptr;
	schan_get(chan, &ptr);
	printf("Got: %d\n", *((int *) ptr));
}

int main()
{
	struct schan *chan = malloc(sizeof(struct schan));
	pthread_t tid;
	pthread_create(&tid, NULL, run_receiver, chan);

	int *msg = malloc(sizeof(int));
	*msg = 1000;
	schan_put(chan, msg);
	pthread_join(tid, NULL);

	free(msg);
	free(chan);
}
