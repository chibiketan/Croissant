/*
 * CroissantException.cpp
 *
 *  Created on: 9 sept. 2014
 *      Author: gcompte
 */

#include "Exception/CroissantException.hpp"

using string = std::string;

namespace Croissant
{
	namespace Exception
	{
		CroissantException::CroissantException(string const& message)
			: m_message(message)
		{
		}

		char const* CroissantException::what() const throw()
		{
			return m_message.c_str();
		}
	}
}

