#include "arguments_parser.h"
#include "directory_encoder.h"
#include "scope_execution_timer.h"

int main(int argc, char *argv[])
{
	int status = 0;
	ArgumentsParser arguments(argc, argv);

	if (arguments.Valid())
	{
		ScopeExecutionTimer<> execution_timer("Ecoding");

		DirectoryEncoder encoder;

		if (arguments.MaxThreads() > 0)
		{
			encoder.SetMaxThreads(arguments.MaxThreads());
		}

		if (!encoder.Encode(Directory(arguments.Path())))
		{
			status = 1;
		}
	}
	else
	{
		arguments.ShowHelp();
		status = 2;
	}

	return status;
}
