#include "stdafx.hpp"
#include "Core/CommandLineBoolElement.hpp"

namespace Croissant
{
	namespace Core
	{
		CommandLineBoolElement::~CommandLineBoolElement()
		{

		}

		CommandLineBoolElement::CommandLineBoolElement(const char* command)
			: CommandLineElement(command, CommandLineElementType::ArgumentTypeBool)
		{
		    m_val = false;
		}

		bool CommandLineBoolElement::Initialize(std::list<String*>& argv)
		{
			auto argvCurrent = argv.begin();
			auto argvEnd = argv.end();

			for (;argvCurrent != argvEnd; ++argvCurrent)
			{
				if ((*argvCurrent)->Compare(GetCommand()) == 0)
				{
					auto tmp = *argvCurrent;
					argv.remove(tmp);
					CROISSANT_DELETE(tmp);
					m_val = true;
					return true;
				}
			}

			m_val = false;
			return false;
		}
	}
}
