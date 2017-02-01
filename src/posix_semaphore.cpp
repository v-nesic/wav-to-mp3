#include "posix_semaphore.h"

PosixSemaphore::PosixSemaphore(unsigned int value) :
	_semaphore(std::make_shared<sem_t>())
{
	sem_init(_semaphore.get(), 0, value);
}

PosixSemaphore::PosixSemaphore(const PosixSemaphore& s) :
	_semaphore(s._semaphore)
{
}

PosixSemaphore&PosixSemaphore::operator=(const PosixSemaphore& s)
{
	_semaphore = s._semaphore;
	return *this;
}
