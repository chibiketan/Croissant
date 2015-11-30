#ifndef SERVICEPROVIDERINTERNAL_HPP_INC
#define SERVICEPROVIDERINTERNAL_HPP_INC

#include "Engine.hpp"

#include <functional>
#include <memory>
#include <map>
#include <assert.h>

#include <string>

namespace Croissant
{
	namespace Module
	{
		class ENGINE_API ServiceProviderInternal final
		{
		public:
			using CreatorDelegate = std::function<void*(const std::string&)>;

			ServiceProviderInternal(ServiceProviderInternal const&) = delete;
			ServiceProviderInternal& operator=(ServiceProviderInternal const&) = delete;
			ServiceProviderInternal(ServiceProviderInternal&&) = delete;
			ServiceProviderInternal& operator=(ServiceProviderInternal&&) = delete;

			ServiceProviderInternal();
			virtual ~ServiceProviderInternal();

			template<typename T> void Provide(CreatorDelegate creatorFunction)
			{
				auto typeName = T::GetTypeName();

				assert(nullptr == (*m_creators)[typeName]);
				(*m_creators)[typeName] = creatorFunction;
			}

			template<typename T> void Resolve(const std::string& moduleName, std::unique_ptr<T>& implementor)
			{
				UNUSED(moduleName);
				auto typeName = T::GetTypeName();
				auto creatorFunction = (*m_creators)[typeName];

				assert(creatorFunction);
				implementor.reset(static_cast<T*>(creatorFunction()));
			}

		private:
			using CreatorListMap = std::map<const std::string, CreatorDelegate>;

			CreatorListMap* m_creators;
		};
	}
}

#endif
