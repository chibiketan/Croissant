#include "stdafx.hpp"
#include "Core/CommandLine.hpp"

namespace Croissant
{
	namespace Core
	{
	    template<>
		class Pimpl<CommandLine>::Implementation
		{
		public:
			Implementation(CommandLine* const ref, std::unique_ptr<CommandLineElement> elements[], std::size_t elementsSize, const char commmandPrefix);
			~Implementation(void);
			bool Initialize(size_t argc, const char** argv);
			const String& ProgramName(void) const;
			bool Bool(const char* commandName) const;

		private:
			void CleanArgv(void);

			std::unique_ptr<CommandLineElement>* m_elements;
			size_t m_elementsSize;
			char m_commandPrefix;
			String* m_programName;

			std::list<String*> m_argv;

			int32_t m_initialArgc;
			char** m_initialArgv;
            CommandLine* const m_ref;
		};

		Pimpl<CommandLine>::Implementation::~Implementation()
		{
			if (NULL != m_programName)
			{
				CROISSANT_DELETE(m_programName);
				m_programName = NULL;
			}

			CleanArgv();
			if (NULL != m_elements)
			{
				CROISSANT_DELETE_ARRAY(m_elements);
				m_elements = NULL;
			}
		}

		Pimpl<CommandLine>::Implementation::Implementation(CommandLine* const ref, std::unique_ptr<CommandLineElement> elements[], std::size_t elementsSize, const char commandPrefix)
			: m_elements(NULL), m_elementsSize(0), m_commandPrefix(commandPrefix), m_programName(nullptr), m_argv(), m_initialArgc(0), m_initialArgv(nullptr), m_ref(ref)
		{
			m_elements = CROISSANT_NEW std::unique_ptr<CommandLineElement>[elementsSize];
			if (0 != elementsSize)
			{
				for (std::size_t i = 0; i < elementsSize; ++i)
				{
					m_elements[i] = std::move(elements[i]);
				}
			}

			m_elementsSize = elementsSize;
		}

		bool Pimpl<CommandLine>::Implementation::Initialize(size_t argc, const char** argv)
		{
			if (NULL == argv || argc < 1)
			{
				return false;
			}

			assert(argv[0] != NULL);
			if (NULL != m_programName)
			{
				CROISSANT_DELETE(m_programName);
			}

			m_programName = CROISSANT_NEW String(argv[0]);
			if (NULL == m_programName)
			{
				return false;
			}

			size_t index;
			const char** tmpArgv = argv + 1;

			CleanArgv();
			for (index = 1; index < argc; ++index)
			{
				m_argv.push_back(CROISSANT_NEW String(*tmpArgv));
				++tmpArgv;
			}

			CommandLineElement* cle = m_elements->get();

			for (index = 0; index < m_elementsSize; ++index)
			{
				cle->Initialize(m_argv);
				++cle;
			}


			return true;
		}

		const String& Pimpl<CommandLine>::Implementation::ProgramName(void) const
		{
			static String defaultValue = String();
			if (NULL != m_programName)
			{
				return *m_programName;
			}

			return defaultValue;
		}

		bool Pimpl<CommandLine>::Implementation::Bool( const char* commandName ) const
		{
			CommandLineElement* cle = m_elements->get();
			size_t index;

			for (index = 0; index < m_elementsSize; ++index)
			{
				if (cle->GetCommand().Compare(commandName) == 0 && cle->getElementType() == CommandLineElementType::ArgumentTypeBool)
				{
					break;
				}

				++cle;
			}

			if (index == m_elementsSize)
			{
				return false;
			}

			bool tmp = *static_cast<bool*>(cle->GetData());
			return tmp;
		}

		void Pimpl<CommandLine>::Implementation::CleanArgv( void )
		{
			if (!m_argv.empty())
			{
				std::list<String*>::iterator begin, end;
				for (begin = m_argv.begin(), end = m_argv.end(); begin != end; ++begin)
				{
					CROISSANT_DELETE((*begin));
				}
			}
		}

		CommandLine::CommandLine(std::unique_ptr<CommandLineElement> elements[], size_t elementsSize)
			: CommandLine(elements, elementsSize, '-')
		{
		}

		CommandLine::CommandLine(std::unique_ptr<CommandLineElement> elements[], std::size_t elementsSize, char commandPrefix)
			: Pimpl<CommandLine>::Detail(elements, elementsSize, commandPrefix)
		{
		}

		bool CommandLine::Initialize(size_t argc, const char** argv)
		{
			Pimpl<CommandLine>::Implementation& imp = **this;

			return imp.Initialize(argc, argv);
		}

		const String& CommandLine::ProgramName(void) const
		{
			const Pimpl<CommandLine>::Implementation& imp = **this;

			return imp.ProgramName();
		}

		bool CommandLine::Bool( const char* commandName ) const
		{
			const Pimpl<CommandLine>::Implementation& imp = **this;

			return imp.Bool(commandName);
		}


	}
}
