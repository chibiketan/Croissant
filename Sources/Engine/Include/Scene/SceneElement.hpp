#pragma once
#ifndef SCENEELEMENT_H_
#define SCENEELEMENT_H_

#include "Engine.hpp"
#include "Scene/Node.hpp"
#include "Scene/Geometry.hpp"
#include "Scene/Spatial.hpp"

namespace Croissant
{
	namespace Scene
	{
		class ENGINE_API SceneElement :
			public Croissant::Scene::Node,
			public Croissant::Scene::Geometry,
			public Croissant::Scene::Spatial
		{
			CROISSANT_TYPE_DECLARATION;
		public:
			SceneElement();
			virtual ~SceneElement();
		};
	}
}

#endif
