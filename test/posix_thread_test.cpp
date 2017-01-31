#include "posix_thread.h"
#include "posix_semaphore.h"

#include <tpunit++.hpp>

#include <atomic>

using std::atomic;

class ThreadObserver : private PosixThread::Observer
{
public:
	ThreadObserver(PosixThread& thread) : _sem(0) { thread.SetObserver(this); }
	bool WaitForThreadFinished() { return _sem.Wait(); }
private:
	virtual void ThreadFinished(PosixThread*) { _sem.Post(); }
	PosixSemaphore _sem;
};

struct PosixThreadTest : public tpunit::TestFixture
{
	PosixThreadTest() :
		TestFixture(
			TEST(PosixThreadTest::SingleThread),
			TEST(PosixThreadTest::TwoThreads))
	{
	}

	void SingleThread()
	{
		bool done = false;
		PosixSemaphore sem(0);
		PosixThread thread([&done] () { done = true; });
		ThreadObserver observer(thread);

		ASSERT_TRUE(thread.Start());
		EXPECT_TRUE(observer.WaitForThreadFinished());
		EXPECT_TRUE(done);
	}

	void TwoThreads()
	{
		PosixSemaphore sem(0);
		atomic<int> threads_done(0);
		auto thread_fn = [&threads_done] () { ++threads_done; };

		PosixThread thread1(thread_fn);
		PosixThread thread2(thread_fn);

		ThreadObserver observer1(thread1);
		ThreadObserver observer2(thread2);

		ASSERT_TRUE(thread1.Start());
		ASSERT_TRUE(thread2.Start());

		EXPECT_TRUE(observer1.WaitForThreadFinished());
		EXPECT_TRUE(observer2.WaitForThreadFinished());
		EXPECT_EQUAL(2, threads_done);
	}

} PosixThreadTest;

