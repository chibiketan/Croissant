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

		// ------------------------------- LogManager::LogEntry Implémentation
		LogManager::LogEntry::LogEntry(const std::string& moduleName, const std::string& message)
			: m_message { message }, m_moduleName { moduleName }, m_time { time(nullptr) }
		{
		}

		// --------------------------------------------- LogManager::Log implementation

		LogManager::Log::Log(std::string fileName)
			: m_fileName { fileName }, m_queue()
			, m_file{ fileName + ".log", Croissant::FileSystem::Directory(Croissant::FileSystem::DEFAULT_DIRECTORY::PROGRAM_DIRECTORY).Child("Logs") }
			, m_output(), m_mutex { }
		{
			if (!m_file.Exist())
			{
				m_file.Parent().Create(true);
			}

			m_output.open(m_file.FullPath(), std::ios::out | std::ios::trunc);
		}

		LogManager::Log::Log(Log&& ref)
			: m_fileName { std::move(ref.m_fileName) }, m_queue { std::move(ref.m_queue) }
			, m_file { std::move(ref.m_file) }, m_output()
		{
			ref.m_output.close();
			m_output.open(m_file.FullPath(), std::ios::out | std::ios::app);
		}

		LogManager::Log::~Log()
		{
			Flush();
			m_output.close();
		}

		void LogManager::Log::Write(const std::string& moduleName, const std::string& message)
		{
			std::lock_guard<std::mutex> lock(m_mutex); // thread sync
			m_queue.emplace(moduleName, message);
		}

		void LogManager::Log::Flush()
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

		LogManager::LogManager(std::string appName)
		:	m_run { true }
			, m_init { false }, m_exitRequested { false }, m_shuttedDown { false }, m_appName { appName },
			 m_logs {}, m_event{},
			 m_thread{ [this](){ this->ThreadEntry(); return 0; } }
			 //m_thread{ MethodBinderHelper(&LogManager::ThreadEntry, this) }
		{
		}

		LogManager::~LogManager()
		{
			m_shuttedDown = false;
			m_exitRequested = true;
			Shutdown();
			while (!m_shuttedDown)
			{
				m_event.Signal(); // permet de s'assurer que le thread va sarrêter
				// spin waiting pour la fin du thread d'écriture
			}
		}

		bool LogManager::Init()
		{
			if (true == m_init)
			{
				return false;
			}

			m_logs.emplace(m_appName, m_appName);
			m_init = true;
			m_run = true;
			Write(m_appName, "Démarrage du LogManager");
			m_event.Signal();
			return true;
		}

		void LogManager::Shutdown()
		{
			if (false == m_init)
			{
				return;
			}

			Write(m_appName, "Arrêt du LogManager");
			m_init = false;
			m_run = false;
			m_event.Signal();

		}

		void LogManager::Write(const std::string& message)
		{
			Write(m_appName, message);
		}

		void LogManager::Write(const std::string& moduleName, const std::string& message)
		{
			auto end = m_logs.end();
			auto f = m_logs.find(moduleName);

			if (f == end)
			{
				f = m_logs.find(m_appName);
			}

			auto& log = f->second;

			log.Write(moduleName, message);
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

						log.Flush();
					}
				}

				m_logs.clear();
			}

			m_shuttedDown = true;
			return 0;
		}

	}
}




