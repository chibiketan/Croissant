/*
 * WindowEvent.cpp
 *
 *  Created on: 9 sept. 2014
 *      Author: Gregory_Compte
 */

#include "Graphic/WindowEvent.hpp"

namespace Croissant
{
	namespace Graphic
	{
		WindowEvent::WindowEvent(WindowEventType const& type)
			: m_type(type)
		{
		}

		WindowEvent::~WindowEvent()
		{
		}

		WindowEventType WindowEvent::GetType() const
		{
			return m_type;
		}
	}
}


