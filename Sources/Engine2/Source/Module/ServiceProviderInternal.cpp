#include "Debug/MemoryManager.hpp"
#include "Module/ServiceProvider.hpp"

namespace Croissant
{
	namespace Module
	{
		ServiceProviderInternal::ServiceProviderInternal()
			: m_creators(nullptr)
		{
			m_creators = CROISSANT_NEW ServiceProviderInternal::CreatorListMap();
		}

		ServiceProviderInternal::~ServiceProviderInternal()
		{
			if (nullptr != m_creators)
			{
				CROISSANT_DELETE(m_creators);
				m_creators = nullptr;
			}
		}
	}
}
