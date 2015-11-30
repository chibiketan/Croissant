/*
 * WindowEventClose.hpp
 *
 *  Created on: 9 sept. 2014
 *      Author: Gregory_Compte
 */

#ifndef WINDOWEVENTCLOSE_HPP_INC
#define WINDOWEVENTCLOSE_HPP_INC

#include "Engine.hpp"
#include "Graphic/WindowEvent.hpp"

namespace Croissant
{
	namespace Graphic
	{
		class ENGINE_API WindowEventClose : public WindowEvent
		{
		public:
			WindowEventClose();
		};
	}
}

#endif /* WINDOWEVENTCLOSE_HPP_INC */
