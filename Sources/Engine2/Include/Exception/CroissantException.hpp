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
		class ENGINE_API CroissantException : public std::exception
		{
		public:
			CroissantException(std::string const& message);

			char const* what() const throw();
		private:

			const std::string m_message;
		};
	}
}


#endif /* CROISSANTEXCEPTION_HPP_INC */
