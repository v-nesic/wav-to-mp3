#ifndef POSIX_THREAD_H
#define POSIX_THREAD_H

#include <functional>

#include <pthread.h>

/*!
 * \class PosixThread
 *
 * \brief a wrapper class for starting pthreads
 *
 * NOTE: Threads are started by a separate method rather than by constructor
 *       because it simplifies ThreadPool handling of (highly improbable)
 *       thread-not-started event.
 */
class PosixThread
{
public:
	/*!
	 * \class Owner thread owner interface
	 *
	 * \brief this class defines an interface which must be implemented by thread
	 *        "owner" in oreder to get notified when the execution is done
	 */
	class Owner
	{
	public:
		/*!
		 * \brief ThreadFinished thread finished event handler
		 *
		 * NOTE: this method is called in the context which allows the owner`
		 *       to safely destroy finished thread object
		 *
		 * \param thread pointer to the object representing finished thread
		 */
		virtual void ThreadFinished(PosixThread* thread) = 0;
	};

	/*!
	 * \brief PosixThread constructor
	 *
	 * \param run_fn function object to be executed
	 */
	PosixThread(std::function<void(void)> run_fn, Owner* owner = 0);

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
	Owner* _owner;
	pthread_t _thread;
};

#endif // POSIX_THREAD_H
