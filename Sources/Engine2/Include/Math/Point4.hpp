//
// Created by Gregory_Compte on 15/12/2015.
//

#ifndef CROISSANT_ENGINE_MATH_POINT4_HPP
#  define CROISSANT_ENGINE_MATH_POINT4_HPP
#  pragma once

#  include "Engine.hpp"
#  include <array>
#  

namespace Croissant
{
	namespace Math
	{
		class ENGINE_API Point4 final
		{
		public:
			using type = float;
			using type_ref = type&;
			using type_cref = type const&;
			Point4();
			Point4(type x, type y, type z, type w = type(1.0f));
			type_ref	X();
			type		X() const;
			type_ref	Y();
			type		Y() const;
			type_ref	Z();
			type		Z() const;
			type_ref	W();
			type		W() const;
			friend std::ostream& operator<<(std::ostream& out, Point4 const& point);

		private:
			std::array<type, 4>	m_elements;
		};

	}
}


#endif //CROISSANT_ENGINE_MATH_POINT4_HPP
