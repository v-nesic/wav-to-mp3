#include "arguments_parser.h"

#include <iostream>

ArgumentsParser::ArgumentsParser(int argc, char* argv[]) :
	_argument(argc, argv),
	_valid(true),
	_path_set(false),
	_max_threads_set(false),
	_path(),
	_max_threads(0)
{
    while (_argument.Next()) // start by skipping over invoked-executable name
	{
		if (_argument.IsOption() && 't' == _argument.GetOption())
		{
			ParseMaxThreads();
		}
		else
		{
			CapturePath();
		}
	}

	if (!_path_set)
	{
		_valid = false;
	}
}

void ArgumentsParser::CapturePath()
{
	if (!_path_set)
	{
		_path.SetFullPath(_argument.GetValue());
		_path_set = true;
		_valid = _path.IsDirectory();
	}
	else
	{
		_valid = false;
	}
}

void ArgumentsParser::ParseMaxThreads()
{
	std::string max_threads = _argument.GetOptionParam();

	if (max_threads.length() == 0 && _argument.Next())
	{
		max_threads = _argument.GetValue();
	}

	// This will accept some things which are not strictly valid,
    // for example -t9mm will be interpeted as 9, but it shall not
	// prevent users from using the tool
	_max_threads = std::atoi(max_threads.c_str());

	if (0 == _max_threads)
	{
		_valid = false;
	}
}

void ArgumentsParser::ShowHelp() const
{
	std::cout << "Usage: WavToMp3 [-t max_threads] path" << std::endl;
}
