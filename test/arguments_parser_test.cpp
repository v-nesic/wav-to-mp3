#include "arguments_parser.h"

#include <tpunit++.hpp>

#include <cstdio>
#include <cstring>
#include <sys/stat.h>

#ifdef linux
#include <unistd.h>
#endif

struct ArgumentsParserTest : public tpunit::TestFixture
{
	char name[16];

	ArgumentsParserTest() :
		TestFixture(
            TEST(ArgumentsParserTest::NoArguments),
            TEST(ArgumentsParserTest::MissingDirectoryPath),
            TEST(ArgumentsParserTest::FileInsteadOfDirectory),
            TEST(ArgumentsParserTest::ValidDirectoryPath),
            TEST(ArgumentsParserTest::TwoInputDirectories),
            TEST(ArgumentsParserTest::MaxThreadsSetV1),
            TEST(ArgumentsParserTest::MaxThreadsSetV2),
            TEST(ArgumentsParserTest::MaxThreadsSetUsesLastParam),
            TEST(ArgumentsParserTest::MaxThreadsValueNotNumber))
	{
		strcpy(name, "exe_name");
	}

	void NoArguments()
	{
		char* argv[] = { name };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);

		EXPECT_FALSE(arguments_parser.Valid());
	}

	void MissingDirectoryPath()
	{
		struct stat st;
		char path[L_tmpnam];
		ASSERT_EQUAL(-1, stat(tmpnam(path), &st));

		char* argv[] = { name, path };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);
		EXPECT_FALSE(arguments_parser.Valid());
	}

	void FileInsteadOfDirectory()
	{
		char file[L_tmpnam];
		FILE* f = fopen(tmpnam(file), "w+");
		ASSERT_NOT_EQUAL(NULL, f);

		char* argv[] = { name, file };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);
		EXPECT_FALSE(arguments_parser.Valid());

		ASSERT_EQUAL(0, fclose(f));
		ASSERT_EQUAL(0, unlink(file));
	}

	void ValidDirectoryPath()
	{
		char path[L_tmpnam];
		ASSERT_EQUAL(0, portable_mkdir(tmpnam(path)));

		char* argv[] = { name, path };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);
		EXPECT_TRUE(arguments_parser.Valid());

		ASSERT_EQUAL(0, rmdir(path));
	}

	void TwoInputDirectories()
	{
		char path1[L_tmpnam];
		ASSERT_EQUAL(0, portable_mkdir(tmpnam(path1)));

		char path2[L_tmpnam];
		ASSERT_EQUAL(0, portable_mkdir(tmpnam(path2)));

		char* argv[] = { name, path1, path2};
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);

		EXPECT_FALSE(arguments_parser.Valid());

		ASSERT_EQUAL(0, rmdir(path1));
		ASSERT_EQUAL(0, rmdir(path2));
	}

	void MaxThreadsSetV1()
	{
		char path[L_tmpnam];
		ASSERT_EQUAL(0, portable_mkdir(tmpnam(path)));

		char threads[] = "-t5";

		char* argv[] = { name, threads, path };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);
		EXPECT_TRUE(arguments_parser.Valid());
		EXPECT_EQUAL(5, arguments_parser.MaxThreads());

		ASSERT_EQUAL(0, rmdir(path));
	}

	void MaxThreadsSetV2()
	{
		char path[L_tmpnam];
		ASSERT_EQUAL(0, portable_mkdir(tmpnam(path)));

		char threads_opt[] = "-t";
		char threads_val[] = "7";

		char* argv[] = { name, path, threads_opt, threads_val };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);
		EXPECT_TRUE(arguments_parser.Valid());
		EXPECT_EQUAL(7, arguments_parser.MaxThreads());

		ASSERT_EQUAL(0, rmdir(path));
	}

	void MaxThreadsSetUsesLastParam()
	{
		char path[L_tmpnam];
		ASSERT_EQUAL(0, portable_mkdir(tmpnam(path)));

		char threads1[] = "-t2";
		char threads2[] = "-t9";

		char* argv[] = { name, path, threads1, threads2 };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);
		EXPECT_TRUE(arguments_parser.Valid());
		EXPECT_EQUAL(9, arguments_parser.MaxThreads());

		ASSERT_EQUAL(0, rmdir(path));
	}

	void MaxThreadsValueNotNumber()
	{
		char path[L_tmpnam];
		ASSERT_EQUAL(0, portable_mkdir(tmpnam(path)));

		char threads[] = "-tx";

		char* argv[] = { name, path, threads };
		int argc = sizeof(argv)/sizeof(argv[0]);

		ArgumentsParser arguments_parser(argc, argv);
		EXPECT_FALSE(arguments_parser.Valid());

		ASSERT_EQUAL(0, rmdir(path));
	}

	int portable_mkdir(const char* path)
	{
#ifdef linux
		return mkdir(path, 0777);
#else
		return mkdir(path);
#endif
	}

} ArgumentsParserTest;

