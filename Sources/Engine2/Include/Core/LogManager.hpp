#ifndef LOGMANAGER_HPP_INC
#define LOGMANAGER_HPP_INC

#include "Engine.hpp"
#include "FileSystem/File.hpp"
#include "Threading/Thread.hpp"
#include "Threading/AutoResetEvent.hpp"
#include <memory>
#include <queue>
#include <map>
#include <fstream>
#include <mutex>

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API LogManager final
		{
		public:
			class Log;

			LogManager() = delete;
			LogManager(LogManager const&) = delete;
			LogManager(LogManager&&) = delete;
			LogManager& operator=(LogManager const&) = delete;
			LogManager& operator=(LogManager&&) = delete;

			static bool Init();
			static void Shutdown();
			static Log GetLog(std::string const& className, bool useDefaultFileName = true);

		private:
			class LogFile;
			class Pimpl;
			static int ThreadEntry();

			static Pimpl*	m_pimpl;

			class ENGINE_API LogEntry final
			{
			public:

				LogEntry(const std::string& moduleName, const std::string& message);
				LogEntry(const LogEntry&) = delete;
				LogEntry(LogEntry&&) = delete;
				~LogEntry();
				std::string const&	GetMessage() const;
				std::string const&	GetModuleName() const;

			private:
				class Pimpl;

				Pimpl*	m_pimpl;
			};

			class ENGINE_API LogFile final
			{
			public:
				explicit LogFile(std::string const& fileName);
				~LogFile();
				void Add(std::string const& moduleName, std::string const& message);
				void Flush();

			private:
				class Pimpl;

				Pimpl*	m_pimpl;
			};

		public:
			class ENGINE_API Log final
			{
			public:
				Log(LogFile& logFile, std::string const& className);
				Log(const Log&);
				Log(Log&& ref) = default;
				~Log();
				void Write(std::string const& message) const;

			private:
				class Pimpl;

				Pimpl*	m_pimpl;
			};

		};

#  if defined(CROISSANT_HAS_TRACE)
		inline void WriteTrace(std::string const& , std::string const& msg)
		{
			LogManager::GetLog("Trace", false).Write(msg);
		}

		inline void WriteTraceDebug(std::string const& msg)
		{
#    if CROISSANT_TRACE_LEVEL >= 3
			WriteTrace("Debug", msg);
#    endif
		}

		inline void WriteTraceInfo(std::string const& msg)
		{
#    if CROISSANT_TRACE_LEVEL >= 2
			WriteTrace("Info", msg);
#    endif
		}
		
		inline void WriteTraceWarn(std::string const& msg)
		{
#    if CROISSANT_TRACE_LEVEL >= 1
			WriteTrace("Warn", msg);
#    endif
		}

		inline void WriteTraceError(std::string const& msg)
		{
#    if CROISSANT_TRACE_LEVEL >= 0
			WriteTrace("Error", msg);
#    endif
		}
#  else // !defined(CROISSANT_HAS_TRACE)
		inline void WriteTrace(std::string const& , std::string const& )
		{
		}

		inline void WriteTraceDebug(std::string const& )
		{
		}

		inline void WriteTraceInfo(std::string const& )
		{
		}

		inline void WriteTraceWarn(std::string const& )
		{
		}

		inline void WriteTraceError(std::string const& )
		{
		}
#  endif


	} // !namespace Core
} // !namespace Croissant


#  define CROISSANT_GET_LOG(clazz) Croissant::Core::LogManager::GetLog(#clazz)
#  define CROISSANT_GET_LOG_WITH_FILE(clazz) Croissant::Core::LogManager::GetLog(#clazz, false)

#endif
