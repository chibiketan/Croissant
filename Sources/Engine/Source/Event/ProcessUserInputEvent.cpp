#include "stdafx.hpp"
#include "Event/ProcessUserInputEvent.hpp"

namespace Croissant
{
	namespace Event
	{
		const String s_eventName = "Game::ProcessUserInput";

		class ProcessUserInputEvent::Pimpl
		{
		public:
			Pimpl();
			~Pimpl();
		private:
		};

		// ------------------------------------ ProcessUserInputEvent::Pimpl implémentation

		ProcessUserInputEvent::Pimpl::Pimpl()
		{

		}

		ProcessUserInputEvent::Pimpl::~Pimpl()
		{
		}
		
		// ------------------------------------ ProcessUserInputEvent implémentation

		ProcessUserInputEvent::ProcessUserInputEvent()
			: Event(s_eventName), m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW ProcessUserInputEvent::Pimpl();
		}

		ProcessUserInputEvent::~ProcessUserInputEvent()
		{
			if (nullptr != m_pimpl)
			{
				CROISSANT_DELETE(m_pimpl);
				m_pimpl = nullptr;
			}
		}

	}
}
