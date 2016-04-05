#ifndef CROISSANT_ENGINE_MATH_TUPLE_HPP_INC
#  define CROISSANT_ENGINE_MATH_TUPLE_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include <type_traits>

namespace Croissant
{
	namespace Math
	{
		template <typename T, int size>
		class ENGINE_API Tuple
		{
		public:
			static_assert(size >= 2 || size <= 4, "La valeur de size doit être comprise entre 2 et 4");
			using type = typename std::decay<T>::type; // supprime les references 
			using type_ref = T&;
			using type_cref = type const&;

			template<typename = typename std::enable_if<size == 2>::type> Tuple(type x, type y);
			template<typename = typename std::enable_if<size == 3>::type> Tuple(type x, type y, type z);
			template<typename = typename std::enable_if<size == 4>::type> Tuple(type x, type y, type z, type w);
			type X() const;
			type Y() const;
			template<typename = typename std::enable_if<size >= 3>::type> type Z() const;
			template<typename = typename std::enable_if<size >= 4>::type> type W() const;
			void X(type x);
			void Y(type y);
			template<typename = typename std::enable_if<size >= 3>::type> void Z(type z);
			template<typename = typename std::enable_if<size >= 4>::type> void W(type w);

		private:
			type m_element[size];
		};

		// ---------------------------------------------- implémentation
		template <typename T, int size>
		template<typename>
		Tuple<T, size>::Tuple(type x, type y)
			: m_element { x, y }
		{
		}

		template <typename T, int size>
		template<typename>
		Tuple<T, size>::Tuple(type x, type y, type z)
			: m_element{ x, y, z }
		{
		}

		template <typename T, int size>
		template<typename>
		Tuple<T, size>::Tuple(type x, type y, type z, type w)
			: m_element{ x, y, z, w }
		{
		}

		template <typename T, int size>
		typename Tuple<T, size>::type Tuple<T, size>::X() const
		{
			return m_element[0];
		}

		template <typename T, int size>
		typename Tuple<T, size>::type Tuple<T, size>::Y() const
		{
			return m_element[1];
		}

		template <typename T, int size>
		template <typename>
		typename Tuple<T, size>::type Tuple<T, size>::Z() const
		{
			return m_element[2];
		}

		template <typename T, int size>
		template <typename>
		typename Tuple<T, size>::type Tuple<T, size>::W() const
		{
			return m_element[3];
		}

		template <typename T, int size>
		void Tuple<T, size>::X(type x)
		{
			m_element[0] = x;
		}

		template <typename T, int size>
		void Tuple<T, size>::Y(type y)
		{
			m_element[1] = y;
		}

		template <typename T, int size>
		template <typename>
		void Tuple<T, size>::Z(type z)
		{
			m_element[2] = z;
		}

		template <typename T, int size>
		template <typename>
		void Tuple<T, size>::W(type w)
		{
			m_element[3] = w;
		}

		// ---------------------------------------------- alias
		//using Point2 = Tuple<float, 2>;
		using Point3 = Tuple<float, 2>;
		//using Point4 = Point<float, 2>;
	}
}

#endif // !CROISSANT_ENGINE_MATH_POINT_HPP_INC
