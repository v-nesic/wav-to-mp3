#ifndef ARGVITERATOR_H
#define ARGVITERATOR_H

#include <string>
#include <vector>

/*!
 * \class ArgvIterator
 *
 * \brief This class is used for iterating over supplied command line arguments
 *
 */
class ArgvIterator
{
public:
	/*!
	 * \brief ArgvIterator constructor, saves argc value and argv pointer
	 *
	 * \param argc number of arguments
	 * \param argv array of pointers to argument value strings
	 */
	ArgvIterator(int argc, char* argv[]);

	/*!
	 * \brief GetValue argument value getter
	 * \return value of the current argument
	 */
	std::string GetValue() const;

	/*!
	 * \brief IsOption check whether the current argument is an option
	 * \return true if the argument starts with '-', false if not
	 */
	bool IsOption() const;

	/*!
	 * \brief GetOption option ID getter
	 * \return the letter following the opening '-' char
	 */
	char GetOption() const;

	/*!
	 * \brief GetOptionParam option parameter getter
	 * \return argument value after the inital '-' and option ID are stripped
	 */
	std::string GetOptionParam() const;

	/*!
	 * \brief Next moves internal "current" pointer to the next argument
	 * \return false if there are no more arguments, true otherwise
	 */
	bool Next();

private:
	int _argc;
	char** _argv;
	int _current;
};

#endif // ARGVITERATOR_H
