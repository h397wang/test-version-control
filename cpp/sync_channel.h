#include <list>
#include <thread>
#include <cstddef>
#include <iostream>

#include <pthread.h>
#include <semaphore.h>

using std::cout;
using std::list;
using std::thread;
using std::size_t;

template <typename T>
class SyncChannel
{
public:
	SyncChannel();
	~SyncChannel();

	SyncChannel(const SyncChannel &) = delete;
	SyncChannel(const SyncChannel &&) = delete;
	SyncChannel& operator=(const SyncChannel &) = delete;

	T get();
	void put(const T& data);

private:
	list<T> buf;
	pthread_mutex_t bufLock;
	sem_t dataExists;
	sem_t spaceExists;
};

template <typename T>
SyncChannel<T>::SyncChannel()
{
	pthread_mutex_init(&bufLock, NULL);
	sem_init(&dataExists, 0, 0);
	sem_init(&spaceExists, 0, 0);
}

template <typename T>
SyncChannel<T>::~SyncChannel()
{
	pthread_mutex_destroy(&bufLock);
	sem_destroy(&dataExists);
	sem_destroy(&spaceExists);
}

template <typename T>
T SyncChannel<T>::get()
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
void SyncChannel<T>::put(const T &data)
{
	pthread_mutex_lock(&bufLock);
	buf.emplace_back(data);
	pthread_mutex_unlock(&bufLock);

	sem_post(&dataExists);	
	sem_wait(&spaceExists);
}
