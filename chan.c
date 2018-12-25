#include "chan.h"
#include <pthread.h>

int schan_init(struct schan *p)
{
	if (!p)
		return -1;

	sem_init(&p->sem, 0, 0);
	sem_init(&p->got, 0, 0);
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
