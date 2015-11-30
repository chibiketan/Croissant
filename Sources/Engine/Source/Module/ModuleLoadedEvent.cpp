#include "stdafx.hpp"
#include "Module/ModuleLoadedEvent.hpp"
#include <sstream>

namespace Croissant
{
	namespace Module
	{
		// ------------------------------ ModuleLoadedEvent implémentation

		ModuleLoadedEvent::ModuleLoadedEvent(const String& moduleName, bool success)
			: Event { "Module::Loaded" }, m_moduleName { moduleName }, m_success { success }
		{

		}

		ModuleLoadedEvent::~ModuleLoadedEvent()
		{

		}

		const String ModuleLoadedEvent::ModuleName() const
		{
			return m_moduleName;
		}

		String ModuleLoadedEvent::ToString() const
		{
			std::ostringstream result;

			result << "{ " << Name() << ", Module name=" << m_moduleName << ", Result =" << (m_success ? "success" : "failure") << " }";
			return String(result.str().c_str());
		}

	}
}
