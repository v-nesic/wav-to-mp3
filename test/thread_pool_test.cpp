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
		bool done = false;

		ThreadPool* thread_pool = new ThreadPool(5);
		ASSERT_NOT_EQUAL(nullptr, thread_pool);
		ASSERT_TRUE(thread_pool->StartNew([&done]() { done = true; }));

		delete thread_pool;
		EXPECT_TRUE(done);
	}

	void StartMultipleThreads()
	{
		static const int max_threads = 10;
		ThreadPool* thread_pool = new ThreadPool(max_threads/2);
		ASSERT_NOT_EQUAL(nullptr, thread_pool);

		atomic<int> threads_run(0);
		for(int i = 0; i < max_threads; ++i)
		{
			ASSERT_TRUE(thread_pool->StartNew([&threads_run](){ ++threads_run; }));
		}

		delete thread_pool;
		EXPECT_EQUAL(max_threads, threads_run);
	}

} ThreadPoolTest;
