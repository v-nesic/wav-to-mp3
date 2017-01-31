#include "posix_semaphore.h"

#include <tpunit++.hpp>

struct PosixSemaphoreTest : public tpunit::TestFixture
{
	unsigned int max_value = 1024;
	PosixSemaphoreTest() :
		TestFixture(
			TEST(PosixSemaphoreTest::PostAndWait),
			TEST(PosixSemaphoreTest::CreateAndWait),
			TEST(PosixSemaphoreTest::CopyConstructor),
			TEST(PosixSemaphoreTest::Assignment),
			TEST(PosixSemaphoreTest::PostAllWaitAll),
			TEST(PosixSemaphoreTest::WaitAllPostAll))
	{
	}

	void PostAndWait()
	{
		PosixSemaphore sem(0);

		EXPECT_FALSE(sem.TryWait());
		ASSERT_TRUE(sem.Post());
		EXPECT_TRUE(sem.Wait())
	}

	void CreateAndWait()
	{
		PosixSemaphore sem(max_value);

		for(unsigned int i = 0; i < max_value; ++i)
		{
			EXPECT_TRUE(sem.Wait());
		}

		EXPECT_FALSE(sem.TryWait());
	}

	void CopyConstructor()
	{
		PosixSemaphore sem1(0);
		PosixSemaphore sem2(sem1);

		EXPECT_FALSE(sem1.TryWait());
		ASSERT_TRUE(sem2.Post());
		EXPECT_TRUE(sem1.Wait());

		EXPECT_FALSE(sem2.TryWait());
		ASSERT_TRUE(sem1.Post());
		EXPECT_TRUE(sem2.Wait());
	}

	void Assignment()
	{
		PosixSemaphore sem1(1);
		PosixSemaphore sem2(0);

		ASSERT_FALSE(sem2.TryWait());
		sem2 = sem1;
		ASSERT_TRUE(sem2.Wait());

		EXPECT_FALSE(sem1.TryWait());
		ASSERT_TRUE(sem2.Post());
		EXPECT_TRUE(sem1.Wait());

		EXPECT_FALSE(sem2.TryWait());
		ASSERT_TRUE(sem1.Post());
		EXPECT_TRUE(sem2.Wait());
	}

	void PostAllWaitAll()
	{
		PosixSemaphore sem(0);

		for(unsigned int i = 0; i < max_value; ++i)
		{
			ASSERT_TRUE(sem.Post());
		}

		for(unsigned int i = 0; i < max_value; ++i)
		{
			EXPECT_TRUE(sem.Wait());
		}

		EXPECT_FALSE(sem.TryWait());
	}

	void WaitAllPostAll()
	{
		PosixSemaphore sem(max_value);

		for(unsigned int i = 0; i < max_value; ++i)
		{
			EXPECT_TRUE(sem.Wait());
		}

		ASSERT_FALSE(sem.TryWait());

		for(unsigned int i = 0; i < max_value; ++i)
		{
			ASSERT_TRUE(sem.Post());
		}

		for(unsigned int i = 0; i < max_value; ++i)
		{
			EXPECT_TRUE(sem.Wait());
		}

		EXPECT_FALSE(sem.TryWait());
	}

} PosixSemaphoreTest;

