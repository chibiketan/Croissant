#include "Debug/MemoryManager.hpp"
#include "Module/ServiceProvider.hpp"

namespace Croissant
{
	namespace Module
	{
		ServiceProvider::ServiceProvider(ServiceProviderInternal& sp, const std::string& moduleName)
			: m_serviceProviderInternal(sp), m_moduleName { moduleName }
		{
		}

	}
}
