#include "chan.h"

#include <stdlib.h>
#include <pthread.h>

int schan_init(struct schan *p)
{
	if (!p)
		return -1;

	sem_init(&p->data, 0, 0);
	sem_init(&p->space, 0, 0);
	pthread_mutex_init(&p->mut, NULL);

	return 0;
}

int schan_destroy(struct schan *p)
{
	if (!p)
		return -1;

	sem_destroy(&p->data);
	sem_destroy(&p->space);
	pthread_mutex_destroy(&p->mut);

	return 0;
}

int schan_put(struct schan *p, void *msg)
{
	if (!p)
		return -1;

	pthread_mutex_lock(&p->mut);
	p->box = msg;	
	pthread_mutex_unlock(&p->mut);
	sem_post(&p->data);
	sem_wait(&p->space);

	return 0;
}

int schan_get(struct schan *p, void **res)
{
	if (!p || !res)
		return -1;

	sem_wait(&p->data);
	pthread_mutex_lock(&p->mut);
	*res = p->box;
	pthread_mutex_unlock(&p->mut);
	sem_post(&p->space);

	return 0;
}

int achan_init(struct achan *p, size_t cap)
{
	if (!p || !cap)
		return -1;

	sem_init(&p->data, 0, 0);
	sem_init(&p->space, 0, cap);
	pthread_mutex_init(&p->mut, NULL);
	p->box = malloc(cap * sizeof(void *));
	p->cap = cap;
	p->front = 0;
	p->back = 0;

	return 0;
}

int achan_destroy(struct achan *p)
{
	if (!p)
		return -1;

	sem_destroy(&p->data);
	sem_destroy(&p->space);
	pthread_mutex_destroy(&p->mut);
	free(p->box);

	return 0;
}

int achan_put(struct achan *p, void *msg)
{
	if (!p)
		return -1;

	sem_wait(&p->space);
	pthread_mutex_lock(&p->mut);

	/** Add to back */
	p->box[p->back] = msg;
	p->back = (p->back + 1) % p->cap;

	pthread_mutex_unlock(&p->mut);
	sem_post(&p->data);

	return 0;
}

int achan_get(struct achan *p, void **res)
{
	if (!p || !res)
		return -1;

	sem_wait(&p->data);
	pthread_mutex_lock(&p->mut);

	/** Remove from front */
	*res = p->box[p->front];
	p->front = (p->front + 1) % p->cap;

	pthread_mutex_unlock(&p->mut);
	sem_post(&p->space);

	return 0;
}
