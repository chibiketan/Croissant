/*
 * NotImplementedException.cpp
 *
 *  Created on: 12 juil. 2013
 *      Author: gcompte
 */

#include "Exception/NotImplementedException.hpp"

using Croissant::Core::String;
using Croissant::Core::EncodingType;

namespace Croissant
{
	namespace Exception
	{
		NotImplementedException::NotImplementedException()
			: m_function { "La fonction n'est pas implemente", EncodingType::ASCII }
		{

		}

		NotImplementedException::NotImplementedException(const String& function)
			: m_function { "La fonction suivante n'est pas implemente : ", EncodingType::ASCII }
		{
			// TODO Auto-generated constructor stub
			m_function += function;
		}

		NotImplementedException::~NotImplementedException()
		{
		}

		const char* NotImplementedException::what() const noexcept
		{
			return m_function.Str();
		}

	} /* namespace Exception */
} /* namespace Croissant */
