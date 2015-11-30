/*
 * WindowEvent.hpp
 *
 *  Created on: 9 sept. 2014
 *      Author: Gregory_Compte
 */

#ifndef WINDOWEVENT_HPP_INC
#define WINDOWEVENT_HPP_INC

#include "Engine.hpp"
#include "Graphic/WindowEventType.hpp"

namespace Croissant
{
	namespace Graphic
	{
		class ENGINE_API WindowEvent
		{
		public:
			virtual ~WindowEvent();
			WindowEventType GetType() const;

		protected:
			WindowEvent(const WindowEventType& type);

		private:
			const WindowEventType m_type;
		};
	}
}



#endif /* WINDOWEVENT_HPP_INC */
