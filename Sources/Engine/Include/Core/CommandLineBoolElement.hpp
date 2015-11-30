#ifndef COMMANDLINEBOOLELEMENT_H_
#define COMMANDLINEBOOLELEMENT_H_
#pragma once

#include "Core/CommandLineElement.hpp"

#include "Core/String.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API CommandLineBoolElement : public CommandLineElement
		{
		public:
			CommandLineBoolElement(const char* command);
			virtual ~CommandLineBoolElement();
			bool Initialize(std::list<String*>& argv);

        private:
            bool m_val;
		};
	}
}

#endif
