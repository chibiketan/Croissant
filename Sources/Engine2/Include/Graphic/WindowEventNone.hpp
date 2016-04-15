/*
 * WindowEventNone.hpp
 *
 *  Created on: 9 sept. 2014
 *      Author: Gregory_Compte
 */

#ifndef WINDOWEVENTNONE_HPP_INC
#define WINDOWEVENTNONE_HPP_INC

#include "Engine.hpp"
#include "WindowEvent.hpp"

namespace Croissant
{
	namespace Graphic
	{
		class ENGINE_API WindowEventNone : public WindowEvent
		{
		public:
			WindowEventNone();
			virtual ~WindowEventNone() {};
		};
	}
}

#endif /* WINDOWEVENTNONE_HPP_INC */
