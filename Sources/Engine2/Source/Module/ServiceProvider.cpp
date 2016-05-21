#include "Debug/MemoryManager.hpp"
#include "Module/ServiceProvider.hpp"

namespace Croissant
{
	namespace Module
	{
		class ServiceProvider::Pimpl
		{
		public:
			explicit Pimpl(std::string const& moduleName)
				: m_moduleName{ moduleName }
			{}

			std::string const m_moduleName;
		};

		ServiceProvider::ServiceProvider(ServiceProviderInternal& sp, const std::string& moduleName)
			: m_pimpl{ CROISSANT_NEW Pimpl{ moduleName } }, m_serviceProviderInternal(sp)
		{
		}

		ServiceProvider::~ServiceProvider()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}
	}
}
