#include "Exception/CroissantException.hpp"
#include "Debug/MemoryManager.hpp"

using string = std::string;

namespace Croissant
{
	namespace Exception
	{
		class CroissantException::Pimpl
		{
		public:
			Pimpl(std::string const& message) : m_message{ message } {}

			std::string const m_message;
		};

		CroissantException::CroissantException(string const& message)
			: m_pimpl{ CROISSANT_NEW Pimpl{ message } }
		{
		}

		CroissantException::~CroissantException()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		char const* CroissantException::what() const throw()
		{
			return m_pimpl->m_message.c_str();
		}
	}
}

