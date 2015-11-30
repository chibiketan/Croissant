#include "stdafx.hpp"
#include "Event/UserEvent.hpp"

namespace Croissant
{
	namespace Event
	{
		const String s_eventName = "Game::Event";

		class UserEvent::Pimpl
		{
		public:
			Pimpl(UserEvent::UserEventKind kind);
			~Pimpl();
			UserEvent::UserEventKind Kind() const;

		private:
			Pimpl& operator=(const Pimpl&) = delete;
			const UserEvent::UserEventKind m_kind;
		};

		// ------------------------------------ UserEvent::Pimpl implémentation

		UserEvent::Pimpl::Pimpl(UserEvent::UserEventKind kind)
			: m_kind(kind)
		{
		}

		UserEvent::Pimpl::~Pimpl()
		{
		}

		UserEvent::UserEventKind UserEvent::Pimpl::Kind() const
		{
			return m_kind;
		}

		// ------------------------------------ UserEvent implémentation


		UserEvent::UserEvent(UserEvent::UserEventKind kind)
			: Event(s_eventName), m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW UserEvent::Pimpl(kind);
		}

		UserEvent::~UserEvent()
		{
			if (nullptr != m_pimpl)
			{
				CROISSANT_DELETE(m_pimpl);
				m_pimpl = nullptr;
			}
		}

		UserEvent::UserEventKind UserEvent::Kind() const
		{
			return m_pimpl->Kind();
		}

	}
}
