#include "Debug/MemoryManager.hpp"
#include "FileSystem/Directory.hpp"
#include "FileSystem/File.hpp"
#include "gtest/gtest.h"

#if defined(CROISSANT_LINUX)
#  define DIRECTORY_EXISTING "/lib"
#  define DIRECTORY_EXISTING_PARENT "/"
#  define DIRECTORY_EXISTING_WITH_TRAILING_SEPARATOR "/lib/"
#  define DIRECTORY_EXISTING_WITH_MULTIPLE_SEPARATOR "//lib"
#  define DIRECTORY_NAME "lib"
#  define DIRECTORY_ROOT "/"
#  define DIRECTORY_ROOT_NAME ""
#  define SEPARATOR "/"
#  define GET_ROOT(dir) String()
#else
#  define DIRECTORY_EXISTING "C:\\Windows\\System32"
#  define DIRECTORY_EXISTING_PARENT "C:\\Windows"
#  define DIRECTORY_EXISTING_WITH_TRAILING_SEPARATOR "C:\\Windows\\System32\\"
#  define DIRECTORY_EXISTING_WITH_MULTIPLE_SEPARATOR "C:\\\\Windows\\\\System32\\\\"
#  define DIRECTORY_NAME "System32"
#  define DIRECTORY_ROOT "C:\\"
#  define DIRECTORY_ROOT_NAME ""
#  define SEPARATOR "\\"
#  define GET_ROOT(dir) dir.FullPath().substr(0, 2)
#endif

class DirectoryTest : public ::testing::Test
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


TEST_F(DirectoryTest, LoadCurrentDirectory)
{
	Croissant::FileSystem::Directory dir(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY);
	char* path;
	path = getcwd(nullptr, 0);

	ASSERT_STREQ(path, dir.FullPath().c_str());
	free(path);
}

TEST_F(DirectoryTest, LoadExecutableDirectory)
{
	Croissant::FileSystem::Directory dir(Croissant::FileSystem::DEFAULT_DIRECTORY::PROGRAM_DIRECTORY);
	ASSERT_STRNE("", dir.FullPath().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryExistOnCurrentDirectory)
{
	Croissant::FileSystem::Directory dir(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY);
	ASSERT_TRUE(dir.Exist());
}

TEST_F(DirectoryTest, CheckDirectoryExistOnInvalidDirectory)
{
	Croissant::FileSystem::Directory dir("C:\\I_do_not_exist");
	ASSERT_FALSE(dir.Exist());
}

TEST_F(DirectoryTest, CheckDirectoryExistOnSpecificDirectoryWithoutTrailingSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING);
	ASSERT_TRUE(dir.Exist());
}

TEST_F(DirectoryTest, CheckDirectoryExistOnSpecificDirectoryWithTrailingSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING_WITH_TRAILING_SEPARATOR);
	ASSERT_TRUE(dir.Exist());
}

TEST_F(DirectoryTest, CheckDirectoryExistOnSpecificDirectoryWithMultipleSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING_WITH_MULTIPLE_SEPARATOR);
	ASSERT_TRUE(dir.Exist());
}

TEST_F(DirectoryTest, CheckDirectoryNameOnSpecificDirectoryWithTrailingSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING_WITH_TRAILING_SEPARATOR);
	ASSERT_STREQ(DIRECTORY_NAME, dir.Name().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryNameOnSpecificDirectoryWithoutTrailingSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING);
	ASSERT_STREQ(DIRECTORY_NAME, dir.Name().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryFullPathOnSpecificDirectoryWithTrailingSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING_WITH_TRAILING_SEPARATOR);
	ASSERT_STREQ(DIRECTORY_EXISTING, dir.FullPath().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryFullPathOnSpecificDirectoryWithMultipleSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING_WITH_MULTIPLE_SEPARATOR);
	ASSERT_STREQ(DIRECTORY_EXISTING, dir.FullPath().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryFullPathOnSpecificDirectoryWithoutTrailingSeparator)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING);
	ASSERT_STREQ(DIRECTORY_EXISTING, dir.FullPath().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryFullPathOnshortRelativeDirectory)
{
	Croissant::FileSystem::Directory dir("a");
	Croissant::FileSystem::Directory currentDir(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY);

	ASSERT_STREQ((currentDir.FullPath() + SEPARATOR "a").c_str(), dir.FullPath().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryFullPathOnshortAbsoluteDirectoryWithoutLetter)
{
	Croissant::FileSystem::Directory dir("/a");
	Croissant::FileSystem::Directory currentDir(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY);

	ASSERT_STREQ((GET_ROOT(currentDir) + SEPARATOR "a").c_str(), dir.FullPath().c_str());
}

TEST_F(DirectoryTest, CheckParentFullPath)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_EXISTING);
	ASSERT_STREQ(DIRECTORY_EXISTING_PARENT, dir.Parent().FullPath().c_str());
}

TEST_F(DirectoryTest, CheckDirectoryEgalityOnFullPathEquality)
{
	Croissant::FileSystem::Directory dir1(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY);
	Croissant::FileSystem::Directory dir2("test");
	Croissant::FileSystem::Directory parent = dir2.Parent();


	ASSERT_EQ(dir1.FullPath(), parent.FullPath());
	ASSERT_EQ(dir1, parent);
}

TEST_F(DirectoryTest, CheckFullPathOfRootDrive)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_ROOT);

	ASSERT_STREQ(DIRECTORY_ROOT, dir.FullPath().c_str());
}

TEST_F(DirectoryTest, CheckParentFromRootIsSamePath)
{
	Croissant::FileSystem::Directory dir(DIRECTORY_ROOT);
	Croissant::FileSystem::Directory parent = dir.Parent();

	ASSERT_EQ(dir.FullPath(), parent.FullPath());
	ASSERT_EQ(dir, parent);
}

TEST_F(DirectoryTest, CheckIfCanGetAllFilesFromDirectory)
{
	Croissant::FileSystem::Directory dir(Croissant::FileSystem::DEFAULT_DIRECTORY::CURRENT_DIRECTORY);
	{
		auto fileList = dir.Files();

		ASSERT_NE(0UL, fileList.size());
		auto begin = fileList.begin();
		auto end = fileList.end();

		for (; begin != end; ++begin)
		{
			ASSERT_TRUE((*begin).Exist());
		}
	}
}

TEST_F(DirectoryTest, CheckIfCanSearchFilesFromDirectory)
{
	Croissant::FileSystem::Directory dir(Croissant::FileSystem::DEFAULT_DIRECTORY::PROGRAM_DIRECTORY);
	Croissant::FileSystem::Directory::Filelist fileList = dir.Parent().Parent().Child("Sources").Child("Engine2").Child("Source").Child("Core").Search("*.cpp");

	ASSERT_NE(0UL, fileList.size());
	auto begin = fileList.begin();
	auto end = fileList.end();

	for (; begin != end; ++begin)
	{
		ASSERT_TRUE((*begin).Exist());
		ASSERT_STREQ("cpp", (*begin).Extension().c_str());
	}
}

