#pragma once
#ifndef SERVICEPROVIDER_H_
#define SERVICEPROVIDER_H_

#include "Engine.hpp"
#include "Core/NonCopyable.hpp"
#include <functional>
#include <memory>
#include <map>
#include <assert.h>
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Module
	{
		class ENGINE_API ServiceProvider : public Croissant::Core::NonCopyable
		{
		public:
			ServiceProvider();
			virtual ~ServiceProvider();

			template<typename T> void Provide(std::function<void*()> creatorFunction)
			{
				auto typeName = T::GetTypeName();

				assert(nullptr == (*m_creators)[typeName]);
				(*m_creators)[typeName] = creatorFunction;
			}

			template<typename T> void Resolve(std::unique_ptr<T>& implementor)
			{
				auto typeName = T::GetTypeName();
				auto creatorFunction = (*m_creators)[typeName];

				assert(creatorFunction);
				implementor.reset(static_cast<T*>(creatorFunction()));
			}

		private:
			typedef std::map<const String, std::function<void*()>> CreatorListMap;

			CreatorListMap* m_creators;
		};
	}
}

#endif
