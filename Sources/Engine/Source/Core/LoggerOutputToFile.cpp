#include "stdafx.hpp"
#include "Core/LoggerOutputToFile.hpp"
#include <fstream>
#include "FileSystem/OFStream.hpp"

using OFStream = Croissant::FileSystem::OFStream;

namespace Croissant
{
	namespace Core
	{
	    template<>
		class Pimpl<LoggerOutputToFile>::Implementation
		{
		public:
			Implementation(LoggerOutputToFile* const ref, const String& filename);
			void Write(const String& data);

		private:
			OFStream m_file;
			LoggerOutputToFile* const m_ref;
		};

		Pimpl<LoggerOutputToFile>::Implementation::Implementation(LoggerOutputToFile* const ref, const String& filename)
            : m_file(filename, std::ios_base::out | std::ios_base::trunc), m_ref(ref)
		{
			std::ios_base::iostate state = m_file.rdstate();

			if ((state & std::ios_base::badbit) == std::ios_base::badbit || (state & std::ios_base::failbit) == std::ios_base::failbit)
			{
			    m_file.close();
			}
		}

		void Pimpl<LoggerOutputToFile>::Implementation::Write( const String& data )
		{
		    if (!m_file.is_open())
			{
				return;
			}

			m_file << data;
			m_file.flush();
		}


		LoggerOutputToFile::LoggerOutputToFile( const String& filename )
			: Pimpl<LoggerOutputToFile>::Detail(filename)
		{
		}

		void LoggerOutputToFile::Write( const String& data )
		{
			Pimpl<LoggerOutputToFile>::Implementation& imp = **this;

			imp.Write(data);
		}

	}
}
