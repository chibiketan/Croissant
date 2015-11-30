#pragma once

#ifndef LOGGEROUTPUTTOFILE_H_
#define LOGGEROUTPUTTOFILE_H_

#include "Core/ILoggerOutput.hpp"
#include "Core/NonCopyable.hpp"
#include "Core/Pimpl.hpp"
#include "Core/String.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API LoggerOutputToFile : public Pimpl<LoggerOutputToFile>::Detail, public ILoggerOutput, private NonCopyable
		{
		public:
			LoggerOutputToFile(const String& filename);
			virtual void Write(const String& data);
		};
	}
}

#endif
