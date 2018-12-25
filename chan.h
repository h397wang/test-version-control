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

int schan_init(struct schan *p);
int schan_destroy(struct schan *p);
int schan_put(struct schan *p, void *msg);
int schan_get(struct schan *p, void **res);
