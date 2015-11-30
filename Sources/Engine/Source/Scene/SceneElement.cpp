#include "stdafx.hpp"
#include "Scene/SceneElement.hpp"

namespace Croissant
{
	namespace Scene
	{
		using Type = Croissant::Core::Type;

		const Type SceneElement::s_type = Type::Make<Node, Geometry, Spatial>("Croissant::Scene::SceneElement");

		SceneElement::SceneElement()
		{
		}

		SceneElement::~SceneElement()
		{
		}
	}
}
