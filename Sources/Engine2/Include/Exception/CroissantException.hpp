/*
 * CroissantException.hpp
 *
 *  Created on: 9 sept. 2014
 *      Author: gcompte
 */

#ifndef CROISSANTEXCEPTION_HPP_INC
#define CROISSANTEXCEPTION_HPP_INC

#include "Engine.hpp"
#include <string>
#include <exception>

namespace Croissant
{
	namespace Exception
	{
		class ENGINE_API CroissantException
		{
		public:
			explicit CroissantException(std::string const& message);
			virtual ~CroissantException();

			char const* what() const noexcept;
		private:
			class Pimpl;

			Pimpl*	m_pimpl;
		};
	}
}


#endif /* CROISSANTEXCEPTION_HPP_INC */
