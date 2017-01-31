#include "directory_encoder.h"

#include <atomic>
#include <iostream>

#include "lame_encoder.h"
#include "thread_pool.h"
#include "scope_execution_timer.h"

using std::atomic;

DirectoryEncoder::DirectoryEncoder() :
	_max_threads(0)
{
}

void DirectoryEncoder::SetMaxThreads(int max_threads)
{
	_max_threads = max_threads;
}

bool DirectoryEncoder::Encode(const Directory& directory)
{
	std::cout << "Ecoding " << directory.Path() << " using up to " << _max_threads << " threads." << std::endl;

	int files_found = 0;
	atomic<int> files_encoded(0);

	ThreadPool threads(_max_threads);
	for(FileName wav : directory)
	{
		if (wav.Type() == "WAV" && wav.IsFile())
		{
			++files_found;

			bool thread_started = threads.StartNew(
				[wav, &files_encoded] () -> void
				{
					if (Encode(wav))
					{
						++files_encoded;
					}
				});

			if (thread_started)
			{
				std::cout << wav.Name() << " scheduled for encoding." << std::endl;
			}
			else
			{
				std::cerr << "Failed to schedule " << wav.Name() << " for encoding!" << std::endl;
			}
		}
	}

	threads.JoinAll();

	std::cout << files_found << " files found." << std::endl;
	std::cout << files_encoded << " files encoded." << std::endl;

	return files_found == files_encoded;
}

bool DirectoryEncoder::Encode(const FileName& wav)
{
	//ScopeExecutionTimer<> execution_timer(wav.Name() + " encoding");

	FileName mp3(wav.Path(), wav.Basename(), "mp3");

	return LameEncoder(wav, mp3).Encode();
}
