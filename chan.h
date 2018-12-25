#include <semaphore.h>

/** Synchronous channel */
struct schan {
	void *box;
	sem_t data;
	sem_t space;
	pthread_mutex_t mut;
};

int schan_init(struct schan *p);
int schan_destroy(struct schan *p);
int schan_put(struct schan *p, void *msg);
int schan_get(struct schan *p, void **res);

/** Asynchronous channel */
struct achan {
	void **box;
	size_t cap;
	size_t front;
	size_t back;
	sem_t data;
	sem_t space;
	pthread_mutex_t mut;
};

int achan_init(struct achan *p, size_t cap);
int achan_destroy(struct achan *p);
int achan_put(struct achan *p, void *msg);
int achan_get(struct achan *p, void **res);
