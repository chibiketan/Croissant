#include "Debug/MemoryManager.hpp"
#include "Core/LogManager.hpp"
#include "FileSystem/Directory.hpp"
#include "Threading/Thread.hpp"

#include <thread>

namespace Croissant
{
	namespace Core
	{
		class LogManager::Pimpl
		{
		public:
			Pimpl() : m_run{ false }, m_init{ false }, m_exitRequested{ false }, m_shuttedDown{ false }
				, m_logs{}, m_event{}, m_thread{ nullptr }, m_mutex{} {}

			bool m_run;
			bool m_init;
			bool m_exitRequested;
			bool m_shuttedDown;
			std::map<std::string, std::unique_ptr<LogFile>> m_logs;
			Threading::AutoResetEvent m_event;
			std::unique_ptr<Threading::Thread> m_thread;
			std::mutex m_mutex;
		};

		class LogManager::LogEntry::Pimpl
		{
		public:
			Pimpl(std::string const& moduleName, std::string const& message) : m_message{ message }
				, m_moduleName{ moduleName }, m_time{ time(nullptr) } {}

			const std::string m_message;
			const std::string m_moduleName;
			time_t m_time;
		};

		class LogManager::LogFile::Pimpl
		{
		public:
			explicit Pimpl(std::string const& fileName ) : m_fileName{ fileName }, m_output() , m_mutex{}
				, m_file{ fileName + ".log", FileSystem::Directory(FileSystem::DEFAULT_DIRECTORY::PROGRAM_DIRECTORY).Child("Logs") }
				, m_queue{}
			{}

			std::string m_fileName;
			std::ofstream m_output;
			std::mutex m_mutex;
			FileSystem::File m_file;
			std::queue<LogEntry> m_queue;
		};

		class LogManager::Log::Pimpl
		{
		public:
			Pimpl(std::string const& className, LogFile& logFile) : m_className{ className }, m_logFile{ logFile } {}

			std::string m_className;
			LogFile&	m_logFile;
		};

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

		// ------------------------------- LogManager::LogEntry Implémentation
		LogManager::LogEntry::LogEntry(const std::string& moduleName, const std::string& message)
			: m_pimpl{ CROISSANT_NEW Pimpl{moduleName, message} }
		{
		}

		LogManager::LogEntry::~LogEntry()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		std::string const& LogManager::LogEntry::GetMessage() const
		{
			return m_pimpl->m_message;
		}

		std::string const& LogManager::LogEntry::GetModuleName() const
		{
			return m_pimpl->m_moduleName;
		}

		// --------------------------------------------- LogManager::Log implementation

		LogManager::Log::Log(LogManager::LogFile& logFile, std::string const& className)
			: m_pimpl{ CROISSANT_NEW Pimpl{ className, logFile } }
		{
		}

		LogManager::Log::Log(const Log& right)
			: m_pimpl{ CROISSANT_NEW Pimpl{ *(right.m_pimpl) }}
		{
		}

		LogManager::Log::~Log()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		void LogManager::Log::Write(std::string const& message) const
		{
			m_pimpl->m_logFile.Add(m_pimpl->m_className, message);
		}

		// --------------------------------------------- LogManager::LogFile
		LogManager::LogFile::LogFile(std::string const& fileName)
			: m_pimpl{ CROISSANT_NEW Pimpl{ fileName } }
		{
			if (!m_pimpl->m_file.Exist())
			{
				m_pimpl->m_file.Parent().Create(true);
			}

			m_pimpl->m_output.open(m_pimpl->m_file.FullPath(), std::ios::out | std::ios::trunc);
		}

		LogManager::LogFile::~LogFile()
		{
			if (m_pimpl != nullptr)
			{
				Flush();
				m_pimpl->m_output.close();
			}
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		void LogManager::LogFile::Add(std::string const& moduleName, std::string const& message)
		{
			std::lock_guard<std::mutex> lock(m_pimpl->m_mutex); // thread sync
			m_pimpl->m_queue.emplace(moduleName, message);
		}

		void LogManager::LogFile::Flush()
		{
			// on échange la liste des éléments avec une liste vide pour pouvoir la traiter tranquile (plante sous VS2015 sinon)
			std::queue<LogEntry> tmpQueue;

			{
				std::lock_guard<std::mutex> lock(m_pimpl->m_mutex);
				// thread safe here
				m_pimpl->m_queue.swap(tmpQueue);
			}

			while (!tmpQueue.empty())
			{
				auto& entry = tmpQueue.front();

				m_pimpl->m_output << entry.GetModuleName() << " : " << entry.GetMessage() << "\n";

				tmpQueue.pop();
			}

			m_pimpl->m_output.flush();
		}

		// --------------------------------------------- LogManager::Impl implementation

		bool LogManager::Init()
		{
			if (nullptr != m_pimpl)
			{
				return false;
			}

			m_pimpl = CROISSANT_NEW Pimpl{};

			// register atexist to shutdown logger
			m_pimpl->m_init = true;
			m_pimpl->m_run = true;
			atexit([]() { Shutdown(); });

			m_pimpl->m_thread = std::make_unique<Threading::Thread>([]() { ThreadEntry(); return 0; });

			auto log = GetLog("LogManager");
			log.Write("Démarrage du LogManager");
			m_pimpl->m_event.Signal();
			return true;
		}

		void LogManager::Shutdown()
		{
			if (nullptr == m_pimpl)
			{
				return;
			}

			auto log = GetLog("LogManager");

			log.Write("Arrêt du LogManager");
			//m_pimpl->m_shuttedDown = false;
			m_pimpl->m_exitRequested = true;
			m_pimpl->m_init = false;
			m_pimpl->m_run = false;
			m_pimpl->m_event.Signal();

			while (!m_pimpl->m_shuttedDown)
			{
				m_pimpl->m_event.Signal(); // permet de s'assurer que le thread va sarrêter
										  // spin waiting pour la fin du thread d'écriture
			}

			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		LogManager::Log LogManager::GetLog(std::string const& className, bool useDefaultFileName)
		{
			auto fileName = useDefaultFileName ? std::string("Default") : className;
			auto el = m_pimpl->m_logs.find(fileName);

			if (m_pimpl->m_logs.end() != el)
			{
				return Log{ *(el->second), className };
			}

			{
				std::lock_guard<std::mutex> guard{ m_pimpl->m_mutex };
				el = m_pimpl->m_logs.find(fileName);

				if (m_pimpl->m_logs.end() != el)
				{
					return Log{ *(el->second), className };
				}

				auto log = std::make_unique<LogFile>(fileName);

				m_pimpl->m_logs.insert(std::make_pair(fileName, std::move(log)));
				return Log{ *(m_pimpl->m_logs[fileName]), className };
			}
		}

		int LogManager::ThreadEntry()
		{
			while (!m_pimpl->m_exitRequested)
			{
				m_pimpl->m_event.Wait();
				while (m_pimpl->m_run)
				{
					m_pimpl->m_event.Wait(1000);
					auto begin = m_pimpl->m_logs.begin();
					auto end = m_pimpl->m_logs.end();

					for (; begin != end; ++begin)
					{
						auto& log = begin->second;

						log->Flush();
					}
				}

				m_pimpl->m_logs.clear();
			}

			m_pimpl->m_shuttedDown = true;
			return 0;
		}

		LogManager::Pimpl*	LogManager::m_pimpl = nullptr;

	}
}




