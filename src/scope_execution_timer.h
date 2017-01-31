#ifndef SCOPEEXECUTIONTIMER_H
#define SCOPEEXECUTIONTIMER_H

#include <chrono>
#include <iostream>
#include <string>

/*!
 * \class ScopeExecutionTimer
 *
 * \brief debugging class for measuring execution time, built as template to
 *        allow using different clock sources
 *
 * NOTE: this class is not synchronized, meaning that output can become
 *       garbled when multiple threads share the same file
 */
template<typename clock_class=std::chrono::steady_clock>
class ScopeExecutionTimer
{
public:
	/*!
	 * \brief ScopeExecutionTimer constructor, start time measurement
	 *
	 * \param scope_name timed entity name (will be printed)
	 * \param out stream to write to
	 */
	ScopeExecutionTimer(std::string scope_name, std::ostream& out = std::cout) :
		_scope_name(scope_name),
		_out(out),
		_start(clock_class::now())
	{
	}

	/*!
	 * \brief ScopeExecutionTimer destructor, prints measured time
	 */
	~ScopeExecutionTimer()
	{
		std::chrono::duration<double> duration = clock_class::now() - _start;
		_out << _scope_name << " duration " << duration.count() << "s" << std::endl;
	}

	// This object can't be moved or copied
	ScopeExecutionTimer(const ScopeExecutionTimer&) = delete;
	ScopeExecutionTimer& operator=(const ScopeExecutionTimer&) = delete;

private:
	std::string _scope_name;
	std::ostream& _out;
	std::chrono::time_point<clock_class> _start;
};

#endif // SCOPEEXECUTIONTIMER_H
