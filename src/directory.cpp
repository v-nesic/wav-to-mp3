#include "directory.h"

using std::string;

// Directory implementation
Directory::Directory(const FileName& path) :
	_path(path)
{
}

Directory::Directory(const Directory& dir) :
	_path(dir._path)
{
}

Directory&Directory::operator=(const Directory& dir)
{
	_path = dir._path;
	return *this;
}

// Directory::Iterator implementation
Directory::Iterator::Iterator() :
	_path(0),
	_dir(0),
	_file(0)
{
}

Directory::Iterator::Iterator(const FileName* path) :
	_path(path),
	_dir(opendir(_path->FullPath().c_str())),
	_file(_dir ? readdir(_dir) : 0)
{
}

Directory::Iterator::Iterator(Iterator&& it) :
	_path(std::move(it._path)),
	_dir(it._dir),
	_file(it._file)
{
	it._dir = 0;
	it._file = 0;
}

Directory::Iterator& Directory::Iterator::operator=(Iterator&& it)
{
	if (_dir)
	{
		closedir(_dir);
	}

	_dir = it._dir;
	_file = it._file;

	it._dir = 0;
	it._file = 0;

	return *this;
}

Directory::Iterator::~Iterator()
{
	if (_dir)
	{
		closedir(_dir);
	}
}

FileName Directory::Iterator::operator*() const
{
	return _path && _file ? FileName(*_path, string(_file->d_name)) : FileName("");
}

Directory::Iterator& Directory::Iterator::operator++()
{
	if (_dir && _file)
	{
		_file = readdir(_dir);
	}
	return *this;
}
