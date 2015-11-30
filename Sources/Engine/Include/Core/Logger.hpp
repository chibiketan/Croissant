#pragma once
#ifndef LOGGER_H_
#define LOGGER_H_

#include "Engine.hpp"
#include "Core/Pimpl.hpp"
#include "Core/NonCopyable.hpp"
#include <memory>
#include "Core/String.hpp"

namespace Croissant
{
	namespace Core
	{
	    class ILoggerOutput;

		class ENGINE_API Logger : public Pimpl<Logger>::Detail,  private NonCopyable
		{
		public:
			Logger();
			~Logger();
			void WriteDebugLine(const String& caller, const String& line);
			void WriteWarnLine(const String& caller, const String& line);
			void WriteErrorLine(const String& caller, const String& line);
			void AddLoggerOutput(std::unique_ptr<ILoggerOutput>&& loggerOutput);

		};
	}
}

#endif
