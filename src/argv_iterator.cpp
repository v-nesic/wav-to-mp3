#include "argv_iterator.h"

#include <cassert>

ArgvIterator::ArgvIterator(int argc, char* argv[]) :
	_argc(argc),
	_argv(argv),
	_current(0)
{
	assert(0 < _argc);
	assert(NULL != _argv);
	for(int i = 0; i < _argc; ++i)
	{
		assert(NULL != _argv[i]);
	}
}

std::string ArgvIterator::GetValue() const
{
	assert(_current < _argc);
	return _argv[_current];
}

bool ArgvIterator::IsOption() const
{
	assert(_current < _argc);
	return '-' == _argv[_current][0] && '\0' != _argv[_current][1];
}

char ArgvIterator::GetOption() const
{
	assert(_current < _argc);
	return _argv[_current][1];
}

std::string ArgvIterator::GetOptionParam() const
{
	assert(_current < _argc);
	return _argv[_current] + 2;
}

bool ArgvIterator::Next()
{
	if (_current < _argc)
	{
		++_current;
	}

	return _current < _argc;
}
