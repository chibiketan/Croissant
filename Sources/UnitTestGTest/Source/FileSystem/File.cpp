#include "Debug/MemoryManager.hpp"
#include "FileSystem/File.hpp"
#include "FileSystem/Directory.hpp"
#include "gtest/gtest.h"

#if defined(CROISSANT_LINUX)
#  define FILE_EXIST_FULLPATH "/usr/bin/ssh"
#  define FILE_EXIST_NAME "ssh"
#  define FILE_EXIST_EXTENSION ""
#  define FILE_NOT_EXIST_FULLPATH "/bin/not_exist.com"
#  define FILE_NOT_EXIST_NAME "not_exist"
#  define FILE_NOT_EXIST_EXTENSION "com"
#  define SEPARATOR "/"
#else
#  define FILE_EXIST_FULLPATH "C:\\Windows\\System32\\cmd.exe"
#  define FILE_EXIST_NAME "cmd"
#  define FILE_EXIST_EXTENSION "exe"
#  define FILE_NOT_EXIST_FULLPATH "C:\\Windows\\System32\\cmd64.exe"
#  define FILE_NOT_EXIST_NAME "cmd64"
#  define FILE_NOT_EXIST_EXTENSION "exe"
#  define SEPARATOR "\\"
#endif

class FileTest : public ::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

protected:

};

TEST_F(FileTest, LoadFileWithAbsolutePath)
{
	Croissant::FileSystem::File file(FILE_EXIST_FULLPATH);

	ASSERT_TRUE(file.Exist());
	ASSERT_STREQ(FILE_EXIST_FULLPATH, file.FullPath().c_str());
	ASSERT_STREQ(FILE_EXIST_NAME, file.Name().c_str());
	ASSERT_STREQ(FILE_EXIST_EXTENSION, file.Extension().c_str());
}

TEST_F(FileTest, LoadFileWithAbsolutePathOnInvalidFile)
{
	Croissant::FileSystem::File file(FILE_NOT_EXIST_FULLPATH);

	ASSERT_FALSE(file.Exist());
	ASSERT_STREQ(FILE_NOT_EXIST_FULLPATH, file.FullPath().c_str());
	ASSERT_STREQ(FILE_NOT_EXIST_NAME, file.Name().c_str());
	ASSERT_STREQ(FILE_NOT_EXIST_EXTENSION, file.Extension().c_str());
}

TEST_F(FileTest, LoadFileWithRelativePath)
{
	Croissant::FileSystem::File file("Logs/MemoryLeak.log");
	Croissant::FileSystem::Directory dir = Croissant::FileSystem::Directory(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY).Child("Logs");

	ASSERT_TRUE(file.Exist());
	ASSERT_EQ(dir.FullPath() + SEPARATOR "MemoryLeak.log", file.FullPath());
	ASSERT_STREQ("MemoryLeak", file.Name().c_str());
	ASSERT_STREQ("log", file.Extension().c_str());
}

TEST_F(FileTest, GetParentFolder)
{
	Croissant::FileSystem::File file("main.cpp");
	Croissant::FileSystem::Directory dir(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY);

	ASSERT_EQ(dir, file.Parent());
}

