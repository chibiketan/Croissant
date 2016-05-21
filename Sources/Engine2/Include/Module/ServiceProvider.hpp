#ifndef SERVICEPROVIDER_HPP_INC
#define SERVICEPROVIDER_HPP_INC

#include "Engine.hpp"
#include "Debug/MemoryManager.hpp"
#include "Module/ServiceProviderInternal.hpp"

#include <functional>
#include <memory>
#include <map>
#include <assert.h>

namespace Croissant
{
	namespace Module
	{
		class ENGINE_API ServiceProvider final
		{
		public:
			ServiceProvider(ServiceProvider const&) = delete;
			ServiceProvider& operator=(ServiceProvider const&) = delete;
			ServiceProvider(ServiceProvider&&) = delete;
			ServiceProvider& operator=(ServiceProvider&&) = delete;

			ServiceProvider(ServiceProviderInternal& sp, const std::string& moduleName);
			~ServiceProvider();

			template<typename T> void Provide(ServiceProviderInternal::CreatorDelegate creatorFunction)
			{
				m_serviceProviderInternal.Provide<T>(creatorFunction);
			}

			template<typename T> void Resolve(std::unique_ptr<T>& implementor)
			{
				m_serviceProviderInternal.Resolve<T>(m_moduleName, implementor);
			}

		private:
			class Pimpl;

			Pimpl*	m_pimpl;
			ServiceProviderInternal& m_serviceProviderInternal;
		};
	}
}

#endif
