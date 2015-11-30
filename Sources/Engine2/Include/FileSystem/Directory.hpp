#pragma once
#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "Engine.hpp"
#include <memory>
#include <list>
#include <string>

namespace Croissant
{
	namespace FileSystem
	{
		class File;

		enum class DEFAULT_DIRECTORY
		{
			CURRENT_DIRECTORY,
			PROGRAM_DIRECTORY
		};

		class ENGINE_API Directory final
		{
		public:
			typedef std::list<File> Filelist;

			Directory(DEFAULT_DIRECTORY mode = DEFAULT_DIRECTORY::CURRENT_DIRECTORY);
			Directory(const std::string& path);
			Directory(Directory&& source);
			~Directory();
			const std::string& FullPath() const;
			const std::string& Name() const;
			bool Exist() const;
			Directory Parent() const;
			Directory Child(const std::string& name) const;
			Filelist Files() const;
			Filelist Search(const std::string& pattern) const;
			bool Create(bool createParents = false);
			void Refresh();

			Directory& operator=(Directory&& source);
			bool operator==(const Directory& source) const;

			static std::string NormalizePath(const std::string& path);
			static std::string GetName(const std::string& path);
		private:

			class Impl;
			friend Impl;
			friend File;

			Directory(const std::string& path, bool needParent);

			std::unique_ptr<Impl> m_impl;
		};
	}
}

#endif
