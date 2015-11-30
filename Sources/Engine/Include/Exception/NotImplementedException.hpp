/*
 * NotImplementedException.hpp
 *
 *  Created on: 12 juil. 2013
 *      Author: gcompte
 */

#ifndef NOTIMPLEMENTEDEXCEPTION_HPP_
#define NOTIMPLEMENTEDEXCEPTION_HPP_

#include "Exception/CroissantException.hpp"
#include "Core/String.hpp"

namespace Croissant
{
	namespace Exception
	{

		class NotImplementedException :
		        public Croissant::Exception::CroissantException
		{
		public:
			NotImplementedException();
			NotImplementedException(const Croissant::Core::String& function);
			NotImplementedException(NotImplementedException&) = default;
			NotImplementedException(NotImplementedException&&) = default;
			virtual ~NotImplementedException();
			virtual const char* what() const noexcept;

		private:
			Croissant::Core::String m_function;
		};

	} /* namespace Exception */
} /* namespace Croissant */
#endif /* NOTIMPLEMENTEDEXCEPTION_HPP_ */
