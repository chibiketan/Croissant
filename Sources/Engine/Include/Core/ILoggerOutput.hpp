#pragma once

#ifndef ILOGGEROUTPUT_H_
#define ILOGGEROUTPUT_H_

#include "Engine.hpp"
#include "Core/String.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API ILoggerOutput
		{
		public:
			virtual void Write(const String& data) = 0;
			virtual ~ILoggerOutput() { };
		
		protected:
		};
	}
}

#endif
