//
// Created by Gregory_Compte on 15/12/2015.
//

#ifndef CROISSANT_ENGINE_MATH_POINT4_HPP
#  define CROISSANT_ENGINE_MATH_POINT4_HPP

#  include "Engine.hpp"
#  include <array>

namespace Croissant
{
	namespace Math
	{
		class ENGINE_API Point4
		{
		public:
			using type = float;
			using type_ref = type&;
			using type_cref = type const&;

		private:
			std::array<type, 4>	m_elements;
		};
	}
}

#endif //CROISSANT_ENGINE_MATH_POINT4_HPP
