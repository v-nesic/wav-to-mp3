#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "posix_semaphore.h"
#include "posix_thread.h"

/*!
 * \class ThreadPool
 *
 * \brief manages thread objects and ensures that no more than max threads are
 *        simultaneously executed at any given point in time.
 *
 * IMPLEMENTATION:
 *   Thread counting and start/stop tracking is done by semaphore:
 *    - inital semaphore value is set to max number of threads (= tokens available)
 *    - before starting, each thread must acquire token (= wait for semaphore)
 *    - every finished thread releases the token (= signals semaphore)
 *    - JoinAll() ensures that no more threads are running by acquiring
 *      all tokens provided by the semaphore
 */
class ThreadPool : public PosixThread::Owner
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
	 * \brief ThreadPool destructor, waits for all running threads to finish
	 */
	virtual ~ThreadPool();

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
