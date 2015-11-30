/*
 * CroissantException.hpp
 *
 *  Created on: 12 juil. 2013
 *      Author: gcompte
 */

#ifndef CROISSANTEXCEPTION_HPP_
#define CROISSANTEXCEPTION_HPP_

#include <exception>

namespace Croissant
{
	namespace Exception
	{

		class CroissantException : public std::exception
		{
		public:
			CroissantException();
			virtual ~CroissantException();
		};

	} /* namespace Exception */
} /* namespace Croissant */

#endif /* CROISSANTEXCEPTION_HPP_ */
