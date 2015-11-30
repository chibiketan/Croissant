#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_

#include "Engine.hpp"
#include "Core/Type.hpp"
#include "Core/String.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API Object
		{
		public:
			Object();
			virtual ~Object();
			virtual String ToString() const;

			CROISSANT_TYPE_DECLARATION;
		};
	}
}

#endif
