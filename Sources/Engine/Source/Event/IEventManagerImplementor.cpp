#include "stdafx.hpp"
#include "Event/IEventManagerImplementor.hpp"

namespace Croissant
{
	namespace Event
	{
		const String TypeName = "EventManager";

		IEventManagerImplementor::~IEventManagerImplementor()
		{

		}

		const String& IEventManagerImplementor::GetTypeName()
		{
			return TypeName;
		}

	}
}
