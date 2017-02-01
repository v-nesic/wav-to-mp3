#include "posix_thread.h"

#include <iostream>

PosixThread::PosixThread(std::function<void(void)> run_fn, Owner* owner) :
	_run_fn(run_fn),
	_owner(owner),
	_thread(pthread_self())
{
}

bool PosixThread::Start()
{
	pthread_attr_t attr;

	bool started = false;

	if (0 == pthread_attr_init(&attr))
	{
		started =
			(0 == pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) &&
			(0 == pthread_create(&_thread, &attr, PosixThread::ThreadRunner, this));

		pthread_attr_destroy(&attr);
	}

	return started;
}

void* PosixThread::ThreadRunner(void* pthread_arg)
{
	PosixThread* thread = static_cast<PosixThread*>(pthread_arg);

	if (thread)
	{
		thread->_run_fn();
		if (thread->_owner)
		{
			thread->_owner->ThreadFinished(thread);
		}
	}

	return NULL;
}
