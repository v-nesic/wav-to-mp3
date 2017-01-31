#ifndef FILENAME_H
#define FILENAME_H

#include <iostream>
#include <string>

/*!
 * \class FileName
 *
 * \brief
 *	Helper class for parsing filesystem names and paths, as well as doing
 *	some elementary file sistem operations, such as checking if the filename
 *	refers to a directory.
 */
class FileName
{
public:
	/*!
	 * \brief FileName default constructor
	 */
	FileName();

	/*!
	 * \brief FileName fullpath constructor
	 *
	 * \param pathname full path referning to the file or directory
	 */
	FileName(const std::string& pathname);

	/*!
	 * \brief FileName path + name constructor
	 *
	 * \param path directory which contains the file
	 * \param filename name of the file (including the extension)
	 */
	FileName(const FileName& path, const std::string& filename);

	/*!
	 * \brief FileName path + basename + extension constructor
	 *
	 * \param path directory which contains the file
	 * \param basename file name (without extension)
	 * \param extension filename extension
	 */
	FileName(const FileName& path, const std::string& basename, const std::string& extension);

	/*!
	 * \brief FileName copy constructor
	 *
	 * \param fn a const reference to a FileName object to be copied
	 */
	FileName(const FileName& fn);

	/*!
	 * \brief FileName move constructor
	 *
	 * \param fn move reference to a FileName object to be moved
	 */
	FileName(FileName&& fn);

	/*!
	 * \brief operator = assignment operator
	 *
	 * \param fn a const reference to a FileName object to be copied
	 *
	 * \return reference to the assigned object
	 */
	FileName& operator=(const FileName& fn);


	/*!
	 * \brief operator = assignment operator
	 *
	 * \param fn move reference to a FileName object to be moved
	 *
	 * \return reference to the assigned object
	 */
	FileName& operator=(FileName&& fn);

	/*!
	 * \brief operator ==
	 *
	 * \param fn file name to compare with
	 *
	 * \return true if the file names are the same, false if not
	 */
	bool operator==(const FileName& fn) const;

	/*!
	 * \brief operator !=
	 *
	 * \param fn file name to compare with
	 *
	 * \return false if the file names are the same, true if not
	 */
	bool operator!=(const FileName& fn) const;

	/*!
	 * \brief PathSeparator getter
	 *
	 * \return path separator for the current system
	 */
	static char PathSeparator() { return kPathSeparator; }

	/*!
	 * \brief SetFullPath setter
	 *
	 * \param fullpath new full path value
	 */
	void SetFullPath(const std::string& fullpath);

	/*!
	 * \brief FullPath getter
	 *
	 * \return a const reference to the string containging current full path
	 */
	const std::string& FullPath() const { return _fullpath; }

	/*!
	 * \brief Path getter
	 *
	 * \return directory part of the full path
	 */
	std::string Path() const;

	/*!
	 * \brief Name getter
	 *
	 * \return file name part of the full path
	 */
	std::string Name() const;

	/*!
	 * \brief Basename getter
	 *
	 * \return base name part of the full path
	 */
	std::string Basename() const;

	/*!
	 * \brief Type getter
	 *
	 * \return uppercase version of the file's extension
	 */
	std::string Type() const;

	/*!
	 * \brief Exists filesystem interface
	 *
	 * \return true if the file exist
	 */
	bool Exists() const;

	/*!
	 * \brief IsFile filesystem interface
	 *
	 * \return true if the filename refers to a file
	 */
	bool IsFile() const;

	/*!
	 * \brief IsDirectory filesystem interface
	 *
	 * \return true if the filename refers to a directory
	 */
	bool IsDirectory() const;

	/*!
	 * \brief Touch creates an emtpy file
	 *
	 * \return true if the file is created, false if not
	 */
	bool Touch() const;

	/*!
	 * \brief Unlink filesystem interface
	 *
	 * \return true if the file is deleted
	 */
	bool Unlink() const;

	/*!
	 * \brief MkDir creates an empty directory
	 *
	 * \return true if the directory is created, false if not
	 */
	bool MkDir() const;

	/*!
	 * \brief RmDir filesystem interface
	 *
	 * \return true if the directory is deleted
	 */
	bool RmDir() const;

private:
	static const char kPathSeparator;

	/*!
	 * \brief StatHasFlags helper template for checking stat flags
	 */
	template<typename T> bool StatHasFlag(T flag) const;

	std::string _fullpath;
};

std::ostream& operator<<(std::ostream& out, const FileName& fn);

#endif // FILENAME_H
