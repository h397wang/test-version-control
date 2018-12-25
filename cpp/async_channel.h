#include <list>
#include <thread>
#include <cstddef>

#include <pthread.h>
#include <semaphore.h>

using std::list;
using std::thread;
using std::size_t;

template <typename T>
class AsyncChannel
{
public:
	AsyncChannel(size_t cap);
	~AsyncChannel();

	AsyncChannel(const AsyncChannel &) = delete;
	AsyncChannel(const AsyncChannel &&) = delete;
	AsyncChannel& operator=(const AsyncChannel &) = delete;

	T get();
	void put(const T& data);

private:
	list<T> buf;
	pthread_mutex_t bufLock;
	sem_t dataExists;
	sem_t spaceExists;
};

template <typename T>
AsyncChannel<T>::AsyncChannel(size_t cap)
{
	pthread_mutex_init(&bufLock, NULL);
	sem_init(&dataExists, 0, 0);
	sem_init(&spaceExists, 0, cap);
}

template <typename T>
AsyncChannel<T>::~AsyncChannel()
{
	pthread_mutex_destroy(&bufLock);
	sem_destroy(&dataExists);
	sem_destroy(&spaceExists);
}

template <typename T>
T AsyncChannel<T>::get()
{
	sem_wait(&dataExists);
	pthread_mutex_lock(&bufLock);

	T msg = buf.front();
	buf.pop_front();

	pthread_mutex_unlock(&bufLock);
	sem_post(&spaceExists);

	return msg;
}

template <typename T>
void AsyncChannel<T>::put(const T &data)
{
	sem_wait(&spaceExists);
	pthread_mutex_lock(&bufLock);

	buf.emplace_back(data);

	pthread_mutex_unlock(&bufLock);
	sem_post(&dataExists);
}
