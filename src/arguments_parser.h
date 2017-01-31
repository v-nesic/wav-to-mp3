#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include "argv_iterator.h"
#include "filename.h"

/*!
 * \class ArgumentsParser
 *
 * \brief helper class for parsing command line arguments
 */
class ArgumentsParser
{
public:
	/*!
	 * \brief ArgumentsParser constructor, parses input data
	 *
	 * \param argc number of arguments (must be greater than 0)
	 * \param argv array of char* pointers to arguments (must not be NULL)
	 */
	ArgumentsParser(int argc, char* argv[]);

	/*!
	 * \brief Valid getter
	 *
	 * \return true if the command line arguments are valid, false if not
	 */
	bool Valid() const { return _valid; }

	/*!
	 * \brief Path getter
	 *
	 * \return directory to be processed
	 */
	const FileName& Path() const { return _path; }

	/*!
	 * \brief MaxThreads getter
	 *
	 * \return max number of threads to be run at the same time
	 */
	unsigned int MaxThreads() const { return _max_threads; }

	/*!
	 * \brief ShowHelp prints help message explaining command line tool arguments
	 */
	void ShowHelp() const;

private:
	/*!
	 * \brief CapturePath processes non-option argument and marks arguments as
	 *                    invalid if we already have directory path or if the
	 *                    path is not refering to the directory
	 */
	void CapturePath();

	/*!
	 * \brief ParseMaxThreads processes max threads argument and marks arguments
	 *                        as not valid the argument is not greather than 0
	 */
	void ParseMaxThreads();

	ArgvIterator _argument;
	bool _valid;
	int _current_argument;
	bool _path_set;
	bool _max_threads_set;
	FileName _path;
	unsigned _max_threads;
};

#endif // ARGUMENTSPARSER_H
