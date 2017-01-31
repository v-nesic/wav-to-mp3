#include "filename.h"

#include <sys/stat.h>
#include <cstdio>

#ifdef linux
#include <unistd.h>
#endif

using std::string;

#ifdef _WIN32
const char FileName::kPathSeparator = '\\';
#else
const char FileName::kPathSeparator = '/';
#endif

FileName::FileName() :
	_fullpath("")
{
}

FileName::FileName(const string& pathname) :
	_fullpath(pathname)
{
}

FileName::FileName(const FileName& path, const string& filename) :
	_fullpath(path._fullpath)
{
	if (kPathSeparator != _fullpath.back() && kPathSeparator != filename.front())
	{
		_fullpath.push_back(kPathSeparator);
	}
	_fullpath.append(filename);
}

FileName::FileName(const FileName& path, const string& basename, const string& extension) :
	FileName(path, basename)
{
	_fullpath.push_back('.');
	_fullpath.append(extension);
}

FileName::FileName(const FileName& fn) :
	_fullpath(fn._fullpath)
{
}

FileName::FileName(FileName&& fn) :
	_fullpath(std::move(fn._fullpath))
{
}

FileName& FileName::operator=(const FileName& fn)
{
	_fullpath = fn._fullpath;
	return *this;
}

FileName& FileName::operator=(FileName&& fn)
{
	_fullpath = std::move(fn._fullpath);
	return *this;
}

bool FileName::operator==(const FileName& fn) const
{
	return _fullpath == fn._fullpath;
}

bool FileName::operator!=(const FileName& fn) const
{
	return _fullpath != fn._fullpath;
}

void FileName::SetFullPath(const std::string& fullpath)
{
	_fullpath = fullpath;
}

string FileName::Path() const
{
	auto last_path_separator = _fullpath.rfind(kPathSeparator);

	if (string::npos != last_path_separator && 0 < last_path_separator)
	{
		return std::move(_fullpath.substr(0, last_path_separator));
	}
	else
	{
		return std::move(string("."));
	}
}

string FileName::Name() const
{
	auto last_path_separator = _fullpath.rfind(kPathSeparator);
	auto filename_offset = string::npos != last_path_separator ? last_path_separator + 1 : 0;
	return std::move(_fullpath.substr(filename_offset));
}

string FileName::Basename() const
{
	string basename = Name();

	auto dot_offset = basename.rfind('.');
	if (string::npos != dot_offset)
	{
		basename.resize(dot_offset);
	}

	return std::move(basename);
}

string FileName::Type() const
{
	string type;

	auto dot_offset = _fullpath.rfind('.');
	if (string::npos != dot_offset)
	{
		auto extension_offset = dot_offset + 1; // skipping the dot

		type = _fullpath.substr(extension_offset);
		for(auto c = type.begin(); c != type.end(); ++c)
		{
			*c = toupper(*c);
		}
	}

	return std::move(type);
}

bool FileName::Exists() const
{
	struct stat st;
	return stat(_fullpath.c_str(), &st) != -1;
}

bool FileName::IsDirectory() const
{
	return StatHasFlag(S_IFDIR);
}

bool FileName::Touch() const
{
	bool created = false;

	if (!Exists())
	{
		FILE* f = fopen(_fullpath.c_str(), "w+");
		if (NULL != f && -1 != fclose(f))
		{
			created = true;
		}
	}

	return created;
}

bool FileName::IsFile() const
{
	return StatHasFlag(S_IFREG);
}

bool FileName::Unlink() const
{
	return 0 == unlink(_fullpath.c_str());
}

bool FileName::MkDir() const
{
#ifdef linux
	return 0 == mkdir(_fullpath.c_str(), 0777);
#else
	return 0 == mkdir(_fullpath.c_str());
#endif
}

bool FileName::RmDir() const
{
	return 0 == rmdir(_fullpath.c_str());
}

template<typename T>
bool FileName::StatHasFlag(T flag) const
{
	struct stat st;
	return (0 == stat(_fullpath.c_str(), &st)) && (st.st_mode & flag);
}

std::ostream& operator<<(std::ostream& out, const FileName& fn)
{
	return out << fn.FullPath();
}
