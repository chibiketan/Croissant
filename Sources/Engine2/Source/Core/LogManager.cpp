#include "Debug/MemoryManager.hpp"
#include "Core/LogManager.hpp"
#include "FileSystem/Directory.hpp"
#include "Threading/Thread.hpp"

#include <thread>

namespace Croissant
{
	namespace Core
	{
		class MethodBinderHelper
		{
		public:
			using callback = int (LogManager::*)();

			MethodBinderHelper(callback func, LogManager* obj)
				: m_this { obj }, m_func { func }
			{

			}
			int operator()(){
				return (m_this->*m_func)();
			}

		private:
			LogManager*	m_this;
			callback	m_func;
		};

		bool LogManager::m_run = false;
		bool LogManager::m_init = false;
		bool LogManager::m_exitRequested = false;
		bool LogManager::m_shuttedDown = false;
		std::map<std::string, std::unique_ptr<LogManager::LogFile>> LogManager::m_logs {};
		Threading::AutoResetEvent LogManager::m_event{};
		std::unique_ptr<Threading::Thread> LogManager::m_thread{};
		std::mutex LogManager::m_mutex{};


		// ------------------------------- LogManager::LogEntry Implémentation
		LogManager::LogEntry::LogEntry(const std::string& moduleName, const std::string& message)
			: m_message { message }, m_moduleName { moduleName }, m_time { time(nullptr) }
		{
		}

		// --------------------------------------------- LogManager::Log implementation

		LogManager::Log::Log(LogManager::LogFile& logFile, std::string const& className)
			: m_className { className }, m_logFile { logFile }
		{
		}

		void LogManager::Log::Write(std::string const& message) const
		{
			m_logFile.Add(m_className, message);
		}

		// --------------------------------------------- LogManager::LogFile
		LogManager::LogFile::LogFile(std::string const& fileName)
			: m_fileName{ fileName }, m_output()
			, m_mutex{}
			, m_file{ fileName + ".log", FileSystem::Directory(FileSystem::DEFAULT_DIRECTORY::PROGRAM_DIRECTORY).Child("Logs") }
			, m_queue {}
		{
			if (!m_file.Exist())
			{
				m_file.Parent().Create(true);
			}

			m_output.open(m_file.FullPath(), std::ios::out | std::ios::trunc);
		}

		LogManager::LogFile::~LogFile()
		{
			Flush();
			m_output.close();
		}

		void LogManager::LogFile::Add(std::string const& moduleName, std::string const& message)
		{
			std::lock_guard<std::mutex> lock(m_mutex); // thread sync
			m_queue.emplace(moduleName, message);
		}

		void LogManager::LogFile::Flush()
		{
			// on échange la liste des éléments avec une liste vide pour pouvoir la traiter tranquile (plante sous VS2015 sinon)
			std::queue<LogEntry> tmpQueue;

			{
				std::lock_guard<std::mutex> lock(m_mutex);
				// thread safe here
				m_queue.swap(tmpQueue);
			}

			while (!tmpQueue.empty())
			{
				auto& entry = tmpQueue.front();

				m_output << entry.m_moduleName << " : " << entry.m_message << "\n";

				tmpQueue.pop();
			}

			m_output.flush();
		}

		// --------------------------------------------- LogManager::Impl implementation

		bool LogManager::Init()
		{
			if (true == m_init)
			{
				return false;
			}

			// register atexist to shutdown logger
			m_init = true;
			m_run = true;
			atexit([]() { Shutdown(); });

			m_thread = std::make_unique<Threading::Thread>([]() { ThreadEntry(); return 0; });

			auto log = GetLog("LogManager");
			log.Write("Démarrage du LogManager");
			m_event.Signal();
			return true;
		}

		void LogManager::Shutdown()
		{
			if (false == m_init)
			{
				return;
			}

			auto log = GetLog("LogManager");

			log.Write("Arrêt du LogManager");
			m_shuttedDown = false;
			m_exitRequested = true;
			m_init = false;
			m_run = false;
			m_event.Signal();

			while (!m_shuttedDown)
			{
				m_event.Signal(); // permet de s'assurer que le thread va sarrêter
								  // spin waiting pour la fin du thread d'écriture
			}

		}

		LogManager::Log LogManager::GetLog(std::string const& className, bool useDefaultFileName)
		{
			auto fileName = useDefaultFileName ? std::string("Default") : className;
			auto el = m_logs.find(fileName);

			if (m_logs.end() != el)
			{
				return Log{ *(el->second), className };
			}

			{
				std::lock_guard<std::mutex> guard{ m_mutex };
				el = m_logs.find(fileName);

				if (m_logs.end() != el)
				{
					return Log{ *(el->second), className };
				}

				auto log = std::make_unique<LogFile>(fileName);

				m_logs.insert(std::make_pair(fileName, std::move(log)));
				return Log{ *(m_logs[fileName]), className };
			}
		}

		int LogManager::ThreadEntry()
		{
			while (!m_exitRequested)
			{
				m_event.Wait();
				while (m_run)
				{
					m_event.Wait(1000);
					auto begin = m_logs.begin();
					auto end = m_logs.end();

					for (; begin != end; ++begin)
					{
						auto& log = begin->second;

						log->Flush();
					}
				}

				m_logs.clear();
			}

			m_shuttedDown = true;
			return 0;
		}

	}
}




