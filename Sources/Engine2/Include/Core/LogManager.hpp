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
			static int ThreadEntry();

			static bool m_run ;
			static bool m_init;
			static bool m_exitRequested;
			static bool m_shuttedDown;
			static std::map<std::string, std::unique_ptr<LogFile>> m_logs;
			static Croissant::Threading::AutoResetEvent m_event;
			static std::unique_ptr<Threading::Thread> m_thread;
			static std::mutex m_mutex;

			class LogEntry
			{
			public:

				LogEntry(const std::string& moduleName, const std::string& message);
				LogEntry(const LogEntry&) = delete;
				LogEntry(LogEntry&&) = delete;

				const std::string m_message;
				const std::string m_moduleName;
				time_t m_time;
			};

			class LogFile final
			{
			public:
				explicit LogFile(std::string const& fileName);
				~LogFile();
				void Add(std::string const& moduleName, std::string const& message);
				void Flush();

			private:
				std::string m_fileName;
				std::ofstream m_output;
				std::mutex m_mutex;
				FileSystem::File m_file;
				std::queue<LogEntry> m_queue;
			};

		public:
			class 	Log
			{
			public:
				Log(LogFile& logFile, std::string const& className);
				Log(const Log&) = default;
				Log(Log&& ref) = default;
				void Write(std::string const& message) const;

			private:
				std::string m_className;
				LogFile&	m_logFile;
			};

		};

#  if defined(CROISSANT_HAS_TRACE)
		inline void WriteTrace(std::string const& lvl, std::string const& msg)
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
		inline void WriteTrace(std::string const& lvl, std::string const& msg)
		{
		}

		inline void WriteTraceDebug(std::string const& msg)
		{
		}

		inline void WriteTraceInfo(std::string const& msg)
		{
		}

		inline void WriteTraceWarn(std::string const& msg)
		{
		}

		inline void WriteTraceError(std::string const& msg)
		{
		}
#  endif


	} // !namespace Core
} // !namespace Croissant


#  define CROISSANT_GET_LOG(clazz) Croissant::Core::LogManager::GetLog(#clazz)
#  define CROISSANT_GET_LOG_WITH_FILE(clazz) Croissant::Core::LogManager::GetLog(#clazz, false)

#endif
