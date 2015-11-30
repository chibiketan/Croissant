#pragma once
#ifndef IMODULE_H_
#define IMODULE_H_

#include "Engine.hpp"
#include <list>
#include <memory>
#include "Core/NonCopyable.hpp"
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Module
	{
		class ServiceProvider;

		class ENGINE_API IModule : private Croissant::Core::NonCopyable
		{
		public:
			virtual ~IModule();
			virtual bool Init(ServiceProvider& serviceProvider) = 0;
			virtual void Cleanup() = 0;
			virtual const String& Name() const = 0;

		protected:

		private:
		};

		typedef std::list<std::unique_ptr<IModule>> ModuleList;
		typedef ModuleList* (*CroissantModuleEntry)();
	}
}

#define CROISSANT_MODULE_ENTRY GetCroissantModule

#endif
