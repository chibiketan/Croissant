#pragma once
#ifndef SPATIAL_H_
#define SPATIAL_H_

#include "Engine.hpp"
#include "Core/Object.hpp"

namespace Croissant
{
	namespace Scene
	{
		class ENGINE_API Spatial : public Croissant::Core::Object
		{
			CROISSANT_TYPE_DECLARATION;

		public:
			Spatial();
			virtual ~Spatial();

		};
	}
}

#endif
