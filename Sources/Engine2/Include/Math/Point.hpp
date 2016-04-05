#ifndef CROISSANT_ENGINE_MATH_POINT_HPP_INC
#  define CROISSANT_ENGINE_MATH_POINT_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include <type_traits>

namespace Croissant
{
	namespace Math
	{
		template <typename T, int size>
		class ENGINE_API Point
		{
		public:
			static_assert(size <2 || size > 4, "La valeur de size doit être comprise entre 2 et 4");
			using type = typename std::decay<T>::type; // supprime les references 
			using type_ref = T&;
			using type_cref = type const&;

			Point(type x, type y);
			Point(type x, type y, type z);
			Point(type x, type y, type z, type w);
			type X() const;
			type Y() const;
			type Z() const;
			type W() const;
			void X(type x);
			void Y(type y);
			void Z(type z);
			void W(type z);

		private:
			type m_element[size];
		};

		// ---------------------------------------------- implémentation
		template <typename T, int size>
		Point<T, size>::Point(type x, type y)
			: m_element { x, y }
		{
		}

		template <typename T, int size>
		Point<T, size>::Point(type x, type y, type z)
			: m_element{ x, y, z }
		{
		}

		template <typename T, int size>
		Point<T, size>::Point(type x, type y, type z, type w)
			: m_element{ x, y, z, w }
		{
		}

		// ---------------------------------------------- alias
		using Point2 = Point<float, 2>;
		using Point3 = Point<float, 2>;
		//using Point4 = Point<float, 2>;
	}
}

#endif // !CROISSANT_ENGINE_MATH_POINT_HPP_INC
