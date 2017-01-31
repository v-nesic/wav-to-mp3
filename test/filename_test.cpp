#include "filename.h"

#include <tpunit++.hpp>

#include <string>
#include <cstdio>
#include <sys/stat.h>

using std::string;

struct FileNameTest : public tpunit::TestFixture
{
	FileNameTest() :
		TestFixture(
            TEST(FileNameTest::DefaultConstructor),
            TEST(FileNameTest::PathnameConstructor),
            TEST(FileNameTest::PathPlusFilenameConstructor),
            TEST(FileNameTest::PathPlusNamePlusExtensionConstructor),
            TEST(FileNameTest::CopyConstructor),
            TEST(FileNameTest::MoveConstructor),
			TEST(FileNameTest::Assignment),
			TEST(FileNameTest::MoveAssignment),
            TEST(FileNameTest::SetFullPath),
            TEST(FileNameTest::ParseFileName),
            TEST(FileNameTest::FileOperations),
            TEST(FileNameTest::DirectoryOperations))
	{
	}

	string NormalizePath(const string& path)
	{
		string normalized_path(path);

		for(auto it = normalized_path.begin(); it!= normalized_path.end(); ++it)
		{
			if ('/' == *it)
			{
				*it = FileName::PathSeparator();
			}
		}

		return std::move(normalized_path);
	}

	void DefaultConstructor()
	{
		FileName fn;

		EXPECT_EQUAL(string(""), fn.FullPath());
		EXPECT_EQUAL(string("."), fn.Path());
		EXPECT_EQUAL(string(""), fn.Name());
		EXPECT_EQUAL(string(""), fn.Basename());
		EXPECT_EQUAL(string(""), fn.Type());
	}

	void PathnameConstructor()
	{
		FileName fn(NormalizePath("/dir/dir/name.ext"));

		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn.FullPath());
		EXPECT_EQUAL(NormalizePath("/dir/dir"), fn.Path());
		EXPECT_EQUAL(string("name.ext"), fn.Name());
		EXPECT_EQUAL(string("name"), fn.Basename());
		EXPECT_EQUAL(string("EXT"), fn.Type());
	}

	void PathPlusFilenameConstructor()
	{
		FileName fn(NormalizePath("/dir/dir"), "name.ext");

		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn.FullPath());
		EXPECT_EQUAL(NormalizePath("/dir/dir"), fn.Path());
		EXPECT_EQUAL(string("name.ext"), fn.Name());
		EXPECT_EQUAL(string("name"), fn.Basename());
		EXPECT_EQUAL(string("EXT"), fn.Type());
	}

	void PathPlusNamePlusExtensionConstructor()
	{
		FileName fn(NormalizePath("/dir/dir"), "name", "ext");

		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn.FullPath());
		EXPECT_EQUAL(NormalizePath("/dir/dir"), fn.Path());
		EXPECT_EQUAL(string("name.ext"), fn.Name());
		EXPECT_EQUAL(string("name"), fn.Basename());
		EXPECT_EQUAL(string("EXT"), fn.Type());
	}

	void CopyConstructor()
	{
		FileName fn(NormalizePath("/dir/dir/name.ext"));
		FileName fn_copy(fn);

		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn_copy.FullPath());
	}

	void MoveConstructor()
	{
		FileName fn(NormalizePath("/dir/dir/name.ext"));
		FileName fn_moved(std::move(fn));

		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn_moved.FullPath());
	}

	void Assignment()
	{
		FileName fn(NormalizePath("/dir/dir/name.ext"));
		FileName fn2("x");

		EXPECT_EQUAL(string("x"), fn2.FullPath());
		fn2 = fn;
		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn2.FullPath());
		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn.FullPath());
	}

	void MoveAssignment()
	{
		FileName fn(NormalizePath("/dir/dir/name.ext"));
		FileName fn2("x");

		EXPECT_EQUAL(string("x"), fn2.FullPath());
		fn2 = std::move(fn);
		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn2.FullPath());
		EXPECT_NOT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn.FullPath());
	}

	void SetFullPath()
	{
		FileName fn;

		fn.SetFullPath(NormalizePath("/dir/dir/name.ext"));

		EXPECT_EQUAL(NormalizePath("/dir/dir/name.ext"), fn.FullPath());
		EXPECT_EQUAL(NormalizePath("/dir/dir"), fn.Path());
		EXPECT_EQUAL(string("name.ext"), fn.Name());
		EXPECT_EQUAL(string("name"), fn.Basename());
		EXPECT_EQUAL(string("EXT"), fn.Type());
	}

	void ParseFileName()
	{
		FileName fn("name.ext");

		EXPECT_EQUAL(string("name.ext"), fn.FullPath());
		EXPECT_EQUAL(string("."), fn.Path());
		EXPECT_EQUAL(string("name.ext"), fn.Name());
		EXPECT_EQUAL(string("name"), fn.Basename());
		EXPECT_EQUAL(string("EXT"), fn.Type());
	}

	void FileOperations()
	{
		FileName file(string(tmpnam(NULL)));

		ASSERT_FALSE(file.Exists());
		EXPECT_FALSE(file.Unlink());
		EXPECT_FALSE(file.IsFile());
		EXPECT_FALSE(file.IsDirectory());

		ASSERT_TRUE(file.Touch());
		EXPECT_TRUE(file.Exists());
		EXPECT_TRUE(file.IsFile());
		EXPECT_FALSE(file.IsDirectory());
		EXPECT_FALSE(file.RmDir());

		ASSERT_TRUE(file.Unlink());
		EXPECT_FALSE(file.Exists());
		EXPECT_FALSE(file.IsFile());
		EXPECT_FALSE(file.IsDirectory());
	}

	void DirectoryOperations()
	{
		FileName dir(string(tmpnam(NULL)));

		ASSERT_FALSE(dir.Exists());
		EXPECT_FALSE(dir.IsFile());
		EXPECT_FALSE(dir.IsDirectory());

		ASSERT_TRUE(dir.MkDir());
		EXPECT_TRUE(dir.Exists());
		EXPECT_TRUE(dir.IsDirectory());
		EXPECT_FALSE(dir.IsFile());
		EXPECT_FALSE(dir.Unlink());

		ASSERT_TRUE(dir.RmDir());
		EXPECT_FALSE(dir.Unlink());
		EXPECT_FALSE(dir.Exists());
		EXPECT_FALSE(dir.IsFile());
		EXPECT_FALSE(dir.IsDirectory());
	}

} FileNameTest;
