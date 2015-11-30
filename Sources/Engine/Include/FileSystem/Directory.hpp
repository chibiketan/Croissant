#pragma once
#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "Engine.hpp"
#include "Core/NonCopyable.hpp"
#include <memory>
#include <list>
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace FileSystem
	{
		class File;

		class ENGINE_API Directory : private Croissant::Core::NonCopyable
		{
		public:
			enum DEFAULT_DIRECTORY
			{
				CURRENT_DIRECTORY,
				PROGRAM_DIRECTORY
			};
			//typedef std::list<std::unique_ptr<File>> Filelist;
			typedef std::list<File> Filelist;

			Directory(DEFAULT_DIRECTORY mode = CURRENT_DIRECTORY);
			Directory(const String& path);
			Directory(Directory&& source);
			~Directory();
			const String& FullPath() const;
			const String& Name() const;
			bool Exist() const;
			Directory Parent() const;
			Directory Child(const String& name) const;
			Filelist Files() const;
			Filelist Search(const String& pattern) const;
			bool Create(bool createParents = false);
		    Directory& operator=(Directory&& source);
			bool operator==(const Directory& source) const;

		private:
			friend class File;

			class Pimpl;

			static String NormalizePath(const String& path);
			static String GetName(const String& path);

			Directory(Directory::Pimpl* pimpl);
			Directory(const String& path, bool doGoUp);

			Pimpl* m_pimpl;
		};
	}
}

#endif
