#include "stdafx.hpp"
#include "Core/DebugMemoryManager.hpp"
#include "Core/Logger.hpp"
#include "Core/ILoggerOutput.hpp"
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

namespace Croissant
{
	namespace Core
	{
#if defined(__linux)
		inline void localtime_s(tm *result, const time_t *timep)
		{
			localtime_r(timep, result);
		}
#endif

	    template<>
		class Pimpl<Logger>::Implementation
		{
		public:
            Implementation(Logger* const ref);
			~Implementation();
			void WriteDebugLine(const String& caller, const String& line);
			void WriteWarnLine(const String& caller, const String& line);
			void WriteErrorLine(const String& caller, const String& line);
			void AddLoggerOutput(std::unique_ptr<ILoggerOutput>&& loggerOutput);

		private:
			//void WriteLine(const std::string& level, const std::string& caller, const std::string& line);
			void WriteLine(const char* level, const String& caller, const String& line);
			static char* GetCurrentDate();

			typedef std::vector<std::unique_ptr<ILoggerOutput>> LoggerOutputList;

			LoggerOutputList m_outputList;
			Logger* const m_ref;;
		};

		Pimpl<Logger>::Implementation::Implementation(Logger* const ref) : m_outputList(), m_ref(ref)
		{

		}

		Pimpl<Logger>::Implementation::~Implementation()
		{
			m_outputList.clear();
		}

		void Pimpl<Logger>::Implementation::AddLoggerOutput(std::unique_ptr<ILoggerOutput>&& loggerOutput)
		{
			m_outputList.push_back(std::move(loggerOutput));
		}

		void Pimpl<Logger>::Implementation::WriteDebugLine(const String& caller, const String& line)
		{
			WriteLine("DEBUG", caller, line);
		}

		void Pimpl<Logger>::Implementation::WriteWarnLine(const String& caller, const String& line)
		{
			WriteLine("WARNING", caller, line);
		}

		//void Logger::pimpl::WriteLine(const std::string& level, const std::string& caller, const std::string& line)
		void Pimpl<Logger>::Implementation::WriteLine(const char*, const String&, const String& line)
		{
			//std::ostringstream osstream;
			//char* date = GetCurrentDate();

			//osstream << date << " : " << std::left << std::setw(7) << level << " : " << std::setw(50) << caller.substr(0, 50) << " : " << line << std::endl;
			//::free(date);
			//std::string result = osstream.str();

			LoggerOutputList::iterator first = m_outputList.begin();
			LoggerOutputList::iterator last = m_outputList.end();

			for (; first != last; ++first)
			{
				// (*first)->Write(result);
				(*first)->Write(line);
			}
		}

		char* Pimpl<Logger>::Implementation::GetCurrentDate()
		{
			char* str = static_cast<char*>(::malloc(20 * sizeof(char)));
			time_t t = time(NULL);
			tm tt;

			str[0] = '\0';
			localtime_s(&tt, &t);
			strftime(str, 20, "%d/%m/%Y %H:%M:%S", &tt);
			return str;
		}

		// --------------------------------------------- Logger impl�mentation

		Logger::Logger() : Pimpl<Logger>::Detail()
		{
		}

		Logger::~Logger()
		{
		}

		void Logger::AddLoggerOutput(std::unique_ptr<ILoggerOutput>&& loggerOutput)
		{
			Pimpl<Logger>::Implementation& imp = **this;

			imp.AddLoggerOutput(std::move(loggerOutput));
		}

		void Logger::WriteDebugLine(const String& caller, const String& line)
		{
			Pimpl<Logger>::Implementation& imp = **this;

			imp.WriteDebugLine(caller, line);
		}

		void Logger::WriteWarnLine(const String& caller, const String& line)
		{
			Pimpl<Logger>::Implementation& imp = **this;

			imp.WriteWarnLine(caller, line);
		}
	}
}
