#include "stdafx.hpp"
#include "Core/Object.hpp"

namespace Croissant
{
	namespace Core
	{
		const Croissant::Core::Type Croissant::Core::Object::s_type = Croissant::Core::Type::Make("Croissant::Core::Oject");

		Object::Object()
		{
		}

		Object::~Object()
		{
		}

		String Object::ToString() const
		{
			return "{" + s_type.Name() + "}";
		}

	}
}
