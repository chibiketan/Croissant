#include "stdafx.hpp"
#include "Event/Event.hpp"
#include <sstream>

namespace Croissant
{
	namespace Event
	{
		class Event::Pimpl
		{
		public:
			Pimpl(const String& name);
			Pimpl(const String&& name);
			~Pimpl();
			const String& Name();

		private:
			Pimpl& operator=(const Pimpl&) = delete;

			const String m_name;
		};

		Event::Pimpl::Pimpl( const String& name )
			: m_name(name)
		{

		}

		Event::Pimpl::Pimpl( const String&& name )
			: m_name(std::move(name))
		{
		}

		Event::Pimpl::~Pimpl()
		{

		}

		const String& Event::Pimpl::Name()
		{
			return m_name;
		}

		// --------------------------------------- Event::pimpl implémentation


		// --------------------------------------- Event implémentation
		Event::Event(const String&& type)
			: m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW Event::Pimpl(std::move(type));
		}

		Event::Event(const String& type)
			: m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW Event::Pimpl(type);
		}

		Event::~Event()
		{
			if (nullptr != m_pimpl)
			{
				CROISSANT_DELETE(m_pimpl);
				m_pimpl = nullptr;
			}
		}

		const String& Event::Name() const
		{
			return m_pimpl->Name();
		}

		String Event::ToString() const
		{
//			std::ostringstream result;
//
//			result << "{ " << m_pimpl->Name() << " }";
//			return result.str();
			return "{ " + m_pimpl->Name() + " }";
		}

	}
}
