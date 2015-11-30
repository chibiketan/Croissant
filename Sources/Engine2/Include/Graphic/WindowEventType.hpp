/*
 * WindowEventType.hpp
 *
 *  Created on: 9 sept. 2014
 *      Author: Gregory_Compte
 */

#ifndef WINDOWEVENTTYPE_HPP_INC
#define WINDOWEVENTTYPE_HPP_INC

namespace Croissant
{
	namespace Graphic
	{
		enum class WindowEventType
		{
			NONE,
			CLOSE,
			KEYDOWN,
			KEYUP,
			DRAW,
			MOUSEMOVE
		};
	}
}



#endif /* WINDOWEVENTTYPE_HPP_INC */
