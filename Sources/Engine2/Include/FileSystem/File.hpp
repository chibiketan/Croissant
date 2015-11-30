#pragma once
#ifndef FILE_H_
#define FILE_H_

#include "Engine.hpp"
#include <string>

namespace Croissant
{
	namespace FileSystem
	{
		class Directory;

		class ENGINE_API File final
		{
		public:
			File(const std::string& path);
			File(const std::string& path, const Directory& parent);
			File(const File& ) = delete;
			File(File&&) = default;
			~File();
			bool Exist() const;
			const std::string& Name() const;
			const std::string& Extension() const;
			const std::string& FullPath() const;
			Directory Parent() const;
			void Refresh();

		private:
			std::string m_fullPath;
			std::string m_name;
			std::string m_extension;
			bool m_exist;
		};
	}
}

#endif
