#include "stdafx.hpp"
#include "Module/ModuleLoadingEvent.hpp"
#include <sstream>

namespace Croissant
{
	namespace Module
	{
		// ------------------------------ ModuleLoadingEvent impl�mentation

		ModuleLoadingEvent::ModuleLoadingEvent(const String& moduleName)
			: Event("Module::Loading"), m_moduleName(moduleName)
		{

		}

		ModuleLoadingEvent::~ModuleLoadingEvent()
		{

		}

		const String ModuleLoadingEvent::ModuleName() const
		{
			return m_moduleName;
		}

		String ModuleLoadingEvent::ToString() const
		{
			std::ostringstream result;

			result << "{ " << Name() << ", Module name=" << m_moduleName << " }";
			return String(result.str().c_str());
		}

	}
}
