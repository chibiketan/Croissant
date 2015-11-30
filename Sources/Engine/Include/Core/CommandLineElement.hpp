#ifndef COMMANDLINEELEMENT_H_
#define COMMANDLINEELEMENT_H_
#pragma once

#include "Engine.hpp"
#include <list>
#include "Core/String.hpp"

namespace Croissant
{
	namespace Core
	{
		enum class CommandLineElementType {
			ArgumentTypeNone,
			ArgumentTypeBool,
			ArgumentTypeInt,
			ArgumentTypeFloat,
			ArgumentTypeString
		};

		class ENGINE_API CommandLineElement
		{
		public:

			CommandLineElement(const char* command, CommandLineElementType type);
			virtual ~CommandLineElement();
			const String& GetCommand() const;
			CommandLineElementType getElementType() const;
			virtual void* GetData() const = 0;
			//************************************
			// Method:    Initialize
			// FullName:  Croissant::Core::CommandLineElement::Initialize
			// Access:    virtual public
			// Returns:   bool
			// Qualifier:
			// Parameter: std::list<std::string * > & argv
			// extract from argv needed data if any and remove them with
			//************************************
			virtual bool Initialize(std::list<String*>& argv) = 0;

		private:
			CommandLineElement(const CommandLineElement& source);
			String* m_command;
			CommandLineElementType m_type;
		};
	}
}

#endif
