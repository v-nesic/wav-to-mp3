#ifndef POSIX_SEMAPHORE_H
#define POSIX_SEMAPHORE_H

#include <memory>

#include <semaphore.h>

/*!
 * \class PosixSemaphore
 *
 * \brief wrapper class providing access to posix semaphores
 */
class PosixSemaphore
{
public:
	/*!
	 * \brief PosixSemaphore constructor
	 *
	 * \param value initial (and at the same time maximal) semaphore value
	 */
	PosixSemaphore(unsigned int value);

	/*!
	 * \brief PosixSemaphore copy constructor
	 *
	 * \param s posix semaphore object to be copied from
	 */
	PosixSemaphore(const PosixSemaphore& s);

	/*!
	 * \brief PosixSemaphore destructor
	 */
	~PosixSemaphore() { sem_destroy(_semaphore.get()); }

	/*!
	 * \brief operator =
	 *
	 * \param s posix semaphore object to be copied from
	 *
	 * \return reference to *this
	 */
	PosixSemaphore& operator=(const PosixSemaphore& s);

	/*!
	 * \brief Post wakes up one waiter
     *
	 * \return true if the operation is successful, false in case of an error
	 */
	bool Post() { return 0 == sem_post(_semaphore.get()); }

	/*!
	 * \brief Wait acquires semaphore or blocks thread execution until
	 *        the semaphore is acquired
	 *
     * \return true if the semaphore is acquired, false in case of an error
	 */
	bool Wait() { return 0 == sem_wait(_semaphore.get()); }

    /*!
     * \brief TryWait tries to acquire semaphore without waiting for it
     *
     * \return true if the semaphore is acquired, false if not
     */
	bool TryWait() { return 0 == sem_trywait(_semaphore.get()); }

private:
	std::shared_ptr<sem_t> _semaphore;
};

#endif // POSIX_SEMAPHORE_H
