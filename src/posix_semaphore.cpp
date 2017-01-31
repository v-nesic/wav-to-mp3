#include "posix_semaphore.h"

PosixSemaphore::PosixSemaphore(unsigned int value) :
	_value(value),
	_semaphore(std::make_shared<sem_t>())
{
	sem_init(_semaphore.get(), 0, _value);
}

PosixSemaphore::PosixSemaphore(const PosixSemaphore& s) :
	_value(s._value),
	_semaphore(s._semaphore)
{
}

PosixSemaphore&PosixSemaphore::operator=(const PosixSemaphore& s)
{
	_value = s._value;
	_semaphore = s._semaphore;
	return *this;
}

bool PosixSemaphore::WaitAll()
{
	for(unsigned i = 0; i < _value; ++i)
	{
		if (!Wait())
		{
			return false;
		}
	}

	// If we have acquired semaphore as many times as possible, any other wait should fail
	return !TryWait();
}
