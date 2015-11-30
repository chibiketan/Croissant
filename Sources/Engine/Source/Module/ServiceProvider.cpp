#include "stdafx.hpp"
#include "Core/DebugMemoryManager.hpp"
#include "Module/ServiceProvider.hpp"

namespace Croissant
{
	namespace Module
	{
		ServiceProvider::ServiceProvider()
			: m_creators(nullptr)
		{
			m_creators = CROISSANT_NEW ServiceProvider::CreatorListMap();
		}

		ServiceProvider::~ServiceProvider()
		{
			if (nullptr != m_creators)
			{
				CROISSANT_DELETE(m_creators);
				m_creators = nullptr;
			}
		}
	}
}
