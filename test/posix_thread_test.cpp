#include "posix_thread.h"
#include "posix_semaphore.h"

#include <tpunit++.hpp>

#include <atomic>

using std::atomic;

class ThreadObserver : public PosixThread::Owner
{
public:
	ThreadObserver() : _sem(0) { }
	bool WaitForThreadFinished() { return _sem.Wait(); }
	virtual void ThreadFinished(PosixThread*) { _sem.Post(); }
private:
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
		ThreadObserver owner;
		PosixThread thread([&done] () { done = true; }, &owner);

		ASSERT_TRUE(thread.Start());
		EXPECT_TRUE(owner.WaitForThreadFinished());
		EXPECT_TRUE(done);
	}

	void TwoThreads()
	{
		PosixSemaphore sem(0);
		atomic<int> threads_done(0);
		auto thread_fn = [&threads_done] () { ++threads_done; };

		ThreadObserver observer1;
		ThreadObserver observer2;

		PosixThread thread1(thread_fn, &observer1);
		PosixThread thread2(thread_fn, &observer2);

		ASSERT_TRUE(thread1.Start());
		ASSERT_TRUE(thread2.Start());

		EXPECT_TRUE(observer1.WaitForThreadFinished());
		EXPECT_TRUE(observer2.WaitForThreadFinished());
		EXPECT_EQUAL(2, threads_done);
	}

} PosixThreadTest;

