#include "thread_pool.h"

#include <thread>

ThreadPool::ThreadPool(unsigned int max_threads) :
	_max_threads(max_threads ? max_threads : std::thread::hardware_concurrency()),
	_semaphore(_max_threads)
{
}

ThreadPool::~ThreadPool()
{
	for(unsigned int i = 0; i < _max_threads; ++i)
	{
		_semaphore.Wait();
	}
}

bool ThreadPool::StartNew(std::function<void ()> thread_fn)
{
	bool started  = false;

	if (_semaphore.Wait())
	{
		PosixThread* thread = new PosixThread(thread_fn, this);

		started = thread->Start();

		if (!started)
		{
			delete thread;
			_semaphore.Post();
		}
	}

	return started;
}

void ThreadPool::ThreadFinished(PosixThread* thread)
{
	delete thread;
	_semaphore.Post();
}
