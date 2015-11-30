#pragma once
#ifndef IFRAMEPROVIDER_H_
#define IFRAMEPROVIDER_H_

#include "Engine.hpp"
#include "Core/Frame.hpp"
#include "Core/String.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API IFrameProvider
		{
		public:
			static const String& GetTypeName();

			virtual ~IFrameProvider() = default;
			virtual const Frame& CurrentFrame() const = 0;
		};
	}  // namespace Core
}  // namespace Croissant

#endif /* IFRAMEPROVIDER_H_ */
