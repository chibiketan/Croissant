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
			LogManager(LogManager const&) = delete;
			LogManager(LogManager&&) = delete;
			LogManager& operator=(LogManager const&) = delete;
			LogManager& operator=(LogManager&&) = delete;

			LogManager(std::string appName);
			~LogManager();
			bool Init();
			void Shutdown();
			void Write(const std::string& message);
			void Write(const std::string& moduleName, const std::string& message);

		private:
			class Log;
			int ThreadEntry();

			bool m_run;
			bool m_init;
			bool m_exitRequested;
			bool m_shuttedDown;
			std::string m_appName;
			std::map<std::string, Log> m_logs;
			Croissant::Threading::AutoResetEvent m_event;
			Croissant::Threading::Thread m_thread;


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

			class Log
			{
			public:
				Log(std::string fileName);
				Log(const Log&) = delete;
				Log(Log&& ref);
				~Log();
				void Write(const std::string& moduleName, const std::string& message);
				void Flush();

			private:
				std::string m_fileName;
				std::queue<LogEntry> m_queue;
				Croissant::FileSystem::File m_file;
				std::ofstream m_output;
				std::mutex m_mutex;
			};
		};
	} // !namespace Core
} // !namespace Croissant

#endif
