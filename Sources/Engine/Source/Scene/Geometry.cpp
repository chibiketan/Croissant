#include "stdafx.hpp"
#include "Scene/Geometry.hpp"

namespace Croissant
{
	namespace Scene
	{
		using Type = Croissant::Core::Type;

		const Type Geometry::s_type = Type::Make<Croissant::Core::Object>("Croissant::Scene::Geometry");

		Geometry::Geometry(void)
		{
		}


		Geometry::~Geometry(void)
		{
		}
	}
}
