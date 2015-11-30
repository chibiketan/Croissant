#pragma once
#ifndef FILE_H_
#define FILE_H_

#include "Engine.hpp"
#include "Core/NonCopyable.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace FileSystem
	{
		class Directory;

		class ENGINE_API File : private Croissant::Core::NonCopyable
		{
		public:
			File(const String& path);
			File(const String& path, const Directory& parent);
			File(File&& source);
			~File();
			bool Exist() const;
			const String& Name() const;
			const String& Extension() const;
			const String& FullPath() const;
			Directory Parent() const;

		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}

#endif
