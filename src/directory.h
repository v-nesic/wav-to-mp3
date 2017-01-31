#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <dirent.h>
#include <iostream>
#include <string>

#include "filename.h"

/*!
 * \class Directory
 *
 * \brief File system interface class for iterating over selected directory
 */
class Directory
{
public:
	class Iterator;

	/*!
	 * \brief Directory constructor
	 *
	 * \param path FileName object providing a path to directory
	 */
	Directory(const FileName& path);

	/*!
	 * \brief Directory copy constructor
	 *
	 * \param dir a const reference to the dir object to be copied
	 */
	Directory(const Directory& dir);

	/*!
	 * \brief begin iterator getter
	 *
	 * \return iterator refering to the first item contained by the directory
	 */
	Iterator begin() const { return std::move(Iterator(&_path)); }

	/*!
	 * \brief end iterator getter
	 *
	 * \return iterator refering to the one-past the last item contained by the directory
	 */
	Iterator end() const { return std::move(Iterator()); }

	/*!
	 * \brief Path getter
	 *
	 * \return directory's path
	 */
	const FileName& Path() const { return _path; }

	/*!
	 * \brief operator = assignment operator
	 *
	 * \param dir directory object to be copied from
	 *
	 * \return a reference to *this
	 */
	Directory& operator=(const Directory& dir);

	/*!
	* \class Helper class for iterating over directory entries
	*/
	class Iterator
	{
	public:
		/*!
		 * \brief
		 * Iterator default constructor, creates an iterator representing
		 * one-past the last item in the directory
		 */
		Iterator();

		/*!
		 * \brief Iterator constructor
		 *
		 * \param directory over which should be iterated
		 */
		Iterator(const FileName* path);

		/*!
		 * \brief Iterator move constructor
		 *
		 * \param it iterator object to be moved
		 */
		Iterator(Iterator&& it);

		/*!
		 * \brief operator = move assignment operator
		 *
		 * \param it iterator object to be moved
		 *
		 * \return a reference to *this
		 */
		Iterator& operator=(Iterator&& it);

		/*!
		 * \brief Iterator destructor, closes open directory handle
		 */
		~Iterator();

		/*!
		 * \brief operator *
		 *
		 * \return FileName refering to the current directory item
		 */
		FileName operator*() const;

		/*!
		 * \brief operator ++ (preincrement)
		 *
		 * \return reference to *this
		 */
		Iterator& operator++();

		/*!
		 * \brief operator ==
		 *
		 * \param it a const reference to the iterator to be compared with
		 *
		 * \return true if the iterators refer to the the same item, false if not
		 */
		bool operator==(const Iterator& it) const { return _file == it._file; }

		/*!
		 * \brief operator !=
		 *
		 * \param it a const reference to the iterator to be compared with
		 *
		 * \return false if the iterators refer to the the same item, true if not
		 */
		bool operator!=(const Iterator& it) const { return _file != it._file; }

	private:
		const FileName* _path;
		DIR* _dir;
		struct dirent* _file;
	};

private:
	FileName _path;
};

#endif // DIRECTORY_H
