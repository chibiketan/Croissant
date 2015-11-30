#include "stdafx.hpp"
#include "Scene/Spatial.hpp"
#include "Core/Type.hpp"

#define TOTO(t) &t::s_type,

namespace Croissant
{
	namespace Scene
	{
		using Type = Croissant::Core::Type;

		const Type Spatial::s_type = Type::Make<Croissant::Core::Object>("Croissant::Scene::Spatial");

		Spatial::Spatial()
		{
		}


		Spatial::~Spatial()
		{
		}
	}
}
