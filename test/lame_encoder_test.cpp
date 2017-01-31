#include "lame_encoder.h"

#include <tpunit++.hpp>

#include "filename.h"

struct LameEncoderTest : public tpunit::TestFixture
{
	FileName output;

	LameEncoderTest() :
		TestFixture(
            AFTER(LameEncoderTest::DeleteEncodedFile),
            TEST(LameEncoderTest::EncodeMonoFile),
            TEST(LameEncoderTest::EncodeStereoFile),
            TEST(LameEncoderTest::EncodeInvalidFile),
            TEST(LameEncoderTest::EncodeDirectory)),
		output("out.mp3")
	{
	}

	void DeleteEncodedFile()
	{
		output.Unlink();
	}

	void EncodeMonoFile()
	{
		FileName input(FileName("../test/files/mono.wav"));
		ASSERT_TRUE(input.IsFile());
		EXPECT_TRUE(LameEncoder(input, output).Encode());
	}

	void EncodeStereoFile()
	{
		FileName input(FileName("../test/files/stereo.wav"));
		ASSERT_TRUE(input.IsFile());
		EXPECT_TRUE(LameEncoder(input, output).Encode());
	}

	void EncodeInvalidFile()
	{
		FileName input(FileName("../test/files/invalid.wav"));
		ASSERT_TRUE(input.IsFile());
		EXPECT_FALSE(LameEncoder(input, output).Encode());
	}

	void EncodeDirectory()
	{
		FileName input(FileName("../test/files/dir.wav"));
		ASSERT_TRUE(input.IsDirectory());
		EXPECT_FALSE(LameEncoder(input, output).Encode());
	}

} LamEncoderTest;
