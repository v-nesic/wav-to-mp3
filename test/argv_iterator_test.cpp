#include "argv_iterator.h"

#include <tpunit++.hpp>

#include <cstring>

struct ArgvIteratorTest : public tpunit::TestFixture
{
	char exe_name[16];

	ArgvIteratorTest() :
		TestFixture(
            TEST(ArgvIteratorTest::GetArguments),
            TEST(ArgvIteratorTest::GetOptions))
	{
		strcpy(exe_name, "exe_name");
	}

	void GetArguments()
	{
		char arg1[] = "arg1";
		char arg2[] = "arg2";
		char arg3[] = "arg3";

		char* argv[] = { exe_name, arg1, arg2, arg3 };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgvIterator it(argc, argv);

		EXPECT_EQUAL(std::string("exe_name"), it.GetValue());
		EXPECT_TRUE(it.Next());

		EXPECT_EQUAL(std::string("arg1"), it.GetValue());
		EXPECT_TRUE(it.Next());

		EXPECT_EQUAL(std::string("arg2"), it.GetValue());
		EXPECT_TRUE(it.Next());

		EXPECT_EQUAL(std::string("arg3"), it.GetValue());
		EXPECT_FALSE(it.Next());
	}

	void GetOptions()
	{
		char arg1[] = "-a1";
		char arg2[] = "-b";

		char* argv[] = { exe_name, arg1, arg2 };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgvIterator it(argc, argv);

		EXPECT_EQUAL(std::string("exe_name"), it.GetValue());
		EXPECT_FALSE(it.IsOption());
		EXPECT_TRUE(it.Next());

		EXPECT_EQUAL(std::string("-a1"), it.GetValue());
		EXPECT_TRUE(it.IsOption());
		EXPECT_EQUAL('a', it.GetOption());
		EXPECT_EQUAL(std::string("1"), it.GetOptionParam());
		EXPECT_TRUE(it.Next());

		EXPECT_EQUAL(std::string("-b"), it.GetValue());
		EXPECT_TRUE(it.IsOption());
		EXPECT_EQUAL('b', it.GetOption());
		EXPECT_EQUAL(std::string(""), it.GetOptionParam());
		EXPECT_FALSE(it.Next());
	}

} ArgvIteratorTest;

