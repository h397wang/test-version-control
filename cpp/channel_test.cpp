#include "sync_channel.h"
#include "async_channel.h"

#include <thread>
#include <iostream>

#include <pthread.h>
#include <semaphore.h>

using std::cout;
using std::thread;

void run_async_channel(AsyncChannel<int> *chan)
{
	while (1) {
		int msg = chan->get();
		cout << "Got: " << msg << '\n';
		if (msg == 9)
			break;
	}
}

int test_async_channel()
{
	AsyncChannel<int> chan(3);

	thread t(run_async_channel, &chan);
	for (int i=0; i<10; ++i) {
		chan.put(i);
	}

	t.join();
}

void run_sync_channel(SyncChannel<int> *chan)
{
	while (1) {
		int msg = chan->get();
		cout << "Got: " << msg << '\n';
		if (msg == 9)
			break;
	}
}

int test_sync_channel()
{
	SyncChannel<int> chan;

	thread t(run_sync_channel, &chan);
	for (int i=0; i<10; ++i) {
		chan.put(i);
	}

	t.join();
}

int main()
{
	test_sync_channel();
	test_async_channel();
}
