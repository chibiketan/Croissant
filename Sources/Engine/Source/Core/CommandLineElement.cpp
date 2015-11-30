#include "stdafx.hpp"
#include "Core/CommandLineElement.hpp"

namespace Croissant
{
	namespace Core
	{
		CommandLineElement::CommandLineElement(const char* command, CommandLineElementType type)
			: m_command(NULL), m_type(type)
		{
			assert(NULL != command);
			assert(type != CommandLineElementType::ArgumentTypeNone);
			//m_command = new std::string(command);
			m_command = CROISSANT_NEW String(command);
		}

		CommandLineElement::~CommandLineElement()
		{
			if (NULL != m_command)
			{
				CROISSANT_DELETE(m_command);
				m_command = NULL;
			}
		}

		const String& CommandLineElement::GetCommand() const
		{
			return *m_command;
		}

		CommandLineElementType CommandLineElement::getElementType() const
		{
			return m_type;
		}

	}
}
