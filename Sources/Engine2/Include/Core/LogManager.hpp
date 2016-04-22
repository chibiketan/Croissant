#ifndef LOGMANAGER_HPP_INC
#define LOGMANAGER_HPP_INC

#include "Engine.hpp"
#include "FileSystem/File.hpp"
#include "Threading/Thread.hpp"
#include "Threading/AutoResetEvent.hpp"
#include <memory>
#include <string>
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
			static std::shared_ptr<Log> GetLog(std::string const& className, bool useDefaultFileName = true);

		private:
			static int ThreadEntry();

			static bool m_run ;
			static bool m_init;
			static bool m_exitRequested;
			static bool m_shuttedDown;
			static std::map<std::string, std::shared_ptr<Log>> m_logs;
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

		public:
			class 	Log
			{
			public:
				Log(std::string const& className, std::string const& fileName);
				Log(const Log&) = delete;
				Log(Log&& ref);
				~Log();
				void Write(std::string const& message);
				void Write(const std::string& moduleName, const std::string& message);
				void Flush();

			private:
				std::string m_className;
				std::string m_fileName;
				std::queue<LogEntry> m_queue;
				Croissant::FileSystem::File m_file;
				std::ofstream m_output;
				std::mutex m_mutex;
			};

		};

	} // !namespace Core
} // !namespace Croissant


#  define CROISSANT_GET_LOG(clazz) Croissant::Core::LogManager::GetLog(#clazz)
#  define CROISSANT_GET_LOG_WITH_FILE(clazz) Croissant::Core::LogManager::GetLog(#clazz, false)

#endif
