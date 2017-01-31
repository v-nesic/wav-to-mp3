#ifndef POSIX_THREAD_H
#define POSIX_THREAD_H

#include <functional>

#include <pthread.h>

/*!
 * \class PosixThread
 *
 * \brief a wrapper class for starting pthreads
 *
 */
class PosixThread
{
public:
	/*!
	 * \class Observer
	 * \brief helper class for monitoring thread execution
	 */
	class Observer
	{
	public:
		/*!
		 * \brief ThreadFinished thread finished event handler
		 *
		 * \param thread pointer to the finished thread
		 */
		virtual void ThreadFinished(PosixThread* thread) = 0;
	};

	/*!
	 * \brief PosixThread constructor
	 *
	 * \param run_fn function object to be executed
	 */
	PosixThread(std::function<void(void)> run_fn);

	/*!
	 * \brief SetObserver sets thread observer
	 *
	 * NOTE: This method is reliable only if called before the thread is started
	 *
	 * \param observer new observer
	 */
	void SetObserver(Observer* observer);

	/*!
	 * \brief Start configures new thread as detached and starts it
	 *
	 * \return true if the thread is started, false if not
	 */
	bool Start();

private:
	/*!
	 * \brief ThreadRunner is a static method for running scheduled threads
	 *
	 * \param pthread_arg should be a pointer to the ThreadRunner object
	 *
	 * \return NULL
	 */
	static void* ThreadRunner(void* pthread_arg);

	std::function<void(void)> _run_fn;
	Observer* _observer;
	pthread_t _thread;
};

#endif // POSIX_THREAD_H
