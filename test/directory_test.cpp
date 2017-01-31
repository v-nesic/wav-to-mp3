#include "directory.h"

#include <tpunit++.hpp>

#include <string>
#include <vector>

using std::string;
using std::vector;

struct DirectoryTest : public tpunit::TestFixture
{
	DirectoryTest() :
		TestFixture(
            TEST(DirectoryTest::Constructor),
            TEST(DirectoryTest::CopyConstructor),
			TEST(DirectoryTest::Assignment),
            TEST(DirectoryTest::Iterator))
	{
	}

	void Constructor()
	{
		Directory dir(FileName("TestDir"));

		EXPECT_EQUAL(FileName("TestDir"), dir.Path());
	}

	void CopyConstructor()
	{
		Directory dir1(FileName("TestDir"));
		Directory dir2(dir1);

		EXPECT_EQUAL(dir1.Path(), dir2.Path());
	}

	void Assignment()
	{
		Directory dir1(FileName("TestDir"));
		Directory dir2(FileName("x"));

		ASSERT_EQUAL(FileName("x"), dir2.Path());
		EXPECT_EQUAL(FileName("TestDir"), (dir2 = dir1).Path());
	}

	void Iterator()
	{
		FileName test_dir("TestDir");

		vector<string> dirs{ "dir1", "dir2" };
		vector<string> files{ "file1", "file2" };

		if (SetupTestDir(test_dir, dirs, files))
		{
			bool dir1_found = false;
			bool dir2_found = false;
			bool file1_found = false;
			bool file2_found = false;

			for(FileName f: Directory(test_dir))
			{
				if (FileName("dir1") == f.Name())
				{
					EXPECT_FALSE(dir1_found);
					dir1_found = true;
				}
				else if (FileName("dir2") == f.Name())
				{
					EXPECT_FALSE(dir2_found);
					dir2_found = true;
				}
				else if (FileName("file1") == f.Name())
				{
					EXPECT_FALSE(file1_found);
					file1_found = true;
				}
				else if (FileName("file2") == f.Name())
				{
					EXPECT_FALSE(file2_found);
					file2_found = true;
				}
                else if (FileName(".") != f.Name())
				{
                    EXPECT_EQUAL(FileName(".."), f.Name());
				}
			}

			EXPECT_TRUE(dir1_found);
			EXPECT_TRUE(dir2_found);
			EXPECT_TRUE(file1_found);
			EXPECT_TRUE(file2_found);
		}

        RemoveTestDir(test_dir, dirs, files);
	}

	bool SetupTestDir(
			const FileName& test_dir,
			const vector<string>& dirs,
			const vector<string>& files) const
	{
		bool set_up = false;

		if (test_dir.MkDir())
		{
			set_up = true;

			for(string d: dirs)
			{
				if (!FileName(test_dir, d).MkDir())
				{
					set_up = false;
				}
			}
			for(string f: files)
			{
				if (!FileName(test_dir, f).Touch())
				{
					set_up = false;
				}
			}
		}

		return set_up;
	}

	bool RemoveTestDir(
			const FileName& test_dir,
			const vector<string>& dirs,
			const vector<string>& files)
	{
		for(string f: files)
		{
			FileName(test_dir, f).Unlink();
		}

		for(string d: dirs)
		{
			FileName(test_dir, d).RmDir();
		}

        return test_dir.RmDir();
	}

} DirectoryTest;
