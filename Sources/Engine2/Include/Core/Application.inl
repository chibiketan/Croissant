#include "Core/Application.hpp"
#include "Core/LogManager.hpp"
#include <chrono>

namespace Croissant
{
	namespace Core
	{


		template <typename App>
		Application<App>::Application(std::string const& name)
			: m_name { name }, m_logManager { CROISSANT_GET_LOG(Application) }
		{
		}

		template<typename App>
		bool Application<App>::Initialize()
		{
			m_lastFrameTime = Clock::now();
			
			if (!FunctionOnInitialize<App>::Execute(*this, *this))
			{
				return false;
			}

			return true;
		}

		template <typename App>
		void Application<App>::Shutdown()
		{
			FunctionOnShutdown<App>::Execute(*this, *this);
		}

		template <typename App>
		int32_t Application<App>::Run()
		{
			FrameTime currentFrame = Clock::now();
			int32_t result = 0;
			while (FunctionOnFrame<App>::Execute(*this, result, m_lastFrameTime, currentFrame))
			{
				m_lastFrameTime = currentFrame;
				currentFrame = Clock::now();
			}

			return result;
		}

		template <typename App>
		std::shared_ptr<LogManager::Log> Application<App>::GetLogManager() const
		{
			return m_logManager;
		}
	}
}