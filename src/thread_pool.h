#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "posix_semaphore.h"
#include "posix_thread.h"

/*!
 * \class ThreadPool
 *
 * \brief manages running threads and ensures that no more than max threads are
 *        executed at any given point in time.
 */
class ThreadPool : public PosixThread::Observer
{
public:
	/*!
	 * \brief ThreadPool constructor
	 *
	 * \param max_threads maximum nuber of threads allowed to run simultaneously,
	 *					  0 sets max_threads to number of CPUs or CPU cores
	 */
	ThreadPool(unsigned int max_threads = 0);

	/*!
	 * \brief StartNew starts a new thread
	 *
	 * NOTE: If max number of threads is already reached, this method will block
	 *       until one of the currently running threads is finished.
	 *
	 * \param thread_fn thread function object to be executed
	 *
	 * \return true if the new thread has been started, false in case of an error
	 */
	bool StartNew(std::function<void(void)> thread_fn);

	/*!
	 * \brief JoinAll waits for all currently running threads to finish
	 */
	void JoinAll();

	// Observer interface
	virtual void ThreadFinished(PosixThread* thread);

	// thread pool objects can't be moved or copied because started threads
	// have pointers refering to them
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
	unsigned int _max_threads;
	PosixSemaphore _semaphore;
};

#endif // THREADPOOL_H
