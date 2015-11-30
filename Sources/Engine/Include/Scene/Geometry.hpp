#pragma once
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "Engine.hpp"
#include "Core/Object.hpp"

namespace Croissant
{
	namespace Scene
	{
		class ENGINE_API Geometry : public Croissant::Core::Object
		{
			CROISSANT_TYPE_DECLARATION;

		public:
			Geometry();
			virtual ~Geometry();
		};
	}
}

#endif
