#include "thread_pool.h"

#include <tpunit++.hpp>

#include <atomic>

using std::atomic;

struct ThreadPoolTest :
		public tpunit::TestFixture
{
	ThreadPoolTest() :
		TestFixture(
			TEST(ThreadPoolTest::StartSingleThread),
			TEST(ThreadPoolTest::StartMultipleThreads))
	{
	}

	void StartSingleThread()
	{
		ThreadPool pool(5);
		bool done = false;

		ASSERT_TRUE(pool.StartNew([&done]() { done = true; }));
		pool.JoinAll();

		EXPECT_TRUE(done);
	}

	void StartMultipleThreads()
	{
		static const int max_threads = 10;
		ThreadPool pool(max_threads/2);

		atomic<int> threads_run(0);
		for(int i = 0; i < max_threads; ++i)
		{
			ASSERT_TRUE(pool.StartNew([&threads_run](){ ++threads_run; }));
		}
		pool.JoinAll();

		EXPECT_EQUAL(max_threads, threads_run);
	}

} ThreadPoolTest;
